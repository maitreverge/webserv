#pragma once
#include <map>
#include <string>
#include <iostream>
#include "master.hpp"
#include "RequestParser.hpp"

typedef	std::map<std::string, std::string> CookiesMap;

class Cookies
{
private:
	CookiesMap _cookies;

public:
	Cookies();

	bool hasSessionCookie() const;
	std::string getCookieValue(const std::string& cookieName) const;
	void setCookie(const std::string& cookieName, const std::string& value);
	void displayCookies() const;
	void deleteCookie(const std::string& cookieName);
	CookiesMap&	getCookies();
	void parseCookies(const std::string& cookieHeader);
	std::string trim(const std::string& str);
	void	checkSessionCookie(Headers& _Headers, Server& server);
};

