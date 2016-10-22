#include "posix/path/PathState.h"
#include <cassert>
#include <regex>

namespace xal {
namespace posix {

Path::PathImpl::PathImpl()
    : path_state_(PathState::CreateState(false)) {}

Path::PathImpl::PathImpl(const Path::PathImpl &that)
    : path_token_seq_(that.path_token_seq_),
      path_state_(PathState::CreateState(that.IsAbsolute())) {}

Path::PathImpl::PathImpl(const std::string path_str) {
    SetPathStateFromString(path_str);
    BuildPathToken(path_str);
}

Path::PathImpl::~PathImpl() {}

Path::PathImpl &Path::PathImpl::operator=(const Path::PathImpl &that) {
    path_token_seq_ = that.path_token_seq_;
    SetState(that.IsAbsolute());
    return *this;
}

void Path::PathImpl::SetPathStateFromString(const std::string &path_str) {
    bool is_absolute = not path_str.empty() and path_str[0] == '/';
    path_state_ = PathState::CreateState(is_absolute);
}

std::string Path::PathImpl::ToString() const {
    return path_state_->PathToString(path_token_seq_);
}

void
Path::PathImpl::BuildPathToken(const std::string &path_str) throw(InvalidPath) {
    TokenSeq token_sequence(TokenizePathStr(path_str));
    for (const auto &token : token_sequence) {
        AppendToken(token);
    }
}

Path::PathImpl::TokenSeq
Path::PathImpl::TokenizePathStr(const std::string &path_str) {
    TokenSeq token_seq;

    // match every token seperated by slash '/'
    std::string token_pattern = "([^/]+)/*";
    std::regex token_rgx(token_pattern);
    std::regex_iterator<decltype(begin(path_str))> rgx_iter(
        begin(path_str), end(path_str), token_rgx), rgx_iter_end;

    for (; rgx_iter != rgx_iter_end;) {
        token_seq.push_back((*rgx_iter)[1]);
        ++rgx_iter;
    }
    return token_seq;
}

void
Path::PathImpl::AppendToken(const std::string &path_token) throw(InvalidPath) {
    if (path_token == "..") {
        path_state_->DealWithDoubleDot(path_token_seq_);
    } else if (path_token != ".") {
        path_token_seq_.push_back(path_token);
    }
}


bool Path::PathImpl::operator==(const Path::PathImpl &rhs) const {
    if (IsAbsolute() != rhs.IsAbsolute()) {
        // comparison of path with different type is non-sense
        // if it's really needed to compare a relative path to a absolute path
        // use AbsolutePath first
        return false;
    }
    return path_token_seq_ == rhs.path_token_seq_;
}

void Path::PathImpl::SetState(bool is_absolute) {
    path_state_ = PathState::CreateState(is_absolute);
}

void Path::PathImpl::Append(
    const Path::PathImpl &tail_path_impl) throw(InvalidPath) {
    PathImpl temp(*this);
    for (const auto &path_token : tail_path_impl.path_token_seq_) {
        temp.AppendToken(path_token);
    }
    *this = std::move(temp);
}

void Path::PathImpl::ToParentDirectory() throw(InvalidPath) {
    path_state_->DealWithDoubleDot(path_token_seq_);
}

bool Path::PathImpl::IsAbsolute() const {
    return path_state_->IsAbsolute();
}

bool Path::PathImpl::IsParentOf(const Path::PathImpl &path) const {
    assert(IsAbsolute() == path.IsAbsolute());
    return path_state_->IsParentOfLatter(path_token_seq_, path.path_token_seq_);
}

void
Path::PathImpl::SetCommonPrefix(const PathImpl &path1, const PathImpl &path2) {
    bool path1_is_shorter = path1.Size() < path2.Size();
    const auto &path1_ref = path1_is_shorter ? path1 : path2;
    const auto &path2_ref = not path1_is_shorter ? path1 : path2;

    const auto &token_seq1(path1_ref.path_token_seq_), token_seq2(
        path2_ref.path_token_seq_);

    auto prefix_tail_iter = std::mismatch(
        token_seq1.begin(), token_seq1.end(), token_seq2.begin()).first;
    path_token_seq_.assign(
        token_seq1.begin(), prefix_tail_iter);
}

size_t Path::PathImpl::Size() const {
    return path_token_seq_.size();
}

std::string Path::PathImpl::FileName() const {
    if (path_token_seq_.empty() or path_token_seq_.back() == "..") {
        return {};
    }
    return path_token_seq_.back();
}

} // namespace posix
} // namespace xal