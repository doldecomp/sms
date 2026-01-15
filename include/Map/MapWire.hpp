#ifndef MAP_MAP_WIRE_HPP
#define MAP_MAP_WIRE_HPP

#include <JSystem/JGeometry/JGVec3.hpp>

class TCubeGeneralInfo;

class TMapWirePoint {
public:
	TMapWirePoint();
};

class TMapWire {
public:
	void drawLower() const;
	void drawUpper() const;
	void getPointPowerAtReleased(f32) const;
	void getPointPosAtReleased(f32, JGeometry::TVec3<f32>*) const;
	void updatePointAtReleased(int);
	void updateMovePointAtReleased();
	void initPointAtJustReleased(f32, TMapWirePoint*);
	void release();
	void getPointPosAtHanged(f32, JGeometry::TVec3<f32>*) const;
	void getPointInfoAtHanged(f32, TMapWirePoint*);
	void setFootPointsAtHanged(MtxPtr);
	void calcViewAndDBEntry();
	void move();
	f32 getPosInWire(const JGeometry::TVec3<f32>&) const;
	void getPointPosOnLine(f32, JGeometry::TVec3<f32>*) const;
	void getPointPosOnWire(f32, JGeometry::TVec3<f32>*) const;
	void getPointPosDefault(f32, JGeometry::TVec3<f32>*) const;
	void initTipPoints(const TCubeGeneralInfo*);
	void init(const TCubeGeneralInfo*);
	TMapWire();

	static f32 mDrawWidth;
	static f32 mDrawHeight;

public:
	/* 0x00 */ JGeometry::TVec3<f32> unk00; // start point???
	/* 0x0C */ JGeometry::TVec3<f32> unk0C; // end point???
	/* 0x18 */ JGeometry::TVec3<f32> unk18;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ f32 unk30;
	/* 0x34 */ f32 unk34;
	/* 0x38 */ f32 unk38;
	/* 0x3C */ void** unk3C; // Unknown type for now
	/* 0x40 */ void** unk40; // Unknown type for now
	/* 0x44 */ u16 unk44;
	/* 0x46 */ u16 unk46;
	/* 0x48 */ u32 unk48;
	/* 0x4C */ f32 unk4C;
	/* 0x50 */ f32 unk50;
	/* 0x54 */ f32 unk54;
	/* 0x58 */ f32 unk58;
	/* 0x5C */ f32 unk5C;
	/* 0x60 */ f32 unk60;
	/* 0x64 */ f32 unk64;
	/* 0x68 */ f32 unk68;
	/* 0x6C */ f32 unk6C;
	/* 0x70 */ f32 unk70;
	/* 0x74 */ f32 unk74;
	/* 0x78 */ f32 unk78;
	/* 0x7C */ u16 unk7C;
};

#endif
