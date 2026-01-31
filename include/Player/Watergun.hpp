#ifndef WATERGUN_HPP
#define WATERGUN_HPP

#include <Player/NozzleBase.hpp>
#include <Player/NozzleTrigger.hpp>
#include <Player/NozzleDeform.hpp>
#include <Player/MarioMain.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Strategic/MirrorActor.hpp>

class TWaterGunParams : public TParams {
public:
	TWaterGunParams(const char* prm)
	    : TParams(prm)
	    , PARAM_INIT(mRocketHeight, 1500.0f)
	    , PARAM_INIT(mHoverHeight, 160.0f)
	    , PARAM_INIT(mLAngleNormal, 60.0f)
	    , PARAM_INIT(mNozzleAngleYSpeed, 1.0f)
	    , PARAM_INIT(mNozzleAngleYBrake, 0.995f)
	    , PARAM_INIT(mNozzleAngleYSpeedMax, 0x2000)
	    , PARAM_INIT(mHoverRotMax, 0x2000)
	    , PARAM_INIT(mHoverSmooth, 0.05f)
	    , PARAM_INIT(mChangeSpeed, 0.1f) {};
	TParamRT<f32> mRocketHeight;
	TParamRT<f32> mHoverHeight;
	TParamRT<f32> mLAngleNormal;
	TParamRT<f32> mNozzleAngleYSpeed;
	TParamRT<f32> mNozzleAngleYBrake;
	TParamRT<s16> mNozzleAngleYSpeedMax;
	TParamRT<s16> mHoverRotMax;
	TParamRT<f32> mHoverSmooth;
	TParamRT<f32> mChangeSpeed;
};

struct NozzleJointData {
	u8 flags; // TODO: This is likely an enum. 0x1 is used, 0x4 is disabled.
	u8 jointIndex;
};

struct NozzleData {
	/* 0x00 */ u32 _00; // Number of water streams emitted by this nozzle
	/* 0x04 */ u32 _04; // Unused padding
	/* 0x08 */ const char* mHelmetPath; // Path to optional helmet model (used
	                                    // by underwater nozzle)
	/* 0x0C */ const char* mPath;       // Path to base nozzle model directory
	/* 0x10 */ const char* mBmdPath;    // Full path to nozzle's BMD model file
	/* 0x14 */ u8 mNumEmitters; // Number of water emitter points on the nozzle
	/* 0x15 */ u8 _15;          // Unknown/padding
	/* 0x16 */ NozzleJointData mJoints[3]; // Array of joint data for the nozzle
};

// Data structure containing model paths and configuration for each FLUDD nozzle
// type
class TNozzleBmdData {
public:
	u8 getEmitterCount(int index) const { return mNozzles[index].mNumEmitters; }

	u8 getFlags(int index, int jointIndex) const
	{
		return mNozzles[index].mJoints[jointIndex].flags;
	}

	u8 getJointIndex(int index, int jointIndex) const
	{
		return mNozzles[index].mJoints[jointIndex].jointIndex;
	}

	u8 setJointIndex(int index, int jointIndex, s32 value)
	{
		return mNozzles[index].mJoints[jointIndex].jointIndex = value;
	}

	const char* getPath(int index) const { return mNozzles[index].mPath; }
	const char* getBmdPath(int index) const { return mNozzles[index].mBmdPath; }

	NozzleData mNozzles[6];
};

extern TNozzleBmdData nozzleBmdData;

class TWaterGun {
public:
	// TODO: I wish these could be combined
	// If i make it a named enum, it defaults to 4 bytes size (i think)
	typedef s8 TNozzleType;
	enum { Spray = 0, Rocket, Underwater, Yoshi, Hover, Turbo };

	TWaterGun(TMario* mario);

	virtual void perform(u32, JDrama::TGraphics*);

	void changeBackup();
	void calcAnimation(JDrama::TGraphics*);
	void changeNozzle(TNozzleType, bool);
	bool damage();
	void emit();
	TNozzleBase* getCurrentNozzle() const;
	MtxPtr getEmitMtx(int);
	void getEmitPosDirSpeed(int, JGeometry::TVec3<f32>* pos,
	                        JGeometry::TVec3<f32>* dir,
	                        JGeometry::TVec3<f32>* speed);
	MtxPtr getNozzleMtx();
	f32 getPressure();
	f32 getPressureMax();
	void init();
	void initInLoadAfter();
	bool isEmitting();
	bool isPressureOn();
	void movement();
	void rotateProp(f32);
	void setAmountToRate(f32);
	void setBaseTRMtx(Mtx);
	bool suck();
	void triggerPressureMovement(const TMarioControllerWork&);

	J3DModel* getModel() { return mFluddModel->unk4; }

	// Fabricated
	// Probably inlined from Yoshi
	inline MtxPtr getYoshiMtx()
	{
		TYoshi* yoshi = (TYoshi*)mMario->mYoshi;
		return yoshi->mActor->unk4->getAnmMtx(yoshi->mJoint);
	}

	// Fabricated
	// TODO: Definitely not from watergun
	inline void playSoundWithInfo(u32 id, const Vec* pos, u32 _unk, f32 _unk2)
	{
		if (gpMSound->gateCheck(id)) {
			MSoundSESystem::MSoundSE::startSoundActorWithInfo(
			    id, pos, nullptr, _unk2, _unk, 0, nullptr, 0, 4);
		}
	}

	// Fabricated
	inline bool hasFlag(u16 flag)
	{
		bool hasFlag;
		if ((mFlags & flag) != 0) {
			hasFlag = true;
		} else {
			hasFlag = false;
		}
		return hasFlag;
	}

	// Fabricated
	s32 getSuckRate()
	{
		return mCurrentPressure
		       * getCurrentNozzle()->mEmitParams.mSuckRate.get();
	}

	// Fabricated
	TNozzleBase* getNozzle(u8 index) { return mNozzleList[index]; }

	// Fabricated
	bool hasWater() const { return mCurrentWater > 0; }

	/* 0x0000 */ u16 mFlags;
	/* 0x0008 */ TMario* mMario;
	/* 0x000C */ TNozzleDeform mNozzleDeform;
	/* 0x0720 */ TNozzleTrigger mNozzleRocket;
	/* 0x0AB0 */ TNozzleBase mNozzleUnderWater;
	/* 0x0E34 */ TNozzleDeform mNozzleYoshiDeform;
	/* 0x1548 */ TNozzleTrigger mNozzleHover;
	/* 0x18D8 */ TNozzleTrigger mNozzleTurbo;
	/* 0x1C68 */ TNozzleBase* mNozzleList[6];
	/* 0x1C80 */ s32 mCurrentWater;
	/* 0x1C84 */ u8 mCurrentNozzle;
	/* 0x1C85 */ u8 mSecondNozzle;
	/* 0x1C86 */ bool mIsEmitWater;
	/* 0x1C87 */ u8 unk1C87;
	/* 0x1C88 */ u32 unk1C88;
	/* 0x1C8C */ u8 mCurrentPressure;
	/* 0x1C8D */ u8 mPreviousPressure;
	/* 0x1C8E */ u8 unk1C8E;
	/* 0x1C8F */ u8 unk1C8F;
	/* 0x1C90 */ JGeometry::TVec3<f32> mEmitPos[4];
	/* 0x1CC0 */ s16 unk1CC0;
	/* 0x1CC2 */ s16 unk1CC2;
	/* 0x1CC4 */ s16 unk1CC4;
	/* 0x1CC6 */ s16 unk1CC6;
	/* 0x1CC8 */ f32 unk1CC8; // mNozzleSpeedY
	/* 0x1CCC */ f32 unk1CCC; // mNozzleSpeedZ
	/* 0x1CD0 */ s16 unk1CD0;
	/* 0x1CD2 */ s16 unk1CD2;
	/* 0x1CD4 */ MActor* mFluddModel; // MActor*
	/* 0x1CD8 */ u8 unk1CD8;          // mCurFluddTransformIdx
	/* 0x1CD9 */ u8 unk1CD9;
	/* 0x1CDA */ u16 unk1CDA;
	/* 0x1CDC */ f32 unk1CDC;
	/* 0x1CE0 */ f32 unk1CE0;
	/* 0x1CE4 */ u32 unk1CE4;
	/* 0x1CE8 */ u32 unk1CE8;
	/* 0x1CEC */ f32 unk1CEC;
	/* 0x1CF0 */ f32 unk1CF0;
	/* 0x1CF4 */ f32 unk1CF4;
	/* 0x1CF8 */ u16 unk1CF8;
	/* 0x1CFA */ u16 unk1CFA;
	/* 0x1CFC */ f32 unk1CFC; // mFluddSwitchTween
	/* 0x1D00 */ f32 unk1D00;
	/* 0x1D04 */ s16 unk1D04;
	/* 0x1D06 */ s16 unk1D06;
	/* 0x1D08 */ u32 unk1D08;
	/* 0x1D0C */ TWaterEmitInfo* mEmitInfo; // TWaterEmitInfo
	/* 0x1D10 */ TMirrorActor* unk1D10;
	/* 0x1D14 */ TWaterGunParams mWatergunParams;
};

#endif
