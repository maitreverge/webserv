#include <gtest/gtest.h>
#include "Error.hpp" // Assure-toi que le chemin d'accès est correct
#include "Client.hpp"

class ErrorTest : public ::testing::Test {
protected:
    Error error; // Instance de la classe à tester
    Client client; 
    // Optionnel : tu peux ajouter des méthodes de configuration ou de nettoyage ici
    void SetUp() override {
        // Code à exécuter avant chaque test
		std::freopen("/dev/null", "w", stdout);
    }

    void TearDown() override {
        std::freopen("/dev/tty", "w", stdout);
        // Code à exécuter après chaque test
    }
};
