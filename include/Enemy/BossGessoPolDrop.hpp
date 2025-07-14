#ifndef ENEMY_BOSS_GESSO_POL_DROP_HPP
#define ENEMY_BOSS_GESSO_POL_DROP_HPP

#include <JSystem/JDrama/JDRActor.hpp>

class MActor;

class TBGPolDrop : public JDrama::TActor {
public:
	TBGPolDrop(const char* name = "<TBGPolDrop>");

	virtual void perform(u32, JDrama::TGraphics*);

	void move();
	void launch(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&);

public:
	/* 0x44 */ JGeometry::TVec3<f32> unk44;
	/* 0x50 */ MActor* unk50;
	/* 0x54 */ MActor* unk54;
	/* 0x58 */ u32 unk58;
};

#endif
