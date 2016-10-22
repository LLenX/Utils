#ifndef XAL_UNIX_PATH_PATH_INTERFACE_H_
#define XAL_UNIX_PATH_PATH_INTERFACE_H_

#include "path/PathExeption.h"
#include <memory>
#include <stdexcept>
#include <string>

namespace xal {

namespace posix {

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
    Path(Path &&that);

    /**
     * destructor preventing compiler from generating a default one in the
     * the header file
     */
    ~Path();

    /**
     * copy assignment operator
     * @param rhs the path to be copied
     */
    Path &operator=(const Path &rhs);

    /**
     * move assignment operator
     * @param rhs the path to be moved
     */
    Path &operator=(Path &&rhs);

    /**
     * determine the equality of two paths
     * note comparison between different type is always false
     * @param rhs another path to compare
     * @return true if two paths are the same, false if not
     */
    bool operator==(const Path &rhs) const;

    /**
     * determine the inequality of two paths
     * @param rhs another path to compare
     * @return true if two paths are different, false if they are the same
     */
    bool operator!=(const Path &rhs) const;

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
     * get the absolute path from of this path, that is a relative path relative
     * to the working directory would be returned, or return the absolute path
     * itself
     * @return the absolute path representation of this path
     */
    Path ToAbsolute() throw(InvalidPath);

  private:
    /**
     * implementation of the class
     */
    std::unique_ptr<PathImpl> path_impl_;
};

} // namespace posix

} // namespace xal

#endif // XAL_UNIX_PATH_PATH_INTERFACE_H_
