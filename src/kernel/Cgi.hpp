#pragma once

#include <unistd.h>
#include <string>
#include <ctime>

#define TIMEOUT_CGI 2

struct Client;

class Cgi
{
	std::clock_t _start;

	bool retHandle(Client & client, ssize_t ret, std::string err, 
        std::string info);
	bool isTimeout(Client & client, std::string err);

    public:
    
        Cgi();
		Cgi & operator=(const Cgi &);
        ~Cgi();  

        int     _fds[2];
        void    launch();  
        bool 	getBody(Client & client);
		void 	setBody(Client & client, bool eof);
};
