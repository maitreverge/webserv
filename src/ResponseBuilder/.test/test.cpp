#include <dirent.h>
#include <iostream>
#include <cstring>

using namespace std;


int main() {

	string clientURI = "/dir1/script.py/hehehe/script/kakakkaka/script.py";
	
	string targetPHP = ".php";
    string targetPython = ".py";

    // Find the first positions of the Python and PHP extensions in the URI
    std::string::size_type pythonLoc = clientURI.find(targetPython);
    std::string::size_type phpLoc = clientURI.find(targetPHP);

	cout << pythonLoc << endl;
	cout << phpLoc << endl;

}
