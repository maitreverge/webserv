#include "ResponseBuilder.hpp"
#include "Logger.hpp"

void ResponseBuilder::initMimes( void ){

	// Init Mimes Types
	{
		_mimeTypes.insert(std::make_pair("html", "text/html"));
		_mimeTypes.insert(std::make_pair("htm", "text/htm"));
		_mimeTypes.insert(std::make_pair("txt", "text/txt"));
		_mimeTypes.insert(std::make_pair("css", "text/css"));
		_mimeTypes.insert(std::make_pair("xml", "text/xml"));
		// Application Content Types
		_mimeTypes.insert(std::make_pair("js", "application/javascript")); // FIXME doubt on this one
		_mimeTypes.insert(std::make_pair("json", "application/json"));
		_mimeTypes.insert(std::make_pair("pdf", "application/pdf"));
		_mimeTypes.insert(std::make_pair("zip", "application/zip"));
		// Image Content Types
		_mimeTypes.insert(std::make_pair("jpeg", "image/jpeg"));
		_mimeTypes.insert(std::make_pair("jpg", "image/jpg"));
		_mimeTypes.insert(std::make_pair("png", "image/png"));
		_mimeTypes.insert(std::make_pair("gif", "image/gif"));
		_mimeTypes.insert(std::make_pair("webp", "image/webp"));
		_mimeTypes.insert(std::make_pair("bmp", "image/bmp"));
		_mimeTypes.insert(std::make_pair("ico", "image/x-icon"));
		// Audio Content Types
		_mimeTypes.insert(std::make_pair("mp3", "audio/mp3"));
		_mimeTypes.insert(std::make_pair("mpeg", "audio/mpeg"));
		_mimeTypes.insert(std::make_pair("ogg", "audio/ogg"));
		_mimeTypes.insert(std::make_pair("wav", "audio/wav"));
		// Video Content Types
		_mimeTypes.insert(std::make_pair("mp4", "video/mp4"));
		_mimeTypes.insert(std::make_pair("webm", "video/webm"));
		_mimeTypes.insert(std::make_pair("ogv", "video/ogv"));
	}
}

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

void	ResponseBuilder::checkAutho( void ){

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

void	ResponseBuilder::checkNature( void ){

	if (stat(_realURI.c_str(), &_fileInfo) == 0)
	{
		if ((_fileInfo.st_mode & S_IFMT) == S_IFDIR) // checks is the given path is a DIRECTORY
			_isDirectory = true;
		else if ((_fileInfo.st_mode & S_IFMT) == S_IFREG) // checks is the given path is a FILE
		{
			_isFile = true;
			_fileName = _realURI.substr(_realURI.find_last_of("/") + 1); // extract file name
			_fileExtension = _fileName.substr(_fileName.find_last_of(".") + 1); // extract file extension
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

void	ResponseBuilder::checkNatureAndAuthoURI( void ){

	checkNature();
	checkAutho();
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