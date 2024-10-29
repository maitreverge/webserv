#include "ResponseBuilder.hpp"
#include "Logger.hpp" 

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