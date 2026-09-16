#include <Enemy/TobiPuku.hpp>
#include <Strategic/Spine.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Map/MapData.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Player/MarioAccess.hpp>
#include <Enemy/PathNode.hpp>
#include <Enemy/Conductor.hpp>
#include <MoveBG/MapObjBlock.hpp>
#include <Enemy/EffectObj.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <System/Particles.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static const char* pukupuku_bastable[] = {
	nullptr,
	"/scene/pukupuku/bas/pukupuku_death.bas",
	"/scene/pukupuku/bas/pukupuku_down_air.bas",
	"/scene/pukupuku/bas/pukupuku_down_land.bas",
	nullptr,
	"/scene/pukupuku/bas/pukupuku_fall_end_land.bas",
	nullptr,
};

static const char* moepuku_bastable[] = {
	nullptr,
	nullptr,
	"/scene/moepuku/bas/moepuku_down_air.bas",
	"/scene/moepuku/bas/moepuku_down_land.bas",
	nullptr,
	"/scene/moepuku/bas/moepuku_fall_end_land.bas",
	nullptr,
};

// Animation slots shared by both variants.
enum {
	PUKU_ANM_ATTACK        = 0,
	PUKU_ANM_DEAD          = 1,
	PUKU_ANM_DOWN_AIR      = 2,
	PUKU_ANM_DOWN_LAND     = 3,
	PUKU_ANM_FALL          = 4,
	PUKU_ANM_FALL_END_LAND = 5,
	PUKU_ANM_JUMP          = 6,
	PUKU_ANM_JUMP_START    = 7,
	PUKU_ANM_PICHI         = 8,
	PUKU_ANM_SWIM          = 9,
};

// TODO: fabricated defaults; the real values come from the .prm file and the
// map records no constructor for this class, so they are unverified.
TTobiPukuParams::TTobiPukuParams(const char* prm)
    : TWalkerEnemyParams(prm)
    , PARAM_INIT(mBoundMax, 3)
    , PARAM_INIT(mBoundDamp, 0.5f)
    , PARAM_INIT(mPichiTime, 60)
    , PARAM_INIT(mFlyGravity, 0.5f)
    , PARAM_INIT(mHitWaterPush, 10.0f)
{
}

f32 TTobiPuku::mLandAngle;
u8 TTobiPuku::mBoundSw;
f32 TTobiPuku::mBoundVelocityY;
u8 TTobiPuku::mReturnLaunchSw;

TMoePuku* gpCurTobiPuku;

static int TobiPukuRollCallback(J3DNode* node, int param);


// TODO: 0% of 180 bytes despite initialising the right fields. The original
// default-constructs mLandPos and mLandDelta through __construct_array with a
// count of 2, and keeps `this` in a stack slot across the base call; ours
// inlines both TVec3 constructors instead. The field set and their values are
// confirmed by the assembly, so only the construction form is wrong.
TTobiPuku::TTobiPuku(const char* name)
    : TWalkerEnemy(name)
{
	unk194   = 0;
	mBoundCount = 0;
	unk19C   = nullptr;
	unk1AC   = 1;
	unk1AD   = 1;
	unk1AE   = 0;
	unk1B0   = 0.0f;
	mLaunchAngle = 0.0f;
	mSwimBaseY      = 0.0f;
	mFlyVelocityY   = 0.0f;
	mReturnPitchStep = 0.0f;
	unk1EC          = 0.0f;
	gpCurTobiPuku   = nullptr;
}

TTobiPukuManager::TTobiPukuManager(const char* name)
    : TSmallEnemyManager(name)
{
}

TTobiPukuLaunchPadManager::TTobiPukuLaunchPadManager(const char* name)
    : TSmallEnemyManager(name)
{
	unk60 = 0;
}

TPukuPuku::TPukuPuku(const char* name)
    : TTobiPuku(name)
{
}

TLiveActor* TTobiPukuManager::createEnemyInstance()
{
	return new TTobiPuku("とびプク");
}

TTobiPukuLaunchPad::TTobiPukuLaunchPad(const char* name)
    : TSmallEnemy(name)
{
	unk194 = 0;
	unk19C = 0.0f;
	unk1A8 = nullptr;
}

void TTobiPukuLaunchPad::init(TLiveManager* manager)
{
	TSmallEnemy::init(manager);
	mActorType = 0x10000012;
	unk198     = (TTobiPukuParams*)getSaveParam();
}

void TMoePukuLaunchPad::launch()
{
	TTobiPuku* puku = (TTobiPuku*)gpConductor->makeOneEnemyAppear(
	    mPosition, "モエプクマネージャー", 1);
	if (puku) {
		forceLaunch(puku);
		unk1A8 = puku;
	}
}

TLiveActor* TMoePukuManager::createEnemyInstance()
{
	return new TMoePuku("モエプク");
}

void TTobiPukuLaunchPad::reset()
{
	TSmallEnemy::reset();
	unk194 = 0;
	unk1A8 = nullptr;
}

TTobiPuku::~TTobiPuku() { }
TMoePuku::~TMoePuku() { }
TPukuPuku::~TPukuPuku() { }
TTobiPukuLaunchPad::~TTobiPukuLaunchPad() { }
TMoePukuLaunchPad::~TMoePukuLaunchPad() { }
TTobiPukuManager::~TTobiPukuManager() { }
TMoePukuManager::~TMoePukuManager() { }
TTobiPukuLaunchPadManager::~TTobiPukuLaunchPadManager() { }
TMoePukuLaunchPadManager::~TMoePukuLaunchPadManager() { }

void TTobiPuku::setAttackAnm() { setBckAnm(PUKU_ANM_ATTACK); }
void TTobiPuku::setDeadAnm() { setBckAnm(PUKU_ANM_DEAD); }
void TTobiPuku::setDownAirAnm() { setBckAnm(PUKU_ANM_DOWN_AIR); }
void TTobiPuku::setDownLandAnm() { setBckAnm(PUKU_ANM_DOWN_LAND); }
void TTobiPuku::setFallAnm() { setBckAnm(PUKU_ANM_FALL); }
void TTobiPuku::setFallEndLandAnm() { setBckAnm(PUKU_ANM_FALL_END_LAND); }
void TTobiPuku::setJumpAnm() { setBckAnm(PUKU_ANM_JUMP); }
void TTobiPuku::setJumpStartAnm()
{
	if (isBckAnm(PUKU_ANM_JUMP_START))
		setBckAnm(PUKU_ANM_JUMP_START);
}
void TTobiPuku::setPichiAnm() { setBckAnm(PUKU_ANM_PICHI); }
void TTobiPuku::setSwimAnm() { setBckAnm(PUKU_ANM_SWIM); }

bool TTobiPuku::isAttackBck() { return isBckAnm(PUKU_ANM_ATTACK) ? true : false; }
BOOL TTobiPuku::isDeadBck() { return isBckAnm(PUKU_ANM_DEAD) ? TRUE : FALSE; }
BOOL TTobiPuku::isFallEndLandBck()
{
	return isBckAnm(PUKU_ANM_FALL_END_LAND) ? TRUE : FALSE;
}
BOOL TTobiPuku::isJumpBck() { return isBckAnm(PUKU_ANM_JUMP) ? TRUE : FALSE; }
bool TTobiPuku::isJumpStartBck()
{
	return isBckAnm(PUKU_ANM_JUMP_START) ? true : false;
}
bool TTobiPuku::isPichiEffect() { return isBckAnm(PUKU_ANM_PICHI) ? true : false; }

void TMoePuku::setAttackAnm() { setBckAnm(PUKU_ANM_ATTACK); }
void TMoePuku::setDeadAnm() { setBckAnm(PUKU_ANM_DEAD); }
void TMoePuku::setDownAirAnm() { setBckAnm(PUKU_ANM_DOWN_AIR); }
void TMoePuku::setDownLandAnm() { setBckAnm(PUKU_ANM_DOWN_LAND); }
void TMoePuku::setFallAnm() { setBckAnm(PUKU_ANM_FALL); }
void TMoePuku::setFallEndLandAnm() { setBckAnm(PUKU_ANM_FALL_END_LAND); }
void TMoePuku::setJumpAnm() { setBckAnm(PUKU_ANM_JUMP); }
void TMoePuku::setJumpStartAnm()
{
	if (isBckAnm(PUKU_ANM_JUMP_START))
		setBckAnm(PUKU_ANM_JUMP_START);
}
void TMoePuku::setPichiAnm() { setBckAnm(PUKU_ANM_PICHI); }
void TMoePuku::setSwimAnm() { setBckAnm(PUKU_ANM_SWIM); }

bool TMoePuku::isAttackBck() { return isBckAnm(PUKU_ANM_ATTACK) ? true : false; }
BOOL TMoePuku::isDeadBck() { return isBckAnm(PUKU_ANM_DEAD) ? TRUE : FALSE; }
BOOL TMoePuku::isFallEndLandBck()
{
	return isBckAnm(PUKU_ANM_FALL_END_LAND) ? TRUE : FALSE;
}
BOOL TMoePuku::isJumpBck() { return isBckAnm(PUKU_ANM_JUMP) ? TRUE : FALSE; }
bool TMoePuku::isJumpStartBck()
{
	return isBckAnm(PUKU_ANM_JUMP_START) ? true : false;
}
bool TMoePuku::isPichiEffect() { return isBckAnm(PUKU_ANM_PICHI) ? true : false; }


// TODO: the nerve bodies below are not reconstructed. Each carries its map
// size; only their destructors, which the macro emits, match so far.

// TODO: incorrect size. Map records 0xb4 (180 bytes).
DEFINE_NERVE(TNerveTobiPukuSwimWander, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0) {
		puku->mSwimBaseY = puku->mPosition.y;
		puku->setSwimAnm();
		puku->initialGraphNode();
		puku->mLiveFlag |= LIVE_FLAG_UNK10;
	}

	if (puku->isReachedToGoalXZ())
		puku->goToRandomNextGraphNode();

	puku->walkBehavior(0, 1.5f);
	return FALSE;
}

// TODO: incorrect size. Map records 0x254 (596 bytes).
// TODO: 88.2% of 596 bytes. Structure and call order match. The remaining
// difference is that the original *calls* JGeometry::TVec3<f>::sub out of line
// where ours inlines its three fsubs; both use the same header inline, so what
// makes MWCC emit the call here has not been identified.
DEFINE_NERVE(TNerveTobiPukuReturnLaunch, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0) {
		TTobiPukuLaunchPad* pad = puku->mLaunchPad;
		TPathNode node(pad->mPosition);
		puku->unkF4  = node;
		puku->unk104 = node;
		puku->unk114.clear();
		puku->setSwimAnm();
		puku->mSwimBaseY = puku->mPosition.y;
	}

	puku->swimEffect();

	if (puku->isReachedToGoalXZ()) {
		spine->pushAfterCurrent(&TNerveTobiPukuPrepareFly::theNerve());
		return TRUE;
	}

	JGeometry::TVec3<f32> toPad(puku->mLaunchPad->mPosition);
	toPad.sub(puku->mPosition);

	JGeometry::TVec3<f32> dir(toPad);
	dir.y = 0.0f;
	MsVECNormalize(dir, dir);

	f32 speed = puku->mMarchSpeed;
	puku->mLaunchVelocity.x *= 0.99f;
	puku->mLaunchVelocity.z *= 0.99f;
	puku->mPosition.x += dir.x * speed - puku->mLaunchVelocity.x;
	puku->mPosition.z += dir.z * speed - puku->mLaunchVelocity.z;

	f32 spread = 1.0f + puku->unk1EC;
	if (spread > 180.0f)
		spread = 180.0f;
	else if (spread < 0.0f)
		spread = 0.0f;
	puku->unk1EC = spread;

	return FALSE;
}

// TODO: incorrect size. Map records 0x1a8 (424 bytes).
DEFINE_NERVE(TNerveTobiPukuPrepareFly, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0) {
		f32 angle = puku->mLaunchAngle;
		while (angle >= 360.0f)
			angle -= 360.0f;
		while (angle < 0.0f)
			angle += 360.0f;
		puku->mRotStep = (angle - puku->mRotation.x) / 60.0f;
	}

	f32 x = puku->mPosition.x;
	puku->mPosition.x
	    = (1.0f / 60.0f) * (puku->mLaunchPad->mPosition.x - x) + x;
	f32 y = puku->mPosition.y;
	puku->mPosition.y
	    = (1.0f / 60.0f) * (puku->mLaunchPad->mPosition.y - y) + y;
	f32 z = puku->mPosition.z;
	puku->mPosition.z
	    = (1.0f / 60.0f) * (puku->mLaunchPad->mPosition.z - z) + z;

	f32 spread = puku->unk1EC - 3.0f;
	if (spread > 180.0f)
		spread = 180.0f;
	else if (spread < 0.0f)
		spread = 0.0f;
	puku->unk1EC = spread;

	puku->mRotation.x += puku->mRotStep;

	if (spine->getTime() == 50.0f)
		puku->setJumpStartAnm();

	if (spine->getTime() > 60.0f) {
		puku->mLaunchPad->forceLaunch(puku);
		puku->reset();
	}
	return FALSE;
}

// TODO: incorrect size. Map records 0x1c8 (456 bytes).
DEFINE_NERVE(TNerveTobiPukuBound, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0) {
		puku->unk1AE = 1;
		int count    = puku->mBoundCount;
		if (count < puku->unk19C->mBoundMax.get()) {
			puku->mBoundCount = count + 1;

			f32 damp = puku->unk19C->mBoundDamp.get();
			JGeometry::TVec3<f32> vel(puku->mLaunchVelocity);
			vel.x *= damp;
			vel.z *= damp;
			vel.y = (TTobiPuku::mBoundVelocityY * damp
			         * (puku->unk1B0 - puku->mGroundHeight))
			        / 30.0f;

			puku->mLaunchVelocity = vel;
			puku->mVelocity       = vel;
			puku->onLiveFlag(LIVE_FLAG_AIRBORNE);
		}
	}

	JGeometry::TVec3<f32> vel(puku->mVelocity);
	if (vel.y > 0.0f)
		puku->unk1B0 = puku->mPosition.y;

	if (!puku->isAirborne()) {
		spine->pushAfterCurrent(&TNerveTobiPukuLand::theNerve());
		return TRUE;
	}
	return FALSE;
}

// TODO: incorrect size. Map records 0x5a8 (1448 bytes).
// TODO: partial, 54.5% of 1448 bytes. Three of the four branches are written:
// the water landing, the bound hand-off, and the fall-end-land interpolation
// over twenty frames. The fourth, taken when TTobiPuku::mReturnLaunchSw is set,
// is **not** written. It halves the launch velocity's x and z, recomputes y
// from mFlyVelocityY against the 600-unit drop, advances mRotation.x by
// mReturnPitchStep clamped to 0..180, scales the horizontal velocity by
// cos(pitch) through the jma table, and hands off once the drop exceeds 600.
// m2c's rendering of that block is ambiguous about which component receives
// which product, so it is left out rather than guessed.
DEFINE_NERVE(TNerveTobiPukuLand, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() < 2) {
		if (puku->mGroundPlane->isWaterSurface()) {
			f32 y             = puku->mPosition.y;
			puku->mPosition.y = y - 10.0f;
			puku->onLiveFlag(LIVE_FLAG_UNK10);
			puku->generateEffectColumWater();
			if (TTobiPuku::mReturnLaunchSw) {
				puku->mFlyVelocityY *= 0.8f;
				puku->mReturnPitchStep
				    = (180.0f - puku->mRotation.x)
				      / fabsf(600.0f / puku->mFlyVelocityY);
			}
			return FALSE;
		}

		if (TTobiPuku::mBoundSw) {
			if (puku->mBoundCount < puku->unk19C->mBoundMax.get()) {
				spine->pushAfterCurrent(&TNerveTobiPukuBound::theNerve());
				return TRUE;
			}
			puku->unk1AE = 0;
		}

		puku->mLandPos = puku->mPosition;
		puku->setFallEndLandAnm();
		puku->mRotation.x = 0.0f;
		return FALSE;
	}

	if (puku->isFallEndLandBck()) {
		if (spine->getTime() == 1) {
			puku->mLandDelta.x = puku->mPosition.x - puku->mLandPos.x;
			puku->mLandDelta.y = puku->mPosition.y - puku->mLandPos.y;
			puku->mLandDelta.z = puku->mPosition.z - puku->mLandPos.z;
		}

		int time = spine->getTime();
		if (time < 20) {
			f32 t             = 0.05f * (f32)time;
			puku->mPosition.x = puku->mLandPos.x;
			puku->mPosition.y = puku->mLandPos.y;
			puku->mPosition.z = puku->mLandPos.z;
			puku->mPosition.x += puku->mLandDelta.x * t;
			puku->mPosition.y += puku->mLandDelta.y * t;
			puku->mPosition.z += puku->mLandDelta.z * t;
		}

		if (puku->checkCurAnmEnd(0)) {
			spine->pushAfterCurrent(&TNerveTobiPukuPitiPiti::theNerve());
			return TRUE;
		}
		return FALSE;
	}

	f32 y             = puku->mPosition.y;
	puku->mPosition.y = y - 12.0f;

	if (puku->isJumpBck()) {
		f32 pitch = puku->mRotation.x;
		if (pitch < TTobiPuku::mLandAngle)
			puku->mRotation.x = pitch + 1.2f;
	}

	if (spine->getTime() > 100) {
		puku->onLiveFlag(LIVE_FLAG_DEAD);
		return TRUE;
	}
	return FALSE;
}

// TODO: incorrect size. Map records 0x1fc (508 bytes).
DEFINE_NERVE(TNerveTobiPukuDie, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0) {
		if (puku->isAirborne()) {
			puku->mHitFlags |= HIT_FLAG_NO_COLLISION;
			JGeometry::TVec3<f32> stop(0.0f, 0.0f, 0.0f);
			JGeometry::TVec3<f32> vel(puku->mVelocity);
			stop.y          = vel.y;
			puku->mVelocity = stop;
			puku->setDownAirAnm();
		} else if (puku->unk1AD != 0) {
			puku->mHitFlags |= HIT_FLAG_NO_COLLISION;
			puku->setDownLandAnm();
		} else {
			puku->onLiveFlag(LIVE_FLAG_UNK20000);
			puku->setDeadAnm();
		}
	}

	if (puku->checkCurAnmEnd(0)) {
		puku->onLiveFlag(LIVE_FLAG_DEAD);
		puku->onLiveFlag(LIVE_FLAG_UNK8);
		puku->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		puku->offLiveFlag(LIVE_FLAG_UNK20000);
		puku->mHolder = nullptr;
		puku->stopAnmSound();
		spine->reset();
		spine->setNext(&TNerveSmallEnemyDie::theNerve());
		spine->pushAfterCurrent(spine->getDefault());
		puku->mHitFlags |= HIT_FLAG_NO_COLLISION;
		puku->genRandomItem();
		return TRUE;
	}
	return FALSE;
}

// TODO: incorrect size. Map records 0x118 (280 bytes).
DEFINE_NERVE(TNerveTobiPukuPitiPiti, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0)
		puku->setPichiAnm();

	if (puku->checkCurAnmEnd(0)
	    && spine->getTime() > puku->getSaveParam2()->mPichiTime.get()) {
		puku->unk1AD = 0;
		spine->pushAfterCurrent(&TNerveTobiPukuDie::theNerve());
		return TRUE;
	}
	return FALSE;
}

// TODO: incorrect size. Map records 0x1e4 (484 bytes).
// TODO: 98.3%. The only difference is how the water-type test is grouped:
// TBGCheckData::isWaterSurface compiles to == 0x100, == 0x101, 0x102..0x105,
// == 0x4104, while the original groups 0x101..0x105 into one range. Same set of
// types, so the original's source lists them in a different order. Fixing it
// means editing the shared predicate in Map/MapData.hpp, which other matched
// callers depend on.
DEFINE_NERVE(TNerveTobiPukuFall, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0) {
		puku->mRotation.x = 0.0f;
		puku->setFallAnm();
	}

	if (!puku->isAirborne()) {
		if (puku->mGroundPlane->isWaterSurface()) {
			spine->pushAfterCurrent(&TNerveTobiPukuDie::theNerve());
			puku->generateEffectColumWater();
			puku->onLiveFlag(LIVE_FLAG_UNK20000);
		} else {
			spine->pushAfterCurrent(&TNerveTobiPukuDie::theNerve());
			puku->onLiveFlag(LIVE_FLAG_UNK20000);
		}
		return TRUE;
	}
	return FALSE;
}

// TODO: incorrect size. Map records 0x250 (592 bytes).
DEFINE_NERVE(TNerveTobiPukuHitWater, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0) {
		if (puku->isAirborne()) {
			if (puku->mPosition.y - puku->mGroundHeight > 50.0f) {
				puku->setAttackAnm();
				puku->hitWater();
			}
		} else if (puku->unk1AE != 0) {
			puku->setPichiAnm();
		}
	}

	if (!puku->isAirborne()) {
		JGeometry::TVec3<f32> away(puku->mPosition.x - SMS_GetMarioPos().x,
		                           0.0f,
		                           puku->mPosition.z - SMS_GetMarioPos().z);
		if (away.x == 0.0f && away.y == 0.0f && away.z == 0.0f)
			away.x += 1.0f;

		MsVECNormalize(away, away);
		away.y = 5.0f;
		away.x *= 5.0f;
		away.z *= 5.0f;
		puku->mVelocity = away;
		puku->onLiveFlag(LIVE_FLAG_AIRBORNE);
		puku->mPosition.y += away.y;
	}

	if (puku->checkCurAnmEnd(0)) {
		if (puku->isAttackBck())
			spine->pushAfterCurrent(&TNerveTobiPukuFall::theNerve());
		return TRUE;
	}
	return FALSE;
}

// TODO: incorrect size. Map records 0x198 (408 bytes).
DEFINE_NERVE(TNerveTobiPukuAttack, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0)
		puku->setAttackAnm();

	if (puku->isAirborne()) {
		if (puku->getCurAnmFrameNo(0) >= 6.0f) {
			puku->unk194 = 0;
			JGeometry::TVec3<f32> vel(puku->mVelocity);
			JGeometry::TVec3<f32> stop(0.0f, vel.y, 0.0f);
			puku->mVelocity = stop;
			puku->mPosition.y += 2.0f;
			puku->onLiveFlag(LIVE_FLAG_AIRBORNE);
		}

		if (puku->checkCurAnmEnd(0)) {
			spine->pushAfterCurrent(&TNerveTobiPukuFall::theNerve());
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}

// TODO: incorrect size. Map records 0x194 (404 bytes).
DEFINE_NERVE(TNerveTobiPukuFly, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0) {
		puku->setJumpStartAnm();
		puku->offLiveFlag(LIVE_FLAG_UNK10);
	}

	if (puku->checkCurAnmEnd(0) && puku->isJumpStartBck())
		puku->setJumpAnm();

	if (!puku->isAirborne()) {
		spine->pushAfterCurrent(&TNerveTobiPukuLand::theNerve());
		return TRUE;
	}

	JGeometry::TVec3<f32> vel(puku->mVelocity);
	puku->mFlyVelocityY = vel.y;

	JGeometry::TVec3<f32> dir(puku->mVelocity);
	puku->mRotation.x = MsGetRotFromZaxis(dir).x;
	return FALSE;
}

// TODO: incorrect size. Map records 0x1ac (428 bytes).
// TODO: 86.9% of 428 bytes. The structure and call order are right; what
// differs is how the two MsGetRotFromZaxis results are stored. The original
// keeps its returned vector in a stack temporary and copies a single float to
// mRotation, where this assigns the whole vector.
DEFINE_NERVE(TNerveTobiPukuGenerate, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0) {
		puku->onLiveFlag(LIVE_FLAG_UNK10);
		puku->mPosition.y -= 300.0f;
		JGeometry::TVec3<f32> dir(puku->mVelocity);
		puku->mRotation.x = MsGetRotFromZaxis(dir).x;
		puku->setJumpAnm();
	}

	puku->mPosition.y += puku->mLaunchVelocity.y;

	if (puku->mPosition.y > puku->unk1B0) {
		puku->mBoundCount = 0;
		puku->unk194      = 1;
		puku->mVelocity   = puku->mLaunchVelocity;
		puku->mLaunchAngle = MsGetRotFromZaxis(puku->mVelocity).x;
		puku->generateEffectColumWater();
		puku->onLiveFlag(LIVE_FLAG_AIRBORNE);
		puku->offLiveFlag(LIVE_FLAG_UNK10);
		spine->pushAfterCurrent(&TNerveTobiPukuFly::theNerve());
		return TRUE;
	}
	return FALSE;
}

const char** TTobiPuku::getBasNameTable() const { return pukupuku_bastable; }

const char** TMoePuku::getBasNameTable() const { return moepuku_bastable; }

void TMoePuku::swimEffect() { }

BOOL TTobiPuku::isInhibitedForceMove()
{
	return checkLiveFlag(LIVE_FLAG_AIRBORNE) ? TRUE : FALSE;
}

f32 TTobiPuku::getGravityY() const
{
	if (unk194)
		return unk19C->mFlyGravity.get();
	return mGravity;
}

void TTobiPuku::genEventCoin() { isDeadBck(); }

void TTobiPuku::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);
	mActorType = 0x10000012;
	unk150     = 0x31;
	unk19C     = (TTobiPukuParams*)getSaveParam();
	mMActor->setJointCallback(1, TobiPukuRollCallback);
}

void TTobiPuku::hitWater()
{
	JGeometry::TVec3<f32> vel(mVelocity);
	JGeometry::TVec3<f32> away(mPosition.x - SMS_GetMarioPos().x,
	                           mPosition.y - SMS_GetMarioPos().y,
	                           mPosition.z - SMS_GetMarioPos().z);
	if (away.x == 0.0f && away.y == 0.0f && away.z == 0.0f)
		away.x += 1.0f;

	MsVECNormalize(away, away);

	f32 push = unk19C->mHitWaterPush.get();
	vel.x    = away.x * push;
	vel.y    = 2.0f * (away.y * push);
	vel.z     = away.z * push;
	mVelocity = vel;

	mLaunchVelocity = vel;
	unk1B0          = mPosition.y;
	mRotation.y     = -((*gpMarioAngleY * (360.0f / 65536.0f)) - 180.0f);
}

void TTobiPuku::moveObject()
{
	mTurnSpeed = unk19C->mSLTurnSpeedLow.get();

	if (TTobiPuku::mBoundSw && isAirborne())
		hitWall();

	TWalkerEnemy::moveObject();
}

// TODO: 97.3%. One instruction differs: the original computes &unk104 into a
// register before the inlined getPoint, which neither the direct expression nor
// binding the result to a reference reproduces.
void TTobiPuku::kill()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;
	// TODO: 98.8%. The original emits `beq +8; b end` here where we fuse to
	// a single `bne end`. Neither a materialised bool nor an explicit else
	// reproduces the redundant branch.
	if (mGroundPlane->isIllegalData())
		return;

	mHitPoints = 1;
	if (mSpine->getCurrentNerve() != &TNerveTobiPukuDie::theNerve()
	    || !unk1AD) {
		unk1AD = 1;
		mSpine->reset();
		mSpine->setNext(&TNerveTobiPukuDie::theNerve());
		mSpine->pushAfterCurrent(mSpine->getDefault());
	}
}

void TTobiPuku::forceKill()
{
	// TODO: the trailing isJumpBck() is evaluated and tested but its result
	// goes nowhere, like genEventCoin() above. Whatever consumed it was
	// compiled out of the retail build.
	if (mSpine->getCurrentNerve() == &TNerveTobiPukuDie::theNerve())
		return;
	if (mSpine->getCurrentNerve() == &TNerveTobiPukuPrepareFly::theNerve())
		return;
	if (mSpine->getCurrentNerve() == &TNerveTobiPukuFly::theNerve())
		return;
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	if (!isJumpBck())
		return;
}

void TTobiPuku::initAttacker(THitActor* param_1)
{
	mRotation = param_1->mRotation;
	mSpine->pushNerve(&TNerveTobiPukuFly::theNerve());
	unk184 = 1;
}

void TTobiPuku::generateEffectColumWater()
{
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT))
		return;

	TEffectColumWater* enemy
	    = (TEffectColumWater*)gpConductor->makeOneEnemyAppear(
	        mPosition, "エフェクト水柱マネージャー", 0);

	if (enemy)
		enemy->generate(mPosition, mScaling);

	// Coming up out of the water while the Generate nerve is still running
	// is the "from water" cue; anything else is an entry splash.
	if (mSpine->getCurrentNerve() != &TNerveTobiPukuGenerate::theNerve())
		SMSGetMSound()->startSoundActor(MSD_SE_EN_TOBIPUKU_TOWATER,
		                                &mPosition, 0, nullptr, 0, 4);
	else
		SMSGetMSound()->startSoundActor(MSD_SE_EN_TOBIPUKU_FRWATER,
		                                &mPosition, 0, nullptr, 0, 4);
}

void TTobiPuku::walkBehavior(int param_1, f32 param_2)
{
	TWalkerEnemy::walkBehavior(param_1, param_2);

	f32 prevY   = mPosition.y;
	mPosition.y = mSwimBaseY + 10.0f * JMASin(2.0f * mSpine->getTime());

	// Only the pitch is taken: the bob is vertical, so yaw and roll are
	// left to whatever TWalkerEnemy::walkBehavior set.
	JGeometry::TVec3<f32> vel(mLinearVelocity);
	vel.y         = prevY - mPosition.y;
	mRotation.x = MsGetRotFromZaxis(vel).x;
}

void TTobiPuku::reset()
{
	gpCurTobiPuku = (TMoePuku*)this;
	TWalkerEnemy::reset();
	mSpine->initWith(&TNerveTobiPukuGenerate::theNerve());
	unk1AD          = 1;
	unk194          = 0;
	mLandDelta      = mPosition;
	mLandPos        = mLandDelta;
	mSwimBaseY      = mPosition.y;
}

void TTobiPuku::scalingChangeActor()
{
	// The flying variant caps its XZ growth at a fixed 3.0 rather than at
	// TSmallEnemyManager::mBlockXZScale, which is what TSmallEnemy uses.
	f32 xzScale = MsClamp(mJuiceBlock->unk140.x + 0.02f, 0.0f, 3.0f);

	mJuiceBlock->unk140.x   = mJuiceBlock->unk140.z = xzScale;
	mJuiceBlock->mScaling.x = mJuiceBlock->mScaling.z = xzScale;

	f32 yScale              = MsClamp(mJuiceBlock->unk140.y + 0.01f, 0.0f,
	                                  TSmallEnemyManager::mBlockYScale);
	mJuiceBlock->unk140.y   = yScale;
	mJuiceBlock->mScaling.y = yScale;
}

void TTobiPuku::changeOut()
{
	offLiveFlag(LIVE_FLAG_HIDDEN);

	// Note the direction: the puku takes the block's position when it pops
	// out, where TSmallEnemy::changeOut moves the block to the enemy.
	mPosition = mJuiceBlock->mPosition;

	gpMarioParticleManager->emitAndBindToPosPtr(0xCD, &mPosition, 0, nullptr);
	getMActor()->setFrameRate(SMSGetAnmFrameRate(), ANM_TYPE_BCK);
	mJuiceBlock->kill();
	mJuiceBlock = nullptr;
}

void TTobiPuku::swimEffect()
{
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT))
		return;

	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    0x178, getMActor()->getModel()->getAnmMtx(6), 1, this);
	if (!emitter)
		return;

	// Deeper water gives the bubble trail a longer life, up to a cap.
	s16 life = (s16)(mGroundHeight - mPosition.y) * 16 / 100 + 20;
	if (life > 200)
		life = 200;
	emitter->mBaseLifetime = life;
}

bool TTobiPuku::isReachedToGoalXZ()
{
	JGeometry::TVec3<f32> d(unk104.getPoint());
	d.x -= mPosition.x;
	d.y -= mPosition.y;
	d.z -= mPosition.z;
	d.y = 0.0f;

	if (d.x == 0.0f && d.z == 0.0f)
		return true;

	if (MsVECMag2(d) < 200.0f)
		return true;

	return false;
}

void TPukuPuku::load(JSUMemoryInputStream& stream)
{
	TSmallEnemy::load(stream);
	reset();
	unk1AC = 0;
}

void TPukuPuku::init(TLiveManager* manager)
{
	TTobiPuku::init(manager);
	mSpine->initWith(&TNerveTobiPukuSwimWander::theNerve());
	gpCurTobiPuku = nullptr;
}

void TPukuPuku::reset()
{
	TTobiPuku::reset();
	mSpine->initWith(&TNerveTobiPukuSwimWander::theNerve());
}
