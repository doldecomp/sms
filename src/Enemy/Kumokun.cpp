#include <Enemy/Kumokun.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <Enemy/Graph.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <Player/MarioAccess.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static const char* kumokun_bastable[] = {
	"/scene/kumokun/bas/kumo_down1.bas",
	"/scene/kumokun/bas/kumo_flying1_loop.bas",
	"/scene/kumokun/bas/kumo_flyingdown1.bas",
	"/scene/kumokun/bas/kumo_hit_end.bas",
	"/scene/kumokun/bas/kumo_hit_loop.bas",
	"/scene/kumokun/bas/kumo_hit_start.bas",
	"/scene/kumokun/bas/kumo_run1_end.bas",
	"/scene/kumokun/bas/kumo_run1_loop.bas",
	nullptr,
	"/scene/kumokun/bas/kumo_turn1_end.bas",
	"/scene/kumokun/bas/kumo_turn1_loop.bas",
	nullptr,
	"/scene/kumokun/bas/kumo_wait1.bas",
};

static const char* kumokun_bas_c_table[] = {
	"/scene/kumokun/bas/kumo_down1_c.bas",
	"/scene/kumokun/bas/kumo_flying1_loop_c.bas",
	"/scene/kumokun/bas/kumo_flyingdown1_c.bas",
	"/scene/kumokun/bas/kumo_hit_end_c.bas",
	"/scene/kumokun/bas/kumo_hit_loop_c.bas",
	"/scene/kumokun/bas/kumo_hit_start_c.bas",
	"/scene/kumokun/bas/kumo_run1_end_c.bas",
	"/scene/kumokun/bas/kumo_run1_loop_c.bas",
	nullptr,
	"/scene/kumokun/bas/kumo_turn1_end_c.bas",
	"/scene/kumokun/bas/kumo_turn1_loop_c.bas",
	nullptr,
	"/scene/kumokun/bas/kumo_wait1_c.bas",
};

class TWallAtGraph {
public:
	TWallAtGraph()
	    : unk0(nullptr)
	{
	}

	bool init(const TGraphWeb*, const JGeometry::TVec3<f32>&,
	          const JGeometry::TVec3<f32>&);
	bool checkWalls(JGeometry::TVec3<f32>*, f32);

	// fabricated
	bool checkWall(TPartition3f* wall, JGeometry::TVec3<f32>* param_2,
	               f32 param_3)
	{
		// TODO: probably fakematch
		JGeometry::TVec3<f32> tmp;
		tmp.set(*param_2);
		f32 dist = wall->mNormal.dot(tmp) - wall->mDist;
		if (param_3 - dist > 0.0f) {
			param_2->scaleAdd(param_3 - dist, tmp, wall->mNormal);
			return true;
		}
		return false;
	}

public:
	/* 0x0 */ TPartition3f* unk0;
	/* 0x4 */ s32 unk4;
};

bool TWallAtGraph::init(const TGraphWeb* param_1,
                        const JGeometry::TVec3<f32>& param_2,
                        const JGeometry::TVec3<f32>& param_3)
{
	if (param_1 == nullptr || param_1->isDummy())
		return false;

	if (!param_1->startIsEnd())
		return false;

	int sz = param_1->getNodeNum();
	unk4   = sz;
	unk0   = new TPartition3f[sz];

	for (int i = 0; i < sz; ++i) {
		JGeometry::TVec3<f32> diff
		    = param_1->indexToPoint(i + 1 - ((i + 1) / sz) * sz);
		diff -= param_1->indexToPoint(i);

		JGeometry::TVec3<f32> local_54 = param_2;
		local_54 -= param_1->indexToPoint(i);

		JGeometry::TVec3<f32> local_70;
		local_70.cross(param_3, diff);
		local_70.normalize();
		if (local_70.dot(local_54) < 0.0f)
			local_70.negate();

		unk0[i] = TPartition3f(local_70, param_1->indexToPoint(i));
	}

	return true;
}

bool TWallAtGraph::checkWalls(JGeometry::TVec3<f32>* param_1, f32 param_2)
{
	if (!unk0)
		return false;

	bool result = false;
	for (TPartition3f *it = unk0, *e = unk0 + unk4; it != e; ++it) {
		result |= checkWall(it, param_1, param_2);
	}
	return result;
}

TKumokun::TKumokun(const char* name)
    : TSmallEnemy(name)
    , unk1D8(3)
    , unk1E8(nullptr)
    , unk1EC(2)
{
}

const char** TKumokun::getBasNameTable() const
{
	if (isFenceSound(unk198))
		return kumokun_bastable;
	return kumokun_bas_c_table;
}

void TKumokun::init(TLiveManager* live_manager)
{
	mManager = live_manager;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("kumo_model1.bmd", 0);
	mSpine->initWith(&TNerveKumokunSearch::theNerve());
	initCollision();
	initAnmSound();
	unk1D4 = false;

	if (unk124->getGraph() != nullptr) {
		unk124->reset();
		goToShortestNextGraphNode();
		unk1E8 = new TWallAtGraph;

		// BUG: memory leak woo hoo
		if (!unk1E8->init(unk124->getGraph(), mPosition, getPlaneNormal()))
			unk1E8 = nullptr;
	}
}

void TKumokun::initCollision()
{
	initHitActor(0x1000002c, 2, -0x70000000, 60.0f, 50.0f, 65.0f, 70.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mBodyRadius       = 100.0f;
	mWallRadius       = 100.0f;
	mHeadHeight       = 75.0f;
	mScaledBodyRadius = 80.0f;
	initAttachPlane();
}

void TKumokun::initAttachPlane()
{
	TBGWallCheckRecord record(mPosition.x, mPosition.y, mPosition.z, 100.0f, 1,
	                          0);

	const TBGCheckData* wall = gpMap->isTouchedWallsAndMoveXZ(&record)
	                               ? record.mResultWalls[0]
	                               : nullptr;
	if (wall) {
		unk198 = wall;
		JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
		unk19C.setRotate(getPlaneNormal(), up, 0.0f);
		return;
	}

	f32 headHeight = mHeadHeight;
	JGeometry::TVec3<f32> pos;
	pos.set(mPosition);
	const TBGCheckData* roof = nullptr;
	f32 dVar6                = gpMap->checkRoof(pos.x, pos.y, pos.z, &roof);
	f32 fVar8                = dVar6 - 1.0f - pos.y;
	if (!(0.0f <= fVar8 && fVar8 < headHeight))
		roof = nullptr;

	if (roof) {
		unk198 = roof;
		unk19C.setEulerZ(JGeometry::TUtil<f32>::PI());
		return;
	}

	JGeometry::TVec3<f32> pos2;
	pos2.set(mPosition);
	pos2.y += mHeadHeight;
	const TBGCheckData* floor = nullptr;
	f32 dVar62 = gpMap->checkGround(pos2.x, pos2.y, pos2.z, &floor);
	dVar62 += 1.0f;
	if (!(pos.y <= dVar62 + 0.05f))
		floor = nullptr;

	if (floor) {
		unk198 = floor;

		JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
		unk19C.setRotate(getPlaneNormal(), up, 0.0f);
	}
}

void TKumokun::reset() { }

void TKumokun::bind()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	if (isFlying()) {
		bindOnFlying();
		return;
	}

	JGeometry::TVec3<f32> local_168 = mLinearVelocity;
	JGeometry::TVec3<f32> local_104 = mVelocity;
	local_168 += local_104;

	bool bVar7;

	JGeometry::TVec3<f32> local_150;
	JGeometry::TVec3<f32> local_15C;

	if (isOnFloor()) {
		JGeometry::TVec3<f32> local_140 = local_168;
		local_140.normalize();

		local_140 += mPosition;

		const TBGCheckData* floor;
		bVar7 = checkOnMovingFloor(&local_150, &floor, local_140, local_168);
		bVar7 |= checkOnMovingFloor(&local_15C, &floor, mPosition, local_168);
	} else if (isOnRoof()) {
		JGeometry::TVec3<f32> local_134 = local_168;
		local_134.normalize();

		local_134 += mPosition;

		const TBGCheckData* floor;
		bVar7 = checkOnMovingRoof(&local_150, &floor, local_134, local_168);
		bVar7 |= checkOnMovingRoof(&local_15C, &floor, mPosition, local_168);
	} else {
		JGeometry::TVec3<f32> local_128 = local_168;
		local_128.setLength(0.5f * mHeadHeight);

		local_128 += mPosition;

		const TBGCheckData* floor;
		bVar7 = checkOnMovingWall(&local_150, &floor, local_128, local_168);
		bVar7 |= checkOnMovingWall(&local_15C, &floor, mPosition, local_168);
	}

	if (unk124->getGraph() != nullptr && unk1E8 != nullptr) {
		JGeometry::TVec3<f32> local_11C = mPosition;
		local_11C += local_15C;
		local_11C += local_150;

		JGeometry::TVec3<f32> local_110 = local_11C;
		bVar7 |= unk1E8->checkWalls(&local_110, mWallRadius);

		JGeometry::TVec3<f32> local_f8 = local_110 - mPosition;

		local_168 += local_f8;
	}

	if (bVar7)
		++mHitPlaneCounter;
	else
		resetHitPlaneCounter();

	mLinearVelocity = local_15C + local_150;
}

bool TKumokun::checkOnMovingWall(JGeometry::TVec3<f32>* param_1,
                                 const TBGCheckData** param_2,
                                 const JGeometry::TVec3<f32>& param_3,
                                 const JGeometry::TVec3<f32>& param_4) const
{
	bool result = false;

	JGeometry::TVec3<f32> normal = getPlaneNormal();

	JGeometry::TVec3<f32> local_30;
	local_30.scaleAdd(100.0f, normal, param_3);

	JGeometry::TVec3<f32> local_3C = local_30;
	local_3C += param_4;

	f32 yTmp   = local_3C.y;
	f32 dVar10 = gpMap->checkGround(local_3C.x, yTmp + mHeadHeight, local_3C.z,
	                                param_2);
	dVar10 += 1.0f;
	dVar10 += mHeadHeight;
	if (yTmp <= dVar10 + 0.05f) {
		result     = true;
		local_3C.y = dVar10;
	}

	JGeometry::TVec3<f32> local_70 = getPlaneNormal();
	JGeometry::TVec3<f32> local_48;
	local_48.scaleAdd(-10.0f, local_3C, local_70);

	const TBGCheckData* wall = checkWallPlane(&local_48, mHeadHeight, 100.0f);

	JGeometry::TVec3<f32> local_3C2;
	if (!wall) {
		result = true;
		local_3C2.set(local_48);
	} else {
		local_3C2.x = local_48.x;
		local_3C2.z = local_48.z;
	}
	(void)local_48; // hmmmmm....

	JGeometry::TVec3<f32> offset = getPlaneNormal();
	offset *= -100.0f;
	JGeometry::TVec3<f32> vec = local_3C2;
	vec += offset;

	param_1->set(vec);
	*param_1 -= param_3;

	return result;
}

bool TKumokun::checkOnMovingFloor(JGeometry::TVec3<f32>* param_1,
                                  const TBGCheckData** param_2,
                                  const JGeometry::TVec3<f32>& param_3,
                                  const JGeometry::TVec3<f32>& param_4) const
{
	bool uVar7 = false;

	JGeometry::TVec3<f32> local_1C = param_3;
	JGeometry::TVec3<f32> local_98 = local_1C;
	JGeometry::TVec3<f32> local_8C = local_98;

	local_8C += param_4;

	JGeometry::TVec3<f32> local_80 = unk198->getNormal();
	local_80 *= -10.0f;
	local_8C += local_80;

	f32 yTmp   = local_8C.y;
	f32 dVar10 = gpMap->checkGround(local_8C.x, yTmp + mHeadHeight, local_8C.z,
	                                param_2);
	dVar10 += 1.0f;
	if (yTmp <= dVar10 + 0.05f) {
		if (30.0f < dVar10 - yTmp) {
			uVar7 = true;
			local_8C.set(local_98);
		} else {
			local_8C.y = yTmp;
		}
	} else {
		uVar7 = true;
		local_8C.set(local_98);
	}

	JGeometry::TVec3<f32> local_70 = local_8C;

	const TBGCheckData* wall = checkWallPlane(&local_70, mHeadHeight, 100.0f);

	*param_2 = wall;
	if (*param_2 != nullptr) {
		uVar7      = true;
		local_8C.x = local_70.x;
		local_8C.z = local_70.z;
	}

	param_1->set(local_8C);
	param_1->sub(param_3);

	return uVar7;
}

bool TKumokun::checkOnMovingRoof(JGeometry::TVec3<f32>* param_1,
                                 const TBGCheckData** param_2,
                                 const JGeometry::TVec3<f32>& param_3,
                                 const JGeometry::TVec3<f32>& param_4) const
{
	bool uVar7 = false;

	JGeometry::TVec3<f32> local_C0 = getPlaneNormal();
	local_C0 *= -mHeadHeight / 2.0f;

	JGeometry::TVec3<f32> local_b4 = param_3;
	local_b4 += local_C0;

	JGeometry::TVec3<f32> local_A8 = local_b4;
	local_A8 += param_4;

	JGeometry::TVec3<f32> local_9C = getPlaneNormal();
	local_9C *= -10.0f;
	local_A8 += local_9C;

	f32 yTmp = local_A8.y;
	f32 dVar10
	    = gpMap->checkRoof(local_A8.x, yTmp - mHeadHeight, local_A8.z, param_2);
	dVar10 -= 1.0f;
	if (yTmp > dVar10 - 0.05f) {
		local_A8.y = yTmp;
	} else if (mHeadHeight < dVar10 - yTmp) {
		local_A8.y = yTmp;
	} else {
		uVar7 = true;
		local_A8.set(local_b4);
	}

	JGeometry::TVec3<f32> local_8C = local_A8;

	const TBGCheckData* wall = checkWallPlane(&local_8C, mHeadHeight, 100.0f);

	*param_2 = wall;
	if (*param_2 != nullptr) {
		uVar7      = true;
		local_A8.x = local_8C.x;
		local_A8.z = local_8C.z;
	}

	local_A8 -= local_C0;

	param_1->set(local_A8);
	param_1->sub(param_3);

	return uVar7;
}

void TKumokun::bindOnFlying()
{
	bool hit = false;

	JGeometry::TVec3<f32> local_74 = mPosition;
	local_74 += mLinearVelocity;
	local_74 += mVelocity;

	const TBGCheckData* roof = checkRoofPlane(&local_74, mHeadHeight);

	if (roof != nullptr) {
		hit = true;
		mVelocity.zero();
		offLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	const TBGCheckData* floor = checkFloorPlane(&local_74, mHeadHeight, 0);

	if (floor) {
		hit = true;
		mVelocity.zero();
		offLiveFlag(LIVE_FLAG_AIRBORNE);
	} else {
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	const TBGCheckData* wall
	    = checkWallPlane(&local_74, mWallRadius, mBodyRadius);

	if (wall)
		hit = true;

	if (hit) {
		mHitPlaneCounter += 1;
		if (mHitPlaneCounter > 30) {
			if (roof)
				unk198 = roof;
			else if (floor)
				unk198 = floor;
			else
				unk198 = wall;
		}
	} else {
		resetHitPlaneCounter();
	}

	mLinearVelocity = local_74 - mPosition;
}

void TKumokun::moveObject()
{
	if (unk1D0 > 0)
		unk1D0 -= 1;

	updateAnimation();

	TSmallEnemy::moveObject();
}

void TKumokun::control()
{
	TLiveActor::control();
	const TBGCheckData* ground;
	mGroundHeight
	    = gpMap->checkGround(mPosition.x, mPosition.y, mPosition.z, &ground);
}

void TKumokun::calcShadowPos() { }

BOOL TKumokun::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return false;

	switch (param_2) {
	case HIT_MESSAGE_PUNCH:
		return behaveHitPunch();

	case HIT_MESSAGE_HIP_DROP:
		return behaveHitHipdrop();

	case HIT_MESSAGE_TRAMPLE:
		return behaveHitTrample();

	default:
		return TSmallEnemy::receiveMessage(param_1, param_2);
	}

	return false;
}

void TKumokun::behaveToWater(THitActor*) { behaveHitWater(); }

void TKumokun::behaveHitWater()
{
	if (unk1D0 > 0)
		return;

	unk1D0 = 30;
	mSpine->reset();
	mSpine->setNext(&TNerveKumokunFreeze::theNerve());
}

bool TKumokun::behaveHitPunch()
{
	if ((!SMS_IsMarioFencing() || isSameNormal(SMS_GetMarioWlPlane(), unk198))
	    && (!SMS_IsMarioRoofing()
	        || isSameNormal(SMS_GetMarioRfPlane(), unk198)))
		return false;

	mSpine->reset();
	mSpine->setNext(&TNerveKumokunPreFly::theNerve());
	return true;
}

bool TKumokun::behaveHitTrample()
{
	if (isOnFloor()) {
		mSpine->reset();
		mSpine->setNext(&TNerveSmallEnemyDie::theNerve());
		unk1EC = 0;
		return true;
	}

	return false;
}

bool TKumokun::behaveHitHipdrop()
{
	if (isOnFloor()) {
		mSpine->reset();
		mSpine->setNext(&TNerveSmallEnemyDie::theNerve());
		unk1EC = 0;
		return true;
	}

	if (isOnRoof()) {
		mSpine->reset();
		mSpine->setNext(&TNerveKumokunPreFly::theNerve());
		return true;
	}

	return false;
}

void TKumokun::updateAnimation()
{
	if (unk1D8.size() > 0 && checkCurAnmEnd(0)) {
		if (mMActor->checkCurAnm(unk1D8.pop(), 0) && unk1D8.size() >= 1)
			changeBck(unk1D8.top()); // TODO: wrong ops
		else
			unk1D8.clear();
	}
}

void TKumokun::updateCollision() { }

void TKumokun::clearAnmStack() { unk1D8.clear(); }

void TKumokun::pushNextAnm(const char* name, bool start)
{
	unk1D8.push(name);
	if (start)
		changeBck(name);
}

void TKumokun::changeBck(const char* name)
{
	mMActor->setBck(name);
	setCurAnmSound();

	f32 fVar1 = 1.0f;
	if (mSpine->getLatestNerve() != &TNerveSmallEnemyDie::theNerve()
	    && (unk1D4
	        || mSpine->getLatestNerve() == &TNerveKumokunPreFly::theNerve())) {
		fVar1 = 2.0f;
	}

	mMActor->getFrameCtrl(0)->setRate(SMSGetAnmFrameRate() * 0.5f * fVar1);
}

void TKumokun::setDeadAnm()
{
	switch (unk1EC) {
	case 0:
		changeBck("kumo_down1");
		break;

	case 1:
		changeBck("kumo_flyingdown1");
		break;

	case 2:
		mMActor->getFrameCtrl(0)->init(1);
		mMActor->getFrameCtrl(0)->setFrame(0.0f);
		break;
	}
}

bool TKumokun::checkSerialAnmEnd() const
{
	return unk1D8.empty() && checkCurAnmEnd(0);
}

void TKumokun::calcRootMatrix()
{
	if (isTaken()) {
		TSpineEnemy::calcRootMatrix();
		return;
	}

	JGeometry::TVec3<f32> offset(0.0f);

	if (isOnWall() || isOnRoof())
		offset.scale(-30.0f, getPlaneNormal());

	JGeometry::TVec3<f32> pos = mPosition;
	pos += offset;

	TPosition3f mtx;
	mtx.setQT(unk19C, pos);
	getModel()->setBaseScale(mScaling);
	getModel()->setBaseTRMtx(mtx);

	if (isFlying()) {
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(
		        PARTICLE_MS_KIL_SMOKE, getModel()->getBaseTRMtx(), 1, this)) {
			emitter->setScale(JGeometry::TVec3<f32>(1.5f));
			emitter->unk180.a = 128;
		}
	}
}

void TKumokun::keepDistance(const THitActor&) { }

void TKumokun::attackToMario()
{
	if (isAttack())
		sendAttackMsgToMario();
}

void TKumokun::prepareWalk()
{
	resetHitPlaneCounter();
	changeBck("kumo_run1_loop");
}

bool TKumokun::doWalk()
{
	JGeometry::TVec3<f32> forward;
	unk19C.getZDir(forward);

	f32 speed = (unk1D4 ? getSaveParam2()->mAttackSpeed
	                    : getSaveParam2()->mMarchSpeed)
	                .get();

	forward *= speed;
	mLinearVelocity = forward;

	bool result = true;
	if (mHitPlaneCounter <= 30)
		result = false;

	return result;
}

void TKumokun::decideTarget()
{
	JGeometry::TVec3<f32> local_34;
	if (isFindOutMario(&local_34)) {
		decideTargetOnFindingMario(local_34);
	} else {
		decideTargetAtRandom();
	}
}

void TKumokun::decideTargetOnFindingMario(const JGeometry::TVec3<f32>& param_1)
{
	decideTargetAtDir(param_1);
	unk1D4 = true;
}

static bool is_antiparallel(const JGeometry::TVec3<f32>& v1,
                            const JGeometry::TVec3<f32>& v2)
{
	f32 fVar8 = v1.dot(v2) - -1.0f;
	f32 eps   = JGeometry::TUtil<f32>::epsilon();
	return -eps <= fVar8 && fVar8 <= eps;
}

void TKumokun::decideTargetAtDir(const JGeometry::TVec3<f32>& param_1)
{
	JGeometry::TVec3<f32> local_C4 = param_1;

	JGeometry::TQuat4<f32> local_b4 = unk19C;

	local_C4.y = 0.0f;
	local_C4.normalize();

	JGeometry::TVec3<f32> forward(0.0f, 0.0f, 1.0f);

	JGeometry::TQuat4<f32> local_A4;
	if (is_antiparallel(local_C4, forward)) {
		local_A4.setEulerY(JGeometry::TUtil<f32>::PI());
	} else {
		local_A4.setRotate(forward, local_C4, 1.0f);
	}

	local_b4.mul(local_A4);

	unk1AC = unk19C;
	unk1BC = local_b4;
	unk1CC = 0.0f;
}

void TKumokun::decideTargetAtRandom()
{
	JGeometry::TQuat4<f32> q = getQuat();

	JGeometry::TQuat4<f32> p;
	p.setRotate(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f),
	            (MsRandF() + 0.5f) * M_PI);

	q.mul(q, p);

	unk1AC = unk19C;
	unk1BC = q;

	unk1CC = 0.0f;

	unk1D4 = false;
}

void TKumokun::decideTargetOnGraph() { }

JGeometry::TVec3<f32>
TKumokun::rotateGoalDirToLocal(const JGeometry::TVec3<f32>& param_1) const
{
	JGeometry::TVec3<f32> diff = param_1;
	diff -= mPosition;

	// unit quat, conj = inv
	JGeometry::TQuat4<f32> inv = getQuat();
	inv.conjugate();

	JGeometry::TVec3<f32> result;
	inv.rotate(diff, result);

	return result;
}

bool TKumokun::doAdjustTarget()
{
	bool result = false;

	f32 fVar2 = getSaveParam2()->mTorqueY.get();
	fVar2 *= unk1D4 ? 3.0f : 1.0f;
	unk1CC += fVar2;
	if (1.0f <= unk1CC) {
		unk1CC = 1.0f;
		result = true;
	}

	unk19C.slerp(unk1AC, unk1BC, unk1CC);
	f32 l = 1.0f; // TODO: fakematch?
	unk19C.setLength(l);

	return result;
}

void TKumokun::prepareFly()
{
	JGeometry::TVec3<f32> vel = getPlaneNormal();
	vel.setLength(getSaveParam2()->mFlySpeed.get());
	mLinearVelocity = vel;

	resetHitPlaneCounter();

	JGeometry::TVec3<f32> inc = getPlaneNormal();
	inc *= mHeadHeight;
	mPosition += inc;

	doFly();
}

bool TKumokun::doFly()
{
	if (mHitPlaneCounter > 30 || (isHitPlane() && mSpine->getTime() > 30)) {
		unk19C.setRotate(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f),
		                 getPlaneNormal(), 1.0f);
		return true;
	}

	return false;
}

bool TKumokun::isOnSamePlaneWithMario() const
{
	if (!unk198)
		return false;

	if ((isOnWall() && SMS_IsMarioFencing()
	     && isSameNormal(SMS_GetMarioWlPlane(), unk198))
	    || (isOnRoof() && SMS_IsMarioRoofing()
	        && isSameNormal(SMS_GetMarioRfPlane(), unk198))
	    || (isOnFloor() && isSameNormal(SMS_GetMarioGrPlane(), unk198)))
		return true;

	return false;
}

bool TKumokun::isOnFloor() const
{
	return 0.7f < getPlaneNormal().dot(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f));
}

bool TKumokun::isOnRoof() const
{
	return getPlaneNormal().dot(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f))
	       < -0.7f;
}

bool TKumokun::isWalking() const
{
	return mSpine->getLatestNerve() == &TNerveKumokunWalk::theNerve();
}

bool TKumokun::isAttack() const
{
	TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();

	if (isOnRoof() && SMS_GetMarioPos().y + 5.0f > mPosition.y)
		return false;

	return nerve == &TNerveKumokunWalk::theNerve()
	       || nerve == &TNerveKumokunSearch::theNerve()
	       || nerve == &TNerveKumokunWait::theNerve()
	       || nerve == &TNerveKumokunPreWalk::theNerve()
	       || nerve == &TNerveKumokunPostWalk::theNerve();
}

bool TKumokun::isFlying() const
{
	return mSpine->getLatestNerve() == &TNerveKumokunFly::theNerve();
}

bool TKumokun::isCrashing() const { }

bool TKumokun::isHitPlane() const { return mHitPlaneCounter > 0; }

bool TKumokun::isFindOutMario(JGeometry::TVec3<f32>* param_1) const
{
	if (!isOnSamePlaneWithMario())
		return false;

	param_1->set(rotateGoalDirToLocal(SMS_GetMarioPos()));

	f32 range = getSaveParam2()->mSearchRange.get();

	f32 fVar2 = mHeadHeight;
	if (isOnRoof())
		fVar2 *= 3.0f;

	return abs(param_1->y) < fVar2 && param_1->squared() < range * range;
}

bool TKumokun::doKeepDistance()
{
	return isOnSamePlaneWithMario() && !isAttack();
}

bool TKumokun::isCollidMove(THitActor*) { return false; }

void TKumokun::resetHitPlaneCounter() { mHitPlaneCounter = 0; }

JGeometry::TVec3<f32> TKumokun::getPlaneNormal() const
{
	if (unk198)
		return unk198->getNormal();
	return JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
}

const TBGCheckData* TKumokun::checkWallPlane(JGeometry::TVec3<f32>* param_1,
                                             f32 param_2, f32 param_3)
{
	TBGWallCheckRecord record(param_1->x, param_1->y + param_2, param_1->z,
	                          param_3, 1, 0);

	const TBGCheckData* wall = nullptr;
	if (gpMap->isTouchedWallsAndMoveXZ(&record))
		wall = record.mResultWalls[0];

	param_1->x = record.mCenter.x;
	param_1->z = record.mCenter.z;

	return wall;
}

const TBGCheckData* TKumokun::checkFloorPlane(JGeometry::TVec3<f32>* param_1,
                                              f32 param_2, f32)
{
	const TBGCheckData* floor = nullptr;

	f32 yTmp2 = param_1->y;
	f32 fVar7
	    = gpMap->checkGround(param_1->x, yTmp2 + param_2, param_1->z, &floor);
	fVar7 += 1.0f;
	if (yTmp2 <= fVar7 + 0.05f) {
		param_1->y = fVar7;
	} else {
		floor = nullptr;
		fVar7 = param_1->y;
	}
	return floor;
}

const TBGCheckData* TKumokun::checkRoofPlane(JGeometry::TVec3<f32>* param_1,
                                             f32 param_2)
{
	const TBGCheckData* roof = nullptr;
	f32 y                    = param_1->y;
	f32 dVar6 = gpMap->checkRoof(param_1->x, y, param_1->z, &roof);
	f32 fVar8 = dVar6 - 1.0f - y;
	if (0.0f <= fVar8 && fVar8 < param_2)
		param_1->y = fVar8;
	else
		roof = nullptr;

	return roof;
}

bool TKumokun::isSameNormal(const TBGCheckData* param_1,
                            const TBGCheckData* param_2)
{
	if (!param_1 || !param_2)
		return false;

	if (0.99f <= param_1->getNormal().dot(param_2->getNormal()))
		return true;

	return false;
}

bool TKumokun::isFenceSound(const TBGCheckData* wall)
{
	if (wall == nullptr)
		return false;

	if (wall->isFence()) {
		if (gpMarDirector->mMap == 8)
			return false;
	} else {
		return false;
	}

	return true;
}

TKumokunParams::TKumokunParams(const char* path)
    : TSmallEnemyParams(path)
    , PARAM_INIT(mTorqueY, 0.01f)
    , PARAM_INIT(mMarchSpeed, 1.0f)
    , PARAM_INIT(mAttackSpeed, 1.5f)
    , PARAM_INIT(mMarchTimer, 600)
    , PARAM_INIT(mWaitTimer, 180)
    , PARAM_INIT(mSearchRange, 1000.0f)
    , PARAM_INIT(mFlySpeed, 30.0f)
{
	TParams::load(mPrmPath);
}

TKumokunManager::TKumokunManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TKumokunManager::load(JSUMemoryInputStream& stream)
{
	TKumokunParams* params = new TKumokunParams("/enemy/kumokun.prm");

	unk38 = params;

	params->mSLAttackRadius.set(60);
	params->mSLAttackRadius.set(50);
	params->mSLDamageRadius.set(60);
	params->mSLDamageHeight.set(70);
	TSmallEnemyManager::load(stream);
}

void TKumokunManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "kumo_model1.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

DEFINE_NERVE(TNerveKumokunPreWalk, TLiveActor)
{
	TKumokun* self = (TKumokun*)spine->getBody();
	if (spine->getTime() == 0)
		self->changeBck("kumo_run1_start");

	if (self->checkSerialAnmEnd()) {
		spine->pushAfterCurrent(&TNerveKumokunWalk::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveKumokunWalk, TLiveActor)
{
	TKumokun* self = (TKumokun*)spine->getBody();
	if (spine->getTime() == 0) {
		self->prepareWalk();
	}

	if ((self->doWalk()
	     || self->getSaveParam2()->mMarchTimer.get() < spine->getTime())
	    && self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveKumokunPostWalk::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveKumokunPostWalk, TLiveActor)
{
	TKumokun* self = (TKumokun*)spine->getBody();
	if (spine->getTime() == 0)
		self->changeBck("kumo_run1_end");

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveKumokunWait::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveKumokunSearch, TLiveActor)
{
	TKumokun* self = (TKumokun*)spine->getBody();

	if (spine->getTime() == 0) {
		self->decideTarget();

		if (self->unk1D4) {
			self->pushNextAnm("kumo_turn1_loop", true);
		} else {
			self->pushNextAnm("kumo_turn1_loop", false);
			self->pushNextAnm("kumo_turn1_start", true);
		}
	}

	if (self->doAdjustTarget() && self->checkCurAnmEnd(0)) {
		if (self->getMActor()->checkCurAnm("kumo_turn1_loop", 0)) {
			self->changeBck("kumo_turn1_end");
		} else {
			spine->pushAfterCurrent(&TNerveKumokunPreWalk::theNerve());
			return true;
		}
	}

	return false;
}

DEFINE_NERVE(TNerveKumokunWait, TLiveActor)
{
	TKumokun* self = (TKumokun*)spine->getBody();

	if (spine->getTime() == 0) {
		self->unk1D4 = false;
		self->changeBck("kumo_wait1");
	}

	JGeometry::TVec3<f32> dummy;
	if (self->isFindOutMario(&dummy)) {
		spine->pushAfterCurrent(&TNerveKumokunSearch::theNerve());
		return true;
	}

	if (self->checkCurAnmEnd(0)
	    && self->getSaveParam2()->mWaitTimer.get() < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveKumokunSearch::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveKumokunFreeze, TLiveActor)
{
	TKumokun* self = (TKumokun*)spine->getBody();
	if (spine->getTime() == 0) {
		self->clearAnmStack();
		if (self->getMActor()->checkCurAnm("kumo_hit_end", 0)
		    || self->getMActor()->checkCurAnm("kumo_hit_start", 0)) {
			self->changeBck("kumo_hit_loop");
		} else if (!self->getMActor()->checkCurAnm("kumo_hit_loop", 0)) {
			self->pushNextAnm("kumo_hit_loop", false);
			self->pushNextAnm("kumo_hit_start", true);
		}
	}

	if (self->unk1D0 <= 0) {
		spine->pushAfterCurrent(&TNerveKumokunPostFreeze::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveKumokunPostFreeze, TLiveActor)
{
	TKumokun* self = (TKumokun*)spine->getBody();
	if (spine->getTime() == 0)
		self->changeBck("kumo_hit_end");

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveKumokunSearch::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveKumokunFly, TLiveActor)
{
	TKumokun* self = (TKumokun*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck("kumo_flying1_loop");
		self->prepareFly();
	}

	if (self->doFly() || spine->getTime() > 1800) {
		spine->pushAfterCurrent(&TNerveSmallEnemyDie::theNerve());
		self->unk1EC = 1;
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveKumokunPreFly, TLiveActor)
{
	TKumokun* self = (TKumokun*)spine->getBody();
	if (spine->getTime() == 0) {
		self->clearAnmStack();
		self->pushNextAnm("kumo_hit_start", true);
	}

	if (self->checkSerialAnmEnd()) {
		spine->pushAfterCurrent(&TNerveKumokunFly::theNerve());
		return true;
	}

	return false;
}
