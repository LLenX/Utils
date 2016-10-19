#include "xal/unix/Path.h"
#include <vector>

using xal::unix::Path;
using std::string;

/**
 * private members of the Path
 */
struct Path::PathImpl {
    /**
     * token of every part of a path seperated by slash
     */
    using PathToken = string;

    /**
     * sequence of tokens represents the whole path
     */
    using TokenSeq = std::vector<PathToken>;

    /**
     * whether the path represents a absolute path
     */
    bool is_absolute_;

    /**
     * sequence of tokens represents the whole path with is_absolute_
     */
    TokenSeq path_token_seq_;
};

bool Path::IsAbsolute() const {
    return path_impl_->is_absolute_;
}
