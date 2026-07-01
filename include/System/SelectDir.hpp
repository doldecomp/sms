#ifndef SYSTEM_SELECT_DIR_HPP
#define SYSTEM_SELECT_DIR_HPP

#include <JSystem/JDrama/JDRDirector.hpp>

namespace JDrama {
class TDisplay;
};

class JKRArchive;
class JPAEmitterManager;
class TSelectGrad;
class TSelectMenu;
class TSelectShineManager;
class TMarioGamePad;

class TSelectDir : public JDrama::TDirector {
public:
	TSelectDir();
	virtual ~TSelectDir();

	virtual int direct();

	void setup(JDrama::TDisplay*, TMarioGamePad*, unsigned char);
	static void* setupThreadFunc(void*);
	int rsetup();
	void changeOrder();

public:
	/* 0x18 */ TMarioGamePad* unk18;
	/* 0x1C */ JDrama::TDisplay* unk1C;
	/* 0x20 */ TSelectMenu* unk20;
	/* 0x24 */ TSelectGrad* unk24;
	/* 0x28 */ TSelectShineManager* unk28;
	/* 0x2C */ JKRArchive* unk2C;
	/* 0x30 */ JPAEmitterManager* unk30;
	/* 0x34 */ JPAEmitterManager* unk34;
	/* 0x38 */ bool unk38;
	/* 0x3C */ void* unk3C;
	/* 0x40 */ u8 unk40;
	/* 0x44 */ JDrama::TViewObj* unk44;
	/* 0x48 */ JDrama::TViewObj* unk48;
	/* 0x4C */ bool unk4C;
};

#endif
