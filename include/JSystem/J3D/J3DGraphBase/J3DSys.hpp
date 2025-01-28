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

public:
	/* 0x0 */ Mtx mViewMtx;
	/* 0x30 */ char pad_000[0x4];
	/* 0x34 */ s32 mFlags;
	/* 0x38 */ u32 unk38;
	/* 0x3C */ void* unk3C;
	/* 0x40 */ u32 unk40;
	/* 0x44 */ u32 unk44;
	/* 0x48 */ u32 unk48;
	/* 0x4C */ u32 unk4C;
	/* 0x50 */ u32 unk50;
	/* 0x54 */ u32 unk54;
	/* 0x58 */ char padd_003[0x4];
	/* 0x5C */ u32 mTexCacheRegionNum;
	/* 0x60 */ GXTexRegion mTexCacheRegion[8];
	/* 0xE0 */ char padd_005[0x20];
	/* 0x100 */ u32 unk100;
	/* 0x104 */ u32 unk104;
	/* 0x108 */ u32 unk108;
	/* 0x10C */ u32 unk10C;
	/* 0x110 */ u32 unk110;
	/* 0x114 */ u32 unk114;
	/* 0x118 */ u32 unk118;
	/* 0x11C */ u32 unk11C;
	/* 0x120 */ char pad_006[0x4];

	static Mtx mCurrentMtx;
	static Vec mCurrentS;
	static Vec mParentS;
	static J3DTexCoordScaleInfo sTexCoordScaleTable[8];
};

extern J3DSys j3dSys;
// TODO: type
extern u32 j3dDefaultViewNo;

#endif
