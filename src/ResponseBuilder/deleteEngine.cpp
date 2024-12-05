#include "ResponseBuilder.hpp"
#include "Logger.hpp"

#include <cstdio> // might need to move it in a global header file

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

	result	<<	"<!DOCTYPE html>\n"
			<<	"<html lang=\"en\">\n"
			<<	"<head>\n"
			<<	"<meta charset=\"UTF-8\">\n"
			<<	"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
			<<	"<title>Resource Deleted</title>\n"
			<<	"</head>\n"
			<<	"<body>\n"
			<<	"<h1>Resource Deleted</h1>\n"
			<<	"<p>The file <b>"
			<<	_fileName
			<<	"</b> has been successfully deleted.</p>\n"
			<<	"</body>\n"
			<<	"</html>";

	_deleteURI = true;
	Logger::getInstance().log(DEBUG, "ResponseBuilder::generateDeleteHTML : The _realURI will be deleted");

	// Modify the URI to be targeted by the size afterwards
	_realURI.erase(_realURI.find_last_of("/") + 1);
	_realURI += deleteFileName;

	ofstream deleteFile(_realURI.c_str());

	deleteFile << result.str();
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