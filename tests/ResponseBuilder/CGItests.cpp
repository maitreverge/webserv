#include "ResponseBuilderTest.hpp"

TEST_F(ResponseBuilderTest, TestPHPURI) {
    responseBuilder->_realURI = "dir1/dir2/script.php/dindon/ducon/mescouilles";
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "/dindon/ducon/mescouilles");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.php");
}

TEST_F(ResponseBuilderTest, TestPythonURI) {
    responseBuilder->_realURI = "dir1/dir2/script.py/dindon/ducon/mescouilles";
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "/dindon/ducon/mescouilles");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.py");
}

TEST_F(ResponseBuilderTest, TestNoCGIURI) {
    responseBuilder->_realURI = "dir1/dir2/script.txt/dindon/ducon/mescouilles";
    responseBuilder->checkCGI();
    EXPECT_FALSE(responseBuilder->_isCGI);
}

TEST_F(ResponseBuilderTest, TestBothPHPAndPythonURI) {
    responseBuilder->_realURI = "dir1/dir2/script.php/script.py/dindon/ducon/mescouilles";
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "/script.py/dindon/ducon/mescouilles");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.php");
}

TEST_F(ResponseBuilderTest, TestPHPNoPathInfo) {
    responseBuilder->_realURI = "dir1/dir2/script.php";
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.php");
}

TEST_F(ResponseBuilderTest, TestPythonNoPathInfo) {
    responseBuilder->_realURI = "dir1/dir2/script.py";
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.py");
}

TEST_F(ResponseBuilderTest, TestWeirdURI1) {
    responseBuilder->_realURI = "dir1/dir2/.php/script.py";
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "/script.py");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/.php");
}

TEST_F(ResponseBuilderTest, TestWeirdURI2) {
    responseBuilder->_realURI = "dir1/dir2/script.py.php";
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.py.php");
}

TEST_F(ResponseBuilderTest, TestWeirdURI3) {
    responseBuilder->_realURI = "dir1/dir2/.py.php";
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/.py.php");
}
