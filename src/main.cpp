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

int main()
{
	signal(2, signalHandle);
	signal(3, signalHandle);
	disableSignalEcho();
		
	std::cout << std::endl;
	Logger::getInstance().log(INFO, "Server is Online!");
	Kernel kernel;
	std::cout << std::endl;
	Logger::getInstance().log(INFO, "Server is Offline.");	

	return (0);
}