#include "RequestParserTest.hpp"

// TEST_F(RequestParserTest, HandlesValidHeaders) {
//     std::istringstream validHeaders("Host: localhost\nUser-Agent: PostmanRuntime/7.28.4\nAccept: */*\n\n");
//     parser.handleHeaderLines(validHeaders);

//     // On vérifie si les en-têtes sont correctement analysés et stockés
//     EXPECT_EQ(parser.getHeader("Host"), "localhost");
//     EXPECT_EQ(parser.getHeader("User-Agent"), "PostmanRuntime/7.28.4");
//     EXPECT_EQ(parser.getHeader("Accept"), "*/*");
// }

// TEST_F(RequestParserTest, HandlesMalformedHeader) {
//     std::istringstream malformedHeader("Host localhost\nUser-Agent PostmanRuntime/7.28.4\nAccept */*\n\n");
//     EXPECT_THROW(parser.handleHeaderLines(malformedHeader), std::runtime_error);  // Exemple avec une exception
// }

// TEST_F(RequestParserTest, HandlesEndOfHeaders) {
//     std::istringstream emptyHeader("\n");
//     parser.handleHeaderLines(emptyHeader);

//     // Vérifie que la méthode reconnaît correctement la fin des en-têtes
//     EXPECT_TRUE(parser.headersAreEmpty());  // Remplacer par une méthode qui vérifie l'absence d'en-têtes si elle existe
// }

