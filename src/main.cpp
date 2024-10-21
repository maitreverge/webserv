#include "master.hpp"
#include "Kernel.hpp"
#include <csignal>
#include <termios.h>

void disableSignalEcho()
{
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);     
    tty.c_lflag &= static_cast<unsigned int>(~ECHOCTL);  
    tcsetattr(STDIN_FILENO, TCSANOW, &tty); 
}

void signalHandle(int)
{
	std::cout << "Bye bye!" << std::endl;
	Kernel::_exit = true;	
}

int main(int ac, char **av, char **envp)
{
	(void)(ac);
	(void)(av);
	(void)(envp);
	
	signal(2, signalHandle);
	disableSignalEcho();
	Kernel kernel;	
}