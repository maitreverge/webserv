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

public:
	Cookies();

	CookiesMap _cookies;
	CookiesMap getCookies() const;
void	checkSessionCookie(Headers& _Headers, Server& server, std::string& URI, Client& client);
};

