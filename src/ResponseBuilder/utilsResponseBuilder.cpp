#include "ResponseBuilder.hpp"
#include "Logger.hpp"

string ResponseBuilder::extractType( const string& extension ) const { // ✅ OKAY FUNCTION
    
    map<string, string>::const_iterator it = _mimeTypes.find(extension);
    if (it != _mimeTypes.end())
        return it->second;
    else
        return "application/octet-stream"; // Default MIME type
}

void	ResponseBuilder::extractMethod( void ){ // ✅ OKAY FUNCTION

	string tempMethod = _client->headerRequest.getMethod();

	// verif already made by Dan
	if (tempMethod == "GET")
		_method = GET;
	else if (tempMethod == "POST")
		_method = POST;
	else
		_method = DELETE;
}

void	ResponseBuilder::setContentLenght(){ // ✅ OKAY FUNCTION

	string targetedAnswer = (_method == POST) ? _fileName : _realURI ; // TODO : handle non existing 404.html

	if (stat(targetedAnswer.c_str(), &_fileInfo) == -1)
	{
		if (errno == EACCES)
			setError(CODE_401_UNAUTHORIZED);
		else if (errno == ENOENT or errno == EFAULT)
			setError(CODE_404_NOT_FOUND);
	}
	else if (_method == GET and _isFile) // valid path and PATH is a file
	{
		Headers.bodyLenght = static_cast<uint64_t>(_fileInfo.st_size); //! the targeted file in a GET requests
	}
	else
		Headers.bodyLenght = static_cast<uint64_t>(_fileInfo.st_size); //! the targeted file in a GET requests
	// ! BOILERPLATE CODE
}

void	ResponseBuilder::checkAutho( void ){ // ✅ OKAY FUNCTION
	
	string targetedAnswer = (_method == POST) ? _config->errorPaths.at(_errorType) : _realURI; // TODO : handle non existing 404.html

	if (stat(targetedAnswer.c_str(), &_fileInfo) == 0)
	{
		_isROK = _fileInfo.st_mode & S_IRUSR;
		_isWOK = _fileInfo.st_mode & S_IWUSR;
		_isXOK = _fileInfo.st_mode & S_IXUSR;

		switch (_method)
		{
			case GET:
				if (_isCGI and not _isXOK)
					setError(CODE_401_UNAUTHORIZED);
				else if (not _isROK)
					setError(CODE_401_UNAUTHORIZED);
				break;
			case POST:
				if (_isCGI and not _isXOK)
					setError(CODE_401_UNAUTHORIZED);
				else if (not _isWOK)
					setError(CODE_401_UNAUTHORIZED);
				break;
			case DELETE:
				if (not _isWOK)
					setError(CODE_401_UNAUTHORIZED);
				break;
			default:
				setError(CODE_405_METHOD_NOT_ALLOWED);
				break;
		}
	}
	else
		setError(CODE_404_NOT_FOUND);
}

void	ResponseBuilder::extractFileNature( string &target){ // ✅ OKAY FUNCTION

	// TODO : Handle shitty names files and put default values
	if (_method == POST)
		_fileName = target;
	else
		_fileName = target.substr(target.find_last_of("/") + 1); // extract file name // DOUBT for POST

	_fileExtension = _fileName.substr(_fileName.find_last_of(".") + 1); // extract file extension
}				

void	ResponseBuilder::checkNature( void ){ // ✅ OKAY FUNCTION

	string targetedAnswer = (_method == POST) ? _config->errorPaths.at(_errorType) : _realURI; // TODO : handle non existing 404.html

	if (stat(targetedAnswer.c_str(), &_fileInfo) == 0)
	{
		if ((_fileInfo.st_mode & S_IFMT) == S_IFDIR) // checks is the given path is a DIRECTORY
			_isDirectory = true;
		else if ((_fileInfo.st_mode & S_IFMT) == S_IFREG) // checks is the given path is a FILE
		{
			_isFile = true;
			if (_method == GET)
				extractFileNature( _realURI );
			else // POST AND DELETE
				extractFileNature( _config->errorPaths.at(_errorType) );
			// _fileName = _realURI.substr(_realURI.find_last_of("/") + 1); // extract file name
			// _fileExtension = _fileName.substr(_fileName.find_last_of(".") + 1); // extract file extension
		}
		else
		{
			// TODO : Do I need to set up an error code if the ressource is neither a file or a directory
			setError(CODE_422_UNPROCESSABLE_ENTITY); return;
		}
	}
	else // Can't access file
	{
		if (errno == EACCES)
		{
			setError(CODE_401_UNAUTHORIZED); return;
		}
		else if (errno == ENOENT or errno == EFAULT) // EFAULT The provided path is invalid or points to a restricted memory space.
		{
			setError(CODE_404_NOT_FOUND); return;
		}
	}
}

void	ResponseBuilder::checkNatureAndAuthoURI( void ){ // ✅ OKAY FUNCTION

	checkAutho();
	checkNature();
}

void ResponseBuilder::setError(e_errorCodes code){ // ✅ OKAY FUNCTION

	_errorType = code;
	
	if (_errorType == CODE_204_NO_CONTENT)
		return;
	
	if (_method == POST)
	{
		extractFileNature(_config->errorPaths.at(_errorType));
	}
	else
		_realURI = _config->errorPaths.at(_errorType); // TODO : handle non existing 404.html
}

void	ResponseBuilder::printAllHeaders( void ) const{ // ⛔ NOT OKAY FUNCTION (need refactoring and polish)

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

bool ResponseBuilder::isErrorRedirect( void ){ // ✅ OKAY FUNCTION

	if (this->_errorType >= CODE_300_MULTIPLE_CHOICES and this->_errorType < CODE_400_BAD_REQUEST)
		return true;
	return false;
}