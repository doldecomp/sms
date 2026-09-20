#include <Enemy/BossWanwan.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MoveBG/ItemManager.hpp>
#include <GC2D/GCConsole2.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Mario.hpp>
#include <Camera/CameraShake.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/Graph.hpp>

// On TSpineBase<T>::getLatestNerve(): retail inlines it in TBossWanwan::perform
// (five sites) and TBWBinder::bind (two), and *calls* it in
// TBWPicket::moveRequest (two). The difference is one inline level, so
// moveRequest goes through the TBossWanwan::getLatestNerve() forwarder in the
// header and the other two read boss->mSpine directly. Spine.hpp itself is
// right as it stands.
//
// TODO: one shared-header gap holds this unit back and it cannot be fixed from
// here:
//   * MsPerpendicFootToLineR in MarioUtil/MathUtil.hpp is 73% against its
//     retail body; TBWBinder::bind is its only caller here.

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

// The three fixed points of the fight, in .bss with a static initialiser:
// the hot spring the boss has to be led into, where the stake starts out and
// where the boss itself starts out.
static JGeometry::TVec3<f32> BW_BATH_POS(-1000.0f, 4.5f, -6217.2f);
static JGeometry::TVec3<f32> BW_PICKET_START(6012.84f, 0.0f, 7323.15f);
static JGeometry::TVec3<f32> BW_HEAD_START(5741.72f, -100.0f, 6311.62f);

static const char* bwanwan_bastable[] = {
	"/scene/bwanwan/bas/bwanwan_bark.bas",
	nullptr,
	"/scene/bwanwan/bas/bwanwan_shake.bas",
	nullptr,
	"/scene/bwanwan/bas/bwanwan_wait.bas",
	"/scene/bwanwan/bas/bwanwan_wait2.bas",
	nullptr,
};

// fabricated: TBWBinder::bind and the graph-wander nerve both keep the
// multiplication by a literal 1.0f and both call TVec3::set<f32> out of line,
// which only happens one inline level below the direction vector, so the
// forward vector was built by a helper taking a length. It is local to the TU
// because the map lists no symbol for it anywhere.
static inline JGeometry::TVec3<f32> MsGetVecFromRotY(f32 rot_y, f32 length)
{
	return JGeometry::TVec3<f32>(length * JMASSin(DEG2SHORTANGLE(rot_y)), 0.0f,
	                             length * JMASCos(DEG2SHORTANGLE(rot_y)));
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

static inline TRope* BWLeashRope(const TBWLeash* p)
{
	TRope* rope = p->mRope;
	return rope;
}

static inline TBWLeash* BWLeashNodeLeash(const TBWLeashNode* p)
{
	TBWLeash* leash = p->mLeash;
	return leash;
}

static inline MActor* BWLeashNodeMActor(const TBWLeashNode* p)
{
	MActor* actor = p->mMActor;
	return actor;
}

static inline TBossWanwan* BWLeashOwner(const TBWLeash* p)
{
	TBossWanwan* owner = p->mOwner;
	return owner;
}

static inline TBWParams* BosswanwanSaveParam2(const TBossWanwan* p)
{
	TBWParams* params = p->getSaveParam2();
	return params;
}

// UNUSED, 0x110 in the map: inlined into TBWLeash's constructor.
TBWLeashNode::TBWLeashNode(TBWLeash* leash, int index, const char* name)
    : THitActor(name)
    , mLeash(leash)
    , mMActor(nullptr)
    , mTemperature(0.0f)
    , mIndex(index)
{
	TBossWanwan* owner = BWLeashOwner(mLeash);
	mMActor = owner->getActorKeeper()->createMActor("bwanwan_chain.bmd", 0);
	mMActor->setBrkFromIndex(2);

	f32 radius = BosswanwanSaveParam2(owner)->mSLChainHitRadius.get();
	f32 height = BosswanwanSaveParam2(owner)->mSLChainHitHeight.get();
	initHitActor(0x0800000C, 1, 0x80000000, 1.2f * radius, 1.2f * height,
	             radius, height);
}

void TBWLeashNode::calcTemperature()
{
	if (mIndex == 0)
		return;

	f32 diff = mLeash->getNode(mIndex - 1)->getTemperature() - mTemperature;
	f32 step;
	if (diff < 0.0f) {
		if (diff < -0.1f)
			step = -0.02f;
		else
			step = -0.005f;
	} else if (diff > 0.1f) {
		step = 0.02f;
	} else {
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
	int index   = mIndex;
	TRope* rope = mLeash->mRope;
	TRopePoint* points = rope->mPoints;

	MtxPtr mtx = mMActor->getModel()->getBaseTRMtx();

	JGeometry::TVec3<f32> pos = points[index].unkC;

	JGeometry::TVec3<f32> zDir;
	if (index < rope->mNumPoints - 1) {
		zDir = points[index + 1].unkC;
		zDir -= pos;
	} else {
		zDir = points[index - 1].unkC;
		zDir -= pos;
		zDir.negate();
	}
	VECNormalize(zDir, zDir);

	JGeometry::TVec3<f32> yDir(0.0f, 1.0f, 0.0f);

	JGeometry::TVec3<f32> xDir;
	xDir.cross(yDir, zDir);
	VECNormalize(xDir, xDir);

	yDir.cross(zDir, xDir);
	VECNormalize(yDir, yDir);

	mtx[0][2] = zDir.x;
	mtx[1][2] = zDir.y;
	mtx[2][2] = zDir.z;

	// Every other link is flipped so that the chain's flat links alternate.
	if (mIndex & 1) {
		mtx[0][0] = xDir.x;
		mtx[1][0] = xDir.y;
		mtx[2][0] = xDir.z;
		mtx[0][1] = yDir.x;
		mtx[1][1] = yDir.y;
		mtx[2][1] = yDir.z;
	} else {
		mtx[0][0] = yDir.x;
		mtx[1][0] = yDir.y;
		mtx[2][0] = yDir.z;
		mtx[0][1] = xDir.x;
		mtx[1][1] = xDir.y;
		mtx[2][1] = xDir.z;
	}

	mtx[0][3] = pos.x;
	mtx[1][3] = 30.0f + pos.y;
	mtx[2][3] = pos.z;

	mPosition = pos;
}

void TBWLeashNode::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		calcTemperature();
		calcMatrix();

		// Only the eight links nearest the head are hot enough to burn.
		if (BWLeashOwner(mLeash)->getHitPoints() != 0 && mIndex < 8) {
			for (int i = 0; i < getColNum(); ++i) {
				THitActor* actor = getCollision(i);
				if (actor->getActorType() == 0x80000001)
					actor->receiveMessage(this, HIT_MESSAGE_UNKA);
			}
		}
	}

	if (cue & CUE_CALC_ANIM) {
		J3DFrameCtrl* ctrl = BWLeashNodeMActor(this)->getFrameCtrl(ANM_TYPE_BRK);
		if (ctrl) {
			int index  = mIndex;
			f32 endFrm = mTemperature * (f32)(ctrl->getEnd() - 1);

			f32 heat;
			if (index < 5) {
				heat = (f32)mIndex / 4.0f
				       + (f32)BWLeashOwner(mLeash)->getHitPoints()
				             / (f32)BWLeashOwner(mLeash)->getSaveParam2()
						   ->mSLBWHitPointMax.get();
			} else {
				u16 num = BWLeashRope(mLeash)->mNumPoints;
				if (index >= (int)(num - 10))
					heat = (f32)(num - index) / 10.0f;
				else
					heat = 1.0f;
			}

			if (heat > 1.0f)
				heat = 1.0f;
			else if (heat < 0.0f)
				heat = 0.0f;

			endFrm *= heat;
			ctrl->setFrame(endFrm);
			ctrl->setRate(0.0f);
		}
	}

	// The last link is hidden inside the stake.
	if (mIndex < (int)(BWLeashRope(mLeash)->mNumPoints - 1))
		BWLeashNodeMActor(this)->perform(cue, graphics);
}

TBWLeash::TBWLeash(TBossWanwan* owner, int node_num, const char* name)
    : JDrama::TViewObj(name)
    , mOwner(owner)
    , mRope(nullptr)
    , mNodes(nullptr)
{
	f32 nodeLen = BosswanwanSaveParam2(mOwner)->mSLLeashNodeLen.get();
	f32 groundRadius = mOwner->getSaveParam2()->mSLChainGroundRadius.get();
	mRope  = new TRope(node_num, mOwner->mPosition, nodeLen, groundRadius, 0.7f,
	                   -2.0f);
	mNodes = new TBWLeashNode*[node_num];

	for (int i = 0; i < node_num; ++i)
		mNodes[i] = new TBWLeashNode(this, i, "鎖部品");

	TIdxGroupObj* group = JDrama::TNameRefGen::search<TIdxGroupObj>(
	    "敵グループ");
	for (int i = 0; i < node_num; ++i) {
		group->getChildren().push_back(mNodes[i]);

		// Only the two links nearest the stake collide with the map, and only
		// the two links nearest the stake are excluded from hitting Mario.
		if (i < node_num - 2)
			mNodes[i]->offHitFlag(HIT_FLAG_NO_COLLISION);
		else
			mNodes[i]->onHitFlag(HIT_FLAG_NO_COLLISION);

		if (i < 2)
			mRope->mPoints[i].unk28 |= 1;
		else
			mRope->mPoints[i].unk28 &= ~1;
	}
}

// UNUSED, 0x3c in the map.
void TBWLeash::invalidateAllCollision()
{
	for (int i = 0; i < mRope->mNumPoints; ++i)
		mNodes[i]->onHitFlag(HIT_FLAG_NO_COLLISION);
}

// UNUSED, 0xc4 in the map: TBWPicket::moveRequest and TBWLeash::perform both
// spell this out instead.
void TBWLeash::pullTail(const JGeometry::TVec3<f32>& where_to)
{
	JGeometry::TVec3<f32> before = mRope->mPoints[0].unkC;
	mRope->constraintTail(where_to);
	before -= mRope->mPoints[0].unkC;
	before.negate();
	mOwner->mPullVelocity = before;
}

void TBWLeash::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		JGeometry::TVec3<f32> head;
		mOwner->getJointTransByIndex(5, &head);
		mRope->moveHead(head);
		mOwner->mIsLeashStretched = 0;

		if (mOwner->mIsPicketPlanted) {
			JGeometry::TVec3<f32> before = mRope->mPoints[0].unkC;
			mRope->constraintTail(mOwner->mPicket->mPosition);
			before -= mRope->mPoints[0].unkC;
			before.negate();
			mOwner->mPullVelocity = before;

			JGeometry::TVec3<f32> toTail = mRope->mPoints[0].unkC;
			JGeometry::TVec3<f32> headPos(mOwner->mPosition);
			headPos.y += 500.0f;
			toTail -= headPos;

			// The chain has run out: drag the boss back towards the stake.
			if (VECMag(toTail) > 650.0f) {
				VECNormalize(toTail, toTail);
				toTail.scale(20.0f);
				toTail.y = 0.0f;
				mOwner->mPosition.add(toTail);
				mOwner->mIsLeashStretched = 1;
			}

			f32 yaw
			    = MsWrap(180.0f + MsGetRotFromZaxisY(toTail), 0.0f, 360.0f);
			f32 turn = MsAngleDiff(yaw, mOwner->mRotation.y);
			if (turn > 0.0f) {
				f32 limit = 1.5f * mOwner->getTurnSpeed();
				turn      = turn > limit ? limit : turn;
			} else {
				f32 limit = 1.5f * -mOwner->getTurnSpeed();
				turn      = turn > limit ? turn : limit;
			}
			mOwner->mRotation.y
			    = MsWrap(mOwner->mRotation.y + turn, 0.0f, 360.0f);
		}
	}

	if (cue & CUE_CALC_ANIM) {
		// Keep every rope point at least 500 units away from the boss's neck
		// so the chain never draws through the body.
		JGeometry::TVec3<f32> neck;
		mOwner->getJointTransByIndex(1, &neck);

		for (int i = 0; i < 15; ++i) {
			TRopePoint* point = &mRope->mPoints[i];

			JGeometry::TVec3<f32> toPoint = point->unkC;
			toPoint -= neck;

			f32 dist = VECMag(toPoint);
			if (dist < 500.0f) {
				f32 push = 500.0f / dist;
				toPoint.scale(push);
				toPoint.add(neck);
				point->unk18.set(0.0f, 0.0f, 0.0f);
				point->unkC.set(toPoint.x, toPoint.y, toPoint.z);
				point->unk0 = point->unkC;
			}
		}
	}

	for (int i = 0; i < mRope->mNumPoints; ++i)
		mNodes[i]->testPerform(cue, graphics);
}

// UNUSED, 0x1b4 in the map: inlined into TBossWanwan::init.
TBWPicket::TBWPicket(TBossWanwan* owner, const char* name)
    : TTakeActor(name)
    , mOwner(owner)
    , mMActor(nullptr)
{
	MTXIdentity(mTakingMtx);

	initHitActor(0x0800000D, 1, 0x80000000,
	             mOwner->getSaveParam2()->mSLPicketRadius.get(),
	             mOwner->getSaveParam2()->mSLPicketHeight.get(),
	             mOwner->getSaveParam2()->mSLPicketRadius.get(),
	             mOwner->getSaveParam2()->mSLPicketHeight.get());

	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);

	offHitFlag(HIT_FLAG_NO_COLLISION);
	mMActor = mOwner->getActorKeeper()->createMActor("bwanwan_picket.bmd", 0);
}

static inline TBWPicket* BosswanwanPicket(const TBossWanwan* p)
{
	TBWPicket* picket = p->mPicket;
	return picket;
}

static inline MSound* BWPicketSound()
{
	MSound* sound = gpMSound;
	return sound;
}

static inline TBossWanwan* BWPicketOwnerRaw(const TBWPicket* p)
{
	return p->mOwner;
}

static inline TBossWanwan* BWPicketOwner(const TBWPicket* p)
{
	TBossWanwan* owner = BWPicketOwnerRaw(p);
	return owner;
}

BOOL TBWPicket::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->getActorType() == 0x80000001) {
		if (message == HIT_MESSAGE_HIP_DROP) {
			TBossWanwan* owner      = BWPicketOwner(this);
			owner->mIsPicketPlanted = 1;
			owner->mPulledTimer     = 0;
			BWPicketSound()->startSoundActor(MSD_SE_BS_WANWAN_LOCK, &mPosition, 0,
			                          nullptr, 0, 4);
			return TRUE;
		}

		if (message == HIT_MESSAGE_TAKE) {
			TBossWanwan* owner = BWPicketOwner(this);
			owner->releasePicket();
			mHolder = (TTakeActor*)sender;
			return TRUE;
		}

		if (message == HIT_MESSAGE_THROWN || message == HIT_MESSAGE_UNK8) {
			mHolder = nullptr;
			return TRUE;
		}
	}

	return FALSE;
}

// Binding level worth +16 of low region, landing TBWPicket::moveRequest's
// frame at 0x68 (batch 124).
static inline const TNerveBase<TLiveActor>*
BosswanwanGetLatestNerve(const TBossWanwan* p)
{
	const TNerveBase<TLiveActor>* latestNerve = p->getLatestNerve();
	return latestNerve;
}

BOOL TBWPicket::moveRequest(const JGeometry::TVec3<f32>& where_to)
{
	if (BosswanwanGetLatestNerve(mOwner) == &TNerveBWJumpToBath::theNerve()
	    || BosswanwanGetLatestNerve(mOwner) == &TNerveBWDie::theNerve())
		return FALSE;

	if (mOwner->getHitPoints() != 0)
		return FALSE;

	mOwner->getLeash()->pullTail(where_to);
	return TRUE;
}

MtxPtr TBWPicket::getTakingMtx() { return mTakingMtx; }

void TBWPicket::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		TRope* rope = mOwner->getLeash()->getRope();
		mPosition   = rope->mPoints[rope->mNumPoints - 1].unkC;

		ensureTakeSituation();

		if (mHolder) {
			TRope* rope2 = mOwner->getLeash()->getRope();

			JGeometry::TVec3<f32> zDir;
			zDir.set(mHolder->mPosition);
			zDir -= rope2->mPoints[0].unkC;

			JGeometry::TVec3<f32> yDir(0.0f, 1.0f, 0.0f);

			JGeometry::TVec3<f32> xDir;
			xDir.cross(yDir, zDir);
			VECNormalize(xDir, xDir);

			yDir.cross(zDir, xDir);
			VECNormalize(yDir, yDir);

			zDir.cross(xDir, yDir);
			VECNormalize(zDir, zDir);

			mTakingMtx.ref(0, 0) = xDir.x;
			mTakingMtx.ref(1, 0) = xDir.y;
			mTakingMtx.ref(2, 0) = xDir.z;
			mTakingMtx.ref(0, 1) = yDir.x;
			mTakingMtx.ref(1, 1) = yDir.y;
			mTakingMtx.ref(2, 1) = yDir.z;
			mTakingMtx.ref(0, 2) = zDir.x;
			mTakingMtx.ref(1, 2) = zDir.y;
			mTakingMtx.ref(2, 2) = zDir.z;
			mTakingMtx.ref(0, 3) = mPosition.x;
			mTakingMtx.ref(1, 3) = mPosition.y;
			mTakingMtx.ref(2, 3) = mPosition.z;

			mHolder->moveRequest(mPosition);
		}
	}

	if (cue & CUE_CALC_ANIM) {
		MtxPtr mtx = mMActor->getModel()->getBaseTRMtx();
		MTXIdentity(mtx);

		// Aim the stake along the last three rope points so it lies flat when
		// it has been pulled out.
		TRope* rope = mOwner->getLeash()->getRope();
		JGeometry::TVec3<f32> zDir;
		zDir.set(mPosition);
		zDir -= rope->mPoints[rope->mNumPoints - 3].unkC;
		if (zDir.isZero())
			zDir.set(0.0f, 0.0f, 1.0f);

		JGeometry::TVec3<f32> yDir(0.0f, 1.0f, 0.0f);

		JGeometry::TVec3<f32> xDir;
		xDir.cross(yDir, zDir);
		VECNormalize(xDir, xDir);

		yDir.cross(zDir, xDir);
		VECNormalize(yDir, yDir);

		zDir.cross(xDir, yDir);
		VECNormalize(zDir, zDir);

		mtx[0][0] = xDir.x;
		mtx[1][0] = xDir.y;
		mtx[2][0] = xDir.z;
		mtx[0][1] = yDir.x;
		mtx[1][1] = yDir.y;
		mtx[2][1] = yDir.z;
		mtx[0][2] = zDir.x;
		mtx[1][2] = zDir.y;
		mtx[2][2] = zDir.z;
		mtx[0][3] = mPosition.x;
		mtx[1][3] = mPosition.y;
		mtx[2][3] = mPosition.z;

		// Planted, the stake stands proud of the ground.
		if (mOwner->mIsPicketPlanted == 0)
			mtx[1][3] += 70.0f;

		// Held, the stake trails a little closer to Mario's hand.
		if (isTaken()) {
			mtx[0][3] -= 50.0f * zDir.x;
			mtx[2][3] -= 50.0f * zDir.z;
		} else {
			mtx[0][3] -= 60.0f * zDir.x;
			mtx[2][3] -= 60.0f * zDir.z;
		}
	}

	mMActor->perform(cue, graphics);
	THitActor::perform(cue, graphics);
}

// UNUSED, 0x68 in the map: inlined into TBossWanwan::init, twice.
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
			if (mOwner->getHitPoints() != 0
			    && actor->mActorType == 0x80000001)
				actor->receiveMessage(mOwner, HIT_MESSAGE_UNKA);
		}
	}

	THitActor::perform(cue, graphics);
}

// UNUSED, 0x3c in the map: inlined into TBossWanwan's constructor.
TBWBinder::TBWBinder() { }

void TBWBinder::bind(TLiveActor* actor)
{
	TBossWanwan* boss = (TBossWanwan*)actor;

	JGeometry::TVec3<f32> velocity = actor->mLinearVelocity;
	JGeometry::TVec3<f32> next(actor->mPosition);
	next.add(velocity);

	if (actor->isAirborne()) {
		JGeometry::TVec3<f32> speed = actor->mVelocity;
		next.add(speed);
		speed.y -= actor->getGravityY();
		if (speed.y < TLiveActor::mVelocityMinY)
			speed.y = TLiveActor::mVelocityMinY;
		actor->mVelocity = speed;
	}

	if (boss->getLatestNerve() == &TNerveBWJumpToBath::theNerve()
	    || boss->getLatestNerve() == &TNerveBWDie::theNerve()) {
		actor->mLinearVelocity = next - actor->mPosition;
		return;
	}

	if (actor->isAirborne()) {
		f32 nextY = next.y;
		f32 nextZ = next.z;

		const TBGCheckData* ground;
		f32 height = gpMap->checkGround(next.x, nextY + boss->getHeadHeight(),
		                                nextZ, &ground)
		             + 1.0f;

		// When falling, also probe from where the boss is now, so it cannot
		// drop through a ledge it is still standing on.
		if (actor->mPosition.y > nextY && !ground->isEnemyThrough()) {
			const TBGCheckData* ground2;
			f32 height2 = gpMap->checkGround(next.x,
			                                 actor->mPosition.y
			                                     + boss->getHeadHeight(),
			                                 nextZ, &ground2)
			              + 1.0f;
			if (height2 > height) {
				height = height2;
				ground = ground2;
			}
		}

		if (next.y <= height && !ground->checkFlag(BG_CHECK_FLAG_ILLEGAL)
		    && !ground->isEnemyThrough()) {
			next.y = height;
			JGeometry::TVec3<f32> stopped(0.0f, 0.0f, 0.0f);
			actor->mVelocity = stopped;
			actor->offLiveFlag(LIVE_FLAG_AIRBORNE);
			actor->offLiveFlag(LIVE_FLAG_UNK8000);
		} else {
			actor->onLiveFlag(LIVE_FLAG_AIRBORNE);
		}

		actor->mGroundHeight = height;
		actor->mGroundPlane  = ground;
	}

	velocity = next - actor->mPosition;

	// Slide the boss along the graph link it is walking on, at three units a
	// frame minimum so it never stalls.
	if (!actor->isAirborne()) {
		TGraphTracer* tracer   = ((TSpineEnemy*)actor)->getTracer();
		const TGraphWeb* graph = tracer->getGraph();
		if (graph) {
			int curr = tracer->getCurGraphIndex();
			int prev = tracer->getPrevIndex();
			if (curr >= 0 && prev >= 0 && curr != prev) {
				const TGraphNode* nodes = graph->unk0;

				JGeometry::TVec3<f32> link;
				JGeometry::TVec3<f32> back;
				nodes[curr].getPoint(link);
				nodes[prev].getPoint(back);

				link.x -= back.x;
				link.y -= back.y;
				link.z -= back.z;
				VECNormalize(link, link);

				f32 along = 0.0f;
				f32 lsq   = link.squared();
				if (lsq != 0.0f)
					along = velocity.dot(link) / lsq;

				f32 step = along;
				if (along < 0.0f) {
					if (along > -3.0f)
						step = -3.0f;
				} else if (along > 0.0f && along < 3.0f) {
					step = 3.0f;
				}

				velocity = link;
				velocity.scale(step);
			}
		}
	}

	// Roll the body by the distance travelled along its facing direction.
	if (!actor->isAirborne()) {
		JGeometry::TVec3<f32> moveXZ(velocity);
		f32 dist = VECMag(moveXZ);
		if (dist != 0.0f) {
			f32 roll = 360.0f * (dist / 3141.5928f);

			JGeometry::TVec3<f32> facing
			    = MsGetVecFromRotY(actor->mRotation.y, 1.0f);
			JGeometry::TVec3<f32> dir = facing;
			if (dir.dot(velocity) < 0.0f)
				roll = -roll;

			roll *= 2.0f;
			if (boss->mIsRolling) {
				boss->mRollAngle = MsAngleWrap(boss->mRollAngle + roll);
			} else if (boss->mRollAngle != 0.0f) {
				f32 wrapped = boss->mRollAngle + roll;
				if (wrapped > 360.0f)
					wrapped = 0.0f;
				boss->mRollAngle = wrapped;
			}
		}
	}

	// Keep the boss within 860 units of the chain's tail.
	if (boss->mIsPicketPlanted) {
		TBWLeash* leash = boss->getLeash();

		JGeometry::TVec3<f32> here(actor->mPosition);
		JGeometry::TVec3<f32> there(here);
		JGeometry::TVec3<f32> origin(here);
		there.add(velocity);

		JGeometry::TVec3<f32> tail = leash->getRope()->mPoints[3].unkC;
		there.sub(tail);

		if (VECMag(there) > 860.0f) {
			VECNormalize(there, there);
			there.scale(860.0f);
			there.add(tail);
			there.sub(origin);
			velocity = there;
		}
	}

	// Pull the boss back onto the link it should be standing on.
	if (!actor->isAirborne()) {
		TGraphTracer* tracer      = ((TSpineEnemy*)actor)->getTracer();
		const TGraphNode* nodes   = tracer->getGraph()->unk0;
		const TGraphNode* backOne = &nodes[tracer->getPrevIndex()];

		JGeometry::TVec3<f32> ahead;
		JGeometry::TVec3<f32> behind;
		nodes[tracer->getCurGraphIndex()].getPoint(ahead);
		backOne->getPoint(behind);

		JGeometry::TVec3<f32> foot
		    = MsPerpendicFootToLineR(behind, ahead, actor->mPosition);

		// The link vector the original computed here and never used.
		ahead.x -= behind.x;
		ahead.y -= behind.y;
		ahead.z -= behind.z;

		JGeometry::TVec3<f32> toFoot(foot);
		toFoot.sub(actor->mPosition);

		f32 pull = VECMag(toFoot);
		if (pull > 10.0f)
			pull = 10.0f;

		if (pull < 0.0001f) {
			toFoot.set(0.0f, 0.0f, 0.0f);
		} else {
			VECNormalize(toFoot, toFoot);
			toFoot.scale(pull);
		}

		velocity.add(toFoot);
	}

	actor->mLinearVelocity.set(velocity.x, velocity.y, velocity.z);
}

// UNUSED, 0x98 in the map: inlined into TBossWanwan::init.
TBossWanwanMtxCalc::TBossWanwanMtxCalc(TBossWanwan* owner)
    : M3UMtxCalcSIAnmBlendQuat(false)
    , mOwner(owner)
{
	mMotionBlendRatio = 0.0f;
}

// UNUSED, 0x50 in the map: inlined into TBossWanwan::changeBck.
void TBossWanwanMtxCalc::joinAnm(int index)
{
	M3UMtxCalcSIAnmBlendQuat::joinAnm(
	    mOwner->getActorKeeper()->getMActorAnmData()->getUnk2C()->getAnmPtr(
	        index));
}

void TBossWanwanMtxCalc::calc(u16 joint)
{
	// While airborne the root joint's translation is thrown away so the boss
	// arcs from its own velocity instead of the animation's.
	if (joint == 0 && mOwner->isAirborne()) {
		j3dSys.setCurrentMtxCalc(this);

		J3DTransformInfo info;
		if (mNewAnm) {
			mNewAnm->getTransform(joint, &info);
		} else {
			info = j3dSys.getModel()
			           ->getModelData()
			           ->getJointNodePointer(joint)
			           ->getTransformInfo();
		}

		info.mTranslate.x = 0.0f;
		info.mTranslate.y = 0.0f;
		info.mTranslate.z = 0.0f;

		calcTransform(joint, info);
		return;
	}

	M3UMtxCalcSIAnmBlendQuat::calc(joint);

	if (joint != 1)
		return;

	// Roll the body around its own forward axis.
	JGeometry::SMatrix34C<f32> scratch;
	MtxPtr roll = scratch;
	scratch.zero();

	f32 s = JMASSin(DEG2SHORTANGLE(mOwner->mRollAngle));
	f32 c = JMASCos(DEG2SHORTANGLE(mOwner->mRollAngle));

	scratch.set(1.0f, 0.0f, 0.0f, 0.0f, //
	            0.0f, c, -s, 0.0f,      //
	            0.0f, s, c, 0.0f);

	MtxPtr anm = mOwner->getModel()->getAnmMtx(joint);
	MTXConcat(anm, roll, anm);
	MTXCopy(anm, J3DSys::mCurrentMtx);
}

TBossWanwan::TBossWanwan(const char* name)
    : TSpineEnemy(name)
    , mMtxCalc(nullptr)
    , mLeash(nullptr)
    , mPicket(nullptr)
    , mRollAngle(0.0f)
    , mIsRolling(0)
    , mIsPicketPlanted(0)
    , mCoolDownTimer(0)
    , mPulledTimer(0)
    , mIsLeashStretched(0)
    , mIsInBath(0)
    , mShineAppeared(0)
    , mSparkRequest(0)
    , mIsPicketFixed(1)
    , mSplashDone(0)
    , mBalloonsShown(0)
    , mIdleTimer(0)
    , mWasSprayed(0)
    , mJumpSmokePos(0.0f, 0.0f, 0.0f)
    , unk1B0(0)
    , unk1B4(0)
{
	mBinder = new TBWBinder();
}

void TBossWanwan::init(TLiveManager* live_manager)
{
	mManager = live_manager;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 0x11);
	mMActor       = mMActorKeeper->createMActor("bwanwan_body.bmd", 0);

	TGraphWeb* graph = gpConductor->getGraphByName("bwanwan");
	graph->initGoalIndex(BW_BATH_POS);
	unk124->setGraph(graph);

	mSpine->initWith(&TNerveBWGraphWander::theNerve());

	mMarchSpeed = getSaveParam2()->mSLMarchSpeed.get();
	mTurnSpeed  = getSaveParam2()->mSLTurnSpeed.get();

	mPosition = BW_HEAD_START;
	reset();

	mLeash  = new TBWLeash(this, 15, "ボスワンワン鎖");
	mPicket = new TBWPicket(this, "ボスワンワンつかみ");
	mPicket->mPosition = BW_PICKET_START;

	mIsPicketPlanted = 1;
	mPulledTimer     = 0;

	goToRandomNextGraphNode();
	initHitActor(0x0800000B, 1, 0x80000000, 0.0f, 0.0f, 0.0f, 0.0f);

	mHits[0] = new TBWHit(this, 3, "ボスワンワンヒット");
	mHits[0]->initHitActor(0x0800000B, 3, 0xA0000000, 500.0f, 500.0f, 450.0f,
	                       500.0f);
	mHits[1] = new TBWHit(this, -1, "ボスワンワンヒット");
	mHits[1]->initHitActor(0x0800000B, 3, 0xA0000000, 300.0f, 500.0f, 270.0f,
	                       500.0f);

	TIdxGroupObj* group = JDrama::TNameRefGen::search<TIdxGroupObj>(
	    "敵グループ");
	for (int i = 0; i < 2; ++i) {
		group->getChildren().push_back(mHits[i]);
		mHits[i]->offHitFlag(HIT_FLAG_NO_COLLISION);
	}

	initAnmSound();
	mScaledBodyRadius = 500.0f;

	mMtxCalc = new TBossWanwanMtxCalc(this);
	// Retail sets both fields under one null check, which setCalcForBck's
	// own guard cannot share with a following statement, so the guard is
	// spelled out here.
	J3DMtxCalc* calc  = mMtxCalc;
	MActorAnmBck* bck = mMActor->getAnmBck();
	if (bck != nullptr) {
		bck->setCalc(calc);
		bck->unk2A = 3;
	}
	mMActor->calc();
	offLiveFlag(LIVE_FLAG_UNK100);

	changeBck(BWANWAN_BCK_WAIT);
	mMActor->setBrkFromIndex(0);
	mMActor->setLightType(1);

	J3DModel* model = mMActor->getModel();
	if (!model->getSkinDeform()) {
		J3DSkinDeform* skinDeform = new J3DSkinDeform;
		model->setSkinDeform(skinDeform, J3D_DEFORM_ATTACH_FLAG_UNK_1);
	}

	mHitPoints = getSaveParam2()->mSLBWHitPointMax.get();
	mPullVelocity.set(0.0f, 0.0f, 0.0f);

	mMapCollisionManager = new TMapCollisionManager(1, "/scene/bwanwan", this);
	mMapCollisionManager->init("bwanwan_ofuro_col.col", 2, nullptr);

	mMapCollisionManager->setUpUnk8TRS(mPosition, mRotation, mScaling);
	if (mMapCollisionManager->getUnk8())
		mMapCollisionManager->getUnk8()->remove();
}

void TBossWanwan::shakeCamera(int mode)
{
	if (!SMS_IsMarioTouchGround4cm())
		return;

	f32 dist = MsSqrtf(getDistToMarioSquared());

	f32 hotRange  = getSaveParam2()->mSLShakeLengthMax.get();
	f32 coolRange = getSaveParam2()->mSLShakeLengthMaxHP0.get();

	f32 heat;
	if (getMActor()->checkCurBckFromIndex(BWANWAN_BCK_BARK))
		heat = 1.0f;
	else
		heat = (f32)getHitPoints()
		       / (f32)getSaveParam2()->mSLBWHitPointMax.get();

	f32 range = hotRange * heat + coolRange * (1.0f - heat);
	f32 power = range - dist;
	if (power < 0.0f)
		return;

	power /= range;
	if (power > 1.0f)
		power = 1.0f;

	gpCameraShake->startShake((EnumCamShakeMode)mode, power * heat);
	SMSRumbleMgr->start(8, &mPosition);
}

BOOL TBossWanwan::receiveMessage(THitActor* sender, u32 message)
{
	int type = sender->getActorType();

	if ((u32)type == 0x80000001)
		return FALSE;

	if ((u32)type == 0x01000001) {
		if (mIsInBath)
			return TRUE;

		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT,
		                             &sender->mPosition, 0, nullptr);

		u8 hp = getHitPoints();
		if (hp == 0) {
			BWPicketSound()->startSoundActor(MSD_SE_BS_WANWAN_COOL_MORE, &mPosition, 0,
			                          nullptr, 0, 4);
		} else if (hp == 1) {
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    BWANWAN_JPA_MS_BWAN_DOWNYUGE, getModel()->getAnmMtx(1), 0,
			    nullptr);
			BWPicketSound()->startSoundActor(MSD_SE_BS_WANWAN_TO_COOL, &mPosition, 0,
			                          nullptr, 0, 4);
		} else {
			BWPicketSound()->startSoundActor(MSD_SE_BS_WANWAN_COOL, &mPosition, 0,
			                          nullptr, 0, 4);
		}

		decHitPoints();
		mSparkRequest += 1;
		return TRUE;
	}

	if (sender->isActorType(0x4000005A)) {
		sender->receiveMessage(this, HIT_MESSAGE_HIP_DROP);
		mHitPoints    = 0;
		mSparkRequest += 1;
		if (mWasSprayed == 0)
			mWasSprayed += 1;
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    BWANWAN_JPA_MS_BWAN_DOWNYUGE, getModel()->getAnmMtx(1), 0, nullptr);
		BWPicketSound()->startSoundActor(MSD_SE_BS_WANWAN_TO_COOL, &mPosition, 0,
		                          nullptr, 0, 4);
	}

	return TSpineEnemy::receiveMessage(sender, message);
}

// UNUSED, 0x184 in the map: TNerveBWDie::execute spells this out, because
// MWCC will not expand a body this big from there.
// TODO: this standalone copy is 0x27c because at depth 1 it expands both
// changeBck and TMapCollisionBase::setMtx; retail's 0x184 means one of the two
// stayed a call in the dead copy. That is also why the weak setMtx the map
// lists for this TU is missing from our object -- the Die nerve inlines it at
// every spelling tried (setUpUnk8TRS, setUpMtx, setMtx + setUp).
void TBossWanwan::takeBath()
{
	gpMarDirector->fireStartDemoCamera("bwanwan_down_camera", nullptr, -1, 0.0f,
	                                   true, nullptr, 0, nullptr, 0);
	mIsRolling = 0;
	mRollAngle = 0.0f;
	mIsInBath  = true;
	mPosition  = BW_BATH_POS;

	JGeometry::TVec3<f32> center(mPosition);
	center.y += 500.0f;

	JGeometry::TVec3<f32> scale(mScaling);
	scale.scale(1.1f);

	TMapCollisionManager* collision = mMapCollisionManager;
	Mtx mtx;
	MsMtxSetTRS(mtx, center, mRotation, scale);
	collision->getUnk8()->setUpMtx(mtx);

	mHits[0]->onHitFlag(HIT_FLAG_NO_COLLISION);
	mHits[1]->onHitFlag(HIT_FLAG_NO_COLLISION);
	TBWLeash* leash = mLeash;
	for (int i = 0; i < leash->getRope()->mNumPoints; ++i)
		leash->getNode(i)->onHitFlag(HIT_FLAG_NO_COLLISION);
	mPicket->onHitFlag(HIT_FLAG_NO_COLLISION);

	changeBck(BWANWAN_BCK_DOWN);
	mMActor->setBtpFromIndex(0);
	mMActor->setBrkFromIndex(1);
}

// UNUSED, 0x84 in the map: the Bark and Shake nerves and TBWPicket's take
// handler all spell this out.
void TBossWanwan::releasePicket()
{
	if (mIsPicketPlanted) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emit(
		    BWANWAN_JPA_MS_BWAN_JUMP_SMOKE, &mPicket->mPosition, 0, nullptr);
		// TODO: setGlobalScale(const TVec3&) would bind a stack temporary the
		// retail code does not have; JPAEmitter.hpp wants three-float
		// setGlobalDynamicsScale/setGlobalParticleScale overloads.
		if (emitter) {
			emitter->mGlobalDynamicsScale.set(0.3f, 0.5f, 0.3f);
			emitter->mGlobalParticleScale.x = 0.3f;
			emitter->mGlobalParticleScale.y = 0.5f;
			emitter->mGlobalParticleScale.z = 0.3f;
		}
	}

	mIsPicketFixed   = 0;
	mIsPicketPlanted = 0;
}

static inline const char* BosswanwanBasName(int index)
{
	return bwanwan_bastable[index];
}

// UNUSED, 0x144 in the map: every nerve that starts an animation spells this
// out.
void TBossWanwan::changeBck(int index)
{
	mMtxCalc->joinAnm(index);
	mMActor->setFrameCtrlForBck(index);
	mMotionBlendStep
	    = 10.0f / (f32)mMActor->getFrameCtrl(ANM_TYPE_BCK)->getEnd();
	// TODO: retail materialises the int-to-float 0x4330 constant before the
	// bwanwan_bastable address and we do it the other way round; the same
	// one-instruction flip is left in BWWakeup, BWShake and BWBark.
	setAnmSound(BosswanwanBasName(index));
}

// UNUSED, 0x4 in the map: an empty function. The gold BRK the boss would have
// flashed when it is about to be defeated was cut.
void TBossWanwan::startGoldBrk() { }

// UNUSED, 0x18 in the map.
bool TBossWanwan::isBurning() { return getHitPoints() != 0; }

// UNUSED, 0x34 in the map.
// TODO: guessed from the size and the name; the chain's hot links are the
// eight nearest the head (TBWLeashNode::perform), so the tail burns while the
// boss has hit points and the stake is still in the ground.
bool TBossWanwan::isTailBurning()
{
	if (getHitPoints() == 0)
		return false;
	return mLeash->getNode(0)->getTemperature() > 0.0f;
}

// UNUSED, 0x8 in the map, so two instructions: the boss always knows where
// Mario is.
// TODO: mSLSearchLength and mSLSearchAngle are the parameters a real
// TSpineEnemy::isInSight call would use and nothing reads them, but a call
// like that cannot fit in eight bytes, so this must be a stub.
bool TBossWanwan::isMarioInSight() { return true; }

// UNUSED, 0x70 in the map: TBossWanwan::control, TNerveBWStun::execute and
// TNerveBWGraphWander::execute all spell this out.
// TODO: 0xa0 here. All three call sites reproduce the two materialised bools
// exactly, so the shape is right and the standalone copy is carrying twelve
// instructions the original did not.
BOOL TBossWanwan::isHeadPulled()
{
	if (mPicket->isTaken()) {
		if (mPullVelocity.squared() < getSaveParam2()->mSLPullLimit.get())
			return FALSE;
		return TRUE;
	}
	return FALSE;
}

void TBossWanwan::calcRootMatrix()
{
	getModel()->setBaseScale(mScaling);
	MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x,
	               500.0f + mPosition.y, mPosition.z, mRotation.x, mRotation.y,
	               mRotation.z);
}

// UNUSED, 0xcc in the map: TNerveBWStun::execute and
// TNerveBWGraphWander::execute spell this out.
void TBossWanwan::reverseNextGraphNode()
{
	TGraphTracer* tracer = getTracer();
	int prev             = tracer->getPrevIndex();

	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario.sub(mPosition);

	getTracer()->mPrevIdx = tracer->getGraph()->getAimToDirNextIndex(
	    prev, tracer->getCurGraphIndex(), toMario, mPosition, -1);
	getTracer()->mCurrIdx = prev;

	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
}

// UNUSED, 0x150 in the map: TNerveBWGraphWander::execute spells this out.
// TODO: 0x160 here. The balloon guard is part of it -- without it the body is
// 0x100, a good 0x50 short -- but four instructions are still unaccounted for.
void TBossWanwan::rollNextGraphNode()
{
	TGraphTracer* tracer   = getTracer();
	int curr               = tracer->getCurGraphIndex();
	int prev               = tracer->getPrevIndex();
	const TGraphWeb* graph = tracer->getGraph();

	// Standing at a fork long enough into the fight earns the hint about
	// leading the boss to the hot spring.
	if (curr >= 0
	    && graph->getGraphNode(curr).getRailNode()->mConnectionNum >= 2
	    && gpMarDirector->unk58 >= 14400)
		showMessage(BALLOON_MSG_BWANWAN_LEAD_TO_HOT);

	JGeometry::TVec3<f32> facing = MsGetVecFromRotY(mRotation.y, 1.0f);

	getTracer()->moveTo(
	    graph->getEscapeDirLimited(prev, curr, facing, mPosition, 100.0f, -1));

	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
}

static inline const TPathNode& BosswanwanGoalNode(const TBossWanwan* p)
{
	const TPathNode& node = p->getUnkF4();
	return node;
}

void TBossWanwan::slideToCurPathNode(f32 march_speed, f32 turn_speed)
{
	JGeometry::TVec3<f32> toGoal = BosswanwanGoalNode(this).getPoint();
	toGoal -= mPosition;

	f32 dist = VECMag(toGoal);

	f32 yaw  = MsWrap(MsGetRotFromZaxisY(toGoal), 0.0f, 360.0f);
	f32 diff = MsAngleDiff(yaw, getRotation().y);

	f32 turn;
	if (diff > 0.0f) {
		if (diff > turn_speed)
			diff = turn_speed;
		turn = diff;
	} else {
		diff = diff > -turn_speed ? diff : -turn_speed;
		turn = diff;
	}
	// TODO: retail keeps the clamp in diff's own FPR and copies once at the
	// merge (fmr f1, f30; fmr f2, f1); ours materialises turn early and pays
	// one extra fmr. Dropping turn costs the merge copy instead.
	mRotation.y = MsWrap(mRotation.y + turn, 0.0f, 360.0f);

	JGeometry::TVec3<f32> velocity = mLinearVelocity;
	if (dist > 0.0f)
		toGoal.scale(march_speed / dist);
	velocity.add(toGoal);
	mLinearVelocity = velocity;
}

void TBossWanwan::control()
{
	TLiveActor::control();

	if (mIsPicketPlanted || isHeadPulled()) {
		mLinearVelocity.add(mPullVelocity);

		JGeometry::TVec3<f32> toTail(mPosition);
		toTail -= mLeash->getRope()->mPoints[3].unkC;

		f32 yaw  = MsWrap(MsGetRotFromZaxisY(toTail), 0.0f, 360.0f);
		f32 turn = MsAngleDiff(yaw, mRotation.y);
		if (turn > 0.0f) {
			f32 limit = 4.0f * getTurnSpeed();
			turn      = turn > limit ? limit : turn;
		} else {
			f32 limit = 4.0f * -getTurnSpeed();
			turn      = turn > limit ? turn : limit;
		}
		mRotation.y = MsWrap(mRotation.y + turn, 0.0f, 360.0f);
	}

	mPullVelocity.set(0.0f, 0.0f, 0.0f);
	updateSquareToMario();
}

static inline MSound* BosswanwanSound()
{
	MSound* sound = gpMSound;
	return sound;
}

static inline TMarioParticleManager* BosswanwanParticles()
{
	TMarioParticleManager* manager = gpMarioParticleManager;
	return manager;
}

void TBossWanwan::emitEffects()
{
	int stomp = 0;
	if (mMActor->checkCurBckFromIndex(BWANWAN_BCK_WAIT)
	    || mMActor->checkCurBckFromIndex(BWANWAN_BCK_WAIT2)) {
		if (mMActor->checkBckPass(8.0f))
			stomp = 1;
	} else if (mMActor->checkCurBckFromIndex(BWANWAN_BCK_SHAKE)
	           && mMActor->checkBckPass(38.0f)) {
		stomp = 1;
	}

	if (stomp) {
		BosswanwanParticles()->emit(BWANWAN_JPA_MS_BWAN_JUMP_ROCK, &mPosition,
		                             0, nullptr);
		gpMarioParticleManager->emit(BWANWAN_JPA_MS_BWAN_JUMP_SMOKE,
		                             &mPosition, 0, nullptr);
		if (getHitPoints() == 0) {
			BosswanwanSound()->startSoundActor(MSD_SE_M_BOSW_CHAIN_CAL_F,
			                          mLeash->getRope()->mPoints[0].unkC, 0,
			                          nullptr, 0, 4);
			BosswanwanSound()->startSoundActor(MSD_SE_M_BOSW_CHAIN_CAL_R,
			                          &mPicket->mPosition, 0, nullptr, 0, 4);
		} else {
			BosswanwanSound()->startSoundActor(MSD_SE_M_BOSW_CHAIN_ANG_F,
			                          mLeash->getRope()->mPoints[0].unkC, 0,
			                          nullptr, 0, 4);
			BosswanwanSound()->startSoundActor(MSD_SE_M_BOSW_CHAIN_ANG_R,
			                          &mPicket->mPosition, 0, nullptr, 0, 4);
		}
	}

	int spark = 0;
	if (mMActor->checkCurBckFromIndex(BWANWAN_BCK_BARK)) {
		if (mMActor->checkBckPass(72.0f))
			spark = 1;
	} else if (mMActor->checkCurBckFromIndex(BWANWAN_BCK_WAIT)
	           || mMActor->checkCurBckFromIndex(BWANWAN_BCK_WAIT2)) {
		if (mMActor->checkBckPass(6.0f) || mMActor->checkBckPass(12.0f))
			spark = 1;
	} else if (mMActor->checkCurBckFromIndex(BWANWAN_BCK_SHAKE)
	           && mMActor->checkBckPass(4.0f)) {
		spark = 1;
	}

	if (spark)
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    BWANWAN_JPA_MS_BWAN_HIBANA, getModel()->getAnmMtx(1), 0, this);

	if ((mMActor->checkCurBckFromIndex(BWANWAN_BCK_WAIT)
	     || mMActor->checkCurBckFromIndex(BWANWAN_BCK_WAIT2))
	    && mMActor->checkBckPass(10.0f))
		shakeCamera(CAM_SHAKE_MODE_BOWA_LAND1);

	if (mMActor->checkCurBckFromIndex(BWANWAN_BCK_BARK)) {
		J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(ANM_TYPE_BCK);
		if (ctrl->checkPass(60.0f) || ctrl->checkPass(127.0f)) {
			shakeCamera(CAM_SHAKE_MODE_BOWA_LAND1);
			gpMarioParticleManager->emit(BWANWAN_JPA_MS_BWAN_JUMP_ROCK,
			                             &mPosition, 0, nullptr);
			gpMarioParticleManager->emit(BWANWAN_JPA_MS_BWAN_JUMP_SMOKE,
			                             &mPosition, 0, nullptr);
		}
		if (ctrl->checkPass(202.0f)) {
			shakeCamera(CAM_SHAKE_MODE_BOWA_LAND2);
			gpMarioParticleManager->emit(BWANWAN_JPA_MS_BWAN_JUMP_ROCK,
			                             &mPosition, 0, nullptr);
			gpMarioParticleManager->emit(BWANWAN_JPA_MS_BWAN_JUMP_SMOKE,
			                             &mPosition, 0, nullptr);
		}
	}

	if (mMActor->checkCurBckFromIndex(BWANWAN_BCK_SHAKE)
	    && mMActor->checkBckPass(40.0f))
		shakeCamera(CAM_SHAKE_MODE_BOWA_LAND1);

	if (getHitPoints() != 0)
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    BWANWAN_JPA_MS_BWAN_YUGAMI, getModel()->getAnmMtx(1), 3, this);

	if (mSparkRequest && getHitPoints() != 0) {
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    BWANWAN_JPA_MS_BWAN_HITYUGE, getModel()->getAnmMtx(1), 1, this);
		mSparkRequest = 0;
	}
}

// UNUSED, 0x64 in the map: TBossWanwan::perform and
// TNerveBWGraphWander::execute raise their balloons inline.
void TBossWanwan::showMessage(u32 message)
{
	u32 mask = 1 << (message - BALLOON_MSG_BWANWAN_PULL_PICKET);
	if (!checkBalloonShown(mask))
		gpMarDirector->getConsole()->startAppearBalloon(message, true);
	onBalloonShown(mask);
}

void TBossWanwan::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mIsInBath) {
		mHits[0]->perform(cue, graphics);
		TSpineEnemy::perform(cue, graphics);
		if (cue & CUE_MOVE)
			mMtxCalc->advanceMotionBlend(-mMotionBlendStep);
		if (cue & CUE_CALC_ANIM) {
			J3DFrameCtrl* ctrl
			    = mMActor->getFrameCtrl(ANM_TYPE_BRK);
			if (ctrl && ctrl->getFrame() > 0.5f * (f32)ctrl->getEnd()) {
				mJumpSmokePos = mPosition;
				mJumpSmokePos.y += 500.0f;
				gpMarioParticleManager->emitAndBindToPosPtr(
				    BWANWAN_JPA_MS_BWAN_KIRA, &mJumpSmokePos, 1, this);
			}
		}
		return;
	}

	if (cue & CUE_CALC_ANIM) {
		J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(ANM_TYPE_BRK);
		f32 heat = (f32)getHitPoints()
		           / (f32)getSaveParam2()->mSLBWHitPointMax.get();
		ctrl->setFrame(heat * (f32)(ctrl->getEnd() - 1));
		ctrl->setRate(0.0f);
		if (getHitPoints() == 0
		    || getHitPoints() == getSaveParam2()->mSLBWHitPointMax.get())
			mLeash->getNode(0)->setTemperature(heat);
		emitEffects();
	}

	mHits[0]->perform(cue, graphics);

	if (cue & CUE_MOVE) {
		mHits[1]->mPosition.set(mHits[0]->mPosition.x,
		                        mHits[0]->mPosition.y - 500.0f,
		                        mHits[0]->mPosition.z);
	}
	mHits[1]->perform(cue, graphics);

	if ((cue & CUE_MOVE) && mIsPicketFixed == 0) {
		mIdleTimer += 1;
		if (mIdleTimer < 0)
			mIdleTimer = 1;
		if (mIdleTimer > 600)
			showMessage(BALLOON_MSG_BWANWAN_PULL_PICKET);
	}

	if ((cue & CUE_MOVE) && mWasSprayed == 0
	    && gpMarDirector->unk58 >= 28800)
		showMessage(BALLOON_MSG_BWANWAN_SPRAY_IT);

	if (cue & CUE_MOVE) {
		if (mSpine->getLatestNerve() != &TNerveBWDie::theNerve()
		    && mSpine->getLatestNerve()
		           != &TNerveBWJumpToBath::theNerve()) {
			if (getHitPoints() != 0) {
				if (mIsPicketPlanted && mIsPicketFixed == 0) {
					mPulledTimer += 1;
					if (mPulledTimer > 600) {
						mSpine->pushNerve(&TNerveBWShake::theNerve());
						mPulledTimer = 0;
					}
				}
			} else {
				mPulledTimer = 0;
			}

			if (getHitPoints() == 0) {
				if (mSpine->getLatestNerve()
				    != &TNerveBWBark::theNerve()) {
					mCoolDownTimer += 1;
					if (mCoolDownTimer > 2400) {
						if (mSpine->getLatestNerve()
						    != &TNerveBWBark::theNerve())
							mSpine->setNext(&TNerveBWBark::theNerve());
					}
				}
			} else {
				if (gpMarDirector->unk58 % 20 == 0) {
					if (getHitPoints()
					    < getSaveParam2()->mSLBWHitPointMax.get())
						mHitPoints = getHitPoints() + 1;
				}
				mCoolDownTimer = 0;
			}
		}
	}

	if (cue & CUE_MOVE) {
		if (mSpine->getLatestNerve() == &TNerveBWGraphWander::theNerve()
		    && mPicket->isTaken()) {
			f32 pull = mPullVelocity.length();
			gpMSound->startSoundActorWithInfo(
			    MSD_SE_BS_WANWAN_M_DRAG, mLeash->getRope()->mPoints[6].unkC,
			    nullptr, pull, 0, 0, nullptr, 0, 4);
		}
	}

	TSpineEnemy::perform(cue, graphics);
	mLeash->testPerform(cue, graphics);
	mPicket->testPerform(cue, graphics);

	if (cue & CUE_MOVE)
		mMtxCalc->advanceMotionBlend(-mMotionBlendStep);
}

TBossWanwanManager::TBossWanwanManager(const char* name)
    : TEnemyManager(name)
{
}

// UNUSED, 0x1b4 in the map: TBossWanwanManager::load loads the eight .jpa
// files inline.
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
	return new TBossWanwan();
}

void TBossWanwanManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "bwanwan_body.bmd", 0x10220000, 0 },
		{ "bwanwan_chain.bmd", 0x10220000, 0 },
		{ "bwanwan_picket.bmd", 0x10220000, 0 },
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
	TBossWanwan* boss = (TBossWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		boss->mIsRolling = 0;
		if (!boss->getMActor()->checkCurBckFromIndex(BWANWAN_BCK_WAIT)
		    && !boss->getMActor()->checkCurBckFromIndex(BWANWAN_BCK_WAIT)) {
			if (boss->getHitPoints() == 0)
				boss->changeBck(BWANWAN_BCK_WAIT2);
			else
				boss->changeBck(BWANWAN_BCK_WAIT);

			J3DFrameCtrl* ctrl
			    = boss->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
			ctrl->setFrame(0.0f);
			ctrl->setRate(SMSGetAnmFrameRate());
		}
		boss->getMActor()->setBtpFromIndex(0);
		J3DFrameCtrl* btk
		    = boss->getMActor()->getFrameCtrl(ANM_TYPE_BTK);
		btk->setFrame(0.0f);
		btk->setRate(0.0f);
	}

	// Cooled down: switch from the angry walk to the calm one.
	if (boss->getMActor()->curAnmEndsNext() && boss->getHitPoints() == 0
	    && !boss->getMActor()->checkCurBckFromIndex(BWANWAN_BCK_WAIT2)) {
		boss->changeBck(BWANWAN_BCK_WAIT2);
		J3DFrameCtrl* ctrl
		    = boss->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		ctrl->setFrame(0.0f);
		ctrl->setRate(SMSGetAnmFrameRate());
	}

	if (boss->mIsPicketPlanted == 0 && boss->isHeadPulled()) {
		TGraphTracer* tracer   = boss->getTracer();
		const TGraphWeb* graph = tracer->getGraph();
		int prev               = tracer->getPrevIndex();

		Vec point;
		graph->getGraphNode(prev).getPoint(&point);
		JGeometry::TVec3<f32> toNode(point.x, point.y, point.z);
		toNode -= boss->getPosition();

		if (VECMag(toNode) < 400.0f && prev == graph->unk10) {
			spine->pushAfterCurrent(&TNerveBWJumpToBath::theNerve());
			return TRUE;
		}

		if (VECMag(toNode) < 400.0f)
			boss->reverseNextGraphNode();

		f32 speed
		    = 3.0f
		      * (((f32)boss->getHitPoints()
		          / (f32)boss->getSaveParam2()->mSLBWHitPointMax.get())
		         * boss->getMarchSpeed());
		boss->slideToCurPathNode(speed, boss->getTurnSpeed());
		return FALSE;
	}

	if (boss->isReachedToGoal()) {
		if (boss->jumpToNextGraphNode() >= 0) {
			spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
			spine->pushAfterCurrent(&TNerveBWJump::theNerve());
		} else {
			spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());

			TGraphTracer* tracer   = boss->getTracer();
			int curr               = tracer->getCurGraphIndex();
			int prev               = tracer->getPrevIndex();
			const TGraphWeb* graph = tracer->getGraph();

			if (curr >= 0
			    && graph->getGraphNode(curr).getRailNode()->mConnectionNum >= 2
			    && gpMarDirector->unk58 >= 14400)
				boss->showMessage(BALLOON_MSG_BWANWAN_LEAD_TO_HOT);

			JGeometry::TVec3<f32> facing
			    = MsGetVecFromRotY(boss->mRotation.y, 1.0f);
			boss->getTracer()->moveTo(graph->getEscapeDirLimited(
			    prev, curr, facing, boss->getPosition(), 100.0f, -1));
			boss->setGoalPathFromGraph();
			boss->unk128 = 0;
			boss->unk12C = 0.0f;
		}
		return TRUE;
	}

	int taken = 0;
	boss->mIsRolling = 0;
	f32 heat = (f32)boss->getHitPoints()
	           / (f32)boss->getSaveParam2()->mSLBWHitPointMax.get();
	if (boss->getPicket()->getHolder())
		taken = 1;

	if (taken) {
		// Mario dragging the stake against the boss's own direction slows it
		// down; dragging with it speeds it up.
		f32 marioYaw = gpMarioOriginal->mIntendedYaw * (360.0f / 65536.0f);
		JGeometry::TVec3<f32> marioDir;
		marioDir.set(MsSin(marioYaw) * 1.0f, 0.0f, MsCos(marioYaw) * 1.0f);

		JGeometry::TVec3<f32> toBoss(boss->getPosition());
		toBoss -= gpMarioOriginal->getPosition();
		VECNormalize(toBoss, toBoss);

		f32 rate
		    = 1.0f
		      - (f32)((f64)(gpMarioOriginal->mIntendedMag * 0.03125f) * 0.75)
		            * -toBoss.dot(marioDir);
		if (rate < 0.0f)
			rate = 0.0f;
		else if (rate > 1.5f)
			rate = 1.5f;

		boss->slideToCurPathNode(rate * (heat * boss->getMarchSpeed()),
		                         boss->getTurnSpeed());
	} else {
		boss->slideToCurPathNode(heat * boss->getMarchSpeed() + 0.2f,
		                         boss->getTurnSpeed());
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBWRoll, TLiveActor)
{
	TBossWanwan* boss = (TBossWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		J3DFrameCtrl* ctrl
		    = boss->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		ctrl->setFrame(0.0f);
		ctrl->setRate(0.0f);
		boss->mIsRolling = 1;
	}

	if (boss->isReachedToGoal()) {
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		J3DFrameCtrl* ctrl = boss->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		ctrl->setRate(SMSGetAnmFrameRate());
		return TRUE;
	}

	f32 speed = boss->getSaveParam2()->mSLAttackSpeed.get();
	boss->walkToCurPathNode(speed, boss->getTurnSpeed(), 0.0f);
	return FALSE;
}

DEFINE_NERVE(TNerveBWBark, TLiveActor)
{
	TBossWanwan* boss = (TBossWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		boss->changeBck(BWANWAN_BCK_BARK);
		boss->mIsRolling = 0;
		boss->mRollAngle = 0.0f;

		if (boss->mIsPicketFixed == 0) {
			boss->releasePicket();
			BWPicketSound()->startSoundActor(MSD_SE_BS_WANWAN_ESCAPE1,
			                          &BosswanwanPicket(boss)->mPosition, 0,
			                          nullptr, 0, 4);
		}
	}

	// The bark that makes the boss angry again.
	if (spine->getTime() == 280)
		boss->mHitPoints = BosswanwanSaveParam2(boss)->mSLBWHitPointMax.get();

	if (boss->getMActor()->curAnmEndsNext()) {
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		boss->showMessage(BALLOON_MSG_BWANWAN_BARKED);
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBWJump, TLiveActor)
{
	TBossWanwan* boss = (TBossWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		const JGeometry::TVec3<f32>& goal = boss->getUnk104().getPoint();
		f32 speed = boss->getTracer()->unkC;
		boss->setVelocity(
		    boss->calcVelocityToJumpToY(goal, speed, boss->getGravityY()));
		boss->onLiveFlag(LIVE_FLAG_AIRBORNE);
		boss->mIsRolling = 0;
	}

	if (boss->isReachedToGoal()) {
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		return TRUE;
	}

	boss->walkToCurPathNode(0.0f, boss->getTurnSpeed(), 0.0f);
	return FALSE;
}

DEFINE_NERVE(TNerveBWStun, TLiveActor)
{
	TBossWanwan* boss = (TBossWanwan*)spine->getBody();

	if (boss->isHeadPulled()) {
		TGraphTracer* tracer   = boss->getTracer();
		const TGraphWeb* graph = tracer->getGraph();
		int prev               = tracer->getPrevIndex();

		Vec point;
		graph->getGraphNode(prev).getPoint(&point);
		JGeometry::TVec3<f32> toNode(point.x, point.y, point.z);
		toNode -= boss->mPosition;

		if (VECMag(toNode) < 100.0f) {
			if (prev == graph->unk10) {
				spine->pushAfterCurrent(&TNerveBWJumpToBath::theNerve());
				return TRUE;
			}
			boss->reverseNextGraphNode();
		}
		return FALSE;
	}

	if (spine->getTime() > boss->getSaveParam2()->mSLStunTimer.get()) {
		boss->mHitPoints = boss->getSaveParam2()->mSLBWHitPointMax.get();
		spine->pushAfterCurrent(&TNerveBWWakeup::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBWWakeup, TLiveActor)
{
	TBossWanwan* boss = (TBossWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		boss->changeBck(BWANWAN_BCK_WAKE);
		boss->getMActor()->setBtpFromIndex(2);
		boss->mIsRolling = 0;
		boss->mRollAngle = 0.0f;
	}

	if (boss->getMActor()->curAnmEndsNext()) {
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBWJumpToBath, TLiveActor)
{
	TBossWanwan* boss = (TBossWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		JGeometry::TVec3<f32> velocity = boss->calcVelocityToJumpToY(
		    BW_BATH_POS, 10.0f, boss->getGravityY());
		boss->setGoalPath(TPathNode(BW_BATH_POS));
		boss->mVelocity = velocity;
		boss->onLiveFlag(LIVE_FLAG_AIRBORNE);
		boss->mIsRolling = 0;
	}

	// The splash as the boss breaks the surface of the hot spring.
	if (spine->getTime() > 120 && boss->mSplashDone == 0
	    && boss->mPosition.y < 10.0f + BW_BATH_POS.y) {
		TEffectColumWater* colum
		    = (TEffectColumWater*)gpConductor->makeOneEnemyAppear(
		        boss->mPosition, "エフェクト水柱マネージャー", 1);
		if (colum) {
			JGeometry::TVec3<f32> scale(5.0f, 5.0f, 5.0f);
			JGeometry::TVec3<f32> pos(boss->mPosition.x,
			                          (500.0f + boss->mPosition.y) - 30.0f,
			                          boss->mPosition.z);
			colum->generate(pos, scale);
			boss->mSplashDone = 1;
		}
		gpMSound->startSoundActor(MSD_SE_BS_WANWAN_DIVE, &boss->mPosition, 0,
		                          nullptr, 0, 4);
	}

	JGeometry::TVec3<f32> bath(BW_BATH_POS);
	JGeometry::TVec3<f32> toBath(bath);
	toBath -= boss->mPosition;

	if (toBath.squared() < 10000.0f && boss->mPosition.y <= BW_BATH_POS.y) {
		boss->mPosition = bath;
		spine->pushAfterCurrent(&TNerveBWDie::theNerve());
		return TRUE;
	}

	boss->walkToCurPathNode(0.0f, boss->getTurnSpeed(), 0.0f);
	return FALSE;
}

DEFINE_NERVE(TNerveBWDie, TLiveActor)
{
	TBossWanwan* boss = (TBossWanwan*)spine->getBody();
	MActor* actor     = boss->getMActor();

	if (spine->getTime() == 0) {
		JGeometry::TVec3<f32> zero(0.0f, 0.0f, 0.0f);
		boss->mVelocity       = zero;
		boss->mLinearVelocity = zero;
		boss->onLiveFlag(LIVE_FLAG_UNK10);
		boss->offLiveFlag(LIVE_FLAG_AIRBORNE);
		gpMarioParticleManager->emit(BWANWAN_JPA_MS_BWAN_DEADYUGE,
		                             &boss->mPosition, 0, nullptr);
	}

	// Cooling the boss down one hit point per frame until the water wins.
	if (boss->getHitPoints() != 0) {
		boss->decHitPoints();
		J3DFrameCtrl* ctrl = actor->getFrameCtrl(ANM_TYPE_BCK);
		ctrl->setFrame(0.0f);
		ctrl->setRate(0.0f);
		spine->pushAfterCurrent(&TNerveBWDie::theNerve());
		return TRUE;
	}

	if (spine->getTime() == 0) {
		// TBossWanwan::takeBath (UNUSED, 0x184) is exactly this block, but
		// MWCC will not expand it from here, so it is spelled out; see the
		// comment on that function.
		SMSGetMarDirector()->fireStartDemoCamera("bwanwan_down_camera", nullptr,
		                                         -1, 0.0f, true, nullptr, 0,
		                                         nullptr, 0);
		boss->mIsRolling = 0;
		boss->mRollAngle = 0.0f;
		boss->mIsInBath  = 1;
		boss->mPosition  = BW_BATH_POS;

		JGeometry::TVec3<f32> center(boss->mPosition);
		center.y += 500.0f;

		JGeometry::TVec3<f32> scale(boss->getScaling());
		scale.scale(1.1f);

		boss->getMapCollisionManager()->setUpUnk8TRS(
		    center, boss->getRotation(), scale);

		boss->mHits[0]->onHitFlag(HIT_FLAG_NO_COLLISION);
		boss->mHits[1]->onHitFlag(HIT_FLAG_NO_COLLISION);
		TBWLeash* leash = boss->getLeash();
		for (int i = 0; i < leash->getRope()->mNumPoints; ++i)
			leash->getNode(i)->onHitFlag(HIT_FLAG_NO_COLLISION);
		boss->getPicket()->onHitFlag(HIT_FLAG_NO_COLLISION);

		boss->changeBck(BWANWAN_BCK_DOWN);
		actor->setBtpFromIndex(0);
		actor->setBrkFromIndex(1);
	}

	if (spine->getTime() > 60 && !boss->mShineAppeared
	    && !gpMarDirector->isDemoMode3()) {
		gpItemManager->makeShineAppearWithDemo(
		    "シャイン（ボス用）", "ボスシャインカメラ", boss->mPosition.x,
		    boss->mPosition.y, boss->mPosition.z);
		boss->mShineAppeared = 1;
	}

	if (actor->curAnmEndsNext(ANM_TYPE_BRK, nullptr))
		actor->getFrameCtrl(ANM_TYPE_BRK)->setRate(0.0f);

	return FALSE;
}

DEFINE_NERVE(TNerveBWJumpAway, TLiveActor)
{
	TBossWanwan* boss = (TBossWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		JGeometry::TVec3<f32> velocity = boss->calcVelocityToJumpToY(
		    BW_HEAD_START, 40.0f, boss->getGravityY());
		boss->setGoalPath(TPathNode(BW_HEAD_START));
		boss->mVelocity = velocity;
		boss->onLiveFlag(LIVE_FLAG_AIRBORNE);
		boss->mIsRolling = 0;
	}

	if (boss->mIsLeashStretched) {
		spine->pushAfterCurrent(&TNerveBWFall::theNerve());
		return TRUE;
	}

	if (boss->isReachedToGoal()) {
		boss->mPosition = BW_HEAD_START;
		boss->unk124->reset();
		boss->unk124->reset2();
		boss->goToShortestNextGraphNode();
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		return TRUE;
	}

	boss->walkToCurPathNode(0.0f, boss->getTurnSpeed(), 0.0f);
	return FALSE;
}

DEFINE_NERVE(TNerveBWShake, TLiveActor)
{
	TBossWanwan* boss = (TBossWanwan*)spine->getBody();
	MActor* actor     = boss->getMActor();

	if (spine->getTime() == 0)
		boss->changeBck(BWANWAN_BCK_SHAKE);

	if (actor->curAnmEndsNext()) {
		boss->releasePicket();
		BWPicketSound()->startSoundActor(MSD_SE_BS_WANWAN_ESCAPE2,
		                          &BosswanwanPicket(boss)->mPosition, 0, nullptr, 0,
		                          4);
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBWFall, TLiveActor)
{
	TBossWanwan* boss = (TBossWanwan*)spine->getBody();

	if (spine->getTime() == 0) {
		const JGeometry::TVec3<f32>& picket = boss->getPicket()->mPosition;
		JGeometry::TVec3<f32> velocity = boss->calcVelocityToJumpToY(
		    picket, 5.0f, boss->getGravityY());
		boss->setGoalPath(TPathNode(boss->mPicket->mPosition));
		boss->mVelocity = velocity;
		boss->onLiveFlag(LIVE_FLAG_AIRBORNE);
		boss->mIsRolling = 0;
	}

	if (boss->isReachedToGoal()) {
		boss->mPosition = boss->getPicket()->mPosition;
		boss->unk124->reset();
		boss->unk124->reset2();
		boss->goToShortestNextGraphNode();
		spine->pushAfterCurrent(&TNerveBWGraphWander::theNerve());
		return TRUE;
	}

	return FALSE;
}
