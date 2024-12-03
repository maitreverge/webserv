#include "ResponseBuilder.hpp"
#include "Logger.hpp"

// ------------------------- METHODS ---------------------------------

void ResponseBuilder::sanatizeURI( string &oldURI ){

	// TODO : refactor this function to avoid trimming usefull "../", as long as we don't escape the root webserv
	string needle = "../";

	std::string::size_type found = oldURI.find(needle);

	while (found != std::string::npos)
	{
		oldURI.erase(found, 3);
		found = oldURI.find(needle);
	}
}

// Need to refactor this function in `void` instead of `bool`
bool ResponseBuilder::redirectURI( void ){

	if (_myconfig.redirection.empty())
		return false;

	// TODO : Client keeps asking the same redirection over and over
	// if (_realURI == _myconfig.redirection)
	if (_myconfig.redirection != "/" and _realURI.find(_myconfig.redirection) != std::string::npos)//!
	{
		Logger::getInstance().log(ERROR, "Redirection Loop Detected");
		setError(CODE_508_LOOP_DETECTED);
	}
	
	/*
		! I had a doubt on this one, need to dig deeper when working on redirections
		Original line :
		_realURI = _myconfig.redirection + _myconfig.indexRedirection;
	*/
	_realURI = _myconfig.redirection;

	
	setError(CODE_302_FOUND);
	return true;
}

void ResponseBuilder::rootMapping( void ){

	if (_myconfig.root.empty())
		return;
	
	// slashManip(_myconfig.root);
	
	Logger::getInstance().log(INFO, "ROOT MAPPING CALLEDc");
	if (_realURI.find(_myconfig.root) == std::string::npos)
	{

		Logger::getInstance().log(DEBUG, "URI BEFORE MAPPING");
		Logger::getInstance().log(DEBUG, _realURI);

		_realURI.replace(0, _myconfig.uri.size(), _myconfig.root);
		// _realURI.insert(0, _myconfig.root);
		
		Logger::getInstance().log(DEBUG, "URI AFTER MAPPING");
		Logger::getInstance().log(DEBUG, _realURI);
	}

		// http://localhost:3027/_websites/festival/index.html

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
	
	if (target.empty())
	{

	}
	else if (target != "/")
	{
		if (beginWithSlash)
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
			target += _myconfig.index; // after checking the nature
		
		// Refresh the bool
		beginWithSlash = !target.empty() && (*target.begin() == '/');
		if (beginWithSlash and target != "/")
			target.erase(target.begin());
    }
}

void ResponseBuilder::resolveURI( void ) {
	
	// ! STEP 1 : Check the rootMapping
	rootMapping();

	// Step 2: Handle redirection
    if (redirectURI())
	{
        return;
    }
	
	// ! STEP 3 : Trim all "../" from the URI for transversal path attacks
	// sanatizeURI(_realURI); // ! STAY COMMENTED until refactoring for better "../" erasing process

	// ! STEP 4  : Deleting URI first 
	slashManip(_realURI, true);

	// ! STEP 5 : Check for DELETE + webserv
	if (_method == DELETE and _realURI == "webserv")
	{
		Logger::getInstance().log(ERROR, "User tried to delete the `webserv` binary");
		setError(CODE_403_FORBIDDEN);
	}
}

void	ResponseBuilder::checkMethod( void ){

	if (_myconfig.allowedMethods.empty())
	{
		return;
	}
	
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

	Logger::getInstance().log(DEBUG, "ResponseBuilder Get Header", inputClient);
		
	_client = &inputClient; // init client
	_config = &inputConfig; // init config

	resetMyVariables();
	
	if (codeInput != CODE_200_OK)
	{
		Logger::getInstance().log(DEBUG, "getHeader invoked from above with an error code");
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
	printMyConfig();
	
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
		{
			checkCGI();
		}

		resolveURI();
		
		checkAutho();
		
		checkNature();
		
		if (_isCGI)
			_cgi.launch(inputClient);
		
		// ! WORK NEEDLE
		if (_isDirectory and (_method == GET) and (not _isCGI))
		{
			generateListingHTML();
		}
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
	{
		setContentLenght(); // Sets up body.lenghts
	}
	
	
	if (_method == DELETE and _errorType == CODE_200_OK)
	{
		deleteEngine();	
	}

	buildHeaders();

	// TO bypass the writting process // ! cf SEB ClientBody
	if (_method == POST and _errorType != CODE_201_CREATED)
		_method = GET;

	// Copying the build Headers in headerResponse
	// ! Si on mixe les headers du CGI + de ResponseBuilder
	if (!_isCGI)
	{
		inputClient.headerRespons = Headers.masterHeader;
	}
	else
	{
		inputClient.headerRespons.clear();
	}

	// printAllHeaders();
}

