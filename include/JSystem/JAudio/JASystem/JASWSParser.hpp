#ifndef JASWSPARSER_HPP
#define JASWSPARSER_HPP

#include "types.h"

namespace JASystem {

class WSParser {
public:
	struct TCtrlGroup;
	struct TCtrlScene;
	struct TCtrl;
	struct TWaveArchiveBank;
	struct TWaveArchive;
	struct TCtrlWave;
	struct TWave;

	static int getGroupCount(void* data);
	static TCtrlGroup* createBasicWaveBank(void* data);
	static TCtrlGroup* createSimpleWaveBank(void* data);
	static unsigned long getUsedHeapSize();

	static unsigned long sUsedHeapSize;
};

template <typename T>
T* JSUConvertOffsetToPtr(const void* base, unsigned long offset);

template <>
WSParser::TCtrlGroup*
JSUConvertOffsetToPtr<WSParser::TCtrlGroup>(const void* base,
                                            unsigned long offset);
template <>
WSParser::TCtrlScene*
JSUConvertOffsetToPtr<WSParser::TCtrlScene>(const void* base,
                                            unsigned long offset);
template <>
WSParser::TCtrl* JSUConvertOffsetToPtr<WSParser::TCtrl>(const void* base,
                                                        unsigned long offset);
template <>
WSParser::TWaveArchiveBank*
JSUConvertOffsetToPtr<WSParser::TWaveArchiveBank>(const void* base,
                                                  unsigned long offset);
template <>
WSParser::TWaveArchive*
JSUConvertOffsetToPtr<WSParser::TWaveArchive>(const void* base,
                                              unsigned long offset);
template <>
WSParser::TCtrlWave*
JSUConvertOffsetToPtr<WSParser::TCtrlWave>(const void* base,
                                           unsigned long offset);
template <>
WSParser::TWave* JSUConvertOffsetToPtr<WSParser::TWave>(const void* base,
                                                        unsigned long offset);

} // namespace JASystem

#endif // JASWSPARSER_HPP
