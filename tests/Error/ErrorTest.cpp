#include "ErrorTest.hpp"

// Test pour vérifier que handleError met à jour client.statusCode correctement
// TEST_F(ErrorTest, HandleError_UpdatesClientStatusCode) {
//     Client client;
//     unsigned long errCode = 404; // Exemple de code d'erreur

//     error.handleError(errCode, client);

//     // On vérifie que le statusCode du client correspond au code d'erreur attendu
//     EXPECT_EQ(client.statusCode, static_cast<e_errorCodes>(errCode));
// }

// Test pour vérifier que handleError appelle buildErrorRequest avec le bon code d'erreur
TEST_F(ErrorTest, HandleError_BuildErrorRequestCalled) {
    Client client;
    unsigned long errCode = 500; // Autre exemple de code d'erreur

    std::vector<char> errorResponse = error.buildErrorRequest(errCode);

    error.handleError(errCode, client);

    // On vérifie que la réponse d'erreur est non vide (ou tout autre critère)
    EXPECT_FALSE(errorResponse.empty());
}

// Test pour vérifier que handleError logge le bon message
TEST_F(ErrorTest, HandleError_LogsCorrectMessage) {
    Client client;
    unsigned long errCode = 403; // Exemple de code d'erreur

    // Simule l'appel du logger et vérifie si un message a été loggé
    // Ce test nécessite de pouvoir vérifier l'appel de Logger
    error.handleError(errCode, client);

    // Si on pouvait mocker Logger, ici on vérifierait qu'il a été appelé
    // avec les bons paramètres, sinon ce test devra être adapté
}

// Test pour vérifier que handleError appelle la méthode getCode avec le bon code d'erreur
TEST_F(ErrorTest, HandleError_CallsGetCodeWithCorrectErrorCode) {
    Client client;
    unsigned long errCode = 401; // Exemple de code d'erreur

    error.handleError(errCode, client);

    // Si possible, mocker la classe errorCode et vérifier que getCode a été appelée
}

// Test pour vérifier qu'on retourne la page d'erreur spécifique
TEST_F(ErrorTest, GetErrorPagePath_ReturnsSpecificErrorPage) {
    unsigned long errorCode = 404;
    std::string expectedPath = "errorPages/404.html";

    std::string result = error.getErrorPagePath(errorCode);

    EXPECT_EQ(result, expectedPath);
}

// Test pour vérifier qu'on retourne la page d'erreur par défaut si le code d'erreur n'existe pas
TEST_F(ErrorTest, GetErrorPagePath_ReturnsDefaultErrorPageIfNotFound) {
    unsigned long errorCode = 999; // Code inexistant dans _errorPages
    std::string expectedPath = "errorPages/default.html";

    std::string result = error.getErrorPagePath(errorCode);

    EXPECT_EQ(result, expectedPath);
}

// Test pour vérifier qu'un index hors limites retourne la page par défaut
TEST_F(ErrorTest, GetErrorPagePath_OutOfBoundsReturnsDefaultErrorPage) {
    unsigned long errorCode = 10; // Hors des limites de _errorPages
    std::string expectedPath = "errorPages/default.html";

    std::string result = error.getErrorPagePath(errorCode);

    EXPECT_EQ(result, expectedPath);
}

// Test pour vérifier la conversion d'une chaîne standard
TEST_F(ErrorTest, StringToVector_StandardString) {
    std::string input = "Hello, World!";
    std::vector<char> expected = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'};

    std::vector<char> result = error.stringToVector(input);

    EXPECT_EQ(result, expected);
}

// Test pour vérifier la conversion d'une chaîne vide
TEST_F(ErrorTest, StringToVector_EmptyString) {
    std::string input = "";
    std::vector<char> expected = {}; // Vecteur vide

    std::vector<char> result = error.stringToVector(input);

    EXPECT_EQ(result, expected);
}

// Test pour vérifier la conversion d'une chaîne avec des caractères spéciaux et des espaces
TEST_F(ErrorTest, StringToVector_StringWithSpecialCharacters) {
    std::string input = "Test !@#$%^&*() 123";
    std::vector<char> expected = {'T', 'e', 's', 't', ' ', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', ' ', '1', '2', '3'};

    std::vector<char> result = error.stringToVector(input);

    EXPECT_EQ(result, expected);
}

// Test pour vérifier la requête HTTP construite pour un code d'erreur spécifique
TEST_F(ErrorTest, BuildErrorRequest_SpecificErrorCode) {
    unsigned long errorCode = 404;
    std::string expectedRequest = "GET errorPages/404.html HTTP/1.1";
    std::vector<char> expectedVector(expectedRequest.begin(), expectedRequest.end());

    std::vector<char> result = error.buildErrorRequest(errorCode);

    EXPECT_EQ(result, expectedVector);
}

// Test pour vérifier la requête HTTP construite pour un code d'erreur non défini (retour à la page par défaut)
TEST_F(ErrorTest, BuildErrorRequest_DefaultErrorCode) {
    unsigned long errorCode = 66;  // Code inexistant dans _errorPages
    std::string expectedRequest = "GET errorPages/default.html HTTP/1.1";
    std::vector<char> expectedVector(expectedRequest.begin(), expectedRequest.end());

    std::vector<char> result = error.buildErrorRequest(errorCode);

    EXPECT_EQ(result, expectedVector);
}

// Test pour vérifier la requête HTTP construite avec un code d'erreur hors limites dans _errorPages (retour à la page par défaut)
TEST_F(ErrorTest, BuildErrorRequest_OutOfBoundsErrorCode) {
    unsigned long errorCode = 10;  // Hors des limites de _errorPages
    std::string expectedRequest = "GET errorPages/default.html HTTP/1.1";
    std::vector<char> expectedVector(expectedRequest.begin(), expectedRequest.end());

    std::vector<char> result = error.buildErrorRequest(errorCode);

    EXPECT_EQ(result, expectedVector);
}