#include <gtest/gtest.h>
#include "ConfigFileParser.hpp" // Assurez-vous que le chemin est correct

class ConfigFileParserTest : public ::testing::Test {
protected:
    ConfigFileParser configParser;
};

TEST_F(ConfigFileParserTest, IsServerData_ReturnsTrue_ForServerCategory) {
    // Arrange
    std::string category = "server1";

    // Act
    bool result = configParser.isServerData(category);

    // Assert
    EXPECT_TRUE(result);
}

TEST_F(ConfigFileParserTest, IsServerData_ReturnsFalse_ForNonServerCategory) {
    // Arrange
    std::string category = "client";

    // Act
    bool result = configParser.isServerData(category);

    // Assert
    EXPECT_FALSE(result);
}

TEST_F(ConfigFileParserTest, IsServerData_ReturnsFalse_ForShortCategoryName) {
    // Arrange
    std::string category = "srv"; // Trop court pour être un serveur

    // Act
    bool result = configParser.isServerData(category);

    // Assert
    EXPECT_FALSE(result);
}

TEST_F(ConfigFileParserTest, IsServerData_ReturnsTrue_ForServerPrefix) {
    // Arrange
    std::string category = "server";

    // Act
    bool result = configParser.isServerData(category);

    // Assert
    EXPECT_TRUE(result);
}
