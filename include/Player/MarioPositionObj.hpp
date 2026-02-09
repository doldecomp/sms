#ifndef PLAYER_MARIO_POSITION_OBJ_HPP
#define PLAYER_MARIO_POSITION_OBJ_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TMarioPositionObj : public JDrama::TViewObj {
public:
	TMarioPositionObj(const char* name = "<MarioPositionObj>")
	    : JDrama::TViewObj(name)
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(unsigned long, JDrama::TGraphics*);

	const JGeometry::TVec3<f32>& getUnk10(u8 idx) const { return unk10[idx]; }
	const JGeometry::TVec3<f32>& getUnk70(u8 idx) const { return unk70[idx]; }

public:
	/* 0x10 */ JGeometry::TVec3<f32> unk10[8];
	/* 0x70 */ JGeometry::TVec3<f32> unk70[8];
	/* 0xD0 */ u32 unkD0;
};

#endif
