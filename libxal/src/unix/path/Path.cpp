#include "xal/unix/path/PathState.h"
#include <regex>

namespace xal {

namespace unix {

Path::Path()
    : path_impl_(new PathImpl) {}

Path::Path(const Path &that)
    : path_impl_(std::make_unique<PathImpl>(*that.path_impl_)) {}

Path::Path(const std::string &path_str) throw(InvalidPath)
    : Path() {
    path_impl_->SetPathStateFromString(path_str);
    path_impl_->BuildPathToken(path_str);
}

Path &Path::operator=(const Path &rhs) {
    *path_impl_ = *rhs.path_impl_;
    return *this;
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

bool Path::ToAbsolute() throw(InvalidPath) {
    if (IsAbsolute()) {
        return false;
    }
    Path new_path(GetCwd());
    new_path.Append(*this);
    *this = std::move(new_path);
    return true;
}


} // namespace unix

} // namespace xal