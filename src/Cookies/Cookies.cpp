#include "Cookies.hpp"
#include "Server.hpp"
#include "Logger.hpp"

Cookies::Cookies()
{}

std::string extractUsername(const std::string& URI)
{
    size_t pos = URI.find("name=");
    if (pos != std::string::npos)
    {
        size_t start = pos + 5; // 5 = longueur de "username="
        size_t end = URI.find("&", start); // Recherche la fin du paramètre
        if (end == std::string::npos)
            end = URI.length();
        return URI.substr(start, end - start);
    }
    return ""; // Retourne une chaîne vide si username n'existe pas
}

void	Cookies::checkSessionCookie(Headers& _Headers, Server& server, std::string& URI, Client& client)
{
	if (_Headers.Cookie.find("sessionID") != _Headers.Cookie.end())
	{
		Logger::getInstance().log(INFO, "Cookie SessionID Found.", client, L_VRB);
		std::string value = _Headers.Cookie.find("sessionID")->second;
		if (server.UserSessions.find(value) != server.UserSessions.end())
		{
			_cookies["sessionID"] = value;
			Logger::getInstance().log(INFO, "Client Connected (Active session found).", client, L_VRB);
		}
		else if (URI.find("connect") != std::string::npos)
		{
			std::string userName = extractUsername(URI);
			SessionData sessionData = {value, userName};
			client._server->UserSessions[value] = sessionData;
			printColor(RED, "****************************************");
			printColor(RED, client._server->UserSessions[value].userName);
			printColor(RED, "****************************************");
			Logger::getInstance().log(INFO, "Client Connected (Client sessionID token added to server)", client, L_VRB);
			_cookies["sessionID"] = value;
			URI = "/connect";
		}
		else
			Logger::getInstance().log(INFO, "Client Not Connected: Client sessionID token not valid", client, L_VRB);
	}
	else if (client.conf->handleCookies)
		Logger::getInstance().log(INFO, "Client Not Connected", client, L_VRB);
}

CookiesMap Cookies::getCookies() const
{
	return _cookies;
}

