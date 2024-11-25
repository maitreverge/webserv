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
		setError(CODE_405_METHOD_NOT_ALLOWED, true);
}

void	ResponseBuilder::setContentLenght(){

	if (stat(_realURI.c_str(), &_fileInfo) == -1)
	{
		if (errno == EACCES)
			setError(CODE_401_UNAUTHORIZED, true);
		else if (errno == ENOENT or errno == EFAULT)
			setError(CODE_404_NOT_FOUND, true);
	}
	else
		Headers.bodyLenght = static_cast<uint64_t>(_fileInfo.st_size); //! the targeted file in a GET requests
}

void	ResponseBuilder::uploadCheck( void ){

	if (!_myconfig.uploadAllowed)
	{
		setError(CODE_403_FORBIDDEN);
	}
	else if (_myconfig.uploadDirectory.empty())
	{
		// ? Pertinent d'upload dans la meme URI si il n'y a pas d'UploadDirectory ?
		// TODO : Which directory to choose if the targeted one is empty ?
		
		/*
		If such directory isn't in the config file, we can take the upload directory by default
		! AND INFORM THE USER
		If the upload have any proble, we create it
		*/
		
		// ! From now, no specified uploadDirectory raise a 403
		Logger::getInstance().log(ERROR, "Upload directory is not specified");
		setError(CODE_403_FORBIDDEN);
	}
	else if ( access(_myconfig.uploadDirectory.c_str(), W_OK) == -1 )
	{
		//!  we can't write on the destination 
		Logger::getInstance().log(ERROR, "Upload directory does not have right access");
		setError(CODE_401_UNAUTHORIZED);
	}

	// Refresh URI
	_realURI = _config->errorPaths.at(_errorType);
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
				if (_isCGI and not _isXOK)
					setError(CODE_401_UNAUTHORIZED);
				else if (not _isROK)
					setError(CODE_401_UNAUTHORIZED);
				break;
			case POST:
				if (_isCGI and not _isXOK)
					setError(CODE_401_UNAUTHORIZED);
				else if (_myconfig.samePathWrite and not _isWOK) // is samePathWrite really usefull ?
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
	{
		Logger::getInstance().log(ERROR, "Check-Autho : File not Found");
		setError(CODE_404_NOT_FOUND);
	}
}

void	ResponseBuilder::extractFileNature( string &target){

	// TODO : Handle shitty names files and put default values
	// if (_method == POST)
	// {
	// 	_fileName = target;
	// }
	// else
	{
		_fileName = target.substr(target.find_last_of("/") + 1); // extract file name // DOUBT for POST
	}

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
				Logger::getInstance().log(ERROR, "Delete Method for a folder detected");
				setError(CODE_403_FORBIDDEN);
			}
		}
		else if ((_fileInfo.st_mode & S_IFMT) == S_IFREG) // checks is the given path is a FILE
		{
			_isFile = true;
			if (_method == GET)
			{
				extractFileNature( _realURI );
			}
			else
			{
				// POST AND DELETE
				if (!_isCGI and _method == POST)
					setError(CODE_401_UNAUTHORIZED);
				if (!_isCGI)
				{
					// ! ne pas ecraser l'URI si c'est un CGI
					extractFileNature( _config->errorPaths.at(_errorType) );
				}
				else
					extractFileNature( _realURI );
			}
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
		Logger::getInstance().log(INFO, "Asked error page is self generated");
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
		throw CodeErrorRaised();
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