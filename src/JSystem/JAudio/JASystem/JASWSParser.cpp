#include "JSystem/JAudio/JASystem/JASWSParser.hpp"

namespace JASystem {

u32 WSParser::sUsedHeapSize = 0;

u32 WSParser::getGroupCount(void* data) { return 0; }

TWaveBank* WSParser::createBasicWaveBank(void* data) { return 0; }

TWaveBank* WSParser::createSimpleWaveBank(void* data) { return 0; }

u32 WSParser::getUsedHeapSize() { return 0; }

} // namespace JASystem

template <typename T> T* JSUConvertOffsetToPtr(const void* base, u32 offset)
{
	if (offset == 0) {
		return 0;
	}

	return (T*)((u32)base + offset);
}
