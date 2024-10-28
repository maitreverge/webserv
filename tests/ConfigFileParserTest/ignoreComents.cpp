#include <gtest/gtest.h>
#include "ConfigFileParser.hpp" // Assurez-vous que le chemin est correct

class ConfigFileParserTest : public ::testing::Test {
protected:
    ConfigFileParser configParser;
};

TEST_F(ConfigFileParserTest, IgnoreComents_RemovesCommentAndLeadingSpaces) {
    // Arrange
    std::string line = "              # This is a comment\n";
    
    // Act
    int result = configParser.ignoreComents(line);

    // Assert
    EXPECT_EQ(result, 0);
    EXPECT_EQ(line, ""); // Laisse uniquement la nouvelle ligne
}

TEST_F(ConfigFileParserTest, IgnoreComents_RemovesLeadingSpaces) {
    // Arrange
    std::string line = "    [category] # Comment";

    // Act
    int result = configParser.ignoreComents(line);

    // Assert
    EXPECT_EQ(result, 1);
    EXPECT_EQ(line, "[category]"); // Retire l'espace devant
}

TEST_F(ConfigFileParserTest, IgnoreComents_RemovesTrailingCharactersAfterBracket) {
    // Arrange
    std::string line = "[category] # This is a comment";

    // Act
    int result = configParser.ignoreComents(line);

    // Assert
    EXPECT_EQ(result, 1);
    EXPECT_EQ(line, "[category]"); // Retire la partie commentaire
}

TEST_F(ConfigFileParserTest, IgnoreComents_ReturnsZero_WhenLineIsEmptyAfterProcessing) {
    // Arrange
    std::string line = "    # Comment line";

    // Act
    int result = configParser.ignoreComents(line);

    // Assert
    EXPECT_EQ(result, 0);
    EXPECT_TRUE(line.empty()); // La ligne devrait être vide
}

TEST_F(ConfigFileParserTest, IgnoreComents_ReturnsOne_WhenLineHasNonEmptyContent) {
    // Arrange
    std::string line = "[category]";

    // Act
    int result = configParser.ignoreComents(line);

    // Assert
    EXPECT_EQ(result, 1);
    EXPECT_EQ(line, "[category]"); // La ligne doit rester intacte
}
