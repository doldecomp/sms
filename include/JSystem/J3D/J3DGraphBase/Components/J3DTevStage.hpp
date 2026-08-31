#ifndef J3D_GRAPH_BASE_COMPONENTS_TEVSTAGE
#define J3D_GRAPH_BASE_COMPONENTS_TEVSTAGE

#include <JSystem/JRenderer.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>

extern const J3DTevSwapModeInfo j3dDefaultTevSwapMode;
extern const J3DTevStageInfo j3dDefaultTevStageInfo;

class J3DTevStage {
public:
	J3DTevStage()
	{
		setTevStageInfo(j3dDefaultTevStageInfo);
		setTevSwapModeInfo(j3dDefaultTevSwapMode);
	}
	J3DTevStage(const J3DTevStageInfo& info)
	{
		setTevStageInfo(info);
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

	void setTevColorOp(u8 op, u8 bias, u8 scale, u8 clamp, u8 out_reg)
	{
		mTevColorOp = mTevColorOp & ~(0x01 << 2) | op << 2;
		if (op <= 1) {
			mTevColorOp = mTevColorOp & ~(0x03 << 4) | scale << 4;
			mTevColorOp = mTevColorOp & ~0x03 | bias;
		} else {
			mTevColorOp = mTevColorOp & ~(0x03 << 4) | (op >> 1 & 3) << 4;
			mTevColorOp = mTevColorOp & ~0x03 | 3;
		}
		mTevColorOp = mTevColorOp & ~(0x01 << 3) | clamp << 3;
		mTevColorOp = mTevColorOp & ~(0x03 << 6) | out_reg << 6;
	}

	u8 getTevColorOp() const
	{
		if ((mTevColorOp & 3) != 3U)
			return mTevColorOp >> 2 & 1;
		else
			return 0x8 + (mTevColorOp >> 2 & 1) + (mTevColorOp >> 3 & 6);
	}

	u8 getTevColorBias() const { return mTevColorOp & 3; }
	u8 getTevColorScale() const { return mTevColorOp >> 4 & 3; }
	u8 getTevColorClamp() const { return mTevColorOp >> 3 & 1; }
	u8 getTevColorOutReg() const { return mTevColorOp >> 6 & 3; }

	void setTevColorAB(u8 a, u8 b) { mTevColorAB = a << 4 | b; }
	void setTevColorCD(u8 c, u8 d) { mTevColorCD = c << 4 | d; }

	u8 getTevColorA() const { return mTevColorAB >> 4 & 0xf; }
	u8 getTevColorB() const { return mTevColorAB & 0xf; }
	u8 getTevColorC() const { return mTevColorCD >> 4 & 0xf; }
	u8 getTevColorD() const { return mTevColorCD & 0xf; }

	/// See setTevColorOp for the bit layout. Note that this one writes the bias
	/// before the scale, and setTevColorOp writes them in the other order.
	void setTevAlphaOp(u8 op, u8 bias, u8 scale, u8 clamp, u8 out_reg)
	{
		mTevAlphaOp = mTevAlphaOp & ~(0x01 << 2) | op << 2;
		if (op <= 1) {
			mTevAlphaOp = mTevAlphaOp & ~0x03 | bias;
			mTevAlphaOp = mTevAlphaOp & ~(0x03 << 4) | scale << 4;
		} else {
			mTevAlphaOp = mTevAlphaOp & ~(0x03 << 4) | (op >> 1 & 3) << 4;
			mTevAlphaOp = mTevAlphaOp & ~0x03 | 3;
		}
		mTevAlphaOp = mTevAlphaOp & ~(0x01 << 3) | clamp << 3;
		mTevAlphaOp = mTevAlphaOp & ~(0x03 << 6) | out_reg << 6;
	}

	u8 getTevAlphaOp() const
	{
		if ((mTevAlphaOp & 3) != 3U)
			return mTevAlphaOp >> 2 & 1;
		else
			return 0x8 + (mTevAlphaOp >> 2 & 1) + (mTevAlphaOp >> 3 & 6);
	}
	u8 getTevAlphaBias() const { return mTevAlphaOp & 3; }
	u8 getTevAlphaScale() const { return mTevAlphaOp >> 4 & 3; }
	u8 getTevAlphaClamp() const { return mTevAlphaOp >> 3 & 1; }
	u8 getTevAlphaOutReg() const { return mTevAlphaOp >> 6 & 3; }

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

	u8 getAlphaA() const { return mTevAlphaAB >> 5 & 7; }
	u8 getAlphaB() const { return mTevAlphaAB >> 2 & 7; }
	u8 getAlphaC() const
	{
		return (mTevAlphaAB << 1 & 0x6) | (mTevSwapModeInfo >> 7 & 1);
	}
	u8 getAlphaD() const { return mTevSwapModeInfo >> 4 & 7; }

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

#endif
