#ifndef J3D_COMPONENTS_HPP
#define J3D_COMPONENTS_HPP

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTevs.hpp>
#include <dolphin/mtx.h>

// These are all similar in that they roughly correspond to *Info structs and
// constitute components of blocks

class J3DGXColorS10 {
public:
	J3DGXColorS10() { }

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
	J3DGXColor() { }

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
	J3DTevStage()
	{
		setTevStageInfo(j3dDefaultTevStageInfo);
		setTevSwapModeInfo(j3dDefaultTevSwapMode);
	}

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

	void setTevColorOp(u8 param_1, u8 param_2, u8 param_3, u8 param_4,
	                   u8 param_5)
	{
		mTevColorOp = mTevColorOp & ~(0x01 << 2) | param_1 << 2;
		if (param_1 <= 1) {
			mTevColorOp = mTevColorOp & ~(0x03 << 4) | param_3 << 4;
			mTevColorOp = mTevColorOp & ~0x03 | param_2;
		} else {
			mTevColorOp = mTevColorOp & ~(0x03 << 4) | (param_1 >> 1 & 3) << 4;
			mTevColorOp = mTevColorOp & ~0x03 | 3;
		}
		mTevColorOp = mTevColorOp & ~(0x01 << 3) | param_4 << 3;
		mTevColorOp = mTevColorOp & ~(0x03 << 6) | param_5 << 6;
	}
	void setTevColorAB(u8 a, u8 b) { mTevColorAB = a << 4 | b; }
	void setTevColorCD(u8 c, u8 d) { mTevColorCD = c << 4 | d; }

	void setTevAlphaOp(u8 param_1, u8 param_2, u8 param_3, u8 param_4,
	                   u8 param_5)
	{
		mTevAlphaOp = mTevAlphaOp & ~(0x01 << 2) | param_1 << 2;
		if (param_1 <= 1) {
			mTevAlphaOp = mTevAlphaOp & ~0x03 | param_2;
			mTevAlphaOp = mTevAlphaOp & ~(0x03 << 4) | param_3 << 4;
		} else {
			mTevAlphaOp = mTevAlphaOp & ~(0x03 << 4) | (param_1 >> 1 & 3) << 4;
			mTevAlphaOp = mTevAlphaOp & ~0x03 | 3;
		}
		mTevAlphaOp = mTevAlphaOp & ~(0x01 << 3) | param_4 << 3;
		mTevAlphaOp = mTevAlphaOp & ~(0x03 << 6) | param_5 << 6;
	}

	void setAlphaA(u8 a) { mTevAlphaAB = mTevAlphaAB & ~(0x07 << 5) | a << 5; }
	void setAlphaB(u8 b) { mTevAlphaAB = mTevAlphaAB & ~(0x07 << 2) | b << 2; }
	void setAlphaC(u8 c)
	{
		mTevAlphaAB      = mTevAlphaAB & ~0x03 | c >> 1;
		mTevSwapModeInfo = mTevSwapModeInfo & ~(0x01 << 7) | c << 7;
	}
	void setAlphaD(u8 d)
	{
		mTevSwapModeInfo = mTevSwapModeInfo & ~(0x07 << 4) | d << 4;
	}
	void setAlphaABCD(u8 a, u8 b, u8 c, u8 d)
	{
		setAlphaA(a);
		setAlphaB(b);
		setAlphaC(c);
		setAlphaD(d);
	}

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

	void setTevStageInfo(const J3DTevStageInfo& info)
	{
		setTevColorOp(info.field_0x5, info.field_0x6, info.field_0x7,
		              info.field_0x8, info.field_0x9);
		setTevColorAB(info.field_0x1, info.field_0x2);
		setTevColorCD(info.field_0x3, info.field_0x4);
		setAlphaABCD(info.field_0xa, info.field_0xb, info.field_0xc,
		             info.field_0xd);
		setTevAlphaOp(info.field_0xe, info.field_0xf, info.field_0x10,
		              info.field_0x11, info.field_0x12);
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
	J3DIndTevStage()
	{
		J3DIndTevStageInfo::operator=(j3dDefaultIndTevStageInfo);
	}

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

struct J3DNBTScale : public J3DNBTScaleInfo {
	J3DNBTScale() { *(J3DNBTScaleInfo*)this = j3dDefaultNBTScaleInfo; }
	explicit J3DNBTScale(const J3DNBTScaleInfo& info)
	{
		*(J3DNBTScaleInfo*)this = info;
	}

	J3DNBTScale& operator=(const J3DNBTScale& other)
	{
		mbHasScale = other.mbHasScale;
		mScale.x   = other.mScale.x;
		mScale.y   = other.mScale.y;
		mScale.z   = other.mScale.z;
		return *this;
	}

	Vec* getScale() { return &mScale; }
};

class J3DTevOrder : public J3DTevOrderInfo {
public:
	J3DTevOrder() { J3DTevOrderInfo::operator=(j3dDefaultTevOrderInfoNull); }

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

struct J3DIndTexOrder : public J3DIndTexOrderInfo {
	J3DIndTexOrder() { *(J3DIndTexOrderInfo*)this = j3dDefaultIndTexOrderNull; }

	u8 getCoord() const { return mCoord; }
	u8 getMap() const { return mMap; }

	void load(u32) const;
};

struct J3DIndTexCoordScale : public J3DIndTexCoordScaleInfo {
	J3DIndTexCoordScale()
	{
		*(J3DIndTexCoordScaleInfo*)this = j3dDefaultIndTexCoordScaleInfo;
	}
	~J3DIndTexCoordScale() { }

	u8 getScaleS() { return mScaleS; }
	u8 getScaleT() { return mScaleT; }

	void load(u32) const { }
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

inline u16 calcColorChanID(u16 enable, u8 matSrc, u8 lightMask, u8 diffuseFn,
                           u8 attnFn, u8 ambSrc)
{
	u32 reg = 0;
	reg     = reg & ~0x0002 | enable << 1;
	reg     = reg & ~0x0001 | matSrc;
	reg     = reg & ~0x0040 | ambSrc << 6;
	reg     = reg & ~0x0004 | bool(lightMask & 0x01) << 2;
	reg     = reg & ~0x0008 | bool(lightMask & 0x02) << 3;
	reg     = reg & ~0x0010 | bool(lightMask & 0x04) << 4;
	reg     = reg & ~0x0020 | bool(lightMask & 0x08) << 5;
	reg     = reg & ~0x0800 | bool(lightMask & 0x10) << 11;
	reg     = reg & ~0x1000 | bool(lightMask & 0x20) << 12;
	reg     = reg & ~0x2000 | bool(lightMask & 0x40) << 13;
	reg     = reg & ~0x4000 | bool(lightMask & 0x80) << 14;
	reg     = reg & ~0x0180 | (attnFn == GX_AF_SPEC ? 0 : diffuseFn) << 7;
	reg     = reg & ~0x0200 | (attnFn != GX_AF_NONE) << 9;
	reg     = reg & ~0x0400 | (attnFn != GX_AF_SPEC) << 10;
	return reg;
}

struct J3DColorChan {
	J3DColorChan() { setColorChanInfo(j3dDefaultColorChanInfo); }

	J3DColorChan& operator=(const J3DColorChan& other)
	{
		mChanCtrl = other.mChanCtrl;
		return *this;
	}

	void setColorChanInfo(const J3DColorChanInfo& info)
	{
		// Comment from TWW/TP:
		// Bug: It compares info.mAmbSrc (an 8 bit integer) with 0xFFFF instead
		// of 0xFF. This inline is only called by the default constructor
		// J3DColorChan(). The J3DColorChan(const J3DColorChanInfo&) constructor
		// does not call this inline, and instead duplicates the same logic but
		// without the bug. See J3DMaterialFactory::newColorChan - both the
		// bugged and correct behavior are present there, as it calls both
		// constructors.
		mChanCtrl = calcColorChanID(
		    info.mEnable, info.mMatSrc, info.mLightMask, info.mDiffuseFn,
		    info.mAttnFn, info.mAmbSrc == 0xFFFF ? (u8)0 : info.mAmbSrc);
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

struct J3DIndTexMtx : public J3DIndTexMtxInfo {
	J3DIndTexMtx() { *(J3DIndTexMtxInfo*)this = j3dDefaultIndTexMtxInfo; }
	~J3DIndTexMtx() { }

	void load(u32 idx)
	{
		JRNSetIndTexMtx((GXIndTexMtxID)idx, mOffsetMtx, mScaleExp);
	}
};

class J3DTexCoord : public J3DTexCoordInfo {
public:
	J3DTexCoord() { J3DTexCoordInfo::operator=(j3dDefaultTexCoordInfo[0]); }

	J3DTexCoord& operator=(const J3DTexCoord& other)
	{
		mTexGenType = other.mTexGenType;
		mTexGenSrc  = other.mTexGenSrc;
		mTexGenMtx  = other.mTexGenMtx;
		return *this;
	}

	GXTexGenType getTexGenType() { return (GXTexGenType)mTexGenType; }
	GXTexGenSrc getTexGenSrc() { return (GXTexGenSrc)mTexGenSrc; }
	u8 getTexGenMtx() { return mTexGenMtx; }
};

class J3DLightObj : public J3DLightInfo {
public:
	J3DLightObj() { setDefault(); }

	// Completely made up to force J3DLightInfo::operator= to NOT inline
	void setDefault() { setInfo(j3dDefaultLightInfo); }
	void setInfo(const J3DLightInfo& info)
	{
		J3DLightInfo::operator=(j3dDefaultLightInfo);
	}

	void load(u32) const;

public:
	char unk34[0x40];
};

struct J3DTexMtx : public J3DTexMtxInfo {
	J3DTexMtx() { J3DTexMtxInfo::operator=(j3dDefaultTexMtxInfo); }

	void calc();
	void load(u32) const;

	Mtx& getViewMtx() { return mViewMtx; }
	void setViewMtx(const Mtx viewMtx) { MTXCopy((MtxPtr)viewMtx, mViewMtx); }

	// Made up
	u32 getMode() { return mInfo & 0x7F; }

	/* 0x64 */ Mtx mTotalMtx;
	/* 0x94 */ Mtx mViewMtx;
};

#endif
