#include "ResponseBuilder.hpp"
#include "Logger.hpp" 

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
}

ResponseBuilder::ResponseBuilder( const ResponseBuilder & src)
{
	this->_streamHead = src._streamHead;
	Logger::getInstance().log(INFO, "ResponseBuilder copy constructor");
	std::cout <<  src._streamHead << std::endl;

	initMimes();

	*this = src;
}

void ResponseBuilder::setMethod( const e_method& method )
{
	_method = method;
}


ResponseBuilder & ResponseBuilder::operator=( const ResponseBuilder & rhs)
{
	Logger::getInstance().log(INFO, "ResponseBuilder operator=");

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

bool ResponseBuilder::redirectURI( void ){

	try
	{
		_config->redirection.at(_realURI);
	}
	catch(const std::exception& e)
	{
		return false;
	}
	
	_errorType = CODE_307_TEMPORARY_REDIRECT;
	return true;
}

void ResponseBuilder::swapForRoot( void ){


}

void ResponseBuilder::resolveURI( void )
{
	// TODO STEP 2 : Resolve URI with rooted path from config file
	swapForRoot();
	
	// ! STEP 3 : Trim all "../" from the URI for transversal path attacks
	sanatizeURI(_realURI);

	if (_realURI.size() > 1)
	{
		_realURI.erase(_realURI.begin() + 0); // turn a regular URI ("/index.html" into "index.html")
		if ( *_realURI.rbegin() == '/' ) // Removing all ending '/' URIs
		{
			_realURI.erase(_realURI.size() -1);
		}
	}

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
					streamContentLenght,
					streamMasterHeader;

	
	// -------------- Madatory Headers --------------  

	streamStatusLine	<< HTTP_PROTOCOL
						<< SPACE
						<< _errorType
						<< SPACE 
						<< codes.getCode(_errorType)
						<< HTTP_HEADER_SEPARATOR;
	Headers.statusLine = streamStatusLine.str();

	streamTimeStamp		<< "Date:"
						<< SPACE 
						<< timeStamp::getTime()
						<< HTTP_HEADER_SEPARATOR;
	Headers.timeStamp = streamTimeStamp.str();
	
	// Content Lenght
	streamContentLenght	<< "Content-Length:"
						<< SPACE
						<< Headers.bodyLenght
						<< HTTP_HEADER_SEPARATOR;
	Headers.contentLenght = streamContentLenght.str();

	
	// --------------  Optionals Headers --------------  

	if (Headers.bodyLenght > 0)
	{
		stringstream streamContentType;
		
		// Content Type
		string contentType = extractType(_fileExtension);
		streamContentType	<< "Content-Type:"
							<< SPACE 
							<< contentType 
							<< HTTP_HEADER_SEPARATOR;
		
		Headers.contentType = streamContentType.str();
	}

	if (isErrorRedirect())
	{
		stringstream streamLocation;
		streamLocation	<< "Location:"
						<< SPACE
						<< _config->redirection.at(_realURI)
						// << "http://www.github.com/maitreverge"
						<< HTTP_HEADER_SEPARATOR;
		Headers.location = streamLocation.str();
	}

	// TODO : Coockie and session generator
	{
		
	}
	// ----------------- Building Final Headers ----------------

	streamMasterHeader	<< Headers.statusLine
						<< Headers.timeStamp
						<< Headers.contentType 
						// Optionals
						<< Headers.location
						// << (Headers.bodyLenght ? Headers.contentLenght : Headers.transfertEncoding)
						<< Headers.contentLenght
						<< HTTP_HEADER_SEPARATOR; // HEADER / BODY SEPARATOR
	
	string tempAllHeaders = streamMasterHeader.str();

	// Insert all headers in a vector char
	Headers.masterHeader.insert(Headers.masterHeader.end(), tempAllHeaders.begin(), tempAllHeaders.end());
}

void	ResponseBuilder::getHeader( Client &inputClient, Config &inputConfig ){

	Logger::getInstance().log(DEBUG, "Response Builder Get Header", inputClient);
		
	_client = &inputClient; // init client
	_config = &inputConfig; // init config
	
	_realURI = _client->header.getURI();

	extractMethod();
	if ( not redirectURI())
	{
		
		resolveURI();
		validateURI();
		
		if (_isCGI and _errorType <= CODE_400_BAD_REQUEST) // or potentially another adress
			launchCGI();
		
		checkNatureAndAuthoURI();
		setContentLenght();

	}
	
	buildHeaders();


	// Copying the build Headers in headerSend
	inputClient.headerSend = Headers.masterHeader;
	
	Headers.masterHeader.clear();//!

	// printAllHeaders();
}

ssize_t	ResponseBuilder::getBody( Client &inputClient ){

	Logger::getInstance().log(INFO, "Response Builder Get Body", inputClient);
	
	// Edge case where you don't need a body
	if (isErrorRedirect() ) // Code 300 Redirect
	{
		Logger::getInstance().log(INFO, "Redirect Non Body Response", inputClient);
		return 0;
	}

	if (!this->_ifs.is_open())
	{
		Logger::getInstance().log(INFO, _realURI.c_str(), inputClient);	
		this->_ifs.open(_realURI.c_str(), std::ios::binary);
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
		// Logger::getInstance().log(INFO, str);  	
	
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