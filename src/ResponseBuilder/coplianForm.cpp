/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coplianForm.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TryHardTeam <TryHardTeam@123.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:09:07 by TryHardTeam       #+#    #+#             */
/*   Updated: 2024/12/16 12:09:08 by TryHardTeam      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseBuilder.hpp"
#include "Logger.hpp" 

void ResponseBuilder::initMimes( void ){

	// Text Based Types
	_mimeTypes.insert(std::make_pair("html", "text/html"));
	_mimeTypes.insert(std::make_pair("htm", "text/htm"));
	_mimeTypes.insert(std::make_pair("txt", "text/plain"));
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

// ------------------------- COPLIAN FORM -----------------------------

void	ResponseBuilder::initServerNames( void ){

	// =========================== TEMPLATES WEBSITES ==========================

	_servernameType.insert(std::make_pair("festival.com", "_websites/festival/"));
	_servernameType.insert(std::make_pair("gaming.com", "_websites/cyborg_gaming/"));
	_servernameType.insert(std::make_pair("villa.com", "_websites/villa_agency/"));

	// =================== ENTRYPOINT FOR CORRECTION WEBSITE ===================
	
	_servernameType.insert(std::make_pair("webserv.correction", "_websites/correction/"));

}

ResponseBuilder::ResponseBuilder( void ){
	
	initMimes();

	initServerNames();

	initForbiddenFiles();

	// Init priv variables
	_isDirectory = false;
	_isFile = false;
	_isCGI = false;
	_errorType = CODE_200_OK;
	_isROK = false;
	_isWOK = false;
	_isXOK = false;
	_backupNameFile = "backup.html";

	_deleteURI = false;

	_backupNameFile = "backup.html";

	this->_ifsStreamHead = 0;

	_writeReady = false;
	_parsedBoundaryToken = false;
	_isMultipart = false;
	_setBodyExtension.clear();
	_uploadTargetDirectory.clear();

	_serverNameChecked = false;
	_isServerName = false;
	_serverName.clear();
}

void ResponseBuilder::initForbiddenFiles()
{
	_forbiddenFiles.push_back("php");
	_forbiddenFiles.push_back("py");
	_forbiddenFiles.push_back("cpp");
	_forbiddenFiles.push_back("hpp");
	_forbiddenFiles.push_back("sh");
	_forbiddenFiles.push_back("ini");
	_forbiddenFiles.push_back("out");
	_forbiddenFiles.push_back("c");
	_forbiddenFiles.push_back("o");
	_forbiddenFiles.push_back("ans");
	_forbiddenFiles.push_back("diff");
}

ResponseBuilder::ResponseBuilder( const ResponseBuilder & src) { *this = src; }

ResponseBuilder & ResponseBuilder::operator=( const ResponseBuilder & rhs)
{
	initMimes();
	initServerNames();
	
	_isDirectory = rhs._isDirectory;
	_isFile = rhs._isFile;
	_isCGI = rhs._isCGI;
	_errorType = rhs._errorType;
	_isROK = rhs._isROK;
	_isWOK = rhs._isWOK;
	_isXOK = rhs._isXOK;

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
	this->_folderCGI = rhs._folderCGI;
	this->_backupNameFile = rhs._backupNameFile;
	this->_deleteURI = rhs._deleteURI;

	this->_backupNameFile = rhs._backupNameFile;

	this->_client = rhs._client;
	this->_config = rhs._config;
	this->Headers = rhs.Headers;

	this->_writeReady = rhs._writeReady;
	this->_parsedBoundaryToken = rhs._parsedBoundaryToken;
	this->_isMultipart = rhs._isMultipart;
	this->_setBodyExtension = rhs._setBodyExtension;
	this->_uploadTargetDirectory = rhs._uploadTargetDirectory;
	this->_serverNameChecked = rhs._serverNameChecked;
	this->_serverName = rhs._serverName;
	this->_isServerName = rhs._isServerName;


	// ! DO NOT TOUCH (Kernel copy stuff)
	this->_ifsStreamHead = rhs._ifsStreamHead;
	this->_ifs.close();
	this->_ofs.close();
	this->_cgi = rhs._cgi;
	return *this;
};

void ResponseBuilder::setMethod( const e_method& method ) { _method = method; }

ResponseBuilder::~ResponseBuilder( void ){}

ResponseBuilder::e_method ResponseBuilder::getMethod( void ){ return this->_method; }