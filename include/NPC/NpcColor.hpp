#ifndef NPC_NPC_COLOR_HPP
#define NPC_NPC_COLOR_HPP

#include <dolphin/gx/GXStruct.h>
class MActor;

// Note: This is used in parts of the NPC::sAllNpcInitData$1622 - Azack
struct TColorChangeInfo {
public:
	/* 0x0 */ u32 unk0;               // some enum?
	/* 0x4 */ const char* unk4;       // material name to update?
	/* 0x8 */ const GXColorS10* unk8; // Array of colors?
	/* 0xC */ const GXColorS10* unkC; // Another array of colors?
};

void SMS_InitChangeNpcColor(const MActor*, const TColorChangeInfo*, s16 param3,
                            const GXColor*);

#endif
