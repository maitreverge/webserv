/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorNotFoundGenerator.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TryHardTeam <TryHardTeam@123.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:03 by TryHardTeam       #+#    #+#             */
/*   Updated: 2024/12/16 12:09:04 by TryHardTeam      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseBuilder.hpp"
#include "Logger.hpp"
#include <stdlib.h>

/**
 * @brief Generates a custom error page for "Not Found" errors.
 *
 * This function constructs an HTML page to be used as a backup when a requested
 * resource is not found. It includes a predefined image and a message indicating
 * the error. The generated HTML is saved to a file specified by `_backupNameFile`.
 *
 * The function also updates `_realURI` to point to the backup file and sets `_deleteURI`
 * to true, indicating that the URI should be deleted.
 *
 * @note This function logs its execution at the DEBUG level.
 */
void	ResponseBuilder::errorNotFoundGenerator( void ){

	Logger::getInstance().log(DEBUG, "FUNCTION CALL : ResponseBuilder::errorNotFoundGenerator");

	errorCode codes;
	
	stringstream result;
	stringstream body;
	stringstream image;

	string imageURL = "https://freeyork.org/wp-content/uploads/2015/06/DKOpAT4VAAEmmes.jpg";

	string widthImage = "40";
	
	string errorName = codes.getCode(_errorType);

	u_int16_t errorNumber = static_cast<u_int16_t>(_errorType);

	// Body text
	body	<< "The "
			<< errorNumber
			<< ".html file has not been found."
			<< "This is a backup self-generated webpage.";
	
	// Image source
	image   << "<img src=\""
			<< imageURL
			<< "\" alt=\"Description of image\" style=\"width:"
			<< widthImage
			<< "%;\">";

	// Assemble all parts
	result	<< "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "<head>\n"
			<< "<title>"
			<< errorNumber
			<< "\t"
			<< errorName
			<< "</title>\n"
			<< "</head>\n"
			<< "<body>\n"
			<< "<h1>"
			<< body.str()
			<< "</h1>\n"
			<< "<h1>"
			<< image.str()
			<< "</h1>\n"
			<< "</body>\n"
			<< "</html>";

	ofstream backupStream(_backupNameFile.c_str());

	if (backupStream.is_open())
	{
		// Write to the stream
		backupStream << result.str();
		
		// Check for write errors
		if (backupStream.fail())
		{
			Logger::getInstance().log(ERROR, "ResponseBuilder::errorNotFoundGenerator : backupStream failed to write data");
			throw Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR);
		}
		else
			Logger::getInstance().log(DEBUG, "ResponseBuilder::errorNotFoundGenerator : backupStream correctly wrote data");
		
		// ! Modify the _realURI
		this->_realURI = _backupNameFile;

		this->_deleteURI = true;
		Logger::getInstance().log(DEBUG, "ResponseBuilder::errorNotFoundGenerator : The _realURI will be deleted");
		
	}
	else 
	{
		Logger::getInstance().log(ERROR, "ResponseBuilder::errorNotFoundGenerator : backupStream failed to open");
		throw Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR);
	}

	backupStream.close();
	if (backupStream.fail())
	{
		Logger::getInstance().log(ERROR, "ResponseBuilder::errorNotFoundGenerator : backupStream failed to close");
		throw Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR);
	}
}