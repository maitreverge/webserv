#include "ResponseBuilder.hpp"
#include "Logger.hpp"

// ------------------------- METHODS ---------------------------------

void ResponseBuilder::sanatizeURI( string &oldURI ){ // ⛔ NOT OKAY FUNCTION

	// TODO : refactor this function to avoid trimming usefull "../", as long as we don't escape the root webserv
	string needle = "../";

	std::string::size_type found = oldURI.find(needle);

	while (found != std::string::npos)
	{
		oldURI.erase(found, 3);
		found = oldURI.find(needle);
	}
}

bool ResponseBuilder::redirectURI( void ){ // ✅ OKAY FUNCTION

	try
	{
		_config->redirection.at(_realURI);
	}
	catch(const std::exception& e)
	{
		return false;
	}
	
	_errorType = CODE_307_TEMPORARY_REDIRECT;
	return true;
}

void ResponseBuilder::rootMapping( void ){ // ✅ OKAY FUNCTION

	string originalURI = _realURI;
	string mainRoute = _realURI;

	// ! STEP 1 : Try every MainRoute in reverse order. If not found, then trim the URI from the end.
	while (not mainRoute.empty())
	{
		try
		{
			// _config->routeMapping.at(mainRoute);
			break;
		}
		catch(const std::exception& e)
		{
			if (mainRoute == "/" or mainRoute.empty())
				return; // MainRoute not found
			// If the first "/" is also the last, we're left with the last "/" default path
			else if (mainRoute.find_first_of('/') == mainRoute.find_last_of('/'))
			{
				// erasing keeping the "/"
				if (mainRoute[mainRoute.length() -1] == '/')
					return;
				mainRoute.erase(mainRoute.find_first_of('/') + 1);
			}
			else
			{
				mainRoute.erase(mainRoute.find_last_of('/'));
			}
		}
	}

	// From this Point, a route has been found
	string target = originalURI.substr(mainRoute.size());

	string needle;
	string reroute;
	try
	{
		// needle = _config->routeMapping.at(mainRoute).begin()->first;
		// reroute = _config->routeMapping.at(mainRoute).begin()->second;
	}
	catch(const std::exception& e)
	{
		// Map needle or reroute has not been found.
		return;
	}

	// From this Point, a needle and a reroute has been found, we need to find them, hotshap them and return
	if (target.compare(0, needle.size(), needle) == 0)
	{
		target.erase(0, needle.size());
		
		originalURI = reroute + target;
	}

	_realURI = originalURI;
}

void ResponseBuilder::resolveURI( void ) {// ⛔ NOT OKAY FUNCTION
	// ! STEP 1 : Check the rootMapping
	// rootMapping();
	
	// ! STEP 2 : Trim all "../" from the URI for transversal path attacks
	// sanatizeURI(_realURI); // ! STAY COMMENTED until refactoring for better "../" erasing process

	if (_realURI.size() > 1)
	{
		_realURI.erase(_realURI.begin() + 0); // turn a regular URI ("/index.html" into "index.html")
		if ( *_realURI.rbegin() == '/' ) // Removing all ending '/' URIs
		{
			_realURI.erase(_realURI.size() -1);
		}
	}
}

void	ResponseBuilder::validateURI( void ){

	// ! STEP 1 = EDGE CASES FOR EMPTY PATH or PATH == "/"
	if (_realURI.empty())
	{
		setError(CODE_404_NOT_FOUND); return;
	}
	else if (_realURI == "/" and _method == GET) // What if the resolved URI is a directory and not just "/"
	{
		// string originalURI = "/";
		vector<string>::iterator it;
		for (it = _config->indexFiles.begin(); it < _config->indexFiles.end(); ++it)
		{
			_realURI = *it;
			
			// Check permissions with stat
			if (stat(_realURI.c_str(), &_fileInfo) == 0)
				break ;
		}

		if (it == _config->indexFiles.end())
		{
			if (errno == EACCES) // The process does not have execute permissions on one or more directories in the path.
			{
				setError(CODE_401_UNAUTHORIZED); return;
			}
			else if (errno == ENOENT) // The file or directory does not exist.
			{
				setError(CODE_404_NOT_FOUND); return;
			}
		}
	}

	// TODO = Is URI a CGI ??
	if (_method != DELETE)
		checkCGI();

	checkNatureAndAuthoURI();

	if (_isDirectory and (_method == GET) and (not _isCGI))
	{
		generateListingHTML();
	}

}

void	ResponseBuilder::checkMethod( void ){

	#ifdef UNIT_TEST
	#else
	_realURI = "/";
	// vector<string> innerVector;
	// innerVector.push_back("GET");

	// _config->allowedMethods.erase("/");
	// map<string, vector<string> > innerMap;
	// innerMap.insert(std::make_pair("wvbgfjerbgvhjer", innerVector));
	// _config->allowedMethods.insert(std::make_pair("/", innerMap));
	#endif

	try
	{
		// _config->allowedMethods.at(_realURI); // look up for the route
	}
	catch(const std::exception& e)
	{
		return; // Route not found == FULL AUTHO
	}

	vector<string> methods;
	try
	{
		methods = _config->routes.at("route1").at("allowedMethods");
	}
	catch(const std::exception& e)
	{
		return; // allowedMethod not found within the route == FULL AUTHO
	}

	if (methods.empty())
		return;
	
	for (std::vector<string>::iterator it = methods.begin(); it < methods.end(); ++it)
	{
		if (*(it) == "GET" and _method == GET)
			return;
		else if (*(it) == "POST" and _method == POST)
			return;
		else if (*(it) == "DELETE" and _method == DELETE)
			return;
	}
	
	#ifdef UNIT_TEST
	_errorType = CODE_405_METHOD_NOT_ALLOWED;
	#else
	setError(CODE_405_METHOD_NOT_ALLOWED);
	#endif
}

void	ResponseBuilder::getHeader( Client &inputClient, Config &inputConfig ){

	Logger::getInstance().log(DEBUG, "Response Builder GET Header", inputClient);
		
	_client = &inputClient; // init client
	_config = &inputConfig; // init config
	
	_realURI = _client->headerRequest.getURI();

	extractRouteConfig();
	extractMethod();
	checkMethod();

	if(_method == DELETE)
		setError(CODE_204_NO_CONTENT);

	if ( not redirectURI())
	{
		resolveURI();
		validateURI();
		
		// if (_isCGI and _errorType <= CODE_400_BAD_REQUEST) // or potentially another adress
		// 	launchCGI();
		
		checkNatureAndAuthoURI(); // double check for this Nature, if the URi has been swapped for an error file
		setContentLenght();
	}
	
	buildHeaders();

	// Copying the build Headers in headerRespons
	inputClient.headerRespons = Headers.masterHeader;

	if (_method == DELETE and _errorType < CODE_400_BAD_REQUEST)
		deleteEngine();	
	// Headers.masterHeader.clear();//!

	// printAllHeaders();
}
