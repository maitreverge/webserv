// #include <dirent.h>
// #include <iostream>
// #include <cstring>

// using namespace std;

// #ifdef UNIT_TEST

// #else

// int main() {
// 	string targetPython = ".py";
// 	string targetPHP = ".php";

// 	string _realURI = "dir1/dir2/script.py/dindon/ducon/mescouilles";

// 	std::string::size_type phpLoc = _realURI.find(targetPHP);
// 	std::string::size_type pythonLoc = _realURI.find(targetPython);

// 	if (phpLoc == std::string::npos and pythonLoc == std::string::npos) {
// 		// _isCGI = false;
// 		return 0;
// 	}

// 	std::string::size_type realLoc = (phpLoc == std::string::npos) ? pythonLoc : phpLoc;
// 	realLoc += (phpLoc == std::string::npos) ? targetPython.length() : targetPHP.length();

// 	string _pathInfo = _realURI.substr(realLoc);

// 	_realURI = _realURI.substr(0, realLoc);


// 	cout << "Path info = " <<  _pathInfo << endl;
// 	cout << "Real URI = " <<  _realURI << endl;

// 	return 0;
// }

// #endif