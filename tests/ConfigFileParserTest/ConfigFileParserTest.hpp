#include <gtest/gtest.h>
#include "ConfigFileParser.hpp" // Assurez-vous que le chemin est correct

class ConfigFileParserTest : public ::testing::Test {
protected:
    ConfigFileParser configParser;

    void SetUp() override {
        // Code à exécuter avant chaque test
		std::freopen("/dev/null", "w", stdout);
    }

    void TearDown() override {
        std::freopen("/dev/tty", "w", stdout);
        // Code à exécuter après chaque test
    }
    // Vous pouvez ajouter des méthodes ou des données supplémentaires si nécessaire
};
