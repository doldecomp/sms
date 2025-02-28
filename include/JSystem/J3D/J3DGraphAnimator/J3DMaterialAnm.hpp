#ifndef J3D_MATERIAL_ANM
#define J3D_MATERIAL_ANM

#include <dolphin/types.h>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>

struct J3DMaterial;

class J3DMatColorAnm {
public:
	J3DMatColorAnm()
	{
		mAnmColor = nullptr;
		mAnmIndex = 0;
	}
	~J3DMatColorAnm() { }
	J3DMatColorAnm(J3DAnmColor* color, u16 idx)
	{
		mAnmColor = color;
		mAnmIndex = idx;
	}

	void setAnmIndex(u16 index) { mAnmIndex = index; }
	void setAnmColor(J3DAnmColor* anmColor) { mAnmColor = anmColor; }
	void calc(GXColor*) const;

private:
	/* 0x0 */ u16 mAnmIndex;
	/* 0x2 */ u16 mAnmFlag;
	/* 0x4 */ J3DAnmColor* mAnmColor;
};

class J3DTexMtxAnm {
public:
	J3DTexMtxAnm()
	{
		mAnmIndex     = 0;
		mAnmTransform = nullptr;
	}
	~J3DTexMtxAnm() { }
	J3DTexMtxAnm(J3DAnmTextureSRTKey* pAnm, u16 anmIdx)
	{
		mAnmIndex     = anmIdx;
		mAnmTransform = pAnm;
	}

	void setAnmIndex(u16 index) { mAnmIndex = index; }
	void setAnmTransform(J3DAnmTextureSRTKey* transform)
	{
		mAnmTransform = transform;
	}
	void calc(J3DTextureSRTInfo*) const;

private:
	/* 0x0 */ u16 mAnmIndex;
	/* 0x2 */ u16 mAnmFlag;
	/* 0x4 */ J3DAnmTextureSRTKey* mAnmTransform;
};

class J3DTexNoAnm {
public:
	J3DTexNoAnm()
	{
		mAnmTexPattern = nullptr;
		mAnmIndex      = 0;
	}
	~J3DTexNoAnm() { }

	void setAnmIndex(u16 index) { mAnmIndex = index; }
	void setAnmTexPattern(J3DAnmTexPattern* pattern)
	{
		mAnmTexPattern = pattern;
	}
	virtual void calc(u16*) const;

private:
	/* 0x4 */ u16 mAnmIndex;
	/* 0x6 */ u16 mAnmFlag;
	/* 0x8 */ J3DAnmTexPattern* mAnmTexPattern;
};

class J3DTevColorAnm {
public:
	J3DTevColorAnm()
	{
		mAnmTevReg = nullptr;
		mAnmIndex  = 0;
	}
	~J3DTevColorAnm() { }
	J3DTevColorAnm(J3DAnmTevRegKey* tev_reg, u16 idx)
	{
		mAnmTevReg = tev_reg;
		mAnmIndex  = idx;
	}

	void setAnmIndex(u16 index) { mAnmIndex = index; }
	void setAnmTevReg(J3DAnmTevRegKey* tevReg) { mAnmTevReg = tevReg; }
	void calc(GXColorS10*) const;

private:
	/* 0x0 */ u16 mAnmIndex;
	/* 0x2 */ u16 mAnmFlag;
	/* 0x4 */ J3DAnmTevRegKey* mAnmTevReg;
};

class J3DTevKColorAnm {
public:
	J3DTevKColorAnm()
	{
		mAnmTevReg = nullptr;
		mAnmIndex  = 0;
	}
	~J3DTevKColorAnm() { }
	J3DTevKColorAnm(J3DAnmTevRegKey* tev_reg, u16 idx)
	{
		mAnmTevReg = tev_reg;
		mAnmIndex  = idx;
	}

	void setAnmIndex(u16 index) { mAnmIndex = index; }
	void setAnmTevReg(J3DAnmTevRegKey* tevReg) { mAnmTevReg = tevReg; }
	void calc(GXColor*) const;

private:
	/* 0x0 */ u16 mAnmIndex;
	/* 0x2 */ u16 mAnmFlag;
	/* 0x4 */ J3DAnmTevRegKey* mAnmTevReg;
};

class J3DMaterialAnm // size 0x6c
{
public:
	J3DMaterialAnm() { initialize(); }

	virtual ~J3DMaterialAnm() { }
	virtual void calc(J3DMaterial*) const;

	void initialize();

	void setMatColorAnm(int i, J3DMatColorAnm* pAnm) { mMatColorAnm[i] = pAnm; }
	void setTexMtxAnm(int i, J3DTexMtxAnm* pAnm) { mTexMtxAnm[i] = pAnm; }
	void setTexNoAnm(int i, J3DTexNoAnm* pAnm) { mTexNoAnm[i] = pAnm; }
	void setTevColorAnm(int i, J3DTevColorAnm* pAnm) { mTevColorAnm[i] = pAnm; }
	void setTevKColorAnm(int i, J3DTevKColorAnm* pAnm)
	{
		mTevKColorAnm[i] = pAnm;
	}

private:
	/* 0x04 */ J3DMatColorAnm* mMatColorAnm[2];
	/* 0x0C */ J3DTexMtxAnm* mTexMtxAnm[8];
	/* 0x2C */ J3DTexNoAnm* mTexNoAnm[8];
	/* 0x4C */ J3DTevColorAnm* mTevColorAnm[4];
	/* 0x5C */ J3DTevKColorAnm* mTevKColorAnm[4];
};

#endif
