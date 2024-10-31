#include "ResponseBuilderTest.hpp"

TEST_F(ResponseBuilderTest, extractFileNature_Test1) {
    
	string target = "/dir1/dir2/image.jpeg";

	responseBuilder->extractFileNature(target);
    
	EXPECT_EQ(responseBuilder->_fileName, "image.jpeg");
    EXPECT_EQ(responseBuilder->_fileExtension, "jpeg");
}

TEST_F(ResponseBuilderTest, extractFileNature_Test2) {
    
	string target = "image.jpeg";

	responseBuilder->extractFileNature(target);
    
	EXPECT_EQ(responseBuilder->_fileName, "image.jpeg");
    EXPECT_EQ(responseBuilder->_fileExtension, "jpeg");
}

TEST_F(ResponseBuilderTest, extractFileNature_Test3) {
    
	string target = "/image.jpeg";

	responseBuilder->extractFileNature(target);
    
	EXPECT_EQ(responseBuilder->_fileName, "image.jpeg");
    EXPECT_EQ(responseBuilder->_fileExtension, "jpeg");
}

TEST_F(ResponseBuilderTest, extractFileNature_Test4) {
    
	string target = "";

	responseBuilder->extractFileNature(target);
    
	EXPECT_EQ(responseBuilder->_fileName, "");
    EXPECT_EQ(responseBuilder->_fileExtension, "");
}

TEST_F(ResponseBuilderTest, extractFileNature_Test5) {
    
	string target = ".jpeg";

	responseBuilder->extractFileNature(target);
    
	EXPECT_EQ(responseBuilder->_fileName, ".jpeg");
    EXPECT_EQ(responseBuilder->_fileExtension, "jpeg");
}

TEST_F(ResponseBuilderTest, extractFileNature_Test6) {
    
	string target = "..jpeg";

	responseBuilder->extractFileNature(target);
    
	EXPECT_EQ(responseBuilder->_fileName, "..jpeg");
    EXPECT_EQ(responseBuilder->_fileExtension, "jpeg");
}