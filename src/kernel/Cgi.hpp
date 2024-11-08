#pragma once

#include <unistd.h>
#include <string>

struct Client;

class Cgi
{
	void retHandle(Client & client, ssize_t & ret, std::string err,
		std::string info);

    public:
    
        Cgi();
		Cgi & operator=(const Cgi &);
        ~Cgi();  

        int     _fds[2];
        void    launch();  
        bool 	getBody(Client & client);
		void 	setBody(Client & client, bool eof);
};