#ifndef XAL_UNIX_PATH_H_
#define XAL_UNIX_PATH_H_

#include <memory>
#include <string>

namespace xal {

namespace unix {

/**
 * Path support the unix-like path format
 */
class Path {
  public:
    /**
     * the path contains a relative path of the current working directory by
     * default
     */
    Path();

    /**
     * construct the path with a string represents either absolute or a
     *  relative path
     * @param path_str the string represent a path
     */
    Path(const std::string &path_str);

    /**
     * copy constructor
     * @param that the path to be copied
     */
    Path(const Path &that) = default;

    /**
     * move constructor
     * @param that the path to be moved
     */
    Path(Path &&that) = default;

    /**
     * copy assignment operator
     * @param rhs the path to be copied
     */
    Path &operator=(const Path &rhs) = default;

    /**
     * move assignment operator
     * @param rhs the path to be moved
     */
    Path &operator=(Path &&rhs) = default;

    /**
     * convert the path to the string representation
     * @return the string representation of the path
     */
    std::string ToString() const;

    /**
     * check whether the path represents an absolute path
     * @return true if the path is an absolute path, false otherwise
     */
    bool IsAbsolute() const;

  private:
    /**
     * pimpl idiom
     */
    struct PathImpl;

    /**
     * implementation of the class
     */
    std::unique_ptr<PathImpl> path_impl_;
};

} // namespace unix

} // namespace xal

#endif // XAL_UNIX_PATH_H_
