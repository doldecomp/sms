#ifndef GC2D_MENU_HPP
#define GC2D_MENU_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TMarioGamePad;
class J2DScreen;
class J2DPane;
class J2DTextBox;

class TMenuBase : public JDrama::TViewObj {
public:
	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ J2DScreen* unk10;
};

class TMenuPlane : public JDrama::TViewObj {
public:
	TMenuPlane(const TMarioGamePad*, J2DPane*, u32, u32);

	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ const TMarioGamePad* unk10;
	/* 0x14 */ J2DPane* unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ JUtility::TColor unk1C;
	/* 0x20 */ JUtility::TColor unk20;
	/* 0x24 */ JUtility::TColor unk24;
	/* 0x28 */ int unk28;
	/* 0x2C */ int unk2C;
	/* 0x30 */ J2DTextBox** unk30;
	/* 0x34 */ u32 unk34;
	/* 0x38 */ u32 unk38;
	/* 0x3C */ u8 unk3C;
};

class TFlashPane : public JDrama::TViewObj {
public:
	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ J2DPane* unk10;
	/* 0x14 */ int unk14;
	/* 0x18 */ int unk18;
};

#endif
