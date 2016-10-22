#ifndef XAL_POSIX_PATH_PATH_STATE_H_
#define XAL_POSIX_PATH_PATH_STATE_H_

#include "PathImpl.h"
#include <memory>
#include <string>

namespace xal {
namespace posix {

class Path::PathImpl::PathState {
  public:
    /**
     * simple factory method to create the state
     * @param is_absolute whether to create an absolute path state
     * @return the pointer to the state newly created
     */
    static std::unique_ptr<PathState> CreateState(bool is_absolute);

    /**
     * convert the path tokens to the path
     * @param token_seq the sequence of the tokens of the path
     * @return the string represents the path
     */
    std::string PathToString(const TokenSeq &token_seq) const;

    /**
     * determine the whether it's a absolute state or a relative state
     * @return true if the state is an absolute path state, false if
     * relativeS
     */
    bool IsAbsolute() const;

    /**
     * deal with the situation that double dot '..' is the token to append
     * @param token_seq
     */
    virtual void
    DealWithDoubleDot(TokenSeq &token_seq) const throw(InvalidPath) = 0;

    /**
     * determine whether the first sequence represents a parent path of the
     * second sequence
     * @param parent_path the sequence that suppose to be parent
     * @param child_path the sequence that suppose to be child
     * @return true if the first path is the parent of the second, false if not
     */
    virtual bool IsParentOfLatter(
        const TokenSeq &parent_path, const TokenSeq &child_path) const = 0;

  protected:
    /**
     * initialize the state of the path of whether is an absolute state
     * @param is_absolute true if it's initialized by the absolute state,
     *                    false if is by relative state
     */
    PathState(bool is_absolute);

    /**
     * get the prefix string of the path
     * @return the prefix of the path
     */
    virtual std::string PathStart() const;

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
    AbsolutePathState()
        : PathState(true) {}

    virtual void
    DealWithDoubleDot(TokenSeq &token_seq) const throw(InvalidPath) override;

    virtual bool IsParentOfLatter(
        const TokenSeq &parent_path, const TokenSeq &child_path) const override;
};

class Path::PathImpl::RelativePathState : public PathImpl::PathState {
  public:
    /**
     * initialize the state to a relative state
     */
    RelativePathState()
        : PathState(false) {}

    virtual void
    DealWithDoubleDot(TokenSeq &token_seq) const throw(InvalidPath) override;

    virtual bool IsParentOfLatter(
        const TokenSeq &parent_path, const TokenSeq &child_path) const override;

  protected:

    /**
     * get the prefix of the relative path
     * @return the prefix of the relative path (".")
     */
    virtual std::string PathStart() const override;
};

} // namespace posix
} // namespace xal

#endif // XAL_POSIX_PATH_PATH_STATE_H_
