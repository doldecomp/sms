#include "MoveBG/MapObjCorona.hpp"
#include "MoveBG/MapObjBase.hpp"
#include <Camera/CameraShake.hpp>
#include <GC2D/GCConsole2.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MSound/MSound.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioAccess.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>

class TKoopa : public JDrama::TNameRef {
public:
	TKoopa(const char*);
	void getDown();
	void stagger(bool);
};

class TBathtubParams {
public:
	s32 unk0[21];
	u32 unk54;
	u32 unk58[4];
	u32 unk68;
	u32 unk6C[4];
	u32 unk7C;
	u32 unk80[29];
	u32 unkF4;
	u32 unkF8[59];
	f32 unk1E4;
	u32 unk1E8[4];
	f32 unk1F8;
};

void TBathtub::loadAfter()
{
	SMS_LoadParticle("/scene/map/map/ms_lkp_yuge1.jpa", 0x1be);
	SMS_LoadParticle("/scene/map/map/ms_kp_funsui.jpa", 0x1bf);
	SMS_LoadParticle("/scene/map/map/ms_kp_break_a.jpa", 0xf6);
	SMS_LoadParticle("/scene/map/map/ms_kp_break_b.jpa", 0xf7);
}

// Preserve the out-of-line call sites emitted for receiveMessage.
#pragma dont_inline on
void TBathtub::hipdrop(const JGeometry::TVec3<f32>& position)
{
	if (unk29A || unk250 > unk16C->unk7C)
		return;

	JGeometry::TVec3<f32> direction(position.x - mPosition.x, 0.0f,
	                                position.z - mPosition.z);
	direction.normalize();

	unk250 = unk16C->unk7C;
	unk258 = unk16C->unk80[4];
	unk25C = unk16C->unk80[4];
	unk254 = unk16C->unk7C;

	TKoopa* koopa = JDrama::TNameRefGen::search<TKoopa>("クッパ");
	koopa->stagger(false);
}
#pragma dont_inline off

void TBathtub::quake(const JGeometry::TVec3<f32>& position)
{
	if (unk29A)
		return;

	JGeometry::TVec3<f32> direction(position.x - mPosition.x, 0.0f,
	                                position.z - mPosition.z);
	direction.normalize();

	unk24C = 300;
	unk250 = unk16C->unk54;
	unk258 = unk16C->unk68;
	unk25C = unk16C->unk68;
	unk254 = unk16C->unk7C;
	unk248 = unk16C->unkF4;

	TKoopa* koopa = JDrama::TNameRefGen::search<TKoopa>("クッパ");
	gpCameraShake->startShake((EnumCamShakeMode)0x25, 1.0f);
	gpCameraShake->startShake((EnumCamShakeMode)0x26, 1.0f);
	SMSRumbleMgr->start(4, (f32*)nullptr);
	SMS_ThrowMario(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f), 40.0f);
	koopa->getDown();
}

u8 TBathtub::getNumGripsDead() const { return 0; }

void TBathtub::tumble(f32, f32) { }

MtxPtr TBathtub::getTakingMtx()
{
	return mMActor->getModel()->getAnmMtx(mMarioJntIdx);
}

MtxPtr TBathtub::getSubmarineMtxInDemo()
{
	return mMActor->getModel()->getAnmMtx(mSubmarineJntIdx);
}

MtxPtr TBathtub::getPeachMtxInDemo()
{
	return mMActor->getModel()->getAnmMtx(mDuckJntIdx);
}

MtxPtr TBathtub::getKoopaJrMtxInDemo()
{
	return mMActor->getModel()->getAnmMtx(mJuniorJntIdx);
}

BOOL TBathtub::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case 0:
		if (!unk29A && unk250 <= unk16C->unk0[11]) {
			unk250 = unk16C->unk0[11];
			unk258 = unk16C->unk0[16];
			unk25C = unk16C->unk0[16];
			unk254 = unk16C->unk7C;
		}
		return true;
	case 1:
		hipdrop(*gpMarioPos);
		return true;
	case 3:
		hipdrop(*gpMarioPos);
		return true;
	default:
		return false;
	}
}

Mtx* TBathtub::getRootJointMtx() const { return nullptr; }

void TBathtub::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TMapObjBase::perform(cue, graphics);

	if (cue & 1) {
		PSMTXCopy(mMActor->getModel()->getAnmMtx(mStarJntIdx),
		          unk29C->getModel()->getBaseTRMtx());
		unk29C->getModel()->setBaseScale(
		    JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
	}

	if (cue & 1) {
		int numDead = getNumGripsDead();
		int frame   = gpMarDirector->unk58;
		switch (numDead) {
		case 0:
			if (frame >= 0x1c20) {
				if (!(unk2A0 & 2))
					gpMarDirector->getConsole()->startAppearBalloon(0xe001f,
					                                               true);
				unk2A0 |= 2;
			} else if (frame >= 0xe10) {
				if (!(unk2A0 & 1))
					gpMarDirector->getConsole()->startAppearBalloon(0xe001e,
					                                               true);
				unk2A0 |= 1;
			}
			break;
		case 1:
			if (!(unk2A0 & 4))
				gpMarDirector->getConsole()->startAppearBalloon(0xe0020, true);
			unk2A0 |= 4;
			break;
		case 2:
			if (!(unk2A0 & 8))
				gpMarDirector->getConsole()->startAppearBalloon(0xe0021, true);
			unk2A0 |= 8;
			break;
		case 3:
			if (!(unk2A0 & 0x10))
				gpMarDirector->getConsole()->startAppearBalloon(0xe0022, true);
			unk2A0 |= 0x10;
			break;
		case 4:
			if (!(unk2A0 & 0x40000))
				gpMarDirector->getConsole()->startAppearBalloon(0xe0030, true);
			unk2A0 |= 0x40000;
			break;
		case 5:
			if (!(unk2A0 & 0x20))
				gpMarDirector->getConsole()->startAppearBalloon(0xe0023, true);
			unk2A0 |= 0x20;
			break;
		}
	}

	if (cue & 2)
		unk29C->calc();
	if (cue & 4)
		unk29C->viewCalc();
	if (cue & 0x200) {
		JGeometry::TVec3<f32> position(
		    unk29C->getModel()->getBaseTRMtx()[0][3],
		    unk29C->getModel()->getBaseTRMtx()[1][3],
		    unk29C->getModel()->getBaseTRMtx()[2][3]);
		unk29C->setLightData(mGroundPlane, position);
		unk29C->entry();
	}
}

void TBathtub::control()
{
	if (unk29A) {
		if (mMActor->curAnmEndsNext(0, 0)) {
			switch (unk294++) {
			case 0:
				startBck("bath_overturn2");
				break;
			case 1:
				startBck("bath_overturn3");
				break;
			}
		}

		JGeometry::TVec3<f32> scale(0.0f, 0.0f, 0.0f);
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    0x128, unk29C->getModel()->getAnmMtx(mShineBodyJntIdx), 1, this);
		if (emitter)
			emitter->setGlobalScale(scale);
		emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    0x129, unk29C->getModel()->getAnmMtx(mShineBodyJntIdx), 1, this);
		if (emitter)
			emitter->setGlobalScale(scale);

		MtxPtr mtx = mMActor->getModel()->getAnmMtx(mStarJntIdx);
		unk200.set(mtx[0][3], mtx[1][3], mtx[2][3]);
		gpMSound->startSoundActor(MSD_SE_SHINE_EXIST, &unk200, 0, 0, 0, 4);

		calcRootMatrix();
		calcBathtubData();
		for (int i = 0; i < 30; ++i)
			unk164[i]->remove();

		TLiveActor* mario = SMS_GetMarioLiveActor();
		if (mario->receiveMessage(this, 4))
			mHolder = mario;
		gpMarioOriginal->mFaceAngle.y = 0x7fff;
		if (unk290 > 0)
			--unk290;
		return;
	}

	if (unk24C > 0)
		--unk24C;
	if (unk248 > 0)
		--unk248;

	if (unk250 > unk16C->unk7C || marioIsOn() && mHolder == 0) {
		f32 amount = unk16C->unk1E4;
		if (unk250 > 0)
			amount += unk16C->unk1F8;
		if (amount > 0.0000000001f) {
			static JGeometry::TVec3<f32> yDown(0.0f, -1.0f, 0.0f);
			JGeometry::TVec3<f32> delta(gpMarioPos->x - mPosition.x,
			                                 gpMarioPos->y - mPosition.y,
			                                 gpMarioPos->z - mPosition.z);
			JGeometry::TVec3<f32> first;
			first.cross(delta, yDown);
			unk1E8 += first.x * amount * 0.00000001f;
			unk1EC += first.y * amount * 0.00000001f;
			unk1F0 += first.z * amount * 0.00000001f;
		}
	}

	updatePosture_();
	calcRootMatrix();
	TMapObjBase::control();
	calcBathtubData();
	mMActor->getModel()->calc();

	if (mBathtubData.unk0C.x * mBathtubData.unk18.at(1, 0)
	        + mBathtubData.unk0C.y * mBathtubData.unk18.at(1, 1)
	        + mBathtubData.unk0C.z * mBathtubData.unk18.at(1, 2)
	    > 0.995f)
		gpMarioParticleManager->emit(0x1be, &unk1F4, 1, this);

	if (mHolder == 0) {
		setupCollisions_();
	} else {
		for (int i = 0; i < 30; ++i)
			unk164[i]->remove();
	}
}

void TBathtub::calcBathtubData() { }

void TBathtub::setupCollisions_() { }

void TBathtub::removeCollisions_() { } // Unused

void TBathtub::startDemo() { }

bool TBathtub::allowsTumble() const { return false; }

void TBathtub::calcRootMatrix() { }

bool TBathtub::getNearGrip(const JGeometry::TVec3<f32>&, f32, f32*) const
{
	return false;
}

u8 TBathtub::getNextJuncture(const JGeometry::TVec3<f32>&,
                             const JGeometry::TVec3<f32>&) const
{
	return 0;
}

u8 TBathtub::getNextGrip(const JGeometry::TVec3<f32>&,
                         const JGeometry::TVec3<f32>&, f32, f32*) const
{
	return 0;
}

void TBathtub::updatePosture_() { }

TBathtub::TBathtub(const char* name)
    : TMapObjBase(name)
{
}

void TBathtub::load(JSUMemoryInputStream&) { }

u8 TBathtub::getNumKillerLaunchable() const { return 0; }

bool TBathtub::isKillerAttackable() const { return false; }

u8 TBathtub::getNumKillerBurstable() const { return 0; }

// Unused
bool TBathtub::isBreaking() const { return false; }

// Unused
bool TBathtub::isKillerLaunchable() const { return false; }

// Unused
void TBathtub::showMessage(u32) { }

// Unused
u8 TBathtub::getNearJuncture(const JGeometry::TVec3<f32>&) const { return 0; }

// Unused
MtxPtr TBathtub::getKoopaMtxInDemo() { return nullptr; }

// Unused
MtxPtr TBathtub::getWaterMtx(s32) { return nullptr; }

// Unused
MtxPtr TBathtub::getShineEffectMtx() { return nullptr; }

// Unused
MtxPtr TBathtub::getShineMtx() { return nullptr; }

// Unused
void TBathtub::liftMario(const JGeometry::TVec3<f32>&) { }

// Unused
void TBathtub::trample(const JGeometry::TVec3<f32>&) { }
