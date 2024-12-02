#include "ResponseBuilder.hpp"
#include "Logger.hpp"

#include <cstdio> // might need to move it in a global header file

void	ResponseBuilder::generateDeleteHTML( void ){

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

	// Make the page dissapear
	_deleteURI = true;

	// Modify the URI to be targeted by the size afterwards
	_realURI.erase(_realURI.find_last_of("/") + 1);
	_realURI += deleteFileName;

	ofstream deleteFile(_realURI.c_str());

	deleteFile << result.str();

}

void	ResponseBuilder::deleteEngine( void ){

	string targetToDelete = _realURI;

	if (std::remove(targetToDelete.c_str()) != 0)
	{
		Logger::getInstance().log(ERROR, "500 Detected from `deleteEngine` : Server failed to delete file with DELETE method");
		setError(CODE_500_INTERNAL_SERVER_ERROR, true); // do not raise an exception
	}
	else
	{
		Logger::getInstance().log(DEBUG, "`deleteEngine` successfully deleted the file");
		generateDeleteHTML();
		setContentLenght();
	}
}