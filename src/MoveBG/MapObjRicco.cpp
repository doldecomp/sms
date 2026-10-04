#include <MoveBG/MapObjRicco.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JSupport/JSUInputStream.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBall.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <math.h>
#include <string.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static JGeometry::TVec3<f32> submarineCranePos_forSound(1956.0f, 1000.0f,
                                                        6425.0f);
static JGeometry::TVec3<f32> submarineSetWtPos_forSound(1956.0f, -100.0f,
                                                        6425.0f);

int TCraneRotY::mWaitTime = 120;

void TCraneRotY::calc() { setRootMtxRotY(); }

void TCraneRotY::control()
{
	TMapObjBase::control();

	switch (mState) {
	case STATE_ROTATE_TO_MAX:
		mRotation.y += unk144;
		if (mRotation.y > unk138 + unk140) {
			startStateTimer(mWaitTime);
			mState = STATE_WAIT_AT_MAX;
		}
		break;
	case STATE_WAIT_AT_MIN:
		if (!isStateTimerEngaged())
			mState = STATE_ROTATE_TO_MAX;
		break;
	case STATE_ROTATE_TO_MIN:
		mRotation.y -= unk144;
		if (mRotation.y < unk138 + unk13C) {
			startStateTimer(mWaitTime);
			mState = STATE_WAIT_AT_MIN;
		}
		break;
	case STATE_WAIT_AT_MAX:
		if (!isStateTimerEngaged())
			mState = STATE_ROTATE_TO_MIN;
		break;
	}

	if (isState(STATE_ROTATE_TO_MAX) || isState(STATE_ROTATE_TO_MIN))
		SMSGetMSound()->startSoundActor(unk148, &mPosition, 0, nullptr, 0, 4);
}

void TCraneRotY::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	stream >> unk140;
	unk138 = mRotation.y;
	unk144 = MsRandF() * 0.1f + 0.05f;
	if (strcmp(mName, "crane90 0") == 0)
		unk148 = MSD_SE_OBJ_CRANE_SIDEMOVE1;
	else
		unk148 = MSD_SE_OBJ_CRANE_SIDEMOVE2;
	mState = STATE_ROTATE_TO_MAX;
}

f32 TCraneUpDown::mRotSpeed = 0.1f;
int TCraneUpDown::mWaitTime = 120;

void TCraneUpDown::control()
{
	TMapObjBase::control();

	switch (mState) {
	case STATE_WAIT_AT_MIN:
		if (!isStateTimerEngaged())
			mState = STATE_ROTATE_TO_MAX;
		break;
	case STATE_ROTATE_TO_MAX:
		mRotation.x += mRotSpeed;
		if (mRotation.x > unk140) {
			startStateTimer(mWaitTime);
			mState = STATE_WAIT_AT_MAX;
		}
		break;
	case STATE_WAIT_AT_MAX:
		if (!isStateTimerEngaged())
			mState = STATE_ROTATE_TO_MIN;
		break;
	case STATE_ROTATE_TO_MIN:
		mRotation.x -= mRotSpeed;
		if (mRotation.x < unk144) {
			startStateTimer(mWaitTime);
			mState = STATE_WAIT_AT_MIN;
		}
		break;
	}

	unk138->mPosition.set(0.0f, 0.0f, 1500.0f);
	MtxPtr mtx = getModel()->getAnmMtx(0);
	Mtx rotY;
	MTXIdentity(rotY);
	MsMtxSetRotX(mtx, mRotation.x);
	MsMtxSetRotY(rotY, mRotation.y);
	MTXConcat(rotY, mtx, mtx);
	MTXMultVec(mtx, &unk138->mPosition, &unk138->mPosition);
	unk138->mPosition.x += mPosition.x;
	unk138->mPosition.y += mPosition.y - getObjCollisionHeightOffset()
	                       + unk138->getObjCollisionHeightOffset();
	unk138->mPosition.z += mPosition.z;

	if (isState(STATE_ROTATE_TO_MIN) || isState(STATE_ROTATE_TO_MAX))
		SMSGetMSound()->startSoundActor(unk13C, &mPosition, 0, nullptr, 0, 4);
}

void TCraneUpDown::initMapObj()
{
	TMapObjBase::initMapObj();
	mMapCollisionManager->getUnk8()->setAllActor(nullptr);
	unk138 = TMapObjBaseManager::newAndRegisterObj("craneCargoUpDown");
	unk138->appear();
	if (strcmp(mName, "craneUpDown 0") == 0) {
		unk144 = -25.0f;
		unk140 = 45.0f;
		unk13C = MSD_SE_OBJ_CRANE_UPDOWN1;
	} else {
		unk144 = -25.0f;
		unk140 = 30.0f;
		unk13C = MSD_SE_OBJ_CRANE_UPDOWN2;
	}
	mRotation.x = MsRandF() * (unk140 - unk144) + unk144;
}

void TCraneCargo::control()
{
	unk158.zero();
	TMapObjBase::control();
}

void TCraneCargo::calc()
{
	updateRootMtxTrans();
	calcLeanMtx(getModel()->getAnmMtx(1));
}

f32 TRiccoWatermill::mRotAccel              = 1.0f;
f32 TRiccoWatermill::mRotSpeedMaxUp         = 3.0f;
f32 TRiccoWatermill::mRotSpeedMaxDown       = 1.0f;
f32 TRiccoWatermill::mRotDown               = 0.05f;
f32 TRiccoWatermill::mSubmarineMoveRate     = 0.5f;
f32 TRiccoWatermill::mSubmarineMaxTransY    = 750.0f;
f32 TRiccoWatermill::mSubmarineBottomTransY = -950.0f;
int TRiccoWatermill::mWaitTime              = 600;
f32 TRiccoWatermill::mSubmarineSurfaceTransY;

u32 TRiccoWatermill::touchWater(THitActor*)
{
	if (isState(STATE_STAY_AT_SURFACE))
		return 1;

	unk140 = 5;
	if (isState(STATE_NORMAL))
		unk13C->setUpMapCollision(1);
	offMapObjFlag(MAP_OBJ_FLAG_UNK100);
	unk13C->offMapObjFlag(MAP_OBJ_FLAG_UNK100);
	if (unk13C->mPosition.y < mSubmarineMaxTransY) {
		unk138 += mRotAccel;
		if (unk138 > mRotSpeedMaxUp)
			unk138 = mRotSpeedMaxUp;
		mState = STATE_RISE;
	} else {
		unk138 = 0.0f;
	}
	return 1;
}

void TRiccoWatermill::control()
{
	TMapObjBase::control();

	if (isState(STATE_RISE) || isState(STATE_SINK_TO_SURFACE)
	    || isState(STATE_SINK_TO_BOTTOM)) {
		if (unk138 != 0.0f) {
			mRotation.z -= unk138;
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_SUBMARINE_MILL, &mPosition, nullptr, fabsf(unk138),
			    0, 0, &unk14C[0], 0, 4);
			f32 move = unk138 * mSubmarineMoveRate;
			unk13C->mPosition.y += move;
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_SUBMARINE_CRANE, &submarineCranePos_forSound,
			    nullptr, fabsf(move), 0, 0, &unk14C[1], 0, 4);
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_SUBMARINE_SINK, &submarineCranePos_forSound, nullptr,
			    fabsf(move), 0, 0, &unk14C[2], 0, 4);
		}

		if (unk140 == 0) {
			unk138 -= mRotDown;
			if (unk138 < -mRotSpeedMaxDown)
				unk138 = -mRotSpeedMaxDown;
		} else {
			--unk140;
		}
	}

	switch (mState) {
	case STATE_NORMAL:
		break;
	case STATE_RISE:
		if (unk13C->mPosition.y > mSubmarineMaxTransY) {
			unk13C->mPosition.y = mSubmarineMaxTransY;
			if (!isStateTimerEngaged()) {
				SMSGetMSound()->startSoundActor(MSD_SE_OBJ_SUBMARINE_SET,
				                                &unk13C->mPosition, 0, nullptr,
				                                0, 4);
				if (!unk144) {
					JGeometry::TVec3<f32> point(
					    2008.0f, mSubmarineMaxTransY + 500.0f, 7066.0f);
					throwObjToFrontFromPoint(unk148, point, 20.0f, 20.0f);
				}
				unk144 = true;
			}
		}
		if (unk138 < 0.0f) {
			if (unk144)
				mState = STATE_SINK_TO_SURFACE;
			else
				mState = STATE_SINK_TO_BOTTOM;
		}
		break;
	case STATE_SINK_TO_SURFACE:
		if (unk13C->mPosition.y <= mSubmarineSurfaceTransY) {
			unk13C->mPosition.y = mSubmarineSurfaceTransY;
			unk13C->setUpMapCollision(0);
			unk138 = 0.0f;
			SMSGetMSound()->startSoundActor(
			    MSD_SE_OBJ_SUBMARINE_SET, &unk13C->mPosition, 0, nullptr, 0, 4);
			startStateTimer(mWaitTime);
			mState = STATE_STAY_AT_SURFACE;
		}
		break;
	case STATE_SINK_TO_BOTTOM:
		if (unk13C->mPosition.y <= mSubmarineBottomTransY) {
			unk13C->mPosition.y = mSubmarineBottomTransY;
			unk13C->setUpMapCollision(0);
			unk138 = 0.0f;
			unk13C->onMapObjFlag(MAP_OBJ_FLAG_UNK100);
			onMapObjFlag(MAP_OBJ_FLAG_UNK100);
			SMSGetMSound()->startSoundActor(SMD_SE_OBJ_SUBMARINE_SET_WT,
			                                &unk13C->mPosition, 0, nullptr, 0,
			                                4);
			mState = STATE_NORMAL;
		}
		break;
	case STATE_STAY_AT_SURFACE:
		break;
	}
}

void TRiccoWatermill::calc() { setRootMtxRotZ(); }

void TRiccoWatermill::loadAfter()
{
	TMapObjBase::loadAfter();
	unk13C
	    = static_cast<TMapObjBase*>(JDrama::TNameRefGen::search("submarine"));
	unk148 = static_cast<TMapObjBase*>(
	    JDrama::TNameRefGen::search("青コイン（潜水艦用）"));
	unk148->makeObjDead();
	unk13C->mPosition.y = mSubmarineBottomTransY;
	unk13C->removeMapCollision();
	unk13C->setUpCurrentMapCollision();
}

TRiccoWatermill::TRiccoWatermill(const char* name)
    : TMapObjBase(name)
    , unk138(0.0f)
    , unk13C(nullptr)
    , unk140(0)
    , unk144(false)
    , unk148(nullptr)
{
	for (int i = 0; i < 3; ++i)
		unk14C[i] = nullptr;
}

void TSurfGesoObj::initMapObj()
{
	TMapObjBase::initMapObj();
	if (strcmp(unkF4, "SurfGesoRed") == 0) {
		unk154.r = 0xFF;
		unk154.g = 0xB4;
		unk154.b = 0xFF;
		unk154.a = 0xFF;
	} else if (strcmp(unkF4, "SurfGesoYellow") == 0) {
		unk154.r = 0xFF;
		unk154.g = 0xFF;
		unk154.b = 0x7D;
		unk154.a = 0xFF;
	} else if (strcmp(unkF4, "SurfGesoGreen") == 0) {
		unk154.r = 0xB4;
		unk154.g = 0xFF;
		unk154.b = 0xB4;
		unk154.a = 0xFF;
	}
	mMActor = SMS_MakeMActorFromSDLModelData(
	    gpMapObjManager->getSurfGessoModelData(),
	    gpMapObjManager->getMActorAnmData(), 3);
	initPacketMatColor(getModel(), GX_TEVREG1, &unk154);
	mMActor->setBck("surfgeso_run1");
}

void TFruitSwitch::pullUp()
{
	getMActor()->getFrameCtrl(ANM_TYPE_BCK)->setFrame(0.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	getModel()->calc();
	mMapCollisionManager->getUnk8()->setUpMtx(getModel()->getAnmMtx(0));
}

void TFruitSwitch::pushDown()
{
	startBck("riccoswitch");
	onHitFlag(HIT_FLAG_NO_COLLISION);
	if (mMapCollisionManager->unk8)
		mMapCollisionManager->unk8->remove();
}

BOOL TFruitSwitch::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_HIP_DROP) {
		pushDown();
		unk138->fireObj();
		return true;
	}
	return false;
}

f32 TFruitLauncher::mObjSpeedXZ    = 1.0f;
f32 TFruitLauncher::mObjSpeedY     = 20.0f;
int TFruitLauncher::mFruitLiveTime = 4800;

TMapObjBase* TFruitLauncher::appearFruit() const
{
	f32 rnd = 100.0f * MsRandF();
	if (rnd < 20.0f)
		return gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                    mPosition.z, 0x40000390, false);
	else if (rnd < 40.0f)
		return gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                    mPosition.z, 0x40000391, false);
	else if (rnd < 60.0f)
		return gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                    mPosition.z, 0x40000392, false);
	else if (rnd < 80.0f)
		return gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                    mPosition.z, 0x40000393, false);
	else
		return gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                    mPosition.z, 0x40000394, false);
}

void TFruitLauncher::fireObj()
{
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_JUMP_ED_B,
	                                            &mPosition, 0, nullptr);
	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_AP_BUTTON, &mPosition, 0,
	                                nullptr, 0, 4);
	SMSGetMSound()->startSoundActor(MSD_SE_SMOKE_EFFECT, &mPosition, 0, nullptr,
	                                0, 4);

	if (unk140 == 0)
		unk140 = 1;
	else
		unk140 = 0;
	unk138[unk140]->pullUp();

	TMapObjBase* fruit = appearFruit();
	if (!fruit)
		fruit = appearFruit();
	if (!fruit)
		fruit = appearFruit();

	if (fruit) {
		fruit->mPosition.set(mPosition);
		fruit->mVelocity.set(mObjSpeedXZ * (MsRandF() - 0.5f), -mObjSpeedY,
		                     mObjSpeedXZ * (MsRandF() - 0.5f));
		fruit->offLiveFlag(LIVE_FLAG_UNK10);
		SMSGetMarDirector()->fireStartDemoCamera("フルーツタンクカメラカメラ",
		                                         &fruit->mPosition, -1, 0.0f,
		                                         true, nullptr, 0, nullptr, 0);
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_COLLECT_DELIGHT, 0,
		                                   nullptr, 0);
		if (isFruit(fruit))
			static_cast<TResetFruit*>(fruit)->killByTimer(mFruitLiveTime);
	}
}

void TFruitLauncher::loadAfter()
{
	TMapObjBase::loadAfter();

	TResetFruit* fruit;
	fruit = static_cast<TResetFruit*>(
	    TMapObjBaseManager::newAndRegisterObj("FruitCoconut"));
	fruit->unk1A4 = 1;
	fruit         = static_cast<TResetFruit*>(
        TMapObjBaseManager::newAndRegisterObj("FruitDurian"));
	fruit->unk1A4 = 1;
	fruit         = static_cast<TResetFruit*>(
        TMapObjBaseManager::newAndRegisterObj("FruitPapaya"));
	fruit->unk1A4 = 1;
	fruit         = static_cast<TResetFruit*>(
        TMapObjBaseManager::newAndRegisterObj("FruitPine"));
	fruit->unk1A4 = 1;
	fruit         = static_cast<TResetFruit*>(
        TMapObjBaseManager::newAndRegisterObj("FruitBanana"));
	fruit->unk1A4 = 1;

	unk138[0] = static_cast<TFruitSwitch*>(
	    JDrama::TNameRefGen::search("タンクスイッチＡ"));
	unk138[0]->unk138 = this;
	unk138[1]         = static_cast<TFruitSwitch*>(
        JDrama::TNameRefGen::search("タンクスイッチＢ"));
	unk138[1]->unk138 = this;
	unk140            = 1;
	unk138[0]->pushDown();
}
