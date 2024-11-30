#include "ResponseBuilder.hpp"
#include "Logger.hpp" 
#include <ctime>
#include <sstream>
#include <string>
#include <iostream>

void	ResponseBuilder::checkSessionIdCookie(Client &inputClient)
{
	if (inputClient.conf->handleCookies && inputClient.isConnected() == false)
		throw Server::ShortCircuitException(CODE_242_CONNECTION);
}


std::string ResponseBuilder::generateUniqueToken(const std::string& clientIP)
{
	// Obtenir le timestamp actuel
	std::time_t currentTime = std::time(NULL);
	
	// Construire une chaîne avec IP + timestamp
	std::stringstream ss;
	ss << clientIP << "_" << currentTime;

	// Une fonction de hachage rudimentaire pour simuler l'unicité
	unsigned int hash = 0;
	std::string baseString = ss.str();
	for (size_t i = 0; i < baseString.size(); ++i)
	{
		hash = (hash * 31) + static_cast<unsigned int>(baseString[i]);
	}

	// Retourner le token unique
	std::stringstream token;
	token << hash;
	return token.str();
}

void	ResponseBuilder::buildSetCookieHeader()
{
	string clientIP = Logger::ipToString(_client->address);
	if (_client->isConnected() == false && _client->conf->handleCookies)
	{
		std::string token = generateUniqueToken(clientIP);
		stringstream streamCookie;
		streamCookie	<< "Set-Cookie:"
						<< SPACE
						<< "sessionID=" << token
						<< HTTP_HEADER_SEPARATOR;
		Headers.cookie = streamCookie.str();
		printColorNoEndl(CYAN, "Unique Token generated: ");
		printColor(CYAN, token);
	}

}