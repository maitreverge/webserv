#include "ResponseBuilder.hpp"
#include "Logger.hpp"

bool ResponseBuilder::getBody( Client &inputClient ){

	Logger::getInstance().log(DEBUG, "Response Builder Get Body", inputClient);
	
	try
	{
		if (this->_isCGI)	
			return this->_cgi.getBody(inputClient);
	}
	catch (const exception & e)
	{
		return false;
	}
	
	if ( isErrorRedirect() )
	{
		Logger::getInstance().log(INFO, "ResponseBuilder::getBody : Redirection happended, no body is required");
		return false;
	}

	if (!this->_ifs.is_open())
	{	
		Logger::getInstance().log(DEBUG, "ResponseBuilder::getBody, the targetet URI is = " + _realURI);	

		if (_method == POST and _isCGI)
			this->_ifs.open(_fileName.c_str(), std::ios::binary);	
		else
			this->_ifs.open(_realURI.c_str(), std::ios::binary);	
		
	}

	if (this->_ifs.eof()) 
	{
		Logger::getInstance().log(DEBUG, "ResponseBuilder::getBody : Body ended");						
		this->_ifs.close();		
		
		if (_deleteURI)
		{
			std::remove(_realURI.c_str());
			Logger::getInstance().log(INFO, "ResponseBuilder::getBody : The self-generated page has been deleted");
		}
	}
	else if (this->_ifs.is_open())
	{
		this->_ifs.seekg(this->_ifsStreamHead);

		inputClient.messageSend.clear();

		inputClient.messageSend.resize(inputClient.conf->send_buff_size);

		this->_ifs.read(inputClient.messageSend.data(), static_cast<std::streamsize>(inputClient.messageSend.size()));

		this->_ifsStreamHead = this->_ifs.tellg();
		
		inputClient.messageSend.erase(inputClient.messageSend.begin() + this->_ifs.gcount(), inputClient.messageSend.end());

		std::stringstream ss;
		ss << "gcount: " << this->_ifs.gcount();
		Logger::getInstance().log(DEBUG, ss.str(), inputClient);
		
		return false;
    }
	else
	{
		Logger::getInstance().log(ERROR, "Failed Stream happend", inputClient);
		setError(CODE_500_INTERNAL_SERVER_ERROR);
    }
	
	return false;
}