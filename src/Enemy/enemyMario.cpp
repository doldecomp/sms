
// This file uses reverse MAP definition order because Enemy is built with
// -inline deferred.
#include <Enemy/EnemyMario.hpp>
#include <Enemy/Emario.hpp>
#include <Enemy/Graph.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Player/MarioRecord.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <System/MSoundMainSide.hpp>
#include <math.h>

void TEnemyMario::changeEMDoing(u16 doing)
{
	mEMDoingTimer = 0;
	mEMDoing      = doing;
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

void TEnemyMario::startRunAway() { changeEMDoing(EM_DOING_RUN_AWAY); }

void TEnemyMario::startGateDrawing()
{
	changePlayerStatus(0x133E, 0, true);
	setAnimation(ANIM_DRAW, 1.0f);
	mEMDoingTimer = 0;
	mEMDoing      = EM_DOING_GATE_DRAWING;
	startSoundActor(0x1980);
}

u8 TEnemyMario::thinkTrample()
{
	if (mEMFlags != 0) {
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
	mGoalFlags |= 1;
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

void TEnemyMario::damageExec(THitActor*, int, int, int, f32, int, f32, s16) { }
