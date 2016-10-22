#ifndef XAL_UNIX_PATH_PATH_EXEPTION_H_
#define XAL_UNIX_PATH_PATH_EXEPTION_H_

#include <stdexcept>

namespace xal {

namespace posix {

/**
 * throwed when encounter logically impossible path like try to access the
 * parent directory of the root directory
 */
class InvalidPath : public std::invalid_argument {
  public:
    /**
     * routine constructor that specified the error message
     * @param what the error message
     */
    InvalidPath(const std::string &what = "")
        : invalid_argument(what) {}
};

/**
 * throwed when try to use the operation that the current path type doesn't support
 */
class WrongPathType : public std::invalid_argument {
  public:
    /**
     * routine constructor that specified error message
     * @param what the error message
     */
    WrongPathType(const std::string &what = "")
        : invalid_argument(what) {}
};

} // namespace posix

} // namespace xal

#endif //XAL_UNIX_PATH_PATH_EXEPTION_H_
