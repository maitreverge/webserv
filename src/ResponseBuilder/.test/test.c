#include <dirent.h>
#include <iostream>
#include <cstring>

using namespace std;
int main() {
    
	string targetPython = ".py";
	string targetPHP = ".php";

	string _realURI = "dir1/dir2/script.py/dindon/farci/aux/pommes";

	std::string::size_type phpLoc = _realURI.find(targetPHP);
	std::string::size_type pythonLoc = _realURI.find(targetPython);

	if (phpLoc == std::string::npos and pythonLoc == std::string::npos )
	{
		// _isCGI = false;
		return 0;
	}
	
	std::string::size_type realLoc = ( phpLoc == std::string::npos ) ? pythonLoc : phpLoc;

	string _pathInfo = _realURI.substr(realLoc);

	_pathInfo.erase(_pathInfo.begin() + _pathInfo.find_first_of('/'));

	cout << _pathInfo << endl;
}
