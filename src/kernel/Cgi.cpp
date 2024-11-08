#include "Cgi.hpp"
#include "Logger.hpp"

void errnoHandle(); //! a suppr

Cgi::Cgi()
{
    this->_fds[0] = -1;
    this->_fds[1] = -1;
}

Cgi & Cgi::operator=(const Cgi & rhs)
{
	this->_fds[0] = rhs._fds[0];
	this->_fds[1] = dup(rhs._fds[1]);
	Kernel::_maxFd = std::max(Kernel::_maxFd, this->_fds[1]);
	FD_SET(this->_fds[1], &Kernel::_actualSet);
	return *this;
}

Cgi::~Cgi()
{
    FD_CLR(this->_fds[1], &Kernel::_actualSet);
    if (this->_fds[1] > 0)
        close(this->_fds[1]);
}

void Cgi::launch() 
{           
    socketpair(AF_UNIX, SOCK_STREAM, 0, this->_fds);  
    Kernel::_maxFd = std::max(Kernel::_maxFd, this->_fds[1]);
    
    FD_SET(this->_fds[1], &Kernel::_actualSet);
    
    pid_t pid = fork();
    if (pid < 0)
        Logger::getInstance().log(ERROR, "fork failed");
    else if (pid == 0)
    {       
        dup2(this->_fds[0], STDIN_FILENO); 
        dup2(this->_fds[0], STDOUT_FILENO); 
        close(this->_fds[0]);//!
        close(this->_fds[1]);//!
        // int t[2] = {1,2, NULL}
        // char *args[] = {NULL};
        // char *envp[] = {NULL};
        // execve("./cgi/a.out", args, envp);
        // chdir("./cgi");//!
        std::string str("PATH_INFO=coucoucpathinfo");
        char *env[] = 
        {
            const_cast<char *>(str.c_str()), NULL
        };
		char *argv[] = {NULL};
        //!FLAG ANTI HERITAGE FD OU CLOSE
        execve("./cgi/a.out", argv, env);  //!     
        // execve("/home/svidot/42_am/webserv/cgi/a.out", argv, env);       
      	Logger::getInstance().log(ERROR, "execve failed");
	    //! LEAKS 
	    exit(1);
    } 
    else                 
        close(this->_fds[0]);
}

bool Cgi::retHandle(Client & client, ssize_t ret, std::string err,
	std::string info)
{
	stringstream ss; ss << "ret: " << ret;
	Logger::getInstance().log(DEBUG, ss.str());

    if (!ret)
    {
        Logger::getInstance().log(INFO, info);        
        return false;
    }
    else if (ret < 0)
    {		
        Logger::getInstance().log(ERROR, err);
        errnoHandle();		
   
		FD_CLR(this->_fds[1], &Kernel::_actualSet);
        close(this->_fds[1]);
        this->_fds[1] = -1;
        client.exitRequired = true;
        return true;
    } 
	return false;
}

bool Cgi::getBody(Client & client)
{
    Logger::getInstance().log(INFO, "Get Body");
         
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
		return false;
   
	client.messageSend.erase(client.messageSend.begin() + ret,
		client.messageSend.end());
	return false; 
}


void Cgi::setBody(Client & client, bool eof)
{
    Logger::getInstance().log(INFO, "Set Body");

    if (!FD_ISSET(this->_fds[1], &Kernel::_writeSet))
    {
        Logger::getInstance().log(DEBUG, "not ready to write");
        return;
    }
    Logger::getInstance().log(DEBUG, "ready to send");

 	ssize_t ret = send(this->_fds[1], client.messageRecv.data(),
        client.messageRecv.size(), 0);
	if (retHandle(client, ret, "send", "cgi exited"))
		return ;
    
    std::string str(client.messageRecv.data(), client.messageRecv.data()
		+ static_cast<size_t>(ret));      
	std::stringstream ss; ss << "data sent to cgi: -" << str << "-";	
	Logger::getInstance().log(DEBUG, ss.str(), client);
		
	client.messageRecv.erase(client.messageRecv.begin(),
        client.messageRecv.begin() + ret);
	if (eof && client.messageRecv.empty())	
		shutdown(_fds[1], SHUT_WR);	
}
