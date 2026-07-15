#ifndef GC2D_SELECT_MENU_HPP
#define GC2D_SELECT_MENU_HPP

#include <dolphin/types.h>
#include <JSystem/JDrama/JDRViewObj.hpp>

class JKRArchive;
class TSelectDir;
class TSelectShineManager;
class TMarioGamePad;

class TSelectMenu : public JDrama::TViewObj {
public:
	TSelectMenu(const char*);
	virtual ~TSelectMenu();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void initData(u8, JKRArchive*, TSelectShineManager*, TSelectDir*);
	void startMove();
	void startOpenWindow();

public:
	/* 0x010 */ u8 unk10[0xF0];
	/* 0x100 */ TMarioGamePad* unk100;
	/* 0x104 */ u8 unk104[0x37];
	/* 0x13B */ u8 unk13B;
	/* 0x13C */ u8 unk13C[0xE];
	/* 0x14A */ bool unk14A;
	/* 0x14B */ bool unk14B;
	/* 0x14C */ u8 unk14C[0x24];
};

class TSelectGrad : public JDrama::TViewObj {
public:
	TSelectGrad(const char*);
	virtual ~TSelectGrad();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void setStageColor(u8);

private:
	/* 0x10 */ u8 unk10[0x14];
};

class TSelectShineManager : public JDrama::TViewObj {
public:
	TSelectShineManager(const char*);
	virtual ~TSelectShineManager();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

private:
	/* 0x010 */ u8 unk10[0x110];
};

#endif
