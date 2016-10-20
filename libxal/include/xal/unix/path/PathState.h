#ifndef XAL_UNIX_PATH_PATH_STATE_H_
#define XAL_UNIX_PATH_PATH_STATE_H_

#include "PathImpl.h"
#include <memory>
#include <string>

namespace xal {

namespace unix {

class Path::PathImpl::PathState {
  public:
    /**
     * convert the path tokens to the path
     * @param token_seq the sequence of the tokens of the path
     * @return the string represents the path
     */
    std::string PathToString(const TokenSeq &token_seq) const;

    /**
     * append the token to the token sequence with the custom style of the path
     * @param token the path token to append
     * @param token_seq the sequence of the tokens represents a path
     */
    void PathAppendToken(const std::string &token, TokenSeq &token_seq) const;

    /**
     * determine the whether it's a absolute state or a relative state
          * @return true if the state is an absolute path state, false if
     * relativeS
     */
    bool IsAbsolute() const;

    /**
     * simple factory method to create the state
     * @param is_absolute whether to create an absolute path state
     * @return the pointer to the state newly created
     */
    static std::unique_ptr<PathState> CreateState(bool is_absolute);

  protected:
    /**
     * initialize the state of the path of whether is an absolute state
     * @param is_absolute true if it's initialized by the absolute state,
     *                    false if is by relative state
     */
    PathState(bool is_absolute);

    /**
     * deal with the situation that double dot '..' is the token to append
     * @param token_seq
     */
    virtual void DealWithDoubleDot(TokenSeq &token_seq) const
        throw(InvalidPath) = 0;

    /**
     * get the prefix string of the path
     * @return the prefix of the path
     */
    virtual std::string PathPrefix() const;

  private:
    /**
     * concatenate the token into a part of the path
     * @param token_seq the sequence of the path token
     * @return the part of the path constructed by the sequence
     */
    std::string ConcatToken(const TokenSeq &token_seq) const;

    /**
     * determine the the state of the path
     */
    bool is_absolute_;
};

class Path::PathImpl::AbsolutePathState : public PathImpl::PathState {
  public:
    /**
     * initialize the state to a absolute state
     */
    AbsolutePathState() : PathState(true) {}

    /**
     * deal with the situation when double dot appear in an absolute path
     * @param token_seq the sequence of the token represents a absolute path
     */
    virtual void DealWithDoubleDot(TokenSeq &token_seq) const
        throw(InvalidPath) override;
};

class Path::PathImpl::RelativePathState : public PathImpl::PathState {
  public:
    /**
     * initialize the state to a relative state
     */
    RelativePathState() : PathState(false) {}

  protected:
    /**
     * deal with the situation when double dot appear in a relative path
     * @param token_seq the sequence of the path represents a relative path
     */
    virtual void DealWithDoubleDot(TokenSeq &token_seq) const
        throw(InvalidPath) override;

    /**
     * get the prefix of the relative path
     * @return the prefix of the relative path (".")
     */
    virtual std::string PathPrefix() const override;
};

} // namespace unix

} // namespace xal

#endif // XAL_UNIX_PATH_PATH_STATE_H_
