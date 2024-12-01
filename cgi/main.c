#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main()
{ 
	char * path_info = getenv("PATH_INFO");
	if (!path_info || !strlen(path_info))
	{
		printf("path info est null\n");
		path_info = "/";
		printf("%s\n", path_info);
	}

	char path[256] = {0};
	printf(" path: -%s-\n", path);
	printf("pathinfo: -%s-\n", path_info);
	strcat(path, path_info + 1); 
	printf("path: -%s-\n", path);

	strcat(path, "body.txt");
	printf("path: -%s-\n", path);

    int fd = open(path, O_CREAT | O_RDWR | O_APPEND, 0777);
    if (fd < 0)
        return write(2, "error open\n", 11), 1;

    char buff[30000] = {0};
    ssize_t ret;	
    while ((ret = read(STDIN_FILENO, buff, 20)) > 0)
	{
		buff[ret] = 0;
		if (write(fd, buff, strlen(buff)) < 0)
			return close(fd), write(2, "error write\n", 12), 1;
	}
    close(fd);		  
    if (ret < 0)    
        return write(2, "error read\n", 11), 1;

    char * body = "<html>\
<head><title>My Styled Page</title></head>\
<body style=\"background-color: #f0f0f0; text-align: center; padding: 50px;\">\
<h1 style=\"color: #ff5733; font-family: Arial, sans-serif;\">Successfully Post</h1>\
<p style=\"color: #555; font-size: 18px;\"> at";  
	char * body_end = " </p>\
</body>\
</html>";
    char * header = "HTTP/1.1 201 CREATED\r\n\
Content-Type: text/html\r\n\
Content-Length: ";
    int len = strlen(body) + strlen(body_end);
	if (path_info)
		len += strlen(path_info);
    char * end = "\r\n\r\n";

    printf("%s", header);
    printf("%d", len);
    printf("%s", end);
    printf("%s", body);
	if (path_info)
		printf("%s", path_info);
    printf("%s", body_end);
    return (0);
}
