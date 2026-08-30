#ifndef M3DUTIL_M_ACTOR_HPP
#define M3DUTIL_M_ACTOR_HPP

#include <JSystem/J3D/J3DGraphAnimator/J3DNode.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <M3DUtil/MActorAnm.hpp>

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

enum {
	/// "Chain Keyframes" -- skeletal animation (softimage|3d terminology)
	ANM_TYPE_BCK = 0,
	/// "cLuster Keyframes" -- morph targets
	ANM_TYPE_BLK = 1,
	/// "Paint (???) Keyframes" -- material color animation
	ANM_TYPE_BPK = 2,
	/// "Texture Pattern" -- simply switches textures
	ANM_TYPE_BTP = 3,
	/// "Texture Keyframes" -- texture SRT animation
	ANM_TYPE_BTK = 4,
	/// "Register Keyframes" -- TEV C/K register animation
	ANM_TYPE_BRK = 5,

	ANM_TYPE_COUNT,
	ANM_TYPE_FIRST = ANM_TYPE_BCK,
};

class MActor {
public:
	MActor(MActorAnmData* anm_data);

	void setMActorAnmData(MActorAnmData*);
	void setModel(J3DModel*, u32);

	bool isCurAnmAlreadyEnd(int type);
	BOOL curAnmEndsNext(int type, char* part_name);
	BOOL curSubAnmEndsNext(int);
	void setAnimation(const char* name, int type);
	void initDL();
	void resetDL();
	void initDLByIndex(u16);
	void unlockDLIfNeed();
	void onMakeDL();
	void offMakeDL();
	const char* getCurAnmName(int) const;
	void setJointCallback(int, J3DNodeCallBack);
	void updateInSubBck();
	void updateOutSubBck();
	void calcAnm();
	void calc();
	void viewCalc();
	void loadSetDeformData(const char*);
	void setLightID(s16);
	void setLightData(const TBGCheckData*, const JGeometry::TVec3<f32>&);
	void setLightType(int);
	void update();
	void entry();
	void frameUpdate();
	void matAnmFrameUpdate();
	void perform(u32 cue, JDrama::TGraphics* graphics);
	BOOL checkCurAnm(const char* name, int type);
	bool checkCurAnmFromIndex(int index, int type);
	bool checkAnmFileExist(const char* name, int type);
	J3DFrameCtrl* getFrameCtrl(int type);
	BOOL checkBckPass(f32 pass_frame);
	int getCurAnmIdx(int type) const;
	void setFrameRate(f32 rate, int type);
	void setBck(const char* name);
	void setBckFromIndex(int index);
	void setSubBckFromIndex(int, int);
	BOOL checkCurBckFromIndex(int);
	void setSubBck(const char*, const char*);
	void setBpk(const char* name);
	void setBpkFromIndex(int index);
	void setBtp(const char* name);
	void setBtpFromIndex(int index);
	void setBtk(const char* name);
	void setBtkFromIndex(int index);
	void setBlk(const char* name);
	void setBlkFromIndex(int index);
	void setBrk(const char* name);
	void setBrkFromIndex(int index);

	void updateIn();
	void updateOut();
	void entryIn();
	void entryOut();
	void updateMatAnm();
	void dumpReport();

	// fabricated
	MActorAnmBase* getUnk28(int i) { return mAnmByType[i]; }
	MActorAnmBck* getAnmBck() { return mAnmBck; }
	J3DModel* getModel() const { return mModel; }
	void unmarkUnk40() { unk40 = false; }
	BOOL curAnmEndsNext() { return curAnmEndsNext(ANM_TYPE_BCK, nullptr); }

	// TODO: cleanup the names of all these stupid wrappers

	// fabricated
	void setCalcForBck(J3DMtxCalc* calc)
	{
		if (!mAnmBck)
			return;

		mAnmBck->setCalc(calc);
	}

	J3DAnmTransform* getBckOldMotionBlendAnmPtr() const
	{
		if (!mAnmBck)
			return nullptr;

		return mAnmBck->getOldMotionBlendAnmPtr();
	}

	void setBckOldMotionBlendAnmPtr(J3DAnmTransform* ptr)
	{
		if (!mAnmBck)
			return;

		mAnmBck->setOldMotionBlendAnmPtr(ptr);
	}

	f32 getBckOldMotionBlendFrame() const
	{
		if (!mAnmBck)
			return 0.0f;

		return mAnmBck->getOldMotionBlendFrame();
	}

	void setFrameCtrlForBck(int param_1) { mAnmBck->setFrameCtrl(param_1); }

	void setMotionBlendRatioForBck(f32 ratio)
	{
		if (!mAnmBck)
			return;

		mAnmBck->setMotionBlendRatio(ratio);
	}

	f32 getMotionBlendRatioForBck()
	{
		if (!mAnmBck)
			return 0.0f;

		return mAnmBck->getMotionBlendRatio();
	}

	void initSimpleMotionBlend(int frame)
	{
		if (!mAnmBck)
			return;

		mAnmBck->initSimpleMotionBlend(frame);
	}

	void initNormalMotionBlend()
	{
		if (!mAnmBck)
			return;

		mAnmBck->initNormalMotionBlend();
	}

	J3DAnmTransformKey* getCurBckAnmPtr()
	{
		if (!mAnmBck)
			return nullptr;

		return mAnmBck->unk24;
	}

	J3DAnmTransformKey* getBckAnm()
	{
		if (!mAnmBck)
			return nullptr;

		return mAnmBck->unk24;
	}

	void setBck(int index)
	{
		if (!checkCurBckFromIndex(index))
			setBckFromIndex(index);
	}

	void copyBckFrmCtrl(J3DFrameCtrl ctrl) { }

	void copyBtpFrmCtrl(J3DFrameCtrl ctrl) { }

public:
	/* 0x00 */ MActorAnmData* mAnmData;
	/* 0x04 */ J3DModel* mModel;
	/* 0x08 */ J3DMtxCalc* unk8;
	/* 0x0C */ MActorAnmBck* mAnmBck;
	/* 0x10 */ MActorAnmBck** unk10;
	/* 0x14 */ MActorAnmBpk* mAnmBpk;
	/* 0x18 */ MActorAnmBtp* mAnmBtp;
	/* 0x1C */ MActorAnmBtk* mAnmBtk;
	/* 0x20 */ MActorAnmBrk* mAnmBrk;
	/* 0x24 */ MActorAnmBlk* mAnmBlk;
	/* 0x28 */ MActorAnmBase** mAnmByType;
	/* 0x2C */ u16* unk2C;
	/* 0x30 */ u16* unk30;
	/* 0x34 */ int mMaterialNum;
	/* 0x38 */ bool mMakeDl;
	/* 0x39 */ bool unk39;
	/* 0x3C */ int mLightId;
	/* 0x40 */ bool unk40;
	/* 0x44 */ u32 unk44;
};

#endif
