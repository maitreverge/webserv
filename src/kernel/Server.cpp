#include <Server.hpp>

Server::Server(sockaddr_in & sockAddr, int & maxFd,
fd_set & actualSet, fd_set & readSet, fd_set & writeSet)
	: _sockAddr(sockAddr), _maxFd(maxFd),
	_actualSet(actualSet), _readSet(readSet), _writeSet(writeSet) 
{	
	this->_readBuffer.reserve(BUFF_SIZE);	
	this->_writeBuffer.reserve(BUFF_SIZE);
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
		Logger::getInstance().log(ERROR, "listen");
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
	printColor(BOLD_RED, strBody.size());
	std::string strtest(ss.str()); 
	std::vector<char> res (strtest.begin(), strtest.end());
	std::vector<char> flo (strBody.begin(), strBody.end());
	
	string flo_file_extension = "index.html";	
	return masterBuilder(flo, (e_errorCodes)(200), flo_file_extension);
	// return res;
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
				if (!this->_clients[i].body)
					this->handleClientRequest(i, ret);
				else
					this->handleClientBody(i, ret);
			}
		}	
	}
}

// void floSimulator(std::vector<char> part)
// {
// 	static ofstream ofs("\\image_chat.jpeg", std::ios::binary);	
// 	ofs.write(part.data(), part.size());
// 	ofs.flush();
// }

void floSimulator(std::vector<char> part)
{
    static std::ofstream ofs("image_chat.jpeg", std::ios::binary);
    
    if (ofs.is_open()) {
        ofs.write(part.data(), part.size());  // Ecrire chaque bloc de données dans le fichier
        ofs.flush();  // S'assurer que toutes les données sont écrites sur le disque immédiatement
    } else {
        std::cerr << "Erreur : impossible d'ouvrir le fichier." << std::endl;
    }
}

void Server::handleClientBody(size_t i, ssize_t ret)
{
	std::cout << "sequel client body: " << ret << std::endl;

	this->_clients[i].message.insert(this->_clients[i].message.end(), 
		this->_readBuffer.begin(), this->_readBuffer.begin() + ret);
	
	this->_clients[i].bodySize += ret;
	if (this->_clients[i].bodySize > this->_parser.getHeaders().ContentLength)
	{
		std::cout << "sequel error content size" << " bs: " << this->_clients[i].bodySize << " cl: " << this->_parser.getHeaders().ContentLength << std::endl;
		this->exitClient(i);
		this->_readBuffer.clear();
		return ;
	}

	std::cout << "\e[36msequel body client: " << std::endl;		
	// for (size_t j = 0; j < this->_clients[i].message.size(); j++)				
	// 	std::cout << this->_clients[i].message[j];
	std::cout << "\e[0m" << std::endl;

	if (this->_clients[i].bodySize == this->_parser.getHeaders().ContentLength)
	{
		std::cout << "sequel body termined" << " var size " << this->_clients[i].message.size() << " bodysize " << this->_clients[i].bodySize << " ctnt len " << this->_parser.getHeaders().ContentLength << std::endl;
		floSimulator(this->_clients[i].message);
		this->_clients[i].body = false;
		this->_clients[i].message.clear();
		this->_clients[i].bodySize = 0;
		this->_readBuffer.clear();
	}
	floSimulator(this->_clients[i].message);
	this->_clients[i].message.clear();
	// flo this->parser, message!
}

void Server::handleClientRequest(size_t i, ssize_t ret)
{
	std::cout << "client header: " << ret << std::endl;
	
	if (ret + static_cast<ssize_t>(this->_clients[i].message.size())
		> MAX_HDR_SIZE)
	{
		Logger::getInstance().log(ERROR, "header size"); //! 431 Request Header Fields Too Large	
		this->exitClient(i);
		return;	
	}
	
	this->_clients[i].message.insert(this->_clients[i].message.end(), 
		this->_readBuffer.begin(), this->_readBuffer.begin() + ret);

	// std::cout << "\e[34mmessage client: " << std::endl;		
	// for (size_t j = 0; j < this->_clients[i].message.size(); j++)				
	// 	std::cout << this->_clients[i].message[j];
	// std::cout << "\e[0m" << std::endl;	
	
	std::string delimiter = "\r\n\r\n";
	std::vector<char>::iterator it = std::search
		(this->_clients[i].message.begin(),
		this->_clients[i].message.end(),
		delimiter.begin(),
		delimiter.end() - 1);
		
	if (it != this->_clients[i].message.end())
	{						
		this->_parser.parse(this->_clients[i]);								
		// this->_parser.displayParsingResult();
		if (this->_parser.getMethod() == "POST")
		{
			this->_clients[i].body = true;
			printColor(RED, "yeah POST!!!");
			std::cout << "CONTENT LENGHT: " <<  this->_parser.getHeaders().ContentLength << std::endl;

			this->_clients[i].message.erase(this->_clients[i].message.begin(),
				it + 4);

			std::cout << "\e[34mstart body: " << std::endl;		
			// for (size_t j = 0; j < this->_clients[i].message.size(); j++)				
			// 	std::cout << this->_clients[i].message[j];
			std::cout << "\e[0m" << std::endl;

			if (this->_parser.getHeaders().ContentLength > MAX_CNT_SIZE)
			{
				std::cout << "error max content size" << " cl: " << this->_parser.getHeaders().ContentLength << std::endl; //!
				this->_readBuffer.clear();
				this->exitClient(i);
				return ;
			}

			this->_clients[i].bodySize += this->_clients[i].message.size();
			if (this->_clients[i].bodySize > this->_parser.getHeaders().ContentLength)
			{
				std::cout << "error content size" << " bs: " << this->_clients[i].bodySize << " cl: " << this->_parser.getHeaders().ContentLength << std::endl;
				this->_readBuffer.clear();
				this->exitClient(i);
				return ;
			}

			if (this->_clients[i].bodySize == this->_parser.getHeaders().ContentLength)
			{
				std::cout << "body termined" << std::endl;
				floSimulator(this->_clients[i].message);
				this->_clients[i].body = false;
				this->_clients[i].message.clear();
				this->_clients[i].bodySize = 0;				
			}
			floSimulator(this->_clients[i].message);
			// flo this->parser, message!
		}		
		this->_clients[i].message.clear();		
	}
	this->_readBuffer.clear();
	
}

void Server::displayClient(Client & client)
{
	std::stringstream ss;
	ss << "new client:" << "fd: " << client.fd << " family: "
		<< client.address.sin_family << " addres: "
		<< inet_ntoa(client.address.sin_addr) << " port: "
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
		Logger::getInstance().log(INFO, "debug send data to client");
		if ((ret = send(client.fd, writeHead, writeSize, 0)) < 0)		
			return Logger::getInstance().log(WARNING, "send");		
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