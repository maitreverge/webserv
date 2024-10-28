#include "RequestParserTest.hpp"

//! Test pour assignHeader avec std::string
TEST_F(RequestParserTest, AssignHeaderString) {
    std::string connection = "keep-alive";
    parser.assignHeader("Connection", connection);
    EXPECT_EQ(connection, "keep-alive");
}

// Test pour assignHeader avec std::vector<std::string>
TEST_F(RequestParserTest, AssignHeaderVector) {
    std::vector<std::string> accept;
    parser._tmpHeaders["Accept"] = {"text/html", "application/xhtml+xml"};
    parser.assignHeader("Accept", accept);
    EXPECT_EQ(accept.size(), 2);
    EXPECT_EQ(accept[0], "text/html");
    EXPECT_EQ(accept[1], "application/xhtml+xml");
}

//! Test pour assignHeader avec size_t
TEST_F(RequestParserTest, AssignHeaderSizeT) {
    size_t contentLength = 123;
    parser.assignHeader("Content-Length", contentLength);
    EXPECT_EQ(contentLength, 123);
}

// Test pour assignHeader avec std::map<std::string, std::string>
TEST_F(RequestParserTest, AssignHeaderMap) {
    parser._tmpHeaders["Cookie"] = {"session_id=abc123; user_id=42"};

    std::map<std::string, std::string> cookies;
    parser.assignHeader("Cookie", cookies);
    EXPECT_EQ(cookies["session_id"], "abc123");
    EXPECT_EQ(cookies["user_id"], "42");
}