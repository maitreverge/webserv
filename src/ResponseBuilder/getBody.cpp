#include "ResponseBuilder.hpp"
#include "Logger.hpp"

bool ResponseBuilder::getBody( Client &inputClient ){

	Logger::getInstance().log(DEBUG, "Response Builder Get Body", inputClient);
	try	{
		if (this->_isCGI)	
			return this->_cgi.getBody(inputClient);
	}
	catch (const exception & e)
	{
		return false;
	}
	
	// Edge case where you don't need a body
	if (isErrorRedirect() ) // Code 300 Redirect
	{
		Logger::getInstance().log(INFO, "Redirect Non Body Response", inputClient);
		return false;
	}

	if (!this->_ifs.is_open())
	{	
		Logger::getInstance().log(DEBUG, _realURI.c_str(), inputClient);	

		if (_method == POST and _isCGI) //!COND JUMP
			this->_ifs.open(_fileName.c_str(), std::ios::binary);	
		else
			this->_ifs.open(_realURI.c_str(), std::ios::binary);	
		
		//! FAIRE CTRL C PNDT UN TRANSFERT ET VALGRINDS
	}
	// ! ADVANCED TEST : keskis passe si le stream fail malgre l'URI correcte 
	if (this->_ifs.eof()) 
	{
		Logger::getInstance().log(DEBUG, "file end", inputClient);						
		this->_ifs.close();		
		
		// Delete the HTML response that has been generated 
		// such as listings.html, and backup.html (for self generated errors)
		if (_deleteURI)
			std::remove(_realURI.c_str());
	}
	else if (this->_ifs.is_open())
	{
		this->_ifs.seekg(this->_ifsStreamHead);
		inputClient.messageSend.clear();
		inputClient.messageSend.resize(inputClient.conf->send_buff_size);//!
		// ! ADVANCED TEST : keskis passe si READ se passe mal 
		this->_ifs.read(inputClient.messageSend.data(),
			static_cast<std::streamsize>(inputClient.messageSend.size()));	
		this->_ifsStreamHead = this->_ifs.tellg();
		
		inputClient.messageSend.erase(inputClient.messageSend.begin()
			+ this->_ifs.gcount(), inputClient.messageSend.end());
		/*
			! Does this EOF condition needs to be nested within this condition ?
		*/
		// if (this->_ifs.eof()) 
		// {
		// 	Logger::getInstance().log(DEBUG, "file end", inputClient);						
		// 	this->_ifs.close();		
			
		// 	if (_deleteURI) // ! Potentially delete my shit
		// 		std::remove(_realURI.c_str());
		// }

		std::stringstream ss;
		ss << "gcount: " << this->_ifs.gcount();
		Logger::getInstance().log(DEBUG, ss.str(), inputClient);
		
		return false;
    }
	else // failed stream
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