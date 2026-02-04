#ifndef GC2D_SUN_GLASS_HPP
#define GC2D_SUN_GLASS_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

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
