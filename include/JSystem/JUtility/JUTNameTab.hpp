#ifndef JUT_NAMETAB_HPP
#define JUT_NAMETAB_HPP

#include <types.h>

struct ResNTAB {
	u16 mEntryNum;
	u16 mPad0;
	struct Entry {
		u16 mKeyCode;
		u16 mOffs;
	} mEntries[1];

	inline const char* getName(u32 index) const
	{
		return ((const char*)mEntries) + mEntries[index].mOffs - 4;
	}
};

class JUTNameTab {
public:
	JUTNameTab();
	JUTNameTab(const ResNTAB* pNameTable);
	~JUTNameTab() { }

	void setResource(const ResNTAB* pNameTable);
	s32 getIndex(char const*) const;
	const char* getName(u16 index) const;
	u16 calcKeyCode(char const* pName) const;
	const ResNTAB* getResNameTable() const { return mNameTable; }

private:
	const ResNTAB* mNameTable;
	const char* mStrData;
	u16 mNameNum;
};

#endif /* JUTNAMETAB_H */
