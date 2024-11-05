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
	
	setError(CODE_307_TEMPORARY_REDIRECT);
	return true;
}

void ResponseBuilder::rootMapping( void ){ // ✅ OKAY FUNCTION

	if (_myconfig.root.empty())
		return;
	
	_realURI.replace(0, _myconfig.uri.size(), _myconfig.root);
}

void ResponseBuilder::resolveURI( void ) {// ⛔ NOT OKAY FUNCTION
	
	// ! STEP 1 : Check the rootMapping
	rootMapping();
	
	// ! STEP 2 : Trim all "../" from the URI for transversal path attacks
	// sanatizeURI(_realURI); // ! STAY COMMENTED until refactoring for better "../" erasing process

	// ! STEP 3  : Deleting URI first 
	if (_realURI.size() > 1)
	{
		_realURI.erase(_realURI.begin() + 0); // turn a regular URI ("/index.html" into "index.html")
		
		// Removing all ending '/' URIs //! maybe useless to delete last "/" char
		
		// if ( *_realURI.rbegin() == '/' )
		// {
		// 	_realURI.erase(_realURI.size() -1);
		// }
	}
}

void	ResponseBuilder::validateURI( void ){

	// ! STEP 1 = EDGE CASES FOR EMPTY PATH or PATH == "/"
	// if (_realURI == "/" and _method == GET) // What if the resolved URI is a directory and not just "/"
	// {
	// 	// string originalURI = "/";
	// 	vector<string>::iterator it;
	// 	for (it = _config->indexFiles.begin(); it < _config->indexFiles.end(); ++it)
	// 	{
	// 		_realURI = *it;
			
	// 		// Check permissions with stat
	// 		if (stat(_realURI.c_str(), &_fileInfo) == 0)
	// 			break ;
	// 	}

	// 	if (it == _config->indexFiles.end())
	// 	{
	// 		if (errno == EACCES) // The process does not have execute permissions on one or more directories in the path.
	// 		{
	// 			setError(CODE_401_UNAUTHORIZED); return;
	// 		}
	// 		else if (errno == ENOENT) // The file or directory does not exist.
	// 		{
	// 			setError(CODE_404_NOT_FOUND); return;
	// 		}
	// 	}
	// }

	// TODO = Is URI a CGI ??
	


	if (_isDirectory and (_method == GET) and (not _isCGI))
	{
		generateListingHTML();
	}

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

	Logger::getInstance().log(DEBUG, "Response Builder GET Header", inputClient);
		
	_client = &inputClient; // init client
	_config = &inputConfig; // init config
	
	_realURI = _client->headerRequest.getURI();

	extractRouteConfig();
	// printMyConfig();

	try
	{
		if ( not redirectURI())
		{
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
			// ! WORK NEEDLE
			checkNature();
			
			validateURI();


		}
	}
	catch(const CodeErrorRaised& e)
	{
		Logger::getInstance().log(INFO, "Code Error Raised in the getHeader building process", inputClient);
	}
	catch(const std::exception& e)
	{

	}


	// {
		
	// 	// if (_isCGI and _errorType <= CODE_400_BAD_REQUEST) // or potentially another adress
	// 	// 	launchCGI();
		
	// 	checkNatureAndAuthoURI(); // double check for this Nature, if the URi has been swapped for an error file
	// 	setContentLenght();
	// }
	
	buildHeaders();

	// // Copying the build Headers in headerRespons
	// inputClient.headerRespons = Headers.masterHeader;

	// if (_method == DELETE and _errorType < CODE_400_BAD_REQUEST)
	// 	deleteEngine();	
	// Headers.masterHeader.clear();//!

	// printAllHeaders();
}
