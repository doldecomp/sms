#ifndef NPC_INBETWEEN_HPP
#define NPC_INBETWEEN_HPP

#include <JSystem/JGeometry/JGVec3.hpp>

class MActor;

class TNpcInbetween {
public:
	TNpcInbetween(int pos_inbetween_frame, int motion_blend_frame)
	    : unk0(pos_inbetween_frame)
	    , unk4(motion_blend_frame)
	    , unk8(0)
	    , unkC(0.0f, 0.0f, 0.0f)
	    , unk18(0.0f, 0.0f, 0.0f)
	    , unk24(0)
	    , unk28(0.0f)
	{
	}

	void execMotionBlend(MActor*);
	void execPosInbetween(JGeometry::TVec3<f32>*);

	// fabricated
	bool isThing() const
	{
		if (unk24 > 0)
			return true;
		else
			return false;
	}

	bool isOtherThing() const
	{
		if (unk28 > 0.0f)
			return true;
		else
			return false;
	}

	void reset()
	{
		unk24 = 0;
		unk28 = 0.0f;
	}

public:
	/* 0x0 */ int unk0;
	/* 0x4 */ int unk4;
	/* 0x8 */ int unk8;
	/* 0xC */ JGeometry::TVec3<f32> unkC;
	/* 0x18 */ JGeometry::TVec3<f32> unk18;
	/* 0x24 */ int unk24;
	/* 0x28 */ f32 unk28;
};

#endif
