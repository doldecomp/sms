#ifndef CAMERA_CAMERA_OPTION_HPP
#define CAMERA_CAMERA_OPTION_HPP

#include <JSystem/JGeometry/JGVec3.hpp>

class TCameraOption {
public:
	TCameraOption(JGeometry::TVec3<f32>, JGeometry::TVec3<f32>*);

	void moveToLoadFromTitle();
	void moveToTitleFromLoad();
	void moveToUp();
	void moveToDown();

public:
	/* 0x0 */ u8 unk0;
	/* 0x4 */ f32 mFovYunk4;
	/* 0x8 */ s16 unk8;
	/* 0xA */ s16 unkA;
	/* 0xC */ s16 unkC;
	/* 0xE */ s16 unkE;
	/* 0x10 */ s16 unk10;
	/* 0x12 */ s16 unk12;
	/* 0x14 */ s16 unk14;
	/* 0x16 */ s16 unk16;
	/* 0x18 */ JGeometry::TVec3<f32> unk18;
	/* 0x24 */ JGeometry::TVec3<f32> unk24;
	/* 0x30 */ JGeometry::TVec3<f32> unk30;
	/* 0x3C */ JGeometry::TVec3<f32>* unk3C;
};

extern TCameraOption* gpCameraOption;

#endif
