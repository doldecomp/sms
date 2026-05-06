#ifndef PLAYER_TONGUE_HPP
#define PLAYER_TONGUE_HPP

#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <Strategic/TakeActor.hpp>

class TYoshi;
class J3DModel;

class TYoshiTongue : public TTakeActor {
public:
	TYoshiTongue(const char* name = "HitActor")
	    : TTakeActor(name)
	{
	}

	virtual MtxPtr getTakingMtx() { return mTipModel->getAnmMtx(0); }
	virtual void init(TYoshi*);
	virtual void movement();
	virtual void checkTaking() { }
	virtual void checkTaken() { }

	void initInLoadAfter();
	void emit(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
	          const JGeometry::TVec3<f32>&);
	void rest(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&);
	BOOL canGo();
	THitActor* findTarget(bool, bool);
	void calcAnim(MtxPtr);
	void viewCalc();
	void entry();

public:
	enum {
		STATE_IDLE,
		STATE_EXTENDING,
		STATE_GRABBED,
		STATE_RETRACTING,
		STATE_UNK4,
		STATE_UNK5,
		STATE_PULLING,
		STATE_PULLING_SLOW,
	};

	/* 0x70 */ TYoshi* mYoshi;
	/* 0x74 */ J3DModel* mModel;
	/* 0x78 */ J3DModel* mTipModel;
	/* 0x7C */ u16 mState;
	/* 0x7E */ u16 mProgress;
	/* 0x80 */ s16 mMaxProgress;
	/* 0x82 */ s16 unk82;
	/* 0x84 */ f32 mInitialSpeed;
	/* 0x88 */ f32 mExtendAmount;
	/* 0x8C */ f32 mRetractAmount;
	/* 0x90 */ f32 mPullAmount;
	/* 0x94 */ f32 mRetractedLength;
	/* 0x98 */ f32 mMaxReach;
	/* 0x9C */ f32 mElasticity;
	/* 0xA0 */ JGeometry::TVec3<f32> mHeadPos;
	/* 0xAC */ JGeometry::TVec3<f32> mHeadDir;
	/* 0xB8 */ JGeometry::TVec3<f32> mTipPos;
	/* 0xC4 */ JGeometry::TVec3<f32> mInitialVelocity;
	/* 0xD0 */ u32 mActorTypeInMouth;
	/* 0xD4 */ u16 unkD4;
	/* 0xD6 */ u16 mSavedColCount;
};

#endif
