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
     * the container of the sequence of path token
     */
    using TokenSeq = std::vector<std::string>;

    /**
     * initialize the path to be a relative path of the current working
     * directory
     */
    PathImpl();

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
    void SetPathState(const std::string &path_str);

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
    TokenSeq TokenizePathStr(const std::string &path_str);

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
     * sequence of tokens represents the whole path with is_absolute_
     */
    TokenSeq path_token_seq_;

    /**
     * the state of the class
     */
    std::unique_ptr<PathState> path_state_;

};

} // namespace unix

} // namespace xal

#endif // XAL_UNIX_PATH_PATHIMPL_H_
