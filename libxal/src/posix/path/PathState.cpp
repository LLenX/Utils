#include "posix/path/PathState.h"
#include <sstream>

namespace xal {

namespace posix {

Path::PathImpl::PathState::PathState(bool is_absolute)
    : is_absolute_(is_absolute) {}

std::string
Path::PathImpl::PathState::PathToString(const TokenSeq &token_seq) const {
    return PathStart() + ConcatToken(token_seq);
}

std::string Path::PathImpl::PathState::PathStart() const {
    return "";
}

std::string
Path::PathImpl::PathState::ConcatToken(const TokenSeq &token_seq) const {
    if (token_seq.empty() and IsAbsolute()) {
        return "/";
    }
    std::ostringstream res_stream;
    for (const auto &token : token_seq) {
        res_stream << '/' << token;
    }
    return res_stream.str();
}

bool Path::PathImpl::PathState::IsAbsolute() const {
    return is_absolute_;
}

std::unique_ptr<Path::PathImpl::PathState>
Path::PathImpl::PathState::CreateState(bool is_absolute) {
    if (is_absolute) {
        return std::make_unique<AbsolutePathState>();
    }
    return std::make_unique<RelativePathState>();
}

std::string Path::PathImpl::RelativePathState::PathStart() const {
    return ".";
}

void Path::PathImpl::RelativePathState::DealWithDoubleDot(
    Path::PathImpl::TokenSeq &token_seq) const throw(InvalidPath) {
    if (token_seq.empty() or token_seq.back() == "..") {
        token_seq.push_back("..");
    } else {
        token_seq.pop_back();
    }
}

bool Path::PathImpl::RelativePathState::IsParentOfLatter(
    const Path::PathImpl::TokenSeq &parent_path,
    const Path::PathImpl::TokenSeq &child_path) const {
    if (parent_path.empty() or parent_path.back() == "..") {
        if (child_path.empty() or child_path.back() == "..") {
            return parent_path.size() > child_path.size();
        }
        return true;
    }
    if (child_path.back() == ".." or parent_path.size() >= child_path.size()) {
        return false;
    }
    return std::mismatch(
        parent_path.begin(), parent_path.end(), child_path.begin()).first ==
           parent_path.end();
}

void Path::PathImpl::AbsolutePathState::DealWithDoubleDot(
    Path::PathImpl::TokenSeq &token_seq) const throw(InvalidPath) {
    if (token_seq.empty()) {
        throw InvalidPath("redundant '..' in absolute path");
    } else {
        token_seq.pop_back();
    }
}

bool Path::PathImpl::AbsolutePathState::IsParentOfLatter(
    const Path::PathImpl::TokenSeq &parent_path,
    const Path::PathImpl::TokenSeq &child_path) const {
    if (parent_path.size() >= child_path.size()) {
        return false;
    }
    return std::mismatch(
        parent_path.begin(), parent_path.end(), child_path.begin()).first ==
           parent_path.end();
}
} // namespace posix

} // namespace xal