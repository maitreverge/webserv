#include "Server.hpp"
#include "Logger.hpp"

void Server::listenClients()
{	
	for (size_t i = 0; i < this->_clients.size(); i++)
	{	
		if (this->_clients[i].ping
			&& FD_ISSET(this->_clients[i].fd, &this->_readSet))
		{
			this->_readBuffer.clear();
			this->_readBuffer.resize(RECV_BUFF_SIZE);
			ssize_t ret = recv(this->_clients[i].fd, this->_readBuffer.data(),
				this->_readBuffer.size(), 0);
			if (ret < 0)			
				Logger::getInstance().log(ERROR, "recv"), this->exitClient(i);	
			else if (ret == 0)					
				this->exitClient(i);		
			else
				clientMessage(i, ret);
		}	
	}
}

void Server::clientMessage(size_t i, ssize_t ret)
{
	this->_clients[i].messageRecv.
	insert(this->_clients[i].messageRecv.end(), 
	this->_readBuffer.begin(),
	this->_readBuffer.begin() + ret);				
	if (!this->_clients[i].bodySize)
		this->handleClientHeader(i, ret);
	else
		this->handleClientBody(i, ret);
}

bool Server::isDelimiterFind(size_t i, std::vector<char>::iterator & it)
{
	std::string delimiter = "\r\n\r\n";
	it = std::search
		(this->_clients[i].messageRecv.begin(),
		this->_clients[i].messageRecv.end(),
		delimiter.begin(),
		delimiter.end() - 1);
	if (it != this->_clients[i].messageRecv.end())
		return true;
	return false;
}

void Server::handleClientHeader(size_t i, ssize_t ret)
{
	stringstream ss;
	ss << "receiv client request" << " " << ret << " bytes";
	Logger::getInstance().log(INFO, ss.str(), this->_clients[i]);
		
	std::vector<char>::iterator it;		
	if (isDelimiterFind(i, it))		
	{	
		Logger::getInstance().log(DEBUG, "header terminated",
			this->_clients[i]);
		
		if (isMaxHeaderSize(it + 4, i))
			return ;				
		this->_clients[i].headerRequest.parse(this->_clients[i]);								
		this->_clients[i].headerRequest.displayParsingResult();
		getRespHeader(i);
		this->_clients[i].messageRecv.
            erase(this->_clients[i].messageRecv.begin(), it + 4);
		this->_clients[i].bodySize += this->_clients[i].messageRecv.size();
		if (!this->isContentLengthValid(i)
			|| this->isBodyTooLarge(i)
			|| this->isBodyTerminated(i))
			return ;						
	}
	else
		isMaxHeaderSize(it + 1, i);
}

void Server::getRespHeader(size_t i)
{
	if (this->_clients[i].headerRequest.getMethod() == "GET")		
		this->_clients[i].responseBuilder.getHeader(this->_clients[i],
			this->_conf);
	else if (this->_clients[i].headerRequest.getMethod() == "POST")		
		this->_clients[i].responseBuilder.getHeaderPost(this->_clients[i],
			this->_conf);
	else if (this->_clients[i].headerRequest.getMethod() == "DELETE")		
		this->_clients[i].responseBuilder.getHeader(this->_clients[i],
		this->_conf);
}

// void floSimulatorPut(std::vector<char> part)
// {	
// 	usleep(50000);
//     Logger::getInstance().log(DEBUG, "FLO POST");

//     static std::ofstream ofs("image_chat.jpeg", std::ios::binary);

// 	ofs.clear();
//     if (ofs.is_open()) {
//         ofs.write(part.data(), static_cast<std::streamsize>(part.size()));  
//         ofs.flush();
// 		if (!ofs)
// 		{
// 			std::cout << "Erreur decriture dans le fichier." << std::endl;
// 		}
//     } else {
//         std::cout << "Erreur : impossible d'ouvrir le fichier." << std::endl;
//     }
// }

void Server::handleClientBody(size_t i, ssize_t ret)
{
	stringstream ss;
	ss << "receive client body" << " " << ret << " bytes";
	Logger::getInstance().log(INFO, ss.str(), this->_clients[i]);

	this->_clients[i].bodySize += static_cast<size_t>(ret);
	if (this->isBodyTooLarge(i) || this->isBodyTerminated(i))
		return ;
	if (this->_clients[i].headerRequest.getMethod() == "POST")
	{
		// floSimulatorPut(this->_clients[i].messageRecv);
		this->_clients[i].responseBuilder.setBodyPost(this->_clients[i].messageRecv);
	}
	this->_clients[i].messageRecv.clear();	
}

#include "Error.hpp"//!
void Server::errorShortCircuit(e_errorCodes err, size_t i)
{
	std::vector<char> errVector = Error::getInstance().
		handleError(err, this->_clients[i]);
	std::string errStr(errVector.begin(), errVector.end());

	std::stringstream ss;
	ss << err << " " << errStr;
	Logger::getInstance().log(INFO, ss.str(), this->_clients[i]);	
	this->_clients[i].headerRequest.setURI(errStr);
	this->_clients[i].responseBuilder = ResponseBuilder();
	this->_clients[i].responseBuilder.getHeader(this->_clients[i],
		this->_conf);
	this->_clients[i].messageRecv.clear();
	this->_clients[i].ping = false;
	this->_clients[i].exitRequired = true;	
}

bool tog = true;
bool Server::isMaxHeaderSize(std::vector<char>::iterator it, size_t i)
{
	// if (tog)
	if (it - this->_clients[i].messageRecv.begin() > MAX_HDR_SIZE)
	{
		// tog = !tog;
		stringstream ss;
		ss << "header size" << " Actual-Size: "
            << it - this->_clients[i].messageRecv.begin()
            << " - Max-Header-Size : "	<< MAX_HDR_SIZE;

		Logger::getInstance().log(ERROR, ss.str(), this->_clients[i]);		
			//! 431 Request Header Fields Too Large !!
		errorShortCircuit(static_cast<e_errorCodes>(431), i);
		return true;	
	}
	return false;
}

bool Server::isContentLengthValid(size_t i)
{
	// if (tog)
	if (this->_clients[i].headerRequest.getHeaders().ContentLength
		> MAX_CNT_SIZE)
	{
		// tog = !tog;
		Logger::getInstance().log(ERROR, "CONTENT YEAH", this->_clients[i]);
		stringstream ss;
		ss << "max content size reached" << " - Content-Lenght: "
			<< this->_clients[i].headerRequest.getHeaders().ContentLength
			<< " - Max content size: " << MAX_CNT_SIZE << std::endl;
		Logger::getInstance().log(ERROR, ss.str(), this->_clients[i]);
			//! 413 Payload Too Large
		errorShortCircuit(static_cast<e_errorCodes>(413), i);
		return false;
	}
	return true;
}

bool Server::isBodyTooLarge(size_t i)
{
	// if (tog)
	if (this->_clients[i].bodySize >
		this->_clients[i].headerRequest.getHeaders().ContentLength)
	{
		// tog = !tog;
		stringstream ss;
		ss << "content size" << " - Body-Size: "
		    << this->_clients[i].bodySize << " Content-Lenght: "
		    << this->_clients[i].headerRequest.getHeaders().ContentLength
            << std::endl;
		Logger::getInstance().log(ERROR, ss.str(), this->_clients[i]);
			//! 413 Payload Too Large
		errorShortCircuit(static_cast<e_errorCodes>(413), i);
		return true;;
	}
	return false;
}

bool Server::isBodyTerminated(size_t i)
{
	if (this->_clients[i].bodySize ==
		this->_clients[i].headerRequest.getHeaders().ContentLength)
	{
		stringstream ss;
		ss << "client body terminated" << " - Body-Size: "
		<< this->_clients[i].bodySize << " Content-Lenght: "
		<< this->_clients[i].headerRequest.getHeaders().ContentLength;
		Logger::getInstance().log(INFO, ss.str(), this->_clients[i]);

		this->_clients[i].bodySize = 0;
		if (this->_clients[i].headerRequest.getMethod() == "POST")
		{
			// floSimulatorPut(this->_clients[i].messageRecv);
			this->_clients[i].responseBuilder.setBodyPost(this->_clients[i].messageRecv);
		}
		this->_clients[i].messageRecv.clear();
		this->_clients[i].ping = false;		
		return true;
	}
	return false;
}
