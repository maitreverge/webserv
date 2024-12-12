#include "ResponseBuilder.hpp"
#include "Logger.hpp"

/**
 * @brief Handles URI redirection based on the configuration.
 *
 * This function checks the redirection settings and updates the _realURI
 * accordingly. It also logs appropriate messages and sets error codes
 * in case of redirection loops or successful redirections.
 *
 * @note If both _realURI and redirection are "/", a 508 Loop Detected error is set.
 * @note If _realURI is "/" and redirection is found within _realURI, a 508 Loop Detected error is set.
 * @note If _realURI is a single "/", a regular redirection is performed and a 302 Found status is set.
 */
void	ResponseBuilder::redirectURI( void ){

	if (_myconfig.redirection.empty())
		return ;
	else if (_realURI == "/" and _myconfig.redirection == "/")
	{
		Logger::getInstance().log(ERROR, "508 LOOP : Both _realURi and Redirection == \"/\"");
		setError(CODE_508_LOOP_DETECTED);
	}
	else if (_myconfig.uri == "/" and _realURI.find(_myconfig.redirection) != std::string::npos )// ! FROM HERE, _REALURI SIZE > 1
	{
		Logger::getInstance().log(ERROR, "Redirection Loop Detected");
		setError(CODE_508_LOOP_DETECTED);
	}
	else
	{
		Logger::getInstance().log(INFO, "Regular redirection activated");
		_realURI = _myconfig.redirection;
		setError(CODE_302_FOUND);
	}
}

/**
 * @brief Maps the root directory to the real URI if it is not already mapped.
 * 
 * This function checks if the root directory specified in the configuration
 * is not empty. If the root directory is not part of the current URI, it
 * prepends the root directory to the URI. It also logs the URI before and
 * after the mapping for debugging purposes.
 */
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

/**
 * @brief Checks if the given path is a directory.
 * 
 * This function takes a string representing a file path and checks if it
 * corresponds to a directory on the filesystem.
 * 
 * @param strInput The file path to check.
 * @return true if the path is a directory, false otherwise.
 */
bool ResponseBuilder::isDirectory(string &strInput) {
	
	if ( (stat(strInput.c_str(), &_fileInfo) == 0) and (_fileInfo.st_mode & S_IFDIR))
	{
		return true;
	}
	return false;
}

/**
 * @brief Manipulates the given target string by adjusting slashes and handling redirections.
 *
 * This function performs the following operations on the target string:
 * - Removes the leading slash if the target is not "/" and starts with a slash.
 * - If the target is a directory and does not end with a slash, it either appends a slash or sets up a 302 redirection if makeRedirection is true and the method is GET.
 * - If the method is GET and directory listing is disabled, appends the index file to the target.
 *
 * @param target The string to be manipulated.
 * @param makeRedirection A boolean flag indicating whether to enable redirection.
 */
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

/**
 * @brief Resolves the URI by performing a series of checks and manipulations.
 *
 * This function performs the following steps to resolve the URI:
 * 1. Checks the root mapping.
 * 2. Handles any redirections.
 * 3. Removes any ".." traversal attacks from the URI.
 * 4. Deletes the first slash from the URI.
 * 5. Checks if the method is DELETE and the URI is "webserv", and if so, logs an error and sets a 403 Forbidden error.
 */
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

/**
 * @brief Checks if the HTTP method used in the request is allowed.
 *
 * This function verifies if the HTTP method specified in the request
 * (_method) is one of the allowed methods defined in the configuration
 * (_myconfig.allowedMethods). If the method is allowed, the function returns
 * immediately. If the method is not allowed, it logs an error and sets the
 * response error code to 405 (Method Not Allowed).
 */
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

	setError(CODE_405_METHOD_NOT_ALLOWED); //! WHY DONT USE SHORT CIRCUIT ? le client ne sera pas exited en cas de bad methode
}

/**
 * @brief Main function that articulates the entire header building process and sub-checks.
 *
 * This function is responsible for constructing the HTTP response headers based on the client's request
 * and the server's configuration. It handles various HTTP methods, error codes, and performs necessary
 * checks and operations such as session ID validation, method extraction, server name checking, CGI handling,
 * and directory listing generation.
 *
 * @param inputClient Reference to the client object containing the request details.
 * @param inputConfig Reference to the server configuration object.
 * @param codeInput Error code to be processed, if any.
 */
void	ResponseBuilder::getHeader( Client &inputClient, Config &inputConfig, e_errorCodes codeInput ){

	Logger::getInstance().log(INFO, "Build Response Header", inputClient, L_VRB);
		
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

	if (Logger::getInstance()._logLevel[INFO]
			&& Logger::getInstance()._flags & L_VRB)
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
			checkCGI();

		resolveURI();
		
		checkAutho();
		
		checkNature();
		
		if (_isCGI)
			_cgi.launch(inputClient);
		else if (isForbiddenFiles())
			setError(CODE_403_FORBIDDEN);
		
		if (_isDirectory and (_method == GET) and (not _isCGI))
			generateListingHTML();
	
	}
	catch(const CodeErrorRaised& e)
	{
		Logger::getInstance().log(DEBUG, "Code Error Raised in the getHeader building process", inputClient);
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

	if (Logger::getInstance()._logLevel[INFO]
			&& Logger::getInstance()._flags & L_VRB)
		Logger::getInstance().log(INFO, "Headers Computed", inputClient, L_VRB),
		printAllHeaders();
}

