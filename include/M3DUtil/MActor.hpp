#ifndef M3DUTIL_M_ACTOR_HPP
#define M3DUTIL_M_ACTOR_HPP

#include <JSystem/J3D/J3DGraphAnimator/J3DNode.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>

class TBGCheckData;
class J3DNode;
class J3DModel;
class MActorAnmData;
class J3DFrameCtrl;
class J3DMtxCalc;

class MActorAnmBase;
class MActorAnmBck;
class MActorAnmBpk;
class MActorAnmBtp;
class MActorAnmBtk;
class MActorAnmBrk;
class MActorAnmBlk;

class MActor {
public:
	MActor(MActorAnmData*);

	void setMActorAnmData(MActorAnmData*);
	void setModel(J3DModel*, u32);
	bool isCurAnmAlreadyEnd(int);
	BOOL curAnmEndsNext(int, char*);
	void curSubAnmEndsNext(int);
	void setAnimation(const char*, int);
	void initDL();
	void resetDL();
	void initDLByIndex(unsigned short);
	void unlockDLIfNeed();
	void onMakeDL();
	void offMakeDL();
	void getCurAnmName(int) const;
	void setJointCallback(int, J3DNodeCallBack);
	void updateInSubBck();
	void updateOutSubBck();
	void calcAnm();
	void calc();
	void viewCalc();
	void loadSetDeformData(const char*);
	void setLightID(short);
	void setLightData(const TBGCheckData*, const JGeometry::TVec3<f32>&);
	void setLightType(int);
	void update();
	void entry();
	void frameUpdate();
	void matAnmFrameUpdate();
	void perform(u32, JDrama::TGraphics*);
	bool checkCurAnm(const char*, int);
	bool checkCurAnmFromIndex(int, int);
	bool checkAnmFileExist(const char*, int);
	J3DFrameCtrl* getFrameCtrl(int);
	bool checkBckPass(float);
	int getCurAnmIdx(int) const;
	void setFrameRate(float, int);
	void setBck(const char*);
	void setBckFromIndex(int);
	void setSubBckFromIndex(int, int);
	bool checkCurBckFromIndex(int);
	void setSubBck(const char*, const char*);
	void setBpk(const char*);
	void setBpkFromIndex(int);
	void setBtp(const char*);
	void setBtpFromIndex(int);
	void setBtk(const char*);
	void setBtkFromIndex(int);
	void setBlk(const char*);
	void setBlkFromIndex(int);
	void setBrk(const char*);
	void setBrkFromIndex(int);
	void updateIn();
	void updateOut();
	void entryIn();
	void entryOut();
	void updateMatAnm();
	void dumpReport();

	// fabricated
	MActorAnmBase* getUnk28(int i) { return unk28[i]; }
	J3DModel* getUnk4() { return unk4; }
	void unmarkUnk40() { unk40 = false; }

public:
	/* 0x00 */ MActorAnmData* unk0;
	/* 0x04 */ J3DModel* unk4;
	/* 0x08 */ J3DMtxCalc* unk8;
	/* 0x0C */ MActorAnmBck* unkC;
	/* 0x10 */ MActorAnmBck** unk10;
	/* 0x14 */ MActorAnmBpk* unk14;
	/* 0x18 */ MActorAnmBtp* unk18;
	/* 0x1C */ MActorAnmBtk* unk1C;
	/* 0x20 */ MActorAnmBrk* unk20;
	/* 0x24 */ MActorAnmBlk* unk24;
	/* 0x28 */ MActorAnmBase** unk28;
	/* 0x2C */ u16* unk2C;
	/* 0x30 */ u16* unk30;
	/* 0x34 */ int unk34;
	/* 0x38 */ bool unk38;
	/* 0x39 */ u8 unk39;
	/* 0x3C */ int unk3C;
	/* 0x40 */ u8 unk40;
	/* 0x44 */ u32 unk44;
};

#endif
