/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TryHardTeam <TryHardTeam@123.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:10:05 by TryHardTeam       #+#    #+#             */
/*   Updated: 2024/12/16 12:10:06 by TryHardTeam      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	
    void	    child(Client & client);
	void 		callExecve(Client & client, const std::string & interpreter);
	std::string getPath(Client & client, const std::string & interpreter);

	void 		hasError(Client &client, std::string err, int option);
	void 		exitCodeHandle(int status, Client & client, std::string & err);
	void 		isTimeout(Client & client, std::string err);
	double 		getTimeSpan(Client & client) const;
    void 		retHandle(Client & client, ssize_t ret, std::string err, 
    	std::string info);
	bool 		shutdownHandle(Client & client, bool eof);
	std::string getApiKey(Client & client, std::string & interPath);

    public:
    
        int _fds[2];

        Cgi();
		Cgi(const Cgi & src);
	    Cgi & operator=(const Cgi &);
        ~Cgi();  

        void    launch(Client & client); 
        bool 	getBody(Client & client);
	    void 	setBody(Client & client, bool eof);
};
