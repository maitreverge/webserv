#include "ResponseBuilder.hpp"
#include "Logger.hpp" 

// ------------------------- COPLIAN FORM -----------------------------
ResponseBuilder::ResponseBuilder( void ) :
	_isDirectory(false),
	_isFile(false),
	_isCGI(false),
	_errorType(CODE_200_OK),
	_headerSent(false)
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

ResponseBuilder::ResponseBuilder( const ResponseBuilder & src) :
	_isDirectory(false),
	_isFile(false),
	_isCGI(false),
	_errorType(CODE_200_OK),
	_headerSent(false)
	{
	this->_client = src._client;
	this->_config = src._config;
	this->_mimeTypes = src._mimeTypes;
	this->_headerSent = src._headerSent;

	this->Headers = src.Headers;
	this->_method = src._method;
	this->_errorType = src._errorType;
	this->_realURI = src._realURI;
	this->_fileExtension = src._fileExtension;
	this->_fileInfo = src._fileInfo;			
	this->_isDirectory = src._isDirectory;	
	this->_isFile = src._isFile;
	this->_isCGI = src._isCGI;
	this->_fileName = src._fileName;
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

ResponseBuilder::~ResponseBuilder( void ){}


// ------------------------- METHODS ---------------------------------


void ResponseBuilder::sanatizeURI( string &oldURI ){

	string needle = "../";

	std::string::size_type found = oldURI.find(needle);

	while (found != std::string::npos)
	{
		oldURI.erase(found, 3);
		found = oldURI.find(needle);
	}
}

void ResponseBuilder::resolveURI( void )
{
	// /images/../config/../../logo.png

	// ! STEP 1 : Trim all "../" from the URI for transversal path attacks
	sanatizeURI(_realURI);

	// TODO STEP 2 : Resolve URI with rooted path from config file
}

void ResponseBuilder::setError(e_errorCodes code){

	_realURI = _config->errorPaths.at(CODE_404_NOT_FOUND);
	_errorType = code;
}

void	ResponseBuilder::validateURI( void ){

	// ! STEP 1 = EDGE CASES FOR EMPTY PATH or PATH == "/"
	if (_realURI.empty())
		setError(CODE_404_NOT_FOUND);
		// _errorType = CODE_404_NOT_FOUND;
	else if (_realURI == "/") // What if the resolved URI is a directory and not just "/"
	{
		vector<string>::iterator it;
		for (it = _config->indexFiles.begin(); it < _config->indexFiles.end(); ++it)
		{
			_realURI += *it;
			
			// Check permissions with stat
			if (stat(_realURI.c_str(), &_fileInfo) == 0)
				break ;
		}

		if (it == _config->indexFiles.end())
		{
			if (errno == EACCES) // The process does not have execute permissions on one or more directories in the path.
				setError(CODE_401_UNAUTHORIZED);
			else if (errno == ENOENT) // The file or directory does not exist.
				setError(CODE_404_NOT_FOUND);
		}
	}

	// ! STEP 2 : Identify URI nature
	if (stat(_realURI.c_str(), &_fileInfo) == -1) // if given path fails
	{
		if (errno == EACCES)
			setError(CODE_401_UNAUTHORIZED);
		else if (errno == ENOENT or errno == EFAULT) // EFAULT The provided path is invalid or points to a restricted memory space.
			setError(CODE_404_NOT_FOUND);
	}
	else // Supposed valid path
	{
		/*
			! STAT FLAGS : https://www.man7.org/linux/man-pages/man7/inode.7.html
		*/
		if ((_fileInfo.st_mode & S_IFMT) == S_IFDIR) // checks is the given path is a DIRECTORY
			_isDirectory = true;
		else if ((_fileInfo.st_mode & S_IFMT) == S_IFREG) // checks is the given path is a FILE
		{
			_isFile = true;
			_fileName = _realURI.substr(_realURI.find_last_of("/") + 1); // extract file name
			_fileExtension = _fileName.substr(_fileName.find_last_of(".") + 1); // extract file extension
		}
		
	}

	// TODO = Is URI a CGI ??
	{

	}
	// ! STEP 3 : Checks URI authorization depending on the method
	// TODO : Simplify or refactor this function
	stat(_realURI.c_str(), &_fileInfo); // hot fix from g++
	switch (_method)
	{
		case GET:
			if (_isCGI)
			{
				if (not( (_fileInfo.st_mode & S_IXUSR) or (_fileInfo.st_mode & S_IXGRP) or (_fileInfo.st_mode & S_IXOTH) )) // exec rights
					setError(CODE_403_FORBIDDEN);
			}
			else
			{
				if (not( (_fileInfo.st_mode & S_IRUSR) or (_fileInfo.st_mode & S_IRGRP) or (_fileInfo.st_mode & S_IROTH) )) // read rights
					setError(CODE_403_FORBIDDEN);
			}
			break;
		case POST:
			if (_isCGI)
			{
				if (not( (_fileInfo.st_mode & S_IXUSR) or (_fileInfo.st_mode & S_IXGRP) or (_fileInfo.st_mode & S_IXOTH) )) // exec rights
					setError(CODE_403_FORBIDDEN);
			}
			else
			{
				if (not( (_fileInfo.st_mode & S_IWUSR) or (_fileInfo.st_mode & S_IWGRP) or (_fileInfo.st_mode & S_IWOTH) )) // Write rights
					setError(CODE_403_FORBIDDEN);
			}
			break;
		case DELETE:
			if (not( (_fileInfo.st_mode & S_IWUSR) or (_fileInfo.st_mode & S_IWGRP) or (_fileInfo.st_mode & S_IWOTH) )) // Write rights
				setError(CODE_403_FORBIDDEN);
			break;
		default:
			setError(CODE_405_METHOD_NOT_ALLOWED);
			break;
	}

	// TODO = Does the route accepts the METHOD ?
	{
		// Set un fichier par défaut comme réponse si la requête est un répertoire.
	}


	if (_isDirectory and (_method == GET) and _config->listingDirectories)
	{
		// TODO :  generate an HTML page for listing directories
		// _realURI = getListing.html
	}
}

void ResponseBuilder::launchCGI( void ){

	// TODO
}

string ResponseBuilder::extractType( const string& extension ) const {
    
    map<string, string>::const_iterator it = _mimeTypes.find(extension);
    if (it != _mimeTypes.end())
        return it->second;
    else
        return "application/octet-stream"; // Default MIME type
}

void	ResponseBuilder::buildHeaders(){

	errorCode codes;

	stringstream	streamStatusLine,
					streamTimeStamp,
					streamContentType,
					streamContentLenght,
					streamMasterHeader,
					stream;

	streamStatusLine	<< HTTP_PROTOCOL
						<< SPACE
						<< _errorType
						<< SPACE 
						<< codes.getCode(_errorType)
						<< HTTP_HEADER_SEPARATOR;
	
	Headers.statusLine = streamStatusLine.str();

	streamTimeStamp	<< "Date:"
					<< SPACE 
					<< timeStamp::getTime() // ! TIMESTAMP
					<< HTTP_HEADER_SEPARATOR;

	Headers.timeStamp = streamTimeStamp.str();

	
	// * Content-Type (if body)
	if (Headers.bodyLenght > 0)
	{
		string contentType = extractType(_fileExtension);
		streamContentType	<< "Content-Type:"
							<< SPACE 
							<< contentType 
							<< HTTP_HEADER_SEPARATOR;
		
		Headers.contentType = streamContentType.str();
	}

	// ContentLenght
	// ! Potential condition for returning or not a body-lenght
	if (Headers.bodyLenght > 0)
	{
		streamContentLenght	<< "Content-Length:"
							<< SPACE
							<< Headers.bodyLenght
							<< HTTP_HEADER_SEPARATOR;
	}
	
	Headers.contentLenght = streamContentLenght.str();

	// Building Final Headers
	streamMasterHeader	<< Headers.statusLine
						<< Headers.timeStamp
						<< Headers.contentType
						<< (Headers.bodyLenght ? Headers.contentLenght : Headers.transfertEncoding)
						<< HTTP_REPONSE_SEPARATOR; // ! potential issue here
	
	string tempAllHeaders = streamMasterHeader.str();

	// Insert all headers in a vector char
	Headers.masterHeader.insert(Headers.masterHeader.end(), tempAllHeaders.begin(), tempAllHeaders.end());
}

void	ResponseBuilder::setContentLenght(){

	/*
		! NOTE ON INVALID 404.html FILES
		Need to handle those cases HERE
	*/
	if (stat(_realURI.c_str(), &_fileInfo) == -1)
	{
		if (errno == EACCES)
			_errorType = CODE_401_UNAUTHORIZED;
		else if (errno == ENOENT or errno == EFAULT)
			_errorType = CODE_404_NOT_FOUND;
	}
	else if (_isFile) // valid path and PATH is a file
		Headers.bodyLenght = static_cast<uint64_t>(_fileInfo.st_size);
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


void	ResponseBuilder::getHeader( Client &inputClient, Config &inputConfig ){

	Logger::getInstance().log(DEBUG, "Response Builder Get Header");
	std::cout << "CLIENT " << inputClient.fd << std::endl;
	
	if (_headerSent)
	{

		/*
			Ask Seb if he prefers a cleared vector 
		!					OR
			a empty vector
		-----------------------------------------------------
		_client.headerSend.clear();
		_client.headerSend = vector<char>();
		*/
		_client->headerSend = vector<char>();
		return;
	}
	
	_client = &inputClient; // init client
	std::cout << "CLIENT " << _client->fd << std::endl;
	_config = &inputConfig; // init config
	
	extractMethod();
	
	// _realURI = _client->header.getURI();
	_realURI = "test.html";

	resolveURI();
	validateURI();
	
	if (_isCGI )
	{
		launchCGI(); // CGI must write within a file
		// _realURI = outputCGI.html
	}
	/*
		! At this point of the function, the _readURI has ben correctly set up
		We can safely extract the ContentLenght with `stat`
	*/
	setContentLenght();
	buildHeaders();

	// Copying the build Headers in headerSend
	_client->headerSend = Headers.masterHeader;

	string temp(Headers.masterHeader.begin(), Headers.masterHeader.end() );
	cout << "HOULA IN FLO" <<  temp << endl;


	_headerSent = true;
}

ssize_t	ResponseBuilder::getBody( Client &inputClient ){

	Logger::getInstance().log(DEBUG, "Response Builder Get Body");
	std::cout << "CLIENT " << inputClient.fd << std::endl;
	std::cout << "CLIENT " << inputClient.fd << std::endl;

	if (!this->_ifs.is_open())
		this->_ifs.open("test.html", std::ios::binary);
	Logger::getInstance().log(DEBUG, "Response Builder Get Body2");
	// this->_bodyStream.open(_realURI.c_str(), std::ios::binary);
// this->inputClient.->messageSend.resize(3000);
		// this->inputClient.->messageSend.resize(SEND_BUFF_SIZE);
		Logger::getInstance().log(DEBUG, "Response Builder Get Body3");
	if (this->_ifs.is_open())
	{
				std::cout << "sssize" << inputClient.messageSend.size() << std::endl;
				Logger::getInstance().log(DEBUG, "Response Builder Get Body4");
		this->_ifs.read(inputClient.messageSend.data(), static_cast<std::streamsize>(inputClient.messageSend.size()));	
			Logger::getInstance().log(DEBUG, "avant ");	
		// std::string str(inputClient.messageSend.data(), this->_ifs.gcount());	
		// Logger::getInstance().log(INFO, str);  
		// 	Logger::getInstance().log(DEBUG, "apres ");	
		// std::string str2(inputClient.messageSend.data(), inputClient.messageSend.size());	
		// Logger::getInstance().log(INFO, str2);  
		if (this->_ifs.eof()) 
		{
			Logger::getInstance().log(DEBUG, "Fin de fichier atteinte");
			this->_ifs.clear(); // Réinitialiser les flags pour continuer la lecture si besoin
			// this->_ifs.close();

			// return false;
			// ofs.seekg(0); // Remettre le pointeur au début du fichier si tu veux recommencer
		} 
		std::cout << "gcount" << this->_ifs.gcount() << " " << this->_ifs.gcount() << std::endl;
	
		return static_cast<ssize_t>(this->_ifs.gcount());
    }
	else
	{
		/*
			! IF THE STREAM CAN'T HAPPEN
			Find a way to regenerate a Code 500 error, or at least tell the client
			Possibly just do
			----------------
			string error = "Internal error Message";
			inputClient->messageSend.insert(inputClient->messageSend.end(), error.begin(), error.end());
			----------------
			despite already sent a Content-Lenght in the headers (or possibly not)
		*/
		Logger::getInstance().log(ERROR, "Failed Stream happend");
    }
	Logger::getInstance().log(DEBUG, "Response Builder Get Body8");
	return 0;
}

// ----------- GETTERS ------------

void	ResponseBuilder::printAllHeaders( void ){

	print("=========== printAllHeaders ========");

	printColorNoEndl(BOLD_RED, "REAL URI : ")
	printColor(BLACK, _realURI);


	printColorNoEndl(BOLD_GREEN, "Status Line :");
	printColor(BLACK, Headers.statusLine);

	printColorNoEndl(BOLD_CYAN, "TimeStamp :");
	printColor(BLACK, Headers.timeStamp);

	printColorNoEndl(BOLD_CYAN, "Content Type :");
	printColor(BLACK, Headers.contentType);

	printColorNoEndl(BOLD_CYAN, "Content Lenght : (header)");
	printColor(BLACK, Headers.contentLenght);

	printColorNoEndl(BOLD_CYAN, "Body Lenght (uint64_t)");
	printColor(BLACK, Headers.bodyLenght);

	print("=========== printAllHeaders ========");
}