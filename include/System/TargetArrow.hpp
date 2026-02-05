#ifndef SYSTEM_TARGET_ARROW_HPP
#define SYSTEM_TARGET_ARROW_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class MActor;
class TTargetArrow;

extern TTargetArrow* gpTargetArrow;

class TTargetArrow : public JDrama::TViewObj {
public:
	TTargetArrow() { }

	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	void setPos(const JGeometry::TVec3<f32>&);

public:
	/* 0x10 */ MActor* unk10;
	/* 0x14 */ u8 unk14;
};

#endif
