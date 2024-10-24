#include <csignal>
#include <termios.h>
#include "master.hpp"
#include "Kernel.hpp"
#include "Logger.hpp"
#include "Error.hpp"
#include "Server.hpp"

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

int main()
{
	signal(2, signalHandle);
	disableSignalEcho();
	
	std::cout << std::endl;
	Logger::getInstance().log(INFO, "\e[1;3;36mServer is Online!\e[0m\n");
	Error::getInstance().handleError(404, NULL);
	Kernel kernel;
	std::cout << std::endl;
	Logger::getInstance().log(INFO, "\e[1;3;91mServer is Offline.\e[0m\n");	

	return (0);
}