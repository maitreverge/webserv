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
                dup2(fds[0], STDIN_FILENO); // met le fd 0 a l aplace de in
                dup2(fds[0], STDOUT_FILENO); // met le fd 0 a l aplace de out
                close(fds[0]);
                close(fds[1]);
                // int t[2] = {1,2, NULL}
                // char *args[] = {NULL};
                // char *envp[] = {NULL};
                // execve("./cgi/a.out", args, envp);
                chdir("./cgi");
                std::string str("PATH_INFO=coucoucpathinfo");
                char *env[] = 
                {
                    const_cast<char *>(str.c_str()), NULL
                };
                execve("a.out", (char *[]){NULL}, env);
                // execve("./cgi/a.out", static_cast<char **>(NULL), static_cast<char **>(NULL));
                std::cout << "execve fail" << std::endl;
            } 
            else
            {
                std::cout << "hello from parent" << std::endl;
                close(fds[0]);
                char buff[150];
                char buff2[5] = {'s','a','l','u','t'};
                send(fds[1], buff2, sizeof(buff2), 0);
                ssize_t retp = recv(fds[1], buff, sizeof(buff), 0);
                for (ssize_t i = 0; i < retp; i++)
                    std::cout << "." << buff[i];
                close(fds[1]);
                // wait(NULL);
            }    
        };
};