#include <gtest/gtest.h>
#include "xal/unix/Path.h"

using xal::unix::Path;

TEST(PathImplTest, TestSetState) {
    Path::PathImpl path_impl;
    path_impl.SetPathStateFromString(".");
    EXPECT_FALSE(path_impl.IsAbsolute());
    path_impl.SetPathStateFromString("/");
    EXPECT_TRUE(path_impl.IsAbsolute());
    path_impl.SetPathStateFromString("/hello/world/");
    EXPECT_TRUE(path_impl.IsAbsolute());
    path_impl.SetPathStateFromString("hello/world/");
    EXPECT_FALSE(path_impl.IsAbsolute());
}

TEST(PathImplTest, TestTokenize) {
    Path::PathImpl::TokenSeq hello_world_sequence{"hello", "world"};
    EXPECT_EQ(Path::PathImpl::TokenizePathStr("hello/world"),
              hello_world_sequence);
    EXPECT_EQ(Path::PathImpl::TokenizePathStr("/hello/world"),
              hello_world_sequence);
    EXPECT_EQ(Path::PathImpl::TokenizePathStr("hello/world/"),
              hello_world_sequence);
    EXPECT_EQ(Path::PathImpl::TokenizePathStr("/hello/world"),
              hello_world_sequence);
    EXPECT_EQ(Path::PathImpl::TokenizePathStr("/"), Path::PathImpl::TokenSeq());

    Path::PathImpl::TokenSeq hello_sequence{"hello"};
    EXPECT_EQ(Path::PathImpl::TokenizePathStr("hello"), hello_sequence);
    EXPECT_EQ(Path::PathImpl::TokenizePathStr("hello/"), hello_sequence);
    EXPECT_EQ(Path::PathImpl::TokenizePathStr("/hello"), hello_sequence);

    EXPECT_EQ(
        Path::PathImpl::TokenizePathStr("////hello///world//a/long/path/"),
        Path::PathImpl::TokenSeq(
            { "hello", "world", "a", "long", "path" }));
}

TEST(PathTest, TestDefaultCtor) {
    Path path;
    std::string s = path.ToString();
    EXPECT_STREQ(s.c_str(), ".");
    EXPECT_FALSE(path.IsAbsolute());
}

TEST(PathTest, TestBasicStringCtor) {
    Path path1("hello/world/");
    std::string s = path1.ToString();
    EXPECT_STREQ(s.data(), "./hello/world");
    EXPECT_FALSE(path1.IsAbsolute());

    Path path2("/hello/world/");
    std::string s2 = path2.ToString();
    EXPECT_STREQ(s2.data(), "/hello/world");
    EXPECT_TRUE(path2.IsAbsolute());
}

TEST(PathTest, TestFromString) {
    EXPECT_STREQ(Path::FromString("/hello/world/../").ToString().data(),
                 "/hello");
    EXPECT_STREQ(Path::FromString("/hello/../world/../").ToString().data(),
                 "/");
    EXPECT_STREQ(Path::FromString("hello/../../").ToString().data(), "./..");
    EXPECT_STREQ(Path::FromString("hello/././../../world/..").ToString().data(),
                 "./..");
    EXPECT_STREQ(Path::FromString("../../hello/world/../").ToString().data(),
                 "./../../hello");

    EXPECT_THROW(Path::FromString("/hello/../.."), xal::unix::InvalidPath);
}

TEST(PathTest, TestCopy) {
    Path path1("/hello"), path2("world"), path3(path1), path4;
    if (true) {
        Path inner_path("inner");
        path4 = inner_path;
    }
    EXPECT_STREQ(path1.ToString().data(), "/hello");
    EXPECT_STREQ(path2.ToString().data(), "./world");
    EXPECT_STREQ(path3.ToString().data(), "/hello");
    EXPECT_STREQ(path4.ToString().data(), "./inner");
}