#ifndef XAL_UNIX_PATH_PATH_EXEPTION_H_
#define XAL_UNIX_PATH_PATH_EXEPTION_H_

#include <stdexcept>

namespace xal {

namespace unix {

/**
 * invalid path that may throwed in the construction of a path
 */
class InvalidPath : public std::invalid_argument {
  public:
    /**
     * routine constructor that specified the error message of the error
     * @param what the error message
     */
    InvalidPath(const std::string &what = "")
        : invalid_argument(what) {}
};

} // namespace unix

} // namespace xal

#endif //XAL_UNIX_PATH_PATH_EXEPTION_H_
