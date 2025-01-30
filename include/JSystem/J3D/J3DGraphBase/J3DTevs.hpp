#ifndef J3D_TEVS_HPP
#define J3D_TEVS_HPP

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

extern const J3DLightInfo j3dDefaultLightInfo;
extern const J3DTexCoordInfo j3dDefaultTexCoordInfo[8];
extern const J3DTexMtxInfo j3dDefaultTexMtxInfo;
extern const J3DIndTexMtxInfo j3dDefaultIndTexMtxInfo;
extern const J3DTevStageInfo j3dDefaultTevStageInfo;
extern const J3DIndTevStageInfo j3dDefaultIndTevStageInfo;
extern const J3DFogInfo j3dDefaultFogInfo;
extern const J3DNBTScaleInfo j3dDefaultNBTScaleInfo;

extern const GXColor j3dDefaultColInfo;
extern const GXColor j3dDefaultAmbInfo;
extern const u8 j3dDefaultColorChanNum;
extern const J3DTevOrderInfo j3dDefaultTevOrderInfoNull;
extern const J3DIndTexOrderInfo j3dDefaultIndTexOrderNull;
extern const GXColorS10 j3dDefaultTevColor;
extern const J3DIndTexCoordScaleInfo j3dDefaultIndTexCoordScaleInfo;
extern const GXColor j3dDefaultTevKColor;
extern const J3DTevSwapModeInfo j3dDefaultTevSwapMode;
extern const J3DTevSwapModeTableInfo j3dDefaultTevSwapModeTable;
extern const J3DBlendInfo j3dDefaultBlendInfo;
extern const J3DColorChanInfo j3dDefaultColorChanInfo;
extern const u8 j3dDefaultTevSwapTableID;
extern const u16 j3dDefaultAlphaCmpID;
extern const u16 j3dDefaultZModeID;

void J3DLoadCPCmd(u8, u32);

void J3DLoadArrayBasePtr(GXAttr, void*);
void J3DSetVtxAttrFmtv(GXVtxFmt, GXVtxAttrFmtList*, bool);

struct J3DLightObj {
	void load(u32) const;

	Vec mPos;
	Vec mDir;
	GXColor mColor;
	Vec mAttnA;
	Vec mAttnK;
};

struct J3DTexMtx {
	void calc();
	void load(u32) const;

	J3DTexMtxInfo mTexMtxInfo;
	/* 0x64 */ Mtx mTotalMtx;
	/* 0x94 */ Mtx mViewMtx;
};

void J3DGDSetTexLookupMode(GXTexMapID, GXTexWrapMode, GXTexWrapMode,
                           GXTexFilter, GXTexFilter, float, float, float, u8,
                           u8, _GXAnisotropy);

struct J3DNBTScale : public J3DNBTScaleInfo {
	// J3DNBTScale() { *(J3DNBTScaleInfo*)this = j3dDefaultNBTScaleInfo; }
	explicit J3DNBTScale(const J3DNBTScaleInfo& info)
	{
		*(J3DNBTScaleInfo*)this = info;
	}
	Vec* getScale() { return &mScale; }
};

struct J3DTevOrder {
	void load(u32) const;
};

struct J3DIndTexOrder {
	void load(u32) const;
};

struct J3DIndTexCoordScale {
	void load(unsigned long) const;
};

#endif
