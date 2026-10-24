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

namespace {

const char cMatName[7]      = "_mat_1";
const GXColorS10 cRedColor  = { 210, 20, 15, 0 };
const GXColorS10 cBlueColor = { 15, 20, 210, 0 };

inline void SMSReportVec(const char*, const JGeometry::TVec3<float>&)
{
	// @UNUSED INLINE???
}

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

void TWireTrap::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);

	s32 param1;
	stream.read(&param1, sizeof(param1));
	s32 fInitialMode;
	stream.read(&fInitialMode, sizeof(fInitialMode));
	stream.read(&mTargetValue, sizeof(mTargetValue));
	s32 isBlue;
	stream.read(&isBlue, sizeof(isBlue));

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

	const TNerveBase<TLiveActor>* nerve = getNerveFromMode(mInitialMode);
	mSpine->setNext(nerve);

	// probs inlined to be emitted like this
	JGeometry::TVec3<f32> actorDir(MsCos(getRotation().y), 0.0f,
	                               MsSin(getRotation().y));
	mWireDirectionMultiplier
	    = actorDir.dot(getWireDir()) >= 0.0f ? 1.0f : -1.0f;

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
	u16 matIdx
	    = getModel()->getModelData()->getMaterialName()->getIndex(cMatName);
	SMS_InitPacket_OneTevColor(getModel(), matIdx, GX_TEVREG2, color);
}

BOOL TWireTrap::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_TRAMPLE:
	case HIT_MESSAGE_HIP_DROP:
		return false;

	case HIT_MESSAGE_SPRAYED_BY_WATER: {
		SMS_EasyEmitParticle(PARTICLE_MS_ENM_WATHIT, &sender->getPosition(),
		                     nullptr, JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, &mPosition, 0,
		                              0.0f, 0, 0, 4);
		mHitCooldownTimer = 30;

		JGeometry::TVec3<f32> diff = mPosition;
		diff -= SMS_GetMarioPos();

		const JGeometry::TVec3<f32>& wireDir = getWireDir();
		f32 dot = diff.dot(wireDir * mWireDirectionMultiplier);

		if (0.0f <= dot) {
			mHitCooldownSpeed = getWaterPow();
		} else {
			mHitCooldownSpeed = -getWaterPow();
		}
		return true;
	}

	case HIT_MESSAGE_ATTACH: {
		if (isTaken()) {
			break;
		}
		onHitFilter(HIT_FILTER_NO_COLLISION);
		mHolder = (TTakeActor*)sender;
		return true;
	}

	case HIT_MESSAGE_PUT: {
		if (!isTaken()) {
			break;
		}
		mHolder = nullptr;
		return true;
	}

	case HIT_MESSAGE_THROWN: {
		kill();
		return true;
	}

	default:
		break;
	}
	return TSpineEnemy::receiveMessage(sender, message);
}

void TWireTrap::behaveHitWater(THitActor* actor) { }

void TWireTrap::kill()
{
	if (!checkLiveFlag(LIVE_FLAG_DEAD)) {
		onLiveFlag(LIVE_FLAG_DEAD);
		onHitFilter(HIT_FILTER_NO_COLLISION);
		onHitFilter(HIT_FILTER_NO_COLLISION);

		mSpine->setNext(&TNerveWaitForever<TLiveActor>::theNerve());

		SMS_EasyEmitParticle(PARTICLE_MS_ENM_DISAP_A, &mPosition, this,
		                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		SMS_EasyEmitParticle(PARTICLE_MS_ENM_DISAP_B, &mPosition, this,
		                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	}
}

void TWireTrap::behaveHitWireTrap(TWireTrap* otherTrap,
                                  const JGeometry::TVec3<float>& dir,
                                  const JGeometry::TVec3<float>& otherDir)
{
	if (otherTrap->unk16C <= 0
	    && otherTrap->mInitialMode == WIRETRAP_MODE_RETURN_MOVE) {
		otherTrap->unk16C = 30;

		if (dir.dot(otherDir) < 0.0f) {
			otherTrap->mWireDirectionMultiplier *= -1.0f;
		}

		otherTrap->mSpine->pushNerve(&TNerveWireTrapWait::theNerve());
	}
}

void TWireTrap::calcRootMatrix()
{
	if (isTaken()) {
		TSpineEnemy::calcRootMatrix();
		return;
	}

	emitEffects();

	// Point argument is inferred; see open items.
	JGeometry::TVec3<f32> wireDir
	    = getWireBinder()->getDirAtPos(mPosition, mWireDirectionMultiplier);

	JGeometry::TVec3<f32> scaledDir(wireDir.x * mModelScaleMultiplier,
	                                wireDir.y * mModelScaleMultiplier,
	                                wireDir.z * mModelScaleMultiplier);

	JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
	TPosition3f matrix;
	SMS_CalcToDirMatrix(matrix, scaledDir, up);

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
			mWireDirectionMultiplier = 0.0f;
		}
	}

	updateCollision();

	checkHitActors();
	TLiveActor::moveObject();

	SMSGetMSound()->startSoundActor(MSD_SE_EN_WIRETRAP_WAIT, &mPosition, 0,
	                                nullptr, 0, 4);
}

void TWireTrap::calcMomentum() { mPositionDelta = getDirAtWirePos(); }

f32 TWireTrap::getWaterPow() const
{
	return getDataParams()->mInWaterPowerRate.get();
}

bool TWireTrap::doReturnMove()
{
	if (unk16C > 0) {
		unk16C--;
	}

	if (getWireBinder()->isEndWire(mPosition, mWireDirectionMultiplier)) {
		mWireDirectionMultiplier *= -1.0f;
		return true;
	}

	calcMomentum();
	return false;
}

bool TWireTrap::doOnewayMove()
{
	if (unk16C > 0) {
		unk16C--;
	}

	if (getWireBinder()->isEndWire(mPosition, mWireDirectionMultiplier)) {
		return true;
	}

	calcMomentum();
	return false;
}

bool TWireTrap::doSearchMove()
{
	if (!SMS_IsMarioOnWire()) {
		return false;
	}

	if (unk16C > 0) {
		unk16C--;
	}

	JGeometry::TVec3<f32> diff = SMS_GetMarioPos();
	diff -= mPosition;

	const JGeometry::TVec3<f32>& wireDir = getWireBinder()->getDir();
	f32 dot                              = diff.dot(wireDir);

	s32 dirMultiplier = 0;
	if (dot > 0.0f) {
		dirMultiplier = 1;
	} else if (dot < 0.0f) {
		dirMultiplier = -1;
	}

	mWireDirectionMultiplier = dirMultiplier;
	calcMomentum();
	return true;
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
	f32 baseOffset = (mWireDirectionMultiplier > 0.0f) ? 0.0f : 1.0f;
	getWireBinder()->getPoint(&mPosition,
	                          (0.01f * mWireDirectionMultiplier) + baseOffset);
}

#pragma dont_inline on
void TWireTrap::checkHitActors()
{
	THitActor** end = mCollisions + mColCount;
	for (THitActor** it = mCollisions; it != end; ++it) {
		switch ((*it)->getActorType()) {
		case ACTOR_TYPE_MARIO:
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
			break;
		case ACTOR_TYPE_WIRE_TRAP: {
			TWireTrap* otherTrap = (TWireTrap*)*it;
			if (otherTrap != this) {
				behaveHitWireTrap(otherTrap, otherTrap->getDirAtWirePos(),
				                  getDirAtWirePos());
				behaveHitWireTrap(this, getDirAtWirePos(),
				                  otherTrap->getDirAtWirePos());
			}
			break;
		}
		}
	}
}
#pragma dont_inline off

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

void TWireTrap::setMoveMode(int arg0) { }

const JGeometry::TVec3<f32> TWireTrap::getDirAtWirePos() const
{
	JGeometry::TVec3<f32> dir = getWireDir();

	f32 speedScale = 1.0f;
	if (mHitCooldownTimer > 0) {
		speedScale
		    = 1.0f + ((mHitCooldownSpeed * (f32)mHitCooldownTimer) / 30.0f);
	}

	dir *= mWireDirectionMultiplier;
	dir *= unk17C;
	return dir;
}

const JGeometry::TVec3<f32>& TWireTrap::getWireDir() const
{
	return getWireBinder()->getDir();
}

bool TWireTrap::isReflect() const { return false; }

#pragma dont_inline on
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
#pragma dont_inline off

f32 TWireTrap::getRangePosInWire() const
{
	return getWireBinder()->getRangePos(mPosition);
}

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
		spine->setNext(spine->getDefault());
		spine->setNext(&TNerveWireTrapWait::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveWireTrapOnewayMoveStart, TLiveActor)
{
	TWireTrap* actor = (TWireTrap*)spine->getBody();
	return actor->doScaleUp();
}

DEFINE_NERVE(TNerveWireTrapOnewayMove, TLiveActor)
{
	TWireTrap* actor = (TWireTrap*)spine->getBody();

	if (spine->getTime() == 0 && actor->mModelScaleMultiplier < 1.0f) {
		spine->setNext(spine->getDefault());
		spine->setNext(&TNerveWireTrapGoWait::theNerve());
		spine->setNext(&TNerveWireTrapOnewayMoveStart::theNerve());
		return true;
	}

	if (actor->doOnewayMove()) {
		spine->setNext(spine->getDefault());
		spine->setNext(&TNerveWireTrapWait::theNerve());
		spine->setNext(&TNerveWireTrapOnewayMoveEnd::theNerve());
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
		spine->setNext(spine->getDefault());
		spine->setNext(&TNerveWireTrapWait::theNerve());
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
