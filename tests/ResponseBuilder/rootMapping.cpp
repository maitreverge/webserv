#include "ResponseBuilderTest.hpp"

// OK TEST
TEST_F(ResponseBuilderTest, RootMapping_MainRouteFoundDirectly1) {
	config->routeMapping["/"] = {{"/coucou", "/tmp/www"}};
	responseBuilder->_realURI = "/main/image.jpeg";

	responseBuilder->rootMapping();

	EXPECT_EQ(responseBuilder->_realURI, "/main/image.jpeg");
}

// OK TEST
TEST_F(ResponseBuilderTest, RootMapping_MainRouteFoundDirectly2) {
	config->routeMapping["/src/default_folder"] = {{"/coucou", "/tmp/www"}};
	responseBuilder->_realURI = "/src/default_folder/coucou/image.jpeg";

	responseBuilder->rootMapping();

	EXPECT_EQ(responseBuilder->_realURI, "/tmp/www/image.jpeg");
}

// OK TEST
TEST_F(ResponseBuilderTest, RootMapping_MainRouteFoundDirectly3) {
	config->routeMapping["/src/default_folder"] = {{"/coucou/les/amis/mdr", "/tmp/www"}};
	responseBuilder->_realURI = "/src/default_folder/coucou/les/amis/mdr/image.jpeg";

	responseBuilder->rootMapping();

	EXPECT_EQ(responseBuilder->_realURI, "/tmp/www/image.jpeg");
}

// OK TEST
TEST_F(ResponseBuilderTest, RootMapping_RouteFoundButNeedleNotFound) {
	config->routeMapping["/src/default_folder"] = {{"/coucou/les/amis/mdr/NOPE", "/tmp/www"}};
	responseBuilder->_realURI = "/src/default_folder/coucou/les/amis/mdr/image.jpeg";

	responseBuilder->rootMapping();

	EXPECT_EQ(responseBuilder->_realURI, "/src/default_folder/coucou/les/amis/mdr/image.jpeg");
}

// OK TEST
TEST_F(ResponseBuilderTest, RootMapping_RouteNotFoundButExistingNeedle) {
	config->routeMapping["/src/default_folder/NOPE"] = {{"/coucou/les/amis/mdr", "/tmp/www"}};
	responseBuilder->_realURI = "/src/default_folder/coucou/les/amis/mdr/image.jpeg";

	responseBuilder->rootMapping();

	EXPECT_EQ(responseBuilder->_realURI, "/src/default_folder/coucou/les/amis/mdr/image.jpeg");
}

// OK TEST
TEST_F(ResponseBuilderTest, RootMapping_RouteNotFoundAndNotExistingNeedle) {
	config->routeMapping["/src/default_folder/NOPE"] = {{"/coucou/les/amis/mdr/NOPE", "/tmp/www"}};
	responseBuilder->_realURI = "/src/default_folder/coucou/les/amis/mdr/image.jpeg";

	responseBuilder->rootMapping();

	EXPECT_EQ(responseBuilder->_realURI, "/src/default_folder/coucou/les/amis/mdr/image.jpeg");
}

// Tests with path imbricated
// OK TEST
TEST_F(ResponseBuilderTest, RootMapping_MultipleRoutesFirstCorrect) {
	config->routeMapping["/src/default_folder"] = {{"/coucou/les/amis/mdr", "/foo/var"}};
	config->routeMapping["/src/default_folder/temp"] = {{"/coucou/les/amis/mdr", "/tmp/www"}};
	responseBuilder->_realURI = "/src/default_folder/coucou/les/amis/mdr/image.jpeg";

	responseBuilder->rootMapping();

	EXPECT_EQ(responseBuilder->_realURI, "/foo/var/image.jpeg");
}

// OK TEST
TEST_F(ResponseBuilderTest, RootMapping_MultipleRoutesSecondCorrect) {
	config->routeMapping["/src/default_folder/temp"] = {{"/coucou/les/amis/mdr", "/tmp/www"}};
	config->routeMapping["/src/default_folder"] = {{"/coucou/les/amis/mdr", "/foo/var"}};
	responseBuilder->_realURI = "/src/default_folder/coucou/les/amis/mdr/image.jpeg";

	responseBuilder->rootMapping();

	EXPECT_EQ(responseBuilder->_realURI, "/foo/var/image.jpeg");
}

// OK TEST
TEST_F(ResponseBuilderTest, RootMapping_MultipleRoutes) {
	config->routeMapping["/src/default_folder"] = {{"/coucou/les/amis/mdr", "/foo/var"}};
	config->routeMapping["/src/default_folder/temp"] = {{"/coucou/les/amis/mdr", "/tmp/www"}};
	responseBuilder->_realURI = "/src/default_folder/temp/coucou/les/amis/mdr/image.jpeg";

	responseBuilder->rootMapping();

	EXPECT_EQ(responseBuilder->_realURI, "/tmp/www/image.jpeg");
}

TEST_F(ResponseBuilderTest, RootMapping_PartialPath) {
	config->routeMapping["/src/default_folder"] = {{"/coucou/les/amis/mdr", "/foo/var"}};
	config->routeMapping["/src/default_folder/temp"] = {{"/coucou/les/amis/mdr", "/tmp/www"}};
	responseBuilder->_realURI = "/src/coucou/les/amis/mdr/image.jpeg";

	responseBuilder->rootMapping();

	EXPECT_EQ(responseBuilder->_realURI, "/src/coucou/les/amis/mdr/image.jpeg");
}