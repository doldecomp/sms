#ifndef J3D_TEVS_HPP
#define J3D_TEVS_HPP

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>
#include <JSystem/JRenderer.hpp>
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

extern u8 j3dTexCoordTable[7624];
extern u8 j3dTevSwapTableTable[1024];
extern u8 j3dAlphaCmpTable[768];
extern u8 j3dZModeTable[96];

class J3DTevStage {
public:
	J3DTevStage();

	J3DTevStage& operator=(const J3DTevStage& other)
	{
		mTevColorOp      = other.mTevColorOp;
		mTevColorAB      = other.mTevColorAB;
		mTevColorCD      = other.mTevColorCD;
		mTevAlphaOp      = other.mTevAlphaOp;
		mTevAlphaAB      = other.mTevAlphaAB;
		mTevSwapModeInfo = other.mTevSwapModeInfo;
		return *this;
	}

	void setTevAlphaOp(u8, u8, u8, u8, u8);
	void setTevColorOp(u8, u8, u8, u8, u8);

	void setTexSel(u8 param_0)
	{
		mTevSwapModeInfo = mTevSwapModeInfo & ~0x0C | param_0 << 2;
	}
	void setRasSel(u8 param_0)
	{
		mTevSwapModeInfo = mTevSwapModeInfo & ~0x03 | param_0;
	}
	void setTevSwapModeInfo(const J3DTevSwapModeInfo& info)
	{
		setTexSel(info.mTexSel);
		setRasSel(info.mRasSel);
	}

	void load(u32) const
	{
		GDOverflowCheck(10);
		J3DGDWriteBPCmd(*(u32*)&mTevColorReg);
		J3DGDWriteBPCmd(*(u32*)&mTevAlphaReg);
	}

public:
	/* 0x0 */ u8 mTevColorReg;
	/* 0x1 */ u8 mTevColorOp;
	/* 0x2 */ u8 mTevColorAB;
	/* 0x3 */ u8 mTevColorCD;
	/* 0x4 */ u8 mTevAlphaReg;
	/* 0x5 */ u8 mTevAlphaOp;
	/* 0x6 */ u8 mTevAlphaAB;
	/* 0x7 */ u8 mTevSwapModeInfo;
};

// TODO: does this really inherit?
class J3DIndTevStage : public J3DIndTevStageInfo {
public:
	J3DIndTevStage& operator=(const J3DIndTevStage& other)
	{
		mIndStage  = other.mIndStage;
		mIndFormat = other.mIndFormat;
		mBiasSel   = other.mBiasSel;
		mMtxSel    = other.mMtxSel;
		mWrapS     = other.mWrapS;
		mWrapT     = other.mWrapT;
		mPrev      = other.mPrev;
		mLod       = other.mLod;
		mAlphaSel  = other.mAlphaSel;
		return *this;
	}

	void load(u32 param_1)
	{
		JRNSetTevIndirect((GXTevStageID)param_1, (GXIndTexStageID)mIndStage,
		                  (GXIndTexFormat)mIndFormat, (GXIndTexBiasSel)mBiasSel,
		                  (GXIndTexMtxID)mMtxSel, (GXIndTexWrap)mWrapS,
		                  (GXIndTexWrap)mWrapT, mPrev, mLod,
		                  (GXIndTexAlphaSel)mAlphaSel);
	}
};

class J3DTevSwapModeTable {
public:
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

void J3DGDSetTexLookupMode(GXTexMapID id, GXTexWrapMode wrapS,
                           GXTexWrapMode wrapT, GXTexFilter minFilter,
                           GXTexFilter magFilter, f32 minLOD, f32 maxLOD,
                           f32 lodBias, u8 biasClamp, u8 edgeLOD,
                           GXAnisotropy maxAniso);

struct J3DNBTScale : public J3DNBTScaleInfo {
	// J3DNBTScale() { *(J3DNBTScaleInfo*)this = j3dDefaultNBTScaleInfo; }
	explicit J3DNBTScale(const J3DNBTScaleInfo& info)
	{
		*(J3DNBTScaleInfo*)this = info;
	}
	Vec* getScale() { return &mScale; }
};

class J3DTevOrder : public J3DTevOrderInfo {
public:
	J3DTevOrder& operator=(const J3DTevOrder& other)
	{
		mTexCoord  = other.mTexCoord;
		mTexMap    = other.mTexMap;
		mColorChan = other.mColorChan;
		return *this;
	}

	J3DTevOrderInfo& getTevOrderInfo() { return *this; }

	void load(u32) const;
};

struct J3DIndTexOrder {
	void load(u32) const;
};

struct J3DIndTexCoordScale {
	void load(unsigned long) const;
};

#endif
