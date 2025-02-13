#ifndef J3DSYS_HPP
#define J3DSYS_HPP

#include <dolphin/gx.h>
#include <dolphin/mtx.h>

enum J3DErrType {
	J3DErrType_Success     = 0,
	J3DErrType_Locked      = 2,
	J3DErrType_OutOfMemory = 4,
};

struct J3DTexCoordScaleInfo {
	/* 0x0 */ u16 field_0x00;
	/* 0x2 */ u16 field_0x02;
	/* 0x4 */ u16 field_0x04;
	/* 0x6 */ u16 field_0x06;
};

class J3DTexture;
class J3DMatPacket;
class J3DMtxCalc;
class J3DModel;
class J3DDrawBuffer;

class J3DSys {
public:
	J3DSys();
	void loadPosMtxIndx(int, u16) const;
	void loadNrmMtxIndx(int, u16) const;
	void loadTexMtx(u32, float (*)[4]) const;
	void setTexCacheRegion(GXTexCacheSize);
	void drawInit();
	void reinitGX();
	void reinitGenMode();
	void reinitLighting();
	void reinitTransform();
	void reinitTexture();
	void reinitTevStages();
	void reinitIndStages();
	void reinitPixelProc();
	void ErrorReport(J3DErrType) const;

	// From TP debug
	void setModelDrawMtx(Mtx* mtx)
	{
		mCurrentDrawMtx = mtx;
		GXSetArray(GX_POS_MTX_ARRAY, mCurrentDrawMtx, sizeof(Mtx));
	}

	void setModelNrmMtx(Mtx33* mtx)
	{
		mCurrentNormMtx = mtx;
		GXSetArray(GX_NRM_MTX_ARRAY, mCurrentNormMtx, sizeof(Mtx33));
	}

	J3DTexture* getTexture() { return mTexture; }

	void setNBTScale(Vec* scale) { mNBTScale = scale; }

	MtxPtr getViewMtx() { return mViewMtx; }

	J3DMatPacket* getMatPacket() { return mMatPacket; }
	void setMatPacket(J3DMatPacket* packet) { mMatPacket = packet; }

	void setCurrentMtxCalc(J3DMtxCalc* pCalc) { mCurrentMtxCalc = pCalc; }

	J3DModel* getModel() { return mModel; }

	J3DMtxCalc* getCurrentMtxCalc() const { return mCurrentMtxCalc; }

	// Type 0: Opa Buffer
	// Type 1: Xlu Buffer
	J3DDrawBuffer* getDrawBuffer(int type) { return mDrawBuffer[type]; }

	void setModel(J3DModel* pModel) { mModel = pModel; }
	void setTexture(J3DTexture* pTex) { mTexture = pTex; }

	// Completely made up
	int checkFlag2() { return mFlags & 2 ? TRUE : FALSE; }
	int checkFlag4() { return mFlags & 4 ? TRUE : FALSE; }
	int checkFlag8() { return mFlags & 8 ? TRUE : FALSE; }

public:
	/* 0x0 */ Mtx mViewMtx;
	/* 0x30 */ J3DMtxCalc* mCurrentMtxCalc;
	/* 0x34 */ s32 mFlags;
	/* 0x038 */ J3DModel* mModel;
	/* 0x3C */ J3DMatPacket* mMatPacket;
	/* 0x40 */ u32 unk40;
	/* 0x44 */ J3DDrawBuffer* mDrawBuffer[2];
	/* 0x4C */ u32 unk4C;
	/* 0x50 */ u32 unk50;
	/* 0x54 */ J3DTexture* mTexture;
	/* 0x58 */ char padd_003[0x4];
	/* 0x5C */ u32 mTexCacheRegionNum;
	/* 0x60 */ GXTexRegion mTexCacheRegion[8];
	/* 0xE0 */ char padd_005[0x20];
	/* 0x100 */ u32 unk100;
	/* 0x104 */ Mtx* mCurrentDrawMtx;
	/* 0x108 */ Mtx33* mCurrentNormMtx;
	/* 0x10C */ void* unk10C;
	/* 0x110 */ void* unk110;
	/* 0x114 */ void* unk114;
	/* 0x118 */ u32 unk118;
	/* 0x11C */ u32 unk11C;
	/* 0x120 */ Vec* mNBTScale;

	static Mtx mCurrentMtx;
	static Vec mCurrentS;
	static Vec mParentS;
	static J3DTexCoordScaleInfo sTexCoordScaleTable[8];
};

extern J3DSys j3dSys;
// TODO: type
extern u32 j3dDefaultViewNo;

#endif
