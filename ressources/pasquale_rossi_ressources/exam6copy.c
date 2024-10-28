#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

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

void    send_resp(int client)
{  
        if  (FD_ISSET(client, &write_set))
		{			
			sprintf(send_buffer, "bravo rema\n");
            if (send(client, send_buffer, strlen(send_buffer), 0) == -1)
                err(NULL);    
		}
}

int     main(int ac, char **av)
{
    // if (ac != 3)
    //     err("Wrong number of arguments");

    struct sockaddr_in  serveraddr;
    struct sockaddr_in  serveraddr2;

    socklen_t           len;
    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
	int serverfd2 = socket(AF_INET, SOCK_STREAM, 0);

    if (serverfd == -1) err(NULL);
	if (serverfd2 == -1) err(NULL);

    maxfd = serverfd;
	maxfd = serverfd2;

    FD_ZERO(&current);
    FD_SET(serverfd, &current);
	FD_SET(serverfd2, &current);

    bzero(clients, sizeof(clients));
    bzero(&serveraddr, sizeof(serveraddr));
	bzero(&serveraddr2, sizeof(serveraddr2));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(atoi(av[1]));

	serveraddr2.sin_family = AF_INET;
    serveraddr2.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr2.sin_port = htons(atoi(av[2]));
	printf("YO\n");
    if (bind(serverfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1 || listen(serverfd, 100) == -1)
        err(NULL);
	printf("YO2\n");
	if (bind(serverfd2, (const struct sockaddr *)&serveraddr2, sizeof(serveraddr2)) == -1 || listen(serverfd2, 100) == -1)
        err(NULL);
	printf("YO3\n");
    while (1)
    {
        read_set = write_set = current;
        if (select(maxfd + 1, &read_set, &write_set, 0, 0) == -1) continue;

        for (int fd = 0; fd <= maxfd; fd++)
        {
            if (FD_ISSET(fd, &read_set))
            {
                if (fd == serverfd)
                {
                    int clientfd = accept(serverfd, (struct sockaddr *)&serveraddr, &len);

                    if (clientfd == -1) continue;

                    if (clientfd > maxfd) maxfd = clientfd;

                    clients[clientfd].id = gid++;

                    FD_SET(clientfd, &current);

                    sprintf(send_buffer, "server: client %d just arrived\n", clients[clientfd].id);
                    send_to_all(clientfd);
			
                }
				else if (fd == serverfd2)
				{
					int clientfd2 = accept(serverfd2, (struct sockaddr *)&serveraddr2, &len);
					if (clientfd2 == -1) continue;

					if (clientfd2 > maxfd) maxfd = clientfd2;

                    clients[clientfd2].id = gid++;

					FD_SET(clientfd2, &current);

					sprintf(send_buffer, "server: client %d just arrived\n", clients[clientfd2].id);
					send_to_all(clientfd2);
					send_resp(clientfd2);
				}
                else
                {
                    int ret = recv(fd, recv_buffer, sizeof(recv_buffer), 0);
                    if (ret <= 0)
                    {
                        sprintf(send_buffer, "server: client %d just left\n", clients[fd].id);
                        send_to_all(fd);
                        FD_CLR(fd, &current);
                        close(fd);
                        bzero(clients[fd].msg, strlen(clients[fd].msg));
                    }
                    else
                    {
						send_resp(fd);
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
                break;
            }
        }
    }
    return (0);
}