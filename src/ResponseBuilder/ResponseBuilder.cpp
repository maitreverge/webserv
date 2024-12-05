#include "ResponseBuilder.hpp"
#include "Logger.hpp"

void	ResponseBuilder::redirectURI( void ){

	if (_myconfig.redirection.empty())
		return ;
	else if (_realURI == "/" and _myconfig.redirection == "/")
	{
		Logger::getInstance().log(ERROR, "508 LOOP : Both _realURi and Redirection == \"/\"");
		setError(CODE_508_LOOP_DETECTED);
	}
	else if (_realURI.size() == 1 and *_realURI.begin() == '/')
	{
		Logger::getInstance().log(INFO, "Regular redirection activated");
		_realURI = _myconfig.redirection;
		setError(CODE_302_FOUND);
	}
	else if (_myconfig.uri == "/" and _realURI.find(_myconfig.redirection) != std::string::npos)
	{
		Logger::getInstance().log(ERROR, "Redirection Loop Detected");
		setError(CODE_508_LOOP_DETECTED);
	}
}

void ResponseBuilder::rootMapping( void ){

	if (_myconfig.root.empty())
		return;
	
	Logger::getInstance().log(INFO, "ROOT MAPPING CALLED");
	if (_realURI.find(_myconfig.root) == std::string::npos)
	{
		Logger::getInstance().log(DEBUG, "URI BEFORE MAPPING");
		Logger::getInstance().log(DEBUG, _realURI);

		_realURI.insert(0, _myconfig.root);
		
		Logger::getInstance().log(DEBUG, "URI AFTER MAPPING");
		Logger::getInstance().log(DEBUG, _realURI);
	}
}

bool ResponseBuilder::isDirectory(string &strInput) {
	
	if ( (stat(strInput.c_str(), &_fileInfo) == 0) and (_fileInfo.st_mode & S_IFDIR))
	{
		return true;
	}
	return false;
}

void	ResponseBuilder::slashManip( string &target, bool makeRedirection ){

	bool beginWithSlash = !target.empty() && (*target.begin() == '/');
	bool endWithSlash = !target.empty() && (*target.rbegin() == '/');
	
	if (target != "/" and beginWithSlash)
	{
		target.erase(target.begin());
	}

	if ( isDirectory(target) )
	{
		if (!endWithSlash)
		{
			if (makeRedirection and _method == GET)
			{
				Logger::getInstance().log(INFO, "Wrong Typed URI, automatic 302 Redirection enabled");
				
				_realURI = _originalURI + "/";

				setError(CODE_302_FOUND);
			}
			else
				target += "/";
		}
		if (_method == GET and _myconfig.listingDirectory == false)
			target += _myconfig.index;
		
		// Refresh the bool
		beginWithSlash = !target.empty() && (*target.begin() == '/');
		if (beginWithSlash and target != "/")
			target.erase(target.begin());
    }
}

void ResponseBuilder::resolveURI( void ) {
	
	// STEP 1 : Check the rootMapping
	rootMapping();

	// STEP 2: Handle redirection
    redirectURI();
	
	//  STEP 3 : Trim all ".." transerval attacks from the URI.
	removeDotSegments();

	//  STEP 4  : Deleting URI first 
	slashManip(_realURI, true);

	//  STEP 5 : Check for DELETE + webserv
	if (_method == DELETE and _realURI == "webserv")
	{
		Logger::getInstance().log(ERROR, "User tried to delete the `webserv` binary");
		setError(CODE_403_FORBIDDEN);
	}
}

void	ResponseBuilder::checkMethod( void ){

	if (_myconfig.allowedMethods.empty())
		return;
	
	for (std::vector<string>::iterator it = _myconfig.allowedMethods.begin(); it != _myconfig.allowedMethods.end(); ++it)
	{
		if (*(it) == "GET" and _method == GET)
			return;
		else if (*(it) == "POST" and _method == POST)
			return;
		else if (*(it) == "DELETE" and _method == DELETE)
			return;
	}

	Logger::getInstance().log(ERROR, "Method not found");

	setError(CODE_405_METHOD_NOT_ALLOWED);
}

void	ResponseBuilder::getHeader( Client &inputClient, Config &inputConfig, e_errorCodes codeInput ){

	Logger::getInstance().log(DEBUG, "FUNCTION CALL : ResponseBuilder::getHeader");
		
	_client = &inputClient;
	_config = &inputConfig;

	resetMyVariables();
	
	if (codeInput != CODE_200_OK)
	{
		Logger::getInstance().log(DEBUG, "ResponseBuilder::getHeader invoked straight with an error code");
		_method = GET;
		setError(codeInput, true);
		setContentLenght();
		buildHeaders();
		inputClient.headerRespons = Headers.masterHeader;
		return;
	}
	
	_realURI = _client->headerRequest.getURI();
	_originalURI = _realURI;

	_errorType = CODE_200_OK;

	extractRouteConfig();

	#ifdef DEB
		printMyConfig();
	#endif

	try
	{
		checkSessionIdCookie(inputClient);

		extractMethod();
		
		checkMethod();

		serverNameChecking();

		extraStartingChecks();

		if (_method == POST)
			_errorType = CODE_201_CREATED;
		else
			_errorType = CODE_200_OK;

		if (_method != DELETE)
			checkCGI();

		resolveURI();
		
		checkAutho();
		
		checkNature();
		
		if (_isCGI)
			_cgi.launch(inputClient);
		
		if (_isDirectory and (_method == GET) and (not _isCGI))
			generateListingHTML();
	
	}
	catch(const CodeErrorRaised& e)
	{
		Logger::getInstance().log(DEBUG, "Code Error Raised in the getHeader building process", inputClient);
	}
	catch(const std::exception& e)
	{
		return;
		Logger::getInstance().log(DEBUG, "Another kind or error has been raised in the getHeader process", inputClient);
	} 

	if (not isErrorRedirect())
		setContentLenght();
	
	if (_method == DELETE and _errorType == CODE_200_OK)
		deleteEngine();	

	buildHeaders();

	// TO bypass the writting process // ! cf SEB ClientBody
	// TODO ! Maybe useless check
	if (_method == POST and _errorType != CODE_201_CREATED)
		_method = GET;

	// Copying the build Headers in headerResponse
	if (!_isCGI)
		inputClient.headerRespons = Headers.masterHeader;
	else
		inputClient.headerRespons.clear();

	#ifdef DEB
		printAllHeaders();
	#endif
}

