#include "ResponseBuilder.hpp"
#include "Logger.hpp"

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

void ResponseBuilder::setMethod( const e_method& method )
{
	_method = method;
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

void ResponseBuilder::initMimes( void ){

	_mimeTypes.insert(std::make_pair("html", "text/html"));
	_mimeTypes.insert(std::make_pair("htm", "text/htm"));
	_mimeTypes.insert(std::make_pair("txt", "text/txt"));
	_mimeTypes.insert(std::make_pair("css", "text/css"));
	_mimeTypes.insert(std::make_pair("xml", "text/xml"));

	// Application Content Types
	_mimeTypes.insert(std::make_pair("js", "application/javascript"));
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