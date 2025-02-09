#ifndef J3D_MATERIAL_ANM
#define J3D_MATERIAL_ANM

#include <dolphin/types.h>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>

struct J3DMaterial;

class J3DMatColorAnm {
public:
	J3DMatColorAnm();
	~J3DMatColorAnm();

	void calc(GXColor*) const;

private:
	/* 0x0 */ u16 mAnmIndex;
	/* 0x2 */ u16 mAnmFlag;
	/* 0x4 */ J3DAnmColor* mAnmColor;
};

class J3DTexMtxAnm {
public:
	J3DTexMtxAnm();
	~J3DTexMtxAnm();

	void calc(J3DTextureSRTInfo*) const;

private:
	/* 0x0 */ u16 mAnmIndex;
	/* 0x2 */ u16 mAnmFlag;
	/* 0x4 */ J3DAnmTextureSRTKey* mAnmTransform;
};

class J3DTexNoAnm {
public:
	J3DTexNoAnm();
	~J3DTexNoAnm();

	virtual void calc(u16*) const;

private:
	/* 0x4 */ u16 mAnmIndex;
	/* 0x6 */ u16 mAnmFlag;
	/* 0x8 */ J3DAnmTexPattern* mAnmTexPattern;
};

class J3DTevColorAnm {
public:
	J3DTevColorAnm();
	~J3DTevColorAnm();

	void calc(GXColorS10*) const;

private:
	/* 0x0 */ u16 mAnmIndex;
	/* 0x2 */ u16 mAnmFlag;
	/* 0x4 */ J3DAnmTevRegKey* mAnmTevReg;
};

class J3DTevKColorAnm {
public:
	J3DTevKColorAnm();
	~J3DTevKColorAnm();

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

private:
	/* 0x04 */ J3DMatColorAnm* mMatColorAnm[2];
	/* 0x0C */ J3DTexMtxAnm* mTexMtxAnm[8];
	/* 0x2C */ J3DTexNoAnm* mTexNoAnm[8];
	/* 0x4C */ J3DTevColorAnm* mTevColorAnm[4];
	/* 0x5C */ J3DTevKColorAnm* mTevKColorAnm[4];
};

#endif
