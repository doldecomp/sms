#ifndef JASKERNELDEBUG_HPP
#define JASKERNELDEBUG_HPP

namespace JASystem {

namespace Kernel {
	void stackInit(unsigned long long* stack, unsigned long size);
	void stackCheck(unsigned long long* stack);
	void setLdHistMax(long max);
	void checkLdHistInit();
	void checkLdHistIn(unsigned long id, unsigned char* data,
	                   unsigned long size);
	void checkLdHistOut();
	void checkLdHistPrint();
} // namespace Kernel

} // namespace JASystem

#endif // JASKERNELDEBUG_HPP
