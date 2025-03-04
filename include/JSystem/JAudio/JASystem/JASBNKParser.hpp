#ifndef JASBNKPARSER_HPP
#define JASBNKPARSER_HPP

#include <JSystem/JAudio/JASystem/JASBasicBank.hpp>
#include <dolphin/types.h>

namespace JASystem {

namespace BNKParser {

	struct THeader;
	struct TInst;
	struct TOsc;
	struct TRand;
	struct TSense;
	struct TKeymap;
	struct TVmap;
	struct TPerc;
	struct TPmap;

	TBasicBank* createBasicBank(void* data);
	TOsc* findOscPtr(TBasicBank* bank, THeader* header, TOsc* osc);
	s16* getOscTableEndPtr(s16* ptr);
	u32 getUsedHeapSize();

	extern u32 sUsedHeapSize;

} // namespace BNKParser

} // namespace JASystem

template <typename T> T* JSUConvertOffsetToPtr(const void* base, u32 offset);

#endif // JASBNKPARSER_HPP
