#include "Cookies.hpp"
#include "Server.hpp"

Cookies::Cookies()
{}

void	Cookies::checkSessionCookie(Headers& _Headers, Server& server)
{
	if (_Headers.Cookie.find("sessionID") != _Headers.Cookie.end())
	{
		// check if value of sessionID is referenced in UserSessions
		print("cookie sessionID found!");
		std::string value = _Headers.Cookie.find("sessionID")->second;
		print("value: " + value);
		if (server.UserSessions.find(value) != server.UserSessions.end())
		{
			print("Active session found!");
		}
		else
		{
			// if no  => redirect to a login page
			printColor(RED, "Your sessionID token is not valid. Get a fuckin cookie!");
			SessionData sessionData = {"userID"};
			server.UserSessions[value] = sessionData;
		}
	}
	else
		// if no  => redirect to a login page
		printColor(BLUE, "You are not connected. Get a fuckin cookie!");
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
