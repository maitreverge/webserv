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
	// Create a socket and set it as the maximum file descriptor
	this->_maxFd = this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0)
		return Logger::getInstance().log(ERROR, "socket"), false;

	// Add the socket file descriptor to the actual set
	FD_SET(this->_fd, &_actualSet);

	// Bind the socket to the provided address
	if (bind(this->_fd, reinterpret_cast<const sockaddr *>(&this->_sockAddr), sizeof(this->_sockAddr)) < 0)
	{
		std::stringstream ss;
		ss << "bind from socket: " << ntohs(this->_sockAddr.sin_port);
		Logger::getInstance().log(ERROR, ss.str());
		this->exitServer();
		return false;
	}

	// Listen for incoming connections with a maximum queue length
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
		
		// Accept the incoming connection and assign the client's file descriptor
		client.fd = accept(this->_fd, reinterpret_cast<sockaddr *>
			(&client.address), &client.len);
		
		// If the accept call fails, log an error message and return
		if (client.fd < 0)		
			return Logger::getInstance().log(ERROR, "accept");		
		displayClient(client);
		
		// Add the client's file descriptor to the set of active file descriptors
		FD_SET(client.fd, &this->_actualSet);
		
		// Update the maximum file descriptor value if necessary
		this->_maxFd = std::max(this->_maxFd, client.fd);
		
		// Add the new client to the list of clients
		this->_clients.push_back(client);
		
		// Build a hardcoded response and send it to the client
		std::vector<char> hardResp = buildHardResponseTest();
		replyClient(client, hardResp);		
	}
}

void Server::listenClients()
{
	 // Iterate through the list of connected clients
	for (size_t i = 0; i < this->_clients.size(); i++)
	{	
		// Check if the client's file descriptor is set in the read set
		if (FD_ISSET(this->_clients[i].fd, &this->_readSet))
		{
			// Resize the read buffer to the predefined buffer size
			this->_readBuffer.resize(BUFF_SIZE);
			
			// Receive data from the client and store it in the read buffer
			ssize_t ret = recv(this->_clients[i].fd, this->_readBuffer.data(),
				this->_readBuffer.size(), 0);
			
			// If the recv call fails, log an error message and disconnect the client
			if (ret < 0)
			{
				Logger::getInstance().log(ERROR, "recv");
				this->exitClient(i);				
			}
			// If the recv call returns 0, indicating the client has closed the connection, disconnect the client
			else if (ret == 0)					
				this->exitClient(i);
			// If data is successfully received, process the client's request		
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
        ofs.write(part.data(), part.size());  
        ofs.flush();
    } else {
        std::cout << "Erreur : impossible d'ouvrir le fichier." << std::endl;
    }
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

	// Append the received data to the client's message buffer
	this->_clients[i].message.insert(this->_clients[i].message.end(), 
		this->_readBuffer.begin(), this->_readBuffer.begin() + ret);

	// std::cout << "\e[34mmessage client: " << std::endl;		
	// for (size_t j = 0; j < this->_clients[i].message.size(); j++)				
	// 	std::cout << this->_clients[i].message[j];
	// std::cout << "\e[0m" << std::endl;	
	
	// Check if the client's message contains the end-of-header delimiter (\r\n\r\n)
	std::string delimiter = "\r\n\r\n";
	std::vector<char>::iterator it = std::search
		(this->_clients[i].message.begin(),
		this->_clients[i].message.end(),
		delimiter.begin(),
		delimiter.end() - 1);
		
	if (it != this->_clients[i].message.end())
	{	
		std::string str(it, this->_clients[i].message.end());
		// std::cout << "\e[36mFIND!!!!!!!!\n\n" << str << "\e[0m" <<std::endl;				
		this->_clients[i].header.parse(this->_clients[i]);								
		this->_clients[i].header.displayParsingResult();
		if (this->_clients[i].header.getMethod() == "POST")
		{
			this->_clients[i].body = true;
			printColor(RED, "yeah POST!!!");
			std::cout << "CONTENT LENGHT: " <<  this->_clients[i].header.getHeaders().ContentLength << std::endl;

			this->_clients[i].message.erase(this->_clients[i].message.begin(),
				it + 4);

			std::cout << "\e[34mstart body: " << std::endl;		
			// for (size_t j = 0; j < this->_clients[i].message.size(); j++)				
			// 	std::cout << this->_clients[i].message[j];
			std::cout << "\e[0m" << std::endl;

			if (this->_clients[i].header.getHeaders().ContentLength > MAX_CNT_SIZE)
			{
				std::cout << "error max content size" << " cl: " << this->_clients[i].header.getHeaders().ContentLength << std::endl; //!
				this->_readBuffer.clear();
				this->exitClient(i);
				return ;
			}

			this->_clients[i].bodySize += this->_clients[i].message.size();
			if (this->_clients[i].bodySize > this->_clients[i].header.getHeaders().ContentLength)
			{
				std::cout << "error content size" << " bs: " << this->_clients[i].bodySize << " cl: " << this->_clients[i].header.getHeaders().ContentLength << std::endl;
				this->_readBuffer.clear();
				this->exitClient(i);
				return ;
			}

			if (this->_clients[i].bodySize == this->_clients[i].header.getHeaders().ContentLength)
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

	// Clear the read buffer for the next read operation
	this->_readBuffer.clear();
	
}

void Server::handleClientBody(size_t i, ssize_t ret)
{
	std::cout << "sequel client body: " << ret << std::endl;

	this->_clients[i].message.insert(this->_clients[i].message.end(), 
		this->_readBuffer.begin(), this->_readBuffer.begin() + ret);
	
	this->_clients[i].bodySize += ret;
	if (this->_clients[i].bodySize > this->_clients[i].header.getHeaders().ContentLength)
	{
		std::cout << "sequel error content size" << " bs: " << this->_clients[i].bodySize << " cl: " << this->_clients[i].header.getHeaders().ContentLength << std::endl;
		this->exitClient(i);
		this->_readBuffer.clear();
		return ;
	}

	std::cout << "\e[36msequel body client: " << std::endl;		
	// for (size_t j = 0; j < this->_clients[i].message.size(); j++)				
	// 	std::cout << this->_clients[i].message[j];
	std::cout << "\e[0m" << std::endl;

	if (this->_clients[i].bodySize == this->_clients[i].header.getHeaders().ContentLength)
	{
		std::cout << "sequel body termined" << " var size " << this->_clients[i].message.size() << " bodysize " << this->_clients[i].bodySize << " ctnt len " << this->_clients[i].header.getHeaders().ContentLength << std::endl;
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
	// Assign the response data to the _writeBuffer
	this->_writeBuffer.assign(response.begin(), response.end());			
	
	// Copy the current set of file descriptors to the read and write sets
	this->_readSet = this->_writeSet = this->_actualSet;		
	
	// Monitor the file descriptors for readiness
	if (select(this->_maxFd + 1, &this->_readSet, &this->_writeSet, 0, NULL)
		< 0)	
		return Logger::getInstance().log(ERROR, "select");	
	
	// Check if the client's file descriptor is ready for writing
	if (!FD_ISSET(client.fd, &this->_writeSet))	
		return Logger::getInstance().
			log(ERROR, "client not ready for response");	
	ssize_t ret;
	char * writeHead = this->_writeBuffer.data();
	size_t writeSize = this->_writeBuffer.size();
	
	// Send the response data to the client in a loop until all data is sent
	while (writeSize > 0)
	{	
		Logger::getInstance().log(INFO, "debug send data to client");
		
		// Attempt to send data to the client
		if ((ret = send(client.fd, writeHead, writeSize, 0)) < 0)		
			return Logger::getInstance().log(WARNING, "send");		
		
		// Update the write buffer pointer and size after each successful send
		writeHead += ret;
		writeSize -= static_cast<size_t>(ret);			
	}
}

void Server::exitClient(size_t i)
{
	Logger::getInstance().log(INFO, "client exited");
	
	// Remove the client's file descriptor from the active set
	FD_CLR(this->_clients[i].fd, &this->_actualSet);
	
	// Close the client's socket
	close(this->_clients[i].fd);
	
	// Erase the client from the clients vector
	this->_clients.erase(this->_clients.begin() + static_cast<int>(i));
}

void Server::exitClients()
{
	for (size_t i = 0; i < this->_clients.size(); i++)
		close(this->_clients[i].fd);	
}

void Server::exitServer()
{
	// Disconnect all connected clients
	this->exitClients();
	// Remove the server's file descriptor from the active set
	FD_CLR(this->_fd, &this->_actualSet);
	// Close the server's socket
	close(this->_fd);	
}