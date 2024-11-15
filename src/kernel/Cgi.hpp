#pragma once

#include <unistd.h>
#include <string>
#include <ctime>



struct Client;

class Cgi
{
    std::clock_t _start;
    pid_t        _pid;

    bool retHandle(Client & client, ssize_t ret, std::string err, 
    std::string info);
    bool isTimeout(Client & client, std::string err);

    public:
    
        int _fds[2];

        Cgi();
	    Cgi & operator=(const Cgi &);
        ~Cgi();  

        void    launch(Client & client); 
        void    child(Client & client);
        bool 	getBody(Client & client);
	    void 	setBody(Client & client, bool eof);
};
