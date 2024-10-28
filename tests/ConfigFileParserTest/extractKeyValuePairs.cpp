// #include <gtest/gtest.h>
// #include "ConfigFileParser.hpp" // Assurez-vous que le chemin est correct

// class ConfigFileParserTest : public ::testing::Test {
// protected:
//     ConfigFileParser configParser;
// };

// TEST_F(ConfigFileParserTest, ExtractKeyValuePairs_StoresSingleValueCorrectly) {
//     // Arrange
//     std::string line = "key1=value1";
//     std::string currentCategory = "server1";

//     // Act
//     configParser.extractKeyValuePairs(line, currentCategory);

//     // Assert
//     EXPECT_EQ(configParser._data[currentCategory]["key1"].size(), 1);
//     EXPECT_EQ(configParser._data[currentCategory]["key1"][0], "value1");
// }

// TEST_F(ConfigFileParserTest, ExtractKeyValuePairs_StoresMultipleValuesCorrectly) {
//     // Arrange
//     std::string line = "key1=value1,value2,value3";
//     std::string currentCategory = "server1";

//     // Act
//     configParser.extractKeyValuePairs(line, currentCategory);

//     // Assert
//     EXPECT_EQ(configParser._data[currentCategory]["key1"].size(), 3);
//     EXPECT_EQ(configParser._data[currentCategory]["key1"][0], "value1");
//     EXPECT_EQ(configParser._data[currentCategory]["key1"][1], "value2");
//     EXPECT_EQ(configParser._data[currentCategory]["key1"][2], "value3");
// }

// TEST_F(ConfigFileParserTest, ExtractKeyValuePairs_HandlesLeadingTrailingSpaces) {
//     // Arrange
//     std::string line = "   key1  =   value1 , value2   ";
//     std::string currentCategory = "server1";

//     // Act
//     configParser.extractKeyValuePairs(line, currentCategory);

//     // Assert
//     EXPECT_EQ(configParser._data[currentCategory]["key1"].size(), 2);
//     EXPECT_EQ(configParser._data[currentCategory]["key1"][0], "value1");
//     EXPECT_EQ(configParser._data[currentCategory]["key1"][1], "value2");
// }

// TEST_F(ConfigFileParserTest, ExtractKeyValuePairs_IgnoresEmptyValue) {
//     // Arrange
//     std::string line = "key1=";
//     std::string currentCategory = "server1";

//     // Act
//     configParser.extractKeyValuePairs(line, currentCategory);

//     // Assert
//     EXPECT_EQ(configParser._data[currentCategory]["key1"].size(), 0); // Aucune valeur ne doit être stockée
// }

// TEST_F(ConfigFileParserTest, ExtractKeyValuePairs_HandlesNoEqualSign) {
//     // Arrange
//     std::string line = "key1 value1"; // Pas de signe égal
//     std::string currentCategory = "server1";

//     // Act
//     configParser.extractKeyValuePairs(line, currentCategory);

//     // Assert
//     EXPECT_EQ(configParser._data[currentCategory].size(), 0); // Aucune donnée ne doit être ajoutée
// }
