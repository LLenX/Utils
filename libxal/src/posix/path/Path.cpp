#include "posix/path/PathState.h"
#include <regex>

namespace xal {

namespace posix {

Path::Path()
    : path_impl_(std::make_unique<PathImpl>()) {}

Path::Path(const Path &that)
    : path_impl_(std::make_unique<PathImpl>(*that.path_impl_)) {}

Path::Path(Path &&that)
    : path_impl_(std::move(that.path_impl_)) {}

Path::Path(const std::string &path_str) throw(InvalidPath)
    : path_impl_(std::make_unique<PathImpl>(path_str)) {}

Path::~Path() {}

Path &Path::operator=(const Path &rhs) {
    *path_impl_ = *rhs.path_impl_;
    return *this;
}

Path &Path::operator=(Path &&rhs) {
    path_impl_ = std::move(rhs.path_impl_);
    return *this;
}

bool Path::operator==(const Path &rhs) const {
    return *path_impl_ == *rhs.path_impl_;
}

bool Path::operator!=(const Path &rhs) const {
    return not(*this == rhs);
}

std::string Path::ToString() const {
    return path_impl_->ToString();
}

Path Path::FromString(const std::string &path_str) throw(InvalidPath) {
    return Path(path_str);
}

Path &Path::Append(const Path &tail_path) throw(InvalidPath, WrongPathType) {
    if (tail_path.IsAbsolute()) {
        throw WrongPathType("try to append an absolute path");
    }
    path_impl_->Append(*tail_path.path_impl_);
    return *this;
}

bool Path::IsAbsolute() const {
    return path_impl_->IsAbsolute();
}

Path Path::ToAbsolute() throw(InvalidPath) {
    if (IsAbsolute()) {
        return *this;
    }
    Path new_path(GetCwd());
    new_path.Append(*this);
    return new_path;
}

} // namespace posix

} // namespace xal