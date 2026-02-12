#ifndef SYSTEM_MENU_DIR_HPP
#define SYSTEM_MENU_DIR_HPP

#include <JSystem/JDrama/JDRDisplay.hpp>
#include <JSystem/JDrama/JDRDirector.hpp>

class TMarioGamePad;
class J2DPane;
class J2DTextBox;
class TFlashPane;
class J2DSetScreen;
class TMenuPlane;

class TMenuDirector : public JDrama::TDirector {
public:
	TMenuDirector();
	~TMenuDirector();
	static void* setupThreadFunc(void*);
	void setup(JDrama::TDisplay*, TMarioGamePad*);
	int rsetup();
	int direct();
	void setFixedStageValue();

public:
	/* 0x18 */ u8 unk18;
	/* 0x1C */ void* unk1C;
	/* 0x20 */ J2DPane* unk20;
	/* 0x24 */ TFlashPane* unk24;
	/* 0x28 */ u32 unk28;
	/* 0x2C */ TMarioGamePad* unk2C;
	/* 0x30 */ u32 unk30;
	/* 0x34 */ f32 unk34;
	/* 0x38 */ J2DTextBox* unk38;
	/* 0x3C */ J2DSetScreen* unk3C;
	/* 0x40 */ TMenuPlane* unk40;
	/* 0x44 */ TMenuPlane* unk44;
	/* 0x48 */ u32 unk48;
	/* 0x4C */ u32 unk4C;
	/* 0x50 */ bool unk50;
};

#endif
