#include <MoveBG/MapObjRicco.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBall.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <stdlib.h>
#include <string.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

/// Where the crane sound is heard from, independent of where the submarine
/// actually is.
static JGeometry::TVec3<f32> submarineCranePos_forSound(1956.0f, 1000.0f,
                                                        6425.0f);
static JGeometry::TVec3<f32> submarineSetWtPos_forSound(1956.0f, -100.0f,
                                                        6425.0f);

// TCraneRotY

static inline MSound* MapObjRiccoGetMSound()
{
	MSound* sound = gpMSound;
	return sound;
}

static inline bool MapObjRiccoIsState(TMapObjBase* self, u32 state)
{
	bool v = self->isState(state);
	return v;
}

int TCraneRotY::mWaitTime = 120;

void TCraneRotY::calc() { setRootMtxRotY(); }

void TCraneRotY::control()
{
	TMapObjBase::control();

	switch (mState) {
	case STATE_TURNING_UP:
		mRotation.y += mRotSpeed;
		if (mRotation.y > mBaseRotY + mRotYMax) {
			mStateTimer = mWaitTime;
			mState       = STATE_WAIT_AT_END;
		}
		break;

	case STATE_WAIT_AT_START:
		if (!isStateTimerEngaged())
			mState = STATE_TURNING_UP;
		break;

	case STATE_TURNING_DOWN:
		mRotation.y -= mRotSpeed;
		if (mRotation.y < mBaseRotY + mRotYMin) {
			mStateTimer = mWaitTime;
			mState       = STATE_WAIT_AT_START;
		}
		break;

	case STATE_WAIT_AT_END:
		if (!isStateTimerEngaged())
			mState = STATE_TURNING_DOWN;
		break;
	}

	if (MapObjRiccoIsState(this, STATE_TURNING_UP)
	    || MapObjRiccoIsState(this, STATE_TURNING_DOWN))
		MapObjRiccoGetMSound()->startSoundActor(mSoundId, &mPosition);
}

void TCraneRotY::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	stream.read(&mRotYMax, 4);
	mBaseRotY = mRotation.y;
	mRotSpeed = 0.1f * MsRandF() + 0.05f;

	if (strcmp(getName(), "crane90 0") == 0)
		mSoundId = 0x3034;
	else
		mSoundId = 0x3035;

	mState = STATE_TURNING_UP;
}

// TCraneUpDown

f32 TCraneUpDown::mRotSpeed = 0.1f;
int TCraneUpDown::mWaitTime = 120;

void TCraneUpDown::control()
{
	TMapObjBase::control();

	switch (mState) {
	case STATE_WAIT_AT_START:
		if (!isStateTimerEngaged())
			mState = STATE_TIPPING_UP;
		break;

	case STATE_TIPPING_UP:
		mRotation.x += mRotSpeed;
		if (mRotation.x > mRotXMax) {
			mStateTimer = mWaitTime;
			mState      = STATE_WAIT_AT_END;
		}
		break;

	case STATE_WAIT_AT_END:
		if (!isStateTimerEngaged())
			mState = STATE_TIPPING_DOWN;
		break;

	case STATE_TIPPING_DOWN:
		mRotation.x -= mRotSpeed;
		if (mRotation.x < mRotXMin) {
			mStateTimer = mWaitTime;
			mState      = STATE_WAIT_AT_START;
		}
		break;
	}

	// The cargo hangs 1500 units out along the arm; rotate that offset by the
	// crane's own X and Y and add the crane's position.
	mCargo->mPosition.set(0.0f, 0.0f, 1500.0f);

	MtxPtr mtx = getModel()->getAnmMtx(0);

	Mtx rotY;
	MTXIdentity(rotY);
	MsMtxSetRotX(mtx, mRotation.x);
	MsMtxSetRotY(rotY, mRotation.y);
	MTXConcat(rotY, mtx, mtx);
	MTXMultVec(mtx, &mCargo->mPosition, &mCargo->mPosition);

	mCargo->mPosition.x += mPosition.x;
	mCargo->mPosition.y += (mPosition.y - mYOffset) + mCargo->mYOffset;
	mCargo->mPosition.z += mPosition.z;

	// TODO: frame 0x90 vs 0xa0 and a commutative fadds on
	// (mPosition.y - mYOffset) + mCargo->mYOffset. The sound/state binders
	// that closed TCraneRotY::control take +0x18; getModel/cargo/dy binders
	// skip 0xa0 (0x98 or 0xa8).
	if (MapObjRiccoIsState(this, STATE_TIPPING_DOWN)
	    || MapObjRiccoIsState(this, STATE_TIPPING_UP))
		MapObjRiccoGetMSound()->startSoundActor(mSoundId, &mPosition);
}

void TCraneUpDown::initMapObj()
{
	TMapObjBase::initMapObj();

	mMapCollisionManager->getUnk8()->setAllActor(nullptr);

	mCargo = (TCraneCargo*)TMapObjBaseManager::newAndRegisterObj(
	    "craneCargoUpDown");
	mCargo->appear();

	if (strcmp(getName(), "craneUpDown 0") == 0) {
		mRotXMin = -25.0f;
		mRotXMax = 45.0f;
		mSoundId = 0x3036;
	} else {
		mRotXMin = -25.0f;
		mRotXMax = 30.0f;
		mSoundId = 0x3037;
	}

	// Start somewhere random in the swing so the two cranes are out of phase.
	mRotation.x = (mRotXMax - mRotXMin) * MsRandF() + mRotXMin;
}

// TCraneCargo

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

// TRiccoWatermill

f32 TRiccoWatermill::mRotAccel              = 1.0f;
f32 TRiccoWatermill::mRotSpeedMaxUp         = 3.0f;
f32 TRiccoWatermill::mRotSpeedMaxDown       = 1.0f;
f32 TRiccoWatermill::mRotDown               = 0.05f;
f32 TRiccoWatermill::mSubmarineMoveRate     = 0.5f;
f32 TRiccoWatermill::mSubmarineMaxTransY    = 750.0f;
f32 TRiccoWatermill::mSubmarineBottomTransY = -950.0f;
int TRiccoWatermill::mWaitTime              = 600;
f32 TRiccoWatermill::mSubmarineSurfaceTransY;

u32 TRiccoWatermill::touchWater(THitActor* sender)
{
	if (isState(STATE_SURFACED))
		return 1;

	mWaterHitTimer = 5;

	if (isState(STATE_DOWN))
		mSubmarine->setUpMapCollision(1);

	offMapObjFlag(MAP_OBJ_FLAG_UNK100);
	mSubmarine->offMapObjFlag(MAP_OBJ_FLAG_UNK100);

	if (mSubmarine->mPosition.y < mSubmarineMaxTransY) {
		mRotSpeed += mRotAccel;
		if (mRotSpeed > mRotSpeedMaxUp)
			mRotSpeed = mRotSpeedMaxUp;
		mState = STATE_RISING;
	} else {
		mRotSpeed = 0.0f;
	}

	return 1;
}

void TRiccoWatermill::control()
{
	TMapObjBase::control();

	if (MapObjRiccoIsState(this, STATE_RISING)
	    || MapObjRiccoIsState(this, STATE_SINKING_DONE)
	    || MapObjRiccoIsState(this, STATE_SINKING)) {
		if (0.0f != mRotSpeed) {
			mRotation.z -= mRotSpeed;
			MapObjRiccoGetMSound()->startSoundActorWithInfo(
			    0x3031, &mPosition, nullptr, fabsf(mRotSpeed), 0, 0,
			    &mWheelSound, 0, 4);

			f32 move = mRotSpeed * mSubmarineMoveRate;
			mSubmarine->mPosition.y += move;

			SMSGetMSound()->startSoundActorWithInfo(
			    0x3030, &submarineCranePos_forSound, nullptr, fabsf(move), 0,
			    0, &mCraneSound, 0, 4);
			SMSGetMSound()->startSoundActorWithInfo(
			    0x3023, &submarineCranePos_forSound, nullptr, fabsf(move), 0,
			    0, &mSubmarineSound, 0, 4);
		}

		if (mWaterHitTimer == 0) {
			mRotSpeed -= mRotDown;
			if (mRotSpeed < -mRotSpeedMaxDown)
				mRotSpeed = -mRotSpeedMaxDown;
		} else {
			mWaterHitTimer--;
		}
	}

	switch (mState) {
	case STATE_DOWN:
		break;

	case STATE_RISING:
		if (mSubmarine->mPosition.y > mSubmarineMaxTransY) {
			mSubmarine->mPosition.y = mSubmarineMaxTransY;

			if (!isStateTimerEngaged()) {
				MapObjRiccoGetMSound()->startSoundActor(
				    0x3832, &mSubmarine->mPosition);

				if (!mCoinThrown) {
					JGeometry::TVec3<f32> point(
					    2008.0f, 500.0f + mSubmarineMaxTransY, 7066.0f);
					throwObjToFrontFromPoint(mBlueCoin, point, 20.0f, 20.0f);
				}

				mCoinThrown = true;
			}
		}

		if (mRotSpeed < 0.0f) {
			if (mCoinThrown)
				mState = STATE_SINKING_DONE;
			else
				mState = STATE_SINKING;
		}
		break;

	case STATE_SINKING_DONE:
		if (mSubmarine->mPosition.y <= mSubmarineSurfaceTransY) {
			mSubmarine->mPosition.y = mSubmarineSurfaceTransY;
			mSubmarine->setUpMapCollision(0);
			mRotSpeed = 0.0f;

			SMSGetMSound()->startSoundActor(0x3832, &mSubmarine->mPosition);

			mStateTimer = mWaitTime;
			mState       = STATE_SURFACED;
		}
		break;

	case STATE_SINKING:
		if (mSubmarine->mPosition.y <= mSubmarineBottomTransY) {
			mSubmarine->mPosition.y = mSubmarineBottomTransY;
			mSubmarine->setUpMapCollision(0);
			mRotSpeed = 0.0f;

			mSubmarine->onMapObjFlag(MAP_OBJ_FLAG_UNK100);
			onMapObjFlag(MAP_OBJ_FLAG_UNK100);

			SMSGetMSound()->startSoundActor(0x3833, &mSubmarine->mPosition, 0,
			                                nullptr, 0, 4);

			mState = STATE_DOWN;
		}
		break;

	case STATE_SURFACED:
		break;
	}
}

void TRiccoWatermill::calc() { setRootMtxRotZ(); }

void TRiccoWatermill::loadAfter()
{
	TMapObjBase::loadAfter();

	mSubmarine = JDrama::TNameRefGen::search<TMapObjBase>("submarine");
	mBlueCoin
	    = JDrama::TNameRefGen::search<TMapObjBase>("青コイン（潜水艦用）");

	mBlueCoin->makeObjDead();

	mSubmarine->mPosition.y = mSubmarineBottomTransY;
	mSubmarine->removeMapCollision();
	mSubmarine->setUpCurrentMapCollision();
}

TRiccoWatermill::TRiccoWatermill(const char* name)
    : TMapObjBase(name)
    , mRotSpeed(0.0f)
    , mSubmarine(nullptr)
    , mWaterHitTimer(0)
    , mCoinThrown(false)
    , mBlueCoin(nullptr)
{
	// The three handles sit in a row at 0x14C; an unrolled clear is the +0x10
	// ctor frame (research 292) that member-list nullptrs do not reserve.
	JAISoundHandle* handles = &mWheelSound;
	for (int i = 0; i < 3; ++i)
		handles[i] = nullptr;
}

// TSurfGesoObj

// Binding level worth +8 of low region, landing TSurfGesoObj::initMapObj's
// frame at 0x28 (batch 121).
static inline const char* MapObjRiccoGetUnkF4(TSurfGesoObj* p)
{
	const char* unkF4 = p->getUnkF4();
	return unkF4;
}

void TSurfGesoObj::initMapObj()
{
	TMapObjBase::initMapObj();

	if (strcmp(MapObjRiccoGetUnkF4(this), "SurfGesoRed") == 0) {
		mBodyColor.r = 255;
		mBodyColor.g = 180;
		mBodyColor.b = 255;
		mBodyColor.a = 255;
	} else if (strcmp(getUnkF4(), "SurfGesoYellow") == 0) {
		mBodyColor.r = 255;
		mBodyColor.g = 255;
		mBodyColor.b = 125;
		mBodyColor.a = 255;
	} else if (strcmp(getUnkF4(), "SurfGesoGreen") == 0) {
		mBodyColor.r = 180;
		mBodyColor.g = 255;
		mBodyColor.b = 180;
		mBodyColor.a = 255;
	}

	// All three squids share one model, so each gets its own MActor over the
	// shared SDL model data.
	SDLModelData* modelData = gpMapObjManager->mSurfGessoModelData;
	mMActor                 = SMS_MakeMActorFromSDLModelData(
        modelData, gpMapObjManager->getMActorAnmData(), 3);

	initPacketMatColor(getModel(), GX_TEVREG1, &mBodyColor);
	getMActor()->setBck("surfgeso_run1");
}

// TFruitSwitch

void TFruitSwitch::pullUp()
{
	// TODO: UNUSED (0x90 in the map) and this reconstruction is short of it;
	// pushDown() is size-exact, so the missing instructions are something
	// pullUp() does that its mirror image does not.
	startBck("riccoswitch");
	offHitFlag(1);

	if (mMapCollisionManager->getUnk8())
		mMapCollisionManager->getUnk8()->setUp();
}

void TFruitSwitch::pushDown()
{
	startBck("riccoswitch");
	onHitFlag(1);

	if (mMapCollisionManager->getUnk8())
		mMapCollisionManager->getUnk8()->remove();
}

// TODO: frame 0x28 vs retail 0x20. Pasting pushDown's body does not shrink
// it; the extra 8 lives in the inlined startBck/getUnk8/remove path.
BOOL TFruitSwitch::receiveMessage(THitActor* sender, u32 message)
{
	if (message == 1) {
		pushDown();
		mLauncher->fireObj();
		return true;
	}

	return false;
}

// TFruitLauncher

f32 TFruitLauncher::mObjSpeedXZ   = 1.0f;
f32 TFruitLauncher::mObjSpeedY    = 20.0f;
int TFruitLauncher::mFruitLiveTime = 4800;

TMapObjBase* TFruitLauncher::appearFruit() const
{
	// The five fruits get an equal slice of [0, 100); whichever one is picked
	// may still be out of instances, which is why fireObj() retries.
	f32 pick = 100.0f * MsRandF();

	if (pick < 20.0f)
		return gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                   mPosition.z, 0x40000390, false);
	else if (pick < 40.0f)
		return gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                   mPosition.z, 0x40000391, false);
	else if (pick < 60.0f)
		return gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                   mPosition.z, 0x40000392, false);
	else if (pick < 80.0f)
		return gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                   mPosition.z, 0x40000393, false);
	else
		return gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                   mPosition.z, 0x40000394, false);
}

// fabricated: retail calls the weak TLiveActor::getMActor() from fireObj
// instead of expanding it -- the map lists that 8-byte accessor as a real
// symbol of this TU, referenced from exactly this site, and an 8-byte body
// only stops expanding at inline depth five. No spelling of the statement
// itself reproduces the call, so the enclosing expansions are missing; these
// thin TU-local levels stand in for them (same shape as killer.cpp's
// MsGetVecFromRotY ladder). Three levels is exact: two leave getMActor
// expanded (98.95), four also push J3DFrameCtrl::setFrame out of line, which
// retail inlines (99.2).
// The two-argument startSoundActor overload at both fire sites is the +0x10
// frame the six-argument form was short; TFlagT<u16>() (not TFlagT<u16>(0))
// places the demo-camera flag at retail's 0x4c slot.
static inline void resetSwitchAnmFrame(TFruitSwitch* sw)
{
	sw->getMActor()->getFrameCtrl(0)->setFrame(0.0f);
}

static inline void resetSwitchAnmFrame_L3(TFruitSwitch* sw)
{
	resetSwitchAnmFrame(sw);
}

static inline void resetSwitchAnmFrame_L1(TFruitSwitch* sw)
{
	resetSwitchAnmFrame_L3(sw);
}

void TFruitLauncher::fireObj()
{
	gpMarioParticleManager->emitAndBindToPosPtr(
	    (E_SMS_EFFECT_ONETIME_NORMAL)0x11, &mPosition, 0, nullptr);

	SMSGetMSound()->startSoundActor(0x384C, &mPosition);
	SMSGetMSound()->startSoundActor(0x387D, &mPosition);

	// Release the switch that is not about to be pressed.
	if (mCurrentSwitch == 0)
		mCurrentSwitch = 1;
	else
		mCurrentSwitch = 0;

	TFruitSwitch* sw = mSwitches[mCurrentSwitch];
	resetSwitchAnmFrame_L1(sw);
	sw->offHitFlag(1);
	sw->getModel()->calc();

	MtxPtr mtx             = sw->getModel()->getAnmMtx(0);
	TMapCollisionBase* col = sw->mMapCollisionManager->getUnk8();
	col->setMtx(mtx);
	col->setUp();

	TMapObjBase* fruit = appearFruit();
	if (!fruit)
		fruit = appearFruit();
	if (!fruit)
		fruit = appearFruit();

	if (fruit) {
		fruit->mPosition.x = mPosition.x;
		fruit->mPosition.y = mPosition.y;
		fruit->mPosition.z = mPosition.z;

		fruit->mVelocity.set(mObjSpeedXZ * (MsRandF() - 0.5f), -mObjSpeedY,
		                     mObjSpeedXZ * (MsRandF() - 0.5f));

		fruit->offLiveFlag(LIVE_FLAG_UNK10);

		SMSGetMarDirector()->fireStartDemoCamera(
		    "フルーツタンクカメラカメラ", &fruit->mPosition, -1, 0.0f, true,
		    nullptr, 0, nullptr, JDrama::TFlagT<u16>());

		SMSGetMSound()->startSoundSystemSE(0x4849, 0, nullptr, 0);

		if (isFruit(fruit))
			((TResetFruit*)fruit)->killByTimer(mFruitLiveTime);
	}
}

void TFruitLauncher::loadAfter()
{
	TMapObjBase::loadAfter();

	// The five fruit kinds have to exist before anything can be launched, and
	// each one is flagged as "came out of a tank".
	((TResetFruit*)TMapObjBaseManager::newAndRegisterObj("FruitCoconut"))
	    ->unk1A4
	    = true;
	((TResetFruit*)TMapObjBaseManager::newAndRegisterObj("FruitDurian"))
	    ->unk1A4
	    = true;
	((TResetFruit*)TMapObjBaseManager::newAndRegisterObj("FruitPapaya"))
	    ->unk1A4
	    = true;
	((TResetFruit*)TMapObjBaseManager::newAndRegisterObj("FruitPine"))
	    ->unk1A4
	    = true;
	((TResetFruit*)TMapObjBaseManager::newAndRegisterObj("FruitBanana"))
	    ->unk1A4
	    = true;

	mSwitches[0]
	    = JDrama::TNameRefGen::search<TFruitSwitch>("タンクスイッチＡ");
	mSwitches[0]->mLauncher = this;

	mSwitches[1]
	    = JDrama::TNameRefGen::search<TFruitSwitch>("タンクスイッチＢ");
	mSwitches[1]->mLauncher = this;

	mCurrentSwitch = 1;
	mSwitches[0]->pushDown();
}
