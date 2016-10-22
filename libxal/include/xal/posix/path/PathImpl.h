#ifndef XAL_POSIX_PATH_PATHIMPL_H_
#define XAL_POSIX_PATH_PATHIMPL_H_

#include "posix/Path.h"
#include "PathExeption.h"
#include <string>
#include <vector>

#ifdef XAL_POSIX_PATH_TEST_PRIVATE

#include <gtest/gtest_prod.h>

#endif

namespace xal {
namespace posix {

/**
 * private members of the Path
 */
class Path::PathImpl {
  public:
    /**
     * the container of the sequence of path token
     */
    using TokenSeq = std::vector<std::string>;

    /**
     * initialize the path to be a relative path of the current working
     * directory
     */
    PathImpl();

    /**
     * copy constructor need to specified because of unique_ptr
     * @param that the PathImpl to be copied
     */
    PathImpl(const PathImpl &that);

    /**
     * default move constructor
     * @param that the PathImpl to be moved
     */
    PathImpl(PathImpl &&that) = default;

    /**
     * prevent compiler from generating the definition in place
     */
    ~PathImpl();

    /**
     * implementation of the constructor of Path that take a string
     * @param path_str the string that represents the path
     */
    PathImpl(const std::string path_str);

    /**
     * copy assignment
     * @param that the PathImpl to be copied
     * @return reference to this
     */
    PathImpl &operator=(const PathImpl &that);

    /**
     * default move assignment operator
     * @param that the PathImpl to be moved
     * @return reference to this
     */
    PathImpl &operator=(PathImpl &&that) = default;

    /**
     * set the token sequence to the common prefix of two paths
     * @param path1 first path
     * @param path2 second path
     */
    void SetCommonPrefix(const PathImpl &path1, const PathImpl &path2);

    /**
     * implementation of the equal comparison of two paths
     * @param rhs another PathImpl to compare
     * @return true if two paths are equal, false if not
     */
    bool operator==(const PathImpl &rhs) const;

    /**
     * proxy of the ToString for the path state
     * @return the string represents the path
     */
    std::string ToString() const;

    /**
     * implementation of append
     * @param tail_path_impl the implementation of the tail relative path
     */
    void Append(const PathImpl &tail_path_impl) throw(InvalidPath);

    /**
     * convert current path to it's parent directory
     */
    void ToParentDirectory() throw(InvalidPath);

    /**
     * determine whether the current path is an absolute path
     * @return true if the path is an absolute path, false if is relative
     */
    bool IsAbsolute() const;

    /**
     * implementation of IsParentOf
     * @return true if this path is a parent path of
     */
    bool IsParentOf(const PathImpl &path) const;

    /**
     * implementation of Path::Size()
     * @return the number of tokens in the token sequence
     */
    size_t Size() const;

    /**
     * implementation of FileName
     * @return the file name of the path leads to or an empty string
     */
    std::string FileName() const;

    /**
     * reset the state of the path
     * @param is_absolute true if set to absolute path state, false if relative
     */
    void SetState(bool is_absolute);

  private:
#ifdef XAL_POSIX_PATH_TEST_PRIVATE

    FRIEND_TEST(PathImplTest, TestTokenize);

#endif // ifdef NDEBUG

    /**
     * tokenize the tokens in the path string seperated by the slash
     * @param path_str the string represents a path
     * @return the sequence after the path string tokenized
     */
    static TokenSeq TokenizePathStr(const std::string &path_str);

    /**
     * represents the state of the path, relative or absolute
     */
    class PathState;

    /**
     * take care of the operation of a relative path
     */
    class RelativePathState;

    /**
     * take care of the operation of an absolute path
     */
    class AbsolutePathState;

    /**
     * sequence of tokens represents the whole path with is_absolute_
     */
    TokenSeq path_token_seq_;

    /**
     * used in `Path(const std::string&)`
     * set the path to relative or absolute
     * @param path_str the string represents the path
     */
    void SetPathStateFromString(const std::string &path_str);

    /**
     * used in `Path(const std::string&)`
     * build the path according to the string of the path
     * @param path_str the copy of the string represents the path
     */
    void BuildPathToken(const std::string &path_str) throw(InvalidPath);

    /**
     * append a token after the current token sequence
     * @param path_token one token of the path
     */
    void AppendToken(const std::string &path_token) throw(InvalidPath);

    /**
     * the state of the class
     */
    std::unique_ptr<PathState> path_state_;

};

} // namespace posix
} // namespace xal

#endif // XAL_POSIX_PATH_PATHIMPL_H_
