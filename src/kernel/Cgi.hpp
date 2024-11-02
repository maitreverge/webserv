#pragma once

#include <unistd.h>

struct Client;

class Cgi
{
    int fds[2];

    public:
             
        void launch();  
        void getBody(Client & client);   
};