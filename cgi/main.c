#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// std::vector<char> buildHardResponseTest()
// {
// 	std::stringstream ss;	
// 	std::string strBody =
// "<html>\
// <head><title>My Styled Page</title></head>\
// <body style=\"background-color: #f0f0f0; text-align: center; padding: 50px;\">\
// <h1 style=\"color: #ff5733; font-family: Arial, sans-serif;\">Hello, World!</h1>\
// <p style=\"color: #555; font-size: 18px;\">This is a simple page with inline CSS.</p>\
// </body>\
// </html>";	
// 	ss << 
// "HTTP/1.1 200 OK\r\n\
// Content-Type: text/html\r\n\
// Content-Length: "
// 	<< strBody.size() <<
// "\r\n\
// Connection: keep-alive\r\n\
// \r\n\
// " << strBody;	
// 	std::string strtest(ss.str()); 
// 	std::vector<char> res (strtest.begin(), strtest.end());
// 	std::vector<char> flo (strBody.begin(), strBody.end());	
// 	string flo_file_extension = "index.html";	
// 	return res;	
// }

int main()
{
  
    char buff[20] ={0};
    ssize_t ret;
	
    while ((ret = read(STDIN_FILENO, buff, 20)) > 0)
    {
        buff[ret] = 0;
		write(2, buff, strlen(buff));
        // printf("coucou%s", buff);
    }

    if (ret < 0)
    {
        printf("error read\n");
    }
     write(2, "whou", 4);

   
    // for (unsigned long i = 0; i < sizeof(buff); i++)
    //     printf(";%c", buff[i]);
    // printf("this is path info %s", getenv("PATH_INFO"));
   
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
    // fflush(stdout);
    // while(1);
    printf("%d", len);
    printf("%s", end);
    printf("%s", body);
    usleep(5);
    return (0);
}
