#include "Cgi.hpp"
#include "Logger.hpp"

Cgi::Cgi()
{
    this->_fds[0] = -1;
    this->_fds[1] = -1;
}

void Cgi::launch() //!WARNING select
{           
    socketpair(AF_UNIX, SOCK_STREAM, 0, this->_fds);  
    Kernel::_maxFd = std::max(Kernel::_maxFd, this->_fds[1]);
    // Kernel::_maxFd = std::max(Kernel::_maxFd, this->_fds[0]);
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
    {                
        close(this->_fds[0]);//!

        // char buff[150];
        // char buff2[5] = {'s','a','l','u','t'};
        // send(this->_fds[1], buff2, sizeof(buff2), 0);
        // ssize_t retp = recv(this->_fds[1], buff, sizeof(buff), 0);
        // for (ssize_t i = 0; i < retp; i++)
            // std::cout << "." << buff[i];   
    }    
}

ssize_t Cgi::getBody(Client & client)
{
    // Logger::getInstance().log(INFO, "hello from parent");
    
    
    // char buff[150];
    // char buff2[5] = {'s','a','l','u','t'};
    // send(this->_fds[1], buff2, sizeof(buff2), 0);
    if (!FD_ISSET(this->_fds[1], &Kernel::_readSet))
        return 73;
    Logger::getInstance().log(INFO, "ive not been retarded");
    client.messageSend.clear();
    client.messageSend.resize(SEND_BUFF_SIZE);//!
    ssize_t ret; 

    ret = recv(this->_fds[1], client.messageSend.data(),
        client.messageSend.size(), 0);
        /* code */    
  
    sleep(1);
    if (ret < 0)
    {
        Logger::getInstance().log(ERROR, "recv cgi");

        switch (errno) 
		{        
            case EWOULDBLOCK:
                printf("Pas de données disponibles pour le moment (socket en mode non bloquant).\n");
                break;
            case EINTR:
                printf("recv a été interrompu par un signal, réessayez.\n");
                break;
            case ECONNRESET:
                printf("Connexion réinitialisée par le pair.\n");
                break;
            case ETIMEDOUT:
                printf("Délai de connexion dépassé.\n");
                break;
            case ENOTCONN:
                printf("La socket n'est pas connectée.\n");
                break;
            case EHOSTUNREACH:
                printf("Hôte distant injoignable.\n");
                break;
            case ENETDOWN:
                printf("Réseau local non disponible.\n");
                break;
            case EMSGSIZE:
                printf("Le message est trop grand pour le tampon de réception.\n");
                break;
            case EBADF:
                printf("Descripteur de fichier invalide.\n");
                break;
            case EINVAL:
                printf("Arguments invalides ou socket fermée.\n");
                break;
            case ENOBUFS:
                printf("Pas assez de mémoire pour traiter la demande.\n");
                break;
            default:
                printf("Erreur inconnue : %d\n", errno);
                break;

    	}
		FD_CLR(this->_fds[1], &Kernel::_actualSet);
        close(this->_fds[1]);//! delete from FD
        this->_fds[1] = -1;
        //!exitclient
        return 0;
    }
    std::cout << "RET " << ret << std::endl;
    // for (ssize_t i = 0; i < ret; i++)
    //     std::cout << "." << buff[i];
    if (!ret)
    {
        Logger::getInstance().log(INFO, "end cgi");
        FD_CLR(this->_fds[1], &Kernel::_actualSet);
        close(this->_fds[1]);//!
        this->_fds[1] = -1;
        //!exitclient
        return 0;
    }
    return ret;
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
        close(this->_fds[1]);//!
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
	if (ret < 0)
    {
        Logger::getInstance().log(ERROR, "recv cgi");

        switch (errno) 
		{        
            case EWOULDBLOCK:
                printf("Pas de données disponibles pour le moment (socket en mode non bloquant).\n");
                break;
            case EINTR:
                printf("recv a été interrompu par un signal, réessayez.\n");
                break;
            case ECONNRESET:
                printf("Connexion réinitialisée par le pair.\n");
                break;
            case ETIMEDOUT:
                printf("Délai de connexion dépassé.\n");
                break;
            case ENOTCONN:
                printf("La socket n'est pas connectée.\n");
                break;
            case EHOSTUNREACH:
                printf("Hôte distant injoignable.\n");
                break;
            case ENETDOWN:
                printf("Réseau local non disponible.\n");
                break;
            case EMSGSIZE:
                printf("Le message est trop grand pour le tampon de réception.\n");
                break;
            case EBADF:
                printf("Descripteur de fichier invalide.\n");
                break;
            case EINVAL:
                printf("Arguments invalides ou socket fermée.\n");
                break;
            case ENOBUFS:
                printf("Pas assez de mémoire pour traiter la demande.\n");
                break;
            default:
                printf("Erreur inconnue : %d\n", errno);
                break;

    	}
		close(this->_fds[1]);//!
        this->_fds[1] = -1;
        //!exitclient
        return ;
    }
	if (!ret) //?????
    {
        Logger::getInstance().log(INFO, "end cgi");
        close(this->_fds[1]);//!
        this->_fds[1] = -1;
        //!exitclient
        return ;
    }
    std::string str(client.messageRecv.data(), client.messageRecv.data()
		+ static_cast<size_t>(ret));      
	std::stringstream ss; ss << "data sent to cgi: -" << str << "-";	
	Logger::getInstance().log(DEBUG, ss.str(), client);

	client.messageRecv.erase(client.messageRecv.begin(),
        client.messageRecv.begin() + ret);
		std::cout << "FF " << eof << " FF " << client.messageRecv.size() << std::endl;
	if (eof && client.messageRecv.empty())
	{
		std::cout << "EOOOOOOOOOOOOOOOOOOOOOOFFF" << std::endl;
		shutdown(_fds[1], SHUT_WR);
	}
}

// bool Server::replyClient(size_t i, std::vector<char> & response,
// 	ssize_t repSize)
// {	
// 	Logger::getInstance().log(INFO, "reply client", this->_clients[i]);
// 	printResponse(response);

// 	response.erase(response.begin() + repSize, response.end());
// 	ssize_t ret;
		
// 	if ((ret = send(this->_clients[i].fd, response.data(), response.size(),
// 		MSG_NOSIGNAL)) < 0)
// 	return Logger::getInstance().log(ERROR, "send", this->_clients[i]),
// 		this->exitClient(i), true;		

// 	std::string str(response.data(), response.data()
// 		+ static_cast<size_t>(ret));      
// 	std::stringstream ss; ss << "data sent to client: -" << str << "-";	
// 	Logger::getInstance().log(DEBUG, ss.str(), this->_clients[i]); 

// 	response.erase(response.begin(), response.begin() + ret);	
// 	return false;
// }

// ssize_t advCircle(ssize_t head, ssize_t stop, ssize_t adv, ssize_t buffSize)
// {
// 	ssize_t tmp;
// 	tmp = (head + adv) % buffSize;
// 	if (tmp > stop) 
// }

// void circularBuffer()
// {
// 	ssize_t readHead = 0, writeHead = 0;
// 	char buff[42];
// 	ssize_t len = sizeof(buff) - readHead;
// 	while (true)
// 	{
// 		ssize_t retr = getBody(readHead, sizeof(buff) - readHead);
// 		writeHead = advCircle(writeHead, readHead, retr, sizeof(buff));
// 		ssize_t rets = send(this->_fds[1], readHead, sizeof(buff) - writeHead, 0);		
// 		readHead = advCircle(readHead, writeHead, rets, sizeof(buff));



// 	}
		

	 
// }

