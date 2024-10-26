#include "RequestParserTest.hpp"
// Test pour une requête valide
TEST_F(RequestParserTest, ValidRequest) {
    std::istringstream requestStream("GET /index.html HTTP/1.1");
    parser.handleFirstLine(requestStream);
    EXPECT_TRUE(parser.getIsValid());  // Assure que la requête est valide
}

// Test pour une méthode non supportée
TEST_F(RequestParserTest, UnsupportedMethod) {
    std::istringstream requestStream("PUT /index.html HTTP/1.1");
    parser.handleFirstLine(requestStream);
    EXPECT_FALSE(parser.getIsValid());  // La méthode PUT n'est pas supportée
}

// Test pour une version HTTP incorrecte
TEST_F(RequestParserTest, InvalidHttpVersion) {
    std::istringstream requestStream("GET /index.html HTTP/1.0");
    parser.handleFirstLine(requestStream);
    EXPECT_FALSE(parser.getIsValid());  // La version HTTP doit être HTTP/1.1
}

// Test pour une ligne de requête incomplète
TEST_F(RequestParserTest, IncompleteRequestLine) {
    std::istringstream requestStream("GET HTTP/1.1");
    parser.handleFirstLine(requestStream);
    EXPECT_FALSE(parser.getIsValid());  // URI manquant
}

// Test pour une ligne de requête vide
TEST_F(RequestParserTest, EmptyRequestLine) {
    std::istringstream requestStream("");
    parser.handleFirstLine(requestStream);
    EXPECT_FALSE(parser.getIsValid());  // La ligne est vide
}

// Test pour des données restantes dans la ligne
TEST_F(RequestParserTest, ExtraDataInRequestLine) {
    std::istringstream requestStream("GET /index.html HTTP/1.1 extraData");
    parser.handleFirstLine(requestStream);
    EXPECT_FALSE(parser.getIsValid());  // Des données supplémentaires après la requête
}

