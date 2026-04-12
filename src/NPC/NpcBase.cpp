#include <NPC/NpcBase.hpp>
#include <System/MarDirector.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <M3DUtil/LodAnm.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MapUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MSound/MSound.hpp>
#include <System/Particles.hpp>
#include <Strategic/Spine.hpp>
#include <NPC/NpcManager.hpp>
#include <NPC/NpcParts.hpp>
#include <NPC/NpcBalloon.hpp>
#include <NPC/NpcTrample.hpp>
#include <NPC/NpcNerve.hpp>
#include <NPC/NpcInbetween.hpp>
#include <NPC/NpcSave.hpp>
#include <NPC/NpcCoin.hpp>
#include <Player/MarioAccess.hpp>
#include <Enemy/Conductor.hpp>
#include <Camera/Camera.hpp>
#include <Camera/cameralib.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TNpcSaveNormal* TBaseNPC::mPtrSaveNormal = nullptr;
s16 TBaseNPC::mAngleYDiffWhenTaken       = 0;
TBaseNPC* gpCurrentNpc                   = nullptr;

TBaseNPC::TBaseNPC(u32 param_1, const char* name)
    : TSpineEnemy(name)
    , unk150(nullptr)
    , unk154(nullptr)
    , unk158(nullptr)
    , unk15C(nullptr)
    , mMultiMtxEffect(nullptr)
    , mNeckJointIndex(-1)
    , unk168(nullptr)
    , unk16C(0)
    , mActionFlag(0)
    , mPollutionAmount(0.0f)
    , mThrowCtrl(nullptr)
    , mTrampleCtrl(nullptr)
    , mCoinCtrl(nullptr)
    , mBalloonCtrl(nullptr)
    , mInbetweenCtrl(nullptr)
    , mKeepAnmCtrl(nullptr)
    , unk1C4(0.0f)
    , unk1C8(0.0f)
    , unk1CC(0)
    , unk1D0(0.0f)
    , mDummyConnectActor(nullptr)
    , unk1D8(0)
    , unk1D9(0)
    , unk1DA(0)
    , unk1DC(0)
    , mTalkForbidCount(120)
    , mWalkForbidCount(0)
    , mDamageParticleForbidCount(0)
    , mHappyEffectMtxPtr(nullptr)
    , mNoteEffectMtxPtr(nullptr)
    , unk1F0(0.0f, 0.0f, 0.0f)
    , mPollutionEffectMtxPtr(nullptr)
    , unk200(nullptr)
    , unk204(nullptr)
    , mSmokeEffectMtxPtr(nullptr)
    , mFireParticlePos(0.0f, 0.0f, 0.0f)
    , mBurnStrength(1.0f)
    , mWaveParticlePos(0.0f, 0.0f, 0.0f)
    , unk22C(nullptr)
    , unk230(nullptr)
{
	gpCurrentNpc         = nullptr;
	mAngleYDiffWhenTaken = 0;

	unk174.r = 255;
	unk174.g = 255;
	unk174.b = 255;
	unk174.a = 0;

	mActorType = param_1;

	if (param_1 != 0x400001C) {
		mKeepAnmCtrl = new TNpcKeepAnm;

		if (isBeTrampledNpc())
			mTrampleCtrl = new TNpcTrample;
	}
}

void TBaseNPC::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);
	unk194        = mPosition;
	unk1A0        = mRotation;
	mInitialScale = mScaling;
	unk1B8        = getFocalPoint();

	if (mActorType >= 0x400001E || mActorType < 0x400001C)
		setIndividualDifference_(stream);
}

void TBaseNPC::loadAfter()
{
	TSpineEnemy::loadAfter();
	if (mActorType == 0x4000018 && gpMarDirector->getCurrentMap() == 1
	    && gpMarDirector->getCurrentStage() == 1) {
		mBalloonCtrl = new TNpcBalloon;
	}
	gpMarDirector->entryNPC(this);
}

bool TBaseNPC::isNormalMonteM() const
{
	bool result = false;
	switch (mActorType) {
	case 0x4000001:
	case 0x4000002:
	case 0x4000003:
	case 0x4000004:
	case 0x4000005:
		result = true;
		break;
	}
	return result;
}

bool TBaseNPC::isNormalMonteW() const
{
	bool result = false;
	switch (mActorType) {
	case 0x400000A:
	case 0x400000B:
	case 0x400000C:
		result = true;
		break;
	}
	return result;
}

bool TBaseNPC::isSpecialMonteM() const
{
	bool result = false;
	switch (mActorType) {
	case 0x4000006:
	case 0x4000007:
	case 0x4000008:
	case 0x4000009:
		result = true;
		break;
	}
	return result;
}

bool TBaseNPC::isSpecialMonteW() const
{
	bool result = false;
	switch (mActorType) {
	case 0x400000D:
		result = true;
		break;
	}
	return result;
}

bool TBaseNPC::isNormalMareM() const
{
	bool result = false;
	switch (mActorType) {
	case 0x400000E:
		result = true;
		break;
	}
	return result;
}

bool TBaseNPC::isNormalMareW() const
{
	bool result = false;
	switch (mActorType) {
	case 0x4000013:
		result = true;
		break;
	}
	return result;
}

bool TBaseNPC::isSpecialMareM() const
{
	bool result = false;
	switch (mActorType) {
	case 0x400000F:
	case 0x4000010:
	case 0x4000011:
	case 0x4000012:
		result = true;
		break;
	}
	return result;
}

bool TBaseNPC::isSpecialMareW() const
{
	bool result = false;
	switch (mActorType) {
	case 0x4000014:
	case 0x4000015:
		result = true;
		break;
	}
	return result;
}

bool TBaseNPC::isJellyFishMare() const
{
	bool result = false;
	switch (mActorType) {
	case 0x4000014:
	case 0x400000F:
		result = true;
		break;
	}
	return result;
}

bool TBaseNPC::isSunflower() const
{
	bool result = false;
	switch (mActorType) {
	case 0x400001A:
	case 0x400001B:
		result = true;
		break;
	}
	return result;
}

bool TBaseNPC::isChild() const
{
	bool result = false;
	if (mScaling.x < 0.7f && mScaling.y < 0.7f && mScaling.z < 0.7f)
		result = true;
	return result;
}

bool TBaseNPC::isSmallNpc() const
{
	bool result = false;
	if (isChild() || isMare())
		result = true;
	else
		switch (mActorType) {
		case 0x4000016:
		case 0x4000017:
			result = true;
			break;
		}
	return result;
}

bool TBaseNPC::isPollutionNpc() const
{
	bool result = false;

	switch (mActorType) {
	case 0x4000001:
	case 0x4000002:
	case 0x4000004:
	case 0x400000A:
	case 0x400000B:
	case 0x400000E:
	case 0x4000013:
	case 0x4000016:
		result = true;
		break;
	}

	return result;
}

bool TBaseNPC::isBeTrampledNpc() const
{
	bool result = false;

	if (!isJellyFishMare()) {
		if (isMonte() || isMare()) {
			result = true;
		} else {
			switch (mActorType) {
			case 0x4000016:
			case 0x4000017:
				result = true;
				break;
			}
		}
	}

	return result;
}

bool TBaseNPC::isMadNpc() const
{
	bool result = false;
	if (isNormalMonte())
		result = true;
	else {
		switch (mActorType) {
		case 0x4000006:
		case 0x4000007:
		case 0x400000D:
			result = true;
			break;
		}
	}
	return result;
}

bool TBaseNPC::isBehaveToWaterNpc() const
{
	bool result = true;
	switch (mActorType) {
	case 0x4000007:
	case 0x4000008:
	case 0x400000F:
	case 0x4000014:
	case 0x400001C:
	case 0x400001D:
		result = false;
		break;
	}
	return result;
}

bool TBaseNPC::isBehaveToHitNpc() const
{
	bool result = false;

	if (isMadNpc()) {
		result = true;
	} else {
		switch (mActorType) {
		case 0x400000E:
		case 0x4000010:
		case 0x4000011:
		case 0x4000013:
		case 0x4000015:
		case 0x4000016:
		case 0x4000017:
			result = true;
		}
	}

	return result;
}

bool TBaseNPC::isPartsAnmNpc() const
{
	bool result = false;
	if (isJellyFishMare()) {
		result = true;
	} else {
		switch (mActorType) {
		case 0x4000010:
		case 0x4000015:
		case 0x4000018:
			result = true;
			break;
		}
	}
	return result;
}

bool TBaseNPC::isNeedNeckStraight() const
{
	bool result = false;
	int anmKind = unkD0->getCurrentAnmKind();
	if ((mHolder != nullptr && mHolder == gpMarioAddress) || !isClean()
	    || mActorType == 0x4000012
	    || (mActorType == 0x4000019 && anmKind == NPC_ANM_KIND_UNK5)
	    || (isMare() && anmKind == NPC_ANM_KIND_UNKC)
	    || ((checkUnk1D8(UNK1D8_FLAG_UNK2) || anmKind == NPC_ANM_KIND_UNK5)
	        & (mActorType == 0x4000018))) {
		result = true;
	}
	return result;
}

bool TBaseNPC::isInBodyTurnSearchRange() const
{
	bool result = false;
	if (abs(SMS_GetMarioPos().y - mPosition.y)
	        < mIndividualParams->mBodyTurnSearchHeight.get()
	    && isInSight(SMS_GetMarioPos(),
	                 mIndividualParams->mBodyTurnSearchDist.get(),
	                 mIndividualParams->mBodyTurnSearchDegree.get(),
	                 mIndividualParams->mBodyTurnSearchAware.get()))
		result = true;
	return result;
}

bool TBaseNPC::isInMadSearchRange() const
{
	bool result = false;
	if (abs(SMS_GetMarioPos().y - mPosition.y)
	        < mIndividualParams->mMadSearchHeight.get()
	    && isInSight(SMS_GetMarioPos(), mIndividualParams->mMadSearchDist.get(),
	                 mIndividualParams->mMadSearchDegree.get(),
	                 mIndividualParams->mMadSearchAware.get()))
		result = true;
	return result;
}

JGeometry::TVec3<f32> TBaseNPC::getCursorPos() const
{
	return JGeometry::TVec3<f32>(
	    mPosition.x,
	    mPosition.y + mInitialScale.y * mIndividualParams->mSLBodyHeight.get()
	        + mIndividualParams->mSLCursorHeight.get(),
	    mPosition.z);
}

JGeometry::TVec3<f32> TBaseNPC::getFocalPoint() const
{
	return JGeometry::TVec3<f32>(
	    mPosition.x,
	    mPosition.y
	        + mInitialScale.y
	              * (mIndividualParams->mSLBodyHeight.get()
	                 - mIndividualParams->mSLLookatHeight.get()),
	    mPosition.z);
}

BOOL TBaseNPC::receiveMessage(THitActor* param_1, u32 param_2)
{
	bool result = false;

	switch (mActorType) {
	case 0x400001C:
		break;

	default:
		if (param_2 == HIT_MESSAGE_TAKE && checkLiveFlag(LIVE_FLAG_UNK100000)
		    && mHolder == nullptr) {
			behaveToBeTaken_(param_1);
			result = true;
		} else if (param_2 == HIT_MESSAGE_TRAMPLE) {
			behaveToBeTrampled_();
			result = true;
		} else if (param_2 == HIT_MESSAGE_SPRAYED_BY_WATER) {
			if (isBehaveToWaterNpc())
				behaveToHitObject_(param_1, HIT_NPC_OBJECT_KIND_WATER_SPRAY);
			result = true;
		} else if (param_2 == HIT_MESSAGE_UNK10
		           || (param_2 == HIT_MESSAGE_ATTACK
		               && param_1->getActorType() == 0x4000005A)) {
			if (mDamageParticleForbidCount == 0) {
				SMS_EasyEmitParticle(PARTICLE_MS_DMG_A, &param_1->mPosition,
				                     nullptr,
				                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
				SMS_EasyEmitParticle(PARTICLE_MS_DMG_B, &param_1->mPosition,
				                     nullptr,
				                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
				SMS_EasyEmitParticle(PARTICLE_MS_DMG_C, &param_1->mPosition,
				                     nullptr,
				                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
				mDamageParticleForbidCount = 16;
			}

			if (isBehaveToHitNpc()) {
				EnumHitNpcObjectKind kind = HIT_NPC_OBJECT_KIND_UNK2;
				if (param_2 == HIT_MESSAGE_UNK10)
					kind = HIT_NPC_OBJECT_KIND_UNK1;
				behaveToHitObject_(param_1, kind);
			}

			result = true;
		}
		break;
	}

	return result;
}

void TBaseNPC::moveObject()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	ensureTakeSituation();
	if (unk158 != nullptr && unk158->getHeldObject() != this)
		releaseTaken_();

	if (mTrampleCtrl != nullptr
	    && mTrampleCtrl->updateTrample(mInitialScale.y, &mScaling.y)
	    && isNerveCanGoToMad() && isStateGoToMad_()) {
		changeNerveToMad_();
	}

	mInbetweenCtrl->applyPos(mPosition);

	if (mCoinCtrl != nullptr)
		mCoinCtrl->updateCoin();

	if (mBalloonCtrl != nullptr) {
		int prev = mBalloonCtrl->unk0;
		if (!gpMarDirector->isTalkOrDemoModeNow()
		    && mBalloonCtrl->updateBalloon()) {
			if (mHolder != nullptr) {
				switch (prev) {
				case 0xE004F:
					mBalloonCtrl->setNextMessage(0xE0051, 0x1C20);
					break;
				case 0xE0050:
					break;
				case 0xE0051:
					mBalloonCtrl->setNextMessage(0xE004F, 0x1C20);
					break;
				}
			} else {
				switch (prev) {
				case 0xE0050:
					mBalloonCtrl->setNextMessage(0xE0050, 0x1C20);
					break;
				}
			}
		}
	}

	updateForbidCount_();

	updateSquareToMario();
	mLinearVelocity.zero();
	mAngularVelocity.zero();
	const TLiveActor* groundActor
	    = SMS_GetGroundActor(mGroundPlane, 0x400000CD);
	if (groundActor) {
		mHeadHeight = mPtrSaveNormal->mSLHeadHeightSandBomb.get();
		behaveToSandBomb_(groundActor);
	} else {
		mHeadHeight = mPtrSaveNormal->mSLHeadHeightNormal.get();
		unk1C8      = 0.0f;
	}

	control();

	if (!checkLiveFlag(LIVE_FLAG_UNK400000))
		calcRideMomentum();

	if (checkLiveFlag(LIVE_FLAG_UNK400000)
	    && !checkLiveFlag(LIVE_FLAG_UNK1000000))
		emitSinkEffect_();

	if (mSpine->getLatestNerve()
	    != &TNerveNPCSetPosAfterSinkBottom::theNerve()) {
		execNpcObjCollision_();

		if (!checkLiveFlag(LIVE_FLAG_UNK10))
			bind();

		if (mHolder != nullptr) {
			MtxPtr mtx = mHolder->getTakingMtx();
			mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);
			if (unk150 == nullptr) {
				s16 angle = CLBDegToShortAngle(mHolder->mRotation.y)
				            - mAngleYDiffWhenTaken;
				mRotation.y = SHORTANGLE2DEG(angle);
			}
		} else {
			if (!checkLiveFlag(LIVE_FLAG_UNK10) && isNerveWalk()
			    && !belongToGround() && mLinearVelocity.y > 5.0f) {
				mLinearVelocity.y = 5.0f;
			}
			mPosition += mLinearVelocity;
			mRotation += mAngularVelocity;
		}
		calcRidePos();
	}
}

void TBaseNPC::execMotionBlend_()
{
	if (!mInbetweenCtrl->isMotionBlending())
		setKeepAnm_();

	mInbetweenCtrl->execMotionBlend(getMActor());

	if (mInbetweenCtrl->isForcedBlendRatio())
		mKeepAnmCtrl->reset();
}

void TBaseNPC::calcRootMatrix()
{
	if (mActorType == 0x400001D) {
		TLiveActor::calcRootMatrix();
		return;
	}

	execMotionBlend_();

	if (mHolder != nullptr && unk150 != nullptr) {
		unk150->setBaseTRMtx(mHolder->getTakingMtx());
		unk150->calc();
		getModel()->setBaseTRMtx(unk154);
		return;
	}

	TLiveActor::calcRootMatrix();
}

void TBaseNPC::movementOnlyTalk_(const JDrama::TGraphics* param_1)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	updateForbidCount_();
	updateSquareToMario();
	control();
	if (param_1->unk0 & 0x2)
		changeNerveProc_();
}

void TBaseNPC::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (mActorType == 0x400001C) {
		if (!(param_1 & 0x1))
			return;

		if (mDummyConnectActor != nullptr) {
			mPosition = mDummyConnectActor->mPosition;
			mRotation = mDummyConnectActor->mRotation;
		}

		movementOnlyTalk_(param_2);
		return;
	}

	if (mActorType == 0x400001D) {
		if (checkLiveFlag(LIVE_FLAG_UNK200 | LIVE_FLAG_DEAD))
			return;

		if (param_1 & 0x1)
			movementOnlyTalk_(param_2);

		TLiveActor::performOnlyDraw(param_1, param_2);
		return;
	}

	bool bVar5 = true;
	if (checkLiveFlag(LIVE_FLAG_DEAD)) {
		bVar5 = false;
	} else if (checkLiveFlag(LIVE_FLAG_UNK200)) {
		bVar5 = false;
	} else if ((param_1 & 0x204)
	           && checkLiveFlag(LIVE_FLAG_CLIPPED_OUT | LIVE_FLAG_HIDDEN)) {
		bVar5 = false;
	} else if ((param_1 & 0x1) && mHolder == nullptr && !isAirborne()
	           && !belongToGround() && mSpine->getTime() != 0
	           && mActorType != 0x4000018 && isNerveMaybeDontMovement()
	           && !checkLiveFlag(LIVE_FLAG_SINK_BOTTOM)) {
		TNPCManager* manager = (TNPCManager*)mManager;
		f32 farClip          = gpConductor->unk84.mEnemyFarClip.get();
		if (manager != nullptr)
			farClip = *manager->unk58;

		s16 angle = matan(gpCamera->unk148.z - gpCamera->unk124.z,
		                  gpCamera->unk148.x - gpCamera->unk124.x);
		JGeometry::TVec3<f32> local_4C = gpCamera->unk124;
		if (!MsIsInSight(local_4C, SHORTANGLE2DEG(angle), mPosition,
		                 farClip + 500.0f, 3.46875f, 4.28125f)) {
			updateSquareToMario();
			bVar5 = false;
		}
	}

	if (!bVar5) {
		offLiveFlag(LIVE_FLAG_UNK10000);
		return;
	}

	gpCurrentNpc = this;
	if (param_1 & 0x1) {
		moveObject();
		if (param_2->unk0 & 0x2) {
			changeNerveProc_();
			if (mHolder == nullptr) {
				if (isNerveWalk())
					walkAnmRateChange_();
				if (unkD0->getCurrentAnmKind() == NPC_ANM_KIND_UNK4) {
					f32 rate = SMSGetAnmFrameRate();
					mMActor->setFrameRate(
					    MsClamp(
					        mTurnSpeed * mIndividualParams->mTurnAnmRate.get()
					            * rate,
					        mIndividualParams->mTurnAnmMinRate.get() * rate,
					        mIndividualParams->mTurnAnmMaxRate.get() * rate),
					    0);
				}
			}

			mInbetweenCtrl->execPosInbetween(&mPosition);
			if (unk1DC > 0) {
				unk1DC -= 1;
				if (unk1DC == 0 && mHolder == nullptr) {
					offHitFlag(HIT_FLAG_NO_COLLISION);
					offLiveFlag(LIVE_FLAG_UNK10000000);
				}
			}

			if (!isJellyFishMare() && mActorType != 0x4000007)
				setVariableDamageRadius_();

			if (isPollutionNpc())
				unk174.a
				    = mPollutionAmount * mIndividualParams->mPollutionMax.get();
		}

		param_1 &= ~0x1;
	}

	if (param_1 & 0x2) {
		if (checkActionFlag(NPC_ACTION_BURNING)) {
			if (SMSGetMSound()->gateCheck(0x8017))
				MSoundSESystem::MSoundSE::startSoundNpcActor(0x8017, &mPosition,
				                                             0, nullptr, 0, 4);
		}

		if (!checkLiveFlag(LIVE_FLAG_UNK1000000 | LIVE_FLAG_HIDDEN
		                   | LIVE_FLAG_CLIPPED_OUT))
			emitParticle_();

		bool r31 = false;

		bool bVar12 = checkLiveFlag2(LIVE_FLAG_HIDDEN | LIVE_FLAG_CLIPPED_OUT
		                             | LIVE_FLAG_DEAD);
		bool bVar6  = checkLiveFlag2(LIVE_FLAG_UNK1000000);

		if (bVar12) {
			r31 = true;
			updateAnmSound();
			execMotionBlend_();
			mMActor->frameUpdate();
			if (unk168 != nullptr && isPartsAnmNpc()) {
				unk168->partsFrameUpdate();
			}
		} else if (mHolder == nullptr) {
			if (!isAirborne() && !belongToGround()
			    && (isNerveMaybeDontCalcAnim0()
			        || isNerveMaybeDontCalcAnim1())) {
				JGeometry::TVec3<f32> diff;
				diff.sub(mPosition, gpCamera->unk124);
				if (CLBSquared(getAnmOffDist_()) < diff.squared() && !bVar6
				    && mSpine->getTime() > 2) {
					r31 = true;
					execMotionBlend_();
				}
			}
		}

		if (bVar6 && !r31 && mMultiMtxEffect != nullptr) {
			mMultiMtxEffect->flagOn(0x2);
		}

		if (r31) {
			param_1 &= ~0x2;
		}
	}

	if ((param_1 & 2) && mMultiMtxEffect != nullptr) {
		mMultiMtxEffect->setUserArea();
		if (mActorType == 0x4000018 && mHolder != nullptr) {
			mMultiMtxEffect->flagOn(0x2);
		}
	}

	if (param_1 & 0x200) {
		offLiveFlag(LIVE_FLAG_UNK1000000);
		JGeometry::TVec3<f32> diff;
		diff.sub(mPosition, gpCamera->unk124);
		if (diff.squared() > CLBSquared(mIndividualParams->mAllDLLockDist.get())
		    && !isSunflower()) {
			getModel()->lock();
		} else {
			mMActor->unlockDLIfNeed();
		}
	}

	TSpineEnemy::perform(param_1, param_2);
	if (unk168 != nullptr)
		unk168->partsPerform(param_1, param_2);
}

void TBaseNPC::setDummyConnectActor(const JDrama::TActor* param_1)
{
	if (mActorType != 0x400001C)
		return;

	mDummyConnectActor = param_1;
	mPosition          = mDummyConnectActor->mPosition;
	mRotation          = mDummyConnectActor->mRotation;
}

void TBaseNPC::setBalloonMessage(u32 param_1, s32 param_2)
{
	mBalloonCtrl->setNextMessage(param_1, param_2);
}

const GXColor* TBaseNPC::getPtrInitPollutionColor() const
{
	const GXColor* result = nullptr;

	if (isPollutionNpc()) {
		result = &unk174;
	} else if (mActorType != 0x4000006
	           && (isSpecialMonte() || isSpecialMare())) {
		result = &unk174;
	}

	return result;
}
