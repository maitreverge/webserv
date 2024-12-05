#pragma once

typedef std::map<std::string, std::string> CookiesMap;

struct Headers
{
	std::string							Connection;
	std::string							ContentType;
	std::string							Host;
	std::string							TransferEncoding;
	std::vector<std::string>			Accept;
	size_t								ContentLength;
	CookiesMap							Cookie;
	
	void	reset();
	bool operator==(const Headers& other) const
	{
	return Connection == other.Connection &&
			ContentType == other.ContentType &&
			Host == other.Host &&
			ContentLength == other.ContentLength &&
			Cookie == other.Cookie &&
			std::equal(Accept.begin(), Accept.end(), other.Accept.begin());
	}
};