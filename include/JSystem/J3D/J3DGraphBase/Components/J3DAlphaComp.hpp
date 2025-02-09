#ifndef J3D_GRAPH_BASE_COMPONENTS_ALPHACOMP
#define J3D_GRAPH_BASE_COMPONENTS_ALPHACOMP

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>
#include <dolphin/gd.h>

extern const u16 j3dDefaultAlphaCmpID;
extern u8 j3dAlphaCmpTable[768];

inline u16 calcAlphaCmpID(u8 comp0, u8 op, u8 comp1)
{
	return (comp0 << 5) + (op << 3) + comp1;
}

class J3DAlphaComp {
public:
	J3DAlphaComp()
	{
		mAlphaCmpID = j3dDefaultAlphaCmpID;
		mRef0       = 0;
		mRef1       = 0;
	}

	J3DAlphaComp(u16 id)
	    : mAlphaCmpID(id)
	    , mRef0(0)
	    , mRef1(0)
	{
	}

	J3DAlphaComp& operator=(const J3DAlphaComp& other)
	{
		mAlphaCmpID = other.mAlphaCmpID;
		mRef0       = other.mRef0;
		mRef1       = other.mRef1;
		return *this;
	}

	void setAlphaCompInfo(const J3DAlphaCompInfo& info)
	{
		mRef0       = info.mRef0;
		mRef1       = info.mRef1;
		mAlphaCmpID = calcAlphaCmpID(info.mComp0, info.mOp, info.mComp1);
	}

	GXCompare getComp0() const
	{
		return GXCompare(j3dAlphaCmpTable[mAlphaCmpID * 3]);
	}
	GXAlphaOp getOp() const
	{
		return GXAlphaOp(j3dAlphaCmpTable[mAlphaCmpID * 3 + 1]);
	}
	GXCompare getComp1() const
	{
		return GXCompare(j3dAlphaCmpTable[mAlphaCmpID * 3 + 2]);
	}
	u8 getRef0() const { return mRef0; }
	u8 getRef1() const { return mRef1; }

	void load()
	{
		if (mAlphaCmpID != 0xFFFF)
			GDSetAlphaCompare(getComp0(), getRef0(), getOp(), getComp1(),
			                  getRef1());
	}

public:
	/* 0x00 */ u16 mAlphaCmpID;
	/* 0x02 */ u8 mRef0;
	/* 0x03 */ u8 mRef1;
};

#endif
