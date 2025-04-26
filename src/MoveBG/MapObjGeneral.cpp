#include <MoveBG/MapObjGeneral.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <Player/MarioAccess.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/PollutionManager.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/Map.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

u32 TMapObjGeneral::mNormalLivingTime       = 960;
u32 TMapObjGeneral::mNormalFlushTime        = 360;
u32 TMapObjGeneral::mNormalFlushInterval    = 10;
u32 TMapObjGeneral::mNormalWaitToAppearTime = 360;
f32 TMapObjGeneral::mNormalAppearingScaleUp = 0.01f;
f32 TMapObjGeneral::mNormalThrowSpeedRate   = 0.5f;

bool TMapObjGeneral::isPollutedGround(const JGeometry::TVec3<f32>& v) const
{
	// WTF? Did they forget to refactor this?!
	const JGeometry::TVec3<f32>& p = unk10C;
	if (gpPollution->isPolluted(v.x, p.y, p.z)
	    || gpPollution->isPolluted(v.x - 32.0f, v.y, v.z - 32.0f)
	    || gpPollution->isPolluted(v.x + 32.0f, v.y, v.z - 32.0f)
	    || gpPollution->isPolluted(v.x - 32.0f, v.y, v.z + 32.0f)
	    || gpPollution->isPolluted(v.x + 32.0f, v.y, v.z + 32.0f))
		return true;

	return false;
}

inline f32 distToMario(const JGeometry::TVec3<f32>& v)
{
	JGeometry::TVec3<f32> r;
	r.sub(v, *gpMarioPos);
	return r.length();
}

void TMapObjGeneral::waitingToAppear()
{
	if (unk104 > 0 ? true : false)
		return;

	if (isActorType(0x4000005a)) {
		if (SMS_GetMarioDamageRadius() + mDamageRadius + 100.0f
		    > distToMario(unk10C))
			appear();
	} else {
		if (SMS_GetMarioDamageRadius() + mDamageRadius > distToMario(unk10C))
			appear();
	}
}

void TMapObjGeneral::waitingToRecover()
{
	if (!isPollutedGround(unk10C))
		recover();
}

void TMapObjGeneral::waitToAppear(s32 param_1)
{
	if (param_1 == 0)
		unk104 = mNormalWaitToAppearTime;
	else
		unk104 = param_1;
	mState = 10;
}

void TMapObjGeneral::sink()
{
	unkAC.x = unkAC.y = unkAC.z = 0.0f;
	onLiveFlag(0x10);
	mState = 7;
	unk144 = mPosition.y;
	setUpMapCollision(1);
	startSound(6);
}

void TMapObjGeneral::put() { }

void TMapObjGeneral::thrown() { }

void TMapObjGeneral::touchingWater()
{
	if (animIsFinished() && hasModelOrAnimData(4))
		startAnim(0);
}

void TMapObjGeneral::touchingPlayer()
{
	if (animIsFinished() && hasModelOrAnimData(4))
		startAnim(0);
}

void TMapObjGeneral::holding()
{
	mPosition = unk68->mPosition;
	unkC8     = gpMap->checkGround(mPosition, &unkC4);
}

void TMapObjGeneral::recovering() { }

void TMapObjGeneral::sinking() { }

void TMapObjGeneral::breaking()
{
	if (animIsFinished()) {
		makeObjDead();
		if (checkMapObjFlag(0x80000)) {
			makeObjDefault();
			waitToAppear(0);
		}
	}
}

void TMapObjGeneral::appearing()
{
	// TODO: uuuuuuuh...
	if (hasAnim(1)) {
		if (animIsFinished())
			goto uuuh;
		return;
	}

	{
		mScaling.x += mNormalAppearingScaleUp;
		mScaling.y += mNormalAppearingScaleUp;
		mScaling.z += mNormalAppearingScaleUp;
		if (mScaling.x < unk124.x)
			return;

		mScaling.set(unk124);
	}

uuuh:
	if (!checkLiveFlag(0x10))
		return;

	makeObjAppeared();
}

void TMapObjGeneral::appeared()
{
	if (checkMapObjFlag(0x40000) && !(unk104 > 0 ? true : false))
		makeObjDead();
}

void TMapObjGeneral::makeObjRecovered()
{
	makeObjDefault();
	makeObjAppeared();
}

void TMapObjGeneral::makeObjBuried()
{
	unk144 = mPosition.y;
	mPosition.y -= unk130->mHit->unkC[2].unkC;
	unk64 |= 1;
	removeMapCollision();
	unk74  = nullptr;
	mState = 8;
}

void TMapObjGeneral::receiveMessageFromPlayer() { startAnim(4); }

u32 TMapObjGeneral::touchWater(THitActor* water)
{
	if (checkMapObjFlag(0x400000)) {
		kill();
		return 1;
	} else {
		if (hasModelOrAnimData(3)) {
			startAnim(3);
			mState = 5;
		}
		return 1;
	}
}

void TMapObjGeneral::touchPlayer(THitActor* player)
{
	TMapObjBase::touchPlayer(player);
	if (hasModelOrAnimData(4)) {
		startAnim(4);
		mState = 4;
	}
}

void TMapObjGeneral::recover()
{
	gpPollution->clean(mPosition.x, unk144, mPosition.z,
	                   (u16)(unk130->mHit->unkC[2].unk0 / 6.0f));

	setUpMapCollision(1);
	startAnim(6);
	mState = 9;
	setObjHitData(0);
	startSound(8);
	mDamageHeight = 0.0f;
	calcEntryRadius();
	unk64 &= ~0x1;
	if (hasModelOrAnimData(6)) {
		f32 tmp     = mPosition.y;
		mPosition.y = unk144;
		unk144      = tmp;
		getModel();
	}
}

void TMapObjGeneral::hold(TTakeActor* actor)
{
	if (unkEC && unkEC->unk8)
		unkEC->unk8->remove();
	unk64 |= 1;
	unk68  = actor;
	mState = 6;
}

void TMapObjGeneral::ensureTakeSituation()
{
	TMapObjBase::ensureTakeSituation();
	if (isState(6) && unk68 == nullptr) {
		mState = 1;
		offLiveFlag(0x10);
	}
}

void TMapObjGeneral::kill()
{
	unk64 |= 1;
	removeMapCollision();
	onLiveFlag(0x18);
	unk104 = 0xffffffff;
	startAnim(2);
	mState = 3;
	startSound(2);
	breaking();
}

void TMapObjGeneral::appear()
{
	makeObjAppeared();
	startAnim(1);
	if (checkMapObjFlag(0x800000)) {
		mScaling.x = mNormalAppearingScaleUp;
		mScaling.y = mNormalAppearingScaleUp;
		mScaling.z = mNormalAppearingScaleUp;
	}

	if (!isActorType(0x20000010)
	    || !TFlagManager::smInstance->getBlueCoinFlag(
	        gpMarDirector->getCurrentMap(), unk134))
		startSound(1);

	appearing();
	if (checkMapObjFlag(0x40000))
		unk104 = getLivingTime();

	mState = 2;
}

void TMapObjGeneral::work()
{
	switch (mState) {
	case 1:
		appeared();
		break;
	case 2:
		appearing();
		break;
	case 3:
		breaking();
		break;
	case 7:
		sinking();
		break;
	case 9:
		recovering();
		break;
	case 4:
		touchingPlayer();
		break;
	case 5:
		touchingWater();
		break;
	case 6:
		holding();
		break;
	case 8:
		waitingToRecover();
		break;
	}
}

void TMapObjGeneral::touchWall(JGeometry::TVec3<f32>* param_1,
                               TBGWallCheckRecord* param_2)
{
	param_1->x = param_2->unk0.x;
	param_1->z = param_2->unk0.z;
	calcReflectingVelocity(param_2->unk1C, unk130->mPhysical->unk4->unk8,
	                       &unkAC);
}

void TMapObjGeneral::checkWallCollision(JGeometry::TVec3<f32>*) { }

void TMapObjGeneral::touchRoof(JGeometry::TVec3<f32>* param_1)
{
	param_1->y = unk140;
}

void TMapObjGeneral::checkRoofCollision(JGeometry::TVec3<f32>* param_1)
{
	unk140
	    = gpMap->checkRoof(param_1->x, param_1->y + unkC0, param_1->z, &unk13C);
	if (param_1->y + unkC0 >= unk140)
		touchRoof(param_1);
}

void TMapObjGeneral::touchGround(JGeometry::TVec3<f32>*) { }

void TMapObjGeneral::checkGroundCollision(JGeometry::TVec3<f32>*) { }

void TMapObjGeneral::calcVelocity() { }

void TMapObjGeneral::bind() { }

void TMapObjGeneral::control()
{
	TMapObjBase::control();
	if (checkMapObjFlag(0x1000000) && isState(1) && !checkLiveFlag2(0x80)
	    && isPollutedGround(mPosition))
		sink();

	work();
}

void TMapObjGeneral::calcRootMatrix() { }

void TMapObjGeneral::perform(u32, JDrama::TGraphics*) { }

u32 TMapObjGeneral::receiveMessage(THitActor*, u32) { }

void TMapObjGeneral::loadAfter()
{
	TMapObjBase::loadAfter();
	if (checkMapObjFlag(0x1000000) && isPollutedGround(mPosition))
		makeObjBuried();
}

TMapObjGeneral::TMapObjGeneral(const char* name)
    : TMapObjBase(name)
    , unk138(0)
    , unk13C(0)
    , unk140(0.0f)
    , unk144(0.0f)
{
}
