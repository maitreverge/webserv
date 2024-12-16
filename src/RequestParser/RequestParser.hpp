/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TryHardTeam <TryHardTeam@123.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:14 by TryHardTeam       #+#    #+#             */
/*   Updated: 2024/12/16 12:09:15 by TryHardTeam      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <cstring>
#include <sstream>
#include "master.hpp"
#include "Headers.hpp"

typedef std::map<std::string, std::vector<std::string> > Headers_Map;
typedef std::map<std::string, std::string> CookiesMap;

struct Client;
class Server;

/**========================================================================
 *                           REQUESTPARSER
 * ? gets the request as a std::vector<char>
 * ? parses it and fills its attributes (including 'Headers' struct)
 *========================================================================**/
class RequestParser
{
	#ifdef UNIT_TEST
	public:
	#else
	private:
	#endif

		//attributes
		std::string											_method;
		std::string											_URI;
		std::string											_HTTP_version;
		bool												_isValid;
		Headers_Map											_tmpHeaders;
		Headers												_Headers;
		Client*												_Client;
		std::string											_WebToken;
		// utils
		std::string	charVectorToString(const std::vector<char>& vector);

		// action
		void	handleFirstLine(std::istringstream& requestStream);
		void	handleHeaderLines(std::istringstream& requestStream);
		void	extractHeaders();
		void	assignHeader(const std::string& key, std::string& value);
		void	assignHeader(const std::string& key, std::vector<std::string>& headerField);
		void	assignHeader(const std::string& key, size_t& headerField);
		void	assignHeader(const std::string& key, CookiesMap& headerField);
		CookiesMap	extractCookies(std::vector<std::string> vec);
		void	displayHeaders() const;
		void	reset_values();
		void	extractWebToken(const std::vector<std::string>& key);
		void 	displayUserSessionsContent(Client& client, Server & server);
	public:
		static void		trim(std::string& str);
		//coplien
		RequestParser();
		~RequestParser();
	
		// getters
		std::string	getMethod() const;
		std::string	getURI() const;
		void		setURI(std::string & uri);
		std::string	getHTTP_version() const;
		bool		getIsValid() const;
		Client*		getClient() const;
		Headers		getHeaders() const;
		Headers_Map	getTmpHeaders() const;
		std::string	getWebToken() const;

		// display methods
		void	displayParsingResult() const;
		void	displayAttributes() const;

		// action method
		void	parse(struct Client& client, Server & server);
};
