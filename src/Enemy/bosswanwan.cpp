#include <Enemy/BossWanwan.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Enemy/EffectObj.hpp>
#include <Camera/CameraShake.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Mario.hpp>
#include <Enemy/Conductor.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <GC2D/GCConsole2.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Enemy/Graph.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MoveBG/ItemManager.hpp>

// to match __sinit__
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static const char* bwanwan_bastable[] = {
	"/scene/bwanwan/bas/bwanwan_bark.bas",
	nullptr,
	"/scene/bwanwan/bas/bwanwan_shake.bas",
	nullptr,
	"/scene/bwanwan/bas/bwanwan_wait.bas",
	"/scene/bwanwan/bas/bwanwan_wait2.bas",
	nullptr,
};

static JGeometry::TVec3<f32> BW_BATH_POS(-1000.0f, 4.5f, -6217.2f);
static JGeometry::TVec3<f32> BW_PICKET_START(6012.84f, 0.0f, 7323.15f);
static JGeometry::TVec3<f32> BW_HEAD_START(5741.72f, -100.0f, 6311.62f);

TBWParams::TBWParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mSLMarchSpeed, 6.0f)
    , PARAM_INIT(mSLShakeLengthMax, 3000.0f)
    , PARAM_INIT(mSLShakeLengthMaxHP0, 2000.0f)
    , PARAM_INIT(mSLTurnSpeed, 1.0f)
    , PARAM_INIT(mSLLeashNodeLen, 120.0f)
    , PARAM_INIT(mSLPicketHeight, 100.0f)
    , PARAM_INIT(mSLPicketRadius, 100.0f)
    , PARAM_INIT(mSLChainHitHeight, 100.0f)
    , PARAM_INIT(mSLChainHitRadius, 100.0f)
    , PARAM_INIT(mSLChainGroundRadius, 60.0f)
    , PARAM_INIT(mSLPullLimit, 1.0f)
    , PARAM_INIT(mSLAttackSpeed, 10.0f)
    , PARAM_INIT(mSLStunTimer, 4000)
    , PARAM_INIT(mSLSearchLength, 10000.0f)
    , PARAM_INIT(mSLSearchAngle, 60.0f)
    , PARAM_INIT(mSLBWHitPointMax, 255)
    , PARAM_INIT(mSLHeadGap, 150.0f)
{
	TParams::load(mPrmPath);
}

TBWLeashNode::TBWLeashNode(TBWLeash* leash, int index, const char* name)
    : THitActor(name)
    , mLeash(leash)
    , mMActor(nullptr)
    , mTemperature(0.0f)
    , mIndex(index)
{
	TBossWanwan* owner = mLeash->mOwner;
	mMActor = owner->getActorKeeper()->createMActor("bwanwan_chain.bmd", 0);
	mMActor->setBrkFromIndex(2);

	f32 hitRadius
	    = ((TBWParams*)owner->getSaveParam())->mSLChainHitRadius.value;
	f32 hitHeight
	    = ((TBWParams*)owner->getSaveParam())->mSLChainHitHeight.value;
	initHitActor(ACTOR_TYPE_BOSS_UNKC, 1, HIT_CATEGORY_PLAYER, hitRadius * 2.0f,
	             hitHeight * 2.0f, hitRadius, hitHeight);
}

void TBWLeashNode::calcTemperature()
{
	if (mIndex == 0) {
		return;
	}

	f32 delta = mLeash->getNode(mIndex - 1)->mTemperature - mTemperature;
	f32 step;

	if (delta < 0.0f) {
		if (delta < 0.1f) {
			step = 0.02f;
		} else {
			step = 0.005f;
		}
	} else {
		if (delta > -0.1f) {
			step = -0.02f;
		} else {
			step = -0.005f;
		}
	}

	mTemperature += step;

	if (mTemperature < 0.0f) {
		mTemperature = 0.0f;
	}

	if (mTemperature > 1.0f) {
		mTemperature = 1.0f;
	}
}

void TBWLeashNode::calcMatrix()
{
	TRope* rope        = mLeash->mRope;
	TRopePoint* points = rope->mPoints;
	MtxPtr mtx         = mMActor->getModel()->getBaseTRMtx();

	JGeometry::TVec3<f32> pos = points[mIndex].unkC;
	JGeometry::TVec3<f32> dir;

	if (mIndex < rope->mNumPoints - 1) {
		dir = points[mIndex + 1].unkC;
		dir -= pos;

	} else {
		dir = points[mIndex - 1].unkC;
		dir -= pos;
		dir.negate();
	}

	PSVECNormalize(&dir, &dir);

	JGeometry::TVec3<f32> n(0.0f, 1.0f, 0.0f);
	JGeometry::TVec3<f32> side;

	side.cross2(n, dir);
	PSVECNormalize(&side, &side);

	n.cross2(dir, side);
	PSVECNormalize(&n, &n);

	mtx[0][2] = dir.x;
	mtx[1][2] = dir.y;
	mtx[2][2] = dir.z;

	if (mIndex & 1) {
		mtx[0][0] = side.x;
		mtx[1][0] = side.y;
		mtx[2][0] = side.z;
		mtx[0][1] = n.x;
		mtx[1][1] = n.y;
		mtx[2][1] = n.z;
	} else {
		mtx[0][0] = n.x;
		mtx[1][0] = n.y;
		mtx[2][0] = n.z;
		mtx[0][1] = side.x;
		mtx[1][1] = side.y;
		mtx[2][1] = side.z;
	}

	mtx[0][3] = pos.x;
	mtx[1][3] = pos.y + 30.0f;
	mtx[2][3] = pos.z;

	mPosition = pos;
}

void TBWLeashNode::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		calcTemperature();
		calcMatrix();
		if (mLeash->mOwner->mHitPoints != 0 && mIndex < 8) {
			for (int i = 0; i < mColCount; i++) {
				THitActor* elem = mCollisions[i];
				if (elem->mActorType == ACTOR_TYPE_MARIO) {
					elem->receiveMessage(this, HIT_MESSAGE_BURN);
				}
			}
		}
	}

	if ((cue & CUE_CALC_ANIM)) {
		J3DFrameCtrl* frameCtrl = mMActor->getFrameCtrl(5);
		if (frameCtrl) {
			f32 frame = (frameCtrl->getEnd() - 1) * mTemperature;

			f32 progress;
			if (mIndex < 5) {
				progress
				    = ((f32)mIndex * 0.25f)
				      + ((f32)mLeash->mOwner->mHitPoints
				         / (f32)((TBWParams*)mLeash->mOwner->getSaveParam())
				               ->mSLBWHitPointMax.get());
			} else if (mIndex >= (mLeash->mRope->mNumPoints - 10)) {
				progress = (f32)(mLeash->mRope->mNumPoints - mIndex) / 10.0f;
			} else {
				progress = 1.0f;
			}

			progress = MsClamp(progress, 0.0f, 1.0f);
			frame *= progress;
			frameCtrl->setFrame(frame);
			frameCtrl->setRate(0.0f);
		}
	}

	if (mIndex < mLeash->mRope->mNumPoints - 1) {
		mMActor->perform(cue, graphics);
	}
}

TBWLeash::TBWLeash(TBossWanwan* owner, int nodeCount, const char* name)
    : JDrama::TViewObj(name)
    , mOwner(owner)
    , mRope(nullptr)
    , mNodes(nullptr)
{
	f32 nodeLen = ((TBWParams*)mOwner->getSaveParam())->mSLLeashNodeLen.get();
	f32 groundRadius
	    = ((TBWParams*)mOwner->getSaveParam())->mSLChainGroundRadius.get();

	mRope = new TRope(nodeCount, mOwner->mPosition, nodeLen, groundRadius, 0.7,
	                  -2.0f);

	mNodes = new TBWLeashNode*[nodeCount];
	for (int i = 0; i < nodeCount; i++) {
		mNodes[i] = new TBWLeashNode(this, i, "鎖部品");
	}

	TIdxGroupObj* group
	    = static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"));

	for (int i = 0; i < nodeCount; i++) {
		group->getChildren().push_back(mNodes[i]);

		if (i < (nodeCount - 2)) {
			mNodes[i]->offHitFilter(HIT_FILTER_NO_COLLISION);

		} else {
			mNodes[i]->onHitFilter(HIT_FILTER_NO_COLLISION);
		}

		if (i < 2) {
			mRope->mPoints[i].unk28 |= 0x1;
		} else {
			mRope->mPoints[i].unk28 &= ~0x1;
		}
	}
}

void TBWLeash::invalidateAllCollision()
{
	for (int i = 0; i < mRope->mNumPoints; i++) {
		mNodes[i]->onHitFilter(1);
	}
}

void TBWLeash::pullTail(const JGeometry::TVec3<f32>& pos)
{
	JGeometry::TVec3<f32> delta = mRope->mPoints->unkC;
	mRope->constraintTail(pos);

	delta.sub(mRope->mPoints->unkC);
	delta.negate();

	mOwner->mPicketPullDelta = delta;
}

void TBWLeash::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		JGeometry::TVec3<f32> headPos;
		mOwner->getJointTransByIndex(5, &headPos);
		mRope->moveHead(headPos);
		mOwner->mIsBurnt = 0;

		if (mOwner->isMarioInSight()) {

			pullTail(mOwner->mPicket->mPosition);

			JGeometry::TVec3<f32> diff      = mRope->mPoints[0].unkC;
			JGeometry::TVec3<f32> centerPos = mOwner->mPosition;
			centerPos.y += 500.0f;
			diff -= centerPos;

			if (PSVECMag(&diff) > 650.0f) {
				PSVECNormalize(&diff, &diff);
				diff.scale(20.0f);
				diff.y = 0.0f;
				mOwner->mPosition.add(diff);
				mOwner->mIsBurnt = 1;
			}

			f32 targetAngle
			    = MsWrap(MsGetRotFromZaxisY(diff) + 180.0f, 0.0f, 360.0f);
			f32 angleDiff = MsAngleDiff(targetAngle, mOwner->mRotation.y);

			f32 turnStep;
			if (angleDiff > 0.0f) {
				f32 maxTurn = 1.5f * mOwner->mTurnSpeed;
				turnStep    = angleDiff > maxTurn ? maxTurn : angleDiff;
			} else {
				f32 minTurn = 1.5f * -mOwner->mTurnSpeed;
				turnStep    = angleDiff <= minTurn ? minTurn : angleDiff;
			}

			mOwner->mRotation.y
			    = MsWrap(mOwner->mRotation.y + turnStep, 0.0f, 360.0f);
		}
	}

	if (cue & CUE_CALC_ANIM) {
		JGeometry::TVec3<f32> bodyPos;
		mOwner->getJointTransByIndex(1, &bodyPos);

		for (int i = 0; i < 15; i++) {
			TRopePoint* point               = &mRope->mPoints[i];
			JGeometry::TVec3<f32> pointDiff = point->unkC;
			pointDiff -= bodyPos;

			f32 dist = PSVECMag(&pointDiff);
			if (dist < 500.0f) {
				pointDiff.scale(500.0f / dist);
				pointDiff += bodyPos;

				point->unk18.zero();
				point->unkC = pointDiff;
				point->unk0 = point->unkC;
			}
		}
	}

	for (int i = 0; i < mRope->mNumPoints; i++) {
		mNodes[i]->testPerform(cue, graphics);
	}
}

TBWPicket::TBWPicket(TBossWanwan* owner, const char* name)
    : TTakeActor(name)
    , mOwner(owner)
    , mActor(nullptr)
{
	PSMTXIdentity(unk74);

	initHitActor(ACTOR_TYPE_BOSS_WANWAN, 1, HIT_CATEGORY_PLAYER,
	             ((TBWParams*)mOwner->getSaveParam())->mSLPicketRadius.get(),
	             ((TBWParams*)mOwner->getSaveParam())->mSLPicketHeight.get(),
	             ((TBWParams*)mOwner->getSaveParam())->mSLPicketRadius.get(),
	             ((TBWParams*)mOwner->getSaveParam())->mSLPicketHeight.get());

	TIdxGroupObj* hitGroup
	    = static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"));
	if (hitGroup != nullptr) {
		hitGroup->getChildren().push_back(this);
	}

	offHitFilter(1);
	mActor = mOwner->mMActorKeeper->createMActor("bwanwan_picket.bmd", 0);
}

BOOL TBWPicket::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->mActorType == ACTOR_TYPE_MARIO) {
		if (message == HIT_MESSAGE_HIP_DROP) {
			TBossWanwan* owner  = mOwner;
			owner->mIsNearMario = 1;
			owner->unk184       = 0;
			SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_LOCK, &mPosition,
			                                0, nullptr, 0, 4);
			return TRUE;
		}
		if (message == HIT_MESSAGE_TAKE) {
			TBossWanwan* owner = mOwner;

			owner->releasePicket();
			owner->unk194       = 0;
			owner->mIsNearMario = 0;
			mHolder             = (TTakeActor*)sender;
			return TRUE;
		}
		if (message == HIT_MESSAGE_THROWN || message == HIT_MESSAGE_DETACH) {
			mHolder = nullptr;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL TBWPicket::moveRequest(const JGeometry::TVec3<float>& pos)
{
	// TODO: getLatestNerve requires 2 levels of inlining to get emitted
	// out-of-line, fabricated the getLatestNerve()
	// function in TBossWanwan to temporarely avoid this issue.
	if (mOwner->getLatestNerve() == &TNerveBWJumpToBath::theNerve()
	    || mOwner->getLatestNerve() == &TNerveBWDie::theNerve()
	    || mOwner->mHitPoints != 0) {
		return FALSE;
	}

	TBWLeash* leash = mOwner->mLeash;
	leash->pullTail(pos);

	return TRUE;
}

MtxPtr TBWPicket::getTakingMtx() { return unk74; }

void TBWPicket::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		mPosition = mOwner->mLeash->mRope
		                ->mPoints[mOwner->mLeash->mRope->mNumPoints - 1]
		                .unkC;

		ensureTakeSituation();
		if (mHolder != nullptr) {
			JGeometry::TVec3<f32> side, up, dir;

			dir.set(mHolder->mPosition);
			dir.sub(mOwner->mLeash->mRope->mPoints[0].unkC);
			up.set(0.0f, 1.0f, 0.0f);

			side.cross2(up, dir);
			PSVECNormalize(&side, &side);
			up.cross2(dir, side);
			PSVECNormalize(&up, &up);
			dir.cross2(side, up);
			PSVECNormalize(&dir, &dir);

			unk74.mMtx[0][0] = side.x;
			unk74.mMtx[1][0] = side.y;
			unk74.mMtx[2][0] = side.z;
			unk74.mMtx[0][1] = up.x;
			unk74.mMtx[1][1] = up.y;
			unk74.mMtx[2][1] = up.z;
			unk74.mMtx[0][2] = dir.x;
			unk74.mMtx[1][2] = dir.y;
			unk74.mMtx[2][2] = dir.z;
			unk74.mMtx[0][3] = mPosition.x;
			unk74.mMtx[1][3] = mPosition.y;
			unk74.mMtx[2][3] = mPosition.z;

			mHolder->moveRequest(mPosition);
		}
	}

	if (cue & CUE_CALC_ANIM) {
		MtxPtr mtx = mActor->getModel()->getBaseTRMtx();
		PSMTXIdentity(mtx);
		JGeometry::TVec3<f32> side, up, dir;

		dir.x = mPosition.x;
		dir.y = mPosition.y;
		dir.z = mPosition.z;
		dir.sub(mOwner->mLeash->mRope
		            ->mPoints[mOwner->mLeash->mRope->mNumPoints - 3]
		            .unkC);

		if (dir.squared() <= JGeometry::TUtil<f32>::epsilon()) {
			dir.set(0.0f, 0.0f, 1.0f);
		}

		up.set(0.0f, 1.0f, 0.0f);
		side.cross2(up, dir);
		PSVECNormalize(&side, &side);
		up.cross2(dir, side);
		PSVECNormalize(&up, &up);
		dir.cross2(side, up);
		PSVECNormalize(&dir, &dir);

		mtx[0][0] = side.x;
		mtx[1][0] = side.y;
		mtx[2][0] = side.z;
		mtx[0][1] = up.x;
		mtx[1][1] = up.y;
		mtx[2][1] = up.z;
		mtx[0][2] = dir.x;
		mtx[1][2] = dir.y;
		mtx[2][2] = dir.z;
		mtx[0][3] = mPosition.x;
		mtx[1][3] = mPosition.y;
		mtx[2][3] = mPosition.z;

		if (!mOwner->isMarioInSight()) {
			mtx[1][3] += 70.0f;
		}

		if (isTaken()) {
			mtx[0][3] -= dir.x * 50.0f;
			mtx[2][3] -= dir.z * 50.0f;
		} else {
			mtx[0][3] -= dir.x * 60.0f;
			mtx[2][3] -= dir.z * 60.0f;
		}
	}

	mActor->perform(cue, graphics);
	THitActor::perform(cue, graphics);
}

TBWHit::TBWHit(TBossWanwan* owner, int jointIndex, const char* name)
    : THitActor(name)
    , mOwner(owner)
    , mJointIndex(jointIndex)
{
}

BOOL TBWHit::receiveMessage(THitActor* sender, u32 message)
{
	return mOwner->receiveMessage(sender, message);
}

void TBWHit::perform(u32 cue, JDrama::TGraphics* graphics)
{
	// stack frame shenanigans
	if (cue & CUE_MOVE) {
		if (mJointIndex >= 0) {
			mOwner->getJointTransByIndex(mJointIndex, &mPosition);
		}
		for (int i = 0; i < mColCount; i++) {
			THitActor* elem = mCollisions[i];
			if (mOwner->mHitPoints != 0
			    && elem->mActorType == ACTOR_TYPE_MARIO) {
				elem->receiveMessage(mOwner, HIT_MESSAGE_BURN);
			}
		}
	}

	THitActor::perform(cue, graphics);
}

TBWBinder::TBWBinder()
    : TBinder()
{
}

void TBWBinder::bind(TLiveActor* actor)
{
	TBossWanwan* boss = (TBossWanwan*)actor;

	JGeometry::TVec3<f32> vel     = boss->mPositionDelta;
	JGeometry::TVec3<f32> nextPos = boss->mPosition;
	nextPos += vel;

	if (boss->isAirborne()) {
		JGeometry::TVec3<f32> v = boss->mVelocity;
		nextPos += v;
		v.y -= boss->getGravityY();
		if (v.y < TLiveActor::mVelocityMinY) {
			v.y = TLiveActor::mVelocityMinY;
		}
		boss->mVelocity = v;
	}

	if (&TNerveBWJumpToBath::theNerve() == boss->mSpine->getLatestNerve()
	    || &TNerveBWDie::theNerve() == boss->mSpine->getLatestNerve()) {

		boss->mPositionDelta = nextPos - boss->mPosition;
		return;
	}

	if (boss->isAirborne()) {
		const TBGCheckData* bgData;
		f32 groundY = gpMap->checkGround(
		    nextPos.x, nextPos.y + boss->getHeadHeight(), nextPos.z, &bgData);

		groundY += 1.0f;

		if (boss->mPosition.y > nextPos.y) {
			if (!bgData->isEnemyThrough()) {
				const TBGCheckData* bgData2;
				f32 groundY2 = gpMap->checkGround(
				    nextPos.x,
				    boss->mPosition.y + boss->mHeadHeight * boss->mBodyScale,
				    nextPos.z, &bgData2);
				if (groundY2 + 1.0f > groundY) {
					groundY = groundY2 + 1.0f;
					bgData  = bgData2;
				}
			}
		}

		if (nextPos.y <= groundY && !bgData->checkFlag(0x10)
		    && !bgData->isEnemyThrough()) {
			nextPos.y       = groundY;
			boss->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
			boss->offLiveFlag(LIVE_FLAG_AIRBORNE);
			boss->offLiveFlag(0x8000);
		} else {
			boss->onLiveFlag(LIVE_FLAG_AIRBORNE);
		}

		boss->mGroundHeight = groundY;
		boss->mGroundPlane  = bgData;
	}

	vel = nextPos - boss->mPosition;

	if (!boss->isAirborne()) {
		TGraphTracer* tracer = boss->unk124;
		if (tracer->unk0 != nullptr) {
			int curIdx  = tracer->mCurrIdx;
			int prevIdx = tracer->mPrevIdx;
			if (curIdx >= 0 && prevIdx >= 0 && curIdx != prevIdx) {
				TGraphNode& curNode  = tracer->unk0->getGraphNode(curIdx);
				TGraphNode& prevNode = tracer->unk0->getGraphNode(prevIdx);

				JGeometry::TVec3<f32> curPoint;
				JGeometry::TVec3<f32> prevPoint;
				curNode.getPoint(&curPoint);
				prevNode.getPoint(&prevPoint);

				curPoint -= prevPoint;
				PSVECNormalize(&curPoint, &curPoint);

				f32 proj  = 0.0f;
				f32 lenSq = curPoint.squared();
				if (lenSq == 0.0f) {
				} else {
					proj = vel.dot(curPoint) / lenSq;
				}

				if (proj < 0.0f) {
					if (proj > -3.0f)
						proj = -3.0f;
				} else if (proj > 0.0f) {
					if (proj < 3.0f)
						proj = 3.0f;
				}

				vel = curPoint;
				vel.scale(proj);
			}
		}
	}

	if (!boss->isAirborne()) {
		JGeometry::TVec3<f32> velCopy = vel;
		f32 speed                     = PSVECMag(&velCopy);
		if (speed != 0.0f) {
			f32 deg = (speed / 3141.5928f) * 360.0f;

			JGeometry::TVec3<f32> dirCopy = boss->getRotDirY(boss->mRotation.y);

			if (dirCopy.dot(vel) < 0.0f) {
				deg = -deg;
			}

			f32 deltaAngle = deg * 2.0f;
			if (boss->unk16C != 0) {
				boss->unk168 = MsAngleWrap(deltaAngle + boss->unk168);
			} else if (boss->unk168 != 0.0f) {
				f32 nextAngle = boss->unk168 + deltaAngle;
				if (nextAngle > 360.0f) {
					nextAngle = 0.0f;
				}
				boss->unk168 = nextAngle;
			}
		}
	}

	if (boss->isMarioInSight()) {
		JGeometry::TVec3<f32> pos  = boss->getPosition();
		JGeometry::TVec3<f32> diff = pos;
		diff += vel;

		JGeometry::TVec3<f32> anchorPos = boss->mLeash->mRope->mPoints[3].unkC;
		diff -= anchorPos;

		if (PSVECMag(&diff) > 860.0f) {
			PSVECNormalize(&diff, &diff);
			diff.scale(860.0f);
			diff += anchorPos;
			diff -= pos;
			vel = diff;
		}
	}

	if (!boss->isAirborne()) {
		TGraphTracer* tracer = boss->unk124;
		int prevIdx          = tracer->mPrevIdx;
		TGraphNode& curNode  = tracer->unk0->getGraphNode(tracer->mCurrIdx);
		TGraphNode& prevNode = tracer->unk0->getGraphNode(prevIdx);

		JGeometry::TVec3<f32> curPoint;
		JGeometry::TVec3<f32> prevPoint;
		curNode.getPoint(&curPoint);
		prevNode.getPoint(&prevPoint);

		JGeometry::TVec3<f32> pull;
		pull = MsPerpendicFootToLineR(prevPoint, curPoint, boss->mPosition);
		curPoint -= prevPoint;
		pull -= boss->mPosition;

		f32 dist = PSVECMag(&pull);
		if (dist > 10.0f) {
			dist = 10.0f;
		}

		if (dist < 0.0001f) {
			pull.zero();
		} else {
			PSVECNormalize(&pull, &pull);
			pull.scale(dist);
		}

		vel += pull;
	}

	boss->mPositionDelta = vel;
}

TBossWanwanMtxCalc::TBossWanwanMtxCalc(TBossWanwan* owner)
    : M3UMtxCalcSIAnmBlendQuat(false)
    , mOwner(owner)
{
	mMotionBlendRatio = 0.0f;
}

void TBossWanwanMtxCalc::joinAnm(int i)
{
	J3DAnmTransformKey* newAnm
	    = mOwner->getMActorAnmData()->mBckAnms->getAnmPtr(i);

	if (mNewAnm != newAnm) {
		mOldAnm           = mNewAnm;
		mNewAnm           = newAnm;
		mMotionBlendRatio = 1.0f;
	}
}

void TBossWanwanMtxCalc::calc(u16 index)
{
	J3DTransformInfo info;
	Mtx rotMtx;
	MtxPtr pRotMtx = rotMtx;

	if (index == 0 && mOwner->isAirborne()) {
		j3dSys.setCurrentMtxCalc(this);

		if (mNewAnm != nullptr) {
			mNewAnm->getTransform(index, &info);
		} else {
			J3DJoint* joint
			    = j3dSys.getModel()->getModelData()->getJointNodePointer(index);
			info = joint->getTransformInfo();
		}

		info.mTranslate.x = 0.0f;
		info.mTranslate.y = 0.0f;
		info.mTranslate.z = 0.0f;

		calcTransform(index, info);
	} else {
		M3UMtxCalcSIAnmBlendQuat::calc(index);

		if (index == 1) {

			rotMtx[2][3] = 0.0f;
			rotMtx[1][3] = 0.0f;
			rotMtx[0][3] = 0.0f;

			rotMtx[2][2] = 0.0f;
			rotMtx[1][2] = 0.0f;
			rotMtx[0][2] = 0.0f;

			rotMtx[2][1] = 0.0f;
			rotMtx[1][1] = 0.0f;
			rotMtx[0][1] = 0.0f;

			rotMtx[2][0] = 0.0f;
			rotMtx[1][0] = 0.0f;
			rotMtx[0][0] = 0.0f;

			MsMtxSetRotX(pRotMtx, mOwner->getUnk168());

			MtxPtr jointMtx = mOwner->getModel()->getAnmMtx(index);
			PSMTXConcat(jointMtx, pRotMtx, jointMtx);
			PSMTXCopy(jointMtx, J3DSys::mCurrentMtx);
		}
	}
}

TBossWanwan::TBossWanwan(const char* name)
    : TSpineEnemy(name)
    , mPicket(nullptr)
    , mLeash(nullptr)
    , unk16C(0)
    , unk168(0.0f)
    , mMtxCalc(nullptr)
    , mIsNearMario(0)
    , unk180(0)
    , unk184(0)
    , mIsBurnt(0)
    , msInvincible(0)
    , unk18D(0)
    , unk190(0)
    , unk194(true)
    , mHasSpawnedWater(0)
    , unk198(0)
    , unk19C(0)
    , unk1A0(0)
    , unk1A4(0.0f)
    , mParams(nullptr)
    , unk1B4(0)
{
	mBinder = new TBWBinder;
}

void TBossWanwan::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);

	mMActorKeeper = new TMActorKeeper(mManager, 0x11);
	mMActor       = mMActorKeeper->createMActor("bwanwan_body.bmd", 0);

	TGraphWeb* graph = gpConductor->getGraphByName("bwanwan");
	graph->initGoalIndex(BW_BATH_POS);
	unk124->unk0 = graph;

	mSpine->initWith(&TNerveBWGraphWander::theNerve());

	mMarchSpeed = ((TBWParams*)getSaveParam())->mSLMarchSpeed.get();
	mTurnSpeed  = ((TBWParams*)getSaveParam())->mSLTurnSpeed.get();

	mPosition = BW_HEAD_START;
	reset();

	mLeash             = new TBWLeash(this, 15);
	mPicket            = new TBWPicket(this);
	mPicket->mPosition = BW_PICKET_START;
	mIsNearMario       = 1;
	unk184             = 0;
	goToRandomNextGraphNode();

	initHitActor(ACTOR_TYPE_BOSS_UNKB, 1, HIT_CATEGORY_PLAYER, 0.0f, 0.0f, 0.0f,
	             0.0f);

	mHits[0] = new TBWHit(this, 3);
	mHits[0]->initHitActor(ACTOR_TYPE_BOSS_UNKB, 3,
	                       HIT_CATEGORY_PLAYER | HIT_CATEGORY_ITEM, 500.0f,
	                       500.0f, 450.0f, 500.0f);

	mHits[1] = new TBWHit(this, -1);
	mHits[1]->initHitActor(ACTOR_TYPE_BOSS_UNKB, 3,
	                       HIT_CATEGORY_PLAYER | HIT_CATEGORY_ITEM, 300.0f,
	                       500.0f, 270.0f, 500.0f);

	TIdxGroupObj* hitGroup
	    = static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"));
	for (int i = 0; i < 2; i++) {
		hitGroup->getChildren().push_back(mHits[i]);
		mHits[i]->offHitFilter(1);
	}

	initAnmSound();
	mScaledBodyRadius = 500.0f;

	mMtxCalc          = new TBossWanwanMtxCalc(this);
	J3DMtxCalc* calc  = mMtxCalc;
	MActorAnmBck* bck = mMActor->getAnmBck();

	if (bck != nullptr) {
		bck->setCalc(mMtxCalc);
		bck->setCalcType(MActorMtxCalcType_User);
	}
	mMActor->calc();
	offLiveFlag(LIVE_FLAG_ENABLE_CLIPPING);
	changeBck(4);

	mMActor->setBrkFromIndex(0);
	mMActor->setLightType(1);

	J3DModel* model = mMActor->getModel();
	if (model->getSkinDeform() == nullptr) {
		J3DSkinDeform* skinDeform = new J3DSkinDeform;
		model->setSkinDeform(skinDeform, J3D_DEFORM_ATTACH_FLAG_UNK_1);
	}

	mHitPoints         = ((TBWParams*)getSaveParam())->mSLBWHitPointMax.get();
	mPicketPullDelta.x = mPicketPullDelta.y = mPicketPullDelta.z = 0.0f;

	mMapCollisionManager = new TMapCollisionManager(1, "/scene/bwanwan", this);
	mMapCollisionManager->init("bwanwan_ofuro_col.col", 2, nullptr);

	TMapCollisionManager* col = mMapCollisionManager;
	Mtx mtx;
	MsMtxSetTRS(mtx, mPosition.x, mPosition.y, mPosition.z, mRotation.x,
	            mRotation.y, mRotation.z, mScaling.x, mScaling.y, mScaling.z);
	TMapCollisionBase* base = col->getActiveCollision();
	base->setUpMtx(mtx);

	if (mMapCollisionManager->getActiveCollision() != nullptr) {
		mMapCollisionManager->getActiveCollision()->remove();
	}
}

void TBossWanwan::shakeCamera(int shakeType)
{
	if (!SMS_IsMarioTouchGround4cm()) {
		return;
	}

	f32 dist = getDistToMarioSquared();

	dist = MsSqrtf(dist);

	TBWParams* params  = (TBWParams*)getSaveParam();
	f32 shakeLengthMax = params->mSLShakeLengthMax.get();

	TBWParams* params2    = (TBWParams*)getSaveParam();
	f32 shakeLengthMaxHP0 = params2->mSLShakeLengthMaxHP0.get();

	f32 ratio;
	if (getMActor()->checkCurBckFromIndex(0)) {
		ratio = 1.0f;
	} else {
		TBWParams* params3 = (TBWParams*)getSaveParam();
		ratio = (f32)mHitPoints / (f32)params3->mSLBWHitPointMax.get();
	}

	f32 effectiveDist
	    = shakeLengthMax * ratio + shakeLengthMaxHP0 * (1.0f - ratio);

	f32 delta = effectiveDist - dist;

	if (delta < 0.0f) {
		return;
	}

	f32 power = delta / effectiveDist;
	if (power > 1.0f) {
		power = 1.0f;
	}

	gpCameraShake->startShake((EnumCamShakeMode)shakeType, power * ratio);
	SMSRumbleMgr->start(8, &mPosition);
}

BOOL TBossWanwan::receiveMessage(THitActor* sender, u32 message)
{
	u32 actorType = sender->getActorType();
	if (actorType == ACTOR_TYPE_MARIO) {
		return false;
	} else if (actorType == ACTOR_TYPE_WATER) {
		if (msInvincible) {
			return TRUE;
		}

		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                             0, 0);
		if (mHitPoints == 0) {
			SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_COOL_MORE,
			                                &mPosition, 0, nullptr, 0, 4);
		} else if (mHitPoints == 1) {
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    BWAN_JPA_MS_DOWNYUGE, getModel()->getAnmMtx(1), 0, 0);
			SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_TO_COOL,
			                                &mPosition, 0, nullptr, 0, 4);
		} else {
			SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_COOL, &mPosition,
			                                0, nullptr, 0, 4);
		}

		decHitPoints();
		unk190++;
		return TRUE;
	}

	if (sender->isActorType(ACTOR_TYPE_WOOD_BARREL)) {
		sender->receiveMessage(this, HIT_MESSAGE_HIP_DROP);
		mHitPoints = 0;
		unk190++;
		if (unk1A0 == 0) {
			unk1A0++;
		}

		gpMarioParticleManager->emitAndBindToMtxPtr(
		    BWAN_JPA_MS_DOWNYUGE, getModel()->getAnmMtx(1), 0, nullptr);

		SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_TO_COOL, &mPosition, 0,
		                                nullptr, 0, 4);
	}

	return TSpineEnemy::receiveMessage(sender, message);
}

// size match
void TBossWanwan::takeBath()
{
	msInvincible = 1;
	mPosition    = BW_BATH_POS;
	JGeometry::TVec3<f32> colScale;
	JGeometry::TVec3<f32> colPos = mPosition;
	colPos.y += 500.0f;
	colScale = mScaling;
	colScale.scale(1.1f);

	TMapCollisionManager* col = mMapCollisionManager;
	Mtx mtx;
	MsMtxSetTRS(mtx, colPos.x, colPos.y, colPos.z, mRotation.x, mRotation.y,
	            mRotation.z, colScale.x, colScale.y, colScale.z);
	TMapCollisionBase* base = col->getActiveCollision();
	base->setMtx(mtx);
	base->setUp();

	mHits[0]->mHitFilter |= 1;
	mHits[1]->mHitFilter |= 1;

	TBWLeash* leash = mLeash;
	leash->invalidateAllCollision();
	mPicket->onHitFilter(1);
}

void TBossWanwan::releasePicket()
{
	if (!isMarioInSight()) {
		JPABaseEmitter* emit = gpMarioParticleManager->emit(
		    BWAN_JPA_JUMP_SMOKE, &mPicket->mPosition, 0, nullptr);
		if (emit != nullptr) {
			JGeometry::TVec3<f32> scale(0.3f, 0.5f, 0.3f);
			emit->setGlobalScale(scale);
		}
	}
}

void TBossWanwan::changeBck(int index)
{
	setBckFromIndex(index);
	J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(0);
	unk178             = 10.0f / ctrl->getEnd();
	const char* path   = bwanwan_bastable[index];
	setAnmSound(path);
}

void TBossWanwan::startGoldBrk()
{
	// Empty body
}

bool TBossWanwan::isBurning() { return mIsBurnt; }

bool TBossWanwan::isTailBurning()
{
	// TODO
	// return (check on mTemperature?)
}

bool TBossWanwan::isMarioInSight() { return mIsNearMario; }

BOOL TBossWanwan::isHeadPulled()
{
	return mPicketPullDelta.squared()
	               < ((TBWParams*)getSaveParam())->mSLPullLimit.get()
	           ? FALSE
	           : TRUE;
}

void TBossWanwan::calcRootMatrix()
{
	getModel()->setBaseScale(mScaling);

	MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x,
	               mPosition.y + 500.0f, mPosition.z, mRotation.x, mRotation.y,
	               mRotation.z);
}

void TBossWanwan::reverseNextGraphNode()
{
	int prev         = unk124->mPrevIdx;
	int curr         = unk124->mCurrIdx;
	TGraphWeb* graph = unk124->unk0;

	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario -= mPosition;

	int next = graph->getAimToDirNextIndex(prev, curr, toMario, mPosition, -1);
	unk124->mPrevIdx = next;
	unk124->mCurrIdx = prev;

	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
}

void TBossWanwan::rollNextGraphNode()
{
	int prev         = unk124->mPrevIdx;
	int curr         = unk124->mCurrIdx;
	TGraphWeb* graph = unk124->unk0;

	if (prev >= 0
	    && graph->getGraphNode(prev).getRailNode()->mConnectionNum >= 2
	    && gpMarDirector->getMoveTickCount() >= 14400) {
		showMessage(0xE001C);
	}

	int next = graph->getEscapeDirLimited(curr, prev, getRotDirY(mRotation.y),
	                                      mPosition, 100.0f, -1);

	unk124->moveTo(next);
	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
}

void TBossWanwan::slideToCurPathNode(float speed, float turnSpeed)
{
	JGeometry::TVec3<f32> pos = unkF4.getPoint();

	pos.sub(mPosition);

	f32 dist        = PSVECMag(&pos);
	f32 targetAngle = MsGetRotFromZaxisY(pos);
	targetAngle     = MsWrap<f32>(targetAngle, 0.0f, 360.0f);

	f32 diff = MsAngleDiff(targetAngle, mRotation.y);

	if (diff > 0.0f) {
		diff = (diff > turnSpeed) ? turnSpeed : diff;
	} else {
		diff = (diff > -turnSpeed) ? diff : -turnSpeed;
	}

	mRotation.y = MsWrap<f32>(mRotation.y + diff, 0.0f, 360.0f);

	JGeometry::TVec3<f32> vel = mPositionDelta;

	if (dist > 0.0f) {
		pos.scale(speed / dist);
	}

	vel.add(pos);
	mPositionDelta = vel;
}

void TBossWanwan::control()
{
	TLiveActor::control();

	if (isMarioInSight() || (mPicket->isTaken() && isHeadPulled())) {
		mPositionDelta += mPicketPullDelta;

		JGeometry::TVec3<f32> pos = mPosition;

		pos -= mLeash->mRope->mPoints[3].unkC;

		f32 targetAngle = MsGetRotFromZaxisY(pos);
		targetAngle     = MsWrap<f32>(targetAngle, 0.0f, 360.0f);

		f32 diff = MsAngleDiff(targetAngle, mRotation.y);

		if (diff > 0.0f) {
			f32 maxStep = mTurnSpeed * 4.0f;
			diff        = (diff > maxStep) ? diff : maxStep;
		} else {
			f32 minStep = -mTurnSpeed * 4.0f;
			diff        = (diff > minStep) ? diff : minStep;
		}

		mRotation.y = MsWrap<f32>(mRotation.y + diff, 0.0f, 360.0f);
	}

	mPicketPullDelta.zero();
	updateSquareToMario();
}

void TBossWanwan::emitEffects()
{
	int emit = false;

	if (mMActor->checkCurBckFromIndex(4) != 0
	    || mMActor->checkCurBckFromIndex(5) != 0) {
		if (mMActor->checkBckPass(8.0f) != 0) {
			emit = true;
		}
	} else {
		if (mMActor->checkCurBckFromIndex(2) != 0
		    && mMActor->checkBckPass(38.0f) != 0) {
			emit = true;
		}
	}

	if (emit) {
		gpMarioParticleManager->emit(BWAN_JPA_JUMP_ROCK, &mPosition, 0,
		                             nullptr);
		gpMarioParticleManager->emit(BWAN_JPA_JUMP_SMOKE, &mPosition, 0,
		                             nullptr);

		if (mHitPoints == 0) {
			SMSGetMSound()->startSoundActor(MSD_SE_M_BOSW_CHAIN_CAL_F,
			                                &mLeash->mRope->mPoints->unkC, 0,
			                                nullptr, 0, 4);
			SMSGetMSound()->startSoundActor(MSD_SE_M_BOSW_CHAIN_CAL_R,
			                                &mPicket->mPosition, 0, nullptr, 0,
			                                4);
		} else {
			SMSGetMSound()->startSoundActor(MSD_SE_M_BOSW_CHAIN_ANG_F,
			                                &mLeash->mRope->mPoints->unkC, 0,
			                                nullptr, 0, 4);
			SMSGetMSound()->startSoundActor(MSD_SE_M_BOSW_CHAIN_ANG_R,
			                                &mPicket->mPosition, 0, nullptr, 0,
			                                4);
		}
	}

	int emit2 = false;

	if (mMActor->checkCurBckFromIndex(0)) {
		if (mMActor->checkBckPass(72.0f)) {
			emit2 = true;
		}
	} else if (mMActor->checkCurBckFromIndex(4)
	           || mMActor->checkCurBckFromIndex(5)) {
		if (mMActor->checkBckPass(6.0f) || mMActor->checkBckPass(12.0f)) {
			emit2 = true;
		}
	} else if (mMActor->checkCurBckFromIndex(2)) {
		if (mMActor->checkBckPass(4.0f)) {
			emit2 = true;
		}
	}

	if (emit2) {
		MtxPtr jointMtx = getModel()->getAnmMtx(1);
		gpMarioParticleManager->emitAndBindToMtxPtr(BWAN_JPA_MS_HIBANA,
		                                            jointMtx, 0, this);
	}

	if ((mMActor->checkCurBckFromIndex(4) || mMActor->checkCurBckFromIndex(5))
	    && mMActor->checkBckPass(10.0f)) {
		shakeCamera(0x16);
	}

	if (mMActor->checkCurBckFromIndex(0)) {
		J3DFrameCtrl* frameCtrl = mMActor->getFrameCtrl(0);

		if (frameCtrl->checkPass(60.0f) || frameCtrl->checkPass(127.0f)) {
			shakeCamera(0x16);
			gpMarioParticleManager->emit(BWAN_JPA_JUMP_ROCK, &mPosition, 0,
			                             nullptr);
			gpMarioParticleManager->emit(BWAN_JPA_JUMP_SMOKE, &mPosition, 0,
			                             nullptr);
		}

		if (frameCtrl->checkPass(202.0f)) {
			shakeCamera(0x17);
			gpMarioParticleManager->emit(BWAN_JPA_JUMP_ROCK, &mPosition, 0,
			                             nullptr);
			gpMarioParticleManager->emit(BWAN_JPA_JUMP_SMOKE, &mPosition, 0,
			                             nullptr);
		}
	}

	if (mMActor->checkCurBckFromIndex(2) && mMActor->checkBckPass(40.0f)) {
		shakeCamera(0x16);
	}

	if (mHitPoints != 0) {
		MtxPtr jointMtx = getModel()->getAnmMtx(1);
		gpMarioParticleManager->emitAndBindToMtxPtr(BWAN_JPA_MS_YUGAMI,
		                                            jointMtx, 3, this);
	}

	if (unk190 != 0 && mHitPoints != 0) {
		MtxPtr jointMtx = getModel()->getAnmMtx(1);
		gpMarioParticleManager->emitAndBindToMtxPtr(BWANWAN_JPA_MS_BWAN_HITYUGE,
		                                            jointMtx, 1, this);
		unk190 = 0;
	}
}

void TBossWanwan::showMessage(u32 msg)
{
	if (!(unk198 & 4)) {
		gpMarDirector->getConsole()->startAppearBalloon(msg, true);
	}
	unk198 |= 4;
}

void TBossWanwan::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (msInvincible != 0) {
		mHits[0]->perform(cue, graphics);
		TSpineEnemy::perform(cue, graphics);

		if (cue & CUE_MOVE) {
			mMtxCalc->advanceMotionBlend(-unk178);
		}

		if (cue & CUE_ENTRY) {
			J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(5);
			if (ctrl != nullptr
			    && ctrl->getFrame() > 0.5f * (f32)ctrl->getEnd()) {
				unk1A4 = mPosition;
				unk1A4.y += 500.0f;
				gpMarioParticleManager->emitAndBindToPosPtr(
				    BWANWAN_JPA_MS_BWAN_KIRA, &unk1A4, 1, this);
			}
		}
		return;
	}

	if (cue & CUE_CALC_ANIM) {
		J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(5);
		TBWParams* params  = (TBWParams*)getSaveParam();
		f32 hpRatio = (f32)mHitPoints / (f32)params->mSLBWHitPointMax.value;

		ctrl->setFrame(hpRatio * (f32)(ctrl->getEnd() - 1));
		ctrl->setRate(0.0f);

		if (mHitPoints == 0
		    || mHitPoints
		           == ((TBWParams*)getSaveParam())->mSLBWHitPointMax.value) {
			mLeash->mNodes[0]->mTemperature = hpRatio;
		}

		emitEffects();
	}

	mHits[0]->perform(cue, graphics);

	if (cue & CUE_MOVE) {
		mHits[1]->mPosition.set(mHits[0]->mPosition.x,
		                        mHits[0]->mPosition.y - 500.0f,
		                        mHits[0]->mPosition.z);
	}

	mHits[1]->perform(cue, graphics);

	if ((cue & CUE_MOVE) && unk194 == 0) {
		unk19C++;

		if (unk19C < 0) {
			unk19C = 1;
		}

		if (unk19C > 600) {
			if (!(unk198 & 1)) {
				gpMarDirector->getConsole()->startAppearBalloon(0xE001A, true);
			}
			unk198 |= 1;
		}
	}

	if ((cue & CUE_MOVE) && unk1A0 == 0
	    && gpMarDirector->getMoveTickCount() >= 28800) {
		if (!(unk198 & 8)) {
			gpMarDirector->getConsole()->startAppearBalloon(0xE001D, true);
		}
		unk198 |= 8;
	}

	if (cue & CUE_MOVE) {
		if (&TNerveBWDie::theNerve() != mSpine->getLatestNerve()
		    && &TNerveBWJumpToBath::theNerve() != mSpine->getLatestNerve()) {
			if (mHitPoints != 0) {
				if (isMarioInSight() && unk194 == 0) {
					unk184++;
					if (unk184 > 600) {
						mSpine->pushNerve(&TNerveBWShake::theNerve());
						unk184 = 0;
					}
				}
			} else {
				unk184 = 0;
			}

			if (mHitPoints == 0
			    && mSpine->getLatestNerve() != &TNerveBWBark::theNerve()) {
				unk180++;
				if (unk180 > 2400) {
					if (mSpine->getLatestNerve() != &TNerveBWBark::theNerve()) {
						mSpine->setNext(&TNerveBWBark::theNerve());
					}
				}
			} else {
				if (gpMarDirector->getMoveTickCount() % 20 == 0) {
					if (mHitPoints < ((TBWParams*)getSaveParam())
					                     ->mSLBWHitPointMax.value) {
						mHitPoints++;
					}
				}
				unk180 = 0;
			}
		}
	}

	if (cue & CUE_MOVE) {
		if (&TNerveBWGraphWander::theNerve() == mSpine->getLatestNerve()
		    && mPicket->isTaken()) {
			f32 pullDist       = MsSqrtf(mPicketPullDelta.squared());
			TRopePoint* points = mLeash->mRope->mPoints;
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_BS_WANWAN_M_DRAG, &points[6].unkC, nullptr, pullDist, 0,
			    0, nullptr, 0, 4);
		}
	}

	TSpineEnemy::perform(cue, graphics);
	mLeash->testPerform(cue, graphics);
	mPicket->testPerform(cue, graphics);

	if (cue & CUE_MOVE) {
		mMtxCalc->advanceMotionBlend(-unk178);
	}
}

TBossWanwanManager::TBossWanwanManager(const char* name)
    : TEnemyManager(name)
{
}

void TBossWanwanManager::initJParticle()
{
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_jump_rock.jpa", 0xad);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_jump_smoke.jpa", 0xae);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_downyuge.jpa", 0xb0);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_hibana.jpa", 0xaf);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_deadyuge.jpa", 0xb1);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_yugami.jpa", 0x1ee);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_hityuge.jpa", 0x167);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_kira.jpa", 0x168);
}

TSpineEnemy* TBossWanwanManager::createEnemyInstance()
{
	return new TBossWanwan;
}

void TBossWanwanManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "bwanwan_body.bmd", 0, 0 },
		{ "bwanwan_chain.bmd", 0, 0 },
		{ "bwanwan_picket.bmd", 0, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);
}

void TBossWanwanManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TBWParams("/enemy/bosswanwan.prm");
	TEnemyManager::load(stream);
	initJParticle();
}

DEFINE_NERVE(TNerveBWGraphWander, TLiveActor)
{
	TBossWanwan* boss = static_cast<TBossWanwan*>(spine->getBody());

	if (spine->getTime() == 0) {
		boss->unk16C = 0;

		if (!boss->getMActor()->checkCurBckFromIndex(4)
		    && !boss->getMActor()->checkCurBckFromIndex(4)) {
			if (boss->mHitPoints == 0) {
				boss->changeBck(5);
			} else {
				boss->changeBck(4);
			}
			J3DFrameCtrl* ctrl = boss->getMActor()->getFrameCtrl(0);
			ctrl->setFrame(0.0f);
			ctrl->setRate(SMSGetAnmFrameRate());
		}

		boss->getMActor()->setBtpFromIndex(0);
		J3DFrameCtrl* btpCtrl = boss->getMActor()->getFrameCtrl(3);
		btpCtrl->setFrame(0.0f);
		btpCtrl->setRate(0.0f);
	}

	if (boss->getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr)
	    && boss->mHitPoints == 0
	    && !boss->getMActor()->checkCurBckFromIndex(5)) {
		boss->changeBck(5);
		J3DFrameCtrl* ctrl = boss->getMActor()->getFrameCtrl(0);
		ctrl->setFrame(0.0f);
		ctrl->setRate(SMSGetAnmFrameRate());
	}

	if (!boss->isMarioInSight() && boss->mPicket->isTaken()
	    && boss->isHeadPulled()) {
		TGraphTracer* tracer = boss->unk124;
		TGraphWeb* graph     = tracer->unk0;
		int prevIdx          = tracer->mPrevIdx;

		JGeometry::TVec3<f32> nodePos;
		graph->getGraphNode(prevIdx).getPoint(&nodePos);
		nodePos -= boss->mPosition;

		if (PSVECMag(&nodePos) < 400.0f && prevIdx == graph->unk10) {
			spine->pushAfterCurrent(&TNerveBWJumpToBath::theNerve());
			return true;
		}

		if (PSVECMag(&nodePos) < 400.0f) {
			boss->reverseNextGraphNode();
		}

		u8 maxHp = ((TBWParams*)boss->getSaveParam())->mSLBWHitPointMax.value;
		boss->slideToCurPathNode(
		    3.0f * (((f32)boss->mHitPoints / (f32)maxHp) * boss->mMarchSpeed),
		    boss->mTurnSpeed);
		return false;
	}

	if (boss->isReachedToGoal()) {
		if (boss->jumpToNextGraphNode() >= 0) {
			spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
			spine->pushAfterCurrent(&TNerveBWJump::theNerve());
		} else {
			spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
			boss->rollNextGraphNode();
		}
		return true;
	}

	boss->unk16C = 0;
	f32 hpRatio
	    = (f32)boss->mHitPoints
	      / (f32)((TBWParams*)boss->getSaveParam())->mSLBWHitPointMax.value;

	if (boss->mPicket->isTaken()) {
		f32 marioYaw = (f32)gpMarioOriginal->mIntendedYaw * (180.0f / 32768.0f);

		JGeometry::TVec3<f32> marioDir = boss->getRotDirY(marioYaw);

		JGeometry::TVec3<f32> diff = boss->mPosition;
		diff -= gpMarioOriginal->mPosition;
		PSVECNormalize(&diff, &diff);

		f32 pullFactor = 1.0f;
		pullFactor -= (f32)((gpMarioOriginal->mIntendedMag / 32.0f) * 0.75)
		              * -diff.dot(marioDir);

		if (pullFactor < 0.0f) {
			pullFactor = 0.0f;
		} else if (1.5f < pullFactor) {
			pullFactor = 1.5f;
		}

		boss->slideToCurPathNode(pullFactor * (hpRatio * boss->mMarchSpeed),
		                         boss->mTurnSpeed);
	} else {
		boss->slideToCurPathNode(hpRatio * boss->mMarchSpeed + 0.2f,
		                         boss->mTurnSpeed);
	}

	return false;
}

DEFINE_NERVE(TNerveBWRoll, TLiveActor)
{
	TBossWanwan* boss = static_cast<TBossWanwan*>(spine->getBody());

	if (spine->getTime() == 0) {
		J3DFrameCtrl* ctrl = boss->mMActor->getFrameCtrl(0);
		ctrl->setFrame(0.0f);
		ctrl->setRate(0.0f);
		boss->unk16C = 1;
	}

	if (boss->isReachedToGoal()) {
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		J3DFrameCtrl* ctrl = boss->mMActor->getFrameCtrl(0);
		ctrl->setRate(SMSGetAnmFrameRate());
		return true;
	} else {
		f32 speed = ((TBWParams*)boss->getSaveParam())->mSLAttackSpeed.get();
		boss->walkToCurPathNode(speed, boss->mTurnSpeed, 0.0f);
		return false;
	}
}

DEFINE_NERVE(TNerveBWBark, TLiveActor)
{
	TBossWanwan* boss = static_cast<TBossWanwan*>(spine->getBody());

	if (spine->getTime() == 0) {

		boss->changeBck(0);
		boss->unk16C = 0;
		boss->unk168 = 0.0f;

		if (!boss->unk194) {
			if (boss->isMarioInSight()) {
				JPABaseEmitter* emit = gpMarioParticleManager->emit(
				    BWAN_JPA_JUMP_SMOKE, &boss->mPicket->mPosition, 0, 0);
				if (emit != nullptr) {
					JGeometry::TVec3<f32> scale(0.3f, 0.5f, 0.3f);
					emit->setGlobalScale(scale);
				}

				boss->unk194       = 0;
				boss->mIsNearMario = 0;
				SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_ESCAPE1,
				                                &boss->mPicket->mPosition, 0,
				                                nullptr, 0, 4);
			}
		}
	}

	if (spine->getTime() == 280) {
		boss->mHitPoints
		    = ((TBWParams*)boss->getSaveParam())->mSLBWHitPointMax.get();
	}

	if (boss->mMActor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());

		if (!(boss->unk198 & 2)) {
			gpMarDirector->getConsole()->startAppearBalloon(0xE001B, true);
		}
		boss->unk198 |= 2;
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBWJump, TLiveActor)
{
	TBossWanwan* boss = static_cast<TBossWanwan*>(spine->getBody());

	if (spine->getTime() == 0) {
		const JGeometry::TVec3<f32>& targetPos
		    = boss->unk104.unk0 != nullptr ? boss->unk104.unk0->mPosition
		                                   : boss->unk104.unk4;

		f32 speed       = boss->unk124->unkC;
		boss->mVelocity = boss->calcVelocityToJumpToY(targetPos, speed,
		                                              boss->getGravityY());

		boss->onLiveFlag(LIVE_FLAG_AIRBORNE);
		boss->unk16C = 0;
	}

	if (boss->isReachedToGoal()) {
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		return true;
	}

	boss->walkToCurPathNode(0.0f, boss->mTurnSpeed, 0.0f);
	return false;
}

DEFINE_NERVE(TNerveBWStun, TLiveActor)
{
	TBossWanwan* boss = static_cast<TBossWanwan*>(spine->getBody());

	if (boss->mPicket->isTaken() && boss->isHeadPulled()) {
		TGraphWeb* graph = boss->unk124->unk0;
		int prevIdx      = boss->unk124->mPrevIdx;

		TGraphNode& prevNode = graph->getGraphNode(prevIdx);
		JGeometry::TVec3<f32> prevPoint;
		prevNode.getPoint(&prevPoint);
		prevPoint -= boss->mPosition;

		if (PSVECMag(&prevPoint) < 100.0f) {
			if (prevIdx == graph->unk10) {
				spine->pushAfterCurrent(&TNerveBWJumpToBath::theNerve());
				return true;
			}
			boss->reverseNextGraphNode();
		}

		return false;
	} else if (spine->getTime()
	           > ((TBWParams*)boss->getSaveParam())->mSLStunTimer.get()) {
		boss->mHitPoints
		    = ((TBWParams*)boss->getSaveParam())->mSLBWHitPointMax.get();
		spine->pushAfterCurrent(&TNerveBWWakeup::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBWWakeup, TLiveActor)
{
	TBossWanwan* boss = static_cast<TBossWanwan*>(spine->getBody());

	if (spine->getTime() == 0) {
		boss->changeBck(6);
		boss->getMActor()->setBtpFromIndex(2);
		boss->unk16C = 0;
		boss->unk168 = 0.0f;
	}

	if (boss->getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBWJumpToBath, TLiveActor)
{
	TBossWanwan* boss = static_cast<TBossWanwan*>(spine->getBody());

	if (spine->getTime() == 0) {
		JGeometry::TVec3<f32> vel = boss->calcVelocityToJumpToY(
		    BW_BATH_POS, 10.0f, boss->getGravityY());

		TPathNode goal(BW_BATH_POS);
		boss->unkF4  = goal;
		boss->unk104 = goal;
		boss->unk114.clear();

		boss->mVelocity = vel;
		boss->mLiveFlag |= 0x80;
		boss->unk16C = 0;
	}

	if (spine->getTime() > 120 && !boss->mHasSpawnedWater
	    && boss->mPosition.y < (BW_BATH_POS.y + 10.0f)) {

		TEffectColumWater* effectBase
		    = (TEffectColumWater*)gpConductor->makeOneEnemyAppear(
		        boss->mPosition, "エフェクト水柱マネージャー", 1);

		if (effectBase != nullptr) {
			JGeometry::TVec3<f32> scale(5.0f, 5.0f, 5.0f);
			JGeometry::TVec3<f32> effectPos;
			effectPos.x = boss->mPosition.x;
			effectPos.y = (boss->mPosition.y + 500.0f) - 30.0f;
			effectPos.z = boss->mPosition.z;
			effectBase->generate(effectPos, scale);
			boss->mHasSpawnedWater = true;
		}

		SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_DIVE, &boss->mPosition,
		                                0, nullptr, 0, 4);
	}

	JGeometry::TVec3<f32> targetPos = BW_BATH_POS;
	JGeometry::TVec3<f32> diff      = targetPos;
	diff.sub(boss->mPosition);

	if (diff.squared() < 10000.0f && boss->mPosition.y <= BW_BATH_POS.y) {
		boss->mPosition = BW_BATH_POS;
		spine->pushAfterCurrent(&TNerveBWDie::theNerve());
		return true;
	}

	boss->walkToCurPathNode(0.0f, boss->mTurnSpeed, 0.0f);
	return false;
}

DEFINE_NERVE(TNerveBWDie, TLiveActor)
{
	TBossWanwan* boss = static_cast<TBossWanwan*>(spine->getBody());
	MActor* actor     = boss->getMActor();

	if (spine->getTime() == 0) {
		JGeometry::TVec3<f32> zero(0.0f, 0.0f, 0.0f);
		boss->mVelocity      = zero;
		boss->mPositionDelta = zero;
		boss->onLiveFlag(LIVE_FLAG_UNK10);
		boss->offLiveFlag(LIVE_FLAG_AIRBORNE);
		gpMarioParticleManager->emit(0xB1, &boss->mPosition, 0, nullptr);
	}

	if (boss->mHitPoints != 0) {
		boss->decHitPoints();
		J3DFrameCtrl* ctrl = actor->getFrameCtrl(0);
		ctrl->setFrame(0.0f);
		ctrl->setRate(0.0f);
		spine->pushAfterCurrent(&TNerveBWDie::theNerve());
		return true;
	}

	if (spine->getTime() == 0) {
		gpMarDirector->fireStartDemoCamera("bwanwan_down_camera", nullptr, -1,
		                                   0.0f, true, nullptr, 0, nullptr,
		                                   JDrama::TFlagT<u16>(0));

		boss->unk16C = 0;
		boss->unk168 = 0.0f;

		// boss->takeBath(); from here*
		boss->msInvincible = 1;
		boss->mPosition    = BW_BATH_POS;
		JGeometry::TVec3<f32> colScale;
		JGeometry::TVec3<f32> colPos = boss->mPosition;
		colPos.y += 500.0f;
		colScale = boss->mScaling;
		colScale.scale(1.1f);

		TMapCollisionManager* col = boss->mMapCollisionManager;
		Mtx mtx;
		MsMtxSetTRS(mtx, colPos.x, colPos.y, colPos.z, boss->mRotation.x,
		            boss->mRotation.y, boss->mRotation.z, colScale.x,
		            colScale.y, colScale.z);
		TMapCollisionBase* base = col->getActiveCollision();
		base->setMtx(mtx);
		base->setUp();

		boss->mHits[0]->mHitFilter |= 1;
		boss->mHits[1]->mHitFilter |= 1;

		TBWLeash* leash = boss->mLeash;
		leash->invalidateAllCollision();
		boss->mPicket->onHitFilter(1);
		// to here*

		boss->changeBck(1);
		actor->setBtpFromIndex(0);
		actor->setBrkFromIndex(1);
	}

	if (spine->getTime() > 60 && boss->unk18D == 0
	    && gpMarDirector->getGameMode() != TMarDirector::GAME_MODE_INNER_DEMO) {
		gpItemManager->makeShineAppearWithDemo(
		    "シャイン（ボス用）", "ボスシャインカメラ", boss->mPosition.x,
		    boss->mPosition.y, boss->mPosition.z);
		boss->unk18D = 1;
	}

	if (actor->curAnmEndsNext(5, nullptr)) {
		actor->getFrameCtrl(5)->setRate(0.0f);
	}

	return false;
}

DEFINE_NERVE(TNerveBWJumpAway, TLiveActor)
{
	TBossWanwan* boss = static_cast<TBossWanwan*>(spine->getBody());

	if (spine->getTime() == 0) {
		JGeometry::TVec3<f32> vel = boss->calcVelocityToJumpToY(
		    BW_HEAD_START, 40.0f, boss->getGravityY());

		TPathNode goal(BW_HEAD_START);
		boss->unkF4  = goal;
		boss->unk104 = goal;
		boss->unk114.clear();

		boss->mVelocity = vel;
		boss->onLiveFlag(LIVE_FLAG_AIRBORNE);
		boss->unk16C = 0;
	}

	if (boss->isBurning()) {
		spine->pushAfterCurrent(&TNerveBWFall::theNerve());
		return true;
	}

	if (boss->isReachedToGoal()) {
		boss->mPosition        = BW_HEAD_START;
		boss->unk124->mPrevIdx = -1;
		boss->unk124->mCurrIdx = -1;
		boss->goToShortestNextGraphNode();

		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		return true;
	}

	boss->walkToCurPathNode(0.0f, boss->mTurnSpeed, 0.0f);
	return false;
}

DEFINE_NERVE(TNerveBWShake, TLiveActor)
{
	TBossWanwan* boss = static_cast<TBossWanwan*>(spine->getBody());
	MActor* actor     = boss->getMActor();

	if (spine->getTime() == 0) {
		boss->changeBck(2);
	}

	if (actor->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
		boss->releasePicket();

		boss->unk194       = 0;
		boss->mIsNearMario = 0;
		SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_ESCAPE2,
		                                &boss->mPicket->mPosition, 0, nullptr,
		                                0, 4);
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBWFall, TLiveActor)
{
	TBossWanwan* boss = static_cast<TBossWanwan*>(spine->getBody());
	if (spine->getTime() == 0) {
		JGeometry::TVec3<f32> vel = boss->calcVelocityToJumpToY(
		    boss->mPicket->mPosition, 5.0f, boss->getGravityY());

		TPathNode goal(boss->mPicket->mPosition);
		boss->unkF4  = goal;
		boss->unk104 = goal;
		boss->unk114.clear();

		boss->mVelocity = vel;
		boss->mLiveFlag |= 0x80;
		boss->unk16C = 0;
	}

	if (boss->isReachedToGoal()) {

		boss->mPosition        = boss->mPicket->mPosition;
		boss->unk124->mPrevIdx = -1;
		boss->unk124->mCurrIdx = -1;
		boss->goToShortestNextGraphNode();

		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		return true;
	}

	return false;
}
