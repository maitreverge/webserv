#include <csignal>
#include <termios.h>
#include "master.hpp"
#include "Kernel.hpp"
#include "Logger.hpp"
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
struct mystruct{int i; mystruct(){i = 42;} };

int main(int argc, char* argv[])
{
	if (argc > 2)
		return (std::cerr << "Usage: ./webserv [config.ini]" << std::endl, 1);

	signal(2, signalHandle);
	signal(3, signalHandle);
	disableSignalEcho();

	std::cout << std::endl;
	Logger::getInstance().log(INFO, "\e[1;3;36mServer is Online!\e[0m");
	std::cout << std::endl;
	if (argc == 1)
		Kernel kernel;
	else
		Kernel kernel(argv[1]);

	std::cout << std::endl;
	Logger::getInstance().log(INFO, "\e[1;3;91mServer is Offline.\e[0m");	
	std::cout << std::endl;	
	return (0);
}