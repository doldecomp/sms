#include <JSystem/JAudio/JASystem/JASKernelDebug.hpp>

namespace JASystem {

namespace Kernel {

	void stackInit(u64* stack, u32 size)
	{
		for (u32 i = 1; i < size; ++i)
			stack[i] = 0xfadebabe12345678;
	}

	void stackCheck(u64* stack) { }

	void setLdHistMax(s32 max) { }

	void checkLdHistInit() { }

	void checkLdHistIn(u32 id, u8* data, u32 size) { }

	void checkLdHistOut() { }

	void checkLdHistPrint() { }

} // namespace Kernel

} // namespace JASystem
