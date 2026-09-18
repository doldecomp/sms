#ifndef MAP_SKY_HPP
#define MAP_SKY_HPP

#include <JSystem/JDrama/JDRActor.hpp>

class MActor;

class TSky : public JDrama::TActor {
public:
	TSky(const char* name = "空");
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	// Fabricated
	MActor* getMActor() { return unk44; }
	f32 getUnk48() const { return unk48; }
	f32 getUnk4C() const { return unk4C; }

public:
	/* 0x44 */ MActor* unk44;
	/* 0x48 */ f32 unk48;
	/* 0x4C */ f32 unk4C;
};

#endif
