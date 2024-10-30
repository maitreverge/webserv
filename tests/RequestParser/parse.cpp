#include "RequestParserTest.hpp"

// TEST_F(RequestParserTest, ParseValidRequest) {
//     // Créer un client fictif avec une requête HTTP basique
//     Client mockClient;
// 		mockClient.message = {'G', 'E', 'T', ' ', '/', ' ', 'H', 'T', 'T', 'P', '/', '1', '.', '1', '\r', '\n',
// 		'H', 'o', 's', 't', ':', ' ', 'l', 'o', 'c', 'a', 'l', 'h', 'o', 's', 't', '\r', '\n',
// 		'C', 'o', 'n', 'n', 'e', 'c', 't', 'i', 'o', 'n', ':', ' ', 'k', 'e', 'e', 'p', '-', 'a', 'l', 'i', 'v', 'e', '\r', '\n',
// 		'C', 'o', 'n', 't', 'e', 'n', 't', '-', 'T', 'y', 'p', 'e', ':', ' ', 't', 'e', 'x', 't', '/', 'h', 't', 'm', 'l', '\r', '\n',
// 		'A', 'c', 'c', 'e', 'p', 't', ':', ' ', '*', '/', '*', '\r', '\n',
// 		'C', 'o', 'n', 't', 'e', 'n', 't', '-', 'L', 'e', 'n', 'g', 't', 'h', ':', ' ', '0', '\r', '\n',
// 		'C', 'o', 'o', 'k', 'i', 'e', ':', ' ', 's', 'e', 's', 's', 'i', 'o', 'n', '=', '1', '2', '3', '4', '5', '\r', '\n',
// 		'\r', '\n'};


//     // Définir les valeurs attendues après parsing
//     std::string expectedMethod = "GET";
//     std::string expectedURI = "/";
//     std::string expectedHTTPVersion = "HTTP/1.1";
//     Headers expectedHeaders;
//     expectedHeaders.Host = "localhost";
//     expectedHeaders.Connection = "keep-alive";

//     // Appeler la méthode parse
//     parser.parse(mockClient);

//     // Vérifier les valeurs de la requête parsée
//     EXPECT_EQ(parser.getMethod(), expectedMethod);
//     EXPECT_EQ(parser.getURI(), expectedURI);
//     EXPECT_EQ(parser.getHTTP_version(), expectedHTTPVersion);
//     EXPECT_EQ(parser.getHeaders().Host, expectedHeaders.Host);
//     EXPECT_EQ(parser.getHeaders().Connection, expectedHeaders.Connection);

//     // Vérifier que le logger a été appelé (à condition d'avoir une méthode de vérification)
//     // Ici, on pourrait implémenter des vérifications pour Logger ou Error si des mocks sont disponibles
// }

