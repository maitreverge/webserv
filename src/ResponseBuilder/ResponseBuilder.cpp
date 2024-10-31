#include "ResponseBuilder.hpp"
#include "Logger.hpp"

// ------------------------- METHODS ---------------------------------

void ResponseBuilder::sanatizeURI( string &oldURI ){

	string needle = "../";

	std::string::size_type found = oldURI.find(needle);

	while (found != std::string::npos)
	{
		oldURI.erase(found, 3);
		found = oldURI.find(needle);
	}
}

bool ResponseBuilder::redirectURI( void ){

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

void ResponseBuilder::rootMapping( void ){

	string originalURI = _realURI;
	string mainRoute = _realURI;

	// ! STEP 1 : Try every MainRoute in reverse order. If not found, then trim the URI from the end.
	while (not mainRoute.empty())
	{
		try
		{
			_config->routeMapping.at(mainRoute);
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
		needle = _config->routeMapping.at(mainRoute).begin()->first;
		reroute = _config->routeMapping.at(mainRoute).begin()->second;
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

void ResponseBuilder::resolveURI( void )
{
	// ! STEP 1 : Check the rootMapping
	rootMapping();
	
	// ! STEP 3 : Trim all "../" from the URI for transversal path attacks
	sanatizeURI(_realURI);

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
	checkCGI();

	// TODO = Does the route accepts the METHOD ?
	{
		// Set un fichier par défaut comme réponse si la requête est un répertoire.
	}
	
	checkNatureAndAuthoURI();

	if (_isDirectory and (_method == GET) and (not _isCGI))
	{
		generateListingHTML();
	}

}


void	ResponseBuilder::buildHeaders(){

	errorCode codes;

	stringstream	streamStatusLine,
					streamTimeStamp,
					streamContentLenght,
					streamMasterHeader;

	
	// -------------- Madatory Headers --------------  

	// ✅ GET FRIENDLY ✅ POST FRIENDLY ⛔ DELETE FRIENDLY
	streamStatusLine	<< HTTP_PROTOCOL
						<< SPACE
						<< _errorType
						<< SPACE 
						<< codes.getCode(_errorType)
						<< HTTP_HEADER_SEPARATOR;
	Headers.statusLine = streamStatusLine.str();

	// ✅ GET FRIENDLY ✅ POST FRIENDLY ⛔ DELETE FRIENDLY
	streamTimeStamp		<< "Date:"
						<< SPACE 
						<< timeStamp::getTime()
						<< HTTP_HEADER_SEPARATOR;
	Headers.timeStamp = streamTimeStamp.str();
	
	// ✅ GET FRIENDLY ✅ POST FRIENDLY ⛔ DELETE FRIENDLY
	streamContentLenght	<< "Content-Length:"
						<< SPACE
						<< Headers.bodyLenght // ! NEEDLE WORKING FOR POST
						<< HTTP_HEADER_SEPARATOR;
	Headers.contentLenght = streamContentLenght.str();

	
	// --------------  Optionals Headers --------------  
	// ✅ GET FRIENDLY ✅ POST FRIENDLY ⛔ DELETE FRIENDLY
	if (Headers.bodyLenght > 0 and _method == GET)
	{
		stringstream streamContentType;
		
		// Content Type
		string contentType = extractType(_fileExtension);
		streamContentType	<< "Content-Type:"
							<< SPACE 
							<< contentType 
							<< HTTP_HEADER_SEPARATOR;
		
		Headers.contentType = streamContentType.str();
	}

	// ✅ REDIRECTION ONLY
	if (isErrorRedirect())
	{
		stringstream streamLocation;
		streamLocation	<< "Location:"
						<< SPACE
						<< _config->redirection.at(_realURI)
						// << "http://www.github.com/maitreverge"
						<< HTTP_HEADER_SEPARATOR;
		Headers.location = streamLocation.str();
	}

	// ======================== POST HEADERS ========================
	// TODO : Implement a redirection logic for a POST request, for avoiding getting stuck
	// if (_method == POST and _errorType == CODE_201_CREATED)
	// {
	// 	stringstream streamLocation;
	// 	streamLocation	<< "Location:"
	// 					<< SPACE
	// 					<< // ! NEEDLE WORK
	// 					<< HTTP_HEADER_SEPARATOR;
	// 	Headers.location = streamLocation.str();
	// }


	// ======================== POST HEADERS ========================


	// ======================== BONUS METHODS ========================
	// TODO : Coockie and session generator
	{

	}
	// ======================== BONUS METHODS ========================

	
	// ======================== BUILDING FINAL HEADERS ========================

	streamMasterHeader	<< Headers.statusLine
						<< Headers.timeStamp
						<< Headers.contentType 
						// Optionals
						<< Headers.location
						// << (Headers.bodyLenght ? Headers.contentLenght : Headers.transfertEncoding)
						<< Headers.contentLenght
						<< HTTP_HEADER_SEPARATOR; // HEADER / BODY SEPARATOR
	
	string tempAllHeaders = streamMasterHeader.str();

	// Insert all headers in a vector char
	Headers.masterHeader.insert(Headers.masterHeader.end(), tempAllHeaders.begin(), tempAllHeaders.end());
}

void	ResponseBuilder::getHeader( Client &inputClient, Config &inputConfig ){

	Logger::getInstance().log(DEBUG, "Response Builder GET Header", inputClient);
		
	_client = &inputClient; // init client
	_config = &inputConfig; // init config
	
	_realURI = _client->headerRequest.getURI();

	extractMethod();
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
	
	// Headers.masterHeader.clear();//!

	// printAllHeaders();
}
