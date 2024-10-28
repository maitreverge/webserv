#include "Server.hpp"
#include "Logger.hpp" 


Server::Server(sockaddr_in & sockAddr, int & maxFd,
fd_set & actualSet, fd_set & readSet, fd_set & writeSet)
	: _sockAddr(sockAddr), _maxFd(maxFd),
	_actualSet(actualSet), _readSet(readSet), _writeSet(writeSet) 
{		
	this->_readBuffer.reserve(BUFF_SIZE);	
	this->_writeBuffer.reserve(BUFF_SIZE);
	this->_writeBuffer.resize(BUFF_SIZE);//!
}

const sockaddr_in & Server::getSockAdress() const
{
	return (this->_sockAddr);
}

Client::Client()
{
	Logger::getInstance().log(INFO, "\e[30;101mclient created\e[0m", *this);

	id = 0;
	fd = 0;		
	memset(&address, 0, sizeof(address));
	len = sizeof(address);
	statusCode = CODE_200_OK;
	len = sizeof(address);	
	bodySize = 0;
	messageSend.reserve(MAX_HDR_SIZE);
	messageSend.resize(SEND_BUFF_SIZE);
	respHeader = false;
headerSend.reserve(300); //!
message.reserve(300); //!
	std::stringstream ss;		
	ss << 
	"HTTP/1.1 200 OK\r\n\
Content-Type: text/html\r\n\
Content-Length: 316\r\n\
Connection: close\r\n\
\r\n\
"; 
	std::string str = ss.str();
	std::vector<char> res(str.begin(), str.end());	
	headerSend = res;
	ping = true;	
	statusCodes = CODE_200_OK;
}
Client::Client(const Client & src)
{
	Logger::getInstance().log(INFO, "\e[30;101mclient copy created\e[0m", *this);

	*this = src;
}
Client::~Client()
{
	Logger::getInstance().log(INFO, "\e[30;101mclient deleted\e[0m", *this);
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
	return res;	
}

void Server::catchClients()
{
	// Logger::getInstance().log(INFO, "Catch clients", *this);

	if (FD_ISSET(this->_fd, &this->_readSet))
	{		
		Client client;			
		client.fd = accept(this->_fd, reinterpret_cast<sockaddr *>
			(&client.address), &client.len);
		if (client.fd < 0)		
			return Logger::getInstance().log(ERROR, "accept");	
		// if (this->_clients.size() >= 1)
		// 	std::cout << this->_clients[0].response._streamHead << std::endl;
		// if (this->_clients.size() >= 2)
		// std::cout << this->_clients[1].response._streamHead << std::endl;
		Logger::getInstance().log(INFO, "\e[30;101mnew client\e[0m", client);

		FD_SET(client.fd, &this->_actualSet);
		this->_maxFd = std::max(this->_maxFd, client.fd);
		this->_clients.push_back(client);

		// if (this->_clients.size() >= 1)		
		// 	std::cout << this->_clients[0].response._streamHead << std::endl;
		// if (this->_clients.size() >= 2)
		// 	std::cout << this->_clients[1].response._streamHead << std::endl;	
		// std::vector<char> hardResp = buildHardResponseTest();
		// replyClient(client, hardResp);
		// Logger::getInstance().log(INFO, "Catch clients end", *this);		
	}
}

void Server::listenClients()
{	
	for (size_t i = 0; i < this->_clients.size(); i++)
	{	
		if (this->_clients[i].ping
			&& FD_ISSET(this->_clients[i].fd, &this->_readSet))
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

std::vector<char> buildHeaderTest()
{
	std::stringstream ss;	
		
	ss << 
"HTTP/1.1 200 OK\r\n\
Content-Type: text/html\r\n\
Content-Length: 317\r\n\
Connection: keep-alive\r\n\
\r\n\
"; 
	std::string str = ss.str();
	std::vector<char> res(str.begin(), str.end());
	return res;	
}

void floSimulatorPut(std::vector<char> part)
{	
	usleep(100000);
    Logger::getInstance().log(DEBUG, "FLO POST");

    static std::ofstream ofs("image_chat.jpeg", std::ios::binary);

	ofs.clear();
    if (ofs.is_open()) {
        ofs.write(part.data(), static_cast<std::streamsize>(part.size()));  
        ofs.flush();
		if (!ofs)
		{
			std::cout << "Erreur decriture dans le fichier." << std::endl;
		}
    } else {
        std::cout << "Erreur : impossible d'ouvrir le fichier." << std::endl;
    }
}

void Server::replyClients()
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{		
		if (!this->_clients[i].ping
			&& FD_ISSET(this->_clients[i].fd, &this->_writeSet))
		{
			if (!this->_clients[i].respHeader)
			{
				Logger::getInstance().log(DEBUG, "first header: tog true", this->_clients[i]);
				this->_clients[i].respHeader = true;				
				if(replyClient(i, this->_clients[i].headerSend,
					static_cast<ssize_t>
					(this->_clients[i].headerSend.size())))
					break ;
				this->_clients[i].headerSend.clear();
			}		
			
			if (ssize_t ret = this->_clients[i].response.
				getBody(this->_clients[i]))
			{	
				std::stringstream ss;
				ss << "stream body continue: " << ret;
				Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]);
						
				if (replyClient(i, this->_clients[i].messageSend, ret))
					break ;
				this->_clients[i].messageSend.clear();	
				this->_clients[i].messageSend.resize(SEND_BUFF_SIZE);				
				usleep(100000);
			}
			else
			{
				Logger::getInstance().log(DEBUG, "reinit response Builder, ready true, tog false", this->_clients[i]);
				// this->_clients[i].response._streamHead = 0;
				this->_clients[i].response._ifs.close();
				this->_clients[i].response = ResponseBuilder();
				this->_clients[i].ping = true;			
				this->_clients[i].respHeader = false;
				// break; //! ?
			}
		}	
	}
}

void printVectorCharTest(const std::vector<char> & vect)
{
	std::cout << std::endl << "\e[34mPrint Vector: \e[31m" << std::endl;
	std::cout << "-";		
	for (size_t i = 0; i < vect.size(); i++)				
		std::cout << vect[i];
	std::cout << "-\e[0m" << std::endl << std::endl;
}

bool Server::replyClient(size_t i, std::vector<char> & response,
	ssize_t repSize)
{	
	Logger::getInstance().log(INFO, "reply client", this->_clients[i]);
	printVectorCharTest(response);

	this->_writeBuffer.assign(response.begin(), response.begin() + repSize);					
	this->_readSet = this->_writeSet = this->_actualSet;		
	if (select(this->_maxFd + 1, &this->_readSet, &this->_writeSet, 0, NULL) //!
		< 0)	
		return Logger::getInstance().log(ERROR, "select", this->_clients[i]),
			true;	
	if(!FD_ISSET(this->_clients[i].fd, &this->_writeSet))	
		return Logger::getInstance().
			log(ERROR, "client not ready for response", this->_clients[i]),
			true;	
	ssize_t ret;
	char * writeHead = this->_writeBuffer.data();
	size_t writeSize = this->_writeBuffer.size();
	while (writeSize > 0)
	{			
		if ((ret = send(this->_clients[i].fd, writeHead, writeSize,
			MSG_NOSIGNAL)) < 0)
		{
			Logger::getInstance().log(ERROR, "send", this->_clients[i]);
			this->exitClient(i);
			return true;
		}		

		std::string str(writeHead, writeHead + static_cast<size_t>(ret));
		std::stringstream ss; ss << "data sent to client: -" << str << "-";	
		Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]); 	

		writeHead += ret;
		writeSize -= static_cast<size_t>(ret);
	}
	return false;
}

void printMessageClientTest(const Client & client)
{
	std::cout << std::endl << "\e[34mmessage client: " << std::endl;		
	for (size_t i = 0; i < client.message.size(); i++)				
		std::cout << client.message[i];
	std::cout << "\e[0m" << std::endl << std::endl;
}	


bool Server::isDelimiterFind(size_t i, std::vector<char>::iterator & it)
{
	std::string delimiter = "\r\n\r\n";
	it = std::search
		(this->_clients[i].message.begin(),
		this->_clients[i].message.end(),
		delimiter.begin(),
		delimiter.end() - 1);
	if (it != this->_clients[i].message.end())
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
		this->_clients[i].header.parse(this->_clients[i]);								
		this->_clients[i].header.displayParsingResult();
		if (this->_clients[i].header.getMethod() == "GET")		
			this->_clients[i].response.getHeader(this->_clients[i],
			this->_conf);

		this->_clients[i].message.erase(this->_clients[i].message.begin(),
			it + 4);
		this->_clients[i].bodySize += this->_clients[i].message.size();
		if (!this->isContentLengthValid(i)
			|| this->isBodyTooLarge(i)
			|| this->isBodyTerminated(i))
			return ;						
	}
	else
		isMaxHeaderSize(it + 1, i);
}

void Server::handleClientBody(size_t i, ssize_t ret)
{
	stringstream ss;
	ss << "receive client body" << " " << ret << " bytes";
	Logger::getInstance().log(INFO, ss.str(), this->_clients[i]);

	this->_clients[i].bodySize += static_cast<size_t>(ret);
	if (this->isBodyTooLarge(i) || this->isBodyTerminated(i))
		return ;
	if (this->_clients[i].header.getMethod() == "POST")
		floSimulatorPut(this->_clients[i].message);
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

		Logger::getInstance().log(ERROR, ss.str(), this->_clients[i]);
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
		Logger::getInstance().log(ERROR, ss.str(), this->_clients[i]);
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
		Logger::getInstance().log(ERROR, ss.str(), this->_clients[i]);
			//! 413 Payload Too Large
		
		this->exitClient(i);
		return true;;
	}
	return false;
}

bool Server::isBodyTerminated(size_t i) 
{
	if (this->_clients[i].bodySize ==
		this->_clients[i].header.getHeaders().ContentLength)
	{
		stringstream ss;
		ss << "client body terminated" << " - Body-Size: "
		<< this->_clients[i].bodySize << " Content-Lenght: "
		<< this->_clients[i].header.getHeaders().ContentLength;
		Logger::getInstance().log(INFO, ss.str(), this->_clients[i]);

		this->_clients[i].bodySize = 0;
		if (this->_clients[i].header.getMethod() == "POST")
			floSimulatorPut(this->_clients[i].message);
		this->_clients[i].message.clear();
		this->_clients[i].ping = false;		
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
	Logger::getInstance().log(INFO, ss.str(), client);
}

void Server::exitClient(size_t i)
{
	Logger::getInstance().log(INFO, "\e[30;101mclient exited\e[0m",
		this->_clients[i]);

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