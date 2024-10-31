#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    printf("hello from cgi\n");
    char buff[20];
    read(STDIN_FILENO, &buff, sizeof(buff)); //lit ds le fd 0
    for (unsigned long i = 0; i < sizeof(buff); i++)
        printf(";%c", buff[i]);
    printf("this is path info %s", getenv("PATH_INFO"));
    return (0);
}
