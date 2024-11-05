#include "ResponseBuilder.hpp"
#include "Logger.hpp"


void ResponseBuilder::launchCGI( void ){

	// ! STEP 1 : Export PATH_INFO
	std::cout << "LAUNCH" << std::endl;
	// ! LAST STEP : UNSET PATH_INFO
	this->_cgi.launch(); 
}

void ResponseBuilder::checkCGI( Client & client ){

	std::cout << "check CGI" << std::endl;
	string targetPython = ".py";
	string targetPHP = ".php";

	std::string::size_type phpLoc = _realURI.find(targetPHP);
	std::string::size_type pythonLoc = _realURI.find(targetPython);

	if (phpLoc == std::string::npos and pythonLoc == std::string::npos )
	{
		_isCGI = false;
		std::cout << "check CGI fin 2" << std::endl;
		return;
	}

	_isCGI = true;
	
	std::string::size_type realLoc = ( phpLoc == std::string::npos ) ? pythonLoc : phpLoc;

	realLoc += (phpLoc == std::string::npos) ? targetPython.length() : targetPHP.length();

	_pathInfo = _realURI.substr(realLoc);

	_realURI = _realURI.substr(0, realLoc);
	std::cout << "check CGI fin" << std::endl;
	client.headerRespons.clear();
}