#include "ResponseBuilderTest.hpp"

TEST_F(ResponseBuilderTest, checkMethod_HappyPath1) {

	responseBuilder->_errorType = CODE_200_OK;
	responseBuilder->_method = static_cast<ResponseBuilder::e_method>(GET);

	vector<string> innerVector{"GET"};
	config->allowedMethods["/"] = {{"allowedMethods", innerVector}};
	responseBuilder->_realURI = "/";

	responseBuilder->checkMethod();

	EXPECT_EQ(responseBuilder->_errorType, CODE_200_OK);
}

TEST_F(ResponseBuilderTest, checkMethod_HappyPath2) {

	responseBuilder->_errorType = CODE_200_OK;
	responseBuilder->_method = static_cast<ResponseBuilder::e_method>(POST);

	vector<string> innerVector{"GET", "POST"};
	config->allowedMethods["/"] = {{"allowedMethods", innerVector}};
	responseBuilder->_realURI = "/";

	responseBuilder->checkMethod();

	EXPECT_EQ(responseBuilder->_errorType, CODE_200_OK);
}

TEST_F(ResponseBuilderTest, checkMethod_HappyPath3) {

	responseBuilder->_errorType = CODE_200_OK;
	responseBuilder->_method = static_cast<ResponseBuilder::e_method>(DELETE);

	vector<string> innerVector{"GET", "POST", "DELETE"};
	config->allowedMethods["/"] = {{"allowedMethods", innerVector}};
	responseBuilder->_realURI = "/";

	responseBuilder->checkMethod();

	EXPECT_EQ(responseBuilder->_errorType, CODE_200_OK);
}

TEST_F(ResponseBuilderTest, checkMethod_NonExistingRouteMathingMethods) {

	responseBuilder->_errorType = CODE_200_OK;
	responseBuilder->_method = static_cast<ResponseBuilder::e_method>(GET);

	vector<string> innerVector{"GET"};
	config->allowedMethods["/"] = {{"allowedMethods", innerVector}};
	responseBuilder->_realURI = "/NOPE_ROUTE"; // route not mathing
	
	responseBuilder->checkMethod();

	EXPECT_EQ(responseBuilder->_errorType, CODE_200_OK);
}

TEST_F(ResponseBuilderTest, checkMethod_ExistingRouteNotMathingMethods) {

	responseBuilder->_errorType = CODE_200_OK;
	responseBuilder->_method = static_cast<ResponseBuilder::e_method>(POST);

	vector<string> innerVector{"GET"};
	config->allowedMethods["/"] = {{"anotherMethod", innerVector}};
	responseBuilder->_realURI = "/"; // route not mathing

	responseBuilder->checkMethod();

	EXPECT_EQ(responseBuilder->_errorType, CODE_200_OK);
}

TEST_F(ResponseBuilderTest, checkMethod_NonExistingRouteNotMathingMethods) {

	responseBuilder->_errorType = CODE_200_OK;
	responseBuilder->_method = static_cast<ResponseBuilder::e_method>(GET);

	vector<string> innerVector{"DELETE"};
	config->allowedMethods["/"] = {{"allowedMethods", innerVector}};
	responseBuilder->_realURI = "/NOPE_ROUTE"; // route not mathing

	responseBuilder->checkMethod();

	EXPECT_EQ(responseBuilder->_errorType, CODE_200_OK);
}

TEST_F(ResponseBuilderTest, checkMethod_ExistingRouteMathingMethods) {

	responseBuilder->_errorType = CODE_200_OK;
	responseBuilder->_method = static_cast<ResponseBuilder::e_method>(DELETE);

	vector<string> innerVector{"GET", "POST"};
	config->allowedMethods["/"] = {{"allowedMethods", innerVector}};
	responseBuilder->_realURI = "/"; // route not mathing

	responseBuilder->checkMethod();

	EXPECT_EQ(responseBuilder->_errorType, CODE_405_METHOD_NOT_ALLOWED);
}