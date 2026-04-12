#include <NPC/NpcBase.hpp>
#include <Strategic/Spine.hpp>
#include <System/MarDirector.hpp>
#include <System/EmitterViewObj.hpp>
#include <MarioUtil/MapUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MSound/MSound.hpp>
#include <Map/Map.hpp>
#include <Map/PollutionManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Camera/cameralib.hpp>
#include <Enemy/Graph.hpp>
#include <NPC/NpcTrample.hpp>
#include <NPC/NpcNerve.hpp>
#include <NPC/NpcInbetween.hpp>
#include <NPC/NpcBalloon.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

bool TBaseNPC::isNerveWalk() const
{
	bool result                         = false;
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	if (nerve == &TNerveNPCGraphWander::theNerve()
	    || nerve == &TNerveNPCUTurn::theNerve()
	    || nerve == &TNerveNPCGraphWait::theNerve())
		result = true;
	return result;
}

bool TBaseNPC::isNerveMaybeDontMovement() const
{
	bool result                         = false;
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	if (nerve == &TNerveNPCWaitContinue::theNerve()
	    || nerve == &TNerveNPCWaitMarioApproach::theNerve()
	    || nerve == &TNerveNPCSink::theNerve())
		result = true;
	return result;
}

bool TBaseNPC::isNerveMaybeDontCalcAnim0() const
{
	bool result                         = false;
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	if (nerve == &TNerveNPCWaitContinue::theNerve()
	    || nerve == &TNerveNPCWaitMarioApproach::theNerve())
		result = true;
	return result;
}

bool TBaseNPC::isNerveMaybeDontCalcAnim1() const
{
	bool result                         = false;
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	if (nerve == &TNerveNPCGraphWait::theNerve()
	    || nerve == &TNerveNPCWaitContinue::theNerve()
	    || nerve == &TNerveNPCWaitMarioApproach::theNerve())
		result = true;
	return result;
}

bool TBaseNPC::isNerveCanGoToTalk() const
{
	bool result                         = false;
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	if (nerve == &TNerveNPCGraphWander::theNerve()
	    || nerve == &TNerveNPCUTurn::theNerve()
	    || nerve == &TNerveNPCGraphWait::theNerve()
	    || nerve == &TNerveNPCTurnToMario::theNerve()
	    || nerve == &TNerveNPCWet::theNerve()
	    || nerve == &TNerveNPCRecoverAfter::theNerve()
	    || nerve == &TNerveNPCMad::theNerve()
	    || nerve == &TNerveNPCMareStand::theNerve()) {
		if (mSpine->getCurrentNerve() != nullptr
		    || (mSpine->getTop() != &TNerveNPCWet::theNerve()
		        && mSpine->getTop() != &TNerveNPCTalk::theNerve())) {
			result = true;
		}
	}
	return result;
}

bool TBaseNPC::isNerveCanGoToWet() const
{
	bool result                         = false;
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	if (nerve == &TNerveNPCGraphWander::theNerve()
	    || nerve == &TNerveNPCUTurn::theNerve()
	    || nerve == &TNerveNPCGraphWait::theNerve()
	    || nerve == &TNerveNPCWaitContinue::theNerve()
	    || nerve == &TNerveNPCWaitMarioApproach::theNerve()
	    || nerve == &TNerveNPCTurnToMario::theNerve()
	    || nerve == &TNerveNPCTalk::theNerve()) {
		if (mSpine->getCurrentNerve() != nullptr
		    || (mSpine->getTop() != &TNerveNPCWet::theNerve()
		        && mSpine->getTop() != &TNerveNPCTalk::theNerve())) {
			result = true;
		}
	}
	return result;
}

bool TBaseNPC::isNerveCanGoToSink() const
{
	bool result                         = false;
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	if (nerve == &TNerveNPCGraphWander::theNerve()
	    || nerve == &TNerveNPCUTurn::theNerve()
	    || nerve == &TNerveNPCGraphWait::theNerve()
	    || nerve == &TNerveNPCWaitContinue::theNerve()
	    || nerve == &TNerveNPCWaitMarioApproach::theNerve()
	    || nerve == &TNerveNPCTurnToMario::theNerve()) {
		if (mSpine->getCurrentNerve() != nullptr
		    || mSpine->getTop() != &TNerveNPCSink::theNerve()) {
			result = true;
		}
	}
	return result;
}

bool TBaseNPC::isNerveCanGoToTaken() const
{
	bool result                         = false;
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	if (nerve == &TNerveNPCGraphWander::theNerve()
	    || nerve == &TNerveNPCUTurn::theNerve()
	    || nerve == &TNerveNPCGraphWait::theNerve()
	    || nerve == &TNerveNPCWaitContinue::theNerve()
	    || nerve == &TNerveNPCWaitMarioApproach::theNerve()
	    || nerve == &TNerveNPCTurnToMario::theNerve()
	    || nerve == &TNerveNPCWet::theNerve()
	    || nerve == &TNerveNPCSink::theNerve()
	    || nerve == &TNerveNPCRecoverFromSink::theNerve()
	    || nerve == &TNerveNPCRecoverAfter::theNerve())
		result = true;
	return result;
}

bool TBaseNPC::isNerveCanGoToThrow() const { }

bool TBaseNPC::isNerveCanGoToMad() const
{
	bool result                         = false;
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	if (nerve == &TNerveNPCGraphWander::theNerve()
	    || nerve == &TNerveNPCUTurn::theNerve()
	    || nerve == &TNerveNPCGraphWait::theNerve()
	    || nerve == &TNerveNPCWaitContinue::theNerve()
	    || nerve == &TNerveNPCWaitMarioApproach::theNerve()
	    || nerve == &TNerveNPCTurnToMario::theNerve()
	    || nerve == &TNerveNPCWet::theNerve())
		result = true;
	return result;
}

bool TBaseNPC::isNerveCanGoToBlown() const
{
	bool result                         = false;
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	if (nerve == &TNerveNPCGraphWander::theNerve()
	    || nerve == &TNerveNPCUTurn::theNerve()
	    || nerve == &TNerveNPCGraphWait::theNerve()
	    || nerve == &TNerveNPCWaitContinue::theNerve()
	    || nerve == &TNerveNPCWaitMarioApproach::theNerve()
	    || nerve == &TNerveNPCTurnToMario::theNerve()
	    || nerve == &TNerveNPCWet::theNerve()
	    || nerve == &TNerveNPCMareStand::theNerve())
		if (!gpMarDirector->isThing())
			result = true;
	return result;
}

void TBaseNPC::changeNerveFromTalk_()
{
	const TNerveBase<TLiveActor>* current = mSpine->getCurrentNerve();
	const TNerveBase<TLiveActor>* top     = mSpine->getTop();

	if (current == &TNerveNPCWet::theNerve()) {
		mSpine->skip();
	} else if (current == nullptr && top == &TNerveNPCTalk::theNerve()) {
		mSpine->skip();
	} else {
		TNerveNPCTalk::theNerve();
	}

	mSpine->setNext(nullptr);

	if (mThrowCtrl != nullptr) {
		if (checkLiveFlag(LIVE_FLAG_UNK20000000)) {
			offLiveFlag(LIVE_FLAG_UNK20000000);
		} else {
			mSpine->setNext(&TNerveNPCThrow::theNerve());
		}
	}

	offLiveFlag(LIVE_FLAG_UNK2000000);
}

void TBaseNPC::changeNerveToWet_() { }

void TBaseNPC::changeNerveToMad_()
{
	if (mSpine->getCurrentNerve() == &TNerveNPCWet::theNerve()) {
		mSpine->setNext(&TNerveNPCMad::theNerve());
	} else {
		mSpine->pushNerve(&TNerveNPCMad::theNerve());
	}
}

void TBaseNPC::releaseTaken_()
{
	f32 fVar1 = mPtrSaveNormal->mThrowSpeedXZ.get();
	s16 uVar4 = CLBDegToShortAngle(unk158->mRotation.y);
	f32 fVar2 = mPtrSaveNormal->mThrowSpeedY.get();
	f32 s     = JMASSin(uVar4);
	f32 c     = JMASCos(uVar4);

	mVelocity.x = fVar1 * s;
	mVelocity.y = fVar2;
	mVelocity.z = fVar1 * c;

	onLiveFlag(LIVE_FLAG_UNK10000000);

	unk1DC = CLBPalFrame(15l);
	unk158 = nullptr;
	if (mActorType == 0x4000018) {
		peachTiredIn_();
		mBalloonCtrl->setNextMessage(0, -1);
	}

	mSpine->setNext(&TNerveNPCWaitMarioApproach::theNerve());
}

void TBaseNPC::behaveToBeTaken_(THitActor* param_1)
{
	s16 sVar2 = CLBDegToShortAngle(mRotation.y);
	s16 sVar3 = CLBDegToShortAngle(param_1->mRotation.y);

	mAngleYDiffWhenTaken = sVar3 - sVar2;

	mInbetweenCtrl->startPosBlend();
	onHitFlag(HIT_FLAG_NO_COLLISION);
	offLiveFlag(LIVE_FLAG_UNK10 | LIVE_FLAG_UNK20000 | LIVE_FLAG_UNK400000);
	if (mActorType == 0x4000018) {
		peachParasolOut_();
		mBalloonCtrl->setNextMessage(0xE004F, 0x2EE);
	}

	mSpine->setNext(&TNerveNPCWaitContinue::theNerve());
	mHolder = (TTakeActor*)param_1;
	unk158  = (TTakeActor*)param_1;
}

void TBaseNPC::behaveToBeTrampled_()
{
	bool cVar7 = isChild();

	u32 uVar10 = 0x8826;
	if (isMonteM()) {
		if (cVar7)
			uVar10 = 0x88AB;
		else
			uVar10 = 0x8844;
	} else if (isMonteW()) {
		if (cVar7)
			uVar10 = 0x88AC;
		else
			uVar10 = 0x8845;
	} else if (isMareM()) {
		if (cVar7) {
			uVar10 = 0x88AD;
		} else {
			switch (mActorType) {
			case 0x4000010:
			case 0x4000011:
				uVar10 = 0x88AF;
				break;

			default:
				uVar10 = 0x8846;
				break;
			}
		}
	} else if (isMareW()) {
		if (cVar7) {
			uVar10 = 0x88AE;
		} else {
			uVar10 = 0x8847;
		}
	} else {
		switch (mActorType) {
		case 0x4000016:
			uVar10 = 0x8849;
			break;

		case 0x4000017:
			uVar10 = 0x8848;
			break;
		}
	}

	if (SMSGetMSound()->gateCheck(uVar10))
		MSoundSESystem::MSoundSE::startSoundNpcActor(uVar10, &mPosition, 0,
		                                             nullptr, 0, 4);
	mTrampleCtrl->startTrample();

	if (!isMare())
		return;

	if (!isBehaveToWaterNpc())
		return;

	if (checkActionFlag(NPC_ACTION_BURNING | NPC_ACTION_HAPPY))
		return;

	switch (unkD0->getCurrentAnmKind()) {
	case NPC_ANM_KIND_UNK1B:
	case NPC_ANM_KIND_UNK7: {
		const TNerveBase<TLiveActor>* current = mSpine->getCurrentNerve();
		const TNerveBase<TLiveActor>* latest  = mSpine->getLatestNerve();
		if (latest == &TNerveNPCWet::theNerve()) {
			mSpine->pushNerve(&TNerveNPCWet::theNerve());
			mSpine->setNext(nullptr);
		} else if (current == nullptr && latest == &TNerveNPCWet::theNerve())
			mSpine->setNext(&TNerveNPCWet::theNerve());
	} break;

	default:
		if (isNerveCanGoToWet())
			mSpine->pushNerve(&TNerveNPCWet::theNerve());
		break;
	}
}

void TBaseNPC::behaveToHitObject_(THitActor* param_1,
                                  EnumHitNpcObjectKind param_2)
{
	if (checkActionFlag(NPC_ACTION_BURNING)) {
		if (param_2 != HIT_NPC_OBJECT_KIND_WATER_SPRAY)
			return;

		if (gpMarDirector->isTalkOrDemoModeNow())
			return;

		gpMarioParticleManager->emit(0xE7, &mPosition, 0, nullptr);
		SMSGetMSound()->startSoundSet(0x6802, &mPosition, 0, 0.0f, 0, 0, 4);
		if (SMSGetMSound()->gateCheck(0x8837))
			MSoundSESystem::MSoundSE::startSoundNpcActor(0x8837, &mPosition, 0,
			                                             nullptr, 0, 4);
		mBurnStrength -= mPtrSaveNormal->mSLFireDecSpeed.get();
		if (!isExtinguished_())
			return;

		mBurnStrength = 0.0f;
		offActionFlag(NPC_ACTION_BURNING | NPC_ACTION_UNK80 | NPC_ACTION_RUN);
		npcHappyIn(1);
		return;
	}

	if (param_2 == HIT_NPC_OBJECT_KIND_WATER_SPRAY) {
		if (!gpMarDirector->isTalkOrDemoModeNow() && isPollutionNpc()
		    && mSpine->getCurrentNerve() == &TNerveNPCWet::theNerve()) {
			if (mPollutionAmount > 0.0f) {
				mPollutionAmount
				    -= mIndividualParams->mPollutionCleanSpeed.get();
				if (mPollutionAmount <= 0.0f) {
					mPollutionAmount = 0.0f;
					mTalkForbidCount = 60;
					onUnk1DA(UNK1DA_FLAG_UNK2);
				}
			}
		}
	}

	if (checkActionFlag(NPC_ACTION_UNK400 | NPC_ACTION_HAPPY))
		return;

	if (isNerveCanGoToWet() && !checkActionFlag(NPC_ACTION_UNK800)) {
		if (!isPeachTired()) {
			if (!isSunflowerReviving()
			    && (isClean() || param_2 != HIT_NPC_OBJECT_KIND_UNK1)
			    && (mActorType != 0x4000006
			        || unkD0->getCurrentAnmKind() == NPC_ANM_KIND_UNK4
			        || unkD0->getCurrentAnmKind() == NPC_ANM_KIND_UNK6)
			    && (mSpine->getCurrentNerve() != &TNerveNPCTalk::theNerve()
			        || mSpine->getTime() >= 4)) {
				if (param_2 == 1)
					onLiveFlag(LIVE_FLAG_UNK4000000);
				mSpine->pushNerve(&TNerveNPCWet::theNerve());
			}
		}
	}
}

void TBaseNPC::behaveToSandBomb_(const TLiveActor* param_1)
{
	f32 fVar1 = unk1C8;
	unk1C8    = SMS_GetSandRiseUpRatio(param_1);
	if (unk1C8 > fVar1 && unk1C8 > 0.05f && fVar1 > 0.001f
	    && isNerveCanGoToBlown()) {
		f32 fVar1 = mPtrSaveNormal->mSLBlownVelocity.get();
		mPosition.y += fVar1;
		onLiveFlag(LIVE_FLAG_AIRBORNE);
		mVelocity = JGeometry::TVec3<f32>(0.0f, fVar1, 0.0f);
		if (mSpine->getCurrentNerve() == &TNerveNPCWet::theNerve()) {
			mSpine->setNext(&TNerveNPCBlown::theNerve());
		} else {
			mSpine->pushNerve(&TNerveNPCBlown::theNerve());
		}
	}
}

bool TBaseNPC::isStateGoToMad_() const
{
	bool result = false;
	if (isMadNpc()
	    && !checkActionFlag(NPC_ACTION_BURNING | NPC_ACTION_UNK400
	                        | NPC_ACTION_HAPPY)
	    && isClean() && isInMadSearchRange()) {
		result = true;
	}
	return result;
}

bool TBaseNPC::isNowCanTaken() const
{
	bool result = false;
	if (checkLiveFlag(LIVE_FLAG_UNK100000) && mActorType != 0x400001C
	    && mHolder == nullptr && mHeldObject == nullptr
	    && !checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN
	                      | LIVE_FLAG_CLIPPED_OUT | LIVE_FLAG_UNK40000
	                      | LIVE_FLAG_SINK_BOTTOM)
	    && !checkHitFlag(HIT_FLAG_NO_COLLISION) && isNerveCanGoToTaken()) {
		result = true;
	}
	return result;
}

void TBaseNPC::changeNerveProc_()
{
	bool bVar4                                = false;
	const TNerveBase<TLiveActor>* latestNerve = mSpine->getLatestNerve();
	if (latestNerve == &TNerveNPCTalk::theNerve()) {
		bVar4 = true;
		onLiveFlag(LIVE_FLAG_UNK20000);
		offLiveFlag(LIVE_FLAG_UNK40000);
	} else {
		bool bVar5 = false;
		if (checkLiveFlag(LIVE_FLAG_UNK40000)) {
			bVar5 = true;
		} else if (mTalkForbidCount == 0 && !isJellyFishMare()
		           && !gpCamera->isTalkCameraInbetween() && mHolder == nullptr
		           && !checkLiveFlag(0xc10207)
		           && !checkActionFlag(NPC_ACTION_BURNING) && isClean()) {

			if (isSunflowerReviving() && isNerveCanGoToTalk()
			    && (mActorType != 0x4000006
			        || unkD0->getCurrentAnmKind() == NPC_ANM_KIND_UNK4)
			    && !SMS_IsMarioOpeningDoor()) {
				if (gpMarDirector->mMap == 7) {
					JGeometry::TVec3<f32> local_58 = mPosition;
					local_58.y += 75.0f;
					if (SMS_IsInSameCameraCube(local_58))
						bVar5 = true;
				}

				f32 fVar1;
				f32 fVar2;
				if (mThrowCtrl != nullptr) {
					fVar1 = mPtrSaveNormal->mSLThrowTalkAcceptHeight.get();
					fVar2 = mPtrSaveNormal->mSLThrowTalkAcceptDist.get();
				} else {
					if (mActorType == 0x400001A) {
						fVar2 = mPtrSaveNormal->mSLSunflowerLTalkDist.get();
					} else {
						fVar2 = mPtrSaveNormal->mTalkAcceptDist.get();
					}
					fVar1 = mPtrSaveNormal->mTalkAcceptHeight.get();
				}

				f32 fVar3;
				if ((checkActionFlag(NPC_ACTION_UNK400 | NPC_ACTION_UNK1))
				    || isSunflower() || mActorType == 0x400001D) {
					fVar3 = mPtrSaveNormal->mSLSitTalkAcceptDegree.get();
				} else {
					fVar3 = mPtrSaveNormal->mTalkAcceptDegree.get();
				}

				if (abs(SMS_GetMarioPos().y - mPosition.y) < fVar1
				    && isInSight(SMS_GetMarioPos(), fVar2, fVar3, -1.0f)
				    && MsIsInSight(
				        SMS_GetMarioPos(), SHORTANGLE2DEG(*gpMarioAngleY),
				        mPosition, fVar2,
				        mPtrSaveNormal->mSLMarioTalkAcceptDegree.get(), 0.0f))
					bVar5 = true;
			}
		}

		if (bVar5) {
			onLiveFlag(LIVE_FLAG_UNK20000);
			if (checkLiveFlag(LIVE_FLAG_UNK40000)) {
				offLiveFlag(LIVE_FLAG_UNK40000);
				const TNerveBase<TLiveActor>* current
				    = mSpine->getCurrentNerve();
				if (current == &TNerveNPCWet::theNerve()
				    || current == &TNerveNPCRecoverAfter::theNerve()
				    || current == &TNerveNPCMad::theNerve()) {
					mSpine->setNext(&TNerveNPCTalk::theNerve());
				} else {
					mSpine->pushNerve(&TNerveNPCTalk::theNerve());
				}
				offLiveFlag(LIVE_FLAG_UNK2000000);
			}
		} else {
			offLiveFlag(LIVE_FLAG_UNK20000);
			offLiveFlag(LIVE_FLAG_UNK40000);
		}
	}

	if (bVar4)
		return;

	if (!isPollutionNpc())
		return;

	if (checkActionFlag(NPC_ACTION_BURNING | NPC_ACTION_UNK400
	                    | NPC_ACTION_HAPPY))
		return;

	if (unk15C == nullptr)
		return;

	if (latestNerve == &TNerveNPCSetPosAfterSinkBottom::theNerve())
		return;

	if (!unk15C->doThing())
		return;

	unk15C->unk0 = 0;

	if (latestNerve == &TNerveNPCSink::theNerve()) {
		if (gpPollution->isPolluted(mPosition.x, mPosition.y, mPosition.z))
			return;

		mSpine->setNext(&TNerveNPCRecoverFromSink::theNerve());
		offHitFlag(HIT_FLAG_NO_COLLISION);
		offLiveFlag(LIVE_FLAG_SINK_BOTTOM);
		requestNpcAnm_(NPC_ANM_KIND_UNK1A, NPC_STOP_MOTION_BLEND_ON);
		if (SMSGetMSound()->gateCheck(0x3811))
			MSoundSESystem::MSoundSE::startSoundNpcActor(0x3811, &mPosition, 0,
			                                             nullptr, 0, 4);
		return;
	}

	if (checkLiveFlag(LIVE_FLAG_UNK400000))
		return;

	if (isAirborne())
		return;

	if (!gpPollution->isPolluted(mPosition.x, mPosition.y, mPosition.z))
		return;

	if (isNerveCanGoToSink()) {
		unk1C4 = mPosition.y;
		mVelocity.set(0.0f, 0.0f, 0.0f);
		offLiveFlag(LIVE_FLAG_UNK20000 | LIVE_FLAG_UNK8000000);
		onLiveFlag(LIVE_FLAG_UNK400000 | LIVE_FLAG_UNK10);
		npcFallIn();
		mSpine->pushNerve(&TNerveNPCSink::theNerve());
	}
}

void TBaseNPC::setPosAndInitAfterSinkBottom()
{
	JGeometry::TVec3<f32> pos = unk194;

	bool cVar8 = gpPollution->isPolluted(pos.x, pos.y, pos.z);
	offLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN | LIVE_FLAG_CLIPPED_OUT
	            | LIVE_FLAG_UNK8 | LIVE_FLAG_UNK10 | LIVE_FLAG_UNK20000
	            | LIVE_FLAG_UNK40000 | LIVE_FLAG_UNK400000
	            | LIVE_FLAG_SINK_BOTTOM | LIVE_FLAG_UNK2000000
	            | LIVE_FLAG_UNK8000000);
	onLiveFlag(LIVE_FLAG_UNK1000000);
	mHeldObject = nullptr;
	mHolder     = nullptr;
	if (unk15C != nullptr)
		unk15C->unk0 = 0;
	mHitPoints = getMaxHitPoints();
	if (mMultiMtxEffect != nullptr)
		mMultiMtxEffect->flagOn(0x2);
	mSpine->reset();
	mInbetweenCtrl->reset();
	mKeepAnmCtrl->reset();
	unk1CC = 0;
	unk1D0 = 0.0f;
	if (cVar8 && isPollutionNpc() && !checkActionFlag(NPC_ACTION_UNK400)) {
		onHitFlag(HIT_FLAG_NO_COLLISION);
		mSpine->setDefaultNext();
		mSpine->pushNerve(&TNerveNPCSink::theNerve());
		mSpine->pushNerve(&TNerveNPCSink::theNerve());
		onLiveFlag(LIVE_FLAG_UNK10 | LIVE_FLAG_UNK400000
		           | LIVE_FLAG_SINK_BOTTOM);
		unk1C4 = mGroundHeight = gpMap->checkGroundIgnoreWaterSurface(
		    pos.x, pos.y + getHeadHeight(), pos.z, &mGroundPlane);
		pos.y = unk1C4 - mIndividualParams->mSinkHeight.get();
		mVelocity.set(0.0f, 0.0f, 0.0f);
	} else {
		offHitFlag(HIT_FLAG_NO_COLLISION);
		mSpine->setDefaultNext();
		mSpine->pushNerve(mSpine->getDefault());
		pos.y += 2.0f;
		mVelocity.set(0.0f, 5.0f, 0.0f);
		onLiveFlag(LIVE_FLAG_UNK8000);
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	mPosition.set(pos);
	mRotation.set(unk1A0);
	mLinearVelocity.set(0.0f, 0.0f, 0.0f);
	unk124->reset();
	unk124->reset2();
	goToShortestNextGraphNode();
	npcWaitIn();
	randomizeBckAndBtpFrame_();
}

void TBaseNPC::kill() { }
