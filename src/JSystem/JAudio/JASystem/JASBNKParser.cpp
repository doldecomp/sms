#include <JSystem/JAudio/JASystem/JASBNKParser.hpp>

namespace JASystem {

namespace BNKParser {

	u32 sUsedHeapSize = 0;

	TBasicBank* createBasicBank(void* data) { return nullptr; }

	TOsc* findOscPtr(TBasicBank* bank, THeader* header, TOsc* osc)
	{
		return nullptr;
	}

	s16* getOscTableEndPtr(s16* ptr) { return nullptr; }

	u32 getUsedHeapSize() { return 0; }

} // namespace BNKParser

} // namespace JASystem

// TODO: Add proper types and implement rest
// template <> TPmap* JSUConvertOffsetToPtr(const void* base, u32 offset)
// {
// 	return nullptr;
// }

template <> short* JSUConvertOffsetToPtr<short>(const void* base, u32 offset)
{
	if (offset == 0) {
		return nullptr;
	}
	return (short*)((u32)base + offset);
}
