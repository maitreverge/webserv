#include "ResponseBuilder.hpp"
#include "Logger.hpp"

#include <cstdio>

void	ResponseBuilder::deleteEngine( void ){

	string targetToDelete = _realURI;

	if (std::remove(targetToDelete.c_str()) != 0)
	{
		Logger::getInstance().log(ERROR, "Delete engine failed deleting the file");
		setError(CODE_500_INTERNAL_SERVER_ERROR);
	}
	else
		Logger::getInstance().log(INFO, "Target URI Successfully deleted");
}
