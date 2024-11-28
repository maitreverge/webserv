#include "Cookies.hpp"
#include "Server.hpp"
#include "Logger.hpp"

Cookies::Cookies()
{}

/**========================================================================
 *!                            COMMENTS!!!
 *!  DOn't forget to replace WARNING flags by INFO!!!
 *  
 *  
 *  
 *========================================================================**/
void	Cookies::checkSessionCookie(Headers& _Headers, Server& server)
{
	if (_Headers.Cookie.find("sessionID") != _Headers.Cookie.end())
	{
		// check if value of sessionID is referenced in UserSessions
		Logger::getInstance().log(WARNING, "cookie sessionID found!");
		std::string value = _Headers.Cookie.find("sessionID")->second;
		print("value: " + value);
		if (server.UserSessions.find(value) != server.UserSessions.end())
		{
			_Headers.isConnected = true;
			_cookies["sessionID"] = value;
			Logger::getInstance().log(WARNING, "Active session found!");
		}
		else
		{
			// if no  => redirect to a login page
			Logger::getInstance().log(WARNING, "Your sessionID token is not valid.");
			
			/**============================================
			 *!               ACHTUNG!
			 *! for now, client is automatically connected.
			 *! front logic must be added to connect through a webpage 
			 *! => see with Flo
			 *! the code below must be executed in case of connection	
			 *! (to be handled through HTTP requests in custom connection page)
			 *=============================================**/
			printColor(RED, "");
			SessionData sessionData = {value};
			server.UserSessions[value] = sessionData;
			Logger::getInstance().log(WARNING, "Your sessionID token was added to the server.");
			_Headers.isConnected = true;
			_cookies["sessionID"] = value;
		}
	}
	else
		// if no  => redirect to a login page
		Logger::getInstance().log(INFO, "You are not connected.");
}

bool	Cookies::isConnected() const
{
	return true;
}

bool Cookies::hasSessionCookie() const
{
	return _cookies.find("session_id") != _cookies.end();
}

std::string Cookies::getCookieValue(const std::string& cookieName) const
{
	CookiesMap::const_iterator it = _cookies.find(cookieName);
	if (it != _cookies.end())
		return it->second;
	return "";
}

void Cookies::setCookie(const std::string& cookieName, const std::string& value)
{
	_cookies[cookieName] = value;
}

void Cookies::displayCookies() const
{
	CookiesMap::const_iterator it;
	print("Cookies:");
	for (it = _cookies.begin(); it != _cookies.end(); ++it)
		std::cout << "  " << it->first << ": " << it->second << std::endl;
}

void Cookies::deleteCookie(const std::string& cookieName)
{
	_cookies.erase(cookieName);
}

CookiesMap&	Cookies::getCookies()
{
	return (_cookies);
}


void Cookies::parseCookies(const std::string& cookieHeader)
{
	size_t start = 0, end;
	while ((end = cookieHeader.find(';', start)) != std::string::npos)
	{
		std::string cookie = cookieHeader.substr(start, end - start);
		size_t separator = cookie.find('=');
		if (separator != std::string::npos)
		{
			std::string key = cookie.substr(0, separator);
			std::string value = cookie.substr(separator + 1);
			_cookies[trim(key)] = trim(value);
		}
		start = end + 1;
	}
	if (start < cookieHeader.length()) {
		std::string cookie = cookieHeader.substr(start);
		size_t separator = cookie.find('=');
		if (separator != std::string::npos)
		{
			std::string key = cookie.substr(0, separator);
			std::string value = cookie.substr(separator + 1);
			_cookies[trim(key)] = trim(value);
		}
	}
}

std::string Cookies::trim(const std::string& str)
{
	size_t first = str.find_first_not_of(" \t");
	size_t last = str.find_last_not_of(" \t");
	return str.substr(first, (last - first + 1));
}

/**========================================================================
 *?                           To be added to Client Class?
 *========================================================================**/
// void Client::setSessionCookie(const std::string& sessionId) {
//     cookies.setCookie("session_id", sessionId);
// }

// void Client::appendCookiesToResponse(Response& response) {
//     for (const auto& cookie : cookies.getCookies()) {
//         response.addHeader("Set-Cookie", cookies.generateSetCookieHeader(cookie.first, cookie.second));
//     }
// }
