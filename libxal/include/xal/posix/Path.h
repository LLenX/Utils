#ifndef XAL_POSIX_PATH_H_
#define XAL_POSIX_PATH_H_

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
    static Path CurrentWorkingDirectory();

    /**
     * get the path of the home directory of current user
     * @return the path represents the home directory of the user
     */
    static Path HomeDirectory();

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
     * construct a the path from a string represents the path, synonym to the
     * constructor
     * @param path_str the string represents the path
     */
    static Path FromString(const std::string &path_str) throw(InvalidPath);

    /**
     * get the common prefix of two paths
     * requiring that two paths are of the same type, WrongPathType would be
     * throwed otherwise
     * @param path1 first path
     * @param path2 second path
     * @return the common prefix of two paths
     */
    static Path
    CommonPrefix(const Path &path1, const Path &path2) throw(WrongPathType);

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
     * swap two paths
     * @param rhs another path to be swapped
     */
    void Swap(Path &rhs);

    /**
     * append the tail_path to current path, require tail_path is a relative path
     * @param tail_path the path to append after this path
     * @return reference to this path
     */
    Path &Append(const Path &tail_path) throw(InvalidPath, WrongPathType);

    /**
     * get the parent directory of the current path, if current directory is
     * root, InvalidPath would be throwed
     * @return reference to this path after it becomes it's parent
     */
    Path &ToParentDirectory() throw(InvalidPath);

    /**
     * check whether the path represents an absolute path
     * @return true if the path is an absolute path, false otherwise
     */
    bool IsAbsolute() const;

    /**
     * determine whether the path is a child of this path, not necessary to be
     * a direct child of this. two paths with different types always return
     * false
     * @param path the path to check
     * @return true if path is a child of this path
     */
    bool IsParentOf(const Path &path) const;

    /**
     * the number of directories or files consist of the path, not including
     * the current directory for relative path, root for absolute path
     * @return the number of subdirectory or file that consist of the path
     */
    size_t Size() const;

    /**
     * get the absolute path from of this path, that is a relative path relative
     * to the working directory would be returned, or return the absolute path
     * itself
     * @return the absolute path representation of this path
     */
    Path AbsolutePath() const throw(InvalidPath);

    /**
     * convenient function to get the parent directory path of the current path
     * @return the path to the parent directory path of the current path
     */
    Path ParentDirectory() throw(InvalidPath);

    /**
     * get the logical file name this path leads to, empty string if encounter
     * 1. current directory 2. root 3. parent directory
     * @return the file name of the path leads to
     */
    std::string FileName() const;

  private:
    /**
     * implementation of the class
     */
    std::unique_ptr<PathImpl> path_impl_;
};

} // namespace posix
} // namespace xal

#endif // XAL_POSIX_PATH_H_
