#include "ResponseBuilderTest.hpp"

vector< string > VEC_PHP_PYTHON = {".php", ".py"};
vector< string > VEC_ONLY_PHP = {".php"};
vector< string > VEC_ONLY_PYTHON = {".py"};
vector< string > VEC_EMPTY = {""};
vector< string > VEC_NOPE_1 = {"php", "py"};
vector< string > VEC_NOPE_2 = {"nope", "nope"};
vector< string > VEC_NOPE_3 = {"php ", "py "};

// ================== HAPPY PATHS TESTS ===========================
TEST_F(ResponseBuilderTest, CGI_PHP_1) {
    responseBuilder->_realURI = "dir1/dir2/script.php/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_ONLY_PHP;
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "/dindon/ducon/mescouilles");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.php");
}

TEST_F(ResponseBuilderTest, CGI_PYTHON_AND_PHP_1) {
    responseBuilder->_realURI = "dir1/dir2/script.php/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_PHP_PYTHON;
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "/dindon/ducon/mescouilles");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.php");
}

TEST_F(ResponseBuilderTest, CGI_PYTHON_1) {
    responseBuilder->_realURI = "dir1/dir2/script.py/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_ONLY_PYTHON;
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "/dindon/ducon/mescouilles");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.php");
}

TEST_F(ResponseBuilderTest, CGI_PYTHON_AND_PHP_2) {
    responseBuilder->_realURI = "dir1/dir2/script.py/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_PHP_PYTHON;
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "/dindon/ducon/mescouilles");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.php");
}

// ================== EMPTY/FALSE PATHS TESTS ===========================

TEST_F(ResponseBuilderTest, CGI_EMPTY_PYTHON) {
    responseBuilder->_realURI = "dir1/dir2/script.py/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_EMPTY;
    responseBuilder->checkCGI();
    EXPECT_FALSE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.py/dindon/ducon/mescouilles");
}

TEST_F(ResponseBuilderTest, CGI_FALSE_PYTHON_1) {
    responseBuilder->_realURI = "dir1/dir2/script.py/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_NOPE_1;
    responseBuilder->checkCGI();
    EXPECT_FALSE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.py/dindon/ducon/mescouilles");
}

TEST_F(ResponseBuilderTest, CGI_FALSE_PYTHON_2) {
    responseBuilder->_realURI = "dir1/dir2/script.py/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_NOPE_2;
    responseBuilder->checkCGI();
    EXPECT_FALSE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.py/dindon/ducon/mescouilles");
}

TEST_F(ResponseBuilderTest, CGI_FALSE_PYTHON_3) {
    responseBuilder->_realURI = "dir1/dir2/script.py/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_NOPE_3;
    responseBuilder->checkCGI();
    EXPECT_FALSE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.py/dindon/ducon/mescouilles");
}

TEST_F(ResponseBuilderTest, CGI_EMPTY_PHP) {
    responseBuilder->_realURI = "dir1/dir2/script.php/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_EMPTY;
    responseBuilder->checkCGI();
    EXPECT_FALSE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.php/dindon/ducon/mescouilles");
}

TEST_F(ResponseBuilderTest, CGI_FALSE_PHP_1) {
    responseBuilder->_realURI = "dir1/dir2/script.php/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_NOPE_1;
    responseBuilder->checkCGI();
    EXPECT_FALSE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.php/dindon/ducon/mescouilles");
}

TEST_F(ResponseBuilderTest, CGI_FALSE_PHP_2) {
    responseBuilder->_realURI = "dir1/dir2/script.php/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_NOPE_2;
    responseBuilder->checkCGI();
    EXPECT_FALSE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.php/dindon/ducon/mescouilles");
}

TEST_F(ResponseBuilderTest, CGI_FALSE_PHP_3) {
    responseBuilder->_realURI = "dir1/dir2/script.php/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_NOPE_3;
    responseBuilder->checkCGI();
    EXPECT_FALSE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.php/dindon/ducon/mescouilles");
}

TEST_F(ResponseBuilderTest, CGI_FALSE_PHP_4) {
    responseBuilder->_realURI = "dir1/dir2/script.php/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_ONLY_PYTHON;
    responseBuilder->checkCGI();
    EXPECT_FALSE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.php/dindon/ducon/mescouilles");
}

TEST_F(ResponseBuilderTest, CGI_FALSE_PYTHON_4) {
    responseBuilder->_realURI = "dir1/dir2/script.py/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_ONLY_PHP;
    responseBuilder->checkCGI();
    EXPECT_FALSE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.py/dindon/ducon/mescouilles");
}



// ================== EDGE CASES URIS TESTS ===========================

TEST_F(ResponseBuilderTest, CGI_NO_CGI_1) {
    responseBuilder->_realURI = "dir1/dir2/script.txt/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_PHP_PYTHON;
    responseBuilder->checkCGI();
    EXPECT_FALSE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.txt/dindon/ducon/mescouilles");
}

TEST_F(ResponseBuilderTest, CGI_NO_CGI_2) {
    responseBuilder->_realURI = "dir1/dir2/script.p.y/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_PHP_PYTHON;
    responseBuilder->checkCGI();
    EXPECT_FALSE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.p.y/dindon/ducon/mescouilles");
}

TEST_F(ResponseBuilderTest, CGI_URI_WITH_PHP_AND_PY_1) {
    responseBuilder->_realURI = "dir1/dir2/script.php/script.py/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = {".php", ".py"}; // .php declared first
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "/script.py/dindon/ducon/mescouilles");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.php");
}

TEST_F(ResponseBuilderTest, CGI_URI_WITH_PHP_AND_PY_2) {
    responseBuilder->_realURI = "dir1/dir2/script.php/script.py/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = {".py", ".php"};  // .py declared first
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "/script.py/dindon/ducon/mescouilles");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.php");
}

TEST_F(ResponseBuilderTest, CGI_URI_WITH_PHP_AND_PY_3) {
    responseBuilder->_realURI = "dir1/dir2/script.php/script.py/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_ONLY_PHP;
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "/script.py/dindon/ducon/mescouilles");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.php");
}

TEST_F(ResponseBuilderTest, CGI_URI_WITH_PHP_AND_PY_4) {
    responseBuilder->_realURI = "dir1/dir2/script.php/script.py/dindon/ducon/mescouilles";
	responseBuilder->_myconfig.cgiAllowed = VEC_ONLY_PYTHON;
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "/dindon/ducon/mescouilles");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.php/script.py");
}


TEST_F(ResponseBuilderTest, CGI_PHPNoPathInfo) {
    responseBuilder->_realURI = "dir1/dir2/script.php";
	responseBuilder->_myconfig.cgiAllowed = VEC_PHP_PYTHON;
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.php");
}

TEST_F(ResponseBuilderTest, CGI_PythonNoPathInfo) {
    responseBuilder->_realURI = "dir1/dir2/script.py";
	responseBuilder->_myconfig.cgiAllowed = VEC_PHP_PYTHON;
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.py");
}

TEST_F(ResponseBuilderTest, CGI_WeirdURI1) {
    responseBuilder->_realURI = "dir1/dir2/.php/script.py";
	responseBuilder->_myconfig.cgiAllowed = VEC_PHP_PYTHON;
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "/script.py");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/.php");
}

TEST_F(ResponseBuilderTest, CGI_WeirdURI2) {
    responseBuilder->_realURI = "dir1/dir2/script.py.php";
	responseBuilder->_myconfig.cgiAllowed = VEC_PHP_PYTHON;
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.py.php");
}

TEST_F(ResponseBuilderTest, CGI_WeirdURI3) {
    responseBuilder->_realURI = "dir1/dir2/script.py.php";
	responseBuilder->_myconfig.cgiAllowed = VEC_ONLY_PHP;
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.py.php");
}

TEST_F(ResponseBuilderTest, CGI_WeirdURI4) {
    responseBuilder->_realURI = "dir1/dir2/script.py.php";
	responseBuilder->_myconfig.cgiAllowed = VEC_ONLY_PYTHON;
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.py.php");
}

TEST_F(ResponseBuilderTest, CGI_WeirdURI5) {
    responseBuilder->_realURI = "dir1/dir2/script.py.php/coucou/hello";
	responseBuilder->_myconfig.cgiAllowed = VEC_ONLY_PYTHON;
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.py.php/coucou/hello");
}

TEST_F(ResponseBuilderTest, CGI_WeirdURI6) {
    responseBuilder->_realURI = "dir1/dir2/script.py.php/coucou/hello";
	responseBuilder->_myconfig.cgiAllowed = VEC_ONLY_PHP;
    responseBuilder->checkCGI();
    EXPECT_TRUE(responseBuilder->_isCGI);
    EXPECT_EQ(responseBuilder->_pathInfo, "/coucou/hello");
    EXPECT_EQ(responseBuilder->_realURI, "dir1/dir2/script.py.php");
}
