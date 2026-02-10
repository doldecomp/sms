#ifndef M3DUTIL_M3U_MODEL_HPP
#define M3DUTIL_M3U_MODEL_HPP

#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>

struct M3UMtxCalcSetInfo;
class J3DModel;
class J3DTexNoAnm;
class J3DMtxCalc;
class J3DMtxCalcBasicAnm;
class J3DMtxCalcSoftimageAnm;

class M3UModelCommon {
public:
	virtual J3DMtxCalc* getMtxCalc(const M3UMtxCalcSetInfo&);

	// Fabricated
	J3DTexNoAnm* getTexNoAnm(u8 idx) { return unkC[idx]; }

public:
	/* 0x4 */ J3DAnmTransform** unk4;
	/* 0x8 */ J3DAnmTexPattern** unk8;
	/* 0xC */ J3DTexNoAnm** unkC;
	/* 0x10 */ J3DMtxCalcBasicAnm* unk10;
	/* 0x14 */ J3DMtxCalcSoftimageAnm* unk14;
};

class M3UMtxCalcSetInfo {
public:
	/* 0x0 */ u16 mJntIdx;
	/* 0x2 */ u8 mAnmType;
	/* 0x3 */ u8 mMtxCalcIdx;
	/* 0x4 */ u8 mAnmTransformIdx;
	/* 0x5 */ u8 mFrameCalcIdx;
};

class M3UModel {
public:
	virtual void changeMtxCalcAnmTransform(int, u8);
	virtual void changeAnmTexPattern(int, u8);
	virtual void setMtxCalc(const M3UMtxCalcSetInfo&) { }
	virtual void updateIn();
	virtual void updateOut();
	virtual void entryIn();
	virtual void entryOut();

	void perform(u32, JDrama::TGraphics*);
	void entryOutTexPatternAnm();
	void entryInTexPatternAnm();
	void updateInTexPatternAnm();
	void updateInMotion();

	// Fabricated
	J3DFrameCtrl& getFrameCtrl(u8 idx) { return unkC[idx]; }

public:
	/* 0x4 */ M3UModelCommon* unk4;
	/* 0x8 */ J3DModel* unk8;
	/* 0xC */ J3DFrameCtrl* unkC; // Array of size 3
	/* 0x10 */ u16 unk10;
	/* 0x14 */ M3UMtxCalcSetInfo* unk14; // Size matches unk1C
	/* 0x18 */ u32 unk18;
	/* 0x1C */ u8* unk1C; // Another small item of 2 u8
};

#endif
