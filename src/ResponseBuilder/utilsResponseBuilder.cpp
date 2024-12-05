#include "ResponseBuilder.hpp"
#include "Logger.hpp"

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

void	ResponseBuilder::setContentLenght(){

	if (stat(_realURI.c_str(), &_fileInfo) == -1)
	{
		if (errno == EACCES) // permission denied
		{
			Logger::getInstance().log(ERROR, "401 Detected from `setContentLenght` : Permission Denied");
			setError(CODE_401_UNAUTHORIZED, true);
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
					Logger::getInstance().log(ERROR, "401 Detected from `checkAutho` : CGI on GET Method has incorrect permissions");
					setError(CODE_401_UNAUTHORIZED);
				}
				else if (not _isROK)
				{
					Logger::getInstance().log(ERROR, "401 Detected from `checkAutho`, regular file on GET method has incorrect permissions");
					setError(CODE_401_UNAUTHORIZED);
				}
				break;
			case POST:
				if ( _isCGI and (not _isXOK or not _isROK) )
				{
					Logger::getInstance().log(ERROR, "401 Detected from `checkAutho` : CGI on POST Method has incorrect permissions");
					setError(CODE_401_UNAUTHORIZED);
				}
				else if (not _isWOK)
				{
					Logger::getInstance().log(ERROR, "401 Detected from `checkAutho`, regular file on POST method has incorrect permissions");
					setError(CODE_401_UNAUTHORIZED);
				}
				if (_uploadTargetDirectory.empty())
				{
					string tempURI = _originalURI;
					pathSlashs(tempURI);
					_uploadTargetDirectory =  tempURI;
				}
				break;
			case DELETE:
				if (not _isWOK)
				{
					Logger::getInstance().log(ERROR, "401 Detected from `checkAutho`, regular file on DELETE method can't be deleted due to permissions");
					setError(CODE_401_UNAUTHORIZED);
				}
				break;
			default:
				Logger::getInstance().log(ERROR, "405 Detected from `checkAutho` default switch case");
				setError(CODE_405_METHOD_NOT_ALLOWED);
				break;
		}
	}
	else
	{
		Logger::getInstance().log(ERROR, "405 Detected from `checkAutho`: File not Found");
		setError(CODE_404_NOT_FOUND);
	}
}

void	ResponseBuilder::extractFileNature( string &target){

	_fileName = target.substr(target.find_last_of("/") + 1); // extract file name // DOUBT for POST

	_fileExtension = _fileName.substr(_fileName.find_last_of(".") + 1); // extract file extension
}				

void	ResponseBuilder::checkNature( void ){

	if (stat(_realURI.c_str(), &_fileInfo) == 0)
	{
		if ((_fileInfo.st_mode & S_IFMT) == S_IFDIR) // checks is the given path is a DIRECTORY
		{
			_isDirectory = true;
			if (_method == DELETE) // ! I decided to reject DELETE methods on folders.
			{
				Logger::getInstance().log(ERROR, "405 Detected from `checkNature`: Delete Method for a folder detected");
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
					Logger::getInstance().log(ERROR, "401 Detected from `checkNature`: POST on a non-CGI file");
					setError(CODE_401_UNAUTHORIZED);
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
			Logger::getInstance().log(ERROR, "401 Detected from `checkNature`: Can't access file");
			setError(CODE_401_UNAUTHORIZED);
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
				throw exception();
		}
	}
	catch(const std::exception& e)
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

	print("=========== printAllHeaders ========");

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

	print("=========== printAllHeaders ========");
}

bool ResponseBuilder::isErrorRedirect( void ){

	if (this->_errorType >= CODE_300_MULTIPLE_CHOICES and this->_errorType < CODE_400_BAD_REQUEST)
		return true;
	return false;
}

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

string ResponseBuilder::generateFileName( void ){

	// TODO : MAKE THIS BOOL FALSE ONCE WEBSERV FINISHED
	bool testing = true;

	string baseName;

	if (testing)
		baseName = "file";
	else
		baseName = generateRandomString(10);
	
	return baseName;
}