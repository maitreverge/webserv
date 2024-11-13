#include "ResponseBuilderTest.hpp"

string nope = "hello";
string one_r = "hello\r";
string rn_ok = "hello\r\n";
string rn_ok_plus_one = "hello\r\na";
string rn_ok_plus_two = "hello\r\nab";

vector<char> NEXT_LINE;

vector<char> NO_RN(nope.begin(), nope.end());
vector<char> ONE_R(one_r.begin(), one_r.end());
vector<char> RN_OK(rn_ok.begin(), rn_ok.end());
vector<char> RN_OK_PLUS_ONE(rn_ok_plus_one.begin(), rn_ok_plus_one.end());
vector<char> RN_OK_PLUS_TWO(rn_ok_plus_two.begin(), rn_ok_plus_two.end());

TEST_F(ResponseBuilderTest, isLineDelim_1) {
    
	NEXT_LINE.clear();

	EXPECT_FALSE(responseBuilder->isLineDelim(NO_RN, NEXT_LINE));

	EXPECT_EQ(NEXT_LINE, std::vector<char>());
	EXPECT_EQ(NO_RN, std::vector<char>({'h', 'e', 'l', 'l', 'o'}));
	
	NEXT_LINE.clear();
}

TEST_F(ResponseBuilderTest, isLineDelim_2) {
    
	NEXT_LINE.clear();

	EXPECT_FALSE(responseBuilder->isLineDelim(ONE_R, NEXT_LINE));

	EXPECT_EQ(NEXT_LINE, std::vector<char>());
	EXPECT_EQ(ONE_R, std::vector<char>({'h', 'e', 'l', 'l', 'o', '\r'}));
	
	NEXT_LINE.clear();
}

TEST_F(ResponseBuilderTest, isLineDelim_3) {
    
	NEXT_LINE.clear();

	EXPECT_TRUE(responseBuilder->isLineDelim(RN_OK, NEXT_LINE));

	EXPECT_EQ(NEXT_LINE, std::vector<char>());
	EXPECT_EQ(RN_OK, std::vector<char>({'h', 'e', 'l', 'l', 'o', '\r', '\n'}));
	
	NEXT_LINE.clear();
}

TEST_F(ResponseBuilderTest, isLineDelim_4) {
    
	NEXT_LINE.clear();

	EXPECT_TRUE(responseBuilder->isLineDelim(RN_OK_PLUS_ONE, NEXT_LINE));

	EXPECT_EQ(NEXT_LINE, std::vector<char>({'a'}));
	EXPECT_EQ(RN_OK_PLUS_ONE, std::vector<char>({'h', 'e', 'l', 'l', 'o', '\r', '\n'}));
	
	// NEXT_LINE.clear();
}

TEST_F(ResponseBuilderTest, isLineDelim_5) {
    
	NEXT_LINE.clear();

	EXPECT_TRUE(responseBuilder->isLineDelim(RN_OK_PLUS_TWO, NEXT_LINE));

	EXPECT_EQ(NEXT_LINE, std::vector<char>({'a', 'b'}));
	EXPECT_EQ(RN_OK_PLUS_TWO, std::vector<char>({'h', 'e', 'l', 'l', 'o', '\r', '\n'}));
	
	NEXT_LINE.clear();
}