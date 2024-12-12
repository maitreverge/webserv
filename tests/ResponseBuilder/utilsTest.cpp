#include "ResponseBuilderTest.hpp"

TEST_F(ResponseBuilderTest, Returns_Correct_Mimes_Types) {
    // Préparer les mime types pour le test
    responseBuilder->_mimeTypes.insert(std::make_pair("html", "text/html"));
    responseBuilder->_mimeTypes.insert(std::make_pair("htm", "text/htm"));
    responseBuilder->_mimeTypes.insert(std::make_pair("txt", "text/plain"));
    responseBuilder->_mimeTypes.insert(std::make_pair("css", "text/css"));
    responseBuilder->_mimeTypes.insert(std::make_pair("xml", "text/xml"));
    responseBuilder->_mimeTypes.insert(std::make_pair("js", "application/javascript"));
    responseBuilder->_mimeTypes.insert(std::make_pair("json", "application/json"));
    responseBuilder->_mimeTypes.insert(std::make_pair("pdf", "application/pdf"));
    responseBuilder->_mimeTypes.insert(std::make_pair("zip", "application/zip"));
    responseBuilder->_mimeTypes.insert(std::make_pair("jpeg", "image/jpeg"));
    responseBuilder->_mimeTypes.insert(std::make_pair("jpg", "image/jpg"));
    responseBuilder->_mimeTypes.insert(std::make_pair("png", "image/png"));
    responseBuilder->_mimeTypes.insert(std::make_pair("gif", "image/gif"));
    responseBuilder->_mimeTypes.insert(std::make_pair("webp", "image/webp"));
    responseBuilder->_mimeTypes.insert(std::make_pair("bmp", "image/bmp"));
    responseBuilder->_mimeTypes.insert(std::make_pair("ico", "image/x-icon"));
    responseBuilder->_mimeTypes.insert(std::make_pair("mp3", "audio/mp3"));
    responseBuilder->_mimeTypes.insert(std::make_pair("mpeg", "audio/mpeg"));
    responseBuilder->_mimeTypes.insert(std::make_pair("ogg", "audio/ogg"));
    responseBuilder->_mimeTypes.insert(std::make_pair("wav", "audio/wav"));
    responseBuilder->_mimeTypes.insert(std::make_pair("mp4", "video/mp4"));
    responseBuilder->_mimeTypes.insert(std::make_pair("webm", "video/webm"));
    responseBuilder->_mimeTypes.insert(std::make_pair("ogv", "video/ogv"));

    // Test for each known MIME type
    EXPECT_EQ(responseBuilder->extractType("html"), "text/html");
    EXPECT_EQ(responseBuilder->extractType("htm"), "text/htm");
    EXPECT_EQ(responseBuilder->extractType("txt"), "text/plain");
    EXPECT_EQ(responseBuilder->extractType("css"), "text/css");
    EXPECT_EQ(responseBuilder->extractType("xml"), "text/xml");
    EXPECT_EQ(responseBuilder->extractType("js"), "application/javascript");
    EXPECT_EQ(responseBuilder->extractType("json"), "application/json");
    EXPECT_EQ(responseBuilder->extractType("pdf"), "application/pdf");
    EXPECT_EQ(responseBuilder->extractType("zip"), "application/zip");
    EXPECT_EQ(responseBuilder->extractType("jpeg"), "image/jpeg");
    EXPECT_EQ(responseBuilder->extractType("jpg"), "image/jpg");
    EXPECT_EQ(responseBuilder->extractType("png"), "image/png");
    EXPECT_EQ(responseBuilder->extractType("gif"), "image/gif");
    EXPECT_EQ(responseBuilder->extractType("webp"), "image/webp");
    EXPECT_EQ(responseBuilder->extractType("bmp"), "image/bmp");
    EXPECT_EQ(responseBuilder->extractType("ico"), "image/x-icon");
    EXPECT_EQ(responseBuilder->extractType("mp3"), "audio/mp3");
    EXPECT_EQ(responseBuilder->extractType("mpeg"), "audio/mpeg");
    EXPECT_EQ(responseBuilder->extractType("ogg"), "audio/ogg");
    EXPECT_EQ(responseBuilder->extractType("wav"), "audio/wav");
    EXPECT_EQ(responseBuilder->extractType("mp4"), "video/mp4");
    EXPECT_EQ(responseBuilder->extractType("webm"), "video/webm");
    EXPECT_EQ(responseBuilder->extractType("ogv"), "video/ogv");

    // Test for an unknown MIME type, should return the default value
    EXPECT_EQ(responseBuilder->extractType("unknown"), "application/octet-stream");
    EXPECT_EQ(responseBuilder->extractType(""), "application/octet-stream"); // Test with empty string as well
}

TEST_F(ResponseBuilderTest, SetMethod_SetsGET) {
    // Tester le cas où la méthode est GET
    responseBuilder->setMethod(ResponseBuilder::GET);
    EXPECT_EQ(responseBuilder->_method, ResponseBuilder::GET);
}

TEST_F(ResponseBuilderTest, SetMethod_SetsPOST) {
    // Tester le cas où la méthode est POST
    responseBuilder->setMethod(ResponseBuilder::POST);
    EXPECT_EQ(responseBuilder->_method, ResponseBuilder::POST);
}

TEST_F(ResponseBuilderTest, SetMethod_SetsDELETE) {
    // Tester le cas où la méthode est DELETE
    responseBuilder->setMethod(ResponseBuilder::DELETE);
    EXPECT_EQ(responseBuilder->_method, ResponseBuilder::DELETE);
}

TEST_F(ResponseBuilderTest, SanatizeURI_RemovesSingleOccurrence) {
    std::string uri = "/path/to/../resource";
    responseBuilder->sanatizeURI(uri);
    EXPECT_EQ(uri, "/path/to/resource");
}

TEST_F(ResponseBuilderTest, SanatizeURI_RemovesMultipleOccurrences) {
    std::string uri = "/path/../../to/../resource/../";
    responseBuilder->sanatizeURI(uri);
    EXPECT_EQ(uri, "/path/to/resource/");
}

TEST_F(ResponseBuilderTest, SanatizeURI_NoChangeWhenNoNeedle) {
    std::string uri = "/path/to/resource";
    responseBuilder->sanatizeURI(uri);
    EXPECT_EQ(uri, "/path/to/resource");
}

TEST_F(ResponseBuilderTest, SanatizeURI_RemovesConsecutiveOccurrences) {
    std::string uri = "/path/../../../to/../resource";
    responseBuilder->sanatizeURI(uri);
    EXPECT_EQ(uri, "/path/to/resource");
}

TEST_F(ResponseBuilderTest, SanatizeURI_OnlyNeedles) {
    std::string uri = "../..";
    responseBuilder->sanatizeURI(uri);
    EXPECT_EQ(uri, "..");
}

TEST_F(ResponseBuilderTest, SanatizeURI_EmptyString) {
    std::string uri = "";
    responseBuilder->sanatizeURI(uri);
    EXPECT_EQ(uri, "");
}


