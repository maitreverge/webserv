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

int searchFlags(char* argv[])
{
	int flags = 0;

	while (*++argv)
	{
		std::string str(*argv);
		if (!str.empty() && str[0] == '-')
			for (size_t i = 1; i < str.size(); ++i)
			{
				if (str[i] == 'v')
					flags |= L_VRB;
				else if (str[i] == 's')
					flags |= L_SLN;
				else if (str[i] == 'd')
					flags |= L_DEB;
				else if (str[i] == 'u')
					flags |= L_UNR;
				else
					std::cerr <<
						"Bad-Flag Usage: ./webserv [-v-s-d] [config.ini]"
						<< std::endl, std::exit(1);					
			}			
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
				std::cerr <<
					"Too-Many-Files Usage: ./webserv [-v -s -d] [config.ini]"
					<< std::endl, std::exit(1);						
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

	std::cout << std::endl;
	Logger::getInstance(searchFlags(argv)).
		log(INFO, "\e[1;3;36mServer is Online!\e[0m", L_ALW);
	std::cout << std::endl;

	Kernel::getInstance(file);

	std::cout << std::endl;
	Logger::getInstance().log(INFO, "\e[1;3;91mServer is Offline.\e[0m", L_ALW);	
	std::cout << std::endl;	
	return (0);
}
