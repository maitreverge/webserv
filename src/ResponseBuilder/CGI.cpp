#include "ResponseBuilder.hpp"
#include "Logger.hpp"


void ResponseBuilder::checkCGI( void ){

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

	_folderCGI.insert(0, ".");
}