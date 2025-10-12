#ifndef MARIO_UTIL_TOOL_DATA_HPP
#define MARIO_UTIL_TOOL_DATA_HPP
#include <types.h>

namespace Koga {
/**
 * @brief ToolData is a class that reads BCSV data. Usage is as follows:
 * Create a ToolData instance, fetch the BCSV data from disk and attach it.
 * One can then call GetValue to retrieve data from the file.
 *
 * The structures and parsing logic are based off the Mario Galaxy decomp's
 * handling of JMap files
 */
class ToolData {
public:
	ToolData();
	virtual ~ToolData(); // if not explicitly defined its not generated ok
	BOOL Attach(const void* bcsvFileData);
	// returns TRUE if the value was found, otherwise FALSE
	BOOL GetValue(int entryIndex, const char* key, long& pValueOut) const;
	// returns TRUE if the value was found, otherwise FALSE
	BOOL GetValue(int entryIndex, const char* key,
	              const char*& pValueOut) const;

	inline bool dataExists() const { return !!mData; }

private:
// JMAP value types, ignored in sunshine
#define JMAP_VALUE_TYPE_LONG       0
#define JMAP_VALUE_TYPE_STRING     1
#define JMAP_VALUE_TYPE_FLOAT      2
#define JMAP_VALUE_TYPE_LONG_2     3
#define JMAP_VALUE_TYPE_SHORT      4
#define JMAP_VALUE_TYPE_BYTE       5
#define JMAP_VALUE_TYPE_STRING_PTR 6
#define JMAP_VALUE_TYPE_NULL       7

	struct JMapItem {
		u32 mHash;     // 0x0
		u32 mMask;     // 0x4
		u16 mOffsData; // 0x8
		u8 mShift;     // 0xA
		u8 mType;      // 0xB
	};

	struct JMapData {
		s32 mNumEntries;         // 0x0
		s32 mNumFields;          // 0x4
		s32 mDataOffset;         // 0x8
		u32 mEntrySize;          // 0xC
		const JMapItem mItems[]; // 0x10
	};

	inline u32 hashString(const char* key) const
	{
		u32 stringHash = 0;
		char current_char;

		while ((current_char = *key) != 0) {
			key++;
			stringHash = (current_char + (stringHash << 8)) % 0x1FFFFD9;
		}
		return stringHash;
	}

	inline s32 searchItemInfo(const char* pKey) const
	{
		s32 nFields = mData->mNumFields;
		u32 hash    = hashString(pKey);

		for (int i = 0; i < nFields; ++i) {
			if (hash == mData->mItems[i].mHash) {
				return i;
			}
		}

		return -1;
	}

	inline const char* getEntryAddress(const JMapData* pData,
	                                   const s32 dataOffset,
	                                   const int entryIndex) const
	{
		return reinterpret_cast<const char*>(pData) + dataOffset
		       + entryIndex * pData->mEntrySize;
	}

	inline BOOL getValue(const int entryIndex, const int itemIndex,
	                     s32* pValueOut) const
	{
		const JMapItem* item = &mData->mItems[itemIndex];
		const char* valuePtr
		    = getEntryAddress(mData, mData->mDataOffset, entryIndex)
		      + item->mOffsData;
		*pValueOut = *reinterpret_cast<const s32*>(valuePtr);
		return TRUE;
	}

	inline BOOL getValue(const int entryIndex, const int itemIndex,
	                     const char** pValueOut) const
	{
		const JMapItem* item = &mData->mItems[itemIndex];
		*pValueOut = getEntryAddress(mData, mData->mDataOffset, entryIndex)
		             + item->mOffsData;
		return TRUE;
	}

public:
	const JMapData* mData;
};
} // namespace Koga
#endif
