#include "ResponseBuilder.hpp"
#include "Logger.hpp"

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

/*
	https://datatracker.ietf.org/doc/html/rfc3986.html#section-5.2

   2.  While the input buffer is not empty, loop as follows:

       A.  If the input buffer begins with a prefix of "../" or "./",
           then remove that prefix from the input buffer; otherwise,

       B.  if the input buffer begins with a prefix of "/./" or "/.",
           where "." is a complete path segment, then replace that
           prefix with "/" in the input buffer; otherwise,

       C.  if the input buffer begins with a prefix of "/../" or "/..",
           where ".." is a complete path segment, then replace that
           prefix with "/" in the input buffer and remove the last
           segment and its preceding "/" (if any) from the output
           buffer; otherwise,

       D.  if the input buffer consists only of "." or "..", then remove
           that from the input buffer; otherwise,

       E.  move the first path segment in the input buffer to the end of
           the output buffer, including the initial "/" character (if
           any) and any subsequent characters up to, but not including,
           the next "/" character or the end of the input buffer.

*/