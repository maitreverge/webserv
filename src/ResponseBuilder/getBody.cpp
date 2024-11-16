#include "ResponseBuilder.hpp"
#include "Logger.hpp"

bool ResponseBuilder::getBody( Client &inputClient ){

	Logger::getInstance().log(INFO, "Response Builder Get Body", inputClient);
	if (this->_isCGI)	
		return this->_cgi.getBody(inputClient);
	
	// Edge case where you don't need a body
	if (isErrorRedirect() ) // Code 300 Redirect
	{
		Logger::getInstance().log(INFO, "Redirect Non Body Response", inputClient);
		return false;
	}

	if (!this->_ifs.is_open())
	{	
		Logger::getInstance().log(INFO, _realURI.c_str(), inputClient);	

		if (_method == POST and _isCGI)
			this->_ifs.open(_fileName.c_str(), std::ios::binary);	
		else
			this->_ifs.open(_realURI.c_str(), std::ios::binary);	
		
		//! FAIRE CTRL C PNDT UN TRANSFERT ET VALGRINDS
	}
	// ! ADVANCED TEST : keskis passe si le stream fail malgre l'URI correcte 
	if (this->_ifs.is_open())
	{
		this->_ifs.seekg(this->_ifsStreamHead);
		inputClient.messageSend.clear();
		inputClient.messageSend.resize(SEND_BUFF_SIZE);//!
		// ! ADVANCED TEST : keskis passe si READ se passe mal 
		this->_ifs.read(inputClient.messageSend.data(),
			static_cast<std::streamsize>(inputClient.messageSend.size()));	
		this->_ifsStreamHead = this->_ifs.tellg();
		
		inputClient.messageSend.erase(inputClient.messageSend.begin()
			+ this->_ifs.gcount(), inputClient.messageSend.end());
		if (this->_ifs.eof()) 
		{
			Logger::getInstance().log(INFO, "file end", inputClient);						
			this->_ifs.close();		
			
			if (_errorNotFound) // ! Potentially delete my shit
				std::remove(_realURI.c_str());
		}

		std::stringstream ss;
		ss << "gcount: " << this->_ifs.gcount();
		Logger::getInstance().log(DEBUG, ss.str(), inputClient);
		
		return false;
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
	
	return false;
}