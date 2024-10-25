#include "RequestParserTest.hpp"

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