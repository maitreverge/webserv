#include "Cgi.hpp"
#include "Logger.hpp"

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

void provC(Client & client) //! a suppr
{
	if (chdir(client.responseBuilder._folderCGI.c_str()) < 0)	
		Logger::getInstance().log(ERROR, "chdir", client), exit(200);			
	char actualPath[PATH_MAX];	
	if (!getcwd(actualPath, PATH_MAX))	
		Logger::getInstance().log(ERROR, "getcwd", client), exit(200);			 
	std::string envPathInfo("PATH_INFO=" + client.responseBuilder._pathInfo);    
	char *env[] = {const_cast<char *>(envPathInfo.c_str()), NULL};
	std::string execPath = std::string(actualPath) + '/'
		+ client.responseBuilder._fileName; 
	char *argv[] = {const_cast<char *>(execPath.c_str()), NULL};	
	Logger::getInstance().~Logger();
	Kernel::getInstance().exitKernel();	
	execve(execPath.c_str(), argv, env);
}

void Cgi::child(Client & client)
{
    Logger::getInstance().log(DEBUG, "Child", client);
	
	if (dup2(this->_fds[0], STDIN_FILENO) < 0
		|| dup2(this->_fds[0], STDOUT_FILENO) < 0)
	{
		Logger::getInstance().log(ERROR, "dup2", client);
		close(this->_fds[0]); this->_fds[0] = -1;	
		close(this->_fds[1]); this->_fds[1] = -1;
		std::exit(200);	
	}		
	close(this->_fds[0]); this->_fds[0] = -1;	
	close(this->_fds[1]); this->_fds[1] = -1;			
	if (client.responseBuilder._fileExtension == "out") //! a suppr
		provC(client);
	else if (client.responseBuilder._fileExtension == "php")		
		this->callExecve(client, "php-cgi");
	else if (client.responseBuilder._fileExtension == "py")	
		this->callExecve(client, "python3");		
	std::exit(242);
}

void Cgi::callExecve(Client & client, const std::string & interpreter)
{
	if (chdir(client.responseBuilder._folderCGI.c_str()) < 0)
		Logger::getInstance().log(ERROR, "chdir", client), exit(200);				
	char actualPath[PATH_MAX];	
	if (!getcwd(actualPath, PATH_MAX))
		Logger::getInstance().log(ERROR, "getcwd", client), exit(200);					 
	std::string interPath = this->getPath(client, interpreter);
	
	std::string envPathInfo("PATH_INFO=" + client.responseBuilder._pathInfo);	 
	char *env[] = {const_cast<char *>(envPathInfo.c_str()), NULL};   
	std::string execPath(std::string(actualPath)
		+ '/' + client.responseBuilder._fileName);
	
	char *argv[] = {const_cast<char *>(interpreter.c_str()),
		const_cast<char *>(execPath.c_str()), NULL};

	Logger::getInstance().~Logger();
	Kernel::getInstance().exitKernel();	
	execve(interPath.c_str(), argv, env);
}

std::string Cgi::getPath(Client & client, const std::string & interpreter)
{
	if (char * env = std::getenv("PATH"))
	{
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
		}
		Logger::getInstance().log(ERROR, "interpreter not exist", client);
		exit(200);			
	}
	Logger::getInstance().log(ERROR, "PATH not exist", client);
	exit(200);		
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
