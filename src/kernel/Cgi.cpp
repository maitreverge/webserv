#include "Cgi.hpp"
#include "Logger.hpp"

void Cgi::launch()
{           
    socketpair(AF_UNIX, SOCK_STREAM, 0, this->fds);
    pid_t pid = fork();
    if (pid < 0)
        Logger::getInstance().log(ERROR, "fork failed");
    else if (pid == 0)
    {
       
        dup2(this->fds[0], STDIN_FILENO); 
        dup2(this->fds[0], STDOUT_FILENO); 
        close(this->fds[0]);
        close(this->fds[1]);
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
        std::cout << "execve fail" << std::endl;
    } 
    else
    {                
        close(this->fds[0]);

        // char buff[150];
        // char buff2[5] = {'s','a','l','u','t'};
        // send(this->fds[1], buff2, sizeof(buff2), 0);
        // ssize_t retp = recv(this->fds[1], buff, sizeof(buff), 0);
        // for (ssize_t i = 0; i < retp; i++)
            // std::cout << "." << buff[i];   
    }    
}

ssize_t Cgi::getBody(Client & client)
{
    Logger::getInstance().log(INFO, "hello from parent");
    
    
    // char buff[150];
    // char buff2[5] = {'s','a','l','u','t'};
    // send(this->fds[1], buff2, sizeof(buff2), 0);
    client.messageSend.clear();
    client.messageSend.resize(SEND_BUFF_SIZE);//!
    ssize_t ret; 

    try
    {
        ret = recv(this->fds[1], client.messageSend.data(),
            client.messageSend.size(), 0);
        /* code */
    }
    catch(const std::exception& e)
    {
        std::cerr << "CATCH" << e.what() << '\n';
    }
    
    if (ret < 0)
    {
        Logger::getInstance().log(ERROR, "recv cgi");
        return 0;
    }
    std::cout << "RET " << ret;
    // for (ssize_t i = 0; i < ret; i++)
    //     std::cout << "." << buff[i];
    if (!ret)
    {
        Logger::getInstance().log(INFO, "end cgi");
        close(this->fds[1]);
            // close(this->fds[0]);
        return 0;
    }
    return ret;
}