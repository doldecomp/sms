#ifndef CAMERA_CAMERA_MARIO_DATA_HPP
#define CAMERA_CAMERA_MARIO_DATA_HPP

#include <JSystem/JGeometry/JGVec3.hpp>
#include <dolphin/mtx.h>

class TCameraMarioData {
public:
	TCameraMarioData();
	void calcAndSetMarioData();
	bool isMarioGoDown() const;
	bool isMarioRocketing() const;
	bool isMarioIndoor() const;
	bool isMarioSlider() const;
	bool isMarioLeanMirror() const;
	bool isMarioBathtub() const;
	bool isMarioDoorDemoStart() const;
	bool isMarioDoorDemoEnd() const;
	bool isMarioClimb(u32) const;

public:
	/* 0x0 */ JGeometry::TVec3<f32> unk0;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ f32 unk1C;
};

extern TCameraMarioData* gpCameraMario;

#endif
