#ifndef J3D_GRAPH_BASE_COMPONENTS_COLORCHAN
#define J3D_GRAPH_BASE_COMPONENTS_COLORCHAN

#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>
#include <JSystem/JRenderer.hpp>

extern const J3DColorChanInfo j3dDefaultColorChanInfo;

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

class J3DColorChan {
public:
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

#endif
