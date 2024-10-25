#include "RequestParserTest.hpp"

TEST_F(RequestParserTest, Trim) {
    std::string str1 = "   Hello World   ";
    std::string str2 = "NoSpaces";
    std::string str3 = "   LeadingSpaces";
    std::string str4 = "TrailingSpaces   ";
    std::string str5 = "";  // Chaîne vide

    parser.trim(str1);
    EXPECT_EQ(str1, "Hello World");

    parser.trim(str2);
    EXPECT_EQ(str2, "NoSpaces");

    parser.trim(str3);
    EXPECT_EQ(str3, "LeadingSpaces");

    parser.trim(str4);
    EXPECT_EQ(str4, "TrailingSpaces");

    parser.trim(str5);
    EXPECT_EQ(str5, "");  // La chaîne reste vide après trim
}
