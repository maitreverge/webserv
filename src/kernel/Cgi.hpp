#pragma once

#include <unistd.h>
#include <Logger.hpp>

class Cgi
{
    public:
        
        Cgi(){};
        void launch()
        {
            int fds[2];
            socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
            pid_t pid = fork();
            if (pid < 0)
                Logger::getInstance().log(ERROR, "fork failed");
            else if (pid == 0)
            {
                std::cout << "hello from child1" << std::endl;
                close(fds[1]);
                dup2(fds[0], STDIN_FILENO); // met le fd 0 a l aplace de in
                dup2(fds[0], STDOUT_FILENO); // met le fd 0 a l aplace de out
                std::cout << "hello from child2" << std::endl;
                
                close(fds[0]);
                char buff[150];
                read(STDIN_FILENO, &buff, sizeof(buff)); //lit ds le fd 0
                for (unsigned long i = 0; i < sizeof(buff); i++)
                    std::cout << "\n" << buff[i];
                std::cout << std::endl;
            } 
            else
            {
                std::cout << "hello from parent" << std::endl;
                close(fds[0]);
                char buff[150];
                ssize_t ret = recv(fds[1], buff, sizeof(buff), 0);
                for (ssize_t i = 0; i < ret; i++)
                    std::cout << "." << buff[i];
                close(fds[1]);
                wait(NULL);
            }    
        };
};