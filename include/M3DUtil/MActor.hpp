#ifndef M3DUTIL_M_ACTOR_HPP
#define M3DUTIL_M_ACTOR_HPP

#include <JSystem/JDrama/JDRGraphics.hpp>

struct TBGCheckData;
class J3DNode;
class J3DModel;
class MActorAnmData;

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
	void checkCurBckFromIndex(int);
	void setSubBckFromIndex(int, int);
	void setBckFromIndex(int);
	void setBck(const char*);
	void setFrameRate(float, int);
	void getCurAnmIdx(int) const;
	void checkBckPass(float);
	void getFrameCtrl(int);
	void checkAnmFileExist(const char*, int);
	void checkCurAnmFromIndex(int, int);
	void checkCurAnm(const char*, int);
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
	void setJointCallback(int, int (*)(J3DNode*, int));
	void getCurAnmName(int) const;
	void offMakeDL();
	void onMakeDL();
	void unlockDLIfNeed();
	void initDLByIndex(unsigned short);
	void resetDL();
	void initDL();
	void setAnimation(const char*, int);
	void curSubAnmEndsNext(int);
	void curAnmEndsNext(int, char*);
	void isCurAnmAlreadyEnd(int);
	void setModel(J3DModel*, u32);
	void setMActorAnmData(MActorAnmData*);
};

#endif
