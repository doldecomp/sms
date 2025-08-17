#ifndef GC2D_MENU_HPP
#define GC2D_MENU_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TMarioGamePad;
class J2DScreen;
class J2DPane;

class TMenuBase : public JDrama::TViewObj {
public:
	void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ J2DScreen* unk10;
};

class TMenuPlane : public JDrama::TViewObj {
public:
	TMenuPlane(const TMarioGamePad*, J2DPane*, u32, u32);
	void perform(u32, JDrama::TGraphics*);
};

class TFlashPane : public JDrama::TViewObj {
public:
	void perform(u32, JDrama::TGraphics*);
};

#endif
