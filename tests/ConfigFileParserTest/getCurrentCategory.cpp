// #include <gtest/gtest.h>
// #include "ConfigFileParserTest.hpp" // Assurez-vous que le chemin est correct

// TEST_F(ConfigFileParserTest, GetCurrentCategory_ReturnsZero_ForValidCategory) {
//     // Arrange
//     std::string line = "[categoryName]";
//     std::string currentCategory;

//     // Act
//     int result = configParser.getCurrentCategory(line, currentCategory);

//     // Assert
//     EXPECT_EQ(result, 0);
//     EXPECT_EQ(currentCategory, "categoryName");
// }

// TEST_F(ConfigFileParserTest, GetCurrentCategory_ReturnsOne_ForEmptyLine) {
//     // Arrange
//     std::string line = "";
//     std::string currentCategory;

//     // Act
//     int result = configParser.getCurrentCategory(line, currentCategory);

//     // Assert
//     EXPECT_EQ(result, 1);
//     EXPECT_TRUE(currentCategory.empty());
// }

// TEST_F(ConfigFileParserTest, GetCurrentCategory_ReturnsOne_ForMalformedLine) {
//     // Arrange
//     std::string line = "[incompleteCategory";
//     std::string currentCategory;

//     // Act
//     int result = configParser.getCurrentCategory(line, currentCategory);

//     // Assert
//     EXPECT_EQ(result, 1);
//     EXPECT_TRUE(currentCategory.empty());
// }

// TEST_F(ConfigFileParserTest, GetCurrentCategory_SetsCategoryForTrimmedLine) {
//     // Arrange
//     std::string line = "   [global]   ";
//     std::string currentCategory;

//     // Act
//     int result = configParser.getCurrentCategory(line, currentCategory);

//     // Assert
//     EXPECT_EQ(result, 0);
//     EXPECT_EQ(currentCategory, "global");
// }

// TEST_F(ConfigFileParserTest, GetCurrentCategory_UpdatesDataMap) {
//     // Arrange
//     std::string line = "[newCategory]";
//     std::string currentCategory;

//     // Act
//     int result = configParser.getCurrentCategory(line, currentCategory);

//     // Assert
//     EXPECT_EQ(result, 0);
//     EXPECT_TRUE(configParser._data.find(currentCategory) != configParser._data.end());
// }