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
			setError(CODE_204_NO_CONTENT); // does not throw exception
		else // if (_method != DELETE)
			checkCGI();
		if (_method == POST and !_isCGI)
			uploadCheck();
		
		resolveURI();
		checkAutho();
		checkNature();
		
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
		setContentLenght();
	
	
	if (_method == DELETE and _errorType < CODE_400_BAD_REQUEST)
		deleteEngine();	

	buildHeaders();

	inputClient.headerRespons = Headers.masterHeader;
	

	// Headers.masterHeader.clear();//!

	printAllHeaders();

}

void	ResponseBuilder::setBodyPost( std::vector<char> bodyParts ){

	usleep(50000);
    Logger::getInstance().log(DEBUG, "setBodyPost");

    // static std::ofstream ofs("uploads/image_upload.jpeg", std::ios::binary);
	if (!this->_ofs.is_open())
	{	
		Logger::getInstance().log(INFO, _realURI.c_str());	
		string pathToWrite = "uploads/apple.jpeg";
		this->_ofs.open(pathToWrite.c_str(), std::ios::binary);
	}

	// _ofs.clear();//!
    if (_ofs.is_open()) {
		this->_ofs.seekp(0, std::ios::end);
		// this->_ofs.end();
        _ofs.write(bodyParts.data(), static_cast<std::streamsize>(bodyParts.size())); 
			// this->_ofsStreamHead = this->_ofs.tellg(); 
        _ofs.flush();
		if (!_ofs)
		{
			std::cout << "Erreur decriture dans le fichier." << std::endl;
		}
    } else {
        std::cout << "Erreur : impossible d'ouvrir le fichier." << std::endl;
    }
}