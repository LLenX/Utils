#ifndef XAL_UNIX_PATH_PATHIMPL_H_
#define XAL_UNIX_PATH_PATHIMPL_H_

#include "PathInterface.h"
#include "PathExeption.h"
#include <string>
#include <vector>

namespace xal {

namespace unix {

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
     * proxy of the ToString for the path state
     * @return the string represents the path
     */
    std::string ToString() const;

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
     * tokenize the tokens in the path string seperated by the slash
     * @param path_str the string represents a path
     * @return the sequence after the path string tokenized
     */
    static TokenSeq TokenizePathStr(const std::string &path_str);

    /**
     * append a token after the current token sequence
     * @param path_token one token of the path
     */
    void AppendToken(const std::string &path_token) throw(InvalidPath);

    /**
     * determine whether the current path is an absolute path
     * @return true if the path is an absolute path, false if is relative
     */
    bool IsAbsolute() const;

  private:
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
     * reset the state of the path
     * @param is_absolute true if set to absolute path state, false if relative
     */
    void SetState(bool is_absolute);

    /**
     * the state of the class
     */
    std::unique_ptr<PathState> path_state_;

};

} // namespace unix

} // namespace xal

#endif // XAL_UNIX_PATH_PATHIMPL_H_
