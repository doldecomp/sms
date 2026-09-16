#include "MoveBG/MapObjCorona.hpp"
#include "MoveBG/MapObjBase.hpp"
#include <Camera/CameraShake.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <Player/MarioAccess.hpp>
#include <System/Particles.hpp>

class TKoopa : public JDrama::TNameRef {
public:
	TKoopa(const char*);
	void getDown();
};

class TBathtubParams {
public:
	u32 unk0[21];
	u32 unk54;
	u32 unk58[4];
	u32 unk68;
	u32 unk6C[4];
	u32 unk7C;
	u32 unk80[29];
	u32 unkF4;
};

void TBathtub::loadAfter()
{
	SMS_LoadParticle("/scene/map/map/ms_lkp_yuge1.jpa", 0x1be);
	SMS_LoadParticle("/scene/map/map/ms_kp_funsui.jpa", 0x1bf);
	SMS_LoadParticle("/scene/map/map/ms_kp_break_a.jpa", 0xf6);
	SMS_LoadParticle("/scene/map/map/ms_kp_break_b.jpa", 0xf7);
}

void TBathtub::hipdrop(const JGeometry::TVec3<f32>&) { }

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

MtxPtr TBathtub::getTakingMtx() { return nullptr; }

MtxPtr TBathtub::getSubmarineMtxInDemo() { return nullptr; }

MtxPtr TBathtub::getPeachMtxInDemo() { return nullptr; }

MtxPtr TBathtub::getKoopaJrMtxInDemo() { return nullptr; }

BOOL TBathtub::receiveMessage(THitActor* sender, u32 message) { return false; }

Mtx* TBathtub::getRootJointMtx() const { return nullptr; }

void TBathtub::perform(u32 cue, JDrama::TGraphics* graphics) { }

void TBathtub::control() { }

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
