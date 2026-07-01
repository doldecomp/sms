#ifndef GC2D_SELECT_MENU_HPP
#define GC2D_SELECT_MENU_HPP

#include <dolphin/types.h>

class JKRArchive;
class TSelectDir;
class TSelectShineManager;
class TMarioGamePad;

class TSelectMenu {
public:
	TSelectMenu(const char*);
	void initData(u8, JKRArchive*, TSelectShineManager*, TSelectDir*);
	void startMove();
	void startOpenWindow();

public:
	/* 0x000 */ u8 unk0[0x100];
	/* 0x100 */ TMarioGamePad* unk100;
	/* 0x104 */ u8 unk104[0x37];
	/* 0x13B */ u8 unk13B;
	/* 0x13C */ u8 unk13C[0xE];
	/* 0x14A */ bool unk14A;
	/* 0x14B */ bool unk14B;
	/* 0x14C */ u8 unk14C[0x24];
};

class TSelectGrad {
public:
	TSelectGrad(const char*);
	void setStageColor(u8);

private:
	/* 0x00 */ u8 unk0[0x24];
};

class TSelectShineManager {
public:
	TSelectShineManager(const char*);

private:
	/* 0x000 */ u8 unk0[0x120];
};

#endif
