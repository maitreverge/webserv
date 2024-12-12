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

void help()
{
	std::cerr << std::endl;
	std::cerr << HIGH_INTENSITY_YELLOW
		<<" Usage: ./webserv -[hsvdeu] [config.ini]" << RESET << std::endl;
	std::cerr << std::endl;
	std::cerr << "-h : help" << std::endl; 
	std::cerr << "-s : silent" << std::endl;
	std::cerr << "-v : verbose" << std::endl;
	std::cerr << "-d : debug" << std::endl;
	std::cerr << "-e : eval" << std::endl;
	std::cerr << "-u : unroll text" << std::endl;
	std::cerr << std::endl;
	std::exit(1);
}

void switchFlags(const char lt, int & flags)
{
	switch (lt)
	{
		case '-':
			break;	
		case 'h':
			help();
			break;
		case 's':
			flags = L_SLN;
			break;
		case 'u':
			flags |= L_UNR;
			break;
		case 'e':
			flags |= L_EVL;	// Intentionally fall through
		case 'd':
			flags |= L_DEB;	// Intentionally fall through
		case 'v':
			flags |= L_VRB;
			flags &= ~L_SLN;
			break;
		default:
			std::cerr << std::endl << "\e[1;31m    🏴‍☠️   Error: \e[1;95m"
				<< "Bad-Flag: -" << lt << "   🤢 🤮\e[0m" << std::endl, help();						
	}
}

int searchFlags(char* argv[])
{
	int flags = 0;
	
	while (*++argv)
	{
		std::string str(*argv);
		if (!str.empty() && str[0] == '-')
			for (size_t i = 1; i < str.size(); ++i)
				switchFlags(str[i], flags);			
	}
	return flags;
}

char * searchFile(char* argv[])
{
	char * file = NULL;

	while (*++argv)
	{
		std::string str(*argv);
		if (!str.empty() && str[0] != '-')
		{
			if (!file)
				file = *argv;
			else
				std::cerr << std::endl << "\e[1;31m  🏴‍☠️   Error: \e[1;95m"
					<< "Too-Many-Args Usage: ./webserv -[hsvdeu] [config.ini]"
					<< "   🤢 🤮\e[0m" << std::endl << std::endl, std::exit(1);						
		}
	}
	return file;
}

int main(int, char* argv[])
{
	std::signal(2, signalHandle);
	std::signal(3, signalHandle);
	disableSignalEcho();

	char * file = searchFile(argv);
	int flags = searchFlags(argv);

	std::cout << std::endl;
	Logger::getInstance(flags).
		log(INFO, "\e[1;3;36mServer is Online!\e[0m", L_ALW);
	std::cout << std::endl;

	try	{
		Kernel::getInstance(file);
	}
	catch(...){}
	
	if (Logger::getInstance()._nl)
		std::cout << std::endl;
	Logger::getInstance().
		log(INFO, "\e[1;3;91mServer is Offline.\e[0m", L_ALW);	
	std::cout << std::endl;	
	return (0);
}
