#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "../../src/RequestParser/RequestParser.hpp"  // Inclure le bon header de ta classe

// Mock de Logger pour éviter d'écrire dans la sortie pendant les tests
class MockLogger {
public:
    static void log(int level, const std::string& message, const RequestParser& request) {
        // On peut simplement afficher dans la console ou ne rien faire pour les tests
        std::cout << "LOG: " << message << std::endl;
    }
};

class RequestParserTest : public ::testing::Test {
protected:
    RequestParser parser;  // Instance de RequestParser à tester

    void SetUp() override {
        // Optionnel : réinitialiser l'état de parser avant chaque test
    }

    void TearDown() override {
        // Optionnel : nettoyer après chaque test
    }
};

// Test pour une requête valide
TEST_F(RequestParserTest, ValidRequest) {
    std::istringstream requestStream("GET /index.html HTTP/1.1");
    parser.handleFirstLine(requestStream);
    EXPECT_TRUE(parser._isValid);  // Assure que la requête est valide
}

// Test pour une méthode non supportée
TEST_F(RequestParserTest, UnsupportedMethod) {
    std::istringstream requestStream("PUT /index.html HTTP/1.1");
    parser.handleFirstLine(requestStream);
    EXPECT_FALSE(parser._isValid);  // La méthode PUT n'est pas supportée
}

// Test pour une version HTTP incorrecte
TEST_F(RequestParserTest, InvalidHttpVersion) {
    std::istringstream requestStream("GET /index.html HTTP/1.0");
    parser.handleFirstLine(requestStream);
    EXPECT_FALSE(parser._isValid);  // La version HTTP doit être HTTP/1.1
}

// Test pour une ligne de requête incomplète
TEST_F(RequestParserTest, IncompleteRequestLine) {
    std::istringstream requestStream("GET HTTP/1.1");
    parser.handleFirstLine(requestStream);
    EXPECT_FALSE(parser._isValid);  // URI manquant
}

// Test pour une ligne de requête vide
TEST_F(RequestParserTest, EmptyRequestLine) {
    std::istringstream requestStream("");
    parser.handleFirstLine(requestStream);
    EXPECT_FALSE(parser._isValid);  // La ligne est vide
}

// Test pour des données restantes dans la ligne
TEST_F(RequestParserTest, ExtraDataInRequestLine) {
    std::istringstream requestStream("GET /index.html HTTP/1.1 extraData");
    parser.handleFirstLine(requestStream);
    EXPECT_FALSE(parser._isValid);  // Des données supplémentaires après la requête
}

// Suppose que ta fonction se trouve dans une classe Utils
class Utils {
public:
    static std::string charVectorToString(const std::vector<char>& vector) {
        return std::string(vector.begin(), vector.end());
    }
};

// Test Suite
TEST(SimpleTest, CharVectorToString) {
    // Cas de test 1 : vecteur vide
    std::vector<char> emptyVector;
    EXPECT_EQ(Utils::charVectorToString(emptyVector), "");

    // Cas de test 2 : vecteur avec des caractères
    std::vector<char> charVector = {'H', 'e', 'l', 'l', 'o'};
    EXPECT_EQ(Utils::charVectorToString(charVector), "Hello");

    // Cas de test 3 : vecteur avec des caractères spéciaux
    std::vector<char> specialChars = {'C', '+', '+', ' ', 'i', 's', ' ', 'f', 'u', 'n', '!'};
    EXPECT_EQ(Utils::charVectorToString(specialChars), "C++ is fun!");

    // Cas de test 4 : vecteur avec des espaces
    std::vector<char> spaces = {' ', 'T', 'e', 's', 't', ' ', 's', 't', 'r', 'i', 'n', 'g', 's', '!'};
    EXPECT_EQ(Utils::charVectorToString(spaces), " Test strings!");
}