#include "ResponseBuilder.hpp"
#include "Logger.hpp"

typedef std::map< string, map<string, vector< string > > >::iterator mapIterator;
typedef std::vector< string >::iterator vectorIterator;

void ResponseBuilder::clearingRoutes( vector< string >&routeNames, vector< string >&routeURIS ){

	vector < string > toClear;
	RoutesData *routes = &_config->_serverStruct[_config->index].routesData;

	// Extracting all uris
	for (mapIterator it = routes->begin(); it != routes->end(); ++it)
	{
		vector< string > curVector;
		try
		{
			curVector =  routes->at(it->first).at("uri");
			if (curVector.empty())
				throw std::bad_exception();
			if (curVector[0].size() > 1 and *curVector[0].begin() != '/')
				curVector[0].insert(0, "/");
				// curVector[0].erase(curVector[0].begin());
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

	// TODO : clear nodes without the URI
	
	// for (vectorIterator it = toClear.begin(); it != toClear.end(); ++it)
	// {
	// 	routes->erase(*it); // erase only in ResponseBuilder scope
	// }

	// TODO : Delete duplicates nodes with the same URI
	{
		// don't forget to update routeURIS and routeNames accordingly
	}
}

void	ResponseBuilder::buildRouteConfig( string path ){


	RoutesData *routes = &_config->_serverStruct[_config->index].routesData;
	
	// ! uri
	try
	{
		_myconfig.uri = *routes->at(path).at("uri").begin();

		// Every URI needs to start with a "/" and end without.
		if (_myconfig.uri.size() > 1)
		{
			if (*_myconfig.uri.begin() != '/')
				_myconfig.uri.insert(0, "/");
			if (*_myconfig.uri.rbegin() == '/' and isDirectory(_myconfig.uri))
				_myconfig.uri.erase(_myconfig.uri.end() - 1);
		}
	}
	catch(const std::exception& e) {}

	// ! allowedMethods
	try
	{
		_myconfig.allowedMethods = routes->at(path).at("allowedMethods");
	}
	catch(const std::exception& e) {}

	// ! redirection
	try
	{
		_myconfig.redirection = *routes->at(path).at("redirection").begin();
		if (_myconfig.redirection.size() > 1)
		{
			if (*_myconfig.redirection.begin() == '/')
				_myconfig.redirection.erase(_myconfig.redirection.begin());
			if (*_myconfig.redirection.rbegin() != '/' and isDirectory(_myconfig.redirection))
				_myconfig.redirection += "/";
		}
	}
	catch(const std::exception& e) {}
	
	// ! root
	try
	{
		_myconfig.root = *routes->at(path).at("root").begin();
		if (_myconfig.root.size() > 1)
		{
			if (*_myconfig.root.begin() == '/')
				_myconfig.root.erase(_myconfig.root.begin());
			if (*_myconfig.root.rbegin() != '/' and isDirectory(_myconfig.root))
				_myconfig.root += "/";
		}
	}
	catch(const std::exception& e) {}

	// ! listingDirectory
	try
	{
		string myString = *routes->at(path).at("listingDirectory").data();
		// Only accepting '1' as a valid input
		if (myString.size() == 1 and myString[0] == '1')
			_myconfig.listingDirectory = true;
		else
			_myconfig.listingDirectory = false;
	}
	catch(const std::exception& e) {}

	// ! index
	try
	{
		_myconfig.index = *routes->at(path).at("index").begin();
	}
	catch(const std::exception& e) {}

	// ! cgiAllowed
	try
	{
		_myconfig.cgiAllowed = routes->at(path).at("cgiAllowed");
	}
	catch(const std::exception& e) {}

	// ! uploadAllowed
	try
	{
		string myString = *routes->at(path).at("uploadAllowed").begin();
		if (myString.size() == 1 and myString[0] == '1')
			_myconfig.uploadAllowed = true;
		else
			_myconfig.uploadAllowed = false;
	}
	catch(const std::exception& e) {}

	// ! uploadDirectory
	try
	{
		_myconfig.uploadDirectory = *routes->at(path).at("uploadDirectory").begin();
		
		// Delete first '/' char from the path if so
		if (!_myconfig.uploadDirectory.empty() && (*_myconfig.uploadDirectory.begin() == '/') )
			_myconfig.uploadDirectory.erase(_myconfig.uploadDirectory.begin());
	}
	catch(const std::exception& e) {}
}

void ResponseBuilder::extractRedirectionIndex( vector< string >&routeNames, vector< string >&routeURIS ){

	static_cast<void>(routeURIS); // might need later for refactoring with Dan routes

	typedef std::vector< string >::iterator itVector;

	string needleIndex;
	string targetRoute;

	needleIndex.clear();
	targetRoute.clear();

	for (itVector outterIT = routeNames.begin(); outterIT != routeNames.end(); ++outterIT)
	{
		try
		{
			*_config->routes.at(*outterIT).at(_myconfig.redirection).begin();
			targetRoute = *outterIT;
			break;
		}
		catch(const std::exception& e)
		{
			continue;
		}
	}
	
	if (not targetRoute.empty())
	{
		try
		{
			needleIndex = *_config->routes.at(targetRoute).at("index").begin();
		}
		catch(const std::exception& e)
		{
			Logger::getInstance().log(DEBUG, "Index Redirection not Found");
		}
	}

	if (*needleIndex.begin() != '/')
		needleIndex.insert(0, "/");
	else if (*needleIndex.rbegin() == '/')
		needleIndex.erase(needleIndex.length() - 1);

	_myconfig.indexRedirection = needleIndex;
}

void	ResponseBuilder::extractRouteConfig( void ){

	vector< string > routeNames;
	vector< string > routeURIS;

	clearingRoutes(routeNames, routeURIS);

	// string originalURI = _realURI;
	string trimmedRoute = _realURI;
	if (trimmedRoute.size() > 1 and *trimmedRoute.rbegin() != '/')
		trimmedRoute += "/";
	// if (trimmedRoute.size() > 1 and  *trimmedRoute.begin() == '/' and trimmedRoute.find_first_of('/') != trimmedRoute.find_last_of('/') )
	// 	trimmedRoute.erase(trimmedRoute.begin());
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
			else if (*trimmedRoute.rbegin() == '/') 
			{
				trimmedRoute.erase(trimmedRoute.size() -1);
			}
			else if (trimmedRoute.find_first_of('/') == trimmedRoute.find_last_of('/'))
			{
				if (trimmedRoute.length() == 1) // (if only left with a /)
					trimmedRoute.erase();
				else
					trimmedRoute.erase(trimmedRoute.find_first_of('/') + 1);
			}
			else
				trimmedRoute.erase(trimmedRoute.find_last_of('/') + 1);
		}
		else
			break;
	}

	if (found)
		buildRouteConfig(routeNames[pos]);
	
	if (not _myconfig.redirection.empty())
	{
		extractRedirectionIndex(routeNames, routeURIS);
	}
}

void ResponseBuilder::resetMyVariables()
{
	// _myconfig part
	_myconfig.allowedMethods.clear();
	_myconfig.redirection.clear();
	_myconfig.uri.clear();
	_myconfig.root.clear();
	_myconfig.listingDirectory = false;
	_myconfig.index.clear();
	_myconfig.cgiAllowed.clear();
	_myconfig.uploadAllowed = true;
	_myconfig.uploadDirectory.clear();
	_myconfig.indexRedirection.clear();

	// my variables part
	{
		// TODO : reset all variables
		_parsedBoundaryToken = false;

	}
}

void	ResponseBuilder::printMyConfig( void ){
	
	print("");
	printColor(CYAN, "================= PRINT MY CONFIG =============\n");
	
	print("ALLOWED METHODS =");
	for (vectorIterator it = _myconfig.allowedMethods.begin(); it != _myconfig.allowedMethods.end(); ++it)
	{
		printColor(BOLD_RED, *it);
	}

	print("URI =");
	printColor(BOLD_RED, _myconfig.uri);

	print("REDIRECTION =");
	printColor(BOLD_RED, _myconfig.redirection);
	print("ROOT =");
	printColor(BOLD_RED, _myconfig.root);

	print("LISTING-DIRECTORY =");
	printColor(BOLD_RED, _myconfig.listingDirectory ? "true" : "false");
	
	print("INDEX =");
	printColor(BOLD_RED, _myconfig.index);

	print("CGI ALLOWED =");
	for (vectorIterator it = _myconfig.cgiAllowed.begin(); it != _myconfig.cgiAllowed.end(); ++it)
	{
		printColor(BOLD_RED, *it);
	}

	print("UPLOAD ALLOWED =");
	printColor(BOLD_RED, _myconfig.uploadAllowed ? "true" : "false");

	print("UPLOAD DIRECTORY =");
	printColor(BOLD_RED, _myconfig.uploadDirectory);

	print("Index Redirection =");
	printColor(BOLD_RED, _myconfig.indexRedirection);

	printColor(CYAN, "================= PRINT MY CONFIG =============");
	print("");
}