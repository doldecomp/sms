#ifndef J3D_GRAPH_BASE_COMPONENTS_TEVSWAPMODETABLE
#define J3D_GRAPH_BASE_COMPONENTS_TEVSWAPMODETABLE

#include <dolphin/types.h>

extern const u8 j3dDefaultTevSwapTableID;
extern u8 j3dTevSwapTableTable[1024];

class J3DTevSwapModeTable {
public:
	J3DTevSwapModeTable() { mIdx = j3dDefaultTevSwapTableID; }

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
