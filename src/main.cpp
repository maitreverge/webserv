#include <csignal>
#include <termios.h>
#include "master.hpp"
#include "Kernel.hpp"
#include "Logger.hpp"
#include "Server.hpp"

void errnoHandle() //! a suppr
{
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
}

void disableSignalEcho()
{
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);     
    tty.c_lflag &= static_cast<unsigned int>(~ECHOCTL);  
    tcsetattr(STDIN_FILENO, TCSANOW, &tty); 
}

void signalHandle(int)
{		
	Kernel::_exit = true;	
}

int main(int argc, char* argv[])
{
	std::signal(2, signalHandle);
	std::signal(3, signalHandle);
	disableSignalEcho();
	int verbose = 0;
	if (argv[1] && std::string(argv[1]) == "-v")
	{
		argc--;
		argv++;
		verbose = 1;
	}
	if (argc > 2)
		return std::cerr << "Usage: ./webserv -v [config.ini]" << std::endl, 1;
	std::cout << std::endl;
	Logger::getInstance(verbose).log(INFO, "\e[1;3;36mServer is Online!\e[0m");
	std::cout << std::endl;
	
	if (argc == 1)
		Kernel::getInstance();
	else
		Kernel::getInstance(argv[1]);
		
	std::cout << std::endl;
	Logger::getInstance().log(INFO, "\e[1;3;91mServer is Offline.\e[0m");	
	std::cout << std::endl;	
	return (0);
}
