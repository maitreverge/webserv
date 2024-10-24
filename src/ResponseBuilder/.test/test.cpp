#include <iostream>
#include <string>
#include <string>
#include <sys/stat.h>

using namespace std;

u_int64_t	setContentLenght(string _realURI)
{
	struct stat _fileInfo;
 

	if (stat(_realURI.c_str(), &_fileInfo) == -1)
	{
		return 0;
	}
	
	return _fileInfo.st_size;
}

int main(void)
{
	string _realURI = "image_chat.jpeg";

	u_int64_t helo =  setContentLenght(_realURI);

	std::cout << helo << std::endl;
	
}