#include "ResponseBuilderTest.hpp"

TEST_F(ResponseBuilderTest, RootMapping_MainRouteFoundImmediately) {
	// responseBuilder->_realURI = "/hello";

	mockConfig->routeMapping["/dir1"] = {{"foo", "bar"}};
	responseBuilder->_realURI = "/dir1/foo";

	responseBuilder->rootMapping();

	EXPECT_EQ(responseBuilder->_realURI, "/dir1/bar");
}

TEST_F(ResponseBuilderTest, RootMapping_MainRouteFoundAfterTrimming) {
	mockConfig->routeMapping["/dir1"] = {{"foo", "bar"}};
	responseBuilder->_realURI = "/dir1/dir2/foo";

	responseBuilder->rootMapping();

	EXPECT_EQ(responseBuilder->_realURI, "/dir1/bar");
}

TEST_F(ResponseBuilderTest, RootMapping_MainRouteNotFound) {
	mockConfig->routeMapping["/dir1"] = {{"foo", "bar"}};
	responseBuilder->_realURI = "/dir2/foo";

	responseBuilder->rootMapping();

	EXPECT_EQ(responseBuilder->_realURI, "/dir2/foo");
}

TEST_F(ResponseBuilderTest, RootMapping_TargetNeedsReroute) {
	mockConfig->routeMapping["/dir1"] = {{"foo", "bar"}};
	responseBuilder->_realURI = "/dir1/foo/bar";

	responseBuilder->rootMapping();

	EXPECT_EQ(responseBuilder->_realURI, "/dir1/bar/bar");
}

TEST_F(ResponseBuilderTest, RootMapping_TargetDoesNotNeedReroute) {
	mockConfig->routeMapping["/dir1"] = {{"foo", "bar"}};
	responseBuilder->_realURI = "/dir1/bar";

	responseBuilder->rootMapping();

	EXPECT_EQ(responseBuilder->_realURI, "/dir1/bar");
}