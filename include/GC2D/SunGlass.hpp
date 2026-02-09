#ifndef GC2D_SUN_GLASS_HPP
#define GC2D_SUN_GLASS_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TMarioGamePad;

class TSunGlass : public JDrama::TViewObj {
public:
	TSunGlass(JUtility::TColor, const char* name = "<SunGlass>");

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
};

class TSunShine : public TSunGlass {
public:
	TSunShine(const char* name = "<SunShine>")
	    : TSunGlass(JUtility::TColor(135, 135, 135, 0), name)
	{
	}

	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
};

#endif
