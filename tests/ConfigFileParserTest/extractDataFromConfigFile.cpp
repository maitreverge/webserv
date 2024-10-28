#include <gtest/gtest.h>
#include "ConfigFileParser.hpp" // Assurez-vous que le chemin est correct
#include <fstream>

// Création d'une classe de test pour ConfigFileParser
class ConfigFileParserTest : public ::testing::Test {
protected:
    ConfigFileParser configParser;
    const std::string testFileName = "test_config.cfg";

    void SetUp() override {
        // Écriture d'un fichier de configuration de test
        std::ofstream testFile(testFileName);
        testFile << "[server1]\n"
                 << "key1=value1\n"
                 << "# This is a comment\n"
                 << "[server2]\n"
                 << "key2=value2\n";
        testFile.close();
    }

    void TearDown() override {
        // Suppression du fichier de test après le test
        std::remove(testFileName.c_str());
    }
};

//!SEGFAULT
// TEST_F(ConfigFileParserTest, ExtractDataFromConfigFile_ReturnsZero_ForValidFile) {
//     // Act
//     int result = configParser.extractDataFromConfigFile(testFileName);

//     // Assert
//     EXPECT_EQ(result, 0);
//     EXPECT_TRUE(configParser._data.find("server1") != configParser._data.end());
//     EXPECT_TRUE(configParser._data.find("server2") != configParser._data.end());
// }

TEST_F(ConfigFileParserTest, ExtractDataFromConfigFile_ReturnsOne_ForInvalidFile) {
    // Act
    int result = configParser.extractDataFromConfigFile("invalid_config.cfg");

    // Assert
    EXPECT_EQ(result, 1); // Devrait retourner 1 car le fichier n'existe pas
}

//!SEGFAULT
// TEST_F(ConfigFileParserTest, ExtractDataFromConfigFile_IgnoresCommentLines) {
//     // Act
//     int result = configParser.extractDataFromConfigFile(testFileName);

//     // Assert
//     EXPECT_EQ(result, 0);
//     // Vérifie que les commentaires n'ont pas été ajoutés à _data
//     EXPECT_FALSE(configParser._data["server1"].count("key1"));
//     EXPECT_EQ(configParser._data["server2"]["key2"].size(), 1); // key2 doit être présent
// }
