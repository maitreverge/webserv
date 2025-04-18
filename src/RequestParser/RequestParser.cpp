/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TryHardTeam <TryHardTeam@123.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:16 by TryHardTeam       #+#    #+#             */
/*   Updated: 2024/12/16 12:09:17 by TryHardTeam      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"
#include "Logger.hpp"

/**========================================================================
 *                  CONSTRUCTORS / DESTRUCTOR / INIT
 *========================================================================**/
RequestParser::RequestParser() : 	_method(""),
									_URI(""),
									_HTTP_version(""),
									_isValid(true),
									_Client(NULL),
									_WebToken("") // ! FLO
{
	_Headers.reset();
}

RequestParser::~RequestParser() {}

/**========================================================================
 *                           GETTERS AND SETTER
 *========================================================================**/
std::string	RequestParser::getMethod() const 		{return (_method);}

std::string	RequestParser::getURI() const 			{return (_URI);}

void	RequestParser::setURI(std::string & uri)	{_URI = uri;}

std::string	RequestParser::getHTTP_version() const	{return (_HTTP_version);}

bool	RequestParser::getIsValid() const			{return (_isValid);}

Headers	RequestParser::getHeaders() const			{return (_Headers);}

Client*		RequestParser::getClient() const		{return (_Client);}

Headers_Map		RequestParser::getTmpHeaders() const
{
	return (_tmpHeaders);
}

std::string	RequestParser::getWebToken() const		{return (_WebToken);}

/**========================================================================
 *                           UTILS
 *========================================================================**/
std::string	RequestParser::charVectorToString(const std::vector<char>& vector)
{
	return (std::string(vector.begin(), vector.end()));
}

void	RequestParser::trim(std::string& str)
{
	str.erase(0, str.find_first_not_of(" \t\r\n"));
	str.erase(str.find_last_not_of(" \t\r\n") + 1);
}
/**========================================================================
 *                           MAIN ACTION
 * parse() is called from outside the class
 * first, intern value are reset
 * then values are extracted from the first line
 * then headers are extracted form the headers (following lines)
 * Everything after '\r\n\r\n' (end of header) is IGNORED
 *========================================================================**/
void	RequestParser::parse(Client& client, Server & server)
{
	reset_values();
	std::istringstream requestStream(charVectorToString(client.messageRecv));
	Logger::getInstance().log(INFO, "Request Parsing Started", *this, L_VRB);
	handleFirstLine(requestStream);
	handleHeaderLines(requestStream);
	extractHeaders();
	client.cookies.checkSessionCookie(_Headers, server, _URI, client);
}

/**========================================================================
 *                           INIT METHODS
 *========================================================================**/
void Headers::reset()
{
	Connection = "";
	ContentType = "";
	Host = "";
	Accept = std::vector<std::string>();
	ContentLength = 0;
	Cookie = CookiesMap();
	
}

void	RequestParser::reset_values()
{
	_method = "";
	_URI = "";
	_HTTP_version = "";
	_isValid = true;
	_tmpHeaders.clear();
	_Headers.reset();
}

/**========================================================================
 *                           HANDLE LINES METHODS
 *========================================================================**/
void	RequestParser::handleFirstLine(std::istringstream& requestStream)
{
	std::string firstLine;
	std::string remainingData;

	if (requestStream.str().empty())
		_isValid = false;
	if (std::getline(requestStream, firstLine))
	{
		std::istringstream iss(firstLine);
		iss >> _method >> _URI >> _HTTP_version;
		if (iss >> 	remainingData || _method.empty() || _URI.empty() || _HTTP_version.empty())
			_isValid = false;
		if (_HTTP_version != "HTTP/1.1")
		{
			_isValid = false;
			throw Server::ShortCircuitException(CODE_505_HTTP_VERSION_NOT_SUPPORTED);
		}
		if (_method != "GET" && _method != "POST" && _method != "DELETE")
		{
			_isValid = false;
			throw Server::ShortCircuitException(CODE_501_NOT_IMPLEMENTED);
		}
	}
	if (_isValid == 0)
		Logger::getInstance().log(ERROR, "request first line wrong", *this);
}

void	RequestParser::handleHeaderLines(std::istringstream& requestStream)
{
	std::string headerLine;
	while (std::getline(requestStream, headerLine) && !headerLine.empty())
	{
		size_t colonPos = headerLine.find(':');
		if (colonPos != std::string::npos)
		{
			std::string key = headerLine.substr(0, colonPos);
			std::string value = headerLine.substr(colonPos + 1);
			trim(key); trim(value);
			std::istringstream valueStream(value);
			std::string singleValue;
			while (std::getline(valueStream, singleValue, ','))
			{
				trim(singleValue);
				_tmpHeaders[key].push_back(singleValue);
			}
		}
	}
}

/**========================================================================
 *                           EXTRACTHEADERS
 * headers first put in map<string, vector<string> > _tmpHeaders
 * then asigned accoriding to their type (see assignHeader overloads)
 *========================================================================**/
void	RequestParser::extractHeaders()
{
	assignHeader("Host", _Headers.Host);
	assignHeader("Connection", _Headers.Connection);
	assignHeader("Content-Type", _Headers.ContentType);
	assignHeader("Accept", _Headers.Accept);
	assignHeader("Content-Length", _Headers.ContentLength);
	assignHeader("Cookie", _Headers.Cookie);
	assignHeader("Transfer-Encoding", _Headers.TransferEncoding);
}

/**========================================================================
 *                           ASSIGNHEADERS OVERLOADS
 *========================================================================**/
void RequestParser::extractWebToken(const std::vector<std::string>& key){

	string target("");
	string needle = "boundary=";

	for (std::vector<std::string>::const_iterator it = key.begin(); it != key.end(); ++it)
	{
		if (it->find(needle) != std::string::npos)
		{
			target = *it;
			break;
		}
	}
	if (target.empty())
		return;
	size_t startPos = target.find(needle);
	size_t endPos;
	startPos += needle.length();
	endPos = target.find(" ", startPos);
	_WebToken = target.substr(startPos, endPos - startPos);
}

void RequestParser::assignHeader(const std::string& key, std::string& headerField)
{
	Headers_Map::const_iterator it = _tmpHeaders.find(key);

	if (it != _tmpHeaders.end())
	{
		if (!it->second.empty())
		{
			headerField = it->second[0];
			if (key == "Content-Type")
			{
				extractWebToken(it->second);
			}
		}
		else
			Logger::getInstance().log(WARNING, "Header found but has no values for key: " + key, *this);
	}
}

void	RequestParser::assignHeader(const std::string& key, std::vector<std::string>& headerField)
{
	Headers_Map::const_iterator it = _tmpHeaders.find(key);

	if (it != _tmpHeaders.end() && !it->second.empty())
	{
		for (std::vector<std::string>::const_iterator valueIt = it->second.begin();
			valueIt != it->second.end(); ++valueIt)
		{
			bool exists = false;
			for (std::vector<std::string>::const_iterator it = headerField.begin();
				it != headerField.end(); it++)
				if (*it == *valueIt)
					exists = true;
			if (!exists)
				headerField.push_back(*valueIt);
		}
	}
}

void	RequestParser::assignHeader(const std::string& key, size_t& headerField)
{
	Headers_Map::const_iterator it = _tmpHeaders.find(key);
	if (it != _tmpHeaders.end() && !it->second.empty())
		headerField = static_cast<size_t>(atoi(it->second[0].c_str()));
}

void	RequestParser::assignHeader(const std::string& key, CookiesMap& headerField)
{
	Headers_Map::const_iterator it = _tmpHeaders.find(key);
	if (it != _tmpHeaders.end() && !it->second.empty())
		headerField = extractCookies(it->second);
}

/**========================================================================
 *                           EXTRACTCOOKIES
 *========================================================================**/
CookiesMap RequestParser::extractCookies(std::vector<std::string> vec)
{
	CookiesMap cookiesMap;

	for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it)
	{
		std::string cookieLine = *it;
		size_t start = 0;
		size_t end;
		while ((end = cookieLine.find(';', start)) != std::string::npos)
		{
			std::string cookiePair = cookieLine.substr(start, end - start);
			size_t equalPos = cookiePair.find('=');
			if (equalPos != std::string::npos)
			{
				std::string key = cookiePair.substr(0, equalPos);
				std::string value = cookiePair.substr(equalPos + 1);
				trim(key);
				trim(value);
				cookiesMap[key] = value;
			}
			start = end + 1;
		}
		std::string cookiePair = cookieLine.substr(start);
		size_t equalPos = cookiePair.find('=');
		if (equalPos != std::string::npos) {
			std::string key = cookiePair.substr(0, equalPos);
			std::string value = cookiePair.substr(equalPos + 1);
			trim(key);
			trim(value);
			cookiesMap[key] = value;
		}
	}
	return cookiesMap;
}

/**========================================================================
 *                           DISPLAY
 * only for dev purposes. 
 * May be commented once project is tested and functional
 *========================================================================**/
void RequestParser::displayUserSessionsContent(Client& client, Server & server)
{
	std::map<std::string, SessionData>& userSessions = server.UserSessions;
	if (userSessions.empty())
	{
		std::cout << "Aucune session active." << std::endl;
		return;
	}
	for (std::map<std::string, SessionData>::iterator it = userSessions.begin(); it != userSessions.end(); ++it)
	{
		const std::string& sessionId = it->first;
		const SessionData& sessionData = it->second;
		std::cout << "Session ID: " << sessionId << std::endl;
		std::cout << "  User ID: " << sessionData.userId << std::endl;
	}
	(void)client;
}


void RequestParser::displayParsingResult() const
{
	displayAttributes();
	displayHeaders();
}

void RequestParser::displayAttributes() const
{
	print("");
	print("------ ATTRIBUTES ------");
	print("Method: " + _method);
	print("URI: " + _URI);
	print("HTTP_VERSION: " + _HTTP_version);
	printNoEndl("_isValid: "); print(_isValid);
}

void		RequestParser::displayHeaders() const
{	
	print("-------- HEADERS -------");
	if (_Headers.Connection.length())
		print("Connection: " + _Headers.Connection);
	if (_Headers.ContentType.length())
		print("ContentType: " + _Headers.ContentType);
	if (_Headers.Host.length())
		print("Host: " + _Headers.Host);
	if (_Headers.TransferEncoding.length())
		print("TransferEncoding: " + _Headers.TransferEncoding);
	if (_Headers.Accept.size())
	{
		print("Accept: ");
		for (size_t i = 0; i < _Headers.Accept.size(); i++)
			print("	" + _Headers.Accept[i]);
	}
	std::cout << "ContentLength: " << _Headers.ContentLength << std::endl; 
	if (_Headers.Cookie.size())
	{
		CookiesMap::const_iterator it = _Headers.Cookie.begin();
		print("Cookie: ");
		for (; it != _Headers.Cookie.end(); it++)
			print("	" + it->first + " => " + it->second);
	}
	print("");
}
