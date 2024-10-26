#include <gtest/gtest.h>
#include "Error.hpp" // Assure-toi que le chemin d'accès est correct
#include "Server.hpp"

class ErrorTest : public ::testing::Test {
protected:
    Error error; // Instance de la classe à tester
    Client client; 
    // Optionnel : tu peux ajouter des méthodes de configuration ou de nettoyage ici
    void SetUp() override {
        // Code à exécuter avant chaque test
    }

    void TearDown() override {
        // Code à exécuter après chaque test
    }
};
