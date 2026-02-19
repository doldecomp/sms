#ifndef GC2D_MENU_HPP
#define GC2D_MENU_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/J2D/J2DPane.hpp>

class TMarioGamePad;
class J2DScreen;
class J2DPane;
class J2DTextBox;

class TMenuBase : public JDrama::TViewObj {
public:
	TMenuBase(J2DScreen* screen, const char* name = "<TMenuBase>")
	    : JDrama::TViewObj(name)
	    , unk10(screen)
	{
	}

	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ J2DScreen* unk10;
};

class TMenuPlane : public JDrama::TViewObj {
public:
	TMenuPlane(const TMarioGamePad*, J2DPane*, u32, u32);

	virtual void perform(u32, JDrama::TGraphics*);

	void show()
	{
		unk3C = 10;
		unk14->show();
		unkC.off(1);
		unk18 = 0;
	}

	void hide()
	{
		unk3C = 10;
		unk14->hide();
		unkC.on(0x1);
		unk18 = 0;
	}

	void fade()
	{
		unk18 |= 0x4;
		unk14->mAlpha = 160;
	}

	void unfade()
	{
		unk18         = 0;
		unk14->mAlpha = 255;
	}

	bool checkFlag(u32 flag) const { return unk18 & flag ? true : false; }

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
	TFlashPane(J2DPane* param_1, const char* name = "<TFlashPane>")
	    : JDrama::TViewObj(name)
	    , unk10(param_1)
	    , unk14(0)
	    , unk18(4)
	{
	}

	virtual void perform(u32, JDrama::TGraphics*);

	void hide()
	{
		unk10->hide();
		unkC.on(0x1);
	}

public:
	/* 0x10 */ J2DPane* unk10;
	/* 0x14 */ int unk14;
	/* 0x18 */ int unk18;
};

#endif
