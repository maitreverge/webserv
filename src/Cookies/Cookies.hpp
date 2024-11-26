#pragma once
#include <map>
#include <string>
#include <iostream>
#include "master.hpp"

typedef	std::map<std::string, std::string> CookiesMap;

struct SessionData
{
	std::string userId;
};

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
};

