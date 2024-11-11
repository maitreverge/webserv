#include "ResponseBuilder.hpp"
#include "Logger.hpp"

void	ResponseBuilder::getHeaderPost( Client &inputClient, Config &inputConfig ){

	Logger::getInstance().log(DEBUG, "ResponseBuilder->getHeader", inputClient);
		
	_client = &inputClient; // init client
	_config = &inputConfig; // init config
	
	_realURI = _client->headerRequest.getURI();
	_originalURI = _client->headerRequest.getURI();

	_errorType = CODE_201_CREATED;

	extractRouteConfig();
	printMyConfig();
	
	try
	{
		extractMethod();

		checkMethod();

		if (_method == DELETE)
		{
			setError(CODE_204_NO_CONTENT, true); // does not throw exception
		}
		else
		{
			checkCGI();
		}

		if (_method == POST and !_isCGI)
		{
			uploadCheck();
		}
		
		resolveURI();
		checkAutho();
		checkNature();
		_cgi.launch(_realURI, _pathInfo);
		// ! WORK NEEDLE
		if (_isDirectory and (_method == GET) and (not _isCGI))
		{
			generateListingHTML();
		}
	}
	catch(const CodeErrorRaised& e)
	{
		Logger::getInstance().log(INFO, "Code Error Raised in the getHeader building process", inputClient);
	}
	catch(const std::exception& e)
	{
		Logger::getInstance().log(INFO, "Another kind or error has been raised in the getHeader process", inputClient);
	} 

	if (not isErrorRedirect())
	{
		setContentLenght(); // Sets up body.lenghts
	}
	
	
	if (_method == DELETE and _errorType < CODE_400_BAD_REQUEST)
	{
		deleteEngine();	
	}

	buildHeaders();


	// Copying the build Headers in headerRespons
	// ! Si on mixe les headers du CGI + de ResponseBuilder
	if (!_isCGI)
	{
		inputClient.headerRespons = Headers.masterHeader;
	}
	else
	{
		inputClient.headerRespons.clear();
	}

	printAllHeaders();
}

void	ResponseBuilder::setBodyPost( Client & client, bool eof ){

	usleep(50000);
    Logger::getInstance().log(DEBUG, "setBodyPost");

	if (this->_isCGI)	
		return this->_cgi.setBody(client, eof);
	
    // static std::ofstream ofs("uploads/image_upload.jpeg", std::ios::binary);
	if (!this->_ofs.is_open())
	{	
		Logger::getInstance().log(INFO, _realURI.c_str());	
		string pathToWrite = "uploads/apple.jpeg";
		this->_ofs.open(pathToWrite.c_str(), std::ios::binary);
			//! GERER L ERREUR
	}

	// _ofs.clear();//!
    if (_ofs.is_open()) {
		this->_ofs.seekp(0, std::ios::end);
		// this->_ofs.end();
        _ofs.write(client.messageRecv.data(), static_cast<std::streamsize>(client.messageRecv.size())); 
        _ofs.flush();
		if (!_ofs)
		{
			std::cout << "Erreur decriture dans le fichier." << std::endl;
			//! GERER L ERREUR 
		}
    } else {
        std::cout << "Erreur : impossible d'ouvrir le fichier." << std::endl;
    }
	client.messageRecv.clear();
}