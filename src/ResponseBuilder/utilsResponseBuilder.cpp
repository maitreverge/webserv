#include "ResponseBuilder.hpp"
#include "Logger.hpp"

string ResponseBuilder::extractType( const string& extension ) const {
    
    map<string, string>::const_iterator it = _mimeTypes.find(extension);
    if (it != _mimeTypes.end())
        return it->second;
    else
        return "application/octet-stream"; // Default MIME type
}

void	ResponseBuilder::extractMethod( void ){

	string tempMethod = _client->header.getMethod();

	// verif already made by Dan
	if (tempMethod == "GET")
		_method = GET;
	else if (tempMethod == "POST")
		_method = POST;
	else
		_method = DELETE;
}

void	ResponseBuilder::setContentLenght(){

	/*
		! NOTE ON INVALID 404.html FILES
		Need to handle those cases HERE
	*/
	if (stat(_realURI.c_str(), &_fileInfo) == -1)
	{
		if (errno == EACCES)
			setError(CODE_401_UNAUTHORIZED);
		else if (errno == ENOENT or errno == EFAULT)
			setError(CODE_404_NOT_FOUND);
	}
	else if (_isFile) // valid path and PATH is a file
		Headers.bodyLenght = static_cast<uint64_t>(_fileInfo.st_size);
}

void	ResponseBuilder::extractAuthorizations( void ){

	if (stat(_realURI.c_str(), &_fileInfo) == 0)
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

void ResponseBuilder::setError(e_errorCodes code){

	_realURI = _config->errorPaths.at(code);
	_errorType = code;
}

void	ResponseBuilder::printAllHeaders( void ){

	print("=========== printAllHeaders ========");

	printColorNoEndl(BOLD_RED, "REAL URI : ");
	print(_realURI);


	printColorNoEndl(BOLD_GREEN, "Status Line :");
	print(Headers.statusLine);

	printColorNoEndl(BOLD_CYAN, "TimeStamp :");
	print(Headers.timeStamp);

	printColorNoEndl(BOLD_CYAN, "Content Type :");
	print(Headers.contentType);

	printColorNoEndl(BOLD_CYAN, "Content Lenght : (header)");
	print(Headers.contentLenght);

	printColorNoEndl(BOLD_BLUE, "Body Lenght (uint64_t)");
	print(Headers.bodyLenght);

	print("=========== printAllHeaders ========");
}