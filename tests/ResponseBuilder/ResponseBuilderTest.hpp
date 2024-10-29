#include <gtest/gtest.h>
#include "ResponseBuilder.hpp" // Inclure le fichier de la classe à tester
#include "Server.hpp"

class MockConfig {
public:
	std::map<std::string, std::map<std::string, std::string>> routeMapping;
	MockConfig( void ){};
};

class ResponseBuilderTest : public ::testing::Test {
protected:
	
	MockConfig* mockConfig;
    // Cette méthode est appelée avant chaque test
    void SetUp() override {
        // Initialisation des objets nécessaires pour les tests
		mockConfig = new MockConfig();
        client = new Client();
        config = new Config();
        responseBuilder = new ResponseBuilder();
		// std::freopen("/dev/null", "w", stdout);
    }

    // Cette méthode est appelée après chaque test
    void TearDown() override {
        // Libération des ressources
        delete client;
        delete config;
        delete responseBuilder;
		delete mockConfig;
        // std::freopen("/dev/tty", "w", stdout);
    }

    // Objets utilisés pour les tests
    Client* client;
    Config* config;
    ResponseBuilder* responseBuilder;
};

