#include "RequestParserTest.hpp"

TEST(HeadersTest, Reset) {
    Headers headers;
    
    // Assigner des valeurs initiales
    headers.Connection = "keep-alive";
    headers.ContentType = "text/html";
    headers.Host = "localhost";
    headers.Accept.push_back("text/plain");
    headers.ContentLength = 123;
    headers.Cookie["session_id"] = "abc123";
    
    // Appeler la méthode reset
    headers.reset();
    
    // Vérifier que tous les champs sont réinitialisés aux valeurs par défaut
    EXPECT_EQ(headers.Connection, "");
    EXPECT_EQ(headers.ContentType, "");
    EXPECT_EQ(headers.Host, "");
    EXPECT_TRUE(headers.Accept.empty());
    EXPECT_EQ(headers.ContentLength, 0);
    EXPECT_TRUE(headers.Cookie.empty());
}


TEST_F(RequestParserTest, ResetValues) {
    // Assigner des valeurs initiales à l'instance RequestParser
    parser._method = "POST";
    parser._URI = "/test";
    parser._HTTP_version = "HTTP/1.1";
    parser._isValid = false;
    parser._tmpHeaders["Custom-Header"] = {"Value"};
    parser._Headers.Connection = "keep-alive";
    
    // Appeler la méthode reset_values
    parser.reset_values();
    
    // Vérifier que toutes les valeurs sont réinitialisées
    EXPECT_EQ(parser.getMethod(), "");
    EXPECT_EQ(parser.getURI(), "");
    EXPECT_EQ(parser.getHTTP_version(), "");
    EXPECT_TRUE(parser.getIsValid());  // doit être true après reset
    EXPECT_TRUE(parser._tmpHeaders.empty());
    EXPECT_EQ(parser.getHeaders().Connection, "");
}

