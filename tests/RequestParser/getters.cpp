#include "RequestParserTest.hpp"

TEST_F(RequestParserTest, GetMethod) {
    parser._method = "GET";  // Initialisation pour le test
    EXPECT_EQ(parser.getMethod(), "GET");
}

TEST_F(RequestParserTest, GetURI) {
    parser._URI = "/index.html";  // Initialisation pour le test
    EXPECT_EQ(parser.getURI(), "/index.html");
}

TEST_F(RequestParserTest, GetHTTPVersion) {
    parser._HTTP_version = "HTTP/1.1";  // Initialisation pour le test
    EXPECT_EQ(parser.getHTTP_version(), "HTTP/1.1");
}

TEST_F(RequestParserTest, GetIsValid) {
    parser._isValid = true;  // Initialisation pour le test
    EXPECT_TRUE(parser.getIsValid());
    parser._isValid = false;
    EXPECT_FALSE(parser.getIsValid());
}

TEST_F(RequestParserTest, GetHeaders) {
    Headers headers;
    headers.Connection = "keep-alive";
    headers.ContentType = "text/html";
    headers.Host = "localhost";
    headers.Accept = {"text/html", "application/json"};
    headers.ContentLength = 1024;
    headers.Cookie = {{"session_id", "abc123"}, {"user", "test"}};

    parser._Headers = headers;
    EXPECT_EQ(parser.getHeaders(), headers);
}

TEST_F(RequestParserTest, GetClient) {
    struct Client client;  // Initialiser un objet Client pour le test
    parser._Client = &client;  // Assigner l'adresse de client
    EXPECT_EQ(parser.getClient(), &client);
}
