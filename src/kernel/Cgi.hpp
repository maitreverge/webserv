#pragma once

#include <unistd.h>

struct Client;

class Cgi
{
    int fds[2];

    public:
             
        void    launch();  
        ssize_t getBody(Client & client);
		void 	setBody(Client & client);
};