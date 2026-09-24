#ifndef J3D_MATERIAL_FACTORY_HPP
#define J3D_MATERIAL_FACTORY_HPP

#include <JSystem/JUtility/JUTDataHeader.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DBlend.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DColorChan.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DFog.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DGXColor.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DGXColorS10.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DIndTexOrder.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DNBTScale.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DTevOrder.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DTevStage.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DTexCoord.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DTexMtx.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DZMode.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DTevSwapModeTable.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DIndTexMtx.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DIndTevStage.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DIndTexCoordScale.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DAlphaComp.hpp>

class J3DMaterial;
struct J3DTexCoord2Info;
struct J3DCurrentMtxInfo;

struct J3DMaterialBlock : public JUTDataBlockHeader {
	/* 0x08 */ u16 mMaterialNum;
	/* 0x0C */ PTR32(void) mpMaterialInitData;
	/* 0x10 */ PTR32(void) mpMaterialID;
	/* 0x14 */ PTR32(void) mpNameTable;
	/* 0x18 */ PTR32(void) mpIndInitData;
	/* 0x1C */ PTR32(void) mpCullMode;
	/* 0x20 */ PTR32(void) mpMatColor;
	/* 0x24 */ PTR32(void) mpColorChanNum;
	/* 0x28 */ PTR32(void) mpColorChanInfo;
	/* 0x2C */ PTR32(void) mpAmbColor;
	/* 0x30 */ PTR32(void) mpLightInfo;
	/* 0x34 */ PTR32(void) mpTexGenNum;
	/* 0x38 */ PTR32(void) mpTexCoordInfo;
	/* 0x3C */ PTR32(void) mpTexCoord2Info;
	/* 0x40 */ PTR32(void) mpTexMtxInfo;
	/* 0x44 */ PTR32(void) field_0x44;
	/* 0x48 */ PTR32(void) mpTexNo;
	/* 0x4C */ PTR32(void) mpTevOrderInfo;
	/* 0x50 */ PTR32(void) mpTevColor;
	/* 0x54 */ PTR32(void) mpTevKColor;
	/* 0x58 */ PTR32(void) mpTevStageNum;
	/* 0x5C */ PTR32(void) mpTevStageInfo;
	/* 0x60 */ PTR32(void) mpTevSwapModeInfo;
	/* 0x64 */ PTR32(void) mpTevSwapModeTableInfo;
	/* 0x68 */ PTR32(void) mpFogInfo;
	/* 0x6C */ PTR32(void) mpAlphaCompInfo;
	/* 0x70 */ PTR32(void) mpBlendInfo;
	/* 0x74 */ PTR32(void) mpZModeInfo;
	/* 0x78 */ PTR32(void) mpZCompLoc;
	/* 0x7C */ PTR32(void) mpDither;
	/* 0x80 */ PTR32(void) mpNBTScaleInfo;
};

struct J3DMaterialInitData {
	/* 0x000 */ u8 mMaterialMode;
	/* 0x001 */ u8 mCullModeIdx;
	/* 0x002 */ u8 mColorChanNumIdx;
	/* 0x003 */ u8 mTexGenNumIdx;
	/* 0x004 */ u8 mTevStageNumIdx;
	/* 0x005 */ u8 mZCompLocIdx;
	/* 0x006 */ u8 mZModeIdx;
	/* 0x007 */ u8 mDitherIdx;
	/* 0x008 */ u16 mMatColorIdx[2];
	/* 0x00C */ u16 mColorChanIdx[4];
	/* 0x014 */ u16 mAmbColorIdx[2];
	/* 0x018 */ u8 field_0x018[16];
	/* 0x028 */ u16 mTexCoordIdx[8];
	/* 0x038 */ u8 field_0x038[16];
	/* 0x048 */ u16 mTexMtxIdx[8];
	/* 0x058 */ u8 field_0x058[44];
	/* 0x084 */ u16 mTexNoIdx[8];
	/* 0x094 */ u16 mTevKColorIdx[4];
	/* 0x09C */ u8 mTevKColorSel[16];
	/* 0x0AC */ u8 mTevKAlphaSel[16];
	/* 0x0BC */ u16 mTevOrderIdx[16];
	/* 0x0DC */ u16 mTevColorIdx[4];
	/* 0x0E4 */ u16 mTevStageIdx[16];
	/* 0x104 */ u16 mTevSwapModeIdx[16];
	/* 0x124 */ u16 mTevSwapModeTableIdx[4];
	/* 0x12C */ u8 field_0x12c[24];
	/* 0x144 */ u16 mFogIdx;
	/* 0x146 */ u16 mAlphaCompIdx;
	/* 0x148 */ u16 mBlendIdx;
	/* 0x14A */ u16 mNBTScaleIdx;
};

struct J3DIndInitData {
	/* 0x000 */ bool mEnabled;
	/* 0x001 */ u8 mIndTexStageNum;
	/* 0x002 */ u8 field_0x002[2];
	/* 0x004 */ J3DIndTexOrderInfo mIndTexOrderInfo[3];
	/* 0x010 */ u8 field_0x010[4];
	/* 0x014 */ J3DIndTexMtxInfo mIndTexMtxInfo[3];
	/* 0x068 */ J3DIndTexCoordScaleInfo mIndTexCoordScaleInfo[3];
	/* 0x074 */ u8 field_0x074[4];
	/* 0x078 */ J3DIndTevStageInfo mIndTevStageInfo[16];
};

struct J3DPatchingInfo {
	/* 0x0 */ u16 mMatColorOffset;
	/* 0x2 */ u16 mColorChanOffset;
	/* 0x4 */ u16 mTexMtxOffset;
	/* 0x6 */ u16 mTexNoOffset;
	/* 0x8 */ u16 mTevRegOffset;
	/* 0xA */ u16 mFogOffset;
	/* 0xC */ u8 field_0xc[4];
};

struct J3DDisplayListInit {
	/* 0x0 */ u32 mOffset;
	/* 0x4 */ u32 mSize;
};

class J3DMaterialFactory {
public:
	J3DMaterialFactory(const J3DMaterialBlock&);
	u16 countUniqueMaterials();
	u32 countTexGens(int) const;
	u32 countStages(int) const;
	J3DMaterial* create(J3DMaterial*, int, u32) const;

	J3DGXColor newMatColor(int, int) const;
	u8 newColorChanNum(int) const;
	J3DColorChan newColorChan(int, int) const;
	J3DGXColor newAmbColor(int, int) const;
	void newLight(int, int) const;
	u32 newTexGenNum(int) const;
	J3DTexCoord newTexCoord(int, int) const;
	J3DTexMtx* newTexMtx(int, int) const;
	u8 newCullMode(int) const;
	u16 newTexNo(int, int) const;
	J3DTevOrder newTevOrder(int, int) const;
	J3DGXColorS10 newTevColor(int, int) const;
	J3DGXColor newTevKColor(int, int) const;
	u8 newTevStageNum(int) const;
	J3DTevStage newTevStage(int, int) const;
	J3DTevSwapModeTable newTevSwapModeTable(int, int) const;
	u8 newIndTexStageNum(int) const;
	J3DIndTexOrder newIndTexOrder(int, int) const;
	J3DIndTexMtx newIndTexMtx(int, int) const;
	J3DIndTevStage newIndTevStage(int, int) const;
	J3DIndTexCoordScale newIndTexCoordScale(int, int) const;
	J3DFog* newFog(int) const;
	J3DAlphaComp newAlphaComp(int) const;
	J3DBlend newBlend(int) const;
	J3DZMode newZMode(int) const;
	u8 newZCompLoc(int) const;
	u8 newDither(int) const;
	J3DNBTScale newNBTScale(int) const;

	J3DMaterialInitData* getMaterialInitData(u16) const; // Unused

	u16 getMaterialID(int idx) const { return mpMaterialID[idx]; }
	u8 getMaterialMode(int idx) const
	{
		return mpMaterialInitData[mpMaterialID[idx]].mMaterialMode;
	}

private:
	/* 0x00 */ u16 mMaterialNum;
	/* 0x02 */
	/* 0x04 */ J3DMaterialInitData* mpMaterialInitData;
	/* 0x08 */ u16* mpMaterialID;
	/* 0x0C */ J3DIndInitData* mpIndInitData;
	/* 0x10 */ GXColor* mpMatColor;
	/* 0x14 */ u8* mpColorChanNum;
	/* 0x18 */ J3DColorChanInfo* mpColorChanInfo;
	/* 0x1C */ GXColor* mpAmbColor;
	/* 0x20 */ J3DLightInfo* mpLightInfo;
	/* 0x24 */ u8* mpTexGenNum;
	/* 0x28 */ J3DTexCoordInfo* mpTexCoordInfo;
	/* 0x2C */ J3DTexCoord2Info* mpTexCoord2Info;
	/* 0x30 */ J3DTexMtxInfo* mpTexMtxInfo;
	/* 0x34 */ J3DTexMtxInfo* field_0x44;
	/* 0x38 */ u16* mpTexNo;
	/* 0x3C */ GXCullMode* mpCullMode;
	/* 0x40 */ J3DTevOrderInfo* mpTevOrderInfo;
	/* 0x44 */ GXColorS10* mpTevColor;
	/* 0x48 */ GXColor* mpTevKColor;
	/* 0x4C */ u8* mpTevStageNum;
	/* 0x50 */ J3DTevStageInfo* mpTevStageInfo;
	/* 0x54 */ J3DTevSwapModeInfo* mpTevSwapModeInfo;
	/* 0x58 */ J3DTevSwapModeTableInfo* mpTevSwapModeTableInfo;
	/* 0x5C */ J3DFogInfo* mpFogInfo;
	/* 0x60 */ J3DAlphaCompInfo* mpAlphaCompInfo;
	/* 0x64 */ J3DBlendInfo* mpBlendInfo;
	/* 0x68 */ J3DZModeInfo* mpZModeInfo;
	/* 0x6C */ u8* mpZCompLoc;
	/* 0x70 */ u8* mpDither;
	/* 0x74 */ J3DNBTScaleInfo* mpNBTScaleInfo;
};

#endif
