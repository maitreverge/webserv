#include <gtest/gtest.h>
#include "ResponseBuilder.hpp" // Inclure le fichier de la classe à tester
#include "Server.hpp"
#include "Client.hpp"
class ResponseBuilderTest : public ::testing::Test {
protected:
    // Objets utilisés pour les tests
    Client* client;
    Config* config;
    ResponseBuilder* responseBuilder;
    
	// Cette méthode est appelée avant chaque test
    void SetUp() override {
        // Initialisation des objets nécessaires pour les tests
		// mockConfig = new MockConfig();
		responseBuilder = new ResponseBuilder();
        client = new Client();
        config = new Config();
		responseBuilder->_config = config;
		// std::freopen("/dev/null", "w", stdout);
    }

    // Cette méthode est appelée après chaque test
    void TearDown() override {
        // Libération des ressources
        delete client;
        delete config;
        delete responseBuilder;
		// delete mockConfig;
        // std::freopen("/dev/tty", "w", stdout);
    }

};

