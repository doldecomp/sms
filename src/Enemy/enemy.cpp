#include <Enemy/Enemy.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Conductor.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Player/MarioAccess.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
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
	unk124->mPrevIdx = -1;
	goToShortestNextGraphNode();
	offLiveFlag(LIVE_FLAG_DEAD);
	offLiveFlag(LIVE_FLAG_UNK8);
	offLiveFlag(LIVE_FLAG_UNK10);
	offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
	onLiveFlag(LIVE_FLAG_UNK800);
	mSpine->reset();
	mSpine->pushDefault();
}

void TSpineEnemy::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);

	char buffer[256];
	stream.readString(buffer, 256);
	TLiveManager* mgr = JDrama::TNameRefGen::search<TLiveManager>(buffer);

	stream.readString(buffer, 256);
	unk124->unk0 = gpConductor->getGraphByName(buffer);

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
			mtx[0][3] = 0.0;
			mtx[1][3] = 0.0;
			mtx[2][3] = 0.0;
		} else {
			if (unk130 >= 1 && !mGroundPlane->checkFlag(0x10)) {
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
				mtx[0][3] = 0.0;
				mtx[1][3] = 0.0;
				mtx[2][3] = 0.0;
			} else {
				MsMtxSetRotRPH(mtx, mRotation.x, mRotation.y, mRotation.z);
			}
		}
	}

	mtx[0][3] = mPosition.x;
	mtx[1][3] = mPosition.y;
	mtx[2][3] = mPosition.z;
}

void TSpineEnemy::calcRootMatrix()
{
	if (mHolder && mHolder->mHeldObject == this) {
		MtxPtr src = getTakingMtx();
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
	mHitPoints = getSaveParam() ? getSaveParam()->mSLHitPointMax.get() : 1;
	offHitFlag(0x1);
	mVelocity = JGeometry::TVec3<f32>(0.0f, 5.0f, 0.0f);
	onLiveFlag(LIVE_FLAG_UNK8000);
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	control();
}

void TSpineEnemy::resetSRTV(const JGeometry::TVec3<f32>& param_1,
                            const JGeometry::TVec3<f32>& param_2,
                            const JGeometry::TVec3<f32>& param_3,
                            const JGeometry::TVec3<f32>& param_4)
{
	mPosition = param_1;
	offLiveFlag(LIVE_FLAG_UNK8);
	offLiveFlag(LIVE_FLAG_DEAD);
	reset();
	mRotation  = param_2;
	mScaling   = param_3;
	mHitPoints = getSaveParam() ? getSaveParam()->mSLHitPointMax.get() : 1;
	offHitFlag(0x1);
	mVelocity = param_4;
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

void TSpineEnemy::calcTurnSpeedToReach(f32 param_1, f32 param_2) const { }

void TSpineEnemy::updateSquareToMario()
{
	// assert?
	(void)gpMarioPos;
	mDistToMarioSquared = PSVECSquareDistance(&mPosition, gpMarioPos);
}

BOOL TSpineEnemy::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == 4 && mHolder == nullptr) {
		mHolder = (TTakeActor*)param_1;
		return true;
	}

	if (param_2 <= 2) {
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
	TPathNode local_3c(local_48);
	unkF4  = local_3c;
	unk104 = local_3c;
	unk114.clear();
}

void TSpineEnemy::goToInitialVisibleNode(f32, f32) { }

void TSpineEnemy::goToInitialGraphNodeCheckY(f32) { }

#pragma dont_inline on
int TSpineEnemy::goToShortestNextGraphNode()
{
	TGraphWeb* web = unk124->unk0;

	if (unk124 == nullptr)
		return -1;

	if (unk124->mCurrIdx < 0)
		unk124->setTo(web->findNearestNodeIndex(mPosition, -1));
	else
		unk124->moveTo(
		    web->getShortestNextIndex(unk124->mCurrIdx, unk124->mPrevIdx, -1));

	if (unk124->mCurrIdx < 0)
		return -1;

	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
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
		unk128 = 0;
		unk12C = 0.0f;
		return idx;
	}

	return -1;
}

void TSpineEnemy::goToRandomNextGraphNode()
{
	if (unk124->mCurrIdx < 0)
		unk124->setTo(unk124->unk0->findNearestNodeIndex(mPosition, -1));
	else
		unk124->moveTo(unk124->unk0->getRandomNextIndex(unk124->mCurrIdx,
		                                                unk124->mPrevIdx, -1));

	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
}

void TSpineEnemy::goToRandomEscapeGraphNode()
{
	if (unk124->mCurrIdx < 0)
		unk124->setTo(unk124->unk0->findNearestNodeIndex(mPosition, -1));
	else
		unk124->moveTo(unk124->unk0->getEscapeFromMarioIndex(
		    unk124->mCurrIdx, -1, mPosition, -1));

	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
}

void TSpineEnemy::goToExclusiveNextGraphNode() { }

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
	unk128 = 0;
	unk12C = 0.0f;
}

void TSpineEnemy::goToDirLimitedNextGraphNode(f32) { }

void TSpineEnemy::updateStayCount(f32) { }

void TSpineEnemy::turnToCurPathNode(f32) { }

void TSpineEnemy::walkToCurPathNode(f32 march_speed, f32 turn_speed, f32) { }

void TSpineEnemy::zigzagToCurPathNode(f32 march_speed, f32 turn_speed,
                                      f32 cycle, f32 angle)
{
}

void TSpineEnemy::doShortCut() { }

void TSpineEnemy::searchNearestBrother() const { }

f32 TSpineEnemy::getCurAnmFrameNo(int param_1) const
{
	TEnemyManager* mgr = (TEnemyManager*)mManager;

	int iVar1             = getMActor()->getCurAnmIdx(param_1);
	TSharedMActorSet* set = mgr->getSharedMActorSet(iVar1);
	if (set == nullptr) {
		return getMActor()->getFrameCtrl(param_1)->getCurrentFrame();
	} else {
		return set->getMActor(getInstanceIndex())
		    ->getFrameCtrl(param_1)
		    ->getCurrentFrame();
	}
}

BOOL TSpineEnemy::checkCurAnmEnd(int param_1) const
{
	TEnemyManager* mgr = (TEnemyManager*)mManager;

	int iVar1             = getMActor()->getCurAnmIdx(param_1);
	TSharedMActorSet* set = mgr->getSharedMActorSet(iVar1);
	if (set == nullptr) {
		return getMActor()->curAnmEndsNext(param_1, nullptr);
	} else {
		return set->getMActor(getInstanceIndex())
		    ->curAnmEndsNext(param_1, nullptr);
	}
}

void TSpineEnemy::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TEnemyManager* mgr = (TEnemyManager*)getManager();

	if (mgr != nullptr) {
		if ((param_1 & 2)
		    && !checkLiveFlag(LIVE_FLAG_UNK2 | LIVE_FLAG_CLIPPED_OUT)
		    && TEnemyManager::mIsCopyAnmMtx && mgr->unk4C >= 0) {
			if (checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_UNK200))
				return;

			if (mgr->copyAnmMtx(this))
				return;
		}
	} else {
		if ((param_1 & 2)
		    && !checkLiveFlag(LIVE_FLAG_UNK2 | LIVE_FLAG_CLIPPED_OUT)
		    && checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_UNK200)) {
			return;
		}
	}

	TLiveActor::perform(param_1, param_2);
}
