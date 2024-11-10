#include "ResponseBuilder.hpp"
#include "Logger.hpp"

#include <cstdio> // might need to move it in a global header file

void	ResponseBuilder::deleteEngine( void ){

	string targetToDelete = _realURI;

	if (std::remove(targetToDelete.c_str()) != 0)
	{
		Logger::getInstance().log(ERROR, "Delete engine failed deleting the file");
		setError(CODE_500_INTERNAL_SERVER_ERROR, true); // do not raise an exception
	}
	else
		Logger::getInstance().log(INFO, "Target URI Successfully deleted");
}
