#include <gtest/gtest.h>
#include "xal/unix/Path.h"

using xal::unix::Path;

TEST(PathTest, TestDefaultCtor) {
    Path path;
    std::string s = path.ToString();
    EXPECT_STREQ(s.c_str(), ".");
    EXPECT_FALSE(path.IsAbsolute());
}

TEST(PathTest, TestBasicStringCtor) {
    Path path1("hello/world");
    std::string s = path1.ToString();
    EXPECT_STREQ(s.data(), "./hello/world");
    EXPECT_FALSE(path1.IsAbsolute());

    Path path2("/hello/world");
    std::string s2 = path2.ToString();
    EXPECT_STREQ(s2.data(), "/hello/world");
    EXPECT_TRUE(path2.IsAbsolute());
}
