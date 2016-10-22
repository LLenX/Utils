#include <gtest/gtest.h>
#include "posix/Path.h"
#include "posix/path/PathImpl.h"

using xal::posix::Path;

#ifdef XAL_POSIX_PATH_TEST_PRIVATE

namespace xal {
namespace posix {

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

} // namespace posix
} // namespace xal

#endif

TEST(PathStaticTest, TestHome) {
    EXPECT_STREQ(Path::HomeDirectory().ToString().data(), "/home/xuemq12");
}

class PathTest : public ::testing::Test {
  protected:
    void SetUp() {
        path1 = Path::FromString("/hello/world");
        path2 = Path::FromString("hello/world");
        path3 = Path::FromString("../../hello/world");
    }

    Path path1, path2, path3;
};

TEST_F(PathTest, TestDefaultCtor) {
    Path path;
    std::string s = path.ToString();
    EXPECT_STREQ(s.c_str(), ".");
    EXPECT_FALSE(path.IsAbsolute());
}

TEST_F(PathTest, TestBasicStringCtor) {
    Path pathA("hello/world/");
    std::string s = pathA.ToString();
    EXPECT_STREQ(s.data(), "./hello/world");
    EXPECT_FALSE(pathA.IsAbsolute());

    Path pathB("/hello/world/");
    std::string s2 = pathB.ToString();
    EXPECT_STREQ(s2.data(), "/hello/world");
    EXPECT_TRUE(pathB.IsAbsolute());
}

TEST_F(PathTest, TestFromString) {
    EXPECT_STREQ(Path::FromString("/hello/world/../").ToString().data(),
                 "/hello");
    EXPECT_STREQ(Path::FromString("/hello/../world/../").ToString().data(),
                 "/");
    EXPECT_STREQ(Path::FromString("hello/../../").ToString().data(), "./..");
    EXPECT_STREQ(Path::FromString("hello/././../../world/..").ToString().data(),
                 "./..");
    EXPECT_STREQ(Path::FromString("../../hello/world/../").ToString().data(),
                 "./../../hello");

    EXPECT_THROW(Path::FromString("/hello/../.."), xal::posix::InvalidPath);
}

TEST_F(PathTest, TestCopy) {
    Path PathA("/hello"), PathB("world"), PathC(PathA), pathD;
    if (true) {
        Path inner_path("inner");
        pathD = inner_path;
    }
    EXPECT_STREQ(PathA.ToString().data(), "/hello");
    EXPECT_STREQ(PathB.ToString().data(), "./world");
    EXPECT_STREQ(PathC.ToString().data(), "/hello");
    EXPECT_STREQ(pathD.ToString().data(), "./inner");
}

TEST_F(PathTest, TestAppend) {
    Path relative_path(Path::FromString("relative")), absolute_path(
        Path::FromString("/absolute"));

    EXPECT_THROW(relative_path.Append(path1), xal::posix::WrongPathType);
    EXPECT_STREQ(relative_path.Append(path3).ToString().data(),
                 "./../hello/world");
    EXPECT_STREQ(relative_path.Append(path2).ToString().data(),
                 "./../hello/world/hello/world");
    EXPECT_THROW(absolute_path.Append(path3), xal::posix::InvalidPath);
    EXPECT_STREQ(absolute_path.Append(path2).ToString().data(),
                 "/absolute/hello/world");
}

TEST_F(PathTest, TestEquality) {
    Path pathA = Path::FromString("/hello/world"), pathB = Path::FromString(
        "hello/world"), pathC = Path::FromString("../../hello/world");
    EXPECT_NE(pathA, pathB);
    EXPECT_NE(pathA, pathC);
    EXPECT_NE(pathB, pathC);
    EXPECT_EQ(pathA, path1);
    EXPECT_EQ(pathB, path2);
    EXPECT_EQ(pathC, path3);
}

TEST_F(PathTest, TestSize) {
    EXPECT_EQ(path1.Size(), 2);
    EXPECT_EQ(path2.Size(), 2);
    EXPECT_EQ(path3.Size(), 4);
    EXPECT_EQ(Path().Size(), 0);
    EXPECT_EQ(Path("/").Size(), 0);
}

TEST_F(PathTest, TestSwap) {
    path1.Swap(path2);
    EXPECT_STREQ(path1.ToString().data(), "./hello/world");
    EXPECT_STREQ(path2.ToString().data(), "/hello/world");
    EXPECT_FALSE(path1.IsAbsolute());
    EXPECT_TRUE(path2.IsAbsolute());
}

TEST_F(PathTest, TestPrefix) {
    EXPECT_THROW(Path::CommonPrefix(path1, path2), xal::posix::WrongPathType);
    EXPECT_THROW(Path::CommonPrefix(path1, path3), xal::posix::WrongPathType);
    EXPECT_STREQ(Path::CommonPrefix(path2, path3).ToString().data(), ".");

    Path path4("/hello/path");
    EXPECT_STREQ(Path::CommonPrefix(path1, path4).ToString().data(), "/hello");
    EXPECT_STREQ(Path::CommonPrefix(path4, path1).ToString().data(), "/hello");

    Path path5("../../");
    EXPECT_STREQ(Path::CommonPrefix(path3, path5).ToString().data(), "./../..");
    EXPECT_STREQ(Path::CommonPrefix(path5, path3).ToString().data(), "./../..");

    Path path6("hello/world");
    EXPECT_EQ(Path::CommonPrefix(path2, path6), path2);
    EXPECT_EQ(Path::CommonPrefix(path6, path2), path2);
}

TEST_F(PathTest, TestToParent) {
    EXPECT_STREQ(path1.ToParentDirectory().ToString().data(), "/hello");
    EXPECT_STREQ(path1.ToParentDirectory().ToString().data(), "/");
    EXPECT_THROW(path1.ToParentDirectory(), xal::posix::InvalidPath);
    EXPECT_STREQ(path3.ToParentDirectory().ToString().data(), "./../../hello");
    EXPECT_STREQ(path3.ToParentDirectory().ToString().data(), "./../..");
    EXPECT_STREQ(path3.ToParentDirectory().ToString().data(), "./../../..");
}

TEST_F(PathTest, TestFileName) {
    EXPECT_STREQ(path1.FileName().data(), "world");
    EXPECT_STREQ(path2.FileName().data(), "world");
    EXPECT_STREQ(Path::FromString("").FileName().data(), "");
    EXPECT_STREQ(Path::FromString(".././..").FileName().data(), "");
    EXPECT_STREQ(Path::FromString("/").FileName().data(), "");
}
