#include "ErrorTest.hpp"


TEST_F(ErrorTest, HandleError_ValidErrorCode_UpdatesClientStatusCode) {
    int testErrorCode = 404; // Code d'erreur à tester
    client.statusCode = static_cast<e_errorCodes>(0); // Valeur initiale

    error.handleError(testErrorCode, client);

    // Vérifie que le statusCode du client a été mis à jour
    EXPECT_EQ(client.statusCode, static_cast<e_errorCodes>(testErrorCode));
}

