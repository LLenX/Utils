#ifndef XAL_UNIX_PATH_PATH_INTERFACE_H_
#define XAL_UNIX_PATH_PATH_INTERFACE_H_

#include "PathExeption.h"
#include <memory>
#include <stdexcept>
#include <string>

namespace xal {

namespace unix {

/**
 * Path support the unix-like path format
 */
class Path {
    // static function for utility use
    // provide basic functionality about the path in the general idea
  public:
    /**
     * get the path of the current working directory
     * @return the path represents the current working directory
     */
    static Path GetCwd();

  public: // methods
    /**
     * pimpl idiom
     */
    class PathImpl;

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
    explicit Path(const std::string &path_str) throw(InvalidPath);

    /**
     * copy constructor, get a copy of that path
     * @param that the path to be copied
     */
    Path(const Path &that);

    /**
     * move constructor
     * @param that the path to be moved
     */
    Path(Path &&that) = default;

    /**
     * copy assignment operator
     * @param rhs the path to be copied
     */
    Path &operator=(const Path &rhs);

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
     * construct a the path from a string represents the path, synonym to the
     * constructor
     * @param path_str the string represents the path
     */
    static Path FromString(const std::string &path_str) throw(InvalidPath);

    /**
     * append the tail_path to current path, require tail_path is a relative path
     * @param tail_path the path to append after this path
     * @return reference to this path
     */
    Path &Append(const Path &tail_path) throw(InvalidPath, WrongPathType);

    /**
     * check whether the path represents an absolute path
     * @return true if the path is an absolute path, false otherwise
     */
    bool IsAbsolute() const;

    /**
     * convert the relative path to an absolute path relative to the current
     * working directory, do nothing and return false if it's a absolute path
     * already
     * @return true if convert successfully, false if it's an absolute path already
     */
    bool ToAbsolute() throw(InvalidPath);

  private:
    /**
     * implementation of the class
     */
    std::unique_ptr<PathImpl> path_impl_;
};

} // namespace unix

} // namespace xal

#endif // XAL_UNIX_PATH_PATH_INTERFACE_H_
