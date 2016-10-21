#include "xal/unix/path/PathState.h"
#include <regex>

namespace xal {

namespace unix {

Path::Path()
    : path_impl_(new PathImpl) {}

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

bool Path::IsAbsolute() const {
    return path_impl_->IsAbsolute();
}

Path::Path(const Path &that)
    : path_impl_(std::make_unique<PathImpl>(*that.path_impl_)) {}

} // namespace unix

} // namespace xal