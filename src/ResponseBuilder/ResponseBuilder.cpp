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

	if (_myconfig.redirection.empty())
		return false;

	_realURI = _myconfig.redirection;
	
	setError(CODE_307_TEMPORARY_REDIRECT);
	return true;
}

void ResponseBuilder::rootMapping( void ){ // ✅ OKAY FUNCTION

	if (_myconfig.root.empty())
		return;
	
	_realURI.replace(0, _myconfig.uri.size(), _myconfig.root);
}

bool ResponseBuilder::isDirectory(string &uri) {
	
	if (stat(uri.c_str(), &_fileInfo) != 0)
		return false;
	else if (_fileInfo.st_mode & S_IFDIR)
		return true;
	else
		return false;
}

void ResponseBuilder::resolveURI( void ) {// ⛔ NOT OKAY FUNCTION
	
	// ! STEP 1 : Check the rootMapping
	rootMapping();

	// Step 2: Handle redirection
    if (redirectURI()) {
        return;
    }
	
	// ! STEP 2 : Trim all "../" from the URI for transversal path attacks
	// sanatizeURI(_realURI); // ! STAY COMMENTED until refactoring for better "../" erasing process

	// ! STEP 3  : Deleting URI first 
	if (isDirectory(_realURI) and not _myconfig.listingDirectory)
	{
		if (*_realURI.begin() == '/')
			_realURI.erase(_realURI.begin() + 0); // turn a regular URI ("/index.html" into "index.html")
		if (*_realURI.rbegin() != '/' or not _realURI.empty())
			_realURI += "/"; 
		// if (not _realURI.empty())
		// 	_realURI += "/";
		_realURI += _myconfig.index; // after checking the nature
		return;
    }
	if ( not (_realURI.size() == 1 and *_realURI.begin() == '/') )
		_realURI.erase(_realURI.begin() + 0); // turn a regular URI ("/index.html" into "index.html")
}

void	ResponseBuilder::checkMethod( void ){

	if (_myconfig.allowedMethods.empty())
		return;
	
	for (std::vector<string>::iterator it = _myconfig.allowedMethods.begin(); it < _myconfig.allowedMethods.end(); ++it)
	{
		if (*(it) == "GET" and _method == GET)
			return;
		else if (*(it) == "POST" and _method == POST)
			return;
		else if (*(it) == "DELETE" and _method == DELETE)
			return;
	}
	
	setError(CODE_405_METHOD_NOT_ALLOWED);
}

void	ResponseBuilder::getHeader( Client &inputClient, Config &inputConfig ){

	Logger::getInstance().log(DEBUG, "ResponseBuilder->getHeader", inputClient);
		
	_client = &inputClient; // init client
	_config = &inputConfig; // init config
	
	_realURI = _client->headerRequest.getURI();
	_originalURI = _client->headerRequest.getURI();

	_errorType = CODE_200_OK;

	extractRouteConfig();
	printMyConfig();
	

	try
	{
		// if ( not redirectURI())
		// {
			extractMethod();

			checkMethod();

			if (_method == DELETE)
				setError(CODE_204_NO_CONTENT); // does not throw exception
			else // if (_method != DELETE)
				checkCGI();
			if (_method == POST and !_isCGI)
				uploadCheck();
			
			resolveURI();
			checkAutho();
			checkNature();
			
			// ! WORK NEEDLE
			if (_isDirectory and (_method == GET) and (not _isCGI))
			{
				generateListingHTML();
			}

			if (_method == DELETE and _errorType < CODE_400_BAD_REQUEST)
				deleteEngine();	
		// }
	}
	catch(const CodeErrorRaised& e)
	{
		Logger::getInstance().log(INFO, "Code Error Raised in the getHeader building process", inputClient);
	}
	catch(const std::exception& e)
	{
		Logger::getInstance().log(INFO, "Another kind or error has been raised in the getHeader process", inputClient);
	} 

	if (not isErrorRedirect())
		setContentLenght();
	
	
	buildHeaders();

	inputClient.headerRespons = Headers.masterHeader;
	

	// // Copying the build Headers in headerRespons

	// Headers.masterHeader.clear();//!

	printAllHeaders();
}
