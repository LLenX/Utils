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

bool Path::IsParentOf(const Path &path) const {
    if (path.IsAbsolute() not_eq IsAbsolute()) {
        return false;
    }
    return path_impl_->IsParentOf(*path.path_impl_);
}

Path Path::AbsolutePath() const throw(InvalidPath) {
    if (IsAbsolute()) {
        return *this;
    }
    Path new_path(CurrentWorkingDirectory());
    new_path.Append(*this);
    return new_path;
}

Path
Path::CommonPrefix(const Path &path1, const Path &path2) throw(WrongPathType) {
    if (path1.IsAbsolute() not_eq path2.IsAbsolute()) {
        throw WrongPathType("trying to get prefix of paths of different types");
    }
    Path result_path;
    result_path.path_impl_->SetState(path1.IsAbsolute());
    result_path.path_impl_->SetCommonPrefix(
        *path1.path_impl_, *path2.path_impl_);
    return result_path;
}

size_t Path::Size() const {
    return path_impl_->Size();
}

void Path::Swap(Path &rhs) {
    path_impl_.swap(rhs.path_impl_);
}

Path &Path::ToParentDirectory() throw(InvalidPath) {
    path_impl_->ToParentDirectory();
    return *this;
}

std::string Path::FileName() const {
    return path_impl_->FileName();
}

Path Path::ParentDirectory() throw(InvalidPath) {
    Path parent_path(*this);
    parent_path.ToParentDirectory();
    return parent_path;
}

std::ostream &operator<<(std::ostream &os, const Path &path) {
    os << path.ToString();
    return os;
}

} // namespace posix

} // namespace xal