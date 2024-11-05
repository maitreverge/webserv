#include <dirent.h>
#include <iostream>
#include <cstring>

using namespace std;


int main() {

	int j = 0;
	for (size_t i = 0; i < 10; i++, j++)
	{
		std::cout << "i =" << i << endl;
		std::cout << "j =" << j << endl;
	}
	

	return 0;
}
