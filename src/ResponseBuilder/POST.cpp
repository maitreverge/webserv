#include "ResponseBuilder.hpp"
#include "Logger.hpp"

void	ResponseBuilder::getHeaderPost( Client &inputClient, Config &inputConfig, e_errorCodes codeInput ){

	Logger::getInstance().log(DEBUG, "ResponseBuilder->getHeader", inputClient);
		
	_client = &inputClient;
	_config = &inputConfig;

	if (codeInput != CODE_200_OK)
	{
		setError(codeInput, true);
		setContentLenght();
		buildHeaders();
		inputClient.headerRespons = Headers.masterHeader;
		return;
	}
	
	_realURI = _client->headerRequest.getURI();
	_originalURI = _realURI;

	_errorType = CODE_201_CREATED; // CODE_201 specific for POST

	extractRouteConfig();
	// printMyConfig();
	
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

		// if (_method == POST) //! pertinent ou pas
		if (_method == POST and !_isCGI)
		{
			uploadCheck();
		}
		
		resolveURI();
		checkAutho();
		checkNature();
		
		if (_isCGI)
			_cgi.launch(inputClient);
		
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

	// printAllHeaders();
}