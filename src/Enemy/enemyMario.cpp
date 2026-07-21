
// This file uses reverse MAP definition order because Enemy is built with
// -inline deferred.
#include <Enemy/EnemyMario.hpp>
#include <Enemy/Emario.hpp>
#include <Enemy/Graph.hpp>
#include <M3DUtil/MActor.hpp>
#include <Map/MapData.hpp>
#include <Map/PollutionManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Player/MarioRecord.hpp>
#include <System/Application.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <System/MSoundMainSide.hpp>
#include <System/Particles.hpp>
#include <math.h>

void TEnemyMario::setStickToAngle(s16 angle, f32 power)
{
	unk108->mStickHS16 = power * (JMASSin(angle) * 64.0f);
	unk108->mStickVS16 = power * (-JMASCos(angle) * 64.0f);
}

void TEnemyMario::startMonteReplay(u32 replayIndex)
{
	int nodeIndex = mEMario->getTracer()->getGraph()->findNearestNodeIndex(
	    mPosition, -1);
	JGeometry::TVec3<f32> currentPoint;
	mEMario->getTracer()->getGraph()->getGraphNode(nodeIndex).getPoint(
	    &currentPoint);
	mPosition = currentPoint;
	JGeometry::TVec3<f32> nextPoint;
	mEMario->getTracer()->getGraph()->getGraphNode(nodeIndex + 1).getPoint(
	    &nextPoint);
	f32 xDifference = nextPoint.x - currentPoint.x;
	f32 zDifference = nextPoint.z - currentPoint.z;
	mFaceAngle.y     = matan(zDifference, xDifference);
	mVel.x       = 0.0f;
	mVel.y       = 0.0f;
	mVel.z       = 0.0f;
	mForwardVel = 0.0f;
	resetHistory();
	changePlayerStatus(MARIO_STATUS_WAIT, 0, true);
	mReplayIndex = replayIndex;
	mInputReplays[mReplayIndex]->reset();
	mInputReplays[mReplayIndex]->start();
	changeEMDoing(EM_DOING_REPLAY_TO_GOAL);
}

void TEnemyMario::changeEMDoing(u16 doing)
{
	mEMDoingTimer = 0;
	mEMDoing      = doing;
}

BOOL TEnemyMario::tryTake()
{
	if (mHeldObject != nullptr && mStatus != MARIO_STATUS_TAKE) {
		return TRUE;
	}

	for (int i = 0; i < mEMario->getColNum(); ++i) {
		THitActor* actor = mEMario->getCollision(i);
		u32 actorType    = actor->getActorType();
		if (actorType == 0x04000018 || actorType == 0x2000002A
		    || actorType == 0x20000022 || actorType == 0x20000009) {
			if (actorType == 0x04000018) {
				((TLiveActor*)actor)->onLiveFlag(LIVE_FLAG_UNK100000);
				mGoalFlags |= EM_GOAL_FLAG_ENFORCE_TAKE;
			}
			unk384 = actor;
			changePlayerStatus(MARIO_STATUS_TAKE, 0, false);
		}
	}
	return FALSE;
}

void TEnemyMario::emWaiting()
{
	s16 angleDifference = mAngleToMario - mFaceAngle.y;
	if (angleDifference < -0x1555 || angleDifference > 0x1555) {
		setStickToAngle(mAngleToMario, 0.2f);
	}

	if (mDistanceToMario < 800.0f) {
		changeEMDoing(EM_DOING_GET_CLOSER);
	}

	if (mDistanceToMario > 1500.0f || rand() < 0x88) {
		TEMario* emario = mEMario;
		emario->getTracer()->reset();
		emario->goToShortestNextGraphNode();
		changeEMDoing(EM_DOING_WALK_GRAPH);
	}
}

void TEnemyMario::startDisappear(u16 doing)
{
	mDisappearPosition = mPosition;

	u8 currentMap      = gpMarDirector->getCurrentMap();
	u8 currentStage    = gpMarDirector->getCurrentStage();
	bool keepBossLives = false;
	if (currentMap == 1 && currentStage == 1) {
		keepBossLives = true;
	} else if (currentMap == 1 && currentStage == 9) {
		keepBossLives = true;
	}

	if (!keepBossLives) {
		MSMainProc::setBossLivesFlag(false);
	}

	gpMarioParticleManager->emitAndBindToPosPtr(0xED, &mDisappearPosition, 0,
	                                            nullptr);
	changeEMDoing(doing);
}

void TEnemyMario::emDisappearToGate()
{
	if (mEMDoingTimer >= 8) {
		mGoalFlags &= ~EM_GOAL_FLAG_DISP_PENCIL;
	} else {
		mGoalFlags |= EM_GOAL_FLAG_DISP_PENCIL;
	}

	onHitFlag(HIT_FLAG_NO_COLLISION);
	mEMario->onHitFlag(HIT_FLAG_NO_COLLISION);
	gpMarioParticleManager->emitAndBindToPosPtr(
	    0x1AA, &mDisappearPosition, 1, this);
	gpMarioParticleManager->emitAndBindToPosPtr(
	    0x1AB, &mDisappearPosition, 1, this);

	if (mEMDoingTimer == 0) {
		mDisappearPosition = mCenterPos;
		gpMarioParticleManager->emit(0xED, &mDisappearPosition, 0, nullptr);
		SMSGetMSound()->startSoundActor(MSD_SE_MA_KAGE_FIELD_AWAY, &mPosition,
		                                 0, nullptr, 0, 4);
	}

	if (mEMDoingTimer > 100) {
		mDisappearPosition.y += 0.025f * mEMDoingTimer;
		mDisappearPosition.z -= 0.03f * mEMDoingTimer;
	}
	++mEMDoingTimer;
}

void TEnemyMario::emReplay()
{
	mInputReplays[mReplayIndex]->play(
	    &mIntendedMag, &mIntendedYaw, &unk108->mInput,
	    &unk108->mFrameInput, &unk108->mAnalogLU8, &unk108->mAnalogRU8);

	if (mSettingParams->mPolluteFlag.get() && gpPollution != nullptr) {
		f32 polluteSize = mSettingParams->mPolluteSize.get();
		gpPollution->pollute(mPosition.x, mPosition.y, mPosition.z,
		                     polluteSize);
	}

	if (mInputReplays[mReplayIndex]->canPlay()) {
		return;
	}

	if (mSettingParams->mCarryFlag.get() == 1 && mHeldObject == nullptr) {
		changeEMDoing(EM_DOING_KEEP_STAY);
		return;
	}

	if (mStampActor != nullptr && mSettingParams->mStampFlag.get() == 1) {
		mStampActor->setBck("stamp_koopa_sign_draw1");
		MActor* stampActor = mStampActor;
		stampActor->setFrameRate(SMSGetAnmFrameRate(), 0);
		changeEMDoing(EM_DOING_DRAW_STAMP);
		startSoundActor(0x1980);
		startSoundActor(0x1981);
		return;
	}

	int nodeIndex = mEMario->getTracer()->getGraph()->findNearestNodeIndex(
	    mPosition, -1);
	if (mEMario->getTracer()->getGraph()->getGraphNode(nodeIndex).checkFlag(
	        0x40)) {
		changeEMDoing(EM_DOING_GOAL);
		return;
	}

	if (mSettingParams->mStopFlag.get() == 1) {
		changeEMDoing(EM_DOING_SLEEPING);
		return;
	}

	nodeIndex = mEMario->getTracer()->getGraph()->findNearestNodeIndex(
	    mPosition, -1);
	if (mEMario->getTracer()->getGraph()->getGraphNode(nodeIndex).checkFlag(2)) {
		mFaceAngle.y = mAngleToMario;
		unk108->mFrameInput |= TMarioControllerWork::A;
		unk108->mInput |= TMarioControllerWork::A;
	}
	changeEMDoing(EM_DOING_TRAMPLED);
}

void TEnemyMario::emDownAnimation()
{
	changePlayerStatus(0x133E, 0, true);
	setAnimation(ANIM_FALL_DOWN_WAIT, 1.0f);

	if (gpMarDirector->isDemoModeNow() || gpMarDirector->isTalkModeNow()) {
		mDownPosition      = mPosition;
		mDisappearPosition = mDownPosition;
		return;
	}

	++mEMDoingTimer;
	mDownPosition      = mPosition;
	mDisappearPosition = mDownPosition;
	if (gpMarDirector->getCurrentMap() != 1
	    && mEMDoingTimer > mSettingParams->mDownTime.get()) {
		mWaterCounter = mSettingParams->mWaterCtMax.get();
		changeEMDoing(EM_DOING_RUN_AWAY);
	}
}

void TEnemyMario::startRunAway() { changeEMDoing(EM_DOING_RUN_AWAY); }

void TEnemyMario::startGateDrawing()
{
	changePlayerStatus(0x133E, 0, true);
	setAnimation(ANIM_DRAW, 1.0f);
	mEMDoingTimer = 0;
	mEMDoing      = EM_DOING_GATE_DRAWING;
	startSoundActor(0x1980);
}

void TEnemyMario::hitWater(THitActor* sender)
{
	if (mSpecialModel != nullptr) {
		return;
	}
	if (mSettingParams->mInvincibleFlag.get()) {
		return;
	}

	if (mEMDoing != EM_DOING_REPLAY && mEMDoing != EM_DOING_SLEEPING
	    && mEMDoing != EM_DOING_TRAMPLED) {
		return;
	}

	mWaterHitTimer = 600;
	if (mWaterCounter > 0) {
		--mWaterCounter;
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT,
		                             &sender->mPosition, 0, nullptr);
		SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK,
		                              &sender->mPosition, 0, 0.0f, 0, 0, 4);
		mWaterEffectTimer = mWaterEffectTimerMax;

		if (mEMDoing == EM_DOING_SLEEPING) {
			sleepingEffectKill();
			int nodeIndex
			    = mEMario->getTracer()->getGraph()->findNearestNodeIndex(
			        mPosition, -1);
			if (mEMario->getTracer()->getGraph()->getGraphNode(nodeIndex).checkFlag(
			        2)) {
				mFaceAngle.y = mAngleToMario;
				unk108->mFrameInput |= TMarioControllerWork::A;
				unk108->mInput |= TMarioControllerWork::A;
			}
			changeEMDoing(EM_DOING_TRAMPLED);
		}
		return;
	}

	if (mStatus == MARIO_STATUS_RUN && canSleep()) {
		if (mHeldObject != nullptr) {
			((TLiveActor*)mHeldObject)->offLiveFlag(LIVE_FLAG_UNK100000);
			dropObject();
		}
		changeEMDoing(EM_DOING_DOWN_ANIMATION);
	}
}

u8 TEnemyMario::thinkTrample()
{
	if (mSpecialModel != nullptr) {
		return FALSE;
	}

	if (mEMDoing == EM_DOING_DOWN_WAIT_TO_TALK) {
		--mTrampleCount;
		if (mTrampleCount > 0) {
			changeEMDoing(EM_DOING_TRAMPLED);
		}

		return TRUE;
	}

	return FALSE;
}

void TEnemyMario::reachGoal()
{
	mGoalFlags |= EM_GOAL_FLAG_REACHED;
	changeEMDoing(EM_DOING_GOAL);
}

void TEnemyMario::checkReturn()
{
	if (!mGroundPlane->checkFlag(BG_CHECK_FLAG_ILLEGAL)) {
		return;
	}

	TGraphWeb* graph = mEMario->unk124->getGraph();
	int nodeIndex    = graph->findNearestNodeIndex(mPosition, -1);
	bool searching   = true;
	while (searching) {
		Vec point;
		graph->getGraphNode(nodeIndex).getPoint(&point);

		if (JGeometry::TVec3<f32>(point - *gpMarioPos).length() > 1000.0f) {
			searching = false;
			mPosition = point;
		}

		nodeIndex = (nodeIndex + 1) % graph->getNodeNum();
	}
}

void TEnemyMario::checkController(JDrama::TGraphics*)
{
	f32 dx        = gpMarioPos->x - mPosition.x;
	f32 dz        = gpMarioPos->z - mPosition.z;
	mAngleToMario = matan(dz, dx);
	mDistanceToMario = std::sqrtf(dx * dx + dz * dz);

	u32 previousInput = unk108->mInput;
	unk108->mStickHS16 = 0;
	unk108->mStickVS16 = 0;
	unk108->mInput = 0;
	unk108->mFrameInput = 0;
	unk108->mAnalogRU8 = 0;
	unk108->mAnalogLU8 = 0;
	consider();

	unk108->mStickH = 0.0f;
	unk108->mStickV = 0.0f;
	if (unk108->mStickHS16 < -7) {
		unk108->mStickH = unk108->mStickHS16 + 6;
	}
	if (unk108->mStickHS16 > 7) {
		unk108->mStickH = unk108->mStickHS16 - 6;
	}
	if (unk108->mStickVS16 < -7) {
		unk108->mStickV = unk108->mStickVS16 + 6;
	}
	if (unk108->mStickVS16 > 7) {
		unk108->mStickV = unk108->mStickVS16 - 6;
	}

	unk108->mStickDist
	    = std::sqrtf(unk108->mStickH * unk108->mStickH
	                 + unk108->mStickV * unk108->mStickV);
	if (unk108->mStickDist > 64.0f) {
		unk108->mStickH *= 64.0f / unk108->mStickDist;
		unk108->mStickV *= 64.0f / unk108->mStickDist;
		unk108->mStickDist = 64.0f;
	}
	unk108->mFrameInput = unk108->mInput & (unk108->mInput ^ previousInput);

	f32 stickRatio = unk108->mStickDist * (1.0f / 64.0f);
	mIntendedMag = 64.0f * (stickRatio * stickRatio) * 0.5f;
	if (mIntendedMag > 0.0f) {
		mIntendedYaw = matan(-unk108->mStickV, unk108->mStickH);
	} else {
		mIntendedYaw = mFaceAngle.y;
	}

	if (mEMDoing == 0xB) {
		emReplay();
	}
	if (mEMDoing == EM_DOING_REPLAY_RUN_AWAY_TO_GATE) {
		mInputReplays[mReplayIndex]->play(
		    &mIntendedMag, &mIntendedYaw, &unk108->mInput,
		    &unk108->mFrameInput, &unk108->mAnalogLU8, &unk108->mAnalogRU8);
		if (!mInputReplays[mReplayIndex]->canPlay()) {
			changePlayerStatus(0x133E, 0, true);
			setAnimation(ANIM_MONTEMAN_WAIT, 1.0f);
			changeEMDoing(EM_DOING_WAITING_TO_INVITE_MARIO);
		}
	}
	if (mEMDoing == EM_DOING_REPLAY_TO_GATE) {
		mGateReplay->play(&mIntendedMag, &mIntendedYaw, &unk108->mInput,
		                  &unk108->mFrameInput, &unk108->mAnalogLU8,
		                  &unk108->mAnalogRU8);
		if (!mGateReplay->canPlay()) {
			changePlayerStatus(0x133E, 0, true);
			setAnimation(ANIM_MONTEMAN_WAIT, 1.0f);
			changeEMDoing(EM_DOING_REACHED_GATE);
		}
	}
	if (mEMDoing == EM_DOING_REPLAY_TO_GOAL) {
		mInputReplays[mReplayIndex]->play(
		    &mIntendedMag, &mIntendedYaw, &unk108->mInput,
		    &unk108->mFrameInput, &unk108->mAnalogLU8, &unk108->mAnalogRU8);
		if (!mInputReplays[mReplayIndex]->canPlay()) {
			mGoalFlags |= 1;
			changeEMDoing(EM_DOING_GOAL);
		}
	}

	if (mIntendedMag > 0.0f) {
		mInput |= 1;
	}
	if (unk108->isAHit()) {
		mInput |= 2;
	}
	if (unk108->isAPressed()) {
		mInput |= 0x80;
	}
	if (unk108->isBPressed()) {
		mInput |= 0x4000;
	}
	if (unk108->isBHit()) {
		mInput |= 0x8000;
	}
}

void TEnemyMario::playerControl(JDrama::TGraphics* graphics)
{
	unk9C  = mFaceAngle.y;
	unk29C = mPosition;
	offUnk114(UNK114_FLAG_UNK8);
	checkPlayerAction(graphics);
	stateMachine();
	stateMachineUpper();
	thinkSituation();
	thinkWaterSurface();
	thinkSand();
	thinkHeight();
	thinkParams();
	checkRideReCalc();
	checkWet();
}

void TEnemyMario::damageExec(THitActor*, int, int, int, f32, int, f32, s16) { }
