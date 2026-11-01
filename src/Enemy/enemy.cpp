#include <Enemy/Enemy.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Conductor.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Player/MarioAccess.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <Strategic/Spine.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JMath.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TSpineEnemy::TSpineEnemy(const char* name)
    : TLiveActor(name)
    , mGoalPath(4)
    , mStayCount(0)
    , mStayReferenceDistance(0.0f)
    , mSurfaceAlignmentMode(SURFACE_ALIGNMENT_NONE)
    , mDistToMarioSquared(0.0f)
    , unk138(nullptr)
    , mHitPoints(1)
    , mMarchSpeed(1.0f)
    , mTurnSpeed(5.0f)
    , mBodyScale(1.0f)
    , mWallRadius(1.0f)
{
	mSpine = new TSpineBase<TLiveActor>(this);
	unk124 = new TGraphTracer;
}

TSpineEnemy::~TSpineEnemy() { }

void TSpineEnemy::init(TLiveManager* param_1)
{
	TLiveActor::init(param_1);
	TSpineEnemyParams* params = getSaveParam();
	if (params) {
		mBodyRadius       = params->mSLBodyRadius.get();
		mWallRadius       = params->mSLWallRadius.get();
		mHeadHeight       = params->mSLHeadHeight.get();
		mScaledBodyRadius = mBodyScale * mBodyRadius;
	}
}

void TSpineEnemy::reset()
{
	unk124->reset();
	goToShortestNextGraphNode();
	offLiveFlag(LIVE_FLAG_DEAD);
	offLiveFlag(LIVE_FLAG_NO_SHADOW);
	offLiveFlag(LIVE_FLAG_NO_BIND);
	offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
	onLiveFlag(LIVE_FLAG_RECYCLABLE);
	mSpine->reset();
	mSpine->pushAfterCurrent(mSpine->getDefault());
}

void TSpineEnemy::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);

	char buffer[256];
	stream.readString(buffer, 256);
	TLiveManager* mgr
	    = static_cast<TLiveManager*>(JDrama::TNameRefGen::search(buffer));

	char buffer2[256];
	stream.readString(buffer2, 256);
	unk124->init(gpConductor->getGraphByName(buffer2));

	mGroundPlane = TMap::getIllegalCheckData();

	init(mgr);
}

void TSpineEnemy::calcEnemyRootMatrix()
{
	J3DModel* pJVar13 = getModel();
	MtxPtr mtx        = pJVar13->getBaseTRMtx();

	pJVar13->setBaseScale(mScaling);

	if (mSurfaceAlignmentMode == SURFACE_ALIGNMENT_NONE) {
		MsMtxSetRotRPH(mtx, mRotation.x, mRotation.y, mRotation.z);
	} else {
		if (mSurfaceAlignmentMode >= SURFACE_ALIGNMENT_GROUND_AND_WALL
		    && !isAirborne() && unk138 != nullptr) {
			JGeometry::TVec3<f32> v2 = unk138->getNormal();

			JGeometry::TVec3<f32> v1;
			v1.cross(v2, JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f));
			v1.normalize();

			JGeometry::TVec3<f32> v3;
			v3.cross(v1, v2);
			v3.normalize();

			mtx[0][0] = v1.x;
			mtx[1][0] = v1.y;
			mtx[2][0] = v1.z;
			mtx[0][1] = v2.x;
			mtx[1][1] = v2.y;
			mtx[2][1] = v2.z;
			mtx[0][2] = v3.x;
			mtx[1][2] = v3.y;
			mtx[2][2] = v3.z;
			mtx[0][3] = 0.0f;
			mtx[1][3] = 0.0f;
			mtx[2][3] = 0.0f;
		} else {
			if (mSurfaceAlignmentMode >= SURFACE_ALIGNMENT_GROUND
			    && !mGroundPlane->isIllegalData()) {
				JGeometry::TVec3<f32> v1(MsSin(mRotation.y), 0.0f,
				                         MsCos(mRotation.y));

				JGeometry::TVec3<f32> v2 = mGroundPlane->getNormal();
				v1.cross(v2, v1);
				v1.normalize();

				JGeometry::TVec3<f32> v3;
				v3.cross(v1, v2);
				v3.normalize();

				mtx[0][0] = v1.x;
				mtx[1][0] = v1.y;
				mtx[2][0] = v1.z;
				mtx[0][1] = v2.x;
				mtx[1][1] = v2.y;
				mtx[2][1] = v2.z;
				mtx[0][2] = v3.x;
				mtx[1][2] = v3.y;
				mtx[2][2] = v3.z;
				mtx[0][3] = 0.0f;
				mtx[1][3] = 0.0f;
				mtx[2][3] = 0.0f;
			} else {
				MsMtxSetRotRPH(mtx, mRotation.x, mRotation.y, mRotation.z);
			}
		}
	}

	mtx[0][3] = mPosition.x;
	mtx[1][3] = mPosition.y;
	mtx[2][3] = mPosition.z;
}

// TODO: the frame is 8 bytes short (0x28, target 0x30).
void TSpineEnemy::calcRootMatrix()
{
	if (mHolder && mHolder->getHeldObject() == this) {
		MtxPtr src = mHolder->getTakingMtx();
		if (src) {
			getModel()->setBaseTRMtx(src);
			mPosition.set(src[0][3], src[1][3], src[2][3]);
			return;
		}
	}

	calcEnemyRootMatrix();
}

TSpineEnemyParams* TSpineEnemy::getSaveParam() const
{
	return ((TEnemyManager*)mManager)->getSaveParam();
}

void TSpineEnemy::resetToPosition(const JGeometry::TVec3<f32>& position)
{
	mPosition = position;
	offLiveFlag(LIVE_FLAG_NO_SHADOW);
	offLiveFlag(LIVE_FLAG_DEAD);
	reset();
	resetHitPoints();
	offHitFilter(HIT_FILTER_NO_COLLISION);
	mVelocity = JGeometry::TVec3<f32>(0.0f, 5.0f, 0.0f);
	onLiveFlag(LIVE_FLAG_JUMPING);
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	control();
}

void TSpineEnemy::resetSRTV(const JGeometry::TVec3<f32>& position,
                            const JGeometry::TVec3<f32>& rotation,
                            const JGeometry::TVec3<f32>& scaling,
                            const JGeometry::TVec3<f32>& velocity)
{
	mPosition = position;
	offLiveFlag(LIVE_FLAG_NO_SHADOW);
	offLiveFlag(LIVE_FLAG_DEAD);
	reset();
	mRotation = rotation;
	mScaling  = scaling;
	resetHitPoints();
	offHitFilter(HIT_FILTER_NO_COLLISION);
	mVelocity = velocity;
	onLiveFlag(LIVE_FLAG_JUMPING);
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	control();
}

f32 TSpineEnemy::calcMinimumTurnRadius(f32 param_1, f32 param_2) const
{
	f32 result;
	if (param_2 >= 90.0f) {
		result = 0.0f;
	} else {
		f32 thing = MsSin(param_2);
		if (thing == 0.0f)
			result = 100000.0f;
		else
			result = param_1 * MsSin(-(param_2 * 0.5f - 90.0f)) / thing;
	}
	return result;
}

f32 TSpineEnemy::calcTurnSpeedToReach(f32 march_speed, f32 param_2) const
{
	if (param_2 == 0.0f)
		return 0.0f;

	f32 fVar12 = param_2 * 2.0f * param_2;
	f32 asdf   = -(march_speed * march_speed - fVar12) / fVar12;
	f32 dVar11 = MsClamp(asdf, -1.0f, 1.0f);

	if (dVar11 == 1.0f)
		return 0.0f;

	if (dVar11 == -1.0f)
		return 180.0f;

	f32 fVar32 = -(dVar11 * dVar11 - 1.0f);

	// TODO: THitActor::calcEntryRadius has same problem
	volatile f32 f = fVar32 * __frsqrte(fVar32);
	f32 tmp        = matan(f, dVar11) * (360.0f / 65536.0f);
	return 90.0f - tmp;
}

void TSpineEnemy::updateSquareToMario()
{
	// assert?
	(void)&SMS_GetMarioPos();
	mDistToMarioSquared = PSVECSquareDistance(&mPosition, &SMS_GetMarioPos());
}

BOOL TSpineEnemy::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_TAKE && mHolder == nullptr) {
		mHolder = (TTakeActor*)sender;
		return true;
	}

	if (message <= HIT_MESSAGE_PUSH_UP) {
		kill();
		return true;
	}

	return false;
}

BOOL TSpineEnemy::isInSight(const JGeometry::TVec3<f32>& pos, f32 length,
                            f32 angle, f32 aware) const
{
	return MsIsInSight(mPosition, mRotation.y, pos, length, angle, aware);
}

void TSpineEnemy::setGoalPathFromGraph()
{
	JGeometry::TVec3<f32> local_48;
	unk124->getCurrent().getPoint(&local_48);
	setGoal(local_48);
}

void TSpineEnemy::goToInitialVisibleNode(f32, f32) { }

void TSpineEnemy::goToInitialGraphNodeCheckY(f32 param_1) { }

#pragma dont_inline on
int TSpineEnemy::goToShortestNextGraphNode()
{
	if (unk124->unk0 == nullptr)
		return -1;

	if (unk124->mCurrIdx < 0)
		unk124->setTo(unk124->unk0->findNearestNodeIndex(mPosition, -1));
	else
		unk124->moveTo(unk124->unk0->getShortestNextIndex(
		    unk124->mCurrIdx, unk124->mPrevIdx, -1));

	if (unk124->mCurrIdx < 0)
		return -1;

	setGoalPathFromGraph();
	resetStayCount();
	return 0;
}
#pragma dont_inline off

int TSpineEnemy::jumpToNextGraphNode()
{
	if (unk124->mCurrIdx < 0 || !unk124->getCurrent().checkFlag(0x1))
		return -1;

	int idx = unk124->unk0->getNeighborNodeIndexByFlag(unk124->mCurrIdx,
	                                                   unk124->mPrevIdx, 2);
	if (idx >= 0) {
		unk124->moveTo(idx);
		setGoalPathFromGraph();
		resetStayCount();
		return idx;
	}

	return -1;
}

void TSpineEnemy::goToRandomNextGraphNode()
{
	if (unk124->getCurGraphIndex() < 0)
		unk124->setTo(unk124->unk0->findNearestNodeIndex(mPosition, -1));
	else
		unk124->moveTo(unk124->unk0->getRandomNextIndex(
		    unk124->getCurGraphIndex(), unk124->getPrevIndex(), -1));

	setGoalPathFromGraph();
	resetStayCount();
}

void TSpineEnemy::goToRandomEscapeGraphNode()
{
	if (unk124->getCurGraphIndex() < 0)
		unk124->setTo(unk124->unk0->findNearestNodeIndex(mPosition, -1));
	else
		unk124->moveTo(unk124->unk0->getEscapeFromMarioIndex(
		    unk124->getCurGraphIndex(), -1, mPosition, -1));

	setGoalPathFromGraph();
	resetStayCount();
}

void TSpineEnemy::goToExclusiveNextGraphNode()
{
	if (mManager == nullptr) {
		goToRandomNextGraphNode();
	} else {
		if (unk124->mCurrIdx < 0) {
			unk124->setTo(unk124->unk0->findNearestNodeIndex(mPosition, -1));
		} else {
			int idx = unk124->unk0->getRandomNextIndex(
			    unk124->getCurGraphIndex(), unk124->getPrevIndex(), -1);
			for (int i = 0; i < mManager->getObjNum(); ++i) {
				TSpineEnemy* enemy = (TSpineEnemy*)mManager->getObj(i);
				if (this != enemy && idx == enemy->unk124->mCurrIdx)
					idx = unk124->unk0->getRandomNextIndex(
					    unk124->getCurGraphIndex(), idx, -1);
			}
			unk124->moveTo(idx);
		}

		setGoalPathFromGraph();
		resetStayCount();
	}
}

void TSpineEnemy::goToDirectedNextGraphNode(
    const JGeometry::TVec3<f32>& param_1)
{
	int currIdx = unk124->mCurrIdx;
	int prevIdx = unk124->mPrevIdx;
	if (currIdx < 0) {
		unk124->setTo(unk124->unk0->findNearestNodeIndex(mPosition, -1));
	} else {
		int idx = unk124->unk0->getAimToDirNextIndex(currIdx, prevIdx, param_1,
		                                             mPosition, -1);
		unk124->moveTo(idx);
	}

	setGoalPathFromGraph();
	resetStayCount();
}

// TODO: fake
static inline JGeometry::TVec3<f32> polarXZ(f32 theta, f32 radius)
{
	f32 c = radius * MsCos(theta);
	f32 s = radius * MsSin(theta);
	return JGeometry::TVec3<f32>(s, 0.0f, c);
}

void TSpineEnemy::goToDirLimitedNextGraphNode(f32 param_1)
{
	int currIdx = unk124->mCurrIdx;
	int prevIdx = unk124->mPrevIdx;
	if (currIdx < 0) {
		unk124->setTo(unk124->unk0->findNearestNodeIndex(mPosition, -1));
	} else {
		TGraphWeb* web                 = unk124->unk0;
		JGeometry::TVec3<f32> local_3c = polarXZ(mRotation.y, 1.0f);

		int idx = web->getRandomButDirLimited(currIdx, prevIdx, local_3c,
		                                      mPosition, param_1, -1);
		unk124->moveTo(idx);
	}

	setGoalPathFromGraph();
	resetStayCount();
}

void TSpineEnemy::updateStayCount(f32 target_distance)
{
	if (abs(target_distance - mStayReferenceDistance) < 100.0f) {
		mStayCount += 1;
	} else {
		mStayCount             = 0;
		mStayReferenceDistance = target_distance;
	}
}

BOOL TSpineEnemy::turnToCurPathNode(f32 param_1)
{
	JGeometry::TVec3<f32> tmp = getCurrentTarget().getPoint();
	tmp -= mPosition;

	f32 rot = MsAngleDiff(MsGetRotFromZaxisY(tmp), mRotation.y);

	BOOL uVar2 = false;
	if (rot > 0.0f) {
		if (rot < param_1) {
			uVar2 = true;
		} else {
			rot = param_1;
		}
	} else {
		if (rot > -param_1) {
			uVar2 = true;
		} else {
			rot = -param_1;
		}
	}

	mRotation.y = MsWrap(mRotation.y + rot, 0.0f, 360.0f);
	return uVar2;
}

void TSpineEnemy::walkToCurPathNode(f32 march_speed, f32 turn_speed,
                                    f32 yaw_offset)
{
	JGeometry::TVec3<f32> toTarget = getCurrentTarget().getPoint();
	toTarget -= mPosition;

	f32 distToTarget = toTarget.length();
	f32 desiredYaw
	    = MsWrap(yaw_offset + MsGetRotFromZaxisY(toTarget), 0.0f, 360.0f);
	f32 desiredTurn = MsAngleDiff(desiredYaw, mRotation.y);

	f32 deltaYaw = desiredTurn;
	if (distToTarget > calcMinimumTurnRadius(march_speed, turn_speed) * 2.0f) {
		if (desiredTurn > 0.0f) {
			deltaYaw = MsMin(desiredTurn, turn_speed);
		} else {
			deltaYaw = MsMax(desiredTurn, -turn_speed);
		}
	} else {
		f32 turnSpeed = calcTurnSpeedToReach(march_speed, distToTarget * 0.5f);

		if (desiredTurn > 0.0f) {
			deltaYaw = MsMin(desiredTurn, turnSpeed);
		} else {
			deltaYaw = MsMax(desiredTurn, -turnSpeed);
		}
	}

	mRotation.y = MsWrap(mRotation.y + deltaYaw, 0.0f, 360.0f);

	JGeometry::TVec3<f32> vel = mPositionDelta;
	vel += polarXZ(mRotation.y, march_speed);
	mPositionDelta = vel;

	updateStayCount(distToTarget);
}

void TSpineEnemy::zigzagToCurPathNode(f32 march_speed, f32 turn_speed,
                                      f32 cycle, f32 angle)
{
	if (unk124->unk10 == 0.0f) {
		walkToCurPathNode(march_speed, turn_speed, 0.0f);
		return;
	}

	f32 f29 = angle * unk124->unk10;

	f32 distToTarget = getCurrentTargetDistance();

	f32 dVar13 = MsWrap(distToTarget, 0.0f, cycle);

	cycle          = 360.0f * (1.0f / cycle);
	f32 phaseShift = getPhaseShift();
	dVar13 *= cycle;
	phaseShift = dVar13 + phaseShift;
	f29 *= MsSin(phaseShift);

	JGeometry::TVec3<f32> local_58 = mCurrentTarget.getPoint();
	local_58 -= mPosition;
	f32 dVar12 = f29 + MsGetRotFromZaxisY(local_58);

	f32 fVar1 = MsAngleDiff(dVar12, mRotation.y);
	f32 fVar2 = fVar1;

	f32 fVar3 = calcMinimumTurnRadius(march_speed, turn_speed);

	if (distToTarget > fVar3 * 2.0f) {
		if (fVar1 > 0.0f) {
			fVar2 = MsMin(fVar1, turn_speed);
		} else {
			fVar2 = MsMax(fVar1, -turn_speed);
		}
	}

	mRotation.y = MsWrap(mRotation.y + fVar2, 0.0f, 360.0f);

	JGeometry::TVec3<f32> vel = mPositionDelta;
	vel += polarXZ(mRotation.y, march_speed);
	mPositionDelta = vel;

	updateStayCount(distToTarget);
}

void TSpineEnemy::doShortCut()
{
	if (mGoalPath.size() <= 0)
		return;

	if (getCurrentTargetDistance() < getBodyRadius()) {
		switchNextGoalPathNode();
		return;
	}

	TPathNode node;
	node = mGoalPath.peek();

	JGeometry::TVec3<f32> local_28 = node.getPoint() - mPosition;
	if (local_28.x == 0.0f && local_28.y == 0.0f && local_28.z == 0.0f)
		local_28.x = 1.0f;
	local_28.normalize();
	local_28 *= mMarchSpeed;
	local_28 += mPosition;
	if (!gpMap->isTouchedOneWallAndMoveXZ(
	        &local_28.x, local_28.y + getHeadHeight(), &local_28.z,
	        getBodyRadius() * 2.0f)) {
		switchNextGoalPathNode();
	}
}

void TSpineEnemy::searchNearestBrother() const { }

f32 TSpineEnemy::getCurAnmFrameNo(int type) const
{
	TEnemyManager* mgr = (TEnemyManager*)mManager;

	int iVar1             = getMActor()->getCurAnmIdx(type);
	TSharedMActorSet* set = mgr->getSharedMActorSet(iVar1);
	if (set == nullptr) {
		return getMActor()->getFrameCtrl(type)->getFrame();
	} else {
		return set->getMActor(getInstanceIndex())
		    ->getFrameCtrl(type)
		    ->getFrame();
	}
}

BOOL TSpineEnemy::checkCurAnmEnd(int type) const
{
	TEnemyManager* mgr = (TEnemyManager*)mManager;

	int iVar1             = getMActor()->getCurAnmIdx(type);
	TSharedMActorSet* set = mgr->getSharedMActorSet(iVar1);
	if (set == nullptr) {
		return getMActor()->curAnmEndsNext(type, nullptr);
	} else {
		return set->getMActor(getInstanceIndex())
		    ->curAnmEndsNext(type, nullptr);
	}
}

void TSpineEnemy::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TEnemyManager* mgr = (TEnemyManager*)getManager();

	if (mgr != nullptr) {
		if ((cue & CUE_CALC_ANIM)
		    && !checkLiveFlag(LIVE_FLAG_HIDDEN | LIVE_FLAG_CLIPPED_OUT)
		    && TEnemyManager::mIsCopyAnmMtx && mgr->getUnk4C() >= 0) {
			if (checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_OUT_OF_AREA))
				return;

			if (mgr->copyAnmMtx(this))
				return;
		}
	} else {
		if ((cue & CUE_CALC_ANIM)
		    && !checkLiveFlag(LIVE_FLAG_HIDDEN | LIVE_FLAG_CLIPPED_OUT)
		    && checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_OUT_OF_AREA)) {
			return;
		}
	}

	TLiveActor::perform(cue, graphics);
}
