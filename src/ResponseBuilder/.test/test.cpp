// #include <dirent.h>
// #include <iostream>
// #include <cstring>

// using namespace std;


// int main() {

// 	string foo = "Content-Disposition: form-data; name=\"file1\"; filename=\"fr.subject.pdf\" \r\n";
	
// 	string needle = "filename=\"";

// 	size_t startPos = foo.find(needle);
// 	size_t endPos;

// 	if (startPos != std::string::npos)
// 	{
// 		startPos += needle.length();

// 		endPos = foo.find("\"", startPos);

// 		string hey = foo.substr(startPos, endPos - startPos);

// 		std::cout << hey << std::endl;
// 	}

// }
