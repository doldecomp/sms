#ifndef ENEMY_BOSS_GESSO_POL_DROP_HPP
#define ENEMY_BOSS_GESSO_POL_DROP_HPP

#include <JSystem/JDrama/JDRActor.hpp>

class MActor;

class TBGPolDrop : public JDrama::TActor {
public:
	TBGPolDrop(const char* name = "<TBGPolDrop>");

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void move();
	void launch(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&);

	// fabricated
	void setMActors(MActor* param_1, MActor* param_2)
	{
		unk50 = param_1;
		unk54 = param_2;
	}
	int getUnk58() const { return unk58; }
	// The accessor level is load-bearing in move(): with the raw member the
	// frame is 8 bytes short and every local sits 4 low.
	const JGeometry::TVec3<f32>& getVelocity() const { return unk44; }

public:
	/* 0x44 */ JGeometry::TVec3<f32> unk44;
	/* 0x50 */ MActor* unk50;
	/* 0x54 */ MActor* unk54;
	/* 0x58 */ int unk58;
};

#endif
