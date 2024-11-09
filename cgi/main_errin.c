#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{  
    char buff[20] ={0};
    ssize_t ret;
	
    while ((ret = read(STDIN_FILENO, buff, 20)) > 0)
    {
        buff[ret] = 0;
		write(2, buff, strlen(buff)); 
        fflush(stdout); 
        while (1);      
    }
    if (ret < 0)    
        printf("error read\n");    
   
    char * body = "<html>\
<head><title>My Styled Page</title></head>\
<body style=\"background-color: #f0f0f0; text-align: center; padding: 50px;\">\
<h1 style=\"color: #ff5733; font-family: Arial, sans-serif;\">Hello, World!</h1>\
<p style=\"color: #555; font-size: 18px;\">This is a simple page with inline CSS.</p>\
</body>\
</html>";
    char * header = "HTTP/1.1 200 OK\r\n\
Content-Type: text/html\r\n\
Content-Length: ";
    int len = strlen(body);
    char * end = "\r\n\r\n";

    printf("%s", header);
    printf("%d", len);
    printf("%s", end);
    printf("%s", body);
    
    return (0);
}