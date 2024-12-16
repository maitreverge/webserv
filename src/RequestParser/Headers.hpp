/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TryHardTeam <TryHardTeam@123.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:17 by TryHardTeam       #+#    #+#             */
/*   Updated: 2024/12/16 12:09:18 by TryHardTeam      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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