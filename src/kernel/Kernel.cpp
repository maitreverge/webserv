#include "Kernel.hpp"

Kernel::Kernel(void)
{
	std::cout << "hello from kernel" << std::endl;
}
Kernel::~Kernel(void)
{

}
Kernel::Kernel(const Kernel &)
{

}

Kernel & Kernel::operator=(const Kernel &)
{
	return *this;
}