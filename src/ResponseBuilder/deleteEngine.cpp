#include "ResponseBuilder.hpp"
#include "Logger.hpp"

#include <cstdio> // might need to move it in a global header file

void	ResponseBuilder::generateDeleteHTML( void ){

	// TODO
}

void	ResponseBuilder::deleteEngine( void ){

	string targetToDelete = _realURI;

	if (std::remove(targetToDelete.c_str()) != 0)
	{
		Logger::getInstance().log(ERROR, "Delete engine failed deleting the file");
		setError(CODE_500_INTERNAL_SERVER_ERROR, true); // do not raise an exception
	}
	else
	{
		Logger::getInstance().log(DEBUG, "Target URI Successfully deleted");
		// TODO : Generate HTML for telling client everything deleted fine
		generateDeleteHTML();
	}
}
