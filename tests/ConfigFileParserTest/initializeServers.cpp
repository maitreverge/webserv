#include <gtest/gtest.h>
#include "ConfigFileParserTest.hpp" // Assurez-vous que le chemin est correct

TEST_F(ConfigFileParserTest, InitializeServers) {
    // Arrange
    Config configStruct; // Créer une instance de Config
    int numberOfServers = 3; // Définir le nombre de serveurs à initialiser

    // Act
    configParser.initializeServers(configStruct, numberOfServers);

    // Assert
    EXPECT_EQ(configStruct.sockAddress.size(), numberOfServers + 1); // Vérifie que le bon nombre de serveurs a été ajouté

    // Vérifie que les adresses sont correctes
    for (int j = 0; j < numberOfServers + 1; ++j) {
        EXPECT_EQ(configStruct.sockAddress[j].sin_family, AF_INET);
        EXPECT_EQ(configStruct.sockAddress[j].sin_addr.s_addr, htonl(INADDR_ANY));
        EXPECT_EQ(configStruct.sockAddress[j].sin_port, htons(1510));
        std::cout << "server " << j + 1 << " initialized" << std::endl; // Pour voir les messages de log
    }
}
