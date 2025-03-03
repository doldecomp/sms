#ifndef JASBNKPARSER_HPP
#define JASBNKPARSER_HPP

#include <JSystem/JAudio/JASystem/JASBasicBank.hpp>

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
	short* getOscTableEndPtr(short* ptr);
	unsigned long getUsedHeapSize();

	extern unsigned long sUsedHeapSize;

} // namespace BNKParser

} // namespace JASystem

template <typename T>
T* JSUConvertOffsetToPtr(const void* base, unsigned long offset);

#endif // JASBNKPARSER_HPP
