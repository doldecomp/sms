#ifndef JASWSPARSER_HPP
#define JASWSPARSER_HPP

#include <dolphin/types.h>

namespace JASystem {

class TWaveBank;

class WSParser {
public:
	struct TCtrlGroup;
	struct TCtrlScene;
	struct TCtrl;
	struct TWaveArchiveBank;
	struct TWaveArchive;
	struct TCtrlWave;
	struct TWave;

	static u32 getGroupCount(void* data);
	static TWaveBank* createBasicWaveBank(void* data);
	static TWaveBank* createSimpleWaveBank(void* data);
	u32 getUsedHeapSize();

	static u32 sUsedHeapSize;
};

} // namespace JASystem

template <typename T> T* JSUConvertOffsetToPtr(const void* base, u32 offset);

#endif // JASWSPARSER_HPP
