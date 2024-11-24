#pragma once

#include <unistd.h>
#include <string>
#include <time.h>

struct Client;

class Cgi
{
    struct timeval	_start;
    pid_t        	_pid;
	int				_lastSpan;
	
	void		hasError(Client & client, std::string err);
    void 		isTimeout(Client & client, std::string err);
	double 		getTimeSpan(Client & client) const;
    void 		retHandle(Client & client, ssize_t ret, std::string err, 
    std::string info);
	std::string getPath(const std::string & exe, Client & client);

    public:
    
        int _fds[2];

        Cgi();
        ~Cgi();  
	    Cgi & operator=(const Cgi &);

        void    launch(Client & client); 
        void    child(Client & client);
        bool 	getBody(Client & client);
	    void 	setBody(Client & client, bool eof);
};
