#ifndef MARIO_UTIL_DL_UTIL_HPP
#define MARIO_UTIL_DL_UTIL_HPP

#include <JSystem/JGeometry.hpp>
#include <dolphin/gx/GXStruct.h>

class TDLTexQuad {
public:
	void initValue(u16);

	virtual void createDLBuffer(u16);
	virtual void createBuffer(u16);
	virtual void reset()
	{
		unk8 = 0;
		unk4 = 1 - unk4;
	}
	virtual bool request(const JGeometry::TVec3<f32>*);
	virtual void setEnd();

	void createPosArrayBuffer(u16);
	void draw();

public:
	/* 0x4 */ u16 unk4;
	/* 0x6 */ u16 unk6;
	/* 0x8 */ u16 unk8;
	/* 0xA */ u16 unkA;

#pragma pack(push, 1)
	struct DL {
		u8 primitive;
		u16 vtxCount;
		u8 indices[];
	};
#pragma pack(pop)

	/* 0xC */ u8* unkC[2];
	/* 0x14 */ f32* unk14[2];
	/* 0x1C */ u32 unk1C;
};

class TDLTexQuadMulti : public TDLTexQuad {
public:
	void reset() { }
	void setChangeNum(u16);
	void createDLBuffer(u16);
	bool request(const JGeometry::TVec3<f32>*);
	void setEnd();
	void draw(int);
};

class TDLColorTexQuad : public TDLTexQuad {
public:
	void createColorArrayBuffer(u16);
	virtual void createBuffer(u16);
	bool requestCol(const JGeometry::TVec3<f32>*, GXColor, int);
	virtual void setEnd();
	virtual void draw();

public:
	/* 0x20 */ GXColor* unk20[2];
};

#endif
