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
    , unk114(4)
    , unk128(0)
    , unk12C(0.0f)
    , unk130(0)
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
	offLiveFlag(LIVE_FLAG_UNK8);
	offLiveFlag(LIVE_FLAG_UNK10);
	offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
	onLiveFlag(LIVE_FLAG_UNK800);
	mSpine->reset();
	mSpine->pushAfterCurrent(mSpine->getDefault());
}

void TSpineEnemy::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);

	char buffer[256];
	stream.readString(buffer, 256);
	TLiveManager* mgr = JDrama::TNameRefGen::search<TLiveManager>(buffer);

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

	if (unk130 == 0) {
		MsMtxSetRotRPH(mtx, mRotation.x, mRotation.y, mRotation.z);
	} else {
		if (unk130 >= 2 && !isAirborne() && unk138 != nullptr) {
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
			if (unk130 >= 1
			    && !mGroundPlane->checkFlag(BG_CHECK_FLAG_ILLEGAL)) {
				JGeometry::TVec3<f32> v1(JMASin(mRotation.y), 0.0f,
				                         JMACos(mRotation.y));

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

// Binding level worth +8 of low region. Same shape as
// SmallEnemyGetHeldObject, which lands TSmallEnemy::isEaten at 0x30.
static inline TTakeActor* EnemyGetHeldObject(TTakeActor* p)
{
	TTakeActor* heldObject = p->getHeldObject();
	return heldObject;
}

void TSpineEnemy::calcRootMatrix()
{
	if (mHolder && EnemyGetHeldObject(mHolder) == this) {
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
	return ((TEnemyManager*)mManager)->unk38;
}

void TSpineEnemy::resetToPosition(const JGeometry::TVec3<f32>& position)
{
	mPosition = position;
	offLiveFlag(LIVE_FLAG_UNK8);
	offLiveFlag(LIVE_FLAG_DEAD);
	reset();
	mHitPoints = getMaxHitPoints();
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mVelocity = JGeometry::TVec3<f32>(0.0f, 5.0f, 0.0f);
	onLiveFlag(LIVE_FLAG_UNK8000);
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	control();
}

void TSpineEnemy::resetSRTV(const JGeometry::TVec3<f32>& position,
                            const JGeometry::TVec3<f32>& rotation,
                            const JGeometry::TVec3<f32>& scaling,
                            const JGeometry::TVec3<f32>& velocity)
{
	mPosition = position;
	offLiveFlag(LIVE_FLAG_UNK8);
	offLiveFlag(LIVE_FLAG_DEAD);
	reset();
	mRotation  = rotation;
	mScaling   = scaling;
	mHitPoints = getSaveParam() ? getSaveParam()->mSLHitPointMax.get() : 1;
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mVelocity = velocity;
	onLiveFlag(LIVE_FLAG_UNK8000);
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	control();
}

f32 TSpineEnemy::calcMinimumTurnRadius(f32 param_1, f32 param_2) const
{
	f32 result;
	if (param_2 >= 90.0f) {
		result = 0.0f;
	} else {
		f32 thing = JMASin(param_2);
		if (thing == 0.0f)
			result = 100000.0f;
		else
			result = param_1 * JMASin(-(param_2 * 0.5f - 90.0f)) / thing;
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

	// TODO: THitActor::calcEntryRadius has same problem. Frame 8 short and the
	// fnmsubs/frsqrte pair swaps f0/f1; a HitActor-style TU-local sqrt inline
	// (volatile-first, double guess, inline argument) is inert.
	volatile f32 f = fVar32 * __frsqrte(fVar32);
	f32 tmp        = matan(f, dVar11) * (360.0f / 65536.0f);
	return 90.0f - tmp;
}

void TSpineEnemy::updateSquareToMario()
{
	// assert?
	(void)gpMarioPos;
	mDistToMarioSquared = PSVECSquareDistance(&mPosition, gpMarioPos);
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

// A forwarder around TGraphNode::getPoint(Vec*): the extra inline level is
// frame-only pool that the ROM's expansions of this body carry.
static inline void getNodePoint(const TGraphNode& node,
                                JGeometry::TVec3<f32>* out)
{
	node.getPoint(out);
}

void TSpineEnemy::setGoalPathFromGraph()
{
	JGeometry::TVec3<f32> local_48;
	getNodePoint(getTracer()->getGraph()->getGraphNode(
	                 getTracer()->getCurGraphIndex()),
	             &local_48);
	setGoalPath(TPathNode(local_48));
}

void TSpineEnemy::goToInitialVisibleNode(f32, f32) { }

void TSpineEnemy::goToInitialGraphNodeCheckY(f32 param_1) { }

// Pragma residue (enemy ladder 339, re-measured in sweep 360): protects
// TSpineEnemy::reset (100 -> 0, the body pasted into it). Our spelling is
// about 12 statements against the depth-1 budget of 14.
// TODO (cc38): fillers before `return 0;` put the gap at four statements;
// naming the graph in both arms is +2, does not tip it, and costs bytes.
#pragma dont_inline on
int TSpineEnemy::goToShortestNextGraphNode()
{
	if (getTracer()->getGraph() == nullptr)
		return -1;

	if (getTracer()->getCurGraphIndex() < 0)
		unk124->setTo(
		    getTracer()->getGraph()->findNearestNodeIndex(mPosition, -1));
	else
		unk124->moveTo(getTracer()->getGraph()->getShortestNextIndex(
		    getTracer()->getCurGraphIndex(), getTracer()->getPrevIndex(), -1));

	if (getTracer()->getCurGraphIndex() < 0)
		return -1;

	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
	return 0;
}
#pragma dont_inline off

// TODO: instruction-exact and the frame agrees; setGoalPathFromGraph's
// block still sits 0xc low (0x88 vs 0x94), the family's shared residue.
int TSpineEnemy::jumpToNextGraphNode()
{
	if (getTracer()->getCurGraphIndex() < 0
	    || !getTracer()
	            ->getGraph()
	            ->getGraphNode(getTracer()->getCurGraphIndex())
	            .checkFlag(0x1))
		return -1;

	int idx = getTracer()->getGraph()->getNeighborNodeIndexByFlag(
	    getTracer()->getCurGraphIndex(), getTracer()->getPrevIndex(), 2);
	if (idx >= 0) {
		unk124->moveTo(idx);
		setGoalPathFromGraph();
		unk128 = 0;
		unk12C = 0.0f;
		return idx;
	}

	return -1;
}

// TODO: setGoalPathFromGraph's block sits 0x14 low at an exact frame. All
// 243 raw/tracer/getTracer() spellings of the five index/graph reads leave
// it at 0x60 or lower: each inline call taking the tracer as an argument
// (member accessor or free function alike) puts about 4 bytes above the
// block, where retail has them below. goToExclusiveNextGraphNode shows the
// same 0x14 between its two pasted blocks.
void TSpineEnemy::goToRandomNextGraphNode()
{
	if (getTracer()->getCurGraphIndex() < 0)
		unk124->setTo(
		    getTracer()->getGraph()->findNearestNodeIndex(mPosition, -1));
	else
		unk124->moveTo(getTracer()->getGraph()->getRandomNextIndex(
		    getTracer()->getCurGraphIndex(), getTracer()->getPrevIndex(), -1));

	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
}

void TSpineEnemy::goToRandomEscapeGraphNode()
{
	if (getTracer()->getCurGraphIndex() < 0)
		unk124->setTo(
		    getTracer()->getGraph()->findNearestNodeIndex(mPosition, -1));
	else
		unk124->moveTo(getTracer()->getGraph()->getEscapeFromMarioIndex(
		    getTracer()->getCurGraphIndex(), -1, mPosition, -1));

	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
}

void TSpineEnemy::goToExclusiveNextGraphNode()
{
	if (mManager == nullptr) {
		goToRandomNextGraphNode();
	} else {
		if (getTracer()->getCurGraphIndex() < 0) {
			unk124->setTo(
			    getTracer()->getGraph()->findNearestNodeIndex(mPosition, -1));
		} else {
			int idx = getTracer()->getGraph()->getRandomNextIndex(
			    getTracer()->getCurGraphIndex(), getTracer()->getPrevIndex(),
			    -1);
			for (int i = 0; i < mManager->getObjNum(); ++i) {
				TSpineEnemy* enemy = (TSpineEnemy*)mManager->getObj(i);
				if (this != enemy
				    && idx == enemy->getTracer()->getCurGraphIndex())
					idx = getTracer()->getGraph()->getRandomNextIndex(
					    getTracer()->getCurGraphIndex(), idx, -1);
			}
			unk124->moveTo(idx);
		}

		setGoalPathFromGraph();
		unk128 = 0;
		unk12C = 0.0f;
	}
}

void TSpineEnemy::goToDirectedNextGraphNode(
    const JGeometry::TVec3<f32>& param_1)
{
	int currIdx = getTracer()->getCurGraphIndex();
	int prevIdx = getTracer()->getPrevIndex();
	if (currIdx < 0) {
		unk124->setTo(
		    getTracer()->getGraph()->findNearestNodeIndex(mPosition, -1));
	} else {
		int idx = getTracer()->getGraph()->getAimToDirNextIndex(
		    currIdx, prevIdx, param_1, mPosition, -1);
		unk124->moveTo(idx);
	}

	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
}

// TODO: fake
static inline JGeometry::TVec3<f32> polarXZ(f32 theta, f32 radius)
{
	f32 c = radius * JMACos(theta);
	f32 s = radius * JMASin(theta);
	return JGeometry::TVec3<f32>(s, 0.0f, c);
}

// TODO: only setGoalPathFromGraph's inline temporaries remain, 0x10 low; the
// same frame gap as the other goTo*GraphNode siblings.
void TSpineEnemy::goToDirLimitedNextGraphNode(f32 param_1)
{
	int prevIdx = getTracer()->getPrevIndex();
	int currIdx = getTracer()->getCurGraphIndex();
	if (currIdx < 0) {
		unk124->setTo(
		    getTracer()->getGraph()->findNearestNodeIndex(mPosition, -1));
	} else {
		TGraphWeb* web                 = getTracer()->getGraph();
		f32 rotY                       = mRotation.y;
		JGeometry::TVec3<f32> local_3c = polarXZ(rotY, 1.0f);

		int idx = web->getRandomButDirLimited(currIdx, prevIdx, local_3c,
		                                      mPosition, param_1, -1);
		unk124->moveTo(idx);
	}

	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
}

void TSpineEnemy::updateStayCount(f32) { }

BOOL TSpineEnemy::turnToCurPathNode(f32 param_1)
{
	const TPathNode& node     = getUnkF4();
	JGeometry::TVec3<f32> tmp = node.getPoint();
	tmp -= mPosition;

	f32 rot = MsAngleDiff(MsGetRotFromZaxisY(tmp), mRotation.y);

	BOOL uVar2 = FALSE;
	if (rot > 0.0f) {
		if (rot < param_1) {
			uVar2 = TRUE;
		} else {
			rot = param_1;
		}
	} else {
		if (rot > -param_1) {
			uVar2 = TRUE;
		} else {
			rot = -param_1;
		}
	}

	mRotation.y = MsWrap(mRotation.y + rot, 0.0f, 360.0f);
	return uVar2;
}

void TSpineEnemy::walkToCurPathNode(f32 march_speed, f32 turn_speed,
                                    f32 param_3)
{
	JGeometry::TVec3<f32> tmp = getUnkF4().getPoint();
	tmp -= mPosition;

	f32 fVar7 = tmp.length();
	f32 angle = MsWrap(param_3 + MsGetRotFromZaxisY(tmp), 0.0f, 360.0f);
	f32 fVar2 = MsAngleDiff(angle, mRotation.y);

	// TODO: identical to a piece of code below, what is this?
	f32 fVar3;
	if (turn_speed >= 90.0f) {
		fVar3 = 0.0f;
	} else {
		f32 s = JMASin(turn_speed);
		if (fVar3 == 0.0f) {
			fVar3 = 100000.0f;
		} else {
			fVar3 = march_speed * JMASin(90.0f - turn_speed * 0.5f) / s;
		}
	}

	// TODO: tons of thi stuff should actually be inlines
	f32 fVar5 = fVar2;
	if (fVar7 > fVar3 * 2.0f) {
		if (fVar2 > 0.0f) {
			fVar5 = fVar2 > turn_speed ? fVar2 : turn_speed;
		} else {
			fVar5 = fVar2 > -turn_speed ? fVar2 : -turn_speed;
		}
	} else {
		f32 fVar3 = calcTurnSpeedToReach(march_speed, fVar7 * 0.5f);

		if (fVar2 > 0.0f) {
			fVar5 = fVar2 > fVar3 ? fVar2 : fVar3;
		} else {
			fVar5 = fVar2 > -fVar3 ? fVar2 : -fVar3;
		}
	}

	mRotation.y = MsWrap(mRotation.y + fVar5, 0.0f, 360.0f);

	JGeometry::TVec3<f32> vel = mLinearVelocity;
	vel += polarXZ(mRotation.y, march_speed);
	mLinearVelocity = vel;

	if (abs(fVar7 - unk12C) < 100.0f) {
		unk128 += 1;
	} else {
		unk128 = 0;
		unk12C = fVar7;
	}
}

void TSpineEnemy::zigzagToCurPathNode(f32 march_speed, f32 turn_speed,
                                      f32 cycle, f32 angle)
{
	if (unk124->unk10 == 0.0f) {
		walkToCurPathNode(march_speed, turn_speed, 0.0f);
		return;
	}

	f32 f29 = angle * unk124->unk10;

	f32 dVar9 = (unkF4.getPoint() - mPosition).length();

	f32 dVar13 = MsWrap(dVar9, 0.0f, cycle);

	dVar13 *= 360.0f * (1.0f / cycle);
	dVar13 += getPhaseShift();

	f29 *= JMASin(dVar13);

	JGeometry::TVec3<f32> local_58 = unkF4.getPoint();
	local_58 -= mPosition;
	f32 dVar12 = f29 + MsGetRotFromZaxisY(local_58);

	f32 fVar1 = MsAngleDiff(dVar12, mRotation.y);

	f32 fVar3;
	if (turn_speed >= 90.0f) {
		fVar3 = 0.0f;
	} else {
		f32 s = JMASin(turn_speed);
		if (fVar3 == 0.0f) {
			fVar3 = 100000.0f;
		} else {
			fVar3 = march_speed * JMASin(90.0f - turn_speed * 0.5f) / s;
		}
	}

	f32 fVar2;
	if (dVar9 > fVar3 * 2.0f) {
		if (fVar1 > 0.0f) {
			fVar2 = fVar1 > turn_speed ? fVar1 : turn_speed;
		} else {
			fVar2 = fVar1 > -turn_speed ? fVar1 : -turn_speed;
		}
	}

	mRotation.y = MsWrap(mRotation.y + fVar2, 0.0f, 360.0f);

	JGeometry::TVec3<f32> vel = mLinearVelocity;
	vel += polarXZ(mRotation.y, march_speed);
	mLinearVelocity = vel;

	if (abs(dVar9 - unk12C) < 100.0f) {
		unk128 += 1;
	} else {
		unk128 = 0;
		unk12C = dVar9;
	}
}

// PathNode.hpp's getPoint() reaches the node's actor through getPosition();
// reading mPosition raw instead is low region here. Header round 27 measured
// the same change made in the header as a tree-wide wash, so it stays parked
// TU-locally.
static inline const JGeometry::TVec3<f32>& EnemyGetPoint(const TPathNode& node)
{
	if (node.unk0 != 0)
		return node.unk0->mPosition;

	return node.unk4;
}

void TSpineEnemy::doShortCut()
{
	if (unk114.size() <= 0)
		return;

	if ((unkF4.getPoint() - mPosition).length() < getBodyRadius()) {
		if (!unk114.empty())
			unkF4 = unk114.pop();
		return;
	}

	// A peek, not a pop: the node is only consumed below, once the wall probe
	// says the short cut is clear. Retail inlines `mData[mSize - 1]` here with
	// no decrement and no empty guard, which is exactly TSolidStack::top().
	// Declared then assigned, not copy-initialised: retail default-constructs
	// the node (a null owner and a zero point) before the four-word copy.
	//
	// TODO: the frame is exact now (the TU-local raw getPoint below is -0x10),
	// but every temporary sits 0x10 above retail's slot, so retail reserves
	// 0x10 more below them than we do.
	TPathNode node;
	node = unk114.top();

	JGeometry::TVec3<f32> local_28 = EnemyGetPoint(node) - mPosition;
	if (local_28.x == 0.0f && local_28.y == 0.0f && local_28.z == 0.0f)
		local_28.x = 1.0f;
	local_28.normalize();
	local_28 *= mMarchSpeed;
	local_28 += mPosition;
	if (!gpMap->isTouchedOneWallAndMoveXZ(
	        &local_28.x, local_28.y + getHeadHeight(), &local_28.z,
	        getBodyRadius() * 2.0f)) {
		if (!unk114.empty())
			unkF4 = unk114.pop();
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

// Binding level worth +8 of low region, landing TSpineEnemy::perform's frame
// at 0x30 (batch 124).
static inline TLiveManager* EnemyGetManager(TSpineEnemy* p)
{
	TLiveManager* manager = p->getManager();
	return manager;
}

void TSpineEnemy::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TEnemyManager* mgr = (TEnemyManager*)EnemyGetManager(this);

	if (mgr != nullptr) {
		if ((cue & CUE_CALC_ANIM)
		    && !checkLiveFlag(LIVE_FLAG_HIDDEN | LIVE_FLAG_CLIPPED_OUT)
		    && TEnemyManager::mIsCopyAnmMtx && mgr->unk4C >= 0) {
			if (checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_UNK200))
				return;

			if (mgr->copyAnmMtx(this))
				return;
		}
	} else {
		if ((cue & CUE_CALC_ANIM)
		    && !checkLiveFlag(LIVE_FLAG_HIDDEN | LIVE_FLAG_CLIPPED_OUT)
		    && checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_UNK200)) {
			return;
		}
	}

	TLiveActor::perform(cue, graphics);
}
