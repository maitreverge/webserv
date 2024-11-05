#pragma once

#include <unistd.h>

struct Client;

class Cgi
{

    public:
    
        Cgi();
		Cgi & operator=(const Cgi &);
        ~Cgi();  
        int _fds[2];
        void    launch();  
        ssize_t getBody(Client & client);
		void 	setBody(Client & client, bool eof);
};