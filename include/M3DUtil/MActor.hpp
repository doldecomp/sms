#ifndef M3DUTIL_M_ACTOR_HPP
#define M3DUTIL_M_ACTOR_HPP

#include <JSystem/J3D/J3DGraphAnimator/J3DNode.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>

struct TBGCheckData;
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

	void dumpReport();
	void updateMatAnm();
	void entryOut();
	void entryIn();
	void updateOut();
	void updateIn();
	void setBrkFromIndex(int);
	void setBrk(const char*);
	void setBlkFromIndex(int);
	void setBlk(const char*);
	void setBtkFromIndex(int);
	void setBtk(const char*);
	void setBtpFromIndex(int);
	void setBtp(const char*);
	void setBpkFromIndex(int);
	void setBpk(const char*);
	void setSubBck(const char*, const char*);
	bool checkCurBckFromIndex(int);
	void setSubBckFromIndex(int, int);
	void setBckFromIndex(int);
	void setBck(const char*);
	void setFrameRate(float, int);
	int getCurAnmIdx(int) const;
	bool checkBckPass(float);
	J3DFrameCtrl* getFrameCtrl(int);
	bool checkAnmFileExist(const char*, int);
	bool checkCurAnmFromIndex(int, int);
	bool checkCurAnm(const char*, int);
	void perform(u32, JDrama::TGraphics*);
	void matAnmFrameUpdate();
	void frameUpdate();
	void entry();
	void update();
	void setLightType(int);
	void setLightData(const TBGCheckData*, const JGeometry::TVec3<f32>&);
	void setLightID(short);
	void loadSetDeformData(const char*);
	void viewCalc();
	void calc();
	void calcAnm();
	void updateOutSubBck();
	void updateInSubBck();
	void setJointCallback(int, J3DNodeCallBack);
	void getCurAnmName(int) const;
	void offMakeDL();
	void onMakeDL();
	void unlockDLIfNeed();
	void initDLByIndex(unsigned short);
	void resetDL();
	void initDL();
	void setAnimation(const char*, int);
	void curSubAnmEndsNext(int);
	bool curAnmEndsNext(int, char*);
	bool isCurAnmAlreadyEnd(int);
	void setModel(J3DModel*, u32);
	void setMActorAnmData(MActorAnmData*);

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
	/* 0x3C */ u32 unk3C;
	/* 0x40 */ u8 unk40;
	/* 0x44 */ u32 unk44;
};

#endif
