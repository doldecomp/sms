#ifndef J3D_GRAPH_BASE_COMPONENTS_TEVSWAPMODETABLE
#define J3D_GRAPH_BASE_COMPONENTS_TEVSWAPMODETABLE

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>
#include <dolphin/types.h>

extern const J3DTevSwapModeTableInfo j3dDefaultTevSwapModeTable;
extern const u8 j3dDefaultTevSwapTableID;
extern u8 j3dTevSwapTableTable[1024];

inline u8 calcTevSwapTableID(u8 r, u8 g, u8 b, u8 a)
{
	return r * 64 + g * 16 + b * 4 + a;
}

class J3DTevSwapModeTable {
public:
	J3DTevSwapModeTable() { mIdx = j3dDefaultTevSwapTableID; }
	J3DTevSwapModeTable(const J3DTevSwapModeTableInfo& info)
	{
		mIdx = calcTevSwapTableID(info.field_0x0, info.field_0x1,
		                          info.field_0x2, info.field_0x3);
	}

	J3DTevSwapModeTable& operator=(const J3DTevSwapModeTable& other)
	{
		mIdx = other.mIdx;
		return *this;
	}

	u8 getR() { return j3dTevSwapTableTable[mIdx * 4]; }
	u8 getG() { return j3dTevSwapTableTable[mIdx * 4 + 1]; }
	u8 getB() { return j3dTevSwapTableTable[mIdx * 4 + 2]; }
	u8 getA() { return j3dTevSwapTableTable[mIdx * 4 + 3]; }

	u8 mIdx;
};

#endif
