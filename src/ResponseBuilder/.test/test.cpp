#include <dirent.h>
#include <iostream>
#include <cstring>
#include <vector>

using namespace std;


int main() {

	#define itVector vector<string>::iterator


	vector< string > targets;

	targets.push_back("/a/b/c/./../../g");
	targets.push_back("../a/b/c/./../../g");
	targets.push_back("./b/c/.../.../.../.g");
	targets.push_back("./b./c/./../../g");
	targets.push_back("/c/../............./../g");
	targets.push_back("/./../../g");
	targets.push_back("/../../g");
	targets.push_back("/../g");
	targets.push_back("/g");
	

	vector< string >needle;
	// To remove
	needle.push_back("../");
	needle.push_back("./");
	needle.push_back("..");


	int i = 0;
	for (itVector it = targets.begin(); it != targets.end(); ++it, ++i)
	{
		std::cout << "Current target "<< i <<  " BEFORE = " << *it << std::endl;
	}
	i = 0;


	for (itVector outterIT = needle.begin(); outterIT != needle.end(); ++outterIT )
	{

		for (itVector innerIT = targets.begin(); innerIT != targets.end(); ++innerIT )
		{
			while (innerIT->find(*outterIT) != std::string::npos)
			{
				innerIT->erase(innerIT->find(*outterIT), outterIT->length());
			}
		}
	}

	std::cout << "=================" << std::endl;

	for (itVector it = targets.begin(); it != targets.end(); ++it, ++i)
	{
		std::cout << "Current target "<< i <<  " AFTER = " << *it << std::endl;
	}

}
