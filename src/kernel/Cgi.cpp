#include "Cgi.hpp"
#include "Logger.hpp"

void errnoHandle(); //! a suppr

Cgi::Cgi()
{
	this->_start = 0; //! reinit start at pong
    this->_fds[0] = -1;
    this->_fds[1] = -1;
}

Cgi & Cgi::operator=(const Cgi & rhs)
{
	this->_start = rhs._start;
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
    Logger::getInstance().log(DEBUG, "Launch Cgi");  

	this->_start = std::clock();
	


    socketpair(AF_UNIX, SOCK_STREAM, 0, this->_fds);  
    Kernel::_maxFd = std::max(Kernel::_maxFd, this->_fds[1]);
    struct timeval timeout = {SND_TIMEOUT, 0};	
	if (setsockopt(this->_fds[1], SOL_SOCKET, SO_SNDTIMEO, &timeout,
		sizeof(timeout)) < 0)
	  	return Logger::getInstance().log(ERROR, "cgi send timeout"); //!exit client req + error page

    FD_SET(this->_fds[1], &Kernel::_actualSet);
    
    pid_t pid = fork();
    if (pid < 0)
        Logger::getInstance().log(ERROR, "fork failed");//!exit client req + error page
    else if (pid == 0)
    {       
        dup2(this->_fds[0], STDIN_FILENO); 
        dup2(this->_fds[0], STDOUT_FILENO); 
        close(this->_fds[0]);
        close(this->_fds[1]);
          
        chdir("./cgi");//!
        std::string str("PATH_INFO=coucoucpathinfo");
        char *env[] = 
        {
            const_cast<char *>(str.c_str()), NULL
        };
		char *argv[] = {NULL};
        //!FLAG ANTI HERITAGE FD OU CLOSE
        execve("a.out", argv, env);  //!     
        // execve("/home/svidot/42_am/webserv/cgi/a.out", argv, env);       
      	Logger::getInstance().log(ERROR, "execve failed");
	    //! LEAKS 
	    //!exit client req + error page
    } 
    else                 
        close(this->_fds[0]);
}

bool Cgi::retHandle(Client & client, ssize_t ret, std::string err,
	std::string info)
{
	static ssize_t gret;
	gret += ret;
	stringstream ss; ss << "ret: " << ret << " gret: " << gret;
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
    return false;	
}

bool Cgi::isTimeout(Client & client, std::string err)
{
	if (!this->_start)
		this->_start = std::clock();
	std::clock_t end = std::clock();
	if (static_cast<double>(end - this->_start) / CLOCKS_PER_SEC >
		TIMEOUT_CGI)
	{		
        Logger::getInstance().log(INFO, err);
     	//!errpage!!
        client.exitRequired = true;
        return true;
    } 
	return false;
}

void Cgi::setBody(Client & client, bool eof)
{
    Logger::getInstance().log(INFO, "Cgi Set Body");

	if (isTimeout(client, "timeout setbody is over"))
		return ;//! true 	
    if (!FD_ISSET(this->_fds[1], &Kernel::_writeSet))
    {
        Logger::getInstance().log(DEBUG, "not ready to write");
        return;
    }
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

	if (isTimeout(client, "timeout getbody is over"))
		return false;

    shutdown(_fds[1], SHUT_WR);
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

