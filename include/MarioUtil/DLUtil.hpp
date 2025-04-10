#ifndef MARIO_UTIL_DL_UTIL_HPP
#define MARIO_UTIL_DL_UTIL_HPP

#include <JSystem/JGeometry.hpp>
#include <dolphin/gx/GXStruct.h>

class TDLTexQuad {
public:
	void initValue(u16);

	virtual void createDLBuffer(u16);
	virtual void createBuffer(u16);
	virtual void reset();
	virtual void request(const JGeometry::TVec3<f32>*);
	virtual void setEnd();

	void createPosArrayBuffer(u16);
	void draw();

public:
	/* 0x0 */ char unk0[0x1C];
};

class TDLTexQuadMulti {
public:
	void reset();
	void setChangeNum(u16);
	void createDLBuffer(u16);
	void request(const JGeometry::TVec3<f32>*);
	void setEnd();
	void draw(int);
};

class TDLColorTexQuad {
public:
	void createColorArrayBuffer(u16);
	void createBuffer(u16);
	void requestCol(const JGeometry::TVec3<f32>*, GXColor, int);
	void setEnd();
	void draw();
};

#endif
