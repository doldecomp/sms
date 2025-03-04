#ifndef JASKERNELDEBUG_HPP
#define JASKERNELDEBUG_HPP

#include <dolphin/types.h>

namespace JASystem {

namespace Kernel {
	void stackInit(u64* stack, u32 size);
	void stackCheck(u64* stack);
	void setLdHistMax(s32 max);
	void checkLdHistInit();
	void checkLdHistIn(u32 id, u8* data, u32 size);
	void checkLdHistOut();
	void checkLdHistPrint();
} // namespace Kernel

} // namespace JASystem

#endif // JASKERNELDEBUG_HPP
