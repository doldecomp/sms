#ifndef CAMERA_CAMERA_INBETWEEN_HPP
#define CAMERA_CAMERA_INBETWEEN_HPP

#include <JSystem/JGeometry/JGVec3.hpp>
#include <dolphin/types.h>

class TCameraInbetween {
public:
	TCameraInbetween();
	void execCameraInbetween(const JGeometry::TVec3<f32>&,
	                         const JGeometry::TVec3<f32>&,
	                         const JGeometry::TVec3<f32>&);
	void initCameraInbetween(const JGeometry::TVec3<f32>&,
	                         const JGeometry::TVec3<f32>&,
	                         const JGeometry::TVec3<f32>&);
	void startCameraInbetween(int);
	void addMoveCameraAndMario(const Vec&);
	void warpPosAndAt(const Vec&, const Vec&);

public:
	/* 0x0 */ u32 unk0;
	/* 0x4 */ s32 unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ s16 unkC;
	/* 0xE */ s16 unkE;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ s16 unk14;
	/* 0x16 */ s16 unk16;
	/* 0x18 */ JGeometry::TVec3<f32> unk18;
	/* 0x24 */ JGeometry::TVec3<f32> unk24;
	/* 0x30 */ JGeometry::TVec3<f32> unk30;
	/* 0x3C */ u32 unk3C;
	/* 0x40 */ u32 unk40;
	/* 0x44 */ f32 unk44;
};

#endif
