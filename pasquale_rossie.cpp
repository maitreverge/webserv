#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

typedef struct s_client
{
    int     id;
    char    msg[30000];
}   t_client;

t_client    clients[1024];
fd_set      read_set, write_set, current;
int         maxfd = 0, gid = 0;
char        send_buffer[300000], recv_buffer[300000];

void    err(char  *msg)
{
    if (msg)
        write(2, msg, strlen(msg));
    else
        write(2, "Fatal error", 11);
    write(2, "\n", 1);
    exit(1);
}

void    send_to_all(int except)
{
    for (int fd = 0; fd <= maxfd; fd++)
    {
        if  (FD_ISSET(fd, &write_set) && fd != except)
            if (send(fd, send_buffer, strlen(send_buffer), 0) == -1)
                err(NULL);
    }
}

// ./a.out PORT1 PORT2 PORT3 ...
int     main(int ac, char **av)
{
    if (ac < 2)
        err("Wrong number of arguments");

	// Stockage des structures
	std::vector<struct sockaddr_in> vector_sockaddrr;

	// Stockage des file descriptors
	std::vector<int> vector_serverfd;
	
    socklen_t           len;

	// Creation des structures
	for (size_t i = 0; i < ac -1; i++)
	{
		struct sockaddr_in new_sock_addrr;
		vector_sockaddrr.push_back(new_sock_addrr);
	}
	
	// Init each socket
	for (size_t i = 0; i < ac -1; i++){

		int serverfd = socket(AF_INET, SOCK_STREAM, 0);
		if (serverfd == -1)
			err(NULL);

		vector_serverfd.push_back(serverfd);
		maxfd = serverfd;

	}

	// FD_ZERO = init the set to be empty, `initializes or clears a set`
    FD_ZERO(&current);

	for (size_t i = 0; i < ac -1; i++){
		// FD_SET = Adds a file descriptor to a set
    	FD_SET(vector_serverfd[i], &current);
	}
	
	for (size_t i = 0; i < ac -1; i++){
		bzero(&vector_sockaddrr[i], sizeof(vector_sockaddrr[i]));
	}

    bzero(clients, sizeof(clients));

	// ! Set up les structures en boucle
	for (size_t i = 0; i < ac -1; i++){
		
		vector_sockaddrr[i].sin_family = AF_INET;
		vector_sockaddrr[i].sin_addr.s_addr = htonl(INADDR_ANY);
		vector_sockaddrr[i].sin_port = htons(atoi(av[i + 1]));
	}

	// Bind each listenning socket to the corresponding port
	for (size_t i = 0; i < ac -1; i++){
		
		std::cout << "YO_" << i << std::endl;
    	if (bind(vector_serverfd[i], (const struct sockaddr *)&vector_sockaddrr[i], sizeof(vector_sockaddrr[i])) == -1)
        	err(NULL);
		
		if (listen(vector_serverfd[i], 100) == -1) // 100 clients max par port
        	err(NULL);
	}

	struct timeval timeout;
	timeout.tv_sec = 5;  // 5 seconds
	timeout.tv_usec = 0; // 0 microseconds

	// Main kernel loop
	bool track = false; // bool for tracking the creation socket part from the client part
	int i = 0;
    while (1)
    {
        read_set = write_set = current; // reset each socket to zero
        if (select(maxfd + 1, &read_set, &write_set, 0, &timeout) == -1){
			printf("Hello from select %i\n", i);
			i++;
			if (i == __INT_MAX__)
				i = 0;
			continue;
		}
		else
		{
			printf("Hello from select2 %i\n", i);
			i++;
			if (i == __INT_MAX__)
				i = 0;

		}
        for (int fd = 0; fd <= maxfd; fd++)
        {
			// FD_ISSET = Checks if a specific file descriptor is set in a set
			
			/*
				! Use FD_ISSET after calling select() to determine
				! if a particular file descriptor is ready (i.e., has data to read or can write).
			*/

            if (FD_ISSET(fd, &read_set))
            {
				track = false;
				for (size_t i = 0; i < ac -1; i++) // ! outter loop for checking within the vectorsZ
				{
					if (fd == vector_serverfd[i])
					{
						track = true;
						int clientfd = accept(vector_serverfd[i], (struct sockaddr *)&vector_sockaddrr[i], &len);

						if (clientfd == -1) continue;

						if (clientfd > maxfd) maxfd = clientfd;

						clients[clientfd].id = gid++;

						// ! FD_SET = Adds a file descriptor to a set
						FD_SET(clientfd, &current);

						sprintf(send_buffer, "server: client %d just arrived\n", clients[clientfd].id);
						send_to_all(clientfd);
						break;
					}
				}
				if (!track)
				{
					int ret = recv(fd, recv_buffer, sizeof(recv_buffer), 0);
					if (ret <= 0)
					{
						sprintf(send_buffer, "server: client %d just left\n", clients[fd].id);
						send_to_all(fd);
						
						// FD_CLR : Removes a file descriptor from a set.

						/*
							! Use FD_CLR when you want to stop monitoring a specific file descriptor.
							! This might happen if a client disconnects, and you want to stop watching its socket.
						*/
						FD_CLR(fd, &current);
						close(fd);
						bzero(clients[fd].msg, strlen(clients[fd].msg));
					}
					else
					{
						for (int i = 0, j = strlen(clients[fd].msg); i < ret; i++, j++)
						{
							clients[fd].msg[j] = recv_buffer[i];
							if (clients[fd].msg[j] == '\n')
							{
								clients[fd].msg[j] = '\0';
								sprintf(send_buffer, "client %d: %s\n", clients[fd].id, clients[fd].msg);
								send_to_all(fd);
								bzero(clients[fd].msg, strlen(clients[fd].msg));
								j = -1;
							}
						}
					}
				}
			} // if (FD_ISSET(fd, &read_set))
		} // for (int fd = 0; fd <= maxfd; fd++)
	} // while(1)
	return (0);
} 