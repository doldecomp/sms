
// This file uses reverse MAP definition order because Enemy is built with
// -inline deferred.
#include <Enemy/EnemyMario.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Emario.hpp>
#include <Enemy/Graph.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/M3UModelMario.hpp>
#include <Map/MapData.hpp>
#include <Map/PollutionManager.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <Player/MarioRecord.hpp>
#include <Strategic/question.hpp>
#include <System/Application.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <System/MSoundMainSide.hpp>
#include <System/Particles.hpp>
#include <math.h>

void TEnemyMario::setStickToAngle(s16 angle, f32 power)
{
	unk108->mStickHS16 = (JMASSin(angle) * 64.0f) * power;
	unk108->mStickVS16 = (-JMASCos(angle) * 64.0f) * power;
}

void TEnemyMario::startMonteReplay(u32 replayIndex)
{
	int nodeIndex
	    = mEMario->getTracer()->getGraph()->findNearestNodeIndex(mPosition, -1);
	JGeometry::TVec3<f32> currentPoint;
	mEMario->getTracer()->getGraph()->getGraphNode(nodeIndex).getPoint(
	    &currentPoint);
	mPosition = currentPoint;
	JGeometry::TVec3<f32> nextPoint;
	mEMario->getTracer()
	    ->getGraph()
	    ->getGraphNode(nodeIndex + 1)
	    .getPoint(&nextPoint);
	f32 xDifference = nextPoint.x - currentPoint.x;
	f32 zDifference = nextPoint.z - currentPoint.z;
	mFaceAngle.y    = matan(zDifference, xDifference);
	mVel.x          = 0.0f;
	mVel.y          = 0.0f;
	mVel.z          = 0.0f;
	mForwardVel     = 0.0f;
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

u8 TEnemyMario::tryTake()
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

void TEnemyMario::emJumping()
{
	if (mStatus & MARIO_STATUS_FLAG_JUMPING) {
		if (mStatus != MARIO_STATUS_WALL_SLIDE || mStatusTimer >= 10) {
			setStickToAngle(mFaceAngle.y, 1.0f);
			unk108->mInput |= TMarioControllerWork::A;
			if (-1.0f < mVel.y && mVel.y < 1.0f && rand() < 0xFFF) {
				unk108->mInput |= TMarioControllerWork::B;
			}
		}
	} else if (mStatus == MARIO_STATUS_HANGING) {
		if (mStatusTimer >= 10) {
			unk108->mInput |= TMarioControllerWork::A;
		}
	} else if (mStatus & 0x600) {
		TPollutionManager* pollution = gpPollution;
		pollution->stamp(1, mPosition.x, mPosition.y, mPosition.z, 384.0f);
		changeEMDoing(EM_DOING_WAITING);
	}
}

void TEnemyMario::emWalkAround()
{
	if (mDistanceToMario < 1500.0f) {
		changeEMDoing(EM_DOING_WAITING);
		return;
	}
	if (rand() < 10) {
		changeEMDoing(EM_DOING_RUN_AWAY_FROM_MARIO);
	}
	if (rand() < 100) {
		unk108->mInput |= TMarioControllerWork::A;
		changeEMDoing(EM_DOING_JUMPING);
		return;
	}
	if (rand() < 100) {
		mTargetAngle = (s16)rand();
		changeEMDoing(EM_DOING_TURNING);
		return;
	}
	if (rand() < 50) {
		mEMario->getTracer()->reset();
		mEMario->goToShortestNextGraphNode();
		changeEMDoing(EM_DOING_WALK_GRAPH);
		return;
	}
	if (rand() < 50) {
		TPollutionManager* pollution = gpPollution;
		pollution->stamp(1, mPosition.x, mPosition.y, mPosition.z, 384.0f);
		changeEMDoing(EM_DOING_HIDDEN);
	}
	if (mWallPlane != nullptr) {
		unk108->mInput |= TMarioControllerWork::A;
		changeEMDoing(EM_DOING_JUMPING);
		return;
	}
	setStickToAngle(mFaceAngle.y, 0.5f);
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
	gpMarioParticleManager->emitAndBindToPosPtr(0x1AA, &mDisappearPosition, 1,
	                                            this);
	gpMarioParticleManager->emitAndBindToPosPtr(0x1AB, &mDisappearPosition, 1,
	                                            this);

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
	mInputReplays[mReplayIndex]->play(&mIntendedMag, &mIntendedYaw,
	                                  &unk108->mInput, &unk108->mFrameInput,
	                                  &unk108->mAnalogLU8, &unk108->mAnalogRU8);

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

	int nodeIndex
	    = mEMario->getTracer()->getGraph()->findNearestNodeIndex(mPosition, -1);
	if (mEMario->getTracer()->getGraph()->getGraphNode(nodeIndex).checkFlag(
	        0x40)) {
		changeEMDoing(EM_DOING_GOAL);
		return;
	}

	if (mSettingParams->mStopFlag.get() == 1) {
		changeEMDoing(EM_DOING_SLEEPING);
		return;
	}

	nodeIndex
	    = mEMario->getTracer()->getGraph()->findNearestNodeIndex(mPosition, -1);
	if (mEMario->getTracer()->getGraph()->getGraphNode(nodeIndex).checkFlag(
	        2)) {
		mFaceAngle.y = mAngleToMario;
		unk108->mFrameInput |= TMarioControllerWork::A;
		unk108->mInput |= TMarioControllerWork::A;
	}
	changeEMDoing(EM_DOING_TRAMPLED);
}

void TEnemyMario::emReplayJumpToNearestNode()
{
	TGraphWeb* graph = mEMario->getTracer()->getGraph();
	int nodeIndex    = graph->findNearestNodeIndex(mPosition, -1);
	if (graph->getGraphNode(nodeIndex).checkFlag(2)) {
		unk108->mFrameInput |= TMarioControllerWork::A;
		unk108->mInput |= TMarioControllerWork::A;
		if (mVel.y > mReplayJumpSpeed) {
			mVel.y = mReplayJumpSpeed;
		}
	}

	++mEMDoingTimer;
	nodeIndex               = graph->findNearestNodeIndex(mPosition, -1);
	TGraphNode* currentNode = &graph->getGraphNode(nodeIndex);
	JGeometry::TVec3<f32> currentPoint;
	currentNode->getPoint(&currentPoint);
	mPosition.x += 0.05f * (currentPoint.x - mPosition.x);
	mPosition.z += 0.05f * (currentPoint.z - mPosition.z);
	mPosition.y += 0.05f * (currentPoint.y - mPosition.y);

	if (mStatus != MARIO_STATUS_WAIT && currentNode->checkFlag(2)) {
		return;
	}

	mPosition = currentPoint;
	mVel.set(0.0f, 0.0f, 0.0f);
	mForwardVel = 0.0f;
	resetHistory();
	changePlayerStatus(MARIO_STATUS_WAIT, 0, true);
	currentNode->getPoint(&mPosition);

	JGeometry::TVec3<f32> marioDirection = *gpMarioPos - currentPoint;
	marioDirection.normalize();
	TGraphNode* nextNode = nullptr;

	if (mSettingParams->mRandomFlag.get() == 0) {
		f32 smallestDot = 1.0f;
		for (int i = 0; i < 3; ++i) {
			TReplayLink& link = mReplayLinks[nodeIndex][i];
			if (link.mNodeIndex == 0xFF) {
				continue;
			}

			TGraphNode* candidate = &graph->getGraphNode(link.mNodeIndex);
			JGeometry::TVec3<f32> candidatePoint;
			candidate->getPoint(&candidatePoint);
			JGeometry::TVec3<f32> candidateDirection
			    = candidatePoint - currentPoint;
			candidateDirection.normalize();
			f32 dot = marioDirection.dot(candidateDirection);
			if (dot < smallestDot) {
				smallestDot  = dot;
				nextNode     = candidate;
				mReplayIndex = link.mReplayIndex;
			}
		}
	} else {
		f32 dots[3];
		int validLinks[3];
		int validCount = 0;
		for (int i = 0; i < 3; ++i) {
			dots[i]           = 0.0f;
			TReplayLink& link = mReplayLinks[nodeIndex][i];
			if (link.mNodeIndex == 0xFF) {
				continue;
			}

			JGeometry::TVec3<f32> candidatePoint;
			graph->getGraphNode(link.mNodeIndex).getPoint(&candidatePoint);
			JGeometry::TVec3<f32> candidateDirection
			    = candidatePoint - currentPoint;
			candidateDirection.normalize();
			dots[validCount]       = marioDirection.dot(candidateDirection);
			validLinks[validCount] = i;
			++validCount;
		}

		f32 weights[3];
		f32 weightTotal = 0.0f;
		for (int i = 0; i < validCount; ++i) {
			weights[i] = powf(1.0f - dots[i], mSettingParams->mRandomPow.get());
			weightTotal += weights[i];
		}
		for (int i = 0; i < validCount; ++i) {
			weights[i] /= weightTotal;
		}

		f32 choice   = MsRandF();
		int selected = 0;
		for (int i = 0; i < validCount; ++i) {
			choice -= weights[i];
			if (choice <= 0.0f) {
				selected = i;
				break;
			}
		}

		TReplayLink& link = mReplayLinks[nodeIndex][validLinks[selected]];
		mReplayIndex      = link.mReplayIndex;
		nextNode          = &graph->getGraphNode(link.mNodeIndex);
	}

	JGeometry::TVec3<f32> nextPoint;
	if (nextNode != nullptr) {
		nextNode->getPoint(&nextPoint);
	}
	mPosition = currentPoint;
	mFaceAngle.y
	    = matan(nextPoint.z - currentPoint.z, nextPoint.x - currentPoint.x);
	mVel.set(0.0f, 0.0f, 0.0f);
	mForwardVel = 0.0f;
	resetHistory();
	changePlayerStatus(MARIO_STATUS_WAIT, 0, true);
	mInputReplays[mReplayIndex]->reset();
	mInputReplays[mReplayIndex]->start();
	changeEMDoing(EM_DOING_REPLAY);
}

void TEnemyMario::emDownAnimation()
{
	changePlayerStatus(0x133E, 0, true);
	setAnimation(ANIM_FALL_DOWN_WAIT, 1.0f);

	if (gpMarDirector->isDemoModeNow() || gpMarDirector->isTalkModeNow()) {
		mReferencePosition = mPosition;
		mDisappearPosition = mReferencePosition;
		return;
	}

	++mEMDoingTimer;
	mReferencePosition = mPosition;
	mDisappearPosition = mReferencePosition;
	if (gpMarDirector->getCurrentMap() != 1
	    && mEMDoingTimer > mSettingParams->mDownTime.get()) {
		mWaterCounter = mSettingParams->mWaterCtMax.get();
		changeEMDoing(EM_DOING_RUN_AWAY);
	}
}

void TEnemyMario::emRunAwayToNearestNode()
{
	TGraphWeb* graph = mEMario->getTracer()->getGraph();
	JGeometry::TVec3<f32> targetPoint;
	graph->getGraphNode(mRunAwayNodeIndex).getPoint(&targetPoint);
	gpMarioParticleManager->emitAndBindToPosPtr(0x1AA, &mDisappearPosition, 1,
	                                            this);
	gpMarioParticleManager->emitAndBindToPosPtr(0x1AB, &mDisappearPosition, 1,
	                                            this);

	if (mEMDoingTimer >= 8 && mEMDoingTimer < 300) {
		mGoalFlags &= ~EM_GOAL_FLAG_DISP_PENCIL;
	} else {
		mGoalFlags |= EM_GOAL_FLAG_DISP_PENCIL;
	}

	switch (mEMDoingTimer) {
	case 0:
		findRunAwayNearestNode();
		mDisappearPosition = mPosition;
		mDisappearPosition.y += 80.0f;
		gpMarioParticleManager->emit(0xED, &mDisappearPosition, 0, nullptr);
		SMSGetMSound()->startSoundActor(MSD_SE_MA_KAGE_FIELD_AWAY, &mPosition,
		                                0, nullptr, 0, 4);
		break;
	case 8:
		break;
	case 100: {
		JGeometry::TVec3<f32> direction = targetPoint - mReferencePosition;
		direction.normalize();
		direction.scale(mRunAwaySpeed);
		mDisappearPosition += direction;
		f32 dx = targetPoint.x - mDisappearPosition.x;
		f32 dz = targetPoint.z - mDisappearPosition.z;
		if (dx * dx + dz * dz < mRunAwaySpeed * mRunAwaySpeed) {
			mEMDoingTimer = 200;
		}
		--mEMDoingTimer;
		break;
	}
	case 200:
		mPosition = targetPoint;
		mPosition.y += 5.0f;
		break;
	case 220:
		gpMarioParticleManager->emit(0xED, &mDisappearPosition, 0, nullptr);
		SMSGetMSound()->startSoundActor(MSD_SE_MA_KAGE_FIELD_APPEAR, &mPosition,
		                                0, nullptr, 0, 4);
		break;
	case 300:
		if (gpMarDirector->getCurrentMap() == 1) {
			JGeometry::TVec3<f32> waitingPoint;
			graph->getGraphNode(7).getPoint(&waitingPoint);
			mFaceAngle.y    = matan(waitingPoint.z - targetPoint.z,
			                        waitingPoint.x - targetPoint.x);
			mModelFaceAngle = mFaceAngle.y;
			mPosition       = targetPoint;
			mPosition.y += 5.0f;
			changePlayerStatus(MARIO_STATUS_WAIT, 0, true);
			mReplayIndex  = mRunAwayNodeIndex;
			mInputReplays = mRunAwayInputReplays;
			mInputReplays[mReplayIndex]->reset();
			mInputReplays[mReplayIndex]->start();
			changeEMDoing(EM_DOING_REPLAY_RUN_AWAY_TO_GATE);
		} else {
			mPosition = targetPoint;
			mPosition.y += 5.0f;
			changePlayerStatus(MARIO_STATUS_WAIT, 0, true);
			int nodeIndex = graph->findNearestNodeIndex(mPosition, -1);
			if (graph->getGraphNode(nodeIndex).checkFlag(2)) {
				mFaceAngle.y = mAngleToMario;
				unk108->mFrameInput |= TMarioControllerWork::A;
				unk108->mInput |= TMarioControllerWork::A;
			}
			changeEMDoing(EM_DOING_TRAMPLED);
		}
		break;
	}
	++mEMDoingTimer;
}

void TEnemyMario::findRunAwayNearestNode()
{
	TGraphWeb* graph    = mEMario->getTracer()->getGraph();
	s16 nearestIndex    = 0;
	s16 secondIndex     = 0;
	f32 nearestDistance = 100000.0f;
	f32 secondDistance  = 100000.0f;
	JGeometry::TVec3<f32> nearestPoint;
	JGeometry::TVec3<f32> secondPoint;

	for (s16 i = 0; i < graph->getNodeNum(); ++i) {
		JGeometry::TVec3<f32> point;
		graph->getGraphNode(i).getPoint(&point);
		f32 distance = (point - mPosition).length();
		if (distance < nearestDistance) {
			secondDistance  = nearestDistance;
			secondIndex     = nearestIndex;
			secondPoint     = nearestPoint;
			nearestDistance = distance;
			nearestIndex    = i;
			nearestPoint    = point;
		} else if (distance < secondDistance) {
			secondDistance = distance;
			secondIndex    = i;
			secondPoint    = point;
		}
	}

	if ((mPosition - nearestPoint).length()
	    < (mPosition - secondPoint).length()) {
		mRunAwayNodeIndex = nearestIndex;
	} else {
		mRunAwayNodeIndex = secondIndex;
	}
}

void TEnemyMario::startRunAway() { changeEMDoing(EM_DOING_RUN_AWAY); }

void TEnemyMario::decideDoingAfterCarry()
{
	if (isEnforceTake()) {
		mGoalFlags &= ~EM_GOAL_FLAG_ENFORCE_TAKE;
		TGraphWeb* graph = mEMario->getTracer()->getGraph();
		int nodeIndex    = graph->findNearestNodeIndex(mPosition, -1);
		if (graph->getGraphNode(nodeIndex).checkFlag(2)) {
			mFaceAngle.y = mAngleToMario;
			unk108->mFrameInput |= TMarioControllerWork::A;
			unk108->mInput |= TMarioControllerWork::A;
		}
		changeEMDoing(EM_DOING_TRAMPLED);
		return;
	}

	if (mSettingParams->mStopFlag.get() == 1) {
		changeEMDoing(EM_DOING_SLEEPING);
		return;
	}

	TGraphWeb* graph = mEMario->getTracer()->getGraph();
	int nodeIndex    = graph->findNearestNodeIndex(mPosition, -1);
	if (graph->getGraphNode(nodeIndex).checkFlag(2)) {
		mFaceAngle.y = mAngleToMario;
		unk108->mFrameInput |= TMarioControllerWork::A;
		unk108->mInput |= TMarioControllerWork::A;
	}
	changeEMDoing(EM_DOING_TRAMPLED);
}

void TEnemyMario::emWaitingToInviteMario()
{
	TGraphWeb* graph = mEMario->getTracer()->getGraph();
	JGeometry::TVec3<f32> waitingPoint;
	graph->getGraphNode(7).getPoint(&waitingPoint);
	mPosition           = waitingPoint;
	s16 angleDifference = mAngleToMario - mFaceAngle.y;
	mFaceAngle.y = mAngleToMario - IConverge(angleDifference, 0, 0x180, 0x180);
	changePlayerStatus(MARIO_STATUS_WAIT, 0, false);
	changeMontemanWaitingAnim();

	f32 dx = mPosition.x - gpMarioPos->x;
	f32 dy = mPosition.y - gpMarioPos->y;
	f32 dz = mPosition.z - gpMarioPos->z;
	if (std::sqrtf(dx * dx + dy * dy + dz * dz)
	        < mSettingParams->mSearchDist.get()
	    && gpMarioPos->y < mPosition.y + mSettingParams->mSearchHeight.get()) {
		JGeometry::TVec3<f32> gatePoint;
		graph->getGraphNode(8).getPoint(&gatePoint);
		mFaceAngle.y
		    = matan(gatePoint.z - waitingPoint.z, gatePoint.x - waitingPoint.x);
		mModelFaceAngle = mFaceAngle.y;
		changePlayerStatus(MARIO_STATUS_WAIT, 0, true);
		mReplayIndex = 0;
		mGateReplay->reset();
		mGateReplay->start();
		changeEMDoing(EM_DOING_REPLAY_TO_GATE);
	}
}

void TEnemyMario::startGateDrawing()
{
	changePlayerStatus(0x133E, 0, true);
	setAnimation(ANIM_DRAW, 1.0f);
	mEMDoingTimer = 0;
	mEMDoing      = EM_DOING_GATE_DRAWING;
	startSoundActor(0x1980);
}

void TEnemyMario::consider()
{
	switch (mEMDoing) {
	case EM_DOING_WAITING:
		emWaiting();
		break;
	case EM_DOING_GET_CLOSER:
		if (mDistanceToMario < 400.0f) {
			unk108->mInput |= TMarioControllerWork::A;
			changeEMDoing(EM_DOING_JUMPING);
		}
		if (mDistanceToMario < 1300.0f) {
			setStickToAngle(mAngleToMario, 1.0f);
			unk108->mStickHS16 = -unk108->mStickHS16;
			unk108->mStickVS16 = -unk108->mStickVS16;
		} else {
			mEMDoing = EM_DOING_WAITING;
		}
		break;
	case EM_DOING_JUMPING:
		emJumping();
		break;
	case EM_DOING_RUN_AWAY_FROM_MARIO:
		if (mDistanceToMario > 1500.0f) {
			setStickToAngle(mAngleToMario, 1.0f);
		} else {
			changeEMDoing(EM_DOING_WAITING);
		}
		break;
	case EM_DOING_WALK_AROUND:
		emWalkAround();
		break;
	case EM_DOING_WALK_GRAPH: {
		JGeometry::TVec3<f32> distance
		    = mEMario->getUnk104().getPoint() - mEMario->mPosition;
		if (distance.length() < 100.0f) {
			if (mDistanceToMario > 3000.0f) {
				mEMario->goToRandomNextGraphNode();
			} else {
				mEMario->goToRandomEscapeGraphNode();
			}
		}
		const JGeometry::TVec3<f32>& goal = mEMario->getUnkF4().getPoint();
		u16 angle = matan(goal.z - mPosition.z, goal.x - mPosition.x);
		setStickToAngle(angle, 1.0f);
		++mEMDoingTimer;
		if (mEMDoingTimer % 100 == 0) {
			gpConductor->makeEnemyAppear(mPosition, "ハムクリマネージャー", 1,
			                             0);
		}
		break;
	}
	default:
		return;
	case EM_DOING_TURNING: {
		s16 angleDifference = mTargetAngle - mFaceAngle.y;
		if (rand() < 100) {
			unk108->mInput |= TMarioControllerWork::A;
			changeEMDoing(EM_DOING_JUMPING);
		} else if (angleDifference < -0x1555 || angleDifference > 0x1555) {
			setStickToAngle(mTargetAngle, 0.2f);
		} else {
			changeEMDoing(EM_DOING_WAITING);
		}
		break;
	}
	case EM_DOING_HIDDEN:
		onHitFlag(HIT_FLAG_NO_COLLISION);
		mEMario->onHitFlag(HIT_FLAG_NO_COLLISION);
		++mEMDoingTimer;
		if (!gpPollution->isPolluted(mPosition.x, mPosition.y, mPosition.z)
		    || mEMDoingTimer > 7200) {
			mEMDoingTimer        = 0;
			mInvincibilityFrames = 120;
			changeEMDoing(EM_DOING_APPEAR);
		}
		break;
	case EM_DOING_APPEAR:
		if (mInvincibilityFrames == 0) {
			changeEMDoing(EM_DOING_WAITING);
			offHitFlag(HIT_FLAG_NO_COLLISION);
			mEMario->offHitFlag(HIT_FLAG_NO_COLLISION);
		}
		break;
	case EM_DOING_DISAPPEAR:
		onHitFlag(HIT_FLAG_NO_COLLISION);
		mEMario->onHitFlag(HIT_FLAG_NO_COLLISION);
		mGoalFlags &= ~EM_GOAL_FLAG_DISP_PENCIL;
		offUnk114(UNK114_FLAG_VISIBLE);
		changePlayerStatus(0x133E, 0, false);
		break;
	case EM_DOING_DISAPPEAR_TO_GATE:
		emDisappearToGate();
		break;
	case EM_DOING_SLEEPING: {
		f32 dx = mPosition.x - gpMarioPos->x;
		f32 dy = mPosition.y - gpMarioPos->y;
		f32 dz = mPosition.z - gpMarioPos->z;
		if (std::sqrtf(dx * dx + dy * dy + dz * dz)
		        < mSettingParams->mSearchDist.get()
		    && gpMarioPos->y
		           < mPosition.y + mSettingParams->mSearchHeight.get()) {
			TGraphWeb* graph = mEMario->getTracer()->getGraph();
			int nodeIndex    = graph->findNearestNodeIndex(mPosition, -1);
			if (graph->getGraphNode(nodeIndex).checkFlag(2)) {
				mFaceAngle.y = mAngleToMario;
				unk108->mFrameInput |= TMarioControllerWork::A;
				unk108->mInput |= TMarioControllerWork::A;
			}
			changeEMDoing(EM_DOING_TRAMPLED);
		}
		break;
	}
	case EM_DOING_TRAMPLED:
		emReplayJumpToNearestNode();
		break;
	case EM_DOING_DOWN_ANIMATION:
		changePlayerStatus(0x133E, 0, true);
		setAnimation(ANIM_SDWNF, 1.0f);
		if (getMotionFrameCtrl().getFrame() > 25.0f) {
			changeEMDoing(EM_DOING_DOWN_WAIT_TO_TALK);
		}
		break;
	case EM_DOING_DOWN_WAIT_TO_TALK:
		emDownAnimation();
		break;
	case EM_DOING_RUN_AWAY:
		emRunAwayToNearestNode();
		break;
	case EM_DOING_GOAL: {
		s16 angleDifference = mAngleToMario - mFaceAngle.y;
		mFaceAngle.y
		    = mAngleToMario - IConverge(angleDifference, 0, 0x180, 0x180);
		changePlayerStatus(MARIO_STATUS_WAIT, 0, false);
		changeMontemanWaitingAnim();
		break;
	}
	case EM_DOING_KEEP_STAY:
		if (tryTake()) {
			if (mStampActor != nullptr
			    && mSettingParams->mStampFlag.get() == 1) {
				mStampActor->setBck("stamp_koopa_sign_draw1");
				changeEMDoing(EM_DOING_DRAW_STAMP);
			} else {
				decideDoingAfterCarry();
			}
		}
		break;
	case EM_DOING_DRAW_STAMP:
		if (mStampActor->curAnmEndsNext()) {
			decideDoingAfterCarry();
		}
		break;
	case EM_DOING_WAITING_TO_INVITE_MARIO:
		emWaitingToInviteMario();
		break;
	case EM_DOING_REACHED_GATE:
	case EM_DOING_REPLAY_WAITING: {
		s16 angleDifference = mAngleToMario - mFaceAngle.y;
		mFaceAngle.y
		    = mAngleToMario - IConverge(angleDifference, 0, 0x180, 0x180);
		break;
	}
	case EM_DOING_GATE_DRAWING:
		mEMario->getTracer()->getGraph()->getGraphNode(8).getPoint(&mPosition);
		mFaceAngle.y    = -0x8000;
		mModelFaceAngle = -0x8000;
		if (isLast1AnimeFrame()) {
			startDisappear(EM_DOING_DISAPPEAR_TO_GATE);
		}
		break;
	case EM_DOING_GET_PAD: {
		JDrama::TGraphics graphics;
		TMario::checkController(&graphics);
		break;
	}
	}
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
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                             0, nullptr);
		SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK,
		                              &sender->mPosition, 0, 0.0f, 0, 0, 4);
		mWaterEffectTimer = mWaterEffectTimerMax;

		if (mEMDoing == EM_DOING_SLEEPING) {
			sleepingEffectKill();
			int nodeIndex
			    = mEMario->getTracer()->getGraph()->findNearestNodeIndex(
			        mPosition, -1);
			if (mEMario->getTracer()
			        ->getGraph()
			        ->getGraphNode(nodeIndex)
			        .checkFlag(2)) {
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
	f32 dx           = gpMarioPos->x - mPosition.x;
	f32 dz           = gpMarioPos->z - mPosition.z;
	mAngleToMario    = matan(dz, dx);
	mDistanceToMario = std::sqrtf(dx * dx + dz * dz);

	u32 previousInput   = unk108->mInput;
	unk108->mStickHS16  = 0;
	unk108->mStickVS16  = 0;
	unk108->mInput      = 0;
	unk108->mFrameInput = 0;
	unk108->mAnalogRU8  = 0;
	unk108->mAnalogLU8  = 0;
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

	unk108->mStickDist = std::sqrtf(unk108->mStickH * unk108->mStickH
	                                + unk108->mStickV * unk108->mStickV);
	if (unk108->mStickDist > 64.0f) {
		unk108->mStickH *= 64.0f / unk108->mStickDist;
		unk108->mStickV *= 64.0f / unk108->mStickDist;
		unk108->mStickDist = 64.0f;
	}
	unk108->mFrameInput = unk108->mInput & (unk108->mInput ^ previousInput);

	f32 stickRatio = unk108->mStickDist * (1.0f / 64.0f);
	mIntendedMag   = 64.0f * (stickRatio * stickRatio) * 0.5f;
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
		    &mIntendedMag, &mIntendedYaw, &unk108->mInput, &unk108->mFrameInput,
		    &unk108->mAnalogLU8, &unk108->mAnalogRU8);
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
		    &mIntendedMag, &mIntendedYaw, &unk108->mInput, &unk108->mFrameInput,
		    &unk108->mAnalogLU8, &unk108->mAnalogRU8);
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

void TEnemyMario::perform(u32 cue, JDrama::TGraphics* graphics)
{
	MActor* emarioActor   = nullptr;
	J3DModel* emarioModel = nullptr;
	if (mSpecialModel == nullptr) {
		emarioActor = mEMario->getMActor();
		emarioModel = emarioActor->getModel();
	}

	if (mFreezeTimer > 0) {
		--mFreezeTimer;
	}
	if (mWaterHitTimer > 0) {
		--mWaterHitTimer;
	}

	u32 isMove = cue & CUE_MOVE;
	if (isMove) {
		if (isForceWaterHit()) {
			mWaterCounter = 0;
			hitWater(this);
		}

		if (mStatus != MARIO_STATUS_RUN || mFreezeTimer == 0) {
			playerControl(graphics);
			setPositions();
		}
	}

	if (isMove) {
		if (mStatus != MARIO_STATUS_RUN || mFreezeTimer == 0) {
			calcAnim(CUE_CALC_ANIM, graphics);
		}

		if (mSpecialModel != nullptr) {
			for (u16 i = 0;
			     i < mModel->getModel()->getModelData()->getJointNum(); ++i) {
				MTXCopy(mModel->getModel()->getAnmMtx(i),
				        mSpecialModel->getAnmMtx(i));
			}
			mSpecialModel->calcWeightEnvelopeMtx();
		} else {
			emarioActor->calcAnm();
			for (u16 i = 0;
			     i < mModel->getModel()->getModelData()->getJointNum(); ++i) {
				MTXCopy(mModel->getModel()->getAnmMtx(i),
				        emarioModel->getAnmMtx(i));
			}
			emarioModel->calcWeightEnvelopeMtx();
			MTXCopy(emarioModel->getAnmMtx(mJointIdHandL),
			        mPencilModel->getBaseTRMtx());
			mPencilModel->calc();

			if (isDispStamp()) {
				TPosition3f scaleMtx;
				TRotation3f rotationMtx;
				MTXScale(scaleMtx, unk42F4, unk42F4, unk42F4);
				MsMtxSetRotRPH(rotationMtx, 0.0f, 180.0f, 0.0f);
				MTXConcat(mModel->getModel()->getBaseTRMtx(), scaleMtx,
				          scaleMtx);
				MTXConcat(scaleMtx, rotationMtx, scaleMtx);
				MTXCopy(scaleMtx, mStampActor->getModel()->getBaseTRMtx());
				mStampActor->calcAnm();
			}
		}

		mAttackRadius = 800.0f;
		mAttackHeight = 150.0f;
		mDamageRadius = 60.0f;
		mDamageHeight = 40.0f;
		calcEntryRadius();
		if (mWaterEffectTimer > 0) {
			--mWaterEffectTimer;
		} else {
			mWaterEffectTimer = 0;
		}
	}

	if (cue & CUE_CALC_VIEW) {
		calcView(graphics);
		if (isDispPencil()) {
			unk390->entryDrawShadow();
			gpQuestionManager->request(mPosition, 50.0f);
		}

		if (mSpecialModel != nullptr) {
			mSpecialModel->viewCalc();
		} else {
			emarioActor->viewCalc();
			mPencilModel->viewCalc();
			if (isDispStamp()) {
				mStampActor->viewCalc();
			}
		}
	}

	if (cue & CUE_ENTRY) {
		if (mSpecialModel == nullptr) {
			if (mWaterEffectTimer > 0) {
				SMS_AddDamageFogEffect(
				    mEMario->getMActor()->getModel()->getModelData(), mPosition,
				    graphics);
			} else {
				SMS_ResetDamageFogEffect(
				    mEMario->getMActor()->getModel()->getModelData());
			}
			if (isDispStamp()) {
				gpPollution->stampModel(mStampActor->getModel());
			}
		}

		if (isDispPencil()) {
			if (mSpecialModel == nullptr) {
				emarioActor->entry();
				mPencilModel->entry();
			} else {
				mSpecialModel->entry();
			}
		}
	}

	if (cue & CUE_DRAW) {
		if (mTrembleModelEffect != nullptr) {
			mTrembleModelEffect->movement();
		}

		int doDraw = 1;
		if (!(unk114 & UNK114_FLAG_VISIBLE)) {
			doDraw = 0;
		}
		if (mInvincibilityFrames > 0 && !(mInvincibilityFrames & 4)) {
			doDraw = 0;
		}
		if (checkFlag(MARIO_FLAG_UNK4)) {
			doDraw = 0;
		}
		if (mEMDoing == EM_DOING_HIDDEN) {
			doDraw = 0;
		}
		if (mFreezeTimer > 0 && !(mFreezeTimer & 4)) {
			doDraw = 0;
		}
		if (doDraw == 1 && mTrembleModelEffect != nullptr) {
			j3dSys.setUnk4C(7);
			unk394->draw();
			unk398->draw();
		}

		if ((mGoalFlags & EM_GOAL_FLAG_DISP_HP_METER) && mWaterHitTimer > 0) {
			drawHPMeter(graphics->getViewMtx());
		}
	}
}
