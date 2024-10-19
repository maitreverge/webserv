#include "master.hpp"
#include "Kernel.hpp"
#include <csignal>
#include <termios.h>
// int main(int ac, char **av, char **envp){

// 	// -Werror unused variables
// 	(void)(ac);
// 	(void)(av);
// 	(void)(envp);
	
// 	Kernel kernel;
// 	print("Hi !");
// 	printNoEndl("I'm a professional C++ programmer.");
// 	printColorNoEndl(BOLD_HIGH_INTENSITY_MAGENTA, 41 + 3 - 1);
// 	printColor(BOLD_BLUE, 42);
// 	printColor(UNDERLINE_CYAN, "Hello Webserv !");
// 	customExit("Exit message", 127);
// }

void disableSignalEcho()
{
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);     
    tty.c_lflag &= ~ECHOCTL;  
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