#include "ResponseBuilder.hpp"
#include "Logger.hpp"

void ResponseBuilder::launchCGI( void ){

	// ! STEP 1 : Export PATH_INFO

	// ! LAST STEP : UNSET PATH_INFO
}

void ResponseBuilder::checkCGI( void ){

    // Define the file extensions for Python and PHP scripts
    string targetPython = ".py";
    string targetPHP = ".php";

    // Find the positions of the Python and PHP extensions in the URI
    std::string::size_type phpLoc = _realURI.find(targetPHP);
    std::string::size_type pythonLoc = _realURI.find(targetPython);

    // If neither Python nor PHP extensions are found, set _isCGI to false and return
    if (phpLoc == std::string::npos and pythonLoc == std::string::npos )
        return;

    // If either extension is found, set _isCGI to true
    _isCGI = true;
    
    // Determine the position of the found extension
    std::string::size_type realLoc = ( phpLoc == std::string::npos ) ? pythonLoc : phpLoc;

    // Adjust the position to the end of the extension
    realLoc += (phpLoc == std::string::npos) ? targetPython.length() : targetPHP.length();

    // Extract the path info after the script extension
    _pathInfo = _realURI.substr(realLoc);

    // Update _realURI to only include the part up to and including the script extension
    _realURI = _realURI.substr(0, realLoc);
}