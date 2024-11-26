#include "Cgi.hpp"
#include "Logger.hpp"

void errnoHandle(); //! a suppr

Cgi::Cgi()
{	
	std::memset(&this->_start, 0, sizeof(this->_start));
	this->_lastSpan = 0;
	this->_pid = -1;
    this->_fds[0] = -1;
    this->_fds[1] = -1;
}

Cgi::Cgi(const Cgi & src)
{	
	this->_fds[1] = -1;
	*this = src;
}

Cgi & Cgi::operator=(const Cgi & rhs)
{
	this->_start = rhs._start;
	this->_lastSpan = rhs._lastSpan;
	this->_pid = rhs._pid;
	this->_fds[0] = rhs._fds[0];	

	if (this->_fds[1] >= 0)
	{
		FD_CLR(this->_fds[1], &Kernel::_actualSet);
		close(this->_fds[1]);
	}
	if (rhs._fds[1] >= 0)
	{
		if ((this->_fds[1] = dup(rhs._fds[1])) >= 0)
			FD_SET(this->_fds[1], &Kernel::_actualSet);
		Kernel::_maxFd = std::max(Kernel::_maxFd, this->_fds[1]);
	}
	else
		this->_fds[1] = rhs._fds[1];	
	return *this;
}

Cgi::~Cgi()
{
    if (this->_fds[1] < 0)
		return ;	
	FD_CLR(this->_fds[1], &Kernel::_actualSet);
	close(this->_fds[1]);		
}

void Cgi::launch(Client & client)
{   
    Logger::getInstance().log(DEBUG, "Launch Cgi");  
	
    if (socketpair(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0, this->_fds) < 0)
		throw (Logger::getInstance().log(ERROR, "socket pair", client),
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));	
    Kernel::_maxFd = std::max(Kernel::_maxFd, this->_fds[1]);
    struct timeval timeout = {SND_TIMEOUT, 0};	
	if (setsockopt(this->_fds[1], SOL_SOCKET, SO_SNDTIMEO, &timeout,
		sizeof(timeout)) < 0)	
		throw (Logger::getInstance().log(ERROR, "cgi opt send timeout", client),
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));	  	
    FD_SET(this->_fds[1], &Kernel::_actualSet);    
    this->_pid = fork();
    if (this->_pid < 0)
        throw (Logger::getInstance().log(ERROR, "fork", client),
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));
    else if (!this->_pid)
        this->child(client);
    else
        close(this->_fds[0]);
}
// void Cgi::child(Client & client)
// {
//     Logger::getInstance().log(DEBUG, "Child", client);
	
// 	try 
// 	{
// 		dup2(this->_fds[0], STDIN_FILENO); 
// 		dup2(this->_fds[0], STDOUT_FILENO); 
// 		close(this->_fds[0]);
// 		close(this->_fds[1]);
// 		this->_fds[0] = -1;	
// 		this->_fds[1] = -1;	
// 		if (chdir(client.responseBuilder._folderCGI.c_str()) < 0)
// 		{
// 			Logger::getInstance().log(ERROR, "chdir", client);
// 			// Logger::getInstance().~Logger();
// 			// Kernel::getInstance().exitKernel();
// 			exit(200);

// 				// Logger::getInstance().~Logger();
// 				// Kernel::getInstance().~Kernel();
// 				// _exit(200);
// 		}
// 			char actualPath[PATH_MAX];	
// 			if (!getcwd(actualPath, PATH_MAX))
// 			{
// 				Logger::getInstance().log(ERROR, "getcwd", client);
// 				// Logger::getInstance().~Logger();
// 				// Kernel::getInstance().~Kernel();
// 				exit(200);	
// 			}

// 		 {
// 			std::string envPathInfo
// 				("PATH_INFO=" + client.responseBuilder._pathInfo);    
// 			char *env[] = {const_cast<char *>(envPathInfo.c_str()), NULL};
// 			std::string execPath = std::string(actualPath) + '/'
// 				+ client.responseBuilder._fileName; 

// 			if (client.responseBuilder._fileExtension == "out")    
// 			{	
// 				Logger::getInstance().~Logger();
// 				// Logger::getInstance().exitLogger();
// 				// Kernel::getInstance().~Kernel();
// 				Kernel::getInstance().exitKernel();
// 				char *argv[] = {NULL};
// 				// execve(path.c_str(), argv, env);
// 				execve("gros caca", argv, env);
// 			}		
// 			else if (client.responseBuilder._fileExtension == "py")
// 			{
// 				std::string interPath = this->getPath("python3", client);	
// 				Logger::getInstance().~Logger();
// 				Kernel::getInstance().exitKernel();
// 				char *argv[] = {const_cast<char *>("python3"),
// 					const_cast<char *>(execPath.c_str()), NULL};
// 				execve(interPath.c_str(), argv, env); 
// 			}
// 			else if (client.responseBuilder._fileExtension == "php")
// 			{
// 				std::string interPath = this->getPath("php-cgi", client);
// 				Logger::getInstance().~Logger();
// 				Kernel::getInstance().exitKernel();	
// 				char *argv[] = {const_cast<char *>("php-cgi"),
// 					const_cast<char *>(execPath.c_str()), NULL};
// 				execve(interPath.c_str(), argv, env);
// 			} 
// 			// Logger::getInstance().log(ERROR, "execve", client);
			
// 			// std::cerr << "\e[1;31mexecve failed\e[0m" << std::endl;
// 			// Logger::getInstance().~Logger();
// 		}
// 		exit(242);// mettre une err spe pour execve 
// 	}
// 	catch (const Server::ShortCircuitException & e)
// 	{
// 		Logger::getInstance().~Logger();
// 		Kernel::getInstance().~Kernel();
// 		_exit(static_cast<int>(e.getCode()));	
// 	}
// }

void provC(Client & client) //! a suppr
{
	if (chdir(client.responseBuilder._folderCGI.c_str()) < 0)	
		throw (Logger::getInstance().log(ERROR, "chdir", client),
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));	
	char actualPath[PATH_MAX];	
	if (!getcwd(actualPath, PATH_MAX))	
		throw (Logger::getInstance().log(ERROR, "getcwd", client),
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));			 
	std::string envPathInfo("PATH_INFO=" + client.responseBuilder._pathInfo);    
	char *env[] = {const_cast<char *>(envPathInfo.c_str()), NULL};
	std::string execPath = std::string(actualPath) + '/'
		+ client.responseBuilder._fileName; 
	char *argv[] = {const_cast<char *>
		(execPath.c_str()), NULL};	
	Logger::getInstance().~Logger();
	Kernel::getInstance().exitKernel();	
	execve(execPath.c_str(), argv, env);
}

void Cgi::child(Client & client)
{
    Logger::getInstance().log(DEBUG, "Child", client);
	
	try 
	{
		if (dup2(this->_fds[0], STDIN_FILENO) < 0
			|| dup2(this->_fds[0], STDOUT_FILENO) < 0)		
			throw (Logger::getInstance().log(ERROR, "dup2", client),
				Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));
		close(this->_fds[0]); this->_fds[0] = -1;	
		close(this->_fds[1]); this->_fds[1] = -1;			
		if (client.responseBuilder._fileExtension == "out") //! a suppr
			provC(client);
		if (client.responseBuilder._fileExtension == "php")		
			this->callExecve(client, "php-cgi");
		else if (client.responseBuilder._fileExtension == "py")	
			this->callExecve(client, "python3");		
		std::exit(242);
	}
	catch (const Server::ShortCircuitException & e)
	{	std::exit(static_cast<int>(e.getCode()));	}
}

void Cgi::callExecve(Client & client, const std::string & interpreter)
{
	if (chdir(client.responseBuilder._folderCGI.c_str()) < 0)
		throw (Logger::getInstance().log(ERROR, "chdir", client),
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));				
	char actualPath[PATH_MAX];	
	if (!getcwd(actualPath, PATH_MAX))
		throw (Logger::getInstance().log(ERROR, "getcwd", client),
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));					 
	std::string envPathInfo("PATH_INFO=" + client.responseBuilder._pathInfo);    
	char *env[] = {const_cast<char *>(envPathInfo.c_str()), NULL};
	std::string execPath = std::string(actualPath) + '/'
		+ client.responseBuilder._fileName; 
	char *argv[] = {const_cast<char *>(interpreter.c_str()),
		const_cast<char *>(execPath.c_str()), NULL};
	std::string interPath = this->getPath(client, interpreter);
	Logger::getInstance().~Logger();
	Kernel::getInstance().exitKernel();	
	execve(interPath.c_str(), argv, env);
}

std::string Cgi::getPath(Client & client, const std::string & interpreter)
{
	if (char * env = std::getenv("PATH"))
	{
		std::string path;
		path.reserve(128);
		std::istringstream ss(env);
		std::string line;
		while (std::getline(ss, line, ':'))
		{
			path = line + '/' + interpreter;
			if (!access(path.c_str(), F_OK | X_OK))
				return path;
		}		
		throw (Logger::getInstance().log(ERROR, "interpreter no exist", client),
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));
	}
	throw (Logger::getInstance().log(ERROR, "path no exist", client),
		Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));		
}

void Cgi::retHandle(Client & client, ssize_t ret, std::string err,
	std::string info)
{		
	std::stringstream ss; ss << "ret: " << ret;
	Logger::getInstance().log(DEBUG, ss.str());

    if (!ret)    
        Logger::getInstance().log(DEBUG, info);
    else if (ret < 0)
    {	      
        errnoHandle();//! a suppr  
		throw (Logger::getInstance().log(ERROR, err, client),
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));
    }
	if (gettimeofday(&this->_start, NULL))
		throw (Logger::getInstance().log(ERROR, "retH gettimeofday", client),
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));	
}

double Cgi::getTimeSpan(Client & client) const
{
	struct timeval actualTime;
	
	if (!gettimeofday(&actualTime, NULL))
	{
		double deltaSec
			= static_cast<double>(actualTime.tv_sec - this->_start.tv_sec);
		double deltauSec
			= static_cast<double>(actualTime.tv_usec - this->_start.tv_usec);
		return (deltaSec + deltauSec / 1000000.0);
	}
	else
		throw (Logger::getInstance().log(ERROR, "gTspan gettimeofday", client),
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));
}

void Cgi::isTimeout(Client & client, std::string err)
{
	Logger::getInstance().log(DEBUG, "Cgi Is Timeout", client);

	if (!this->_start.tv_sec && gettimeofday(&this->_start, NULL))
		throw (Logger::getInstance().log(ERROR, "isTimeS gettimeofday", client),
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));
    double span = getTimeSpan(client);   
    if (span >= client.conf->timeoutCgi / 2.0
		&& this->_lastSpan != static_cast<int>(span))
    { 
		this->_lastSpan = static_cast<int>(span);
        std::stringstream ss;
		ss << "Timeout : "  << this->_lastSpan << " secs/"
		<< client.conf->timeoutCgi;			
        Logger::getInstance().log(WARNING, ss.str(), client);	
    }
	if (span >= client.conf->timeoutCgi)
	{      
        kill(this->_pid, SIGTERM);
		std::memset(&this->_start, 0, sizeof(this->_start));
		throw (Logger::getInstance().log(ERROR, err, client),
			Server::ShortCircuitException(CODE_508_LOOP_DETECTED));           
    } 
}

void Cgi::hasError(Client & client, std::string err)
{
	Logger::getInstance().log(DEBUG, "Cgi Has Error", client);

	int status;
	pid_t pid = waitpid(this->_pid, &status, WNOHANG);
	if (pid > 0)
	{	
		Logger::getInstance().log(DEBUG, "cgi exited", client);	
		if (WIFEXITED(status))
		{		
			int exitCode = WEXITSTATUS(status);
			std::stringstream ss; ss << "cgi exited with code : " << exitCode;
			Logger::getInstance().log(DEBUG, ss.str(), client);
			if (exitCode == 242)
				Logger::getInstance().log(ERROR, "execve", client);
			if (exitCode != 0)		
				throw (Logger::getInstance().log(ERROR, err, client), Server::
					ShortCircuitException(CODE_503_SERVICE_UNAVAILABLE)); 					
		}
		else if (WIFSIGNALED(status)) {
            // int signal = WTERMSIG(status);
            throw (Logger::getInstance().log(ERROR, err, client), Server::
				ShortCircuitException(CODE_502_BAD_GATEWAY)); 
        }
	}
	else if (pid < 0){Logger::getInstance().log(ERROR, "waitpid", client);}
	else {Logger::getInstance().log(DEBUG, "cgi is running", client);}
		// throw (Logger::getInstance().log(ERROR, "waitpid", client),
		// 		Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));
}

void Cgi::setBody(Client & client, bool eof)
{
    Logger::getInstance().log(DEBUG, "Cgi Set Body", client);

	this->hasError(client, "cgi get body has error");
	this->isTimeout(client, "Timeout is over");
    if (!FD_ISSET(this->_fds[1], &Kernel::_writeSet))    
        return Logger::getInstance().log(DEBUG, "cgi not ready to send");     
    Logger::getInstance().log(DEBUG, "\e[31;43mcgi ready to send\e[0m");
 	ssize_t ret = send(this->_fds[1], client.messageRecv.data(),
        client.messageRecv.size(), MSG_NOSIGNAL);
    Kernel::cleanFdSet(client);	
	this->retHandle(client, ret, "send", "cgi exited");    			
	std::vector<char> str(client.messageRecv.data(), client.messageRecv.data()
		+ static_cast<size_t>(ret));
	Logger::getInstance().log(DEBUG, "sent to cgi", client);	
		Server::printVector(client, str);
	client.messageRecv.erase(client.messageRecv.begin(),
        client.messageRecv.begin() + ret);
	if (eof && client.messageRecv.empty() && shutdown(_fds[1], SHUT_WR < 0))			
		throw (Logger::getInstance().log(ERROR, "set body shutdown", client),
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));
}

bool Cgi::getBody(Client & client)
{
    Logger::getInstance().log(DEBUG, "Cgi Get Body", client);

	this->hasError(client, "cgi get body has error");
	this->isTimeout(client, "Timeout is over");
    if (shutdown(_fds[1], SHUT_WR) < 0){}
		//throw (Logger::getInstance().log(ERROR, "get body shutdown", client),
			//Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));
    if (!FD_ISSET(this->_fds[1], &Kernel::_readSet))  
        return Logger::getInstance().
			log(DEBUG, "cgi not ready to recev", client), true;  
    client.messageSend.clear();
    client.messageSend.resize(client.conf->send_buff_size);
	Logger::getInstance().log(DEBUG, "\e[31;43mcgi ready to recev\e[0m",
		client);
    ssize_t ret = recv(this->_fds[1], client.messageSend.data(),
        client.messageSend.size(), 0);
	Kernel::cleanFdSet(client);
	this->retHandle(client, ret, "recv", "end cgi");
	client.messageSend.erase(client.messageSend.begin() + ret,
		client.messageSend.end());
	return false; 
}
