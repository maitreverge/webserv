#include <csignal>
#include <termios.h>
#include "master.hpp"
#include "Kernel.hpp"
#include "Logger.hpp"
#include "Server.hpp"

void disableSignalEcho()
{
    struct termios tty;
	memset(&tty, 0, sizeof(tty));
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
	int verbose = 1;
	if (argv[1] && std::string(argv[1]) == "-s")	
		argc--,	argv++,	verbose = 0;	
	if (argc > 2)
		return std::cerr << "Usage: ./webserv -s [config.ini]" << std::endl, 1;
	std::cout << std::endl;
	Logger::getInstance(verbose).log(INFO, "\e[1;3;36mServer is Online!\e[0m",
		L_ALW);
	std::cout << std::endl;	
	if (argc == 1)
		Kernel::getInstance();
	else
		Kernel::getInstance(argv[1]);		
	std::cout << std::endl;
	Logger::getInstance().log(INFO, "\e[1;3;91mServer is Offline.\e[0m",
		L_ALW);	
	std::cout << std::endl;	
	return (0);
}
