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

void Path::PathImpl::PathState::PathAppendToken(
    const std::string &token, Path::PathImpl::TokenSeq &token_seq) const {
    if (token == "..") {
        DealWithDoubleDot(token_seq);
    } else if (token != ".") {
        token_seq.push_back(token);
    }
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

void Path::PathImpl::AbsolutePathState::DealWithDoubleDot(
    Path::PathImpl::TokenSeq &token_seq) const throw(InvalidPath) {
    if (token_seq.empty()) {
        throw InvalidPath("redundant '..' in absolute path");
    } else {
        token_seq.pop_back();
    }
}
} // namespace posix

} // namespace xal