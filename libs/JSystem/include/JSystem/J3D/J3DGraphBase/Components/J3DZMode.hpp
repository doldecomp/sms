#ifndef J3D_GRAPH_BASE_COMPONENTS_ZMODE
#define J3D_GRAPH_BASE_COMPONENTS_ZMODE

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>
#include <dolphin/gd.h>

extern const u16 j3dDefaultZModeID;
extern u8 j3dZModeTable[96];

inline u16 calcZModeID(u8 param_0, u8 param_1, u8 param_2)
{
	return (param_1 * 2) + (param_0 * 0x10) + param_2;
}

class J3DZMode {
public:
	J3DZMode() { mZModeID = j3dDefaultZModeID; }
	J3DZMode(u16 id) { mZModeID = id; }
	J3DZMode(const J3DZModeInfo& info) { setZModeInfo(info); }

	J3DZMode& operator=(const J3DZMode& other)
	{
		mZModeID = other.mZModeID;
		return *this;
	}

	u8 getCompareEnable() const { return j3dZModeTable[mZModeID * 3 + 0]; }
	u8 getFunc() const { return j3dZModeTable[mZModeID * 3 + 1]; }
	u8 getUpdateEnable() const { return j3dZModeTable[mZModeID * 3 + 2]; }

	void setZModeInfo(const J3DZModeInfo& info)
	{
		mZModeID
		    = calcZModeID(info.mCompareEnable, info.mFunc, info.mUpdateEnable);
	}

	void load() const
	{
		if (mZModeID != 0xFFFF)
			GDSetZMode(getCompareEnable(), (GXCompare)getFunc(),
			           getUpdateEnable());
	}

	void setCompareEnable(u8 enable)
	{
		mZModeID = calcZModeID(enable, j3dZModeTable[mZModeID * 3 + 1],
		                       j3dZModeTable[mZModeID * 3 + 2]);
	}

	void setCompareFunc(u8 func)
	{
		mZModeID = calcZModeID(j3dZModeTable[mZModeID * 3], func,
		                       j3dZModeTable[mZModeID * 3 + 2]);
	}

	void setUpdateEnable(u8 enable)
	{
		mZModeID = calcZModeID(j3dZModeTable[mZModeID * 3],
		                       j3dZModeTable[mZModeID * 3 + 1], enable);
	}

public:
	u16 mZModeID;
};

#endif
