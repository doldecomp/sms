#include <Enemy/Enemy.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>
#include <Strategic/Spine.hpp>
#include "Enemy/Launcher.hpp"
#include "JSystem/J3D/J3DGraphAnimator/J3DModel.hpp"
#include "JSystem/JGeometry/JGVec3.hpp"
#include "JSystem/JUtility/JUTNameTab.hpp"
#include "MSound/MSound.hpp"
#include "MarioUtil/MathUtil.hpp"
#include "MarioUtil/MtxUtil.hpp"
#include "MarioUtil/PacketUtil.hpp"
#include "Player/MarioAccess.hpp"
#include "Strategic/ActorTypes.hpp"
#include "Strategic/LiveActor.hpp"
#include "Strategic/Nerve.hpp"
#include "Strategic/ObjModel.hpp"
#include "Strategic/TakeActor.hpp"
#include "System/Particles.hpp"
#include "dolphin/gx/GXEnum.h"
#include "dolphin/mtx.h"
#include "macros.h"
#include <Enemy/WireTrap.hpp>
#include <Enemy/WireBinder.hpp>
#include <M3DUtil/MActorAnm.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

namespace {

const char cMatName[7]      = "_mat_1";
const GXColorS10 cRedColor  = { 210, 20, 15, 0 };
const GXColorS10 cBlueColor = { 15, 20, 210, 0 };

void SMSReportVec(const char*, const JGeometry::TVec3<float>&) { }

} // namespace

TWireTrap::TWireTrap(const char* name)
    : TSpineEnemy(name)
    , unk160(0)
    , mHitCooldownTimer(0)
{
}

void TWireTrap::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);

	initWire();
	initCollision();
	initParticle();
}

// TODO: fake
static inline JGeometry::TVec3<f32> fromPolar(f32 theta, f32 radius)
{
	return JGeometry::TVec3<f32>(radius * MsSin(theta), 0.0f,
	                             radius * MsCos(theta));
}

void TWireTrap::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);

	s32 param1;
	stream >> param1;
	s32 fInitialMode;
	stream >> fInitialMode;
	stream >> mTargetValue;
	s32 isBlue;
	stream >> isBlue;

	unk17C = param1 / 10.0f;
	if (fInitialMode == -1) {
		fInitialMode = WIRETRAP_MODE_RETURN_MOVE;
	}
	mInitialMode = fInitialMode;

	if (isBlue == -1) {
		initThisColor(&cRedColor);
	} else {
		initThisColor(&cBlueColor);
	}

	getWireBinder()->init(getPosition());

	setMoveMode(mInitialMode);

	mWireDirectionMultiplier
	    = 0.0f <= fromPolar(mRotation.y, 1.0f).dot(getWireDir()) ? 1.0f : -1.0f;

	mModelScaleMultiplier = 1.0f;
}

void TWireTrap::initCollision()
{
	initHitActor(ACTOR_TYPE_WIRE_TRAP, 2,
	             HIT_CATEGORY_PLAYER | HIT_CATEGORY_ENEMY, 20.0f, 30.0f, 40.0f,
	             40.0f);
	offHitFilter(HIT_FILTER_NO_COLLISION);
}

void TWireTrap::initParticle()
{
	SMS_LoadParticle("/scene/wireTrap/jpa/ms_wrt_biri_a.jpa",
	                 WIRETRAP_JPA_NS_WRT_BIRI_A);
	SMS_LoadParticle("/scene/wireTrap/jpa/ms_wrt_biri_b.jpa",
	                 WIRETRAP_JPA_NS_WRT_BIRI_B);
}

void TWireTrap::initWire()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("wire_trap.bmd", 0);
	mBinder       = new TWireBinder;
	// inline getter?
	const TNerveBase<TLiveActor>* pNerve
	    = &TNerveWaitForever<TLiveActor>::theNerve();
	mSpine->initWith(pNerve);
}

void TWireTrap::initThisColor(const GXColorS10* color)
{
	int matIdx
	    = getModel()->getModelData()->getMaterialName()->getIndex(cMatName);
	SMS_InitPacket_OneTevColor(getModel(), matIdx, GX_TEVREG2, color);
}

BOOL TWireTrap::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		behaveHitWater(sender);
		return true;

	case HIT_MESSAGE_TAKE:
		if (mHolder)
			break;
		onHitFilter(HIT_FILTER_NO_COLLISION);
		mHolder = (TTakeActor*)sender;
		return true;

	case HIT_MESSAGE_THROWN:
	case HIT_MESSAGE_DETACH:
		if (!mHolder)
			break;
		mHolder = nullptr;
		return true;

	case HIT_MESSAGE_UNKB:
		kill();
		return true;
	}
	return TSpineEnemy::receiveMessage(sender, message);
}

void TWireTrap::behaveHitWater(THitActor* actor)
{
	SMS_EasyEmitParticle(PARTICLE_MS_ENM_WATHIT, &mPosition, nullptr,
	                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, &mPosition, 0,
	                              0.0f, 0, 0, 4);
	mHitCooldownTimer = 30;

	JGeometry::TVec3<f32> diff = mPosition;
	diff -= SMS_GetMarioPos();

	JGeometry::TVec3<f32> dir = getWireDir();
	dir *= mWireDirectionMultiplier;

	if (0.0f <= diff.dot(dir)) {
		mHitCooldownSpeed = getWaterPow();
	} else {
		mHitCooldownSpeed = -getWaterPow();
	}
}

void TWireTrap::kill()
{
	if (!checkLiveFlag(LIVE_FLAG_DEAD)) {
		onLiveFlag(LIVE_FLAG_DEAD);
		onHitFilter(HIT_FILTER_NO_COLLISION);

		mSpine->reset();
		mSpine->setNext(&TNerveWaitForever<TLiveActor>::theNerve());

		SMS_EasyEmitParticle(PARTICLE_MS_ENM_DISAP_A, &mPosition, nullptr,
		                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		SMS_EasyEmitParticle(PARTICLE_MS_ENM_DISAP_B, &mPosition, nullptr,
		                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	}
}

void TWireTrap::behaveHitWireTrap(TWireTrap* otherTrap,
                                  const JGeometry::TVec3<float>& dir,
                                  const JGeometry::TVec3<float>& otherDir)
{
	if (!isReflect() && mInitialMode == WIRETRAP_MODE_RETURN_MOVE) {
		unk16C = 30;
		SMSReportVec("", calcMomentum());
		SMSReportVec("", otherTrap->calcMomentum());

		if (dir.dot(otherDir) < 0.0f) {
			mWireDirectionMultiplier *= -1.0f;
		}

		mSpine->pushNerve(&TNerveWireTrapWait::theNerve());
	}
}

void TWireTrap::calcRootMatrix()
{
	if (mHolder) {
		TSpineEnemy::calcRootMatrix();
		return;
	}

	emitEffects();

	JGeometry::TVec3<f32> dir = getDirAtWirePos();
	dir.scale(mWireDirectionMultiplier);

	JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
	TPosition3f matrix;
	SMS_CalcToDirMatrix(matrix, dir, up);

	JGeometry::TQuat4<f32> quat;
	matrix.getQuat(quat);

	// Spin about the wire. The angle is the float at +0x38 on the base.
	mRotation.z += -17.75f;
	mRotation.z = std::fmodf(360.0f + mRotation.z, 360.0f);
	f32 halfRad = 0.5f * DEG_TO_RAD(mRotation.z);
	f32 s       = sinf(halfRad);
	f32 c       = cosf(halfRad);

	// quat * (0, 0, s, c), expanded.
	f32 rx = quat.x * c + quat.y * s;
	f32 ry = quat.y * c - quat.x * s;
	f32 rz = quat.z * c + quat.w * s;
	f32 rw = quat.w * c - quat.z * s;

	matrix.mMtx[0][0] = 1.0f - 2.0f * ry * ry - 2.0f * rz * rz;
	matrix.mMtx[0][1] = 2.0f * rx * ry - 2.0f * rw * rz;
	matrix.mMtx[0][2] = 2.0f * rx * rz + 2.0f * rw * ry;
	matrix.mMtx[1][0] = 2.0f * rx * ry + 2.0f * rw * rz;
	matrix.mMtx[1][1] = 1.0f - 2.0f * rx * rx - 2.0f * rz * rz;
	matrix.mMtx[1][2] = 2.0f * ry * rz - 2.0f * rw * rx;
	matrix.mMtx[2][0] = 2.0f * rx * rz - 2.0f * rw * ry;
	matrix.mMtx[2][1] = 2.0f * ry * rz + 2.0f * rw * rx;
	matrix.mMtx[2][2] = 1.0f - 2.0f * rx * rx - 2.0f * ry * ry;

	matrix.mMtx[0][3] = mPosition.x;
	matrix.mMtx[1][3] = mPosition.y;
	matrix.mMtx[2][3] = mPosition.z;

	MTXCopy(matrix.mMtx, getModel()->getBaseTRMtx());

	JGeometry::TVec3<f32> baseScale = mScaling;
	baseScale *= mModelScaleMultiplier;
	getModel()->setBaseScale(baseScale);
}

void TWireTrap::moveObject()
{
	if (mHitCooldownTimer > 0) {
		mHitCooldownTimer -= 1;
		if (mHitCooldownTimer <= 0) {
			mHitCooldownSpeed = 0.0f;
		}
	}

	updateCollision();

	checkHitActors();
	TLiveActor::moveObject();

	SMSGetMSound()->startSoundActor(MSD_SE_EN_WIRETRAP_WAIT, &mPosition, 0,
	                                nullptr, 0, 4);
}

const JGeometry::TVec3<f32> TWireTrap::calcMomentum()
{
	JGeometry::TVec3<f32> dir = getWireDir();

	f32 speedScale
	    = mHitCooldownTimer > 0
	          ? 1.0f + ((mHitCooldownSpeed * (f32)mHitCooldownTimer) / 30.0f)
	          : 1.0f;

	dir *= mWireDirectionMultiplier * speedScale;
	dir *= unk17C;
	return dir;
}

f32 TWireTrap::getWaterPow() const
{
	return getDataParams()->mInWaterPowerRate.get();
}

bool TWireTrap::doReturnMove()
{
	if (isReflect()) {
		unk16C--;
	}

	if (isEndWire()) {
		mWireDirectionMultiplier *= -1.0f;
		return true;
	}

	mPositionDelta = calcMomentum();
	return false;
}

bool TWireTrap::doOnewayMove()
{
	if (isReflect()) {
		unk16C--;
	}

	if (isEndWire()) {
		return true;
	}

	mPositionDelta = calcMomentum();
	return false;
}

bool TWireTrap::doSearchMove()
{
	if (!SMS_IsMarioOnWire()) {
		return false;
	}

	if (isReflect()) {
		unk16C--;
	}

	JGeometry::TVec3<f32> diff = SMS_GetMarioPos();
	diff -= mPosition;

	f32 dot                  = diff.dot(getWireBinder()->getDir());
	mWireDirectionMultiplier = dot > 0.0f ? 1 : (dot < 0.0f ? -1 : 0);
	mPositionDelta           = calcMomentum();
	return false;
}

bool TWireTrap::doScaleUp()
{
	onHitFilter(HIT_FILTER_NO_COLLISION);
	mModelScaleMultiplier += 1.0f / getDataParams()->mScaleTimerMax.get();
	if (1.0f <= mModelScaleMultiplier) { // operand order from the fcmpo
		mModelScaleMultiplier = 1.0f;
		offHitFilter(HIT_FILTER_NO_COLLISION);
		return true;
	}
	return false;
}
bool TWireTrap::doScaleDown()
{
	onHitFilter(HIT_FILTER_NO_COLLISION);
	mModelScaleMultiplier -= 1.0f / getDataParams()->mScaleTimerMax.get();
	if (mModelScaleMultiplier <= 0.0f) {
		mModelScaleMultiplier = 0.0f;
		offHitFilter(HIT_FILTER_NO_COLLISION);
		return true;
	}
	return false;
}
void TWireTrap::doResetToEdge()
{
	f32 baseOffset = 0.0f < mWireDirectionMultiplier ? 0.0f : 1.0f;
	getWireBinder()->getPoint(&mPosition,
	                          (0.01f * mWireDirectionMultiplier) + baseOffset);
}

void TWireTrap::checkHitActors()
{
	THitActor** end = mCollisions + mColCount;
	for (THitActor** it = mCollisions; it != end; ++it) {
		switch ((*it)->getActorType()) {
		case ACTOR_TYPE_MARIO:
			SMS_SendMessageToMario(this, HIT_MESSAGE_ELECTRIC_SHOCK);
			break;
		case ACTOR_TYPE_WIRE_TRAP: {
			TWireTrap* otherTrap = (TWireTrap*)*it;
			if (otherTrap == this)
				break;

			const JGeometry::TVec3<f32> dir      = calcMomentum();
			const JGeometry::TVec3<f32> otherDir = otherTrap->calcMomentum();
			behaveHitWireTrap(otherTrap, dir, otherDir);
			otherTrap->behaveHitWireTrap(otherTrap, otherDir, dir);
			break;
		}
		}
	}
}

void TWireTrap::updateCollision()
{
	setHitParams(20.0f * mScaling.x, 30.0f * mScaling.y, 40.0f * mScaling.x,
	             40.0f * mScaling.y);
}
void TWireTrap::emitEffects()
{
	JPABaseEmitter* emitter1 = gpMarioParticleManager->emitAndBindToPosPtr(
	    WIRETRAP_JPA_NS_WRT_BIRI_A, &mPosition, 1, this);
	if (emitter1)
		emitter1->setGlobalScale(mScaling);

	JPABaseEmitter* emitter2 = gpMarioParticleManager->emitAndBindToPosPtr(
	    WIRETRAP_JPA_NS_WRT_BIRI_B, &mPosition, 1, this);
	if (emitter2)
		emitter2->setGlobalScale(mScaling);
}

void TWireTrap::setMoveMode(int mode)
{
	mSpine->reset();
	mSpine->setNext(getNerveFromMode(mode));
}

const JGeometry::TVec3<f32> TWireTrap::getDirAtWirePos() const
{
	return getWireBinder()->getDirAtPos(mPosition, mWireDirectionMultiplier);
}

const JGeometry::TVec3<f32>& TWireTrap::getWireDir() const
{
	return getWireBinder()->getDir();
}

bool TWireTrap::isReflect() const { return unk16C > 0; }

const TNerveBase<TLiveActor>* TWireTrap::getNerveFromMode(int mode)
{
	switch (mode) {
	case WIRETRAP_MODE_RETURN_MOVE:
		return &TNerveWireTrapReturnMove::theNerve();
	case WIRETRAP_MODE_ONEWAY_MOVE:
		return &TNerveWireTrapOnewayMove::theNerve();
	case WIRETRAP_MODE_SEARCH:
		return &TNerveWireTrapSearch::theNerve();
	default:
		return nullptr;
	}
}

f32 TWireTrap::getRangePosInWire() const
{
	return getWireBinder()->getRangePos(mPosition);
}

TWireBinder* TWireTrap::getWireBinder() { return (TWireBinder*)mBinder; }

TWireBinder* TWireTrap::getWireBinder() const { return (TWireBinder*)mBinder; }

bool TWireTrap::isStartWire() const
{
	return getWireBinder()->isStartWire(mPosition, mWireDirectionMultiplier);
}

bool TWireTrap::isEndWire() const
{
	return getWireBinder()->isEndWire(mPosition, mWireDirectionMultiplier);
}

TWireTrapParams::TWireTrapParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mInWaterPowerRate, 0.9f)
    , PARAM_INIT(mScaleTimerMax, 60)
    , PARAM_INIT(mGoTimerMax, 90)
{
	TParams::load(mPrmPath);
}

TWireTrapManager::TWireTrapManager(const char* name)
    : TEnemyManager(name)
{
}

void TWireTrapManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TWireTrapParams("/enemy/wiretrap.prm");
	TEnemyManager::load(stream);
}

void TWireTrapManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "wire_trap.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_MaterialTexGenFull
		      | J3DMLF_UseUniqueMaterials | (4 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

DEFINE_NERVE(TNerveWireTrapReturnMove, TLiveActor)
{
	TWireTrap* actor = (TWireTrap*)spine->getBody();
	if (actor->doReturnMove()) {
		spine->pushAfterCurrent(this);
		spine->pushAfterCurrent(&TNerveWireTrapWait::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveWireTrapOnewayMoveStart, TLiveActor)
{
	TWireTrap* actor = (TWireTrap*)spine->getBody();
	if (actor->doScaleUp())
		return true;
	return false;
}

DEFINE_NERVE(TNerveWireTrapOnewayMove, TLiveActor)
{
	TWireTrap* actor = (TWireTrap*)spine->getBody();

	if (spine->getTime() == 0 && actor->mModelScaleMultiplier < 1.0f) {
		spine->pushAfterCurrent(this);
		spine->pushAfterCurrent(&TNerveWireTrapGoWait::theNerve());
		spine->pushAfterCurrent(&TNerveWireTrapOnewayMoveStart::theNerve());
		return true;
	}

	if (actor->doOnewayMove()) {
		spine->pushAfterCurrent(this);
		spine->pushAfterCurrent(&TNerveWireTrapWait::theNerve());
		spine->pushAfterCurrent(&TNerveWireTrapOnewayMoveEnd::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveWireTrapOnewayMoveEnd, TLiveActor)
{
	TWireTrap* actor = (TWireTrap*)spine->getBody();
	if (actor->doScaleDown()) {
		actor->doResetToEdge();
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveWireTrapSearch, TLiveActor)
{
	TWireTrap* actor = (TWireTrap*)spine->getBody();
	if (actor->doSearchMove()) {
		spine->pushAfterCurrent(this);
		spine->pushAfterCurrent(&TNerveWireTrapWait::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveWireTrapWait, TLiveActor)
{
	TWireTrap* actor = (TWireTrap*)spine->getBody();

	if (actor->mTargetValue < spine->getTime()) {
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveWireTrapGoWait, TLiveActor)
{
	TWireTrap* actor = (TWireTrap*)spine->getBody();

	if (actor->getDataParams()->mGoTimerMax.get() < spine->getTime()) {
		return true;
	}
	return false;
}
