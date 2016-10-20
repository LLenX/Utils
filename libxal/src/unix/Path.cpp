#include "xal/unix/Path.h"
#include <regex>
#include <sstream>
#include <vector>

using xal::unix::Path;

/**
 * private members of the Path
 */
struct Path::PathImpl {
    /**
     * initialize the path to be a relative path of the current working
     * directory
     */
    PathImpl();

    /**
     * used in `Path(const std::string&)`
     * set the path to relative or absolute
     * @param path_str the string represents the path
     */
    void SetPathType(const std::string &path_str);

    /**
     * used in `Path(const std::string&)`
     * build the path according to the string of the path
     * @param path_str the copy of the string represents the path
     */
    void BuildPathToken(std::string path_str);

    /**
     * whether the path represents a absolute path
     */
    bool is_absolute_;

    /**
     * sequence of tokens represents the whole path with is_absolute_
     */
    std::vector<std::string> path_token_seq_;
};

Path::Path() : path_impl_(std::make_unique<PathImpl>()) {}

Path::PathImpl::PathImpl() : is_absolute_(false) {}

Path::Path(const std::string &path_str) : Path() {
    path_impl_->SetPathType(path_str);
    path_impl_->BuildPathToken(path_str);
}

void Path::PathImpl::SetPathType(const std::string &path_str) {
    is_absolute_ = not path_str.empty() and path_str[0] == '/';
}

void Path::PathImpl::BuildPathToken(std::string path_str) {
    std::string token_pattern = "([^/]+)/*";
    std::regex token_rgx(token_pattern);
    std::regex_iterator<std::string::iterator> rgx_iter(
        begin(path_str), end(path_str), token_rgx),
        rgx_iter_end;

    const std::string kCurrentDir("."), kParentDir("..");
    for (; rgx_iter != rgx_iter_end;) {
        const std::string &token = (*rgx_iter)[1];
        if (token == kParentDir) {
            if (not path_token_seq_.empty()) {
                path_token_seq_.pop_back();
            }
            // TODO throw error
        } else if (token != kCurrentDir) {
            path_token_seq_.push_back(token);
        } // ignore the current path
    }
}

std::string Path::ToString() const {
    std::ostringstream res_stream;
    if (IsAbsolute()) {
        res_stream << '/'; // absolute path begin with a slash
    } else {
        res_stream << "./"; // relative path begin with ./
    }
    auto &path_token_seq = path_impl_->path_token_seq_;
    for (auto token_iter = begin(path_token_seq);
         token_iter != end(path_token_seq);) {
        res_stream << *token_iter++;
        if (token_iter != end(path_token_seq)) {
            res_stream << '/'; // a slash seperate every token
        }
    }
    return res_stream.str();
}

Path Path::FromString(const std::string &path_str) {
    return Path(path_str);
}

bool Path::IsAbsolute() const {
    return path_impl_->is_absolute_;
}
