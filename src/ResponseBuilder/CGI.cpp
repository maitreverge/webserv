/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TryHardTeam <TryHardTeam@123.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:10 by TryHardTeam       #+#    #+#             */
/*   Updated: 2024/12/16 12:09:11 by TryHardTeam      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseBuilder.hpp"
#include "Logger.hpp"

/**
 * @brief Checks if the requested URI corresponds to a CGI script (PHP or Python).
 * 
 * This function determines if the requested URI contains a PHP or Python script extension.
 * If a CGI script is found and allowed, it sets the appropriate flags and extracts the path info.
 * It also updates the URI to include only the script part and sets the CGI folder path.
 */
void ResponseBuilder::checkCGI( void ){

	Logger::getInstance().log(DEBUG, "FUNCTION CALL : ResponseBuilder::checkCGI");

    // Define the file extensions for Python and PHP scripts
    string extensionPHP = ".php";
    string extensionPython = ".py";
	string extensionTarget;

    // Find the first positions of the Python and PHP extensions in the URI
    std::string::size_type phpLoc = _realURI.find(extensionPHP);
    std::string::size_type pythonLoc = _realURI.find(extensionPython);

	std::string::size_type targetLoc;
    
    // Determine the position of the found extension
    if (phpLoc == std::string::npos and pythonLoc == std::string::npos )
        return; // _isCGI stays `false`
    else if (phpLoc != std::string::npos and pythonLoc != std::string::npos )
	{
		// Determine which one is found first to split correctly
		targetLoc = (phpLoc < pythonLoc) ? phpLoc : pythonLoc;
	}
    else if (phpLoc != std::string::npos)
		targetLoc = phpLoc;
	else
		targetLoc = pythonLoc;
	
	extensionTarget = (targetLoc == phpLoc) ? extensionPHP : extensionPython;

	// if the extensionTarget has not been found within the cgiAllowed vector.
	if ( std::find(	_myconfig.cgiAllowed.begin(),
					_myconfig.cgiAllowed.end(),
					extensionTarget) == _myconfig.cgiAllowed.end() )
	{
		// do nothing, treat the URI like a regular one
		return;
	}

    // If either extension is found, set _isCGI to true
    _isCGI = true;
    
    // Adjust the position to the end of the extension
	targetLoc += extensionTarget.length();

    // Extract the path info after the script extension
    _pathInfo = _realURI.substr(targetLoc);

    // Update _realURI to only include the part up to and including the script extension
    _realURI = _realURI.substr(0, targetLoc);

	_folderCGI = _realURI.substr(0, _realURI.find_last_of('/') + 1);

	if (!_folderCGI.empty() && _folderCGI[0] == '/')
		_folderCGI.erase(_folderCGI.begin());
	// _folderCGI.insert(0, ".");

	Logger::getInstance().log(DEBUG, "RESULT OF FUNCTION : ResponseBuilder::checkCGI");
	Logger::getInstance().log(DEBUG, "_pathInfo = " + _pathInfo);
	Logger::getInstance().log(DEBUG, "_folderCGI = " + _folderCGI);
	Logger::getInstance().log(DEBUG, "_realURI = " + _realURI);
}