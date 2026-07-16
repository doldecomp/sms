#include <MoveBG/MapObjSirena.hpp>
#include <Enemy/Conductor.hpp>
#include <Map/Map.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <Player/MarioAccess.hpp>

// Definition order in this file is the REVERSE of the order symbols appear in
// mario.MAP, because MoveBG is compiled with -inline deferred.

static TSirenaRollMapObj* gpCurObject;

TSirenaRollMapObj::TSirenaRollMapObj(const char* name)
    : TMapObjBase(name)
    , unk138(nullptr)
    , unk13C(nullptr)
    , unk148(0)
    , unk14C(0.0f)
    , unk150(0.0f)
    , unk154(0.5f)
    , unk158(10.0f)
    , unk15C(0.1f)
    , unk160(0.2f)
    , unk164(1)
{
	gpCurObject = nullptr;
}

TCloset::TCloset(const char* name)
    : TSirenaRollMapObj(name)
    , mMapCollisionWarp(nullptr)
    , unk16C(false)
    , unk16D(0)
{
}

void TWarpAreaActor::perform(u32 cue, JDrama::TGraphics* graphics)
{
	THitActor::perform(cue, graphics);
	if (cue & CUE_MOVE) {
		if (getColNum() != 0) {
			if (*gpMarioSpeedY > 0.0f) {
				if (unk68 != -1)
					gpMap->changeModel(unk68);
			}
			if (*gpMarioSpeedY < 0.0f) {
				if (unk6A != -1)
					gpMap->changeModel(unk6A);
			}
		}
	}
}

void TWarpAreaActor::load(JSUMemoryInputStream& stream)
{
	THitActor::load(stream);
	u32 v;
	stream.read(&v, 4);
	unk68 = v;
	stream.read(&v, 4);
	unk6A = v;
	initHitActor(0x4000019D, 1, -0x80000000, 100.0f * mScaling.x,
	             100.0f * mScaling.y, 0.0f, 0.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	gpConductor->registerOtherObj(this);
}

TWarpAreaActor::TWarpAreaActor(const char* name)
    : THitActor(name)
    , unk68(-1)
    , unk6A(-1)
{
}

u32 TChestRevolve::touchWater(THitActor* actor)
{
	if (isState(1)) {
		mState = 2;
		startAnim(1);
		setUpMapCollision(1);
	}
	return 1;
}

void TChestRevolve::control()
{
	TMapObjBase::control();
	switch (mState) {
	case 2:
		if (animIsFinished()) {
			mState = 1;
			setUpMapCollision(0);
		}
		break;
	}
}

BOOL TPanelRevolve::receiveMessage(THitActor* actor, u32 message)
{
	if (isState(1)) {
		gpMSound->startSoundActor(MSD_SE_OBJ_PANEL_ROLL, &mPosition, 0,
		                          nullptr, 0, 4);
		mState = 2;
		startAnim(1);
		removeMapCollision();
	}
	return 1;
}

void TPanelRevolve::touchPlayer(THitActor* actor)
{
	if (marioHipAttack() && isState(1)) {
		gpMSound->startSoundActor(MSD_SE_OBJ_PANEL_ROLL, &mPosition, 0,
		                          nullptr, 0, 4);
		mState = 2;
		startAnim(1);
		removeMapCollision();
	}
}

void TPanelRevolve::control()
{
	TMapObjBase::control();
	switch (mState) {
	case 2:
		if (animIsFinished()) {
			mState = 1;
			setUpCurrentMapCollision();
		}
		break;
	}
}
