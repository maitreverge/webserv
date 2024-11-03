#include "Cgi.hpp"
#include "Logger.hpp"

void Cgi::launch() //!WARNING select
{           
    socketpair(AF_UNIX, SOCK_STREAM, 0, this->fds);
    pid_t pid = fork();
    if (pid < 0)
        Logger::getInstance().log(ERROR, "fork failed");
    else if (pid == 0)
    {       
        dup2(this->fds[0], STDIN_FILENO); 
        dup2(this->fds[0], STDOUT_FILENO); 
        close(this->fds[0]);//!
        close(this->fds[1]);//!
        // int t[2] = {1,2, NULL}
        // char *args[] = {NULL};
        // char *envp[] = {NULL};
        // execve("./cgi/a.out", args, envp);
        chdir("./cgi");
        std::string str("PATH_INFO=coucoucpathinfo");
        char *env[] = 
        {
            const_cast<char *>(str.c_str()), NULL
        };
		char *argv[] = {NULL};
        execve("a.out", argv, env);       
        std::cout << "execve fail" << std::endl;
    } 
    else
    {                
        close(this->fds[0]);//!

        // char buff[150];
        // char buff2[5] = {'s','a','l','u','t'};
        // send(this->fds[1], buff2, sizeof(buff2), 0);
        // ssize_t retp = recv(this->fds[1], buff, sizeof(buff), 0);
        // for (ssize_t i = 0; i < retp; i++)
            // std::cout << "." << buff[i];   
    }    
}

ssize_t Cgi::getBody(Client & client)
{
    Logger::getInstance().log(INFO, "hello from parent");
    
    
    // char buff[150];
    // char buff2[5] = {'s','a','l','u','t'};
    // send(this->fds[1], buff2, sizeof(buff2), 0);
    client.messageSend.clear();
    client.messageSend.resize(SEND_BUFF_SIZE);//!
    ssize_t ret; 

    try
    {
        ret = recv(this->fds[1], client.messageSend.data(),
            client.messageSend.size(), 0);
        /* code */
    }
    catch(const std::exception& e)
    {
        std::cerr << "CATCH" << e.what() << '\n';
    }
    
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
		close(this->fds[1]);//!
        return 0;
    }
    std::cout << "RET " << ret << std::endl;
    // for (ssize_t i = 0; i < ret; i++)
    //     std::cout << "." << buff[i];
    if (!ret)
    {
        Logger::getInstance().log(INFO, "end cgi");
        close(this->fds[1]);//!
        
        return 0;
    }
    return ret;
}

void Cgi::setBody(Client & client)
{
 	ssize_t ret = send(this->fds[1], client.messageRecv.data(),
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
		close(this->fds[1]);//!
        return 0;
    }
	if (!ret) //?????
    {
        Logger::getInstance().log(INFO, "end cgi");
        close(this->fds[1]);//!
        
        return 0;
    }
	//! system de recup send
}

ssize_t advCircle(ssize_t head, ssize_t stop, ssize_t adv, ssize_t buffSize)
{
	ssize_t tmp;
	tmp = (head + adv) % buffSize;
	if (tmp > stop) 
}

void circularBuffer()
{
	ssize_t readHead = 0, writeHead = 0;
	char buff[42];
	ssize_t len = sizeof(buff) - readHead;
	while (true)
	{
		ssize_t retr = getBody(readHead, sizeof(buff) - readHead);
		writeHead = advCircle(writeHead, readHead, retr, sizeof(buff));
		ssize_t rets = send(this->fds[1], readHead, sizeof(buff) - writeHead, 0);		
		readHead = advCircle(readHead, writeHead, rets, sizeof(buff));



	}
		

	 
}

