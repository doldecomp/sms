#include "MoveBG/MapObjCorona.hpp"
#include "MoveBG/MapObjBase.hpp"
#include <M3DUtil/MActor.hpp>

void TBathtub::loadAfter() { }

void TBathtub::hipdrop(const JGeometry::TVec3<f32>&) { }

void TBathtub::quake(const JGeometry::TVec3<f32>&) { }

int TBathtub::getNumGripsDead() const { return 0; }

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

BOOL TBathtub::receiveMessage(THitActor* sender, u32 message) { return false; }

Mtx* TBathtub::getRootJointMtx() const
{
	if (unk29A)
		return (Mtx*)getModel()->getAnmMtx(0);
	return (Mtx*)getModel()->getBaseTRMtx();
}

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

bool TBathtub::isKillerAttackable() const { return unk248 <= 0; }

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
