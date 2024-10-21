#pragma once

#include "utils_templates.hpp"
#include "libraries.hpp"
#define SPACE " "

class timeStamp
{

private:

	// Useless Coplian methods
	timeStamp( const timeStamp& copy );
	timeStamp& operator=( const timeStamp& right_operator );

public:

	timeStamp( void );
	~timeStamp();

	static string getTime( void );

};