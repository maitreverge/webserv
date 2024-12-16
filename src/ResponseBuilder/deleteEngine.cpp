/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteEngine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TryHardTeam <TryHardTeam@123.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:05 by TryHardTeam       #+#    #+#             */
/*   Updated: 2024/12/16 12:09:06 by TryHardTeam      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseBuilder.hpp"
#include "Logger.hpp"
#include <cstdio>

/**
 * @brief Generates an HTML response for a successful DELETE operation.
 * 
 * This function constructs an HTML page indicating that a resource has been successfully deleted.
 * The generated HTML includes a message with the name of the deleted resource, a decorative image,
 * and a link to return to the home page.
 * 
 * @param result A stringstream object where the generated HTML content will be appended.
 */
void ResponseBuilder::makeDeleteHTML(std::stringstream &result) {
	
	result << "<!DOCTYPE html>"
		   << "<html lang=\"en\">"
		   << "<head>"
		   << "<meta charset=\"UTF-8\">"
		   << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
		   << "<title>DELETE SUCCESSFULL</title>"
		   << "<style>"
		   << "body {"
		   << "font-family: Arial, sans-serif;"
		   << "background: linear-gradient(to right, #6b7280, #f05656, #4ade80);"
		   << "text-align: center;"
		   << "padding: 40px;"
		   << "}"
		   << ".soft-frame {"
		   << "border-radius: 15px;"
		   << "box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);"
		   << "display: block;"
		   << "margin-left: auto;"
		   << "margin-right: auto;"
		   << "}"
		   << ".homepage {"
		   << "display: inline-block;"
		   << "font-size: 21px;"
		   << "padding: 10px 20px;"
		   << "background-color: #8da6ec;"
		   << "color: white;"
		   << "text-decoration: none;"
		   << "border-radius: 5px;"
		   << "transition: background-color 0.3s ease, transform 0.3s ease;"
		   << "}"
		   << ".homepage:hover {"
		   << "background-color: #0300b2;"
		   << "transform: scale(1.2) rotate(5deg);"
		   << "}"
		   << "</style>"
		   << "</head>"
		   << "<body>"
		   << "<h1>🗑️ THE RESSOURCE <i><b>"
		   << _fileName
		   << "</b></i> HAS BEEN DELETED 🗑️</h1>"
		   << "<img src=\"https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fi.redd.it%2F6856jz1ig2e11.jpg&f=1&nofb=1&ipt=ef599cf975882e5cdff3967f984a20be9a45ddb682d78357109753fc3c14717d&ipo=images\""
		   << "alt=\"cat\" class=\"soft-frame\" style=\"width:500px;height:auto;\">"
		   << "<br>"
		   << "<a href=\"/\" class=\"homepage\">🏠 BACK TO HOME PAGE</a>"
		   << "</body>"
		   << "</html>";
}

/**
 * @brief Generates an HTML page indicating that a resource has been deleted.
 *
 * This function constructs an HTML document that informs the user that a specified file
 * has been successfully deleted.
 * The generated HTML is saved to a file named "delete.html" in the directory specified
 * by the _realURI member variable.
 * The function also updates the _realURI to point to the newly created "delete.html" file
 * and sets the _deleteURI flag to true.
 */
void	ResponseBuilder::generateDeleteHTML( void ){

	Logger::getInstance().log(DEBUG, "FUNCTION CALL : ResponseBuilder::generateDeleteHTML");
	
	// Building buffer
	stringstream result;

	string deleteFileName = "delete.html";

	makeDeleteHTML(result);

	// Modify the URI to be targeted by the size afterwards
	_realURI.erase(_realURI.find_last_of("/") + 1);
	_realURI += deleteFileName;

	ofstream deleteFileStream(_realURI.c_str());

	if (deleteFileStream.is_open())
	{
		// Write to the stream
		deleteFileStream << result.str();
		
		// Check for write errors
		if (deleteFileStream.fail())
		{
			Logger::getInstance().log(ERROR, "ResponseBuilder::generateDeleteHTML : deleteFileStream failed to write data");
			throw Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR);
		}
		else
			Logger::getInstance().log(DEBUG, "ResponseBuilder::generateDeleteHTML : deleteFileStream correctly wrote data");
		
		_deleteURI = true;
		Logger::getInstance().log(DEBUG, "ResponseBuilder::generateDeleteHTML : The _realURI will be deleted");
		
	}
	else 
	{
		Logger::getInstance().log(ERROR, "ResponseBuilder::generateDeleteHTML : deleteFileStream failed to open");
		throw Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR);
	}

	deleteFileStream.close();
	if (deleteFileStream.fail())
	{
		Logger::getInstance().log(ERROR, "ResponseBuilder::generateDeleteHTML : deleteFileStream failed to close");
		throw Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR);
	}
}

/**
 * @brief Handles the DELETE HTTP method by attempting to delete the target file.
 * 
 * This function attempts to delete the file specified by the _realURI member variable.
 * If the deletion is successful, it generates the appropriate HTML response and sets the content length.
 * If the deletion fails, it logs an error and sets the response to a 500 Internal Server Error without raising an exception.
 */
void	ResponseBuilder::deleteEngine( void ){

	Logger::getInstance().log(DEBUG, "FUNCTION CALL : ResponseBuilder::deleteEngine");

	string targetToDelete = _realURI;

	if (std::remove(targetToDelete.c_str()) != 0)
	{
		Logger::getInstance().log(ERROR, "500 Detected from ResponseBuilder::deleteEngine : Server failed to delete file with DELETE method");
		setError(CODE_500_INTERNAL_SERVER_ERROR, true); // do not raise an exception
	}
	else
	{
		Logger::getInstance().log(DEBUG, "ResponseBuilder::deleteEngine successfully deleted the file");
		generateDeleteHTML();
		setContentLenght();
	}
}