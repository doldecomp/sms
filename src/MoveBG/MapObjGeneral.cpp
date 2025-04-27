#include <MoveBG/MapObjGeneral.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <Player/MarioAccess.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/PollutionManager.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/Map.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

u32 TMapObjGeneral::mNormalLivingTime       = 960;
u32 TMapObjGeneral::mNormalFlushTime        = 360;
int TMapObjGeneral::mNormalFlushInterval    = 10;
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

void TMapObjGeneral::recovering()
{
	startSound(9);
	if (hasModelOrAnimData(6)) {
		J3DModel* model = getModel();
		MtxPtr mat      = model->getAnmMtx(0);
		f32 fVar1       = mat[3][1] - unk144;
		mDamageHeight += fVar1;
		calcEntryRadius();
		if (unk6C)
			unk6C->mPosition.y += fVar1;
		unk144 = mat[3][1];
		if (!animIsFinished())
			return;
	} else if (mPosition.y < unk144) {
		mPosition.y += unk130->mSink->unk4;
		if (unk6C)
			unk6C->mPosition.y += unk130->mSink->unk4;
		return;
	}

	makeObjRecovered();
}

void TMapObjGeneral::sinking()
{
	mPosition.y -= unk130->mSink->unk0;

	for (int i = 0; i < getColNum(); ++i) {
		if (getCollision(i)->checkActorType(0x1000000)) {
			recover();
			return;
		}
	}

	if (mPosition.y + unk130->mHit->unkC[2].unk4 < unk144) {
		if (mPosition.x != unk10C.x || mPosition.z != unk10C.z) {
			makeObjDefault();
			makeObjAppeared();
		} else {
			makeObjBuried();
		}
	}
}

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

void TMapObjGeneral::checkWallCollision(JGeometry::TVec3<f32>* param_1)
{
	param_1->y += unk130->mPhysical->unk4->unk1C;

	TBGWallCheckRecord check(*param_1, unkBC, 4, unk130->mPhysical->unk8);

	bool touched = gpMap->isTouchedWallsAndMoveXZ(&check);

	param_1->y -= unk130->mPhysical->unk4->unk1C;

	if (touched) {
		unk138 = check.unk1C;
		touchWall(param_1, &check);
	} else {
		unk138 = 0;
	}
}

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

void TMapObjGeneral::touchGround(JGeometry::TVec3<f32>* param_1)
{
	if (unk130->mPhysical ? true : false) {
		unkAC.x *= unk130->mPhysical->unk4->unk10;
		unkAC.z *= unk130->mPhysical->unk4->unk10;
	}

	if ((unk130->mPhysical ? true : false)
	    && std::abs(JGeometry::TVec3<f32>(unkAC).y)
	           > unk130->mPhysical->unk4->unkC) {
		param_1->y -= JGeometry::TVec3<f32>(unkAC).y;
		unkAC.y *= -unk130->mPhysical->unk4->unk4;
		if (isCoin(this)) {
			// TODO: this is an inline 100%
			f32 a = std::fabsf(JGeometry::TVec3<f32>(unkAC).y);
			if (gpMSound->gateCheck(0x4842)) {
				MSoundSESystem::MSoundSE::startSoundActorWithInfo(
				    0x4842, mPosition, nullptr, a, 0, 0, nullptr, 0, 4);
			}
		} else {
			startSound(4);
		}
	} else {
		offLiveFlag(0x80);
		unkAC.x = unkAC.y = unkAC.z = 0.0f;
		onLiveFlag(0x10);
		param_1->y = unkC8;
	}
}

void TMapObjGeneral::checkGroundCollision(JGeometry::TVec3<f32>* param_1)
{
	unkC8 = gpMap->checkGround(param_1->x, param_1->y + unkC0, param_1->z,
	                           &unkC4);
	unkC8 += 1.0f;
	if (param_1->y <= unkC8) {
		touchGround(param_1);
	} else if (!unkD4)
		onLiveFlag(0x80);
}

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

void TMapObjGeneral::calcRootMatrix()
{
	J3DModel* model = getModel();

	if (isState(6) && unk68) {
		if (unk130->mHold) {
			TMapObjHoldData* hold = unk130->mHold;

			MtxPtr src = getTakingMtx();
			MTXCopy(src, hold->unkC->getBaseTRMtx());
			hold->unkC->calc();

			MtxPtr src2 = hold->unk10;
			MTXCopy(src2, model->getBaseTRMtx());
			mPosition.set(src2[3][0], src2[3][1], src2[3][2]);
		} else {
			MtxPtr src = getTakingMtx();
			MTXCopy(src, checkMapObjFlag(0x100) ? model->getAnmMtx(0)
			                                    : model->getBaseTRMtx());
			mPosition.set(src[3][0], src[3][1], src[3][2]);
		}
	} else {
		JGeometry::TVec3<f32> pos(mPosition.x, mPosition.y - unk108,
		                          mPosition.z);
		JGeometry::TVec3<f32> rot;
		rot.scale(65536.0f / 360.0f, mRotation);
		MsMtxSetXYZRPH(model->getBaseTRMtx(), pos.x, pos.y, pos.z, rot.x, rot.y,
		               rot.z);
	}
	model->unk14 = mScaling;
}

void TMapObjGeneral::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 1) {
		if (isState(10)) {
			waitingToAppear();
		}
	} else {
		if (checkMapObjFlag(0x40000) && isUnk104Positive()
		    && getUnk104() < getFlushTime()
		    && ((getUnk104() / mNormalFlushInterval) & 1) != 0) {
			return;
		}
	}

	TMapObjBase::perform(param_1, param_2);
}

u32 TMapObjGeneral::receiveMessage(THitActor* param_1, u32 param_2)
{
	int ret = TMapObjBase::receiveMessage(param_1, param_2);
	if (ret)
		return true;

	// TODO: concerning. Is unkAC actually a Vec?
	if (param_2 == 4 && checkMapObjFlag(0x100000)
	    && JGeometry::TVec3<f32>(unkAC).squared() <= 3.814697e-06f
	    && (isState(2) || isState(1) || isState(4) || isState(5))) {
		hold((TTakeActor*)param_1);
		return true;
	}

	if (param_2 == 4 && isActorType(0x10000025) && (isState(2) || isState(1))) {
		hold((TTakeActor*)param_1);
		return 1;
	}

	if (param_2 == 6 && isState(6)) {
		put();
		return true;
	}

	if (param_2 == 7 && isState(6) && unk130->mPhysical != nullptr) {
		thrown();
		return true;
	}

	if (param_2 == 1 && checkMapObjFlag(0x200000)) {
		kill();
		return true;
	}

	if (isActorType(0x80000001) && (param_2 == 0 || param_2 == 1)) {
		receiveMessageFromPlayer();
		return true;
	}

	if (param_2 == 11 && checkMapObjFlag(0x200000)) {
		kill();
	}

	return false;
}

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
