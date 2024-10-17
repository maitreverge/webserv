#include <iostream>
#include <Config.hpp>

class Kernel
{
	Config config;

	public:
		Kernel(void);
		~Kernel(void);
		Kernel(const Kernel & src);
		Kernel & operator=(const Kernel & rhs);		

};