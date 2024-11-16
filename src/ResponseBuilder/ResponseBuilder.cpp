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

bool ResponseBuilder::redirectURI( void ){

	if (_myconfig.redirection.empty())
		return false;

	// Client keeps asking the same redirection over and over
	if (_realURI == _myconfig.redirection)
		setError(CODE_508_LOOP_DETECTED);
	
	_realURI = _myconfig.redirection + _myconfig.indexRedirection;
	
	setError(CODE_302_FOUND);
	return true;
}

void ResponseBuilder::rootMapping( void ){

	if (_myconfig.root.empty())
		return;
	
	slashManip(_myconfig.root);
	
	if (_realURI.find(_myconfig.root) == std::string::npos)
		_realURI.replace(0, _myconfig.uri.size(), _myconfig.root);

}

bool ResponseBuilder::isDirectory(string &uri) {
	
	if ( (stat(uri.c_str(), &_fileInfo) == 0) and (_fileInfo.st_mode & S_IFDIR))
	{
		return true;
	}
	return false;
}

void	ResponseBuilder::slashManip( string &target ){

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
	// else if (target == "/")
	// {
	// 	// return;
	// }

	if ( isDirectory(target) )
	{
		if (!endWithSlash)
			target += "/";
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
	
	// ! STEP 2 : Trim all "../" from the URI for transversal path attacks
	// sanatizeURI(_realURI); // ! STAY COMMENTED until refactoring for better "../" erasing process

	// ! STEP 3  : Deleting URI first 
	slashManip(_realURI);

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
	
	setError(CODE_405_METHOD_NOT_ALLOWED);
}

void	ResponseBuilder::getHeader( Client &inputClient, Config &inputConfig, e_errorCodes codeInput ){

	Logger::getInstance().log(DEBUG, "ResponseBuilder->getHeader", inputClient);
		
	_client = &inputClient; // init client
	_config = &inputConfig; // init config

	if (codeInput != CODE_200_OK)
	{
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
	// printMyConfig();
	
	try
	{
		extractMethod();

		checkMethod();

		if (_method == DELETE)
		{
			setError(CODE_204_NO_CONTENT, true); // does not throw exception
		}
		else
		{
			checkCGI();
		}

		if (_method == POST and !_isCGI)
		{
			uploadCheck();
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
		Logger::getInstance().log(INFO, "Code Error Raised in the getHeader building process", inputClient);
	}
	catch(const std::exception& e)
	{
		return;
		Logger::getInstance().log(INFO, "Another kind or error has been raised in the getHeader process", inputClient);
	} 

	if (not isErrorRedirect())
	{
		setContentLenght(); // Sets up body.lenghts
	}
	
	
	if (_method == DELETE and _errorType < CODE_400_BAD_REQUEST)
	{
		deleteEngine();	
	}

	

	buildHeaders();


	// Copying the build Headers in headerRespons
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

