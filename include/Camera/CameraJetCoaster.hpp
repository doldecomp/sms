#ifndef CAMERA_CAMERA_JET_COASTER_HPP
#define CAMERA_CAMERA_JET_COASTER_HPP

#include <JSystem/JGeometry/JGVec3.hpp>

class TCamSaveJetCoaster;

class TCameraJetCoaster {
public:
	TCameraJetCoaster();
	void calcNowOffsetAngle(f32, f32);

	// Fabricated
	void toggleLButtonMode() { mLButtonMode ^= 1; }
	bool isLButtonMode() { return mLButtonMode & 1; }

public:
	/* 0x0 */ TCamSaveJetCoaster* unk0;
	/* 0x4 */ s16 unk4;
	/* 0x6 */ s16 unk6;
	/* 0x8 */ s16 unk8;
	/* 0xA */ s16 unkA;
	/* 0xC */ u8 mLButtonMode;
	/* 0x10 */ JGeometry::TVec3<f32> unk10;
	/* 0x1C */ JGeometry::TVec3<f32> unk1C;
	/* 0x28 */ JGeometry::TVec3<f32> unk28;
	/* 0x34 */ f32 unk34;
	/* 0x38 */ u16 unk38;
};

#endif
