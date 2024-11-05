#include <dirent.h>
#include <iostream>
#include <cstring>

using namespace std;


int main() {

	string clientURI = "/dir111/dir2/dir3";
	
	std::cout << "clientURI = " << clientURI << std::endl;

	string racine = "/dir1";

	string root = "/salut/bande/de/salopes/hehehehhehehehehehheheheheheheh";

	clientURI.replace(0, racine.size(), root);

	std::cout << "racine = " << racine << std::endl;

	std::cout << "clientURI after modification = " << clientURI << std::endl;

}
