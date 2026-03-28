#ifndef GC2D_SUN_GLASS_HPP
#define GC2D_SUN_GLASS_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TMarioGamePad;

class TSunGlass : public JDrama::TViewObj {
public:
	TSunGlass(JUtility::TColor param_1, const char* name = "<SunGlass>")
	    : JDrama::TViewObj(name)
	    , unk10(nullptr)
	    , unk14(param_1)
	    , unk18(0)
	    , unk19(3)
	    , unk1A(0)
	    , unk1E(60)
	    , unk1F(0)
	    , unk20(100)
	    , unk22(100)
	    , unk24(0)
	    , unk26(0)
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);

	void getShineAlpha();
	void draw(const JDrama::TRect&, JUtility::TColor);
	void changeAlpha(u8*);
	void startFade(int, bool);

public:
	/* 0x10 */ TMarioGamePad* unk10;
	/* 0x14 */ JUtility::TColor unk14;
	/* 0x18 */ u8 unk18;
	/* 0x19 */ u8 unk19;
	/* 0x1A */ u16 unk1A;
	/* 0x1C */ u8 unk1C;
	/* 0x1D */ u8 unk1D;
	/* 0x1E */ u8 unk1E;
	/* 0x1F */ u8 unk1F;
	/* 0x20 */ u16 unk20;
	/* 0x22 */ u16 unk22;
	/* 0x24 */ u16 unk24;
	/* 0x26 */ u8 unk26;
	/* 0x27 */ u8 unk27;
	/* 0x28 */ u8 unk28;
};

class TSunShine : public TSunGlass {
public:
	TSunShine(const char* name = "<SunShine>")
	    : TSunGlass(JUtility::TColor(135, 135, 135, 0), name)
	{
		unk14 = JUtility::TColor(135, 135, 135, 0);
		unk1A = 0;
		unk28 = 0;
	}

	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
};

#endif
