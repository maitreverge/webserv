// #include <dirent.h>
// #include <iostream>
// #include <cstring>

// using namespace std;


// int main() {

// 	string foo = "Content-Type: multipart/form-data; boundary=----WebKitFormBoundary1XN99skGpOHP8Og8";
	
// 	string needle = "boundary=";

// 	size_t startPos = foo.find(needle);
// 	size_t endPos;

// 	if (startPos != std::string::npos)
// 	{
// 		startPos += needle.length();

// 		endPos = foo.find(" ", startPos);

// 		string hey = foo.substr(startPos, endPos - startPos);

// 		std::cout << hey << std::endl;
// 	}

// }
