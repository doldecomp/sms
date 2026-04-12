#ifndef NPC_BASE_NPC_HPP
#define NPC_BASE_NPC_HPP

#include <Enemy/Enemy.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <M3DUtil/LodAnm.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <NPC/NpcAnmKind.hpp>
#include <NPC/NpcSave.hpp>
#include <Camera/Camera.hpp>

struct TNpcInitInfo;
class SDLModel;
class TMultiMtxEffect;

class TNpcSaveNormal;
// TODO: should this be here?
enum EnumNpcStopMotionBlendOnOff {
	NPC_STOP_MOTION_BLEND_OFF = 0,
	NPC_STOP_MOTION_BLEND_ON  = 1,
};
enum EnumHitNpcObjectKind {
	HIT_NPC_OBJECT_KIND_WATER_SPRAY = 0,
	HIT_NPC_OBJECT_KIND_UNK1        = 1,
	HIT_NPC_OBJECT_KIND_UNK2        = 2,
};

class TNpcParts;
class TNpcSaveIndividual;
class TNpcThrow;
class TNpcTrample;
class TNpcCoin;
class TNpcBalloon;
class TNpcInbetween;

// "Delayed" animation that will be applied after motion blending is done.
struct TNpcKeepAnm {
	TNpcKeepAnm()
	    : mKind(NPC_ANM_KIND_INVALID)
	    , mBlendOn(0)
	{
	}

	void reset() { mKind = NPC_ANM_KIND_INVALID; }
	void keep(EnumNpcAnmKind anm, EnumNpcStopMotionBlendOnOff blend)
	{
		mKind    = anm;
		mBlendOn = blend != NPC_STOP_MOTION_BLEND_OFF;
	}

	EnumNpcAnmKind getKind() const { return mKind; }
	EnumNpcStopMotionBlendOnOff getBlend() const
	{
		return (EnumNpcStopMotionBlendOnOff)mBlendOn;
	}

private:
	/* 0x0 */ EnumNpcAnmKind mKind;
	/* 0x4 */ bool mBlendOn;
};

class TBaseNPC : public TSpineEnemy {
public:
	TBaseNPC(u32, const char* name = "?");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual void moveObject();
	virtual void kill();
	virtual JGeometry::TVec3<f32> getFocalPoint() const;
	virtual const char** getBasNameTable() const;

	bool isCanWalk() const;
	bool isNeedTurnToFirstState() const;
	bool isTurnToMarioWhenTalk() const;
	bool isTurnToMarioWhenApproach() const;
	bool isNerveWalk() const;
	bool isNerveMaybeDontMovement() const;
	bool isNerveMaybeDontCalcAnim0() const;
	bool isNerveMaybeDontCalcAnim1() const;
	bool isNerveCanGoToTalk() const;
	bool isNerveCanGoToWet() const;
	bool isNerveCanGoToSink() const;
	bool isNerveCanGoToTaken() const;
	bool isNerveCanGoToThrow() const;
	bool isNerveCanGoToMad() const;
	bool isNerveCanGoToBlown() const;

	bool isNormalMonteM() const;
	bool isNormalMonteW() const;
	bool isNormalMonte() const { return isNormalMonteM() || isNormalMonteW(); }
	bool isSpecialMonteM() const;
	bool isSpecialMonteW() const;
	bool isSpecialMonte() const
	{
		return isSpecialMonteM() || isSpecialMonteW();
	}
	bool isMonte() const { return isNormalMonte() || isSpecialMonte(); }
	bool isMonteM() const { return isNormalMonteM() || isSpecialMonteM(); }
	bool isMonteW() const { return isNormalMonteW() || isSpecialMonteW(); }

	bool isNormalMareM() const;
	bool isNormalMareW() const;
	bool isNormalMare() const { return isNormalMareM() || isNormalMareW(); }
	bool isSpecialMareM() const;
	bool isSpecialMareW() const;
	bool isSpecialMare() const { return isSpecialMareM() || isSpecialMareW(); }
	bool isMare() const { return isNormalMare() || isSpecialMare(); }
	bool isMareM() const { return isNormalMareM() || isSpecialMareM(); }
	bool isMareW() const { return isNormalMareW() || isSpecialMareW(); }

	bool isJellyFishMare() const;
	bool isSunflower() const;
	bool isChild() const;
	bool isSmallNpc() const;
	bool isPollutionNpc() const;
	bool isBeTrampledNpc() const;
	bool isMadNpc() const;
	bool isBehaveToWaterNpc() const;
	bool isBehaveToHitNpc() const;
	bool isPartsAnmNpc() const;
	bool isNeedNeckStraight() const;
	bool isInBodyTurnSearchRange() const;
	bool isInMadSearchRange() const;
	bool isNowCanTaken() const;

	void execWalk(bool);
	bool execUTurn();
	bool execTurnToFirstState();
	void setPosAndInitAfterSinkBottom();

	JGeometry::TVec3<f32> getCursorPos() const;
	void setDummyConnectActor(const JDrama::TActor*);
	void setBalloonMessage(u32, long);
	const GXColor* getPtrInitPollutionColor() const;
	void isNowMotionBlend() const;
	void offStopMotionBlend();
	void onStopMotionBlend();
	void npcWaitIn();
	void npcFallIn();
	bool npcRecoverFromSinking();
	void npcRecoverAfterIn();

	void npcStepIn();
	void npcTalkIn();
	void npcTalking();
	void npcTalkOut();
	void npcTakenIn();
	void npcDanceIn();
	void npcHappyIn(u8);
	void npcWetIn();
	bool npcWetting();
	void npcWetOut();
	void npcSinking();
	void npcThrowIn();
	bool npcThrowing();
	void npcMadIn();
	bool npcMadding();
	void npcBlownIn();
	bool npcBlowning();
	void npcMareStandIn();
	bool npcMareStanding();
	void sunflowerReviveIn();
	bool sunflowerReviving();
	void monteMESetAnmWhenFar();
	void monteMESetAnmWhenNear();

	static TNpcSaveNormal* mPtrSaveNormal;
	static s16 mAngleYDiffWhenTaken;

	// fabricated
	void onUnk1D8(u32 flag) { unk1D8 |= flag; }
	void offUnk1D8(u32 flag) { unk1D8 &= ~flag; }
	bool checkUnk1D8(u32 flag) const { return (unk1D8 & flag) != 0; }

	void onUnk1DA(u32 flag) { unk1DA |= flag; }
	void offUnk1DA(u32 flag) { unk1DA &= ~flag; }
	bool checkUnk1DA(u32 flag) const { return (unk1DA & flag) != 0; }

	void onActionFlag(u32 flag) { mActionFlag |= flag; }
	void offActionFlag(u32 flag) { mActionFlag &= ~flag; }
	bool checkActionFlag(u32 flag) const { return (mActionFlag & flag) != 0; }

	bool isSunflowerReviving() const
	{
		bool result = false;
		if (isSunflower() && (unk1D8 & UNK1D8_FLAG_UNK2))
			result = true;
		return result;
	}

	bool isPeachTired() const
	{
		bool result = false;
		if (mActorType == 0x4000018 && (unk1D8 & UNK1D8_FLAG_UNK2))
			result = true;
		return result;
	}

	bool isClean() const { return mPollutionAmount == 0.0f; }

private:
	void setIndividualDifference_(JSUMemoryInputStream&);
	void initIndividualAnm_();
	void initBaseActionFlag_();
	void initNpcLight_();
	void setMtxEffect_();
	void initSinkNpc_();
	void changeNerveFromTalk_();
	void changeNerveToWet_();
	void changeNerveToMad_();
	void releaseTaken_();
	void behaveToBeTaken_(THitActor*);
	void behaveToBeTrampled_();
	void behaveToHitObject_(THitActor*, EnumHitNpcObjectKind);
	void behaveToSandBomb_(const TLiveActor*);
	bool isStateGoToMad_() const;
	void changeNerveProc_();
	void execMotionBlend_();
	void movementOnlyTalk_(const JDrama::TGraphics*);
	void updateForbidCount_()
	{
		(void)0;
		if (mTalkForbidCount != 0)
			mTalkForbidCount -= 1;
		if (mWalkForbidCount != 0)
			mWalkForbidCount -= 1;
		if (mDamageParticleForbidCount != 0)
			mDamageParticleForbidCount -= 1;
	}
	f32 getAnmOffDist_()
	{
		bool bVar3 = false;
		f32 fVar1  = gpCamera->mFar;
		u32 uVar5  = unkD0->getCurrentAnmKind();
		f32 fVar2  = mPtrSaveNormal->mSLDanceAnmOffDist.get();
		if (checkActionFlag(NPC_ACTION_HAPPY | NPC_ACTION_DANCE)
		    || mActorType == 0x400000D || uVar5 || uVar5 == 23) {
			bVar3 = true;
		}

		if (!isNerveMaybeDontCalcAnim0() && !isNerveMaybeDontCalcAnim1()) {
			fVar1 = mIndividualParams->mWaitAnmOffDist1.get();
			if (bVar3 && fVar1 < fVar2)
				fVar1 = fVar2;
		} else {
			fVar1 = mIndividualParams->mWaitAnmOffDist0.get();
			if (bVar3 && fVar1 < fVar2)
				fVar1 = fVar2;
		}

		return fVar1;
	}
	void setNpcAnm_(EnumNpcAnmKind, EnumNpcStopMotionBlendOnOff);
	void requestNpcAnm_(EnumNpcAnmKind, EnumNpcStopMotionBlendOnOff);
	void setKeepAnm_();
	void requestTalkAnm_();
	void randomizeBckAndBtpFrame_();
	void walkAnmRateChange_();
	EnumNpcAnmKind getNpcWaitAnmBase_();
	void initNpcObjCollision_(const TNpcInitInfo*);
	void execNpcObjCollision_();
	void setVariableDamageRadius_();
	void setMonteActionFlag_();
	void setMareActionFlag_();
	void setKinoActionFlag_();
	void setHappyEffectMtxPtr_(const JUTNameTab*);
	void setNoteEffectMtxPtr_(const JUTNameTab*);
	void setPollutionEffectMtxPtr_(const JUTNameTab*);
	void setSmokeEffectMtxPtr_(bool);
	JGeometry::TVec3<f32> getEffectScale_() const;
	void emitSinkEffect_();
	void emitHappyEffect_();
	void emitPollutionParticle_(int, MtxPtr); // fabricated
	void emitDirtyEffect_();                  // fabricated
	void emitWashEffect_();                   // fabricated
	bool isPolWaitCEffectEmitTime_() const;
	bool isPolWaitLEffectEmitTime_() const;
	bool isPolWaitREffectEmitTime_() const;
	void emitParticle_();

	void peachParasolIn_();
	void peachParasolOut_();
	void peachTiredIn_();
	void peachTiredOut_();
	void sunflowerDownIn_();

	// fabricated
	void resetToWait_()
	{
		mMarchSpeed = 0.0f;
		mTurnSpeed  = mIndividualParams->mWaitTurnSpeed.get();
		unk1CC      = 0;
		unk1D0      = 0.0f;
	}
	void resetToTurn_()
	{
		mMarchSpeed  = 0.0f;
		mTurnSpeed   = mIndividualParams->mWaitTurnSpeed.get();
		unk1CC       = 0;
		unk1D0       = 0.0f;
		unk22C->unk0 = 0;
	}

	bool isExtinguished_() const { return mBurnStrength <= 0.0f; }

public:
	/* 0x150 */ SDLModel* unk150;
	/* 0x154 */ MtxPtr unk154;
	/* 0x158 */ TTakeActor* unk158;

	// fabricated
	class TNpcSink {
	public:
		TNpcSink(int param_1, int param_2)
		    : unk0(param_1)
		    , unk4(param_2)
		{
		}

		bool doThing()
		{
			bool result = false;
			unk0 += 1;
			if (unk0 >= unk4) {
				unk0   = unk4;
				result = true;
			}
			return result;
		}

		/* 0x0 */ int unk0;
		/* 0x4 */ int unk4;
	};

	/* 0x15C */ TNpcSink* unk15C;
	/* 0x160 */ TMultiMtxEffect* mMultiMtxEffect;
	/* 0x164 */ int mNeckJointIndex;
	/* 0x168 */ TNpcParts* unk168;
	/* 0x16C */ s32 unk16C;

	enum {
		NPC_ACTION_UNK1    = 0x1,
		NPC_ACTION_UNK2    = 0x2,
		NPC_ACTION_DANCE   = 0x4,
		NPC_ACTION_RUN     = 0x8,
		NPC_ACTION_UNK10   = 0x10,
		NPC_ACTION_UNK20   = 0x20,
		NPC_ACTION_UNK40   = 0x40,
		NPC_ACTION_UNK80   = 0x80,
		NPC_ACTION_UNK100  = 0x100,
		NPC_ACTION_HAPPY   = 0x200,
		NPC_ACTION_UNK400  = 0x400,
		NPC_ACTION_UNK800  = 0x800,
		NPC_ACTION_UNK1000 = 0x1000,
		NPC_ACTION_UNK2000 = 0x2000,
		NPC_ACTION_BURNING = 0x4000,
	};

	/* 0x170 */ u32 mActionFlag;
	/* 0x174 */ GXColor unk174;
	/* 0x178 */ f32 mPollutionAmount;
	/* 0x17C */ TNpcThrow* mThrowCtrl;
	/* 0x180 */ TNpcTrample* mTrampleCtrl;
	/* 0x184 */ TNpcCoin* mCoinCtrl;
	/* 0x188 */ TNpcBalloon* mBalloonCtrl;
	/* 0x18C */ TNpcInbetween* mInbetweenCtrl;
	/* 0x190 */ TNpcKeepAnm* mKeepAnmCtrl;
	/* 0x194 */ JGeometry::TVec3<f32> unk194;
	/* 0x1A0 */ JGeometry::TVec3<f32> unk1A0;
	/* 0x1AC */ JGeometry::TVec3<f32> mInitialScale;
	/* 0x1B8 */ JGeometry::TVec3<f32> unk1B8;
	/* 0x1C4 */ f32 unk1C4;
	/* 0x1C8 */ f32 unk1C8;
	/* 0x1CC */ int unk1CC;
	/* 0x1D0 */ f32 unk1D0;
	/* 0x1D4 */ const JDrama::TActor* mDummyConnectActor;

	enum {
		UNK1D8_FLAG_UNK1 = 0x1,
		UNK1D8_FLAG_UNK2 = 0x2,
		UNK1D8_FLAG_UNK4 = 0x4,
	};

	/* 0x1D8 */ u8 unk1D8;
	/* 0x1D9 */ u8 unk1D9;

	enum {
		UNK1DA_FLAG_UNK1 = 0x1,
		UNK1DA_FLAG_UNK2 = 0x2,
	};

	/* 0x1DA */ u8 unk1DA;
	/* 0x1DC */ int unk1DC;
	/* 0x1E0 */ u16 mTalkForbidCount;
	/* 0x1E2 */ u16 mWalkForbidCount;
	/* 0x1E4 */ u16 mDamageParticleForbidCount;
	/* 0x1E8 */ MtxPtr mHappyEffectMtxPtr;
	/* 0x1EC */ MtxPtr mNoteEffectMtxPtr;
	/* 0x1F0 */ JGeometry::TVec3<f32> unk1F0;
	/* 0x1FC */ MtxPtr mPollutionEffectMtxPtr;
	/* 0x200 */ MtxPtr unk200;
	/* 0x204 */ MtxPtr unk204;
	/* 0x208 */ MtxPtr mSmokeEffectMtxPtr;
	/* 0x20C */ JGeometry::TVec3<f32> mFireParticlePos;
	/* 0x218 */ f32 mBurnStrength;
	/* 0x21C */ JGeometry::TVec3<f32> mWaveParticlePos;
	/* 0x228 */ TNpcSaveIndividual* mIndividualParams;

	class TNpcUnk22CStruct {
	public:
		TNpcUnk22CStruct()
		    : unk0(0)
		    , unk4(1)
		{
		}

		void doThing()
		{
			int iVar13 = mPtrSaveNormal->mSLGraphWanderMinFrame.get();
			int sVar1  = mPtrSaveNormal->mSLGraphWanderMaxFrame.get();

			unk0 = 0;
			unk4 = MsRandI(sVar1, iVar13);
		}

		bool doThing2()
		{
			bool result = false;
			unk0 += 1;
			if (unk0 >= unk4) {
				unk0   = unk4;
				result = true;
			}
			return result;
		}

		void doThing3(int l, int r)
		{
			if (unk0 == 0) {
				unk0 = 0;
				unk4 = MsRandI(l, r);
			}
		}

		/* 0x0 */ int unk0;
		/* 0x4 */ int unk4;
	};

	/* 0x22C */ TNpcUnk22CStruct* unk22C;

	class TNpcUnk230Struct {
	public:
		TNpcUnk230Struct()
		    : unk0(0)
		    , unk2(0)
		{
		}

		void set(s16 x, s16 y)
		{
			unk0 = x;
			unk2 = y;
		}

		/* 0x0 */ s16 unk0;
		/* 0x2 */ s16 unk2;
	};

	/* 0x230 */ TNpcUnk230Struct* unk230;
};

extern TBaseNPC* gpCurrentNpc;

#endif
