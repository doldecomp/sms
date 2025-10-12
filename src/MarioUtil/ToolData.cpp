#include <MarioUtil/ToolData.hpp>
#include <macros.h>

namespace Koga {
ToolData::ToolData() { mData = nullptr; }
ToolData::~ToolData() { }

BOOL ToolData::Attach(const void* jmapData)
{
	if (!jmapData)
		return FALSE;
	this->mData = (JMapData*)jmapData;
	return TRUE;
}

BOOL ToolData::GetValue(int entryIndex, const char* key, long& pValueOut) const
{
	s32 itemIndex = searchItemInfo(key);
	if (itemIndex < 0) {
		return FALSE;
	}
	return getValue(entryIndex, itemIndex, &pValueOut);
}

BOOL ToolData::GetValue(int entryIndex, const char* key,
                        const char*& pValueOut) const
{
	s32 itemIndex = searchItemInfo(key);
	if (itemIndex < 0) {
		return FALSE;
	}
	return getValue(entryIndex, itemIndex, &pValueOut);
}
} // namespace Koga
