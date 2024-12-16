#include "ResponseBuilder.hpp"
#include "Logger.hpp"

/**
 * @brief Extracts the MIME type based on the file extension.
 *
 * This function searches for the given file extension in the _mimeTypes map.
 * If the extension is found, it returns the corresponding MIME type.
 * If the extension is not found, it returns the default MIME type "application/octet-stream".
 *
 * @param extension The file extension for which to find the MIME type.
 * @return The corresponding MIME type if found, otherwise "application/octet-stream".
 */
string ResponseBuilder::extractType( const string& extension ) const {
    
    map<string, string>::const_iterator it = _mimeTypes.find(extension);
    if (it != _mimeTypes.end())
	{
        return it->second;
	}
    else
	{
        return "application/octet-stream"; // Default MIME type
	}
}

/**
 * @brief Extracts the HTTP method from the client's request and sets the internal method state.
 *
 * This function retrieves the HTTP method from the client's request header and sets the internal
 * method state accordingly. Supported methods are GET, POST, and DELETE. If the method is not
 * recognized, it logs an error and sets an error state with a 405 Method Not Allowed status.
 */
void	ResponseBuilder::extractMethod( void ){

	string tempMethod = _client->headerRequest.getMethod();

	if (tempMethod == "GET")
	{
		_method = GET;
	}
	else if (tempMethod == "POST")
	{
		_method = POST;
	}
	else if (tempMethod == "DELETE")
	{
		_method = DELETE;
	}
	else
	{
		Logger::getInstance().log(ERROR, "405 Detected from `extractMethod` : Permission Denied");
		setError(CODE_405_METHOD_NOT_ALLOWED, true);
	}
}

/**
 * @brief Sets the content length of the response based on the size of the file at the given URI.
 * 
 * This function uses the stat system call to retrieve information about the file specified by _realURI.
 * If the file cannot be accessed due to permission issues, a 403 Unauthorized error is set.
 * If the file is missing or the address is invalid, a 404 Not Found error is set.
 * If the file is successfully accessed, the content length is set to the size of the file.
 */
void	ResponseBuilder::setContentLenght(){

	if (stat(_realURI.c_str(), &_fileInfo) == -1)
	{
		if (errno == EACCES) // permission denied
		{
			Logger::getInstance().log(ERROR, "403 Detected from `setContentLenght` : Permission Denied");
			setError(CODE_403_FORBIDDEN, true);
		}
		else if (errno == ENOENT or errno == EFAULT) // Missing file or bad adress
		{
			Logger::getInstance().log(ERROR, "404 Detected from `setContentLenght` : Missing file or bad adress");
			setError(CODE_404_NOT_FOUND, true);
		}
	}
	else
		Headers.bodyLenght = static_cast<uint64_t>(_fileInfo.st_size); //! the targeted file in a GET requests
}

/**
 * @brief Checks the authorization of the requested resource based on the HTTP method.
 *
 * This function verifies the file permissions of the requested resource (_realURI) 
 * and sets the appropriate error code if the permissions are not sufficient for the 
 * requested HTTP method (GET, POST, DELETE). It logs errors and sets error codes 
 * accordingly.
 */
void	ResponseBuilder::checkAutho( void ){
	
	if (stat(_realURI.c_str(), &_fileInfo) == 0) // ! empty URI = fail
	{
		_isROK = _fileInfo.st_mode & S_IRUSR;
		_isWOK = _fileInfo.st_mode & S_IWUSR;
		_isXOK = _fileInfo.st_mode & S_IXUSR;

		switch (_method)
		{
			case GET:
				if ( _isCGI and (not _isXOK or not _isROK) )
				{
					Logger::getInstance().log(ERROR, "403 Detected from `checkAutho` : CGI on GET Method has incorrect permissions");
					setError(CODE_403_FORBIDDEN);
				}
				else if (not _isROK)
				{
					Logger::getInstance().log(ERROR, "403 Detected from `checkAutho`, regular file on GET method has incorrect permissions");
					setError(CODE_403_FORBIDDEN);
				}
				break;
			case POST:
				if ( _isCGI and (not _isXOK or not _isROK) )
				{
					Logger::getInstance().log(ERROR, "403 Detected from `checkAutho` : CGI on POST Method has incorrect permissions");
					setError(CODE_403_FORBIDDEN);
				}
				else if (not _isWOK)
				{
					Logger::getInstance().log(ERROR, "403 Detected from `checkAutho`, regular file on POST method has incorrect permissions");
					setError(CODE_403_FORBIDDEN);
				}
				if (_uploadTargetDirectory.empty())
				{
					string tempURI = _realURI;
					pathSlashs(tempURI);
					_uploadTargetDirectory =  tempURI;
				}
				break;
			case DELETE:
				if (not _isWOK)
				{
					Logger::getInstance().log(ERROR, "403 Detected from `checkAutho`, regular file on DELETE method can't be deleted due to permissions");
					setError(CODE_403_FORBIDDEN);
				}
				break;
			default:
				Logger::getInstance().log(ERROR, "405 Detected from `checkAutho` default switch case");
				setError(CODE_501_NOT_IMPLEMENTED);
				break;
		}
	}
	else
	{
		Logger::getInstance().log(ERROR, "404 Detected from `checkAutho`: File not Found");
		setError(CODE_404_NOT_FOUND);
	}
}

/**
 * @brief Extracts the file name and file extension from the given target string.
 *
 * This function takes a reference to a string representing a file path, extracts the file name
 * and file extension, and stores them in the member variables _fileName and _fileExtension respectively.
 *
 * @param target A reference to a string containing the file path.
 */
void	ResponseBuilder::extractFileNature( string &target){

	// extract file name
	_fileName = target.substr(target.find_last_of("/") + 1);

	// extract file extension
	_fileExtension = _fileName.substr(_fileName.find_last_of(".") + 1);
}				

/**
 * @brief Checks the nature of the resource specified by _realURI.
 *
 * This function determines whether the resource specified by _realURI is a file or a directory.
 * It sets the appropriate flags (_isDirectory, _isFile) and handles different HTTP methods (GET, POST, DELETE).
 * It also logs errors and sets error codes for various conditions such as unauthorized access, invalid paths, 
 * and unsupported operations on directories or non-CGI files.
 */
void	ResponseBuilder::checkNature( void ){

	if (stat(_realURI.c_str(), &_fileInfo) == 0)
	{
		if ((_fileInfo.st_mode & S_IFMT) == S_IFDIR) // checks is the given path is a DIRECTORY
		{
			_isDirectory = true;
			if (_method == DELETE) // ! I decided to reject DELETE methods on folders.
			{
				Logger::getInstance().log(ERROR, "403 Detected from `checkNature`: Delete Method for a folder detected");
				setError(CODE_403_FORBIDDEN);
			}
			else if (_method == POST)
			{
				_realURI = _config->errorPaths.at(_errorType);
				extractFileNature( _config->errorPaths.at(_errorType) );
			}
		}
		else if ((_fileInfo.st_mode & S_IFMT) == S_IFREG) // checks is the given path is a FILE
		{
			_isFile = true;
			if (_method == GET or _method == DELETE)
			{
				extractFileNature( _realURI );
			}
			else
			{
				// POST
				if (!_isCGI)
				{
					Logger::getInstance().log(ERROR, "403 Detected from `checkNature`: POST on a non-CGI file");
					setError(CODE_403_FORBIDDEN);
				}
				else
					extractFileNature( _realURI );
			}
		}
		else // The file is neither a file or a directory, then webserv can't process it
		{
			Logger::getInstance().log(ERROR, "422 Detected from `checkNature`");
			setError(CODE_422_UNPROCESSABLE_ENTITY);
			return;
		}
	}
	else // Can't access file
	{
		if (errno == EACCES)
		{
			Logger::getInstance().log(ERROR, "403 Detected from `checkNature`: Can't access file");
			setError(CODE_403_FORBIDDEN);
			return;
		}
		else if (errno == ENOENT or errno == EFAULT) // EFAULT The provided path is invalid OR points to a restricted memory space.
		{
			Logger::getInstance().log(ERROR, "404 Detected from `checkNature`: Invalid file or adress");
			setError(CODE_404_NOT_FOUND);
			return;
		}
	}
}

/**
 * @brief Sets the error state for the ResponseBuilder.
 * 
 * This function sets the error type based on the provided error code and handles
 * the generation of an error response. It checks for error redirection, attempts
 * to locate the appropriate error page, and logs warnings if the error page is not found.
 * If the error page is not accessible, it generates a default error page.
 * 
 * @param code The error code to set.
 * @param skip If true, skips additional checks and raises an exception immediately.
 * 
 * @throws CodeErrorRaised If an error redirection is required.
 * @throws Server::ShortCircuitException If skip is false, indicating an internal error.
 */
void ResponseBuilder::setError(e_errorCodes code, bool skip){

	_errorType = code;

	_isCGI = false;
	if (isErrorRedirect())
	{
		Headers.bodyLenght = 0; 
		throw CodeErrorRaised();
	}

	try
	{
		if (_errorType != CODE_204_NO_CONTENT)
		{
			_realURI = _config->errorPaths.at(_errorType);

			if (access(_realURI.c_str(), F_OK) == -1 or access(_realURI.c_str(), R_OK) )
				throw exception(); // inner function throw...
		}
	}
	catch(const std::exception& e) // ... catched here
	{
		Logger::getInstance().log(WARNING, "Required Error Page is not found. Webserv generates a new one.");
		errorNotFoundGenerator();
		setContentLenght();
	}
	
	
	if (_errorType != CODE_204_NO_CONTENT)
	{
		extractFileNature(_realURI);
		extractType(_realURI);
	}
	
	// Allows the setError function to raise an exception, and skip the useless others checks
	if (!skip)
	{
		Logger::getInstance().log(ERROR, "Internal Error raised from ResponseBuilder");
		throw Server::ShortCircuitException(code);
	}
}

void	ResponseBuilder::printAllHeaders( void ) const{
	
	print("");
	print("=========== printAllHeaders ========");
	print("");
	
	printColorNoEndl(BOLD_RED, "REAL URI : ");
	print(_realURI);

	printColorNoEndl(BOLD_GREEN, "Status Line :");
	print(Headers.statusLine);

	printColorNoEndl(BOLD_CYAN, "TimeStamp :");
	print(Headers.timeStamp);

	printColorNoEndl(BOLD_CYAN, "Content Type :");
	print(Headers.contentType);

	printColorNoEndl(BOLD_CYAN, "Location :");
	print(Headers.location);

	printColorNoEndl(BOLD_CYAN, "Content Lenght : (header)");
	print(Headers.contentLenght);

	printColorNoEndl(BOLD_BLUE, "Body Lenght (uint64_t)");
	print(Headers.bodyLenght);

	print("");
	print("=========== printAllHeaders ========");
	print("");
}

/**
 * @brief Checks if the current error type is a redirection error.
 * 
 * This function determines whether the error type falls within the range of
 * HTTP redirection status codes (300-399).
 * 
 * @return true if the error type is a redirection error, false otherwise.
 */
bool ResponseBuilder::isErrorRedirect( void ){

	if (this->_errorType >= CODE_300_MULTIPLE_CHOICES and this->_errorType < CODE_400_BAD_REQUEST)
		return true;
	return false;
}

/**
 * @brief Performs initial checks and setups for the response building process.
 *
 * This function checks the content type of the request to determine if it is multipart form data.
 * It also sets the body extension based on the content type if it is not multipart.
 * Additionally, it verifies the upload directory's existence and write permissions if specified in the configuration.
 * If the upload directory is not allowed or does not have write permissions, it sets an error code.
 */
void ResponseBuilder::extraStartingChecks()
{
	string target;
	string contentType = _client->headerRequest.getHeaders().ContentType;
	
	target.clear();

	// Detects if the current body is multipart form data
	if (contentType.find("multipart/form-data") != std::string::npos)
		_isMultipart = true;
	else if (!contentType.empty()) // Extract the _setBodyExtension
	{
		// Looks for the Content-Type within the _mimeTypes map
		// Example : Content-Type: application/pdf
		for (std::map<string, string>::iterator it = _mimeTypes.begin(); it != _mimeTypes.end(); ++it)
		{
			if (it->second == contentType)
			{
				target = it->first;
				break;
			}
		}
		if (target.empty())
			_setBodyExtension.clear();
		else
			_setBodyExtension = "." + target;
	}

	// ! STEP 2 : Determine where I'm supposed to write 

	// If there is a uploadDirectory, check the rights
	if (!_myconfig.uploadDirectory.empty())
	{
		pathSlashs(_myconfig.uploadDirectory);
		_myconfig.uploadDirectory.insert(0, _serverName);
		if (stat(_myconfig.uploadDirectory.c_str(), &_fileInfo) == 0 and (_fileInfo.st_mode & S_IFDIR))
		{
			bool uploadWrite = _fileInfo.st_mode & S_IWUSR;
			if (!uploadWrite)
			{
				Logger::getInstance().log(ERROR, "403 Detected from `extraStartingChecks`: Target Directory do not have WRITE permissions");
				setError(CODE_403_FORBIDDEN);
			}
			_uploadTargetDirectory = _myconfig.uploadDirectory;
			_myconfig.uploadAllowed = true;
		}
	}
	else if (!_myconfig.uploadAllowed) // TODO : need to test this variable
	{
		Logger::getInstance().log(ERROR, "403 Detected from `extraStartingChecks`: The route do not have `uploadAllowed` route config enabled");
		setError(CODE_403_FORBIDDEN);
	}
}

/**
 * @brief Adjusts the slashes at the beginning and end of the target path.
 *
 * This function removes a leading slash from the target string if it exists.
 * If the target represents a directory and does not end with a slash, a slash
 * is appended to the end. If the target starts with a slash after these adjustments,
 * the leading slash is removed again.
 *
 * @param target The path string to be adjusted.
 */
void ResponseBuilder::pathSlashs(string &target){

	bool beginWithSlash = !target.empty() && (*target.begin() == '/');
	bool endWithSlash = !target.empty() && (*target.rbegin() == '/');
	
	if (target.empty())
		return;
	else if (beginWithSlash)
		target.erase(target.begin());
	if ( isDirectory(target) )
	{
		if (!endWithSlash)
			target += "/";
		
		// Refresh the bool
		beginWithSlash = !target.empty() && (*target.begin() == '/');
		if (beginWithSlash)
			target.erase(target.begin());
    }
}

/**
 * @brief Generates a random string of specified length.
 *
 * This function generates a random string consisting of lowercase letters,
 * uppercase letters, and digits. If the underscoreNeeded parameter is true,
 * the generated string will start with an underscore.
 *
 * @param length The length of the random string to be generated.
 * @param underscoreNeeded A boolean flag indicating whether the generated string should start with an underscore.
 * @return A random string of the specified length.
 */
string ResponseBuilder::generateRandomString(size_t length, bool underscoreNeeded)
{
	std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::string randomString;

	if (underscoreNeeded)
		randomString += "_";

	for (size_t i = 0; i < length; ++i)
	{
		randomString += characters[static_cast<size_t>(rand()) % characters.size()];
	}
	return randomString;
}

/**
 * @brief Generates a file name for the response.
 * 
 * This function generates a file name based on the current testing mode.
 * If testing mode is enabled, it returns a fixed name "file".
 * Otherwise, it generates a random string of 10 characters as the file name.
 * 
 * @return A string representing the generated file name.
 */
string ResponseBuilder::generateFileName( void ){

	// TODO : MAKE THIS BOOL FALSE ONCE WEBSERV FINISHED
	bool testing = false;

	string baseName;

	if (testing)
		baseName = "file";
	else
		baseName = generateRandomString(10);
	
	return baseName;
}

/**
 * @brief Checks if the _fileExtension of the asked ressource is part
 * of forbidden file list as in coplianForm.cpp
 * 
 * @return true 
 * @return false 
 */
bool	ResponseBuilder::isForbiddenFiles( void ){

	for (vector<string>::iterator itVec = _forbiddenFiles.begin(); itVec != _forbiddenFiles.end(); ++itVec)
	{
		if (*itVec == _fileExtension)
		{
			Logger::getInstance().log(WARNING, "A forbidden file extension has been detected, ressource refused.");
			return true;
		}
	}

	return false;
}