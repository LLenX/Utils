#include "xal/unix/path/PathState.h"
#include <regex>

namespace xal {

namespace unix {

Path::PathImpl::PathImpl()
    : path_state_(PathState::CreateState(false)) {}

Path::PathImpl::PathImpl(const Path::PathImpl &that)
    : path_token_seq_(that.path_token_seq_),
      path_state_(PathState::CreateState(that.IsAbsolute())) {}

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

bool Path::PathImpl::IsAbsolute() const {
    return path_state_->IsAbsolute();
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
    path_state_->PathAppendToken(path_token, path_token_seq_);
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

} // namespace unix

} // namespace xal