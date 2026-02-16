#ifndef MARIO_UTIL_TOOL_DATA_HPP
#define MARIO_UTIL_TOOL_DATA_HPP

#include <dolphin/types.h>

namespace Koga {

class ToolData {
public:
	ToolData();

	virtual ~ToolData();

	bool Attach(const void*);
	void Detach();
	static u32 Hash(const char*);

	bool GetValue(int, const char*, u32&) const;
	bool GetValue(int, const char*, s32&) const;
	bool GetValue(int, const char*, bool&) const;
	bool GetValue(int, const char*, const char*&) const;
	bool GetValue(int, const char*, f32&) const;

	int SearchItemInfo(const char*) const;

	void FindElement(int, u32, int) const;
	void FindElement(int, s32, int) const;
	void FindElement(int, bool, int) const;
	void FindElement(int, const char*, int) const;
	void FindElement(int, f32, int) const;

	const void* GetData() const { return unk4; }
	int GetGroupCount() const { return unk4->unk0; }

public:
	struct ItemInfo {
		u16 getUnk8() const { return unk8; }
		u32 getUnk0() const { return unk0; }

		/* 0x0 */ u32 unk0;
		/* 0x4 */ char unk4[0x4];
		/* 0x8 */ u16 unk8;
	};

	struct Header {
		/* 0x0 */ int unk0;
		/* 0x4 */ int unk4;
		/* 0x8 */ u32 unk8;
		/* 0xC */ u32 unkC;
		/* 0x10 */ ItemInfo unk10[];
	};

	/* 0x4 */ const Header* unk4;
};

} // namespace Koga

#endif
