#include "posix/Path.h"
#include <unistd.h>

namespace xal {

namespace posix {

Path Path::CurrentWorkingDirectory() {
    int64_t path_max_length(4096);
#if defined(PATH_MAX)
    path_max_length = PATH_MAX;
#elif defined(_PC_PATH_MAX)
    path_max_length = pathconf("/", _PC_PATH_MAX);
#endif
    // allocate memory
    char *path_buffer = new char[path_max_length];
    char *temp = getcwd(path_buffer, static_cast<size_t>(path_max_length));
    for (; not temp;) { // if getcwd fail, indicating insufficient path_max_length
        path_max_length <<= 1;
        delete[] path_buffer;
        path_buffer = new char[path_max_length]; // try again
        temp = getcwd(path_buffer, static_cast<size_t>(path_max_length));
    }
    Path cwd(path_buffer);
    delete[] path_buffer;
    return cwd;
}

Path Path::HomeDirectory() {
    return Path(getenv("HOME"));
}

} // namespace xal

} // namespace posix
