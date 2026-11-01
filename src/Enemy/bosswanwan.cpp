#include <Enemy/BossWanwan.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/Graph.hpp>
#include <Camera/CameraShake.hpp>
#include <GC2D/GCConsole2.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <M3DUtil/MActor.hpp>
#include <System/EmitterViewObj.hpp>
#include <M3DUtil/MActorData.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MSound/MSound.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Mario.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// How the chomp moves, in short:
//
// - It lives on the "bwanwan" graph. TBWBinder::bind() only lets it move
//   along the link between the previous and the current graph node, and pulls
//   it back onto that link.
// - TNerveBWGraphWander is the normal state. On reaching a node, the chomp
//   either jumps (graph node flag 1, see jumpToNextGraphNode) or picks the next
//   node with rollNextGraphNode(): among the links within 100 degrees of where
//   it faces, the one that leads most away from Mario.
// - Its walking speed scales with its heat (mHitPoints / mSLBWHitPointMax).
//   Water cools it down by one point per hit, a thrown wooden barrel cools it
//   down completely, and it heats back up by one point every 20 frames.
// - Once the picket is out of the ground and Mario is holding it, a pull on
//   the chain drags the chomp. While dragged it treats the previous node as
//   the one ahead, and on reaching it picks the link pointing most towards
//   Mario (reverseNextGraphNode). A hot chomp resists by sliding back towards
//   its node. Dragging it within 400 units of the node closest to the bath
//   (BW_BATH_POS) makes it jump into the bath.

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

// fabricated (the same helper is faked in bgtentacle.cpp and bossgesso.cpp,
// so the real one probably lives in a shared header)
// TODO: the original emits the out-of-line TVec3::set<f> right after
// TNerveBWRoll::execute, ours right after TNerveBWGraphWander::execute;
// validate-symbol-order reports this.
static inline JGeometry::TVec3<f32> fromPolar(f32 theta, f32 radius)
{
	return JGeometry::TVec3<f32>(radius * JMASSin(theta * (65536.0f / 360.0f)),
	                             0.0f,
	                             radius * JMASCos(theta * (65536.0f / 360.0f)));
}

TBWParams::TBWParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mSLMarchSpeed, 6.0f)
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
    , PARAM_INIT(mSLShakeLengthMax, 3000.0f)
    , PARAM_INIT(mSLShakeLengthMaxHP0, 2000.0f)
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

	f32 radius = owner->getBWParams()->mSLChainHitRadius.get();
	f32 height = owner->getBWParams()->mSLChainHitHeight.get();
	initHitActor(ACTOR_TYPE_BOSS_UNKC, 1, HIT_CATEGORY_PLAYER, 1.2f * radius,
	             1.2f * height, radius, height);
}

void TBWLeashNode::calcTemperature()
{
	if (mIndex == 0)
		return;

	int prevIndex = mIndex - 1;
	f32 diff      = mLeash->mNodes[prevIndex]->mTemperature - mTemperature;
	f32 step;
	if (diff < 0.0f) {
		if (diff < -0.1f)
			step = -0.02f;
		else
			step = -0.005f;
	} else {
		if (diff > 0.1f)
			step = 0.02f;
		else
			step = 0.005f;
	}

	mTemperature += step;
	if (mTemperature < 0.0f)
		mTemperature = 0.0f;
	if (mTemperature > 1.0f)
		mTemperature = 1.0f;
}

void TBWLeashNode::calcMatrix()
{
	TRope* rope        = mLeash->mRope;
	TRopePoint* points = rope->mPoints;
	MtxPtr mtx         = mMActor->getModel()->getBaseTRMtx();

	JGeometry::TVec3<f32> pos = points[mIndex].unkC;
	JGeometry::TVec3<f32> front;
	if (mIndex < rope->mNumPoints - 1) {
		front = points[mIndex + 1].unkC;
		front -= pos;
	} else {
		front = points[mIndex - 1].unkC;
		front -= pos;
		front.negate();
	}
	VECNormalize(&front, &front);

	JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
	JGeometry::TVec3<f32> side;
	side.cross(up, front);
	VECNormalize(&side, &side);
	up.cross(front, side);
	VECNormalize(&up, &up);

	mtx[0][2] = front.x;
	mtx[1][2] = front.y;
	mtx[2][2] = front.z;

	// Every other link is turned by 90 degrees, like a real chain.
	if (mIndex & 1) {
		mtx[0][0] = side.x;
		mtx[1][0] = side.y;
		mtx[2][0] = side.z;
		mtx[0][1] = up.x;
		mtx[1][1] = up.y;
		mtx[2][1] = up.z;
	} else {
		mtx[0][0] = up.x;
		mtx[1][0] = up.y;
		mtx[2][0] = up.z;
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
			for (int i = 0; i < getColNum(); ++i) {
				THitActor* actor = getCollision(i);
				if (actor->getActorType() == ACTOR_TYPE_MARIO)
					actor->receiveMessage(this, HIT_MESSAGE_BURN);
			}
		}
	}

	if (cue & CUE_CALC_ANIM) {
		J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(5);
		if (ctrl != nullptr) {
			f32 end = mTemperature * (ctrl->getEnd() - 1);
			f32 heat;
			if (mIndex < 5) {
				heat = mIndex * 0.25f
				       + (f32)mLeash->mOwner->mHitPoints
				             / (f32)mLeash->mOwner->getBWParams()
				                   ->mSLBWHitPointMax.get();
			} else if (mIndex >= mLeash->mRope->mNumPoints - 10) {
				heat = (mLeash->mRope->mNumPoints - mIndex) / 10.0f;
			} else {
				heat = 1.0f;
			}

			if (heat > 1.0f)
				heat = 1.0f;
			else if (heat < 0.0f)
				heat = 0.0f;

			end *= heat;
			ctrl->setFrame(end);
			ctrl->setRate(0.0f);
		}
	}

	if (mIndex < mLeash->mRope->mNumPoints - 1)
		mMActor->perform(cue, graphics);
}

TBWLeash::TBWLeash(TBossWanwan* owner, int num, const char* name)
    : JDrama::TViewObj(name)
    , mOwner(owner)
    , mRope(nullptr)
    , mNodes(nullptr)
{
	f32 nodeLen   = mOwner->getBWParams()->mSLLeashNodeLen.get();
	f32 groundRad = mOwner->getBWParams()->mSLChainGroundRadius.get();
	mRope = new TRope(num, mOwner->mPosition, nodeLen, groundRad, 0.7f, -2.0f);

	mNodes = new TBWLeashNode*[num];
	for (int i = 0; i < num; ++i)
		mNodes[i] = new TBWLeashNode(this, i, "鎖部品");

	TIdxGroupObj* group
	    = static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"));
	for (int i = 0; i < num; ++i) {
		group->getChildren().push_back(mNodes[i]);

		// The last two links next to the picket never collide.
		if (i < num - 2)
			mNodes[i]->offHitFilter(HIT_FILTER_NO_COLLISION);
		else
			mNodes[i]->onHitFilter(HIT_FILTER_NO_COLLISION);

		if (i < 2)
			mRope->mPoints[i].unk28 |= 1;
		else
			mRope->mPoints[i].unk28 &= ~1;
	}
}

void TBWLeash::invalidateAllCollision()
{
	for (int i = 0; i < mRope->mNumPoints; ++i)
		mNodes[i]->onHitFilter(HIT_FILTER_NO_COLLISION);
}

void TBWLeash::pullTail(const JGeometry::TVec3<f32>& tail)
{
	JGeometry::TVec3<f32> head;
	head = mRope->mPoints[0].unkC;
	mRope->constraintTail(tail);
	head -= mRope->mPoints[0].unkC;
	head.negate();
	mOwner->unk15C = head;
}

void TBWLeash::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		JGeometry::TVec3<f32> head;
		mOwner->getJointTransByIndex(5, &head);
		mRope->moveHead(head);

		mOwner->unk188 = 0;
		if (mOwner->unk17C != 0) {
			pullTail(mOwner->mPicket->mPosition);

			// Keep the chomp within reach of the staked picket.
			JGeometry::TVec3<f32> toHead = mRope->mPoints[0].unkC;
			JGeometry::TVec3<f32> body   = mOwner->mPosition;
			body.y += 500.0f;
			toHead -= body;
			if (VECMag(&toHead) > 650.0f) {
				VECNormalize(&toHead, &toHead);
				toHead *= 20.0f;
				toHead.y = 0.0f;
				mOwner->mPosition += toHead;
				mOwner->unk188 = 1;
			}

			f32 target
			    = MsWrap(MsGetRotFromZaxisY(toHead) + 180.0f, 0.0f, 360.0f);
			f32 diff = MsAngleDiff(target, mOwner->mRotation.y);
			f32 turn;
			if (diff > 0.0f)
				turn = MsMin(diff, 1.5f * mOwner->mTurnSpeed);
			else
				turn = MsMax(diff, 1.5f * -mOwner->mTurnSpeed);
			mOwner->mRotation.y
			    = MsWrap(mOwner->mRotation.y + turn, 0.0f, 360.0f);
		}
	}

	if (cue & CUE_CALC_ANIM) {
		JGeometry::TVec3<f32> center;
		mOwner->getJointTransByIndex(1, &center);
		for (int i = 0; i < 15; ++i) {
			TRopePoint& point          = mRope->mPoints[i];
			JGeometry::TVec3<f32> diff = point.unkC;
			diff -= center;
			f32 dist = VECMag(&diff);
			if (dist < 500.0f) {
				diff *= 500.0f / dist;
				diff += center;
				point.unk18.zero();
				point.unkC = diff;
				point.unk0 = point.unkC;
			}
		}
	}

	for (int i = 0; i < mRope->mNumPoints; ++i)
		mNodes[i]->testPerform(cue, graphics);
}

TBWPicket::TBWPicket(TBossWanwan* owner, const char* name)
    : TTakeActor(name)
    , mOwner(owner)
    , mMActor(nullptr)
{
	MTXIdentity(mTakingMtx);
	initHitActor(ACTOR_TYPE_BOSS_WANWAN, 1, HIT_CATEGORY_PLAYER,
	             mOwner->getBWParams()->mSLPicketRadius.get(),
	             mOwner->getBWParams()->mSLPicketHeight.get(),
	             mOwner->getBWParams()->mSLPicketRadius.get(),
	             mOwner->getBWParams()->mSLPicketHeight.get());

	TIdxGroupObj* group
	    = static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"));
	group->getChildren().push_back(this);

	offHitFilter(HIT_FILTER_NO_COLLISION);
	mMActor = mOwner->getActorKeeper()->createMActor("bwanwan_picket.bmd", 0);
}

BOOL TBWPicket::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->getActorType() == ACTOR_TYPE_MARIO) {
		if (message == HIT_MESSAGE_HIP_DROP) {
			// Ground pounding the picket stakes it back into the ground.
			TBossWanwan* owner = mOwner;
			owner->unk17C      = 1;
			owner->unk184      = 0;
			SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_LOCK, &mPosition,
			                                0, nullptr, 0, 4);
			return true;
		}

		if (message == HIT_MESSAGE_TAKE) {
			mOwner->releasePicket();
			mHolder = (TTakeActor*)sender;
			return true;
		}

		if (message == HIT_MESSAGE_THROWN || message == HIT_MESSAGE_DETACH) {
			mHolder = nullptr;
			return true;
		}
	}

	return false;
}

BOOL TBWPicket::moveRequest(const JGeometry::TVec3<f32>& where_to)
{
	if (mOwner->getLatestNerve() == &TNerveBWJumpToBath::theNerve()
	    || mOwner->getLatestNerve() == &TNerveBWDie::theNerve())
		return false;

	if (mOwner->mHitPoints != 0)
		return false;

	mOwner->mLeash->pullTail(where_to);
	return true;
}

MtxPtr TBWPicket::getTakingMtx() { return mTakingMtx; }

void TBWPicket::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		TRope* rope = mOwner->mLeash->mRope;
		mPosition   = rope->mPoints[rope->mNumPoints - 1].unkC;
		ensureTakeSituation();

		if (mHolder != nullptr) {
			TBWLeash* leash = mOwner->mLeash;
			JGeometry::TVec3<f32> front;
			front.set(mHolder->mPosition);
			front -= leash->mRope->mPoints[0].unkC;
			JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
			JGeometry::TVec3<f32> side;
			side.cross(up, front);
			VECNormalize(&side, &side);
			up.cross(front, side);
			VECNormalize(&up, &up);
			front.cross(side, up);
			VECNormalize(&front, &front);

			mTakingMtx.mMtx[0][0] = side.x;
			mTakingMtx.mMtx[1][0] = side.y;
			mTakingMtx.mMtx[2][0] = side.z;
			mTakingMtx.mMtx[0][1] = up.x;
			mTakingMtx.mMtx[1][1] = up.y;
			mTakingMtx.mMtx[2][1] = up.z;
			mTakingMtx.mMtx[0][2] = front.x;
			mTakingMtx.mMtx[1][2] = front.y;
			mTakingMtx.mMtx[2][2] = front.z;
			mTakingMtx.mMtx[0][3] = mPosition.x;
			mTakingMtx.mMtx[1][3] = mPosition.y;
			mTakingMtx.mMtx[2][3] = mPosition.z;

			mHolder->moveRequest(mPosition);
		}
	}

	if (cue & CUE_CALC_ANIM) {
		MtxPtr mtx = mMActor->getModel()->getBaseTRMtx();
		MTXIdentity(mtx);

		TBWLeash* leash = mOwner->mLeash;
		JGeometry::TVec3<f32> front;
		front.set(mPosition);
		front -= leash->mRope->mPoints[leash->mRope->mNumPoints - 3].unkC;
		if (front.squared() <= 3.814697265625e-06f)
			front.set(0.0f, 0.0f, 1.0f);

		JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
		JGeometry::TVec3<f32> side;
		side.cross(up, front);
		VECNormalize(&side, &side);
		up.cross(front, side);
		VECNormalize(&up, &up);
		front.cross(side, up);
		VECNormalize(&front, &front);

		mtx[0][0] = side.x;
		mtx[1][0] = side.y;
		mtx[2][0] = side.z;
		mtx[0][1] = up.x;
		mtx[1][1] = up.y;
		mtx[2][1] = up.z;
		mtx[0][2] = front.x;
		mtx[1][2] = front.y;
		mtx[2][2] = front.z;
		mtx[0][3] = mPosition.x;
		mtx[1][3] = mPosition.y;
		mtx[2][3] = mPosition.z;

		if (mOwner->unk17C == 0)
			mtx[1][3] += 70.0f;

		if (isTaken()) {
			mtx[0][3] -= 50.0f * front.x;
			mtx[2][3] -= 50.0f * front.z;
		} else {
			mtx[0][3] -= 60.0f * front.x;
			mtx[2][3] -= 60.0f * front.z;
		}
	}

	mMActor->perform(cue, graphics);
	THitActor::perform(cue, graphics);
}

TBWHit::TBWHit(TBossWanwan* owner, int joint_index, const char* name)
    : THitActor(name)
    , mOwner(owner)
    , mJointIndex(joint_index)
{
}

BOOL TBWHit::receiveMessage(THitActor* sender, u32 message)
{
	return mOwner->receiveMessage(sender, message);
}

void TBWHit::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		if (mJointIndex >= 0)
			mOwner->getJointTransByIndex(mJointIndex, &mPosition);

		for (int i = 0; i < getColNum(); ++i) {
			THitActor* actor = getCollision(i);
			if (mOwner->mHitPoints != 0
			    && actor->getActorType() == ACTOR_TYPE_MARIO)
				actor->receiveMessage(mOwner, HIT_MESSAGE_BURN);
		}
	}

	THitActor::perform(cue, graphics);
}

TBWBinder::TBWBinder() { }

void TBWBinder::bind(TLiveActor* actor)
{
	TBossWanwan* bw = (TBossWanwan*)actor;

	JGeometry::TVec3<f32> delta = bw->mPositionDelta;
	JGeometry::TVec3<f32> next  = bw->mPosition;
	next += delta;

	if (bw->isAirborne()) {
		JGeometry::TVec3<f32> vel = bw->mVelocity;
		next += vel;
		vel.y -= bw->getGravityY();
		if (vel.y < TLiveActor::mVelocityMinY)
			vel.y = TLiveActor::mVelocityMinY;
		bw->mVelocity = vel;
	}

	if (bw->mSpine->isNerve(&TNerveBWJumpToBath::theNerve())
	    || bw->mSpine->isNerve(&TNerveBWDie::theNerve())) {
		bw->mPositionDelta = next - bw->mPosition;
		return;
	}

	if (bw->isAirborne()) {
		const TBGCheckData* ground;
		f32 groundY = gpMap->checkGround(next.x, next.y + bw->getHeadHeight(),
		                                 next.z, &ground);
		groundY += 1.0f;
		if (bw->mPosition.y > next.y && !ground->isEnemyThrough()) {
			const TBGCheckData* ground2;
			f32 groundY2 = gpMap->checkGround(
			    next.x, bw->mPosition.y + bw->getHeadHeight(), next.z,
			    &ground2);
			groundY2 += 1.0f;
			if (groundY2 > groundY) {
				groundY = groundY2;
				ground  = ground2;
			}
		}

		if (next.y <= groundY && !ground->checkFlag(0x10)
		    && !ground->isEnemyThrough()) {
			next.y = groundY;
			JGeometry::TVec3<f32> zero(0.0f, 0.0f, 0.0f);
			bw->mVelocity = zero;
			bw->offLiveFlag(LIVE_FLAG_AIRBORNE);
			bw->offLiveFlag(LIVE_FLAG_UNK8000);
		} else {
			bw->onLiveFlag(LIVE_FLAG_AIRBORNE);
		}

		bw->mGroundHeight = groundY;
		bw->mGroundPlane  = ground;
	}

	delta = next - bw->mPosition;

	// Only allow movement along the current graph link.
	if (!bw->isAirborne()) {
		TGraphTracer* tracer = bw->unk124;
		if (tracer->getGraph() != nullptr) {
			int cur  = tracer->mCurrIdx;
			int prev = tracer->mPrevIdx;
			if (cur >= 0 && prev >= 0 && cur != prev) {
				TGraphNode& curNode  = tracer->getCurrent();
				TGraphNode& prevNode = tracer->getPrevious();
				JGeometry::TVec3<f32> a;
				JGeometry::TVec3<f32> b;
				curNode.getPoint(&a);
				prevNode.getPoint(&b);
				a -= b;
				VECNormalize(&a, &a);

				f32 sq  = a.squared();
				f32 len = sq == 0.0f ? 0.0f : delta.dot(a) / sq;

				f32 speed = len;
				if (len < 0.0f) {
					if (len > -3.0f)
						speed = -3.0f;
				} else if (len > 0.0f && len < 3.0f) {
					speed = 3.0f;
				}

				delta = a;
				delta *= speed;
			}
		}
	}

	// Roll the body by the distance it moved.
	if (!bw->isAirborne()) {
		JGeometry::TVec3<f32> moved = delta;
		f32 dist                    = VECMag(&moved);
		if (dist != 0.0f) {
			f32 roll                    = 360.0f * (dist / 3141.5927f);
			JGeometry::TVec3<f32> front = fromPolar(bw->mRotation.y, 1.0f);
			if (front.dot(delta) < 0.0f)
				roll = -roll;
			roll *= 2.0f;

			if (bw->unk16C != 0) {
				bw->unk168 = MsAngleWrap(roll + bw->unk168);
			} else {
				f32 angle = bw->unk168;
				if (angle != 0.0f) {
					angle += roll;
					if (angle > 360.0f)
						angle = 0.0f;
					bw->unk168 = angle;
				}
			}
		}
	}

	// While staked, the chomp can never go further than the chain allows.
	if (bw->unk17C != 0) {
		TBWLeash* leash               = bw->mLeash;
		JGeometry::TVec3<f32> pos     = bw->mPosition;
		JGeometry::TVec3<f32> nextPos = pos;
		JGeometry::TVec3<f32> origPos = pos;
		nextPos += delta;
		JGeometry::TVec3<f32> anchor = leash->mRope->mPoints[3].unkC;
		nextPos -= anchor;
		if (VECMag(&nextPos) > 860.0f) {
			VECNormalize(&nextPos, &nextPos);
			nextPos *= 860.0f;
			nextPos += anchor;
			nextPos -= origPos;
			delta = nextPos;
		}
	}

	// Pull it back onto the current link.
	if (!bw->isAirborne()) {
		TGraphTracer* tracer = bw->unk124;
		TGraphWeb* graph     = tracer->getGraph();
		TGraphNode& curNode  = graph->getGraphNode(tracer->mCurrIdx);
		TGraphNode& prevNode = graph->getGraphNode(tracer->mPrevIdx);
		JGeometry::TVec3<f32> a;
		JGeometry::TVec3<f32> b;
		curNode.getPoint(&a);
		prevNode.getPoint(&b);
		JGeometry::TVec3<f32> foot
		    = MsPerpendicFootToLineR(b, a, bw->mPosition);
		a -= b;
		foot -= bw->mPosition;
		f32 dist = VECMag(&foot);
		if (dist > 10.0f)
			dist = 10.0f;
		if (dist < 0.0001f) {
			foot.zero();
		} else {
			VECNormalize(&foot, &foot);
			foot *= dist;
		}
		delta += foot;
	}

	bw->mPositionDelta = delta;
}

TBossWanwanMtxCalc::TBossWanwanMtxCalc(TBossWanwan* owner)
    : M3UMtxCalcSIAnmBlendQuat(false)
    , mOwner(owner)
{
	mMotionBlendRatio = 0.0f;
}

void TBossWanwanMtxCalc::joinAnm(int index)
{
	M3UMtxCalcSIAnmBlendQuat::joinAnm(getBckAnmPtr(index));
}

void TBossWanwanMtxCalc::calc(u16 joint_no)
{
	if (joint_no == 0 && mOwner->isAirborne()) {
		// While airborne the root joint ignores the animation's translation.
		j3dSys.setCurrentMtxCalc(this);

		J3DTransformInfo info;
		if (mNewAnm != nullptr)
			mNewAnm->getTransform(joint_no, &info);
		else
			info = j3dSys.getModel()
			           ->getModelData()
			           ->getJointNodePointer(joint_no)
			           ->getTransformInfo();

		info.mTranslate.x = 0.0f;
		info.mTranslate.y = 0.0f;
		info.mTranslate.z = 0.0f;
		calcTransform(joint_no, info);
	} else {
		M3UMtxCalcSIAnmBlendQuat::calc(joint_no);

		if (joint_no == 1) {
			// Rolling the ball by unk168 degrees.
			// TODO: the original keeps &rot pinned in r29 across getModel()
			// (and has a 0x10 bigger frame); not found what causes that yet.
			TPosition3f rot;
			rot.zero();
			MsMtxSetRotX(rot, mOwner->unk168);

			MtxPtr mtx = mOwner->getModel()->getAnmMtx(joint_no);
			MTXConcat(mtx, rot, mtx);
			MTXCopy(mtx, J3DSys::mCurrentMtx);
		}
	}
}

TBossWanwan::TBossWanwan(const char* name)
    : TSpineEnemy(name)
    , mMtxCalc(nullptr)
    , mLeash(nullptr)
    , mPicket(nullptr)
    , unk168(0.0f)
    , unk16C(0)
    , unk17C(0)
    , unk180(0)
    , unk184(0)
    , unk188(0)
    , unk18C(0)
    , unk18D(0)
    , unk190(0)
    , unk194(1)
    , unk195(0)
    , unk198(0)
    , unk19C(0)
    , unk1A0(0)
    , unk1A4(0.0f, 0.0f, 0.0f)
    , unk1B0(0)
    , unk1B4(0)
{
	mBinder = new TBWBinder;
}

void TBossWanwan::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);

	mMActorKeeper = new TMActorKeeper(mManager, 17);
	mMActor       = mMActorKeeper->createMActor("bwanwan_body.bmd", 0);

	TGraphWeb* graph = gpConductor->getGraphByName("bwanwan");
	graph->initGoalIndex(BW_BATH_POS);
	unk124->setGraph(graph);

	mSpine->initWith(&TNerveBWGraphWander::theNerve());

	mMarchSpeed = getBWParams()->mSLMarchSpeed.get();
	mTurnSpeed  = getBWParams()->mSLTurnSpeed.get();

	mPosition = BW_HEAD_START;
	reset();

	mLeash             = new TBWLeash(this, 15, "ボスワンワン鎖");
	mPicket            = new TBWPicket(this, "ボスワンワンつかみ");
	mPicket->mPosition = BW_PICKET_START;
	unk17C             = 1;
	unk184             = 0;

	goToRandomNextGraphNode();

	initHitActor(ACTOR_TYPE_BOSS_UNKB, 1, HIT_CATEGORY_PLAYER, 0.0f, 0.0f, 0.0f,
	             0.0f);

	unk170[0] = new TBWHit(this, 3, "ボスワンワンヒット");
	unk170[0]->initHitActor(ACTOR_TYPE_BOSS_UNKB, 3,
	                        HIT_CATEGORY_PLAYER | HIT_CATEGORY_ITEM, 500.0f,
	                        500.0f, 450.0f, 500.0f);
	unk170[1] = new TBWHit(this, -1, "ボスワンワンヒット");
	unk170[1]->initHitActor(ACTOR_TYPE_BOSS_UNKB, 3,
	                        HIT_CATEGORY_PLAYER | HIT_CATEGORY_ITEM, 300.0f,
	                        500.0f, 270.0f, 500.0f);

	TIdxGroupObj* group
	    = static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"));
	for (int i = 0; i < 2; ++i) {
		group->getChildren().push_back(unk170[i]);
		unk170[i]->offHitFilter(HIT_FILTER_NO_COLLISION);
	}

	initAnmSound();
	mScaledBodyRadius = 500.0f;

	mMtxCalc = new TBossWanwanMtxCalc(this);
	mMActor->setBckMtxCalc(mMtxCalc);
	mMActor->calc();
	offLiveFlag(LIVE_FLAG_ENABLE_CLIPPING);

	changeBck(4);
	mMActor->setBrkFromIndex(0);
	mMActor->setLightType(1);

	J3DModel* model = mMActor->getModel();
	if (model->getSkinDeform() == nullptr)
		model->setSkinDeform(new J3DSkinDeform, J3DDeformAttachFlag(1));

	mHitPoints = getBWParams()->mSLBWHitPointMax.get();
	unk15C.zero();

	mMapCollisionManager = new TMapCollisionManager(1, "/scene/bwanwan", this);
	mMapCollisionManager->init("bwanwan_ofuro_col.col", 2, nullptr);
	mMapCollisionManager->setUpActiveCollisionTRS(mPosition, mRotation,
	                                              mScaling);
	mMapCollisionManager->removeActiveCollision();
}

void TBossWanwan::shakeCamera(int mode)
{
	if (!SMS_IsMarioTouchGround4cm())
		return;

	f32 dist      = MsSqrtf(mDistToMarioSquared);
	f32 lenMax    = getBWParams()->mSLShakeLengthMax.get();
	f32 lenMaxHP0 = getBWParams()->mSLShakeLengthMaxHP0.get();
	f32 heat;
	if (getMActor()->checkCurBckFromIndex(0))
		heat = 1.0f;
	else
		heat = (f32)getHitPoints() / (f32)getBWParams()->mSLBWHitPointMax.get();

	f32 length = lenMax * heat + lenMaxHP0 * (1.0f - heat);
	f32 left   = length - dist;
	if (left < 0.0f)
		return;

	f32 power = left / length;
	if (power > 1.0f)
		power = 1.0f;

	gpCameraShake->startShake((EnumCamShakeMode)mode, power * heat);
	SMSRumbleMgr->start(8, &mPosition);
}

BOOL TBossWanwan::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->getActorType() == ACTOR_TYPE_MARIO)
		return false;

	if (sender->getActorType() == ACTOR_TYPE_WATER) {
		if (unk18C != 0)
			return true;

		gpMarioParticleManager->emit(0xE7, &sender->mPosition, 0, nullptr);
		if (mHitPoints == 0) {
			SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_COOL_MORE,
			                                &mPosition, 0, nullptr, 0, 4);
		} else if (mHitPoints == 1) {
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    BWANWAN_JPA_MS_BWAN_DOWNYUGE, getModel()->getAnmMtx(1), 0,
			    nullptr);
			SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_TO_COOL,
			                                &mPosition, 0, nullptr, 0, 4);
		} else {
			SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_COOL, &mPosition,
			                                0, nullptr, 0, 4);
		}

		decHitPoints();
		unk190 += 1;
		return true;
	}

	if (sender->getActorType() == ACTOR_TYPE_WOOD_BARREL ? true : false) {
		// A barrel full of water cools the chomp down at once.
		sender->receiveMessage(this, HIT_MESSAGE_HIP_DROP);
		mHitPoints = 0;
		unk190 += 1;
		if (unk1A0 == 0)
			unk1A0 += 1;

		gpMarioParticleManager->emitAndBindToMtxPtr(
		    BWANWAN_JPA_MS_BWAN_DOWNYUGE, getModel()->getAnmMtx(1), 0, nullptr);
		SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_TO_COOL, &mPosition, 0,
		                                nullptr, 0, 4);
	}

	return TSpineEnemy::receiveMessage(sender, message);
}

void TBossWanwan::takeBath()
{
	unk16C = 0;
	unk168 = 0.0f;
	unk18C = 1;

	mPosition                 = BW_BATH_POS;
	JGeometry::TVec3<f32> pos = mPosition;
	pos.y += 500.0f;
	JGeometry::TVec3<f32> scale = mScaling;
	scale *= 1.1f;
	mMapCollisionManager->setUpActiveCollisionTRS(pos, mRotation, scale);

	unk170[0]->onHitFilter(HIT_FILTER_NO_COLLISION);
	unk170[1]->onHitFilter(HIT_FILTER_NO_COLLISION);
	mLeash->invalidateAllCollision();
	mPicket->onHitFilter(HIT_FILTER_NO_COLLISION);
}

void TBossWanwan::releasePicket()
{
	if (unk17C != 0) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emit(
		    BWANWAN_JPA_MS_BWAN_JUMP_SMOKE, &mPicket->mPosition, 0, nullptr);
		if (emitter != nullptr) {
			JGeometry::TVec3<f32> scale(0.3f, 0.5f, 0.3f);
			emitter->setGlobalScale(scale);
		}
	}

	unk194 = 0;
	unk17C = 0;
}

void TBossWanwan::changeBck(int index)
{
	mMtxCalc->joinAnm(index);
	getMActor()->setBckFrameCtrl(index);
	J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(0);
	unk178             = 10.0f / (f32)ctrl->getEnd();
	const char* bas    = bwanwan_bastable[index];
	setAnmSound(bas);
}

void TBossWanwan::startGoldBrk() { }

BOOL TBossWanwan::isBurning()
{
	// TODO: nothing in the binary calls this, so the body is a guess
	return mHitPoints != 0;
}

BOOL TBossWanwan::isTailBurning()
{
	// TODO: nothing in the binary calls this, so the body is unknown
	return FALSE;
}

BOOL TBossWanwan::isMarioInSight() { return FALSE; }

BOOL TBossWanwan::isHeadPulled()
{
	return unk15C.squared() < getBWParams()->mSLPullLimit.get() ? FALSE : TRUE;
}

void TBossWanwan::calcRootMatrix()
{
	J3DModel* model = getModel();
	model->setBaseScale(mScaling);
	MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x,
	               mPosition.y + 500.0f, mPosition.z, mRotation.x, mRotation.y,
	               mRotation.z);
}

void TBossWanwan::reverseNextGraphNode()
{
	TGraphTracer* tracer = unk124;
	int ahead            = tracer->mPrevIdx;
	int cur              = tracer->mCurrIdx;
	TGraphWeb* graph     = tracer->getGraph();

	JGeometry::TVec3<f32> toMario = SMS_GetMarioPos();
	toMario -= mPosition;
	unk124->mPrevIdx = graph->getAimToDirNextIndex(ahead, cur, toMario,
	                                               mPosition, 0xffffffff);
	unk124->mCurrIdx = ahead;

	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
}

void TBossWanwan::rollNextGraphNode()
{
	TGraphTracer* tracer = unk124;
	int prev             = tracer->mPrevIdx;
	int cur              = tracer->mCurrIdx;
	TGraphWeb* graph     = tracer->getGraph();

	if (prev >= 0
	    && graph->getGraphNode(prev).getRailNode()->mConnectionNum >= 2
	    && gpMarDirector->getMoveTickCount() >= 14400)
		showMessage(0xE001C);

	unk124->moveTo(graph->getEscapeDirLimited(cur, prev,
	                                          fromPolar(mRotation.y, 1.0f),
	                                          mPosition, 100.0f, 0xffffffff));
	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
}

void TBossWanwan::slideToCurPathNode(f32 speed, f32 turn_speed)
{
	JGeometry::TVec3<f32> toGoal = getCurrentTarget().getPoint();
	toGoal -= mPosition;
	f32 dist = VECMag(&toGoal);

	f32 target = MsWrap(MsGetRotFromZaxisY(toGoal), 0.0f, 360.0f);
	f32 diff   = MsAngleDiff(target, mRotation.y);
	f32 turn;
	if (diff > 0.0f)
		turn = MsMin(diff, turn_speed);
	else
		turn = MsMax(diff, -turn_speed);
	mRotation.y = MsWrap(mRotation.y + turn, 0.0f, 360.0f);

	JGeometry::TVec3<f32> delta = mPositionDelta;
	if (dist > 0.0f)
		toGoal *= speed / dist;
	delta += toGoal;
	mPositionDelta = delta;
}

void TBossWanwan::control()
{
	TLiveActor::control();

	if (unk17C != 0 || (mPicket->isTaken() && isHeadPulled())) {
		// Being pulled by the chain: move with it and look away from it.
		mPositionDelta += unk15C;

		JGeometry::TVec3<f32> away = mPosition;
		away -= mLeash->mRope->mPoints[3].unkC;

		f32 target = MsWrap(MsGetRotFromZaxisY(away), 0.0f, 360.0f);
		f32 diff   = MsAngleDiff(target, mRotation.y);
		f32 turn;
		if (diff > 0.0f)
			turn = MsMin(diff, 4.0f * mTurnSpeed);
		else
			turn = MsMax(diff, 4.0f * -mTurnSpeed);
		mRotation.y = MsWrap(mRotation.y + turn, 0.0f, 360.0f);
	}

	unk15C.zero();
	updateSquareToMario();
}

void TBossWanwan::emitEffects()
{
	BOOL landed = FALSE;
	if (mMActor->checkCurBckFromIndex(4) || mMActor->checkCurBckFromIndex(5)) {
		if (mMActor->checkBckPass(8.0f))
			landed = TRUE;
	} else if (mMActor->checkCurBckFromIndex(2)
	           && mMActor->checkBckPass(38.0f)) {
		landed = TRUE;
	}

	if (landed) {
		gpMarioParticleManager->emit(BWANWAN_JPA_MS_BWAN_JUMP_ROCK, &mPosition,
		                             0, nullptr);
		gpMarioParticleManager->emit(BWANWAN_JPA_MS_BWAN_JUMP_SMOKE, &mPosition,
		                             0, nullptr);
		if (mHitPoints == 0) {
			SMSGetMSound()->startSoundActor(MSD_SE_M_BOSW_CHAIN_CAL_F,
			                                &mLeash->mRope->mPoints[0].unkC, 0,
			                                nullptr, 0, 4);
			SMSGetMSound()->startSoundActor(MSD_SE_M_BOSW_CHAIN_CAL_R,
			                                &mPicket->mPosition, 0, nullptr, 0,
			                                4);
		} else {
			SMSGetMSound()->startSoundActor(MSD_SE_M_BOSW_CHAIN_ANG_F,
			                                &mLeash->mRope->mPoints[0].unkC, 0,
			                                nullptr, 0, 4);
			SMSGetMSound()->startSoundActor(MSD_SE_M_BOSW_CHAIN_ANG_R,
			                                &mPicket->mPosition, 0, nullptr, 0,
			                                4);
		}
	}

	BOOL sparks = FALSE;
	if (mMActor->checkCurBckFromIndex(0)) {
		if (mMActor->checkBckPass(72.0f))
			sparks = TRUE;
	} else if (mMActor->checkCurBckFromIndex(4)
	           || mMActor->checkCurBckFromIndex(5)) {
		if (mMActor->checkBckPass(6.0f) || mMActor->checkBckPass(12.0f))
			sparks = TRUE;
	} else if (mMActor->checkCurBckFromIndex(2)
	           && mMActor->checkBckPass(4.0f)) {
		sparks = TRUE;
	}

	if (sparks)
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    BWANWAN_JPA_MS_BWAN_HIBANA, getModel()->getAnmMtx(1), 0, this);

	if ((mMActor->checkCurBckFromIndex(4) || mMActor->checkCurBckFromIndex(5))
	    && mMActor->checkBckPass(10.0f))
		shakeCamera(0x16);

	if (mMActor->checkCurBckFromIndex(0)) {
		J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
		if (ctrl->checkPass(60.0f) || ctrl->checkPass(127.0f)) {
			shakeCamera(0x16);
			gpMarioParticleManager->emit(BWANWAN_JPA_MS_BWAN_JUMP_ROCK,
			                             &mPosition, 0, nullptr);
			gpMarioParticleManager->emit(BWANWAN_JPA_MS_BWAN_JUMP_SMOKE,
			                             &mPosition, 0, nullptr);
		}

		if (ctrl->checkPass(202.0f)) {
			shakeCamera(0x17);
			gpMarioParticleManager->emit(BWANWAN_JPA_MS_BWAN_JUMP_ROCK,
			                             &mPosition, 0, nullptr);
			gpMarioParticleManager->emit(BWANWAN_JPA_MS_BWAN_JUMP_SMOKE,
			                             &mPosition, 0, nullptr);
		}
	}

	if (mMActor->checkCurBckFromIndex(2) && mMActor->checkBckPass(40.0f))
		shakeCamera(0x16);

	if (mHitPoints != 0)
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    BWANWAN_JPA_MS_BWAN_YUGAMI, getModel()->getAnmMtx(1), 3, this);

	if (unk190 != 0 && mHitPoints != 0) {
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    BWANWAN_JPA_MS_BWAN_HITYUGE, getModel()->getAnmMtx(1), 1, this);
		unk190 = 0;
	}
}

void TBossWanwan::showMessage(u32 message)
{
	u32 bit = 1 << (message - 0xE001A);
	if (!(unk198 & bit))
		gpMarDirector->getConsole()->startAppearBalloon(message, true);
	unk198 |= bit;
}

void TBossWanwan::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (unk18C != 0) {
		// In the bath.
		unk170[0]->perform(cue, graphics);
		TSpineEnemy::perform(cue, graphics);

		if (cue & CUE_MOVE)
			mMtxCalc->advanceMotionBlend(-unk178);

		if (cue & 0x200) {
			J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(5);
			if (ctrl != nullptr && ctrl->getFrame() > 0.5f * ctrl->getEnd()) {
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
		f32 heat = (f32)mHitPoints / (f32)getBWParams()->mSLBWHitPointMax.get();
		ctrl->setFrame(heat * (ctrl->getEnd() - 1));
		ctrl->setRate(0.0f);
		if (mHitPoints == 0
		    || mHitPoints == getBWParams()->mSLBWHitPointMax.get())
			mLeash->mNodes[0]->mTemperature = heat;
		emitEffects();
	}

	unk170[0]->perform(cue, graphics);
	if (cue & CUE_MOVE)
		unk170[1]->mPosition.set(unk170[0]->mPosition.x,
		                         unk170[0]->mPosition.y - 500.0f,
		                         unk170[0]->mPosition.z);
	unk170[1]->perform(cue, graphics);

	if ((cue & CUE_MOVE) && unk194 == 0) {
		unk19C += 1;
		if (unk19C < 0)
			unk19C = 1;
		if (unk19C > 600)
			showMessage(0xE001A);
	}

	if ((cue & CUE_MOVE) && unk1A0 == 0
	    && gpMarDirector->getMoveTickCount() >= 28800)
		showMessage(0xE001D);

	if ((cue & CUE_MOVE) && !mSpine->isNerve(&TNerveBWDie::theNerve())
	    && !mSpine->isNerve(&TNerveBWJumpToBath::theNerve())) {
		if (mHitPoints != 0) {
			// A hot chomp that stays staked for 10 seconds pulls the
			// picket out of the ground.
			if (unk17C != 0 && unk194 == 0) {
				unk184 += 1;
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
			// Cold for 40 seconds: bark and heat back up.
			unk180 += 1;
			if (unk180 > 2400
			    && mSpine->getLatestNerve() != &TNerveBWBark::theNerve())
				mSpine->setNext(&TNerveBWBark::theNerve());
		} else {
			// Heat up by one point every 20 frames.
			if (gpMarDirector->getMoveTickCount() % 20 == 0
			    && mHitPoints < getBWParams()->mSLBWHitPointMax.get())
				mHitPoints += 1;
			unk180 = 0;
		}
	}

	if ((cue & CUE_MOVE) && mSpine->isNerve(&TNerveBWGraphWander::theNerve())
	    && mPicket->isTaken()) {
		f32 pull = unk15C.length();
		SMSGetMSound()->startSoundActorWithInfo(
		    MSD_SE_BS_WANWAN_M_DRAG, &mLeash->mRope->mPoints[6].unkC, nullptr,
		    pull, 0, 0, nullptr, 0, 4);
	}

	TSpineEnemy::perform(cue, graphics);
	mLeash->testPerform(cue, graphics);
	mPicket->testPerform(cue, graphics);

	if (cue & CUE_MOVE)
		mMtxCalc->advanceMotionBlend(-unk178);
}

TBossWanwanManager::TBossWanwanManager(const char* name)
    : TEnemyManager(name)
{
}

void TBossWanwanManager::initJParticle()
{
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_jump_rock.jpa",
	                 BWANWAN_JPA_MS_BWAN_JUMP_ROCK);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_jump_smoke.jpa",
	                 BWANWAN_JPA_MS_BWAN_JUMP_SMOKE);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_downyuge.jpa",
	                 BWANWAN_JPA_MS_BWAN_DOWNYUGE);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_hibana.jpa",
	                 BWANWAN_JPA_MS_BWAN_HIBANA);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_deadyuge.jpa",
	                 BWANWAN_JPA_MS_BWAN_DEADYUGE);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_yugami.jpa",
	                 BWANWAN_JPA_MS_BWAN_YUGAMI);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_hityuge.jpa",
	                 BWANWAN_JPA_MS_BWAN_HITYUGE);
	SMS_LoadParticle("/scene/bwanwan/jpa/ms_bwan_kira.jpa",
	                 BWANWAN_JPA_MS_BWAN_KIRA);
}

TSpineEnemy* TBossWanwanManager::createEnemyInstance()
{
	return new TBossWanwan("ボスワンワン");
}

void TBossWanwanManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "bwanwan_body.bmd",
		  J3DMLF_MaterialPEFull | (2 << J3DMLF_TevStageNumShift), 0 },
		{ "bwanwan_chain.bmd",
		  J3DMLF_MaterialPEFull | (2 << J3DMLF_TevStageNumShift), 0 },
		{ "bwanwan_picket.bmd",
		  J3DMLF_MaterialPEFull | (2 << J3DMLF_TevStageNumShift), 0 },
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
	TBossWanwan* self = (TBossWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		self->unk16C = 0;
		// TODO: the second check was probably meant to be index 5
		if (!self->getMActor()->checkCurBckFromIndex(4)
		    && !self->getMActor()->checkCurBckFromIndex(4)) {
			if (self->mHitPoints == 0)
				self->changeBck(5);
			else
				self->changeBck(4);

			J3DFrameCtrl* ctrl = self->getMActor()->getFrameCtrl(0);
			ctrl->setFrame(0.0f);
			ctrl->setRate(SMSGetAnmFrameRate());
		}

		self->getMActor()->setBtpFromIndex(0);
		J3DFrameCtrl* ctrl = self->getMActor()->getFrameCtrl(3);
		ctrl->setFrame(0.0f);
		ctrl->setRate(0.0f);
	}

	if (self->getMActor()->curAnmEndsNext(0, nullptr) && self->mHitPoints == 0
	    && !self->getMActor()->checkCurBckFromIndex(5)) {
		self->changeBck(5);
		J3DFrameCtrl* ctrl = self->getMActor()->getFrameCtrl(0);
		ctrl->setFrame(0.0f);
		ctrl->setRate(SMSGetAnmFrameRate());
	}

	if (self->unk17C == 0 && self->mPicket->isTaken() && self->isHeadPulled()) {
		// Mario is dragging the chomp by its chain.
		TGraphTracer* tracer = self->unk124;
		TGraphWeb* graph     = tracer->getGraph();
		int ahead            = tracer->mPrevIdx;

		JGeometry::TVec3<f32> toNode;
		tracer->getPrevious().getPoint(&toNode);
		toNode -= self->mPosition;
		if (VECMag(&toNode) < 400.0f && ahead == graph->unk10) {
			spine->pushAfterCurrent(&TNerveBWJumpToBath::theNerve());
			return true;
		}

		if (VECMag(&toNode) < 400.0f)
			self->reverseNextGraphNode();

		f32 heat = (f32)self->mHitPoints
		           / (f32)self->getBWParams()->mSLBWHitPointMax.get();
		self->slideToCurPathNode(3.0f * (heat * self->mMarchSpeed),
		                         self->mTurnSpeed);
		return false;
	}

	if (self->isReachedToGoal()) {
		if (self->jumpToNextGraphNode() >= 0) {
			spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
			spine->pushAfterCurrent(&TNerveBWJump::theNerve());
		} else {
			spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
			self->rollNextGraphNode();
		}
		return true;
	}

	self->unk16C = 0;
	f32 heat     = (f32)self->mHitPoints
	           / (f32)self->getBWParams()->mSLBWHitPointMax.get();
	if (self->mPicket->isTaken()) {
		// Mario holding the picket can hold the chomp back by leaning away.
		JGeometry::TVec3<f32> stick = fromPolar(
		    gpMarioOriginal->mIntendedYaw * (360.0f / 65536.0f), 1.0f);
		JGeometry::TVec3<f32> fromMario = self->mPosition;
		fromMario -= gpMarioOriginal->mPosition;
		VECNormalize(&fromMario, &fromMario);

		f32 pull = gpMarioOriginal->mIntendedMag / 32.0f * 0.75;
		pull *= -fromMario.dot(stick);
		f32 rate = 1.0f - pull;
		if (rate < 0.0f)
			rate = 0.0f;
		else if (1.5f < rate)
			rate = 1.5f;

		self->slideToCurPathNode(rate * (heat * self->mMarchSpeed),
		                         self->mTurnSpeed);
	} else {
		self->slideToCurPathNode(heat * self->mMarchSpeed + 0.2f,
		                         self->mTurnSpeed);
	}

	return false;
}

DEFINE_NERVE(TNerveBWRoll, TLiveActor)
{
	TBossWanwan* self = (TBossWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		J3DFrameCtrl* ctrl = self->getMActor()->getFrameCtrl(0);
		ctrl->setFrame(0.0f);
		ctrl->setRate(0.0f);
		self->unk16C = 1;
	}

	if (self->isReachedToGoal()) {
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		J3DFrameCtrl* ctrl = self->getMActor()->getFrameCtrl(0);
		ctrl->setRate(SMSGetAnmFrameRate());
		return true;
	}

	self->walkToCurPathNode(self->getBWParams()->mSLAttackSpeed.get(),
	                        self->mTurnSpeed, 0.0f);
	return false;
}

DEFINE_NERVE(TNerveBWBark, TLiveActor)
{
	TBossWanwan* self = (TBossWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(0);
		self->unk16C = 0;
		self->unk168 = 0.0f;
		if (self->unk194 == 0) {
			self->releasePicket();
			SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_ESCAPE1,
			                                &self->mPicket->mPosition, 0,
			                                nullptr, 0, 4);
		}
	}

	// Barking heats the chomp back up completely.
	if (spine->getTime() == 280)
		self->mHitPoints = self->getBWParams()->mSLBWHitPointMax.get();

	if (self->getMActor()->curAnmEndsNext(0, nullptr)) {
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		self->showMessage(0xE001B);
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBWJump, TLiveActor)
{
	TBossWanwan* self = (TBossWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		const JGeometry::TVec3<f32>& goal = self->getGoal().getPoint();
		f32 speed                         = self->unk124->unkC;
		JGeometry::TVec3<f32> vel
		    = self->calcVelocityToJumpToY(goal, speed, self->getGravityY());
		self->mVelocity = vel;
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
		self->unk16C = 0;
	}

	if (self->isReachedToGoal()) {
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		return true;
	}

	self->walkToCurPathNode(0.0f, self->mTurnSpeed, 0.0f);
	return false;
}

DEFINE_NERVE(TNerveBWStun, TLiveActor)
{
	TBossWanwan* self = (TBossWanwan*)spine->getBody();

	if (self->mPicket->isTaken() && self->isHeadPulled()) {
		TGraphTracer* tracer = self->unk124;
		TGraphWeb* graph     = tracer->getGraph();
		int ahead            = tracer->mPrevIdx;

		JGeometry::TVec3<f32> toNode;
		tracer->getPrevious().getPoint(&toNode);
		toNode -= self->mPosition;
		if (VECMag(&toNode) < 100.0f) {
			if (ahead == graph->unk10) {
				spine->pushAfterCurrent(&TNerveBWJumpToBath::theNerve());
				return true;
			}

			self->reverseNextGraphNode();
		}
		return false;
	}

	if (spine->getTime() > self->getBWParams()->mSLStunTimer.get()) {
		self->mHitPoints = self->getBWParams()->mSLBWHitPointMax.get();
		spine->pushAfterCurrent(&TNerveBWWakeup::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBWWakeup, TLiveActor)
{
	TBossWanwan* self = (TBossWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(6);
		self->getMActor()->setBtpFromIndex(2);
		self->unk16C = 0;
		self->unk168 = 0.0f;
	}

	if (self->getMActor()->curAnmEndsNext(0, nullptr)) {
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBWJumpToBath, TLiveActor)
{
	TBossWanwan* self = (TBossWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		JGeometry::TVec3<f32> vel = self->calcVelocityToJumpToY(
		    BW_BATH_POS, 10.0f, self->getGravityY());
		self->setGoal(BW_BATH_POS);
		self->mVelocity = vel;
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
		self->unk16C = 0;
	}

	if (spine->getTime() > 120 && self->unk195 == 0
	    && self->mPosition.y < 10.0f + BW_BATH_POS.y) {
		TEffectColumWater* splash
		    = (TEffectColumWater*)gpConductor->makeOneEnemyAppear(
		        self->mPosition, "エフェクト水柱マネージャー", 1);
		if (splash != nullptr) {
			JGeometry::TVec3<f32> scale(5.0f, 5.0f, 5.0f);
			JGeometry::TVec3<f32> pos(self->mPosition.x,
			                          500.0f + self->mPosition.y - 30.0f,
			                          self->mPosition.z);
			splash->generate(pos, scale);
			self->unk195 = 1;
		}
		SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_DIVE, &self->mPosition,
		                                0, nullptr, 0, 4);
	}

	JGeometry::TVec3<f32> diff = BW_BATH_POS;
	diff -= self->mPosition;
	if (diff.squared() < 10000.0f && self->mPosition.y <= BW_BATH_POS.y) {
		self->mPosition = BW_BATH_POS;
		spine->pushAfterCurrent(&TNerveBWDie::theNerve());
		return true;
	}

	self->walkToCurPathNode(0.0f, self->mTurnSpeed, 0.0f);
	return false;
}

DEFINE_NERVE(TNerveBWDie, TLiveActor)
{
	TBossWanwan* self = (TBossWanwan*)spine->getBody();
	MActor* mactor    = self->getMActor();

	if (spine->getTime() == 0) {
		JGeometry::TVec3<f32> zero;
		zero.zero();
		self->mVelocity      = zero;
		self->mPositionDelta = zero;
		self->onLiveFlag(LIVE_FLAG_UNK10);
		self->offLiveFlag(LIVE_FLAG_AIRBORNE);
		gpMarioParticleManager->emit(BWANWAN_JPA_MS_BWAN_DEADYUGE,
		                             &self->mPosition, 0, nullptr);
	}

	// Cool down completely first.
	if (self->getHitPoints() != 0) {
		self->decHitPoints();
		J3DFrameCtrl* ctrl = mactor->getFrameCtrl(0);
		ctrl->setFrame(0.0f);
		ctrl->setRate(0.0f);
		spine->pushAfterCurrent(&TNerveBWDie::theNerve());
		return true;
	}

	if (spine->getTime() == 0) {
		SMSGetMarDirector()->fireStartDemoCamera("bwanwan_down_camera", nullptr,
		                                         -1, 0.0f, true, nullptr, 0,
		                                         nullptr, 0);
		self->takeBath();
		self->changeBck(1);
		mactor->setBtpFromIndex(0);
		mactor->setBrkFromIndex(1);
	}

	if (spine->getTime() > 60 && self->unk18D == 0
	    && gpMarDirector->getGameMode() != 3) {
		gpItemManager->makeShineAppearWithDemo(
		    "シャイン（ボス用）", "ボスシャインカメラ", self->mPosition.x,
		    self->mPosition.y, self->mPosition.z);
		self->unk18D = 1;
	}

	if (mactor->curAnmEndsNext(5, nullptr))
		mactor->getFrameCtrl(5)->setRate(0.0f);

	return false;
}

DEFINE_NERVE(TNerveBWJumpAway, TLiveActor)
{
	TBossWanwan* self = (TBossWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		JGeometry::TVec3<f32> vel = self->calcVelocityToJumpToY(
		    BW_HEAD_START, 40.0f, self->getGravityY());
		self->setGoal(BW_HEAD_START);
		self->mVelocity = vel;
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
		self->unk16C = 0;
	}

	if (self->unk188 != 0) {
		spine->pushAfterCurrent(&TNerveBWFall::theNerve());
		return true;
	}

	if (self->isReachedToGoal()) {
		self->mPosition = BW_HEAD_START;
		self->unk124->reset();
		self->unk124->reset2();
		self->goToShortestNextGraphNode();
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		return true;
	}

	self->walkToCurPathNode(0.0f, self->mTurnSpeed, 0.0f);
	return false;
}

DEFINE_NERVE(TNerveBWShake, TLiveActor)
{
	TBossWanwan* self = (TBossWanwan*)spine->getBody();
	MActor* mactor    = self->getMActor();

	if (spine->getTime() == 0)
		self->changeBck(2);

	if (mactor->curAnmEndsNext(0, nullptr)) {
		self->releasePicket();
		SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_ESCAPE2,
		                                &self->mPicket->mPosition, 0, nullptr,
		                                0, 4);
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBWFall, TLiveActor)
{
	TBossWanwan* self = (TBossWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		const JGeometry::TVec3<f32>& goal = self->mPicket->mPosition;
		JGeometry::TVec3<f32> vel
		    = self->calcVelocityToJumpToY(goal, 5.0f, self->getGravityY());
		self->setGoal(self->mPicket->mPosition);
		self->mVelocity = vel;
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
		self->unk16C = 0;
	}

	if (self->isReachedToGoal()) {
		self->mPosition = self->mPicket->mPosition;
		self->getTracer()->reset();
		self->getTracer()->reset2();
		self->goToShortestNextGraphNode();
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		return true;
	}

	return false;
}
