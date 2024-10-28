#include "ResponseBuilder.hpp"
#include "Logger.hpp"

void ResponseBuilder::launchCGI( void ){

	// ! STEP 1 : Export PATH_INFO

	// ! LAST STEP : UNSET PATH_INFO
}

void ResponseBuilder::checkCGI( void ){

	// GET /cgi-bin/script.py/user/123/profile HTTP/1.1

	// string _realURI = "dir1/dir2/script.py/dindon/farci/aux/pommes";
	string targetPython = ".py";
	string targetPHP = ".php";

	std::string::size_type phpLoc = _realURI.find(targetPHP);
	std::string::size_type pythonLoc = _realURI.find(targetPython);

	if (phpLoc == std::string::npos and phpLoc == std::string::npos )
	{
		_isCGI = false;
		return;
	}
	
	std::string::size_type realLoc = ( phpLoc == std::string::npos ) ? pythonLoc : phpLoc;

	_pathInfo = _realURI.substr(realLoc);
}