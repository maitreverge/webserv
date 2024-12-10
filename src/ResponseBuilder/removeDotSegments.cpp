#include "ResponseBuilder.hpp"
#include "Logger.hpp"

/**
 * @brief Removes dot segments from the URI.
 * 
 * This function removes occurrences of "../", "./", and ".." from the _realURI member variable.
 * It iterates through a predefined list of target strings and erases them from the URI if found.
 */
void ResponseBuilder::removeDotSegments( void ){

	#define itVector vector<string>::iterator

	vector< string >targetsToErase;

	// Targets to remove
	targetsToErase.push_back("../");
	targetsToErase.push_back("./");
	targetsToErase.push_back("..");

	std::string::size_type found;

	for (itVector it = targetsToErase.begin(); it != targetsToErase.end(); ++it )
	{
		found = _realURI.find(*it);
		while (found != std::string::npos)
		{
			_realURI.erase(found, it->length());
		}
	}
}