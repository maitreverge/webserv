#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{  
	// char * path_info = getenv("PATH_INFO");
    char buff[2000] = {0};
    ssize_t ret;
	char send[2000] = {0};
    while ((ret = read(STDIN_FILENO, buff, 2000)) > 0)
    {
		// write(2, "while\n", 6);
		// write(2, (ret), 6);
		// write(2, "\n", 1);
    	buff[ret] = 0;
		// write(2, buff, strlen(buff));        
		// strcpy(send, buff);
    }
    if (ret < 0)    
        printf("error read\n");
    
    // if (getenv("PATH_INFO"))
    //     write(2, getenv("PATH_INFO"), strlen(getenv("PATH_INFO")));
    // int fd = open("local", O_RDONLY);
    // if (fd < 0)
    //     printf("error open\n");
    // else
    // {
    //     while ((ret = read(fd, buff, 20)) > 0)
    //     {
    //         buff[ret] = 0;
    //         write(2, buff, strlen(buff));        
    //     }
    // }
    // close(fd);

    char * body = "<html>\
<head><title>My Styled Page</title></head>\
<body style=\"background-color: #f0f0f0; text-align: center; padding: 50px;\">\
<h1 style=\"color: #ff5733; font-family: Arial, sans-serif;\">Hello, World!</h1>\
<p style=\"color: #555; font-size: 18px;\">";  
	char * body_end = " </p>\
</body>\
</html>";
    char * header = "HTTP/1.1 200 OK\r\n\
Content-Type: text/html\r\n\
Content-Length: ";
    int len = strlen(body) + strlen(body_end);
	// if (path_info)
	// 	len += strlen(path_info);

	len += strlen(buff);
	// len += ret;
    char * end = "\r\n\r\n";

    printf("%s", header);
    printf("%d", len);
    printf("%s", end);
    printf("%s", body);
	// if (path_info)
	// 	printf("%s", path_info);
	// if (path_info)
	printf("%s", buff);
    printf("%s", body_end);
	// while (1);
    return (0);
}