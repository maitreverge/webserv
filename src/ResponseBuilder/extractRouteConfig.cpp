#include "ResponseBuilder.hpp"
#include "Logger.hpp"

typedef std::map< string, map<string, vector< string > > >::iterator mapIterator;
void	ResponseBuilder::extractRouteConfig( void ){

	string targetRoute;

	// u_int16_t nbRoutes = _config->routes.size();

	// vector< string > routeNames;
	vector< string > routeURIS;

	// extract routes names
	// for (mapIterator it = _config->routes.begin(); it != _config->routes.end(); ++it)
	// {
	// 	routeNames.push_back(it->first);
	// }

	vector < string > toClear;



	// extracting all uris
	for (mapIterator it = _config->routes.begin(); it != _config->routes.end(); ++it)
	{
		vector< string > curVector;
		string curURI;
		try
		{
			curVector =  _config->routes.at(it->first).at("uri");
			curURI = curVector[0];
			routeURIS.push_back(curURI);
		}
		catch(const std::exception& e)
		{
			// not URI key found in the current node, need to delete the node
			toClear.push_back(it->first);
		}
		

		curURI.clear();
		curVector.clear();
	}

	// clear nodes without the URI
	for (std::vector< string >::iterator it = toClear.begin(); it != toClear.end(); ++it)
	{
		_config->routes.erase(*it);
	}

	// Clear duplicates URIs
	
	
}