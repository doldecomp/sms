#ifndef PLAYER_MARIO_EFFECT_HPP
#define PLAYER_MARIO_EFFECT_HPP

#include <Strategic/HitActor.hpp>

class TMario;
class MActor;

class TMarioEffect : public THitActor {
public:
	TMarioEffect(const char* name = "マリオエフェクト")
	    : THitActor(name)
	{
	}

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void init(TMario*);
	void setJumpIntoWaterEffect();
	void setJumpIntoWaterEffectSmall();
	int getJumpIntoWaterModelData();
	void startDashEffect();
	void endDashEffect();

	int getThing()
	{
		int idx = -1;
		if (unk6C[0] == 0)
			idx = 0;
		if (unk6C[1] == 0)
			idx = 1;
		return idx;
	}

public:
	/* 0x68 */ TMario* unk68;
	/* 0x6C */ int unk6C[2];
	/* 0x74 */ MActor* unk74[2];
	/* 0x7C */ int unk7C;
	/* 0x80 */ MActor* unk80;
};

#endif
