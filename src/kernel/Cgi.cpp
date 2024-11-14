#include "Cgi.hpp"
#include "Logger.hpp"

void errnoHandle(); //! a suppr

Cgi::Cgi()
{
	this->_start = 0; //! verif wrap arround
	this->_pid = -1;
    this->_fds[0] = -1;
    this->_fds[1] = -1;
}

Cgi & Cgi::operator=(const Cgi & rhs)
{
	this->_start = rhs._start;
	this->_pid = rhs._pid;
	this->_fds[0] = rhs._fds[0];	
	if (this->_fds[1] > 0)
	{
		FD_CLR(this->_fds[1], &Kernel::_actualSet);
		close(this->_fds[1]);
	}
	this->_fds[1] = dup(rhs._fds[1]);
	FD_SET(this->_fds[1], &Kernel::_actualSet);
	Kernel::_maxFd = std::max(Kernel::_maxFd, this->_fds[1]);
	return *this;
}

Cgi::~Cgi()
{
    if (this->_fds[1] > 0)
	{	
  		FD_CLR(this->_fds[1], &Kernel::_actualSet);
        close(this->_fds[1]);	
	}	
}

void Cgi::launch(Client & client)
{   
    Logger::getInstance().log(DEBUG, "Launch Cgi");  
	
    socketpair(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0, this->_fds);  
	
    Kernel::_maxFd = std::max(Kernel::_maxFd, this->_fds[1]);
    struct timeval timeout = {SND_TIMEOUT, 0};	
	if (setsockopt(this->_fds[1], SOL_SOCKET, SO_SNDTIMEO, &timeout,
		sizeof(timeout)) < 0)
	  	return Logger::getInstance().log(ERROR, "cgi send timeout"); //!exit client req + error page
    FD_SET(this->_fds[1], &Kernel::_actualSet);    
    this->_pid = fork();
    if (this->_pid < 0)
        Logger::getInstance().log(ERROR, "fork failed"); //!exit client req + error page
    else if (!this->_pid)
        child(client);
    else
        close(this->_fds[0]);
}

void Cgi::child(Client & client)
{
    dup2(this->_fds[0], STDIN_FILENO); 
    dup2(this->_fds[0], STDOUT_FILENO); 
    close(this->_fds[0]);
    close(this->_fds[1]);
        
    chdir(client.responseBuilder._folderCGI.c_str());
    std::string envPathInfo
        ("PATH_INFO=" + client.responseBuilder._pathInfo);
    
    char *env[] = {const_cast<char *>(envPathInfo.c_str()), NULL};

    std::string path = client.responseBuilder._fileName;
    Logger::getInstance().~Logger();
    Kernel::getInstance(NULL).~Kernel();
    // if (client.responseBuilder._fileExtension == "out")    
    // {
	// 	char *argv[] = {NULL};
    //     execve(path.c_str(), argv, env); //!
	// }
	// else 
	if (client.responseBuilder._fileExtension == "py")
	{	
		char *argv[] = {const_cast<char *>("/usr/bin/python3"),
			const_cast<char *>(path.c_str()), NULL};
        execve("/usr/bin/python3", argv, env); //!
	}
    else if (client.responseBuilder._fileExtension == "php")
	{	
		char *argv[] = {const_cast<char *>("/usr/bin/php-cgi"),
			const_cast<char *>(path.c_str()), NULL};
        execve("/usr/bin/php-cgi", argv, env); //!
	}
		// execve(path.c_str(), argv, env);
    // execve("/home/seblin/42/42_webserv/cgi/main_inout.out", argv, env); 		    
    // execve("/home/svidot/42_am/webserv/cgi/main_inout.out", argv, env); 
    // execve(client.responseBuilder., argv, env);   
    std::cerr << "\e[1;31mexecve failed\e[0m" << std::endl;
    _exit(1);
}

bool Cgi::retHandle(Client & client, ssize_t ret, std::string err,
	std::string info)
{		
	stringstream ss; ss << "ret: " << ret;
	Logger::getInstance().log(DEBUG, ss.str());

    if (!ret)    
        Logger::getInstance().log(INFO, info);
    else if (ret < 0)
    {		
        Logger::getInstance().log(ERROR, err);
        errnoHandle();		

		// FD_CLR(this->_fds[1], &Kernel::_actualSet);//! err 4..
        // close(this->_fds[1]);
        // this->_fds[1] = -1;

        client.exitRequired = true;
        return true;
    }
	this->_start = std::clock(); 
    return false;	
}

bool Cgi::isTimeout(Client & client, std::string err)
{
	if (!this->_start)
		this->_start = std::clock();
	std::clock_t end = std::clock();
    double span = static_cast<double>(end - this->_start) * 5000
        / CLOCKS_PER_SEC;
    if (span < 0)
        this->_start = 0;
    if (span > TIMEOUT_CGI / 2.0)
    { 
        std::stringstream ss; ss << "Timeout - start: "
            << this->_start << " end: " << end << " diff: "
            << static_cast<double>(end - this->_start) << " sec: "
            << span << "/" << TIMEOUT_CGI << std::endl;
        Logger::getInstance().log(WARNING, ss.str(), client);	
    }
	if (span > TIMEOUT_CGI)
	{	
        Logger::getInstance().log(ERROR, err);   	//!errpage!!
        kill(this->_pid, SIGTERM);
		this->_start = 0;
        client.exitRequired = true;
		client.ping = 2;	
        return true;
    } 
	return false;
}

void Cgi::setBody(Client & client, bool eof)
{
    Logger::getInstance().log(INFO, "Cgi Set Body");

	if (isTimeout(client, "Timeout Cgi Set Body is over"))
		return ;//! true 	
    if (!FD_ISSET(this->_fds[1], &Kernel::_writeSet))    
        return Logger::getInstance().log(DEBUG, "not ready to send");
     
    Logger::getInstance().log(DEBUG, "ready to send");
 	ssize_t ret = send(this->_fds[1], client.messageRecv.data(),
        client.messageRecv.size(), MSG_NOSIGNAL);
	if (retHandle(client, ret, "send", "cgi exited"))
        ret = static_cast<ssize_t>(client.messageRecv.size());			
    
	std::string str(client.messageRecv.data(), client.messageRecv.data()
		+ static_cast<size_t>(ret));      
	std::stringstream ss; ss << "data sent to cgi: -" << str << "-";	
	Logger::getInstance().log(DEBUG, ss.str(), client);	

	client.messageRecv.erase(client.messageRecv.begin(),
        client.messageRecv.begin() + ret);
	if (eof && client.messageRecv.empty())	
		shutdown(_fds[1], SHUT_WR);
}

bool Cgi::getBody(Client & client)
{
    Logger::getInstance().log(INFO, "Cgi Get Body");

	if (isTimeout(client, "Timeout Cgi Get Body is over"))
		return false;

    if (shutdown(_fds[1], SHUT_WR) < 0)
		Logger::getInstance().log(ERROR, "shutdown", client);//! ret ?
    if (!FD_ISSET(this->_fds[1], &Kernel::_readSet))
    {
        sleep(1);
        Logger::getInstance().log(DEBUG, "not ready to recev");
        return true;
    }
    client.messageSend.clear();
    client.messageSend.resize(SEND_BUFF_SIZE);
    ssize_t ret = recv(this->_fds[1], client.messageSend.data(),
        client.messageSend.size(), 0);  
	if (retHandle(client, ret, "recv", "end cgi"))
        ret = 0;  
	client.messageSend.erase(client.messageSend.begin() + ret,
		client.messageSend.end());
	return false; 
}

