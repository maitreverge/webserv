#include "RequestParserTest.hpp"

// Test de la méthode handleHeaderLines
TEST_F(RequestParserTest, HandleHeaderLines_ParsesHeadersCorrectly) {
    // Arrange
    std::istringstream requestStream(
        "Host: example.com\n"
        "Connection: keep-alive\n"
        "Accept: text/html,application/json\n"
        "\n"  // Ligne vide pour terminer l'en-tête
    );

    // Act
    parser.handleHeaderLines(requestStream);

    // Assert
    const auto& headers = parser._tmpHeaders;  // Accéder à _tmpHeaders
    EXPECT_EQ(headers.at("Host").size(), 1);
    EXPECT_EQ(headers.at("Host")[0], "example.com");
    EXPECT_EQ(headers.at("Connection").size(), 1);
    EXPECT_EQ(headers.at("Connection")[0], "keep-alive");
    EXPECT_EQ(headers.at("Accept").size(), 2);
    EXPECT_EQ(headers.at("Accept")[0], "text/html");
    EXPECT_EQ(headers.at("Accept")[1], "application/json");
}