#include "Cookies.hpp"
#include "Server.hpp"
#include "Logger.hpp"

Cookies::Cookies()
{}

void	Cookies::checkSessionCookie(Headers& _Headers, Server& server, std::string URI, Client& client)
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
		else if (URI == "/connect")
		{
			SessionData sessionData = {value};
			server.UserSessions[value] = sessionData;
			Logger::getInstance().log(INFO, "Client Connected (Client sessionID token added to server)", client, L_VRB);
			_cookies["sessionID"] = value;
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
