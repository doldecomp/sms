#include <JSystem/JUtility/JUTNameTab.hpp>
#include <string.h>

void JUTNameTab::setResource(const ResNTAB* pNameTable)
{
	mNameTable = pNameTable;
	mStrData   = (const char*)(pNameTable->mEntries + pNameTable->mEntryNum);
	mNameNum   = pNameTable->mEntryNum;
}

JUTNameTab::JUTNameTab() { setResource(nullptr); }

JUTNameTab::JUTNameTab(const ResNTAB* pNameTable) { setResource(pNameTable); }

s32 JUTNameTab::getIndex(const char* pName) const
{
	const ResNTAB::Entry* pEntry = mNameTable->mEntries;
	u16 keyCode                  = calcKeyCode(pName);

	for (u16 i = 0; i < mNameNum; pEntry++, i++) {
		if (pEntry->mKeyCode != keyCode)
			continue;

		s32 idx = i;

		if (!strcmp(mNameTable->mEntries[idx].mOffs + (char*)mNameTable, pName))
			return idx;
	}

	return -1;
}

const char* JUTNameTab::getName(u16 index) const
{
	if (index < mNameNum)
		return mNameTable->getName(index);
	return nullptr;
}

u16 JUTNameTab::calcKeyCode(const char* pName) const
{
	u32 keyCode = 0;
	while (*pName)
		keyCode = (keyCode * 3) + *pName++;
	return keyCode;
}
