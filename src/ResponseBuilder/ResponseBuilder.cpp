#include "ResponseBuilder.hpp"
#include "Logger.hpp" 


// // ------------------------- COPLIAN FORM -----------------------------
// ResponseBuilder::ResponseBuilder( void ) :
// 	_isDirectory(false),
// 	_isFile(false),
// 	_isCGI(false),
// 	_errorType(CODE_200_OK)	
// 	{
// 	Logger::getInstance().log(INFO, "ResponseBuilder constructor");
// 	this->_streamHead = 0;
// 	_mimeTypes.insert(std::make_pair("html", "text/html"));
// 	_mimeTypes.insert(std::make_pair("htm", "text/htm"));
// 	_mimeTypes.insert(std::make_pair("txt", "text/txt"));
// 	_mimeTypes.insert(std::make_pair("css", "text/css"));
// 	_mimeTypes.insert(std::make_pair("xml", "text/xml"));
// 	// Application Content Types
// 	_mimeTypes.insert(std::make_pair("js", "application/javascript")); // FIXME doubt on this one
// 	_mimeTypes.insert(std::make_pair("json", "application/json"));
// 	_mimeTypes.insert(std::make_pair("pdf", "application/pdf"));
// 	_mimeTypes.insert(std::make_pair("zip", "application/zip"));
// 	// Image Content Types
// 	_mimeTypes.insert(std::make_pair("jpeg", "image/jpeg"));
// 	_mimeTypes.insert(std::make_pair("jpg", "image/jpg"));
// 	_mimeTypes.insert(std::make_pair("png", "image/png"));
// 	_mimeTypes.insert(std::make_pair("gif", "image/gif"));
// 	_mimeTypes.insert(std::make_pair("webp", "image/webp"));
// 	_mimeTypes.insert(std::make_pair("bmp", "image/bmp"));
// 	// Audio Content Types
// 	_mimeTypes.insert(std::make_pair("mp3", "audio/mp3"));
// 	_mimeTypes.insert(std::make_pair("mpeg", "audio/mpeg"));
// 	_mimeTypes.insert(std::make_pair("ogg", "audio/ogg"));
// 	_mimeTypes.insert(std::make_pair("wav", "audio/wav"));
// 	// Video Content Types
// 	_mimeTypes.insert(std::make_pair("mp4", "video/mp4"));
// 	_mimeTypes.insert(std::make_pair("webm", "video/webm"));
// 	_mimeTypes.insert(std::make_pair("ogv", "video/ogv"));
// }

// ResponseBuilder::ResponseBuilder( const ResponseBuilder & rhs ) :
// 	_isDirectory(false),
// 	_isFile(false),
// 	_isCGI(false),
// 	_errorType(CODE_200_OK)	
// 	{
		
// 		this->_streamHead = rhs._streamHead;
// 		Logger::getInstance().log(INFO, "ResponseBuilder copy constructor");
// 		std::cout <<  rhs._streamHead << std::endl;
// 	// this->_client = src._client;
// 	// this->_config = src._config;
// 	// this->_mimeTypes = src._mimeTypes;
// 	// this->_headerSent = src._headerSent;

// 	// this->Headers = src.Headers;
// 	// this->_method = src._method;
// 	// this->_errorType = src._errorType;
// 	// this->_realURI = src._realURI;
// 	// this->_fileExtension = src._fileExtension;
// 	// this->_fileInfo = src._fileInfo;			
// 	// this->_isDirectory = src._isDirectory;	
// 	// this->_isFile = src._isFile;
// 	// this->_isCGI = src._isCGI;
// 	// this->_fileName = src._fileName;
// 	_mimeTypes.insert(std::make_pair("html", "text/html"));
// 	_mimeTypes.insert(std::make_pair("htm", "text/htm"));
// 	_mimeTypes.insert(std::make_pair("txt", "text/txt"));
// 	_mimeTypes.insert(std::make_pair("css", "text/css"));
// 	_mimeTypes.insert(std::make_pair("xml", "text/xml"));
// 	// Application Content Types
// 	_mimeTypes.insert(std::make_pair("js", "application/javascript")); // FIXME doubt on this one
// 	_mimeTypes.insert(std::make_pair("json", "application/json"));
// 	_mimeTypes.insert(std::make_pair("pdf", "application/pdf"));
// 	_mimeTypes.insert(std::make_pair("zip", "application/zip"));
// 	// Image Content Types
// 	_mimeTypes.insert(std::make_pair("jpeg", "image/jpeg"));
// 	_mimeTypes.insert(std::make_pair("jpg", "image/jpg"));
// 	_mimeTypes.insert(std::make_pair("png", "image/png"));
// 	_mimeTypes.insert(std::make_pair("gif", "image/gif"));
// 	_mimeTypes.insert(std::make_pair("webp", "image/webp"));
// 	_mimeTypes.insert(std::make_pair("bmp", "image/bmp"));
// 	// Audio Content Types
// 	_mimeTypes.insert(std::make_pair("mp3", "audio/mp3"));
// 	_mimeTypes.insert(std::make_pair("mpeg", "audio/mpeg"));
// 	_mimeTypes.insert(std::make_pair("ogg", "audio/ogg"));
// 	_mimeTypes.insert(std::make_pair("wav", "audio/wav"));
// 	// Video Content Types
// 	_mimeTypes.insert(std::make_pair("mp4", "video/mp4"));
// 	_mimeTypes.insert(std::make_pair("webm", "video/webm"));
// 	_mimeTypes.insert(std::make_pair("ogv", "video/ogv"));
// }

// ResponseBuilder & ResponseBuilder::operator=( const ResponseBuilder & rhs)
// {
// 	Logger::getInstance().log(INFO, "ResponseBuilder operator=");

// 	this->_streamHead = rhs._streamHead;
// 	this->_client = rhs._client;
// 	this->_config = rhs._config;
// 	this->_mimeTypes = rhs._mimeTypes;
// 	// this->_headerSent = rhs._headerSent;

// 	this->Headers = rhs.Headers;
// 	this->_method = rhs._method;
// 	this->_errorType = rhs._errorType;
// 	this->_realURI = rhs._realURI;
// 	this->_fileExtension = rhs._fileExtension;
// 	this->_fileInfo = rhs._fileInfo;			
// 	this->_isDirectory = rhs._isDirectory;	
// 	this->_isFile = rhs._isFile;
// 	this->_isCGI = rhs._isCGI;
// 	this->_fileName = rhs._fileName;	

// 	return *this;
// };

// ResponseBuilder::~ResponseBuilder( void )
// {
// 	this->_ifs.close();
// }



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

// ------------------------- COPLIAN FORM -----------------------------

ResponseBuilder::ResponseBuilder( void ){
	
	// SEB UTILS
	Logger::getInstance().log(INFO, "ResponseBuilder constructor");
	initMimes();
	
	// Init priv variables
	_isDirectory = false;
	_isFile = false;
	_isCGI = false;
	_errorType = CODE_200_OK;
	_isROK = false;
	_isWOK = false;
	_isXOK = false;

	this->_streamHead = 0; // ! NE PAS TOUCHER
	// this->test = true;
}

ResponseBuilder::ResponseBuilder( const ResponseBuilder & src)
{
	this->_streamHead = src._streamHead;
	Logger::getInstance().log(INFO, "ResponseBuilder copy constructor");
	std::cout <<  src._streamHead << std::endl;

	initMimes();

	*this = src;
}

ResponseBuilder & ResponseBuilder::operator=( const ResponseBuilder & rhs)
{
	Logger::getInstance().log(INFO, "ResponseBuilder operator=");

	initMimes();//!
	
	// Init priv variables
	_isDirectory = false;
	_isFile = false;
	_isCGI = false;
	_errorType = CODE_200_OK;
	_isROK = false;
	_isWOK = false;
	_isXOK = false;//!

	// PRIV
	this->_mimeTypes = rhs._mimeTypes;
	this->_realURI = rhs._realURI;
	this->_errorType = rhs._errorType;
	this->_method = rhs._method;
	this->_isDirectory = rhs._isDirectory;	
	this->_isFile = rhs._isFile;
	this->_isCGI = rhs._isCGI;
	this->_pathInfo = rhs._pathInfo;
	this->_isROK = rhs._isROK;
	this->_isWOK = rhs._isWOK;
	this->_isXOK = rhs._isXOK;
	this->_fileName = rhs._fileName;
	this->_fileExtension = rhs._fileExtension;

	this->_client = rhs._client;
	this->_config = rhs._config;

	this->Headers = rhs.Headers;

	// ! DO NOT FUCKING TOUCH (Kernel copy stuff)
	this->_streamHead = rhs._streamHead;
	this->_ifs.close();
	// this->test = rhs.test;
	return *this;
};

ResponseBuilder::~ResponseBuilder( void )
{
	// possiblement ne sert a rien, a voir avec Seb
	this->_ifs.close();
}


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
	// ! STEP 1 : Trim all "../" from the URI for transversal path attacks
	sanatizeURI(_realURI);

	if (_realURI.size() > 1)
	{
		_realURI.erase(_realURI.begin() + 0); // turn a regular URI ("/index.html" into "index.html")
		if ( *_realURI.rbegin() == '/' ) // Removing all ending '/' URIs
		{
			_realURI.erase(_realURI.size() -1);
		}
	}

	// TODO STEP 2 : Resolve URI with rooted path from config file
}

void	ResponseBuilder::validateURI( void ){

	// ! STEP 1 = EDGE CASES FOR EMPTY PATH or PATH == "/"
	if (_realURI.empty())
	{
		setError(CODE_404_NOT_FOUND); return;
	}
	else if (_realURI == "/") // What if the resolved URI is a directory and not just "/"
	{
		// string originalURI = "/";
		vector<string>::iterator it;
		for (it = _config->indexFiles.begin(); it < _config->indexFiles.end(); ++it)
		{
			_realURI = *it;
			
			// Check permissions with stat
			if (stat(_realURI.c_str(), &_fileInfo) == 0)
				break ;
		}

		if (it == _config->indexFiles.end())
		{
			if (errno == EACCES) // The process does not have execute permissions on one or more directories in the path.
			{
				setError(CODE_401_UNAUTHORIZED); return;
			}
			else if (errno == ENOENT) // The file or directory does not exist.
			{
				setError(CODE_404_NOT_FOUND); return;
			}
		}
	}

	// TODO = Is URI a CGI ??
	checkCGI();

	// TODO = Does the route accepts the METHOD ?
	{
		// Set un fichier par défaut comme réponse si la requête est un répertoire.
	}

	// ! STEP 2 : Identify URI nature
	
	
	checkNatureAndAuthoURI();

	if (_isDirectory and (_method == GET) and (not _isCGI))
	{
		generateListingHTML();
	}

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

	// TODO : Coockie and session generator
	{
		
	}

	// Building Final Headers
	streamMasterHeader	<< Headers.statusLine
						<< Headers.timeStamp
						<< Headers.contentType
						<< (Headers.bodyLenght ? Headers.contentLenght : Headers.transfertEncoding)
						<< HTTP_HEADER_SEPARATOR; // HEADER / BODY SEPARATOR
	
	string tempAllHeaders = streamMasterHeader.str();

	// Insert all headers in a vector char
	Headers.masterHeader.insert(Headers.masterHeader.end(), tempAllHeaders.begin(), tempAllHeaders.end());
}

void	ResponseBuilder::getHeader( Client &inputClient, Config &inputConfig ){

	Logger::getInstance().log(DEBUG, "Response Builder Get Header", inputClient);
		
	_client = &inputClient; // init client
	_config = &inputConfig; // init config
	
	extractMethod();
	
	_realURI = _client->header.getURI();

	resolveURI();
	validateURI();
	
	if (_isCGI and _errorType <= CODE_400_BAD_REQUEST) // or potentially another adress
		launchCGI();
	
	checkNatureAndAuthoURI();
	setContentLenght();
	buildHeaders();


	// Copying the build Headers in headerSend
	inputClient.headerSend = Headers.masterHeader;
	
	Headers.masterHeader.clear();//!

	// printAllHeaders();
}

ssize_t	ResponseBuilder::getBody( Client &inputClient ){

	Logger::getInstance().log(INFO, "Response Builder Get Body", inputClient);
	/*
		if ( pas de body, par exemple le 404html est pas dispo )
			return 0;
	
	*/
	if (!this->_ifs.is_open())
	{
		std::cout << "\e[103m avant" << std::endl;
		Logger::getInstance().log(INFO, _realURI.c_str(), inputClient);	
		std::cout << " apres\e[0m" << std::endl;
		this->_ifs.open(_realURI.c_str(), std::ios::binary);
		// this->test = false;
		// this->_ifs.open("test.html", std::ios::binary);
	}

	// this->_bodyStream.open(_realURI.c_str(), std::ios::binary);
// this->inputClient.->messageSend.resize(3000);
		// this->inputClient.->messageSend.resize(SEND_BUFF_SIZE);

	// ! ADVANCED TEST : keskis passe si le stream fail malgre l'URI correcte 
	if (this->_ifs.is_open())
	{
		this->_ifs.seekg(this->_streamHead);
		inputClient.messageSend.clear(); inputClient.messageSend.resize(SEND_BUFF_SIZE);//!
		// ! ADVANCED TEST : keskis passe si READ se passe mal 
		this->_ifs.read(inputClient.messageSend.data(), static_cast<std::streamsize>(inputClient.messageSend.size()));	
		this->_streamHead = this->_ifs.tellg();

		// std::string str(inputClient.messageSend.data(), static_cast<int>(this->_ifs.gcount()));	
		Logger::getInstance().log(INFO, "file open");  	
	
		std::streamsize gcount = this->_ifs.gcount();
		if (this->_ifs.eof()) 
		{
			Logger::getInstance().log(INFO, "file end", inputClient);
			this->_ifs.clear(); // Réinitialiser les flags pour continuer la lecture si besoin
			
			// this->_ifs.close();//!			
		}

		std::stringstream ss;
		ss << "gcount: " << this->_ifs.gcount();
		Logger::getInstance().log(DEBUG, ss.str(), inputClient);
		
		return static_cast<ssize_t>(gcount);
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

			exitClient();
		*/
		Logger::getInstance().log(ERROR, "Failed Stream happend", inputClient);
    }	
	return 0;
}