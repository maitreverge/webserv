#include "Cookies.hpp"
#include "Server.hpp"
#include "Logger.hpp"

Cookies::Cookies()
{}

void	Cookies::checkSessionCookie(Headers& _Headers, Server& server, std::string URI)
{
	if (_Headers.Cookie.find("sessionID") != _Headers.Cookie.end())
	{
		Logger::getInstance().log(INFO, "cookie sessionID found.");
		std::string value = _Headers.Cookie.find("sessionID")->second;
		if (server.UserSessions.find(value) != server.UserSessions.end())
		{
			_cookies["sessionID"] = value;
			Logger::getInstance().log(INFO, "Active session found.");
		}
		else if (URI == "/connect?") //! this URI should be changed
		{
			SessionData sessionData = {value};
			server.UserSessions[value] = sessionData;
			Logger::getInstance().log(INFO, "Your sessionID token was added to the server.");
			_cookies["sessionID"] = value;
		}
		else
		{
			Logger::getInstance().log(INFO, "Your sessionID token is not valid.");
		}
	}
	else
		Logger::getInstance().log(INFO, "You are not connected.");
}
