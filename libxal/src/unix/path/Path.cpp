#include "xal/unix/path/PathState.h"
#include "xal/unix/path/PathImpl.h"
#include "xal/unix/Path.h"
#include <regex>

namespace xal {

namespace unix {

Path::Path() : path_impl_(new PathImpl) {}

Path::Path(const std::string &path_str) throw(InvalidPath) : Path() {
    path_impl_->SetPathState(path_str);
    path_impl_->BuildPathToken(path_str);
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

} // namespace unix

} // namespace xal