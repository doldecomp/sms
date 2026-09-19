#include <MoveBG/MapObjPinna.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Camera/cameralib.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Yoshi.hpp>
#include <Strategic/ObjManager.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TFerrisWheel

s32 TFerrisWheel::becomeCalmlyCallback(u32 param_1, u32 param_2)
{
	if (param_2 == 0) {
		TFerrisWheel* wheel = reinterpret_cast<TFerrisWheel*>(param_1);
		wheel->setState(2);

		MSound* sound = SMSGetMSound();
		if (sound->unk80) {
			sound->unk80->setVolume(0.0f, 200, 0);
			sound->unk80->setPitch(0.5f, 200, 0);
		}

		wheel->startStateTimer(120);
	}

	return 0;
}

void TFerrisWheel::control()
{
	TMapObjBase::control();

	// State 2 is "slowing down after the manta fight"; once the wheel has
	// coasted back to the idle rate it goes back to state 1 for good.
	if (isState(2) && !isStateTimerEngaged()) {
		if (mAnmRate > SMSGetAnmFrameRate() * 0.25f)
			mAnmRate -= 0.015f;
		else
			setState(1);
	}

	if (mAnmRate > SMSGetAnmFrameRate() * 0.25f) {
		MSound* sound = SMSGetMSound();
		sound->startSoundActor(MSD_SE_OBJ_MAHRE_GATE_LIGHT, &mPosition, 0,
		                       &sound->unk80, 0, 4);
	}

	f32 rate = mAnmRate;
	getMActor()->getFrameCtrl(0)->setFrame(
	    rate + getMActor()->getFrameCtrl(0)->getFrame());

	for (int i = 0; i < mGondolaNum; i++) {
		TMapObjBase* gondola = mGondolas[i];
		MtxPtr mtx           = getModel()->getAnmMtx(i + 1);
		MTXCopy(mtx, gondola->getModel()->getAnmMtx(0));
		gondola->mPosition.set(mtx[0][3], mtx[1][3] + gondola->mYOffset,
		                       mtx[2][3]);
	}
}

void TFerrisWheel::initMapObj()
{
	TMapObjBase::initMapObj();

	// One gondola per joint, minus the wheel's own root joint.
	mGondolaNum = getModel()->getModelData()->getJointNum() - 1;
	mGondolas   = new TMapObjBase*[mGondolaNum];

	for (u16 i = 0; i < mGondolaNum; i++) {
		mGondolas[i] = TMapObjBaseManager::newAndRegisterObj("FerrisGondola");
		mGondolas[i]->appear();
	}

	// The manta arena wants the wheel running much faster than usual.
	if (SMSGetMarDirector()->getCurrentStage() == 2)
		mAnmRate = 10.0f;
	else
		mAnmRate = SMSGetAnmFrameRate() * 0.25f;
}

TFerrisWheel::TFerrisWheel(const char* name)
    : TMapObjBase(name)
    , mGondolaNum(0)
    , mGondolas(nullptr)
    , mAnmRate(0.0f)
{
}

// THorizontalViking

void THorizontalViking::updateTrans()
{
	mPosition.x = getInitialPosition().x
	              + mSwingRadius * sinf(3.14f * (mSwingAngle / 180.0f));

	// Named so it is read before cosf(); retail parks it in f31 across the
	// call.
	f32 yOffset = mYOffset;
	mPosition.y
	    = yOffset
	      + (getInitialPosition().y
	         + mSwingRadius * (1.0f - cosf(3.14f * (mSwingAngle / 180.0f))));
}

void THorizontalViking::moveNormal()
{
	switch (mState) {
	case STATE_SWING_DOWN:
		mSwingSpeed -= mSwingAccel;
		mSwingAngle += mSwingSpeed;
		if (mSwingAngle < 0.0f)
			mState = STATE_SWING_UP;
		break;

	case STATE_SWING_UP:
		mSwingSpeed += mSwingAccel;
		mSwingAngle += mSwingSpeed;
		if (mSwingAngle > 0.0f)
			mState = STATE_SWING_DOWN;
		break;
	}
}

void THorizontalViking::control()
{
	TMapObjBase::control();
	moveNormal();
	updateTrans();
}

void THorizontalViking::reset()
{
	mSwingSpeed = mSwingSpeedInit;
	mSwingAngle = 0.0f;

	if (mSwingSpeed > 0.0f)
		mState = STATE_SWING_DOWN;
	else
		mState = STATE_SWING_UP;
}

void THorizontalViking::initMapObj()
{
	TMapObjBase::initMapObj();

	mSwingRadius    = 2500.0f;
	mSwingAccel     = 0.0008f;
	mSwingSpeedInit = 0.23f;

	reset();
}

THorizontalViking::THorizontalViking(const char* name)
    : TMapObjBase(name)
    , mSwingRadius(0.0f)
    , mSwingAccel(0.0f)
    , mSwingSpeedInit(0.0f)
    , mSwingSpeed(0.0f)
    , mSwingAngle(0.0f)
{
}

// TViking

void TViking::roll()
{
	switch (mState) {
	case STATE_ROLL_DOWN_FWD:
		mSwingSpeed *= mSpeedGainFwd;
		mSwingSpeed -= mSwingAccel;
		mSwingAngle += mSwingSpeed;

		if (mSwingAngle < 0.0f) {
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_PIN_BIKING_WING, &mPosition, nullptr,
			    fabsf(mSwingSpeed), 0, 0, nullptr, 0, 4);
			mState = STATE_ROLL_UP_FWD;
		}

		if (mSwingAngle > 180.0f) {
			mSwingAngle -= 360.0f;
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_PIN_BIKING_WING, &mPosition, nullptr,
			    fabsf(mSwingSpeed), 0, 0, nullptr, 0, 4);
			mState = STATE_ROLL_UP_BACK;
		}
		break;

	case STATE_ROLL_UP_FWD:
		mSwingSpeed *= mSpeedGainFwd;
		mSwingSpeed += mSwingAccel;
		mSwingAngle += mSwingSpeed;

		if (mSwingAngle > 0.0f) {
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_PIN_BIKING_WING, &mPosition, nullptr,
			    fabsf(mSwingSpeed), 0, 0, nullptr, 0, 4);
			mState = STATE_ROLL_DOWN_FWD;
		}

		if (mSwingAngle < -180.0f) {
			mSwingAngle += 360.0f;
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_PIN_BIKING_WING, &mPosition, nullptr,
			    fabsf(mSwingSpeed), 0, 0, nullptr, 0, 4);
			mState = STATE_ROLL_DOWN_BACK;
		}
		break;

	case STATE_ROLL_DOWN_BACK:
		mSwingSpeed *= mSpeedGainBack;
		mSwingSpeed -= mSwingAccel;
		mSwingAngle += mSwingSpeed;

		if (mSwingAngle < 0.0f) {
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_PIN_BIKING_WING, &mPosition, nullptr,
			    fabsf(mSwingSpeed), 0, 0, nullptr, 0, 4);

			// Too slow to keep looping: fall back to swinging.
			if (mSwingSpeed > -mSwingSpeedMin)
				mState = STATE_ROLL_UP_FWD;
			else
				mState = STATE_ROLL_UP_BACK;
		}
		break;

	case STATE_ROLL_UP_BACK:
		mSwingSpeed *= mSpeedGainBack;
		mSwingSpeed += mSwingAccel;
		mSwingAngle += mSwingSpeed;

		if (mSwingAngle > 0.0f) {
			SMSGetMSound()->startSoundActorWithInfo(
			    MSD_SE_OBJ_PIN_BIKING_WING, &mPosition, nullptr,
			    fabsf(mSwingSpeed), 0, 0, nullptr, 0, 4);

			if (mSwingSpeed < mSwingSpeedMin)
				mState = STATE_ROLL_DOWN_FWD;
			else
				mState = STATE_ROLL_DOWN_BACK;
		}
		break;
	}
}

void TViking::control()
{
	switch (mMode) {
	case MODE_SWING:
		moveNormal();
		break;

	case MODE_ROLL:
		roll();
		break;
	}

	updateTrans();
	mRotation.z = mSwingAngle;
	updateObjMtx();
}

void TViking::reset()
{
	mSwingSpeed = mSwingSpeedInit;
	mSwingAngle = 0.0f;

	if (mSwingSpeedInit > 0.0f)
		mState = STATE_ROLL_DOWN_FWD;
	else
		mState = STATE_ROLL_UP_FWD;
}

void TViking::loadAfter()
{
	TMapObjBase::loadAfter();
	reset();
}

void TViking::initMapObj()
{
	mMode = MODE_ROLL;

	// Only the first ship of the pair starts out swinging backwards.
	if (strcmp(getName(), "viking 0") == 0) {
		mSwingRadius    = 1400.0f;
		mSwingAccel     = 0.001f;
		mSpeedGainFwd   = 1.001f;
		mSpeedGainBack  = 0.999f;
		mSwingSpeedInit = -0.3f;
		mSwingSpeedMin  = 0.3f;
	} else {
		mSwingRadius    = 1400.0f;
		mSwingAccel     = 0.001f;
		mSpeedGainFwd   = 1.001f;
		mSpeedGainBack  = 0.999f;
		mSwingSpeedInit = 0.3f;
		mSwingSpeedMin  = 0.3f;
	}

	mPosition.y -= mSwingRadius;

	TMapObjBase::initMapObj();
}

TViking::TViking(const char* name)
    : THorizontalViking(name)
    , mMode(MODE_SWING)
    , mSwingSpeedMin(0.0f)
    , mSpeedGainFwd(0.0f)
    , mSpeedGainBack(0.0f)
{
}

// TPinnaShell

void TPinnaShell::opened()
{
	mRotX  = -TShellCup::mOpenRotMax;
	mTimer = 360;

	if (mContent && !mContent->checkLiveFlag(LIVE_FLAG_DEAD)) {
		if (mContent->isActorType(0x20000010))
			SMSGetMSound()->startSoundSystemSE(0x483F, 0, nullptr, 0);
		else
			SMSGetMSound()->startSoundSystemSE(0x4813, 0, nullptr, 0);
	} else {
		SMSGetMSound()->startSoundSystemSE(0x483D, 0, nullptr, 0);
	}

	mState = STATE_OPEN;
}

BOOL TPinnaShell::receiveMessage(THitActor* sender, u32 message)
{
	if (message == 0xF) {
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT,
		                             &sender->mPosition, 0, nullptr);
		SMSGetMSound()->startSoundSet(0x6802, &mPosition, 0, 0.0f, 0, 0, 4);

		if (mState == STATE_CLOSED) {
			mRotX -= TShellCup::mWaterOpenAccel;
			if (mRotX < -TShellCup::mOpenRotMax)
				mState = STATE_OPENING;
		}

		return true;
	}

	return false;
}

void TPinnaShell::control()
{
	if (mTimer > 0)
		mTimer--;

	switch (mState) {
	case STATE_CLOSED:
		// Drifting shut again at a randomised rate.
		if (mRotX < 0.0f)
			mRotX += TShellCup::mCloseAccel * (0.5f * MsRandF() + 0.5f);
		else
			mRotX = 0.0f;
		break;

	case STATE_OPENING:
		mRotX -= 0.8f;
		if (mRotX < -TShellCup::mOpenRotMax)
			opened();
		break;

	case STATE_OPEN:
		if (mTimer <= 0) {
			mState = STATE_CLOSING;
			SMSGetMSound()->startSoundActor(0x389F, &mPosition, 0, nullptr, 0,
			                                4);
		}
		break;

	case STATE_CLOSING:
		mRotX += mCloseSpeed;
		if (mRotX >= -TShellCup::mShellDamageRot)
			mDamageObj->offHitFlag(1);
		if (mRotX >= 0.0f) {
			mRotX  = 0.0f;
			mState = STATE_CLOSED;
			mDamageObj->onHitFlag(1);
		}
		break;
	}

	// The shell hangs off the cup's joint, pulled 30% of the way back towards
	// the cup's centre and 100 units down.
	mPosition.set(0.7f * (mJointMtx[0][3] - mOwner->mPosition.x)
	                  + mOwner->mPosition.x,
	              mJointMtx[1][3] - 100.0f,
	              0.7f * (mJointMtx[2][3] - mOwner->mPosition.z)
	                  + mOwner->mPosition.z);
	mDamageObj->mPosition.set(mPosition);

	if (getColNum() != 0) {
		Mtx mtx;
		MsMtxSetRotX(mtx, mRotX);
		TMapObjBase::concatOnlyRotFromRight(mJointMtx, mtx, mtx);
		mCollision->moveMtx(mtx);
	}
}

TPinnaShell::TPinnaShell(const char* name)
    : THitActor(name)
    , mState(STATE_CLOSED)
    , mRotX(0.0f)
    , mCloseSpeed(0.0f)
    , mJointMtx(nullptr)
    , mJoint(nullptr)
    , mTimer(0)
    , mContent(nullptr)
    , mCollision(nullptr)
    , mDamageObj(nullptr)
    , mOwner(nullptr)
{
	initHitActor(0x4000013A, 1, 0x80000000, 250.0f, 400.0f, 250.0f, 200.0f);
}

// TShellCup

f32 TShellCup::mOpenRotMax      = 90.0f;
// TODO: UNUSED, so the value is unknowable; it only has to be non-zero to land
// in .sdata next to its neighbours.
f32 TShellCup::mAutoOpenRot     = 45.0f;
f32 TShellCup::mShellDamageRot  = 45.0f;
f32 TShellCup::mWaterOpenAccel  = 5.0f;
f32 TShellCup::mCloseAccel      = 3.5f;
// TODO: UNUSED; 360 is the value TPinnaShell::opened() puts in mTimer.
int TShellCup::mWaitTimeToClose = 360;

void TShellCup::control()
{
	getMActor()->calc();

	for (int i = 0; i < 6; i++)
		mShells[i].control();
}

void TShellCup::attachCoin(TCoin* coin, int index)
{
	// TODO: UNUSED (0x30 in the map) and this reconstruction is 0x1c. The
	// missing four instructions are probably a second statement on the coin.
	mShells[index].mContent = coin;
}

void TShellCup::calcAfter()
{
	// UNUSED in the map: perform() is its only caller and inlines it. That
	// extra level is what keeps MsMtxSetRotX a call there while
	// TPinnaShell::control() expands it in place.
	for (int i = 0; i < 6; i++)
		mShells[i].calcJointMtx();

	if (!mBlueCoin->checkLiveFlag(LIVE_FLAG_DEAD)) {
		mBlueCoin->mPosition.x = mShells[0].mPosition.x;
		mBlueCoin->mPosition.y = mShells[0].mPosition.y;
		mBlueCoin->mPosition.z = mShells[0].mPosition.z;
	}

	if (!mCoin0->checkLiveFlag(LIVE_FLAG_DEAD)) {
		mCoin0->mPosition.x = mShells[2].mPosition.x;
		mCoin0->mPosition.y = mShells[2].mPosition.y;
		mCoin0->mPosition.z = mShells[2].mPosition.z;
	}

	if (!mCoin1->checkLiveFlag(LIVE_FLAG_DEAD)) {
		mCoin1->mPosition.x = mShells[4].mPosition.x;
		mCoin1->mPosition.y = mShells[4].mPosition.y;
		mCoin1->mPosition.z = mShells[4].mPosition.z;
	}
}

void TShellCup::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TMapObjBase::perform(cue, graphics);

	if (cue & 2) {
		// Frozen during conversations, but demo cameras still need the shells
		// and their coins placed.
		if (gpMarDirector->isTalkModeNow()) {
			if (!gpMarDirector->isDemoModeNow())
				return;
		}

		for (int i = 0; i < 6; i++) {
			TPinnaShell* shell = &mShells[i];
			shell->calcJointMtx();
		}

		TMapObjBase* blueCoin = mBlueCoin;
		if (!blueCoin->checkLiveFlag(LIVE_FLAG_DEAD)) {
			blueCoin->mPosition.x = mShells[0].mPosition.x;
			blueCoin->mPosition.y = mShells[0].mPosition.y;
			blueCoin->mPosition.z = mShells[0].mPosition.z;
		}

		TCoin* coin0 = mCoin0;
		if (!coin0->checkLiveFlag(LIVE_FLAG_DEAD)) {
			coin0->mPosition.x = mShells[2].mPosition.x;
			coin0->mPosition.y = mShells[2].mPosition.y;
			coin0->mPosition.z = mShells[2].mPosition.z;
		}

		TCoin* coin1 = mCoin1;
		if (!coin1->checkLiveFlag(LIVE_FLAG_DEAD)) {
			coin1->mPosition.x = mShells[4].mPosition.x;
			coin1->mPosition.y = mShells[4].mPosition.y;
			coin1->mPosition.z = mShells[4].mPosition.z;
		}
	}
}

// Binding level worth +8 of low region, landing TShellCup::loadAfter's frame
// at 0x68 (batch 121).
static inline u32 MapObjPinnaGetEventId(TMapObjBase* p)
{
	u32 eventId = p->getEventId();
	return eventId;
}

void TShellCup::loadAfter()
{
	TMapObjBase::loadAfter();

	for (int i = 0; i < 6; i++)
		joinToGroup("オブジェクトグループ", mShells[i].mDamageObj);

	mBlueCoin = TMapObjBaseManager::newAndRegisterObj("coin_blue");
	mCoin0    = gpItemManager->newAndRegisterCoinReal();
	mCoin1    = gpItemManager->newAndRegisterCoinReal();

	mBlueCoin->setEventId(2);
	if (!TFlagManager::smInstance->getBlueCoinFlag(
	        gpMarDirector->getCurrentMap(), MapObjPinnaGetEventId(mBlueCoin))) {
		mBlueCoin->makeObjAppeared();
		mShells[0].mContent = mBlueCoin;
	}

	mCoin0->makeObjAppeared();
	mCoin0->onMapObjFlag(TMapObjBase::MAP_OBJ_FLAG_UNK10000000);
	mCoin1->makeObjAppeared();
	mCoin1->onMapObjFlag(TMapObjBase::MAP_OBJ_FLAG_UNK10000000);

	mShells[2].mContent = mCoin0;
	mShells[4].mContent = mCoin1;
}

void TShellCup::initMapObj()
{
	TMapObjBase::initMapObj();

	for (int i = 0; i < 6; i++) {
		mShells[i].mState      = TPinnaShell::STATE_CLOSED;
		mShells[i].mRotX       = 0.0f;
		mShells[i].mCloseSpeed = 8.0f;
		mShells[i].mJointMtx   = getModel()->getAnmMtx(i + 1);
		mShells[i].mJoint
		    = getModel()->getModelData()->getJointNodePointer(i + 1);
		mShells[i].mTimer = (int)(1200.0f * MsRandF());
		mShells[i].mOwner = this;

		joinToGroup("オブジェクトグループ", &mShells[i]);

		mShells[i].mCollision = new TMapCollisionMove();
		mShells[i].mCollision->init("/mapObj/ShellCup", 0, this);
		mShells[i].mCollision->setUp();

		mShells[i].mDamageObj = new TDamageObj();
		mShells[i].mDamageObj->mScaling.set(2.0f, 1.2f, 2.0f);
		mShells[i].mDamageObj->init(0x10000036);
		mShells[i].mDamageObj->onHitFlag(1);
	}

	TMapCollisionStatic* rink = new TMapCollisionStatic();
	rink->init("/mapObj/ShellCup_rink", 2, this);
	rink->setMtx(getModel()->getAnmMtx(0));
	rink->setUp();
}

TShellCup::TShellCup(const char* name)
    : TMapObjBase(name)
    , mBlueCoin(nullptr)
    , mCoin0(nullptr)
    , mCoin1(nullptr)
{
}

// TMerrygoround

f32 TMerrygoround::mRotSpeed = 0.1f;

void TMerrygoround::control()
{
	TMapObjBase::control();

	mRotation.y += mRotSpeed;
	if (mRotation.y > 360.0f)
		mRotation.y -= 360.0f;

	for (int i = 0; i < 2; i++) {
		u16 joint  = mEggJoints[i];
		MtxPtr mtx = getModel()->getAnmMtx(joint);
		mEggs[i]->setModelMtx(mtx);
		mEggs[i]->mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);
	}

	for (int i = 0; i < 9; i++) {
		u16 joint  = mPoleJoints[i];
		MtxPtr mtx = getModel()->getAnmMtx(joint);
		mPoles[i]->unk138.set(mtx);
		mPoles[i]->mPosition.set(mtx[0][3], mtx[1][3] - 600.0f, mtx[2][3]);
	}

	// The warp only accepts Mario while he is riding Yoshi.
	if (SMS_IsMarioOnYoshi())
		mWarp->offHitFlag(1);
	else
		mWarp->onHitFlag(1);

	u16 joint  = mWarpJoint;
	MtxPtr mtx = getModel()->getAnmMtx(joint);
	mWarp->mPosition.set(mtx[0][3], mtx[1][3] - 600.0f, mtx[2][3]);
}

void TMerrygoround::draw() const { }

// Binding level over a raw member read, worth +8 of low region in
// TMerrygoround::initMapObj (batch 127).
static inline TMapObjChangeStage* MapObjPinnaWarp(const TMerrygoround* p)
{
	TMapObjChangeStage* warp = p->mWarp;
	return warp;
}

void TMerrygoround::initMapObj()
{
	TMapObjBase::initMapObj();

	int eggNum  = 0;
	int poleNum = 0;

	for (u16 i = 1; i < getModel()->getModelData()->getJointNum(); i++) {
		const char* name
		    = getModel()->getModelData()->getJointName()->getName(i);

		if (strstr(name, "egg")) {
			mEggJoints[eggNum] = i;
			eggNum++;
		} else if (strcmp(name, "yoshi_warp") == 0) {
			mWarpJoint = i;
		} else if (strcmp(name, "up") != 0 && strcmp(name, "down") != 0
		           && strcmp(name, "KAGE_2") != 0) {
			mPoleJoints[poleNum] = i;
			poleNum++;
		}
	}

	for (int i = 0; i < 2; i++) {
		mEggs[i] = TMapObjBaseManager::newAndRegisterObj("merry_egg");
		mEggs[i]->appear();
	}

	for (int i = 0; i < 9; i++) {
		mPoles[i] = (TMerryPole*)TMapObjBaseManager::newAndRegisterObj(
		    "merry_pole");
		mPoles[i]->appear();

		mPoleCollisions[i] = new TMapCollisionMove();
		mPoleCollisions[i]->init("/scene/mapObj/merry_yoshi.col", 0, this);
		mPoleCollisions[i]->setUp();
	}

	mWarp = (TMapObjChangeStage*)TMapObjBaseManager::newAndRegisterObj(
	    "ChangeStageMerrygoround");
	mWarp->unk138 = 0x29;
	mWarp->mScaling.y *= 1.5f;
	MapObjPinnaWarp(this)->makeObjAppeared();
	joinToGroup("マップグループ", mWarp);
}

TMerrygoround::TMerrygoround(const char* name)
    : TMapObjBase(name)
{
	mWarp      = nullptr;
	mWarpJoint = 0;

	for (int i = 0; i < 2; i++) {
		mEggs[i]      = nullptr;
		mEggJoints[i] = 0;
	}

	for (int i = 0; i < 9; i++) {
		mPoles[i]          = nullptr;
		mPoleJoints[i]     = 0;
		mPoleCollisions[i] = nullptr;
	}
}

// TChangeStageMerrygoround

void TChangeStageMerrygoround::touchPlayer(THitActor* sender)
{
	if (!isStateTimerEngaged()) {
		if (SMS_GetYoshi()->mType == 1) {
			SMSGetMSound()->startSoundSystemSE(0x4840, 0, nullptr, 0);
			TMapObjChangeStage::touchPlayer(sender);
			unk13C = 1;
		} else {
			SMSGetMSound()->startSoundSystemSE(0x483E, 0, nullptr, 0);
		}

		startStateTimer(600);
	}
}

void TChangeStageMerrygoround::calc()
{
	if (unk13C) {
		gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_HIKAGE1_A, &SMS_GetMarioPos(), 1, this);
		gpMarioParticleManager->emitAndBindToPosPtr(
		    (E_SMS_EFFECT_LOOP_NORMAL)0x101, &SMS_GetMarioPos(), 1, this);
	}
}

// TBalloonKoopaJr

void TBalloonKoopaJr::touchActor(THitActor* sender) { kill(); }

void TBalloonKoopaJr::kill()
{
	TMapObjGeneral::kill();

	// TODO: these three ids are the .jpa files load() registers; they have no
	// name in System/Particles.hpp yet.
	emitAndScale(0x5A, 0, &mCenterPos);
	emitAndScale(0x5B, 0, &mCenterPos);
	emitAndScale(0x5C, 0, &mCenterPos);

	TFlagManager::smInstance->incFlag(0x60001, 1);

	SMSGetMSound()->startSoundActor(0x28B8, &mPosition);
}

void TBalloonKoopaJr::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);

	SMS_LoadParticle("/scene/mapObj/balloonKoopaJr.jpa", 0x5A);
	SMS_LoadParticle("/scene/mapObj/balloonKoopaJrA.jpa", 0x5B);
	SMS_LoadParticle("/scene/mapObj/balloonKoopaJrB.jpa", 0x5C);

	s32 joint = getModel()->getModelData()->getJointName()->getIndex("center");
	MtxPtr mtx = getModel()->getAnmMtx((u16)joint);
	mCenterPos.set(mtx[0][3], mtx[1][3], mtx[2][3]);
}

// TPinnaEntrance

void TPinnaEntrance::loadAfter()
{
	TMapObjBase::loadAfter();

	// TODO: 96.6%. Retail lays the defaulted scale temporary out below the
	// explicit rotation one; passing the scale explicitly does not swap them.
	TMapObjBaseManager::newAndRegisterObj(
	    "GateManta", getPosition(), JGeometry::TVec3<f32>(90.0f, 0.0f, 0.0f));
}

// TWaterRecoverObj

void TWaterRecoverObj::touchPlayer(THitActor* sender)
{
	if (sender->isActorType(0x80000001) && !isStateTimerEngaged()) {
		sender->receiveMessage(this, 0xE);
		startStateTimer(600);
	}
}

// TAmiKing

void TAmiKing::loadAfter()
{
	TMapObjBase::loadAfter();
	SMS_LoadParticle("/scene/mapObj/amiking.jpa", 0x184);
}

void TAmiKing::initMapObj()
{
	TMapObjBase::initMapObj();

	initAnmSound();
	getMActor()->setBck("amiking_sleep1");
	setAnmSound("/scene/mapObj/amiking_sleep1.bas");
	offLiveFlag(LIVE_FLAG_UNK10);

	// TODO: the ROM really does walk every joint with an empty body here; the
	// statement that used to be inside was dead-stripped or optimised away.
	for (u8 i = 0;
	     i < getMActor()->getModel()->getModelData()->getJointNum(); i++) {
		;
	}
}

// Parked here: retail materialises each of the four `isState` results and then
// the whole `||` chain once more before the `if`, which is what a predicate
// with a two-`return` body emits. It belongs on TMapObjBase in MapObjBase.hpp.
// TODO: retail reloads `mState` between the first and the second `isState`
// where this form keeps the first load; the remaining instruction.
static inline bool MapObjPinnaIsGateBroken(TMapObjBase* gate)
{
	if (gate->isState(TMapObjGeneral::STATE_BREAKING)
	    || gate->isState(TMapObjGeneral::STATE_TOUCHING_WATER)
	    || gate->isState(TMapObjGeneral::STATE_TOUCHING_PLAYER)
	    || gate->isState(TMapObjGeneral::STATE_HOLDING))
		return true;
	return false;
}

void TAmiKing::moveObject()
{
	TLiveActor::moveObject();

	if (mFlying) {
		if (getMActor()->checkCurAnm("amiking_flying1_start", 0)) {
			if (getMActor()->curAnmEndsNext(0, nullptr))
				getMActor()->setBck("amiking_flying1_loop");
		} else if (mGroundPlane->isWaterSurface() && !isAirborne()) {
			SMSGetMSound()->startSoundActor(0x2921, &mPosition, 0, nullptr, 0,
			                                4);

			JPABaseEmitter* splash
			    = gpMarioParticleManager->emitAndBindToMtxPtr(
			        PARTICLE_MS_DNK_SHIBIRE_B,
			        getMActor()->getModel()->getAnmMtx(0), 0, nullptr);
			if (splash)
				splash->setGlobalScale(
				    JGeometry::TVec3<f32>(4.0f, 4.0f, 4.0f));

			TSpineEnemy* column = gpConductor->makeOneEnemyAppear(
			    mPosition, "エフェクト水柱マネージャー", 1);
			if (column) {
				JGeometry::TVec3<f32> scale(4.0f, 4.0f, 4.0f);
				((TEffectColumWater*)column)->generate(mPosition, scale);
			}

			gpItemManager->makeShineAppearWithDemo(
			    "シャイン（観覧車シャイン用）", "観覧車シャインカメラ",
			    mPosition.x, mPosition.y, mPosition.z);

			TFerrisWheel* wheel
			    = JDrama::TNameRefGen::search<TFerrisWheel>("FerrisWheel");
			SMSGetMarDirector()->fireStartDemoCamera(
			    "観覧車正常化カメラ", &wheel->mPosition, -1, 0.0f, true,
			    &TFerrisWheel::becomeCalmlyCallback,
			    reinterpret_cast<u32>(wheel), nullptr,
			    JDrama::TFlagT<u16>(0));

			kill();
		}
	} else if (mGroundPlane->mActor) {
		// Wake up when the gate the net is sitting on gets broken.
		TMapObjBase* gate = (TMapObjBase*)mGroundPlane->mActor;
		if (gate->mActorType == 0x4000006A) {
			if (MapObjPinnaIsGateBroken(gate)) {
				mFlying = true;

				mVelocity.x = 5.0f;
				mVelocity.y = 10.0f;
				mVelocity.z = -10.0f;

				offLiveFlag(LIVE_FLAG_UNK10);
				getMActor()->setBck("amiking_flying1_start");
				setAnmSound(nullptr);

				SMSGetMarDirector()->fireStartDemoCamera(
				    "観覧車ボス撃沈カメラ", &mPosition, -1, 0.0f, true, nullptr,
				    0, nullptr, JDrama::TFlagT<u16>(0));
			}
		}
	}
}

// Binding level over a raw member read, worth +16 of low region in
// TAmiKing::calc (batch 127).
static inline TMarioParticleManager* MapObjPinnaGetMarioParticleManager()
{
	TMarioParticleManager* marioParticleManager = gpMarioParticleManager;
	return marioParticleManager;
}

void TAmiKing::calc()
{
	MapObjPinnaGetMarioParticleManager()->emitAndBindToMtxPtr(
	    (E_SMS_EFFECT_LOOP_NORMAL)0x184, getModel()->getAnmMtx(0), 1, this);

	if (mFlying == 0) {
		MtxPtr joint = getMActor()->getModel()->getAnmMtx(6);
		mEffectPos.set(joint[0][3], joint[1][3], joint[2][3]);

		JGeometry::TVec3<f32> offset(0.0f, 0.0f, 200.0f);
		Mtx mtx;
		MsMtxSetRotRPH(mtx, 0.0f, mRotation.y, 0.0f);
		MTXMultVec(mtx, &offset, &offset);

		mEffectPos.x += offset.x;
		mEffectPos.y += offset.y;
		mEffectPos.z += offset.z;

		JPABaseEmitter* zzz = MapObjPinnaGetMarioParticleManager()->emitAndBindToPosPtr(
		    PARTICLE_MS_POI_ZZZ, &mEffectPos, 1, this);
		if (zzz)
			zzz->setGlobalScale(JGeometry::TVec3<f32>(2.0f, 2.0f, 2.0f));

		SMSGetMSound()->startSoundActor(0x214F, &mPosition, 0, nullptr, 0, 4);
	} else {
		SMSGetMSound()->startSoundActor(0x2120, &mPosition, 0, nullptr, 0, 4);
	}
}

void TAmiKing::bind()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		gpMap->checkGround(mPosition.x, mPosition.y + mHeadHeight, mPosition.z,
		                   &mGroundPlane);
	else
		TLiveActor::bind();
}

void TAmiKing::touchPlayer(THitActor* sender)
{
	SMS_SendMessageToMario(this, 9);
}

// TPinnaCoaster

static int switchSnd;

void TPinnaCoaster::control()
{
	TMapObjBase::control();

	mRail->frameUpdate();
	mRail->calc();

	MtxPtr railMtx = mRail->getModel()->getAnmMtx(0);
	// TODO: 98.7%. Retail splits the destination into `mr r4, r3` + `addi r4,
	// r4, 0x20` and has 16 more bytes of frame, so getBaseTRMtx() sat behind
	// one more inline level here; a named J3DModel* local is not it (no
	// instruction change, -8 of frame).
	MTXCopy(railMtx, getModel()->getBaseTRMtx());

	getMActor()->frameUpdate();
	getMActor()->calc();

	MtxPtr mtx = getModel()->getAnmMtx(0);
	mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);

	// Same idiom as TMario::soundTorocco, which drives the same sound id.
	f32 speed = JGeometry::TVec3<f32>(mPosition - mPrevPos).length();

	// Only every other frame, so two coaster cars don't fight over the channel.
	if (switchSnd) {
		SMSGetMSound()->startSoundActorWithInfo(MSD_SE_OBJ_JET_COASTER,
		                                        &mPosition, nullptr, speed, 0,
		                                        0, nullptr, 0, 4);
	}
	switchSnd ^= 1;

	mPrevPos.set(mPosition);
}

void TPinnaCoaster::initMapObj()
{
	TMapObjBase::initMapObj();

	mRail = SMS_MakeMActorWithAnmData("/scene/mapObj/CoasterRail.bmd",
	                                  getManager()->getMActorAnmData(), 3,
	                                  0x10210000);
	mRail->setBck("coasterrail");

	MsMtxSetXYZRPH(mRail->getModel()->getBaseTRMtx(), mPosition.x, mPosition.y,
	               mPosition.z, mRotation.x, mRotation.y, mRotation.z);

	mRail->getFrameCtrl(0)->setRate(0.25f * SMSGetAnmFrameRate());

	mPrevPos.set(mPosition);
}

TPinnaCoaster::TPinnaCoaster(const char* name)
    : TMapObjBase(name)
{
	mRail = nullptr;
	mPrevPos.zero();
}

// The map records every destructor in this file as weak, so they are all
// defined in the class body; MWCC emits them here because the vtables refer to
// them.
