#ifndef J3D_COMPONENTS_HPP
#define J3D_COMPONENTS_HPP

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTevs.hpp>

// These are all similar in that they roughly correspond to *Info structs and
// constitute components of blocks

struct J3DGXColorS10 {

	J3DGXColorS10& operator=(const GXColorS10& other)
	{
		color.r = other.r;
		color.g = other.g;
		color.b = other.b;
		color.a = other.a;
		return *this;
	}

	GXColorS10 color;
};

struct J3DGXColor {

	J3DGXColor& operator=(const GXColor& other)
	{
		color.r = other.r;
		color.g = other.g;
		color.b = other.b;
		color.a = other.a;
		return *this;
	}

	GXColor color;
};

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

struct J3DFog : public J3DFogInfo {
	J3DFog() { J3DFogInfo::operator=(j3dDefaultFogInfo); }

	void load() const
	{
		J3DGDSetFog(GXFogType(mType), mStartZ, mEndZ, mNearZ, mFarZ, mColor);
		JRNISetFogRangeAdj(mAdjEnable, mCenter, (GXFogAdjTable*)mFogAdjTable);
	}
};

struct J3DAlphaComp {
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

class J3DBlend : public J3DBlendInfo {
public:
	J3DBlend() { *(J3DBlendInfo*)this = j3dDefaultBlendInfo; }

	J3DBlend& operator=(const J3DBlend& other)
	{
		mBlendMode = other.mBlendMode;
		mSrcFactor = other.mSrcFactor;
		mDstFactor = other.mDstFactor;
		mLogicOp   = other.mLogicOp;
		return *this;
	}

	void load()
	{
		GDSetBlendMode((GXBlendMode)mBlendMode, (GXBlendFactor)mSrcFactor,
		               (GXBlendFactor)mDstFactor, (GXLogicOp)mLogicOp);
	}
};

class J3DZMode {
public:
	J3DZMode() { mZModeID = j3dDefaultZModeID; }

	u8 getCompareEnable() const { return j3dZModeTable[mZModeID * 3 + 0]; }
	u8 getFunc() const { return j3dZModeTable[mZModeID * 3 + 1]; }
	u8 getUpdateEnable() const { return j3dZModeTable[mZModeID * 3 + 2]; }

	void load() const
	{
		if (mZModeID != 0xFFFF)
			GDSetZMode(getCompareEnable(), (GXCompare)getFunc(),
			           getUpdateEnable());
	}

public:
	u16 mZModeID;
};

inline u32 setChanCtrlMacro(u8 enable, GXColorSrc ambSrc, GXColorSrc matSrc,
                            u32 lightMask, GXDiffuseFn diffuseFn,
                            GXAttnFn attnFn)
{
	u32 ret = matSrc << 0; // Putting this as a separate statement fixes
	                       // codegen, but regalloc is still wrong
	return ret | enable << 1 | (lightMask & 0x0F) << 2 | ambSrc << 6
	       | ((attnFn == GX_AF_SPEC) ? GX_DF_NONE : diffuseFn) << 7
	       | (attnFn != GX_AF_NONE) << 9 | (attnFn != GX_AF_SPEC) << 10
	       | (lightMask >> 4 & 0x0F) << 11;
}

struct J3DColorChan {
	J3DColorChan& operator=(const J3DColorChan& other)
	{
		mChanCtrl = other.mChanCtrl;
		return *this;
	}

	GXAttnFn getAttnFn()
	{
		u8 attnFnTbl[] = { GX_AF_NONE, GX_AF_SPEC, GX_AF_NONE, GX_AF_SPOT };
		return (GXAttnFn)attnFnTbl[mChanCtrl >> 9 & 0x03];
	}
	GXDiffuseFn getDiffuseFn() { return (GXDiffuseFn)(mChanCtrl >> 7 & 3); }
	u8 getLightMask()
	{
		return (mChanCtrl >> 2 & 0x0F) | (mChanCtrl >> 11 & 0x0f) << 4;
	}
	GXColorSrc getMatSrc() { return (GXColorSrc)(mChanCtrl >> 0 & 1); }
	GXColorSrc getAmbSrc() { return (GXColorSrc)(mChanCtrl >> 6 & 1); }
	u8 getEnable() { return (mChanCtrl >> 1) & 1; }

	void load(u32 chan)
	{
		const GXChannelID chanTbl[]
		    = { GX_COLOR0, GX_ALPHA0, GX_COLOR1, GX_ALPHA1 };
		J3DGDSetChanCtrl(chanTbl[chan], getEnable(), getAmbSrc(), getMatSrc(),
		                 getLightMask(), getDiffuseFn(), getAttnFn());
	}

	/* 0x0 */ u16 mChanCtrl;
};

#endif
