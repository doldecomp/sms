#ifndef M3DUTIL_M3U_MODEL_HPP
#define M3DUTIL_M3U_MODEL_HPP

#include <JSystem/JDrama/JDRGraphics.hpp>

struct M3UMtxCalcSetInfo;
class J3DModel;
class J3DFrameCtrl;
class J3DAnmBase;
class J3DAnmTexPattern;
class J3DTexNoAnm;

class M3UModelCommon {
public:
	virtual void getMtxCalc(const M3UMtxCalcSetInfo&);

public:
	/* 0x4 */ J3DAnmBase** unk4;
	/* 0x8 */ J3DAnmTexPattern** unk8;
	/* 0xC */ J3DTexNoAnm** unkC;
	/* 0x10 */ char unk10[0x8];
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

public:
	/* 0x4 */ M3UModelCommon* unk4;
	/* 0x8 */ J3DModel* unk8;
	/* 0xC */ J3DFrameCtrl* unkC;
	/* 0x10 */ u16 unk10;
	/* 0x14 */ u8* unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ u8* unk1C;
};

#endif
