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
	disableSignalEcho();
	
	struct mystruct truc;
	struct mystruct truc2;
	std::cout << "truc " << truc.i << std::endl;
	std::cout << "truc2 " << truc2.i << std::endl;
	std::vector<struct mystruct> myvect;
	myvect.reserve(10);
	myvect.push_back(truc);
	struct mystruct * trucptr = &myvect[0];
	std::cout << "one " << trucptr->i << std::endl;
	myvect.push_back(truc2);
	std::cout << "two " << trucptr->i << std::endl;
	std::cout << std::endl;
	Logger::getInstance().log(INFO, "\e[1;3;36mServer is Online!\e[0m\n");
	Kernel kernel;
	std::cout << std::endl;
	Logger::getInstance().log(INFO, "\e[1;3;91mServer is Offline.\e[0m\n");	

	return (0);
}