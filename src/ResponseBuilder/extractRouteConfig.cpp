#include "ResponseBuilder.hpp"
#include "Logger.hpp"

typedef std::map< string, map<string, vector< string > > >::iterator mapIterator;
typedef std::vector< string >::iterator vectorIterator;

void ResponseBuilder::clearingRoutes( vector< string >&routeNames, vector< string >&routeURIS ){

	vector < string > toClear;

	// extracting all uris
	for (mapIterator it = _config->routes.begin(); it != _config->routes.end(); ++it)
	{
		vector< string > curVector;
		try
		{
			curVector =  _config->routes.at(it->first).at("uri");
			if (curVector.empty())
				throw std::bad_exception();
			routeURIS.push_back(curVector[0]);
			routeNames.push_back(it->first); // get the current route Names [route1], [route2], ect...
		}
		catch(const std::exception& e)
		{
			// not URI key found in the current node, need to delete the node
			toClear.push_back(it->first);
		}
		curVector.clear();
	}

	// clear nodes without the URI
	for (vectorIterator it = toClear.begin(); it != toClear.end(); ++it)
	{
		_config->routes.erase(*it); // erase only in ResponseBuilder scope
	}

	// TODO : Delete duplicates nodes
	{
		// don't forget to update routeURIS and routeNames accordingly
	}
}

void	foundPath( string path ){

	
}

void	ResponseBuilder::extractRouteConfig( void ){

	vector< string > routeNames;
	vector< string > routeURIS;

	clearingRoutes(routeNames, routeURIS);

	// string originalURI = _realURI;
	string trimmedRoute = _realURI;
	bool found = false;
	u_int8_t pos;

	// From this point, we only got route with existing `uris`
	// ! STEP 1 : Try every MainRoute in reverse order. If not found, then trim the URI from the end.
	while (not trimmedRoute.empty())
	{
		pos = 0;
		// Look accross routeURIS
		for (vectorIterator it = routeURIS.begin(); it != routeURIS.end(); ++it, ++pos)
		{
			if (*it == trimmedRoute ) // happy path, found mathcing URI
			{
				found = true;
				break;
			}
		}
		if (not found) // trimmed the URI
		{
			if (trimmedRoute.length() == 1)
				break; // not found the "/" Route
			else if (trimmedRoute.find_first_of('/') == trimmedRoute.find_last_of('/'))
			{
				if (trimmedRoute.length() == 1) // (if only left with a /)
					trimmedRoute.erase();
				else
					trimmedRoute.erase(trimmedRoute.find_first_of('/') + 1);
			}
			else
				trimmedRoute.erase(trimmedRoute.find_last_of('/'));
		}
		else
			break;
	}

	if (found)
	{
		foundPath(routeNames[pos]);


	}
	else
	{

	}
	

}