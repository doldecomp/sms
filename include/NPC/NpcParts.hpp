#ifndef NPC_PARTS_HPP
#define NPC_PARTS_HPP

#include <JSystem/JDrama/JDRGraphics.hpp>

class J3DGXColorS10;
class TBaseNPC;
class MActor;
class TSharedParts;

class TNpcParts {
public:
	TNpcParts(u32, const J3DGXColorS10*, TBaseNPC*);

	void addJellyFishParts(f32);
	void setPartsAnmFrame(f32);
	MActor* getPartsMActor(int, int);
	void partsFrameUpdate();
	void partsPerform(u32, JDrama::TGraphics*);

public:
	/* 0x0 */ TSharedParts* unk0[2][12];
	/* 0x60 */ TBaseNPC* unk60;
};

#endif
