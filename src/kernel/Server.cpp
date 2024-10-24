#include <Server.hpp>

Server::Server(sockaddr_in & sockAddr, int & maxFd,
fd_set & actualSet, fd_set & readSet, fd_set & writeSet)
	: _sockAddr(sockAddr), _maxFd(maxFd),
	_actualSet(actualSet), _readSet(readSet), _writeSet(writeSet) 
{	
	this->_readBuffer.reserve(BUFF_SIZE);	
	this->_writeBuffer.reserve(BUFF_SIZE);
}

const sockaddr_in & Server::getSockAdress() const
{
	return (this->_sockAddr);
}

bool Server::setup()
{	
	this->_maxFd = this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0)	
		return Logger::getInstance().log(ERROR, "socket"), false;
	FD_SET(this->_fd, &_actualSet);
	if (bind(this->_fd, reinterpret_cast<const sockaddr *>
		(&this->_sockAddr), sizeof(this->_sockAddr)) < 0)
	{
		std::stringstream ss;
		ss << "bind from socket: " << ntohs(this->_sockAddr.sin_port);
		Logger::getInstance().log(ERROR, ss.str());

		this->exitServer();		
		return false;		
	}	
	if (listen(this->_fd, this->_conf.maxClient) < 0)
	{
		Logger::getInstance().log(ERROR, "listen", *this);

		this->exitServer();
		return false;
	}	
	return true;
}

std::vector<char> buildHardResponseTest()
{
	std::stringstream ss;	
	std::string strBody =
"<html>\
<head><title>My Styled Page</title></head>\
<body style=\"background-color: #f0f0f0; text-align: center; padding: 50px;\">\
<h1 style=\"color: #ff5733; font-family: Arial, sans-serif;\">Hello, World!</h1>\
<p style=\"color: #555; font-size: 18px;\">This is a simple page with inline CSS.</p>\
</body>\
</html>";	
	ss << 
"HTTP/1.1 200 OK\r\n\
Content-Type: text/html\r\n\
Content-Length: "
	<< strBody.size() <<
"\r\n\
Connection: keep-alive\r\n\
\r\n\
" << strBody;	
	std::string strtest(ss.str()); 
	std::vector<char> res (strtest.begin(), strtest.end());
	std::vector<char> flo (strBody.begin(), strBody.end());	
	string flo_file_extension = "index.html";	
	return masterBuilder(flo, (e_errorCodes)(200), flo_file_extension);	
}

void Server::catchClients()
{	
	if (FD_ISSET(this->_fd, &this->_readSet))
	{		
		Client client;			
		client.fd = accept(this->_fd, reinterpret_cast<sockaddr *>
			(&client.address), &client.len);
		if (client.fd < 0)		
			return Logger::getInstance().log(ERROR, "accept");	

		displayClient(client);
		FD_SET(client.fd, &this->_actualSet);
		this->_maxFd = std::max(this->_maxFd, client.fd);
		this->_clients.push_back(client);
			
		std::vector<char> hardResp = buildHardResponseTest();
		replyClient(client, hardResp);		
	}
}

void Server::listenClients()
{	
	for (size_t i = 0; i < this->_clients.size(); i++)
	{	
		if (FD_ISSET(this->_clients[i].fd, &this->_readSet))
		{
			this->_readBuffer.clear();
			this->_readBuffer.resize(BUFF_SIZE);
			ssize_t ret = recv(this->_clients[i].fd, this->_readBuffer.data(),
				this->_readBuffer.size(), 0);
			if (ret < 0)
			{
				Logger::getInstance().log(ERROR, "recv");

				this->exitClient(i);				
			}
			else if (ret == 0)					
				this->exitClient(i);		
			else
			{
				this->_clients[i].message.insert(this->_clients[i].message.end(), 
					this->_readBuffer.begin(), this->_readBuffer.begin() + ret);				
				if (!this->_clients[i].bodySize)
					this->handleClientHeader(i, ret);
				else
					this->handleClientBody(i, ret);
			}
		}	
	}
}

void floSimulator(std::vector<char> part)
{
    static std::ofstream ofs("image_chat.jpeg", std::ios::binary);
    
    if (ofs.is_open()) {
        ofs.write(part.data(), static_cast<std::streamsize>(part.size()));  
        ofs.flush();
    } else {
        std::cout << "Erreur : impossible d'ouvrir le fichier." << std::endl;
    }
}

void printMessageClientTest(Client & client)
{
	std::cout << std::endl << "\e[34mmessage client: " << std::endl;		
	for (size_t i = 0; i < client.message.size(); i++)				
		std::cout << client.message[i];
	std::cout << "\e[0m" << std::endl << std::endl;
}	

void Server::handleClientHeader(size_t i, ssize_t ret)
{
	stringstream ss;
	ss << "receiv client request" << " " << ret << " bytes";
	Logger::getInstance().log(INFO, ss.str());

	std::string delimiter = "\r\n\r\n";
	std::vector<char>::iterator it = std::search
		(this->_clients[i].message.begin(),
		this->_clients[i].message.end(),
		delimiter.begin(),
		delimiter.end() - 1);		
	if (it != this->_clients[i].message.end())		
	{	
		if (isMaxHeaderSize(it + 4, i))
			return ;				
		this->_clients[i].header.parse(this->_clients[i]);								
		this->_clients[i].header.displayParsingResult();			
		this->_clients[i].message.erase(this->_clients[i].message.begin(),
			it + 4);
		this->_clients[i].bodySize += this->_clients[i].message.size();
		if (!this->isContentLengthValid(i)
			|| this->isBodyTooLarge(i)
			|| this->isBodyTerminated(i, false))
			return ;						
	}
	else
		isMaxHeaderSize(it + 1, i);
}

void Server::handleClientBody(size_t i, ssize_t ret)
{
	stringstream ss;
	ss << "receive client body" << " " << ret << " bytes";
	Logger::getInstance().log(INFO, ss.str());

	this->_clients[i].bodySize += static_cast<size_t>(ret);
	if (this->isBodyTooLarge(i) || this->isBodyTerminated(i, true))
		return ;
	if (this->_clients[i].header.getMethod() == "POST")
		floSimulator(this->_clients[i].message); //? POST
	this->_clients[i].message.clear();	
}

bool Server::isMaxHeaderSize(std::vector<char>::iterator it, size_t i)
{
	if (it - this->_clients[i].message.begin() > MAX_HDR_SIZE)
	{
		stringstream ss;
		ss << "header size" << " Actual-Size: " <<
			it - this->_clients[i].message.begin() << " - Max-Header-Size : "
			<< MAX_HDR_SIZE;

		Logger::getInstance().log(ERROR, ss.str());
			//! 431 Request Header Fields Too Large !!

		this->exitClient(i);
		return true;	
	}
	return false;
}

bool Server::isContentLengthValid(size_t i)
{
	if (this->_clients[i].header.getHeaders().ContentLength
		> MAX_CNT_SIZE)
	{
		stringstream ss;
		ss << "max content size reached" << " - Content-Lenght: "
			<< this->_clients[i].header.getHeaders().ContentLength
			<< " - Max content size: " << MAX_CNT_SIZE << std::endl;
		Logger::getInstance().log(ERROR, ss.str());
			//! 413 Payload Too Large
	
		this->exitClient(i);
		return false;
	}
	return true;
}

bool Server::isBodyTooLarge(size_t i)
{
	if (this->_clients[i].bodySize >
		this->_clients[i].header.getHeaders().ContentLength)
	{
		stringstream ss;
		ss << "content size" << " - Body-Size: "
		<< this->_clients[i].bodySize << " Content-Lenght: "
		<< this->_clients[i].header.getHeaders().ContentLength << std::endl;
		Logger::getInstance().log(ERROR, ss.str());
			//! 413 Payload Too Large
		
		this->exitClient(i);
		return true;;
	}
	return false;
}

bool Server::isBodyTerminated(size_t i, bool flag) 
{
	if (this->_clients[i].bodySize ==
		this->_clients[i].header.getHeaders().ContentLength)
	{
		stringstream ss;
		ss << "client body terminated" << " - Body-Size: "
		<< this->_clients[i].bodySize << " Content-Lenght: "
		<< this->_clients[i].header.getHeaders().ContentLength << std::endl;
		Logger::getInstance().log(INFO, ss.str());

		this->_clients[i].bodySize = 0;
		if (flag && this->_clients[i].header.getMethod() == "POST" || !flag)
			floSimulator(this->_clients[i].message);//? POST
		this->_clients[i].message.clear();		
		return true;
	}
	return false;
}

void Server::displayClient(Client & client) const
{
	std::stringstream ss;
	ss << "new client" << " - Fd: " << client.fd << " Family: "
		<< client.address.sin_family << " Addres: "
		<< inet_ntoa(client.address.sin_addr) << " Port: "
		<< ntohs(client.address.sin_port);
	Logger::getInstance().log(INFO, ss.str());
}

void Server::replyClient(Client & client, std::vector<char> & response)
{	
	this->_writeBuffer.assign(response.begin(), response.end());			
	this->_readSet = this->_writeSet = this->_actualSet;		
	if (select(this->_maxFd + 1, &this->_readSet, &this->_writeSet, 0, NULL)
		< 0)	
		return Logger::getInstance().log(ERROR, "select");	
	if(!FD_ISSET(client.fd, &this->_writeSet))	
		return Logger::getInstance().
			log(ERROR, "client not ready for response");	
	ssize_t ret;
	char * writeHead = this->_writeBuffer.data();
	size_t writeSize = this->_writeBuffer.size();
	while (writeSize > 0)
	{	
		Logger::getInstance().log(INFO, "send data to client");

		if ((ret = send(client.fd, writeHead, writeSize, 0)) < 0)		
			return Logger::getInstance().log(ERROR, "send");					
		writeHead += ret;
		writeSize -= static_cast<size_t>(ret);			
	}
}

void Server::exitClient(size_t i)
{
	Logger::getInstance().log(INFO, "client exited");

	FD_CLR(this->_clients[i].fd, &this->_actualSet);
	close(this->_clients[i].fd);	
	this->_clients.erase(this->_clients.begin() + static_cast<int>(i));
}

void Server::exitClients()
{
	for (size_t i = 0; i < this->_clients.size(); i++)
		close(this->_clients[i].fd);	
}

void Server::exitServer()
{
	this->exitClients();
	FD_CLR(this->_fd, &this->_actualSet);
	close(this->_fd);	
}