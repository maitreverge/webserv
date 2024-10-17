#include "master.hpp"
#include "Kernel.hpp"

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

int main(int ac, char **av, char **envp)
{
	(void)(ac);
	(void)(av);
	(void)(envp);
	
	Kernel kernel;
	kernel.setup();
}