#include "Cookies.hpp"
#include "Server.hpp"
#include "Logger.hpp"

Cookies::Cookies()
{}

void	Cookies::checkSessionCookie(Headers& _Headers, Server& server, std::string URI, Client& client)
{
	if (_Headers.Cookie.find("sessionID") != _Headers.Cookie.end())
	{
		Logger::getInstance().log(INFO, "Cookie sessionID found.", client);
		std::string value = _Headers.Cookie.find("sessionID")->second;
		if (server.UserSessions.find(value) != server.UserSessions.end())
		{
			_cookies["sessionID"] = value;
			Logger::getInstance().log(INFO, "Client connected (Active session found).", client);
		}
		else if (URI == "/connect")
		{
			SessionData sessionData = {value};
			server.UserSessions[value] = sessionData;
			Logger::getInstance().log(INFO, "Client connected (Client sessionID token added to server)", client);
			_cookies["sessionID"] = value;
		}
		else
			Logger::getInstance().log(INFO, "Client not connected: Client sessionID token not valid", client);
	}
	else if (client.conf->handleCookies)
		Logger::getInstance().log(INFO, "Client not connected", client);
}

CookiesMap Cookies::getCookies() const
{
	return _cookies;
}
