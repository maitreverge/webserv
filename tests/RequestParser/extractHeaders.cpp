#include "RequestParserTest.hpp"

// void RequestParser::assignHeader(const std::string& headerName, std::string& headerValue) {
//     if (_tmpHeaders.find(headerName) != _tmpHeaders.end()) {
//         headerValue = _tmpHeaders[headerName][0];  // supposons que le premier élément soit utilisé
//     }
// }

// void RequestParser::assignHeader(const std::string& headerName, std::vector<std::string>& headerValue) {
//     if (_tmpHeaders.find(headerName) != _tmpHeaders.end()) {
//         headerValue = _tmpHeaders[headerName];
//     }
// }

// void RequestParser::assignHeader(const std::string& headerName, size_t& headerValue) {
//     if (_tmpHeaders.find(headerName) != _tmpHeaders.end()) {
//         headerValue = std::stoul(_tmpHeaders[headerName][0]);  // conversion de string en size_t
//     }
// }

// void RequestParser::assignHeader(const std::string& headerName, std::map<std::string, std::string>& headerValue) {
//     // Logique d'extraction pour les cookies ici si nécessaire
//     // Exemple simple : si "Cookie" contient plusieurs paires clé=valeur séparées par ";"
//     if (_tmpHeaders.find(headerName) != _tmpHeaders.end()) {
//         std::istringstream cookieStream(_tmpHeaders[headerName][0]);
//         std::string cookiePart;
//         while (std::getline(cookieStream, cookiePart, ';')) {
//             size_t eqPos = cookiePart.find('=');
//             if (eqPos != std::string::npos) {
//                 std::string key = cookiePart.substr(0, eqPos);
//                 std::string value = cookiePart.substr(eqPos + 1);
//                 headerValue[key] = value;
//             }
//         }
//     }
// }
