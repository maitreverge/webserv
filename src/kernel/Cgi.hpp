#pragma once

#include <unistd.h>

struct Client;

class Cgi
{
	bool retHandle(ssize_t ret, std::string err, std::string info);

    public:
    
        Cgi();
		Cgi & operator=(const Cgi &);
        ~Cgi();  

        int     _fds[2];
        void    launch();  
        bool 	getBody(Client & client);
		void 	setBody(Client & client, bool eof);
};