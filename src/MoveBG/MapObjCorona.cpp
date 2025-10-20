#include "MoveBG/MapObjCorona.hpp"
#include "MoveBG/MapObjBase.hpp"

void TBathtub::loadAfter() { }

void TBathtub::hipdrop(const JGeometry::TVec3<float>&) { }

void TBathtub::quake(const JGeometry::TVec3<float>&) { }

u8 TBathtub::getNumGripsDead() const { return 0; }

void TBathtub::tumble(float, float) { }

MtxPtr TBathtub::getTakingMtx() { return nullptr; }

MtxPtr TBathtub::getSubmarineMtxInDemo() { return nullptr; }

MtxPtr TBathtub::getPeachMtxInDemo() { return nullptr; }

MtxPtr TBathtub::getKoopaJrMtxInDemo() { return nullptr; }

BOOL TBathtub::receiveMessage(THitActor* sender, u32 message) { return false; }

Mtx* TBathtub::getRootJointMtx() const { return nullptr; }

void TBathtub::perform(unsigned long, JDrama::TGraphics*) { }

void TBathtub::control() { }

void TBathtub::calcBathtubData() { }

void TBathtub::setupCollisions_() { }

void TBathtub::removeCollisions_() { } // Unused

void TBathtub::startDemo() { }

bool TBathtub::allowsTumble() const { return false; }

void TBathtub::calcRootMatrix() { }

bool TBathtub::getNearGrip(const JGeometry::TVec3<float>&, float, float*) const
{
	return false;
}

u8 TBathtub::getNextJuncture(const JGeometry::TVec3<float>&,
                             const JGeometry::TVec3<float>&) const
{
	return 0;
}

bool TBathtub::getNextGrip(const JGeometry::TVec3<float>&,
                           const JGeometry::TVec3<float>&, float, float*) const
{
	return false;
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
void TBathtub::showMessage(unsigned long) { }

// Unused
u8 TBathtub::getNearJuncture(const JGeometry::TVec3<float>&) const { return 0; }

// Unused
MtxPtr TBathtub::getKoopaMtxInDemo() { return nullptr; }

// Unused
MtxPtr TBathtub::getWaterMtx(int) { return nullptr; }

// Unused
MtxPtr TBathtub::getShineEffectMtx() { return nullptr; }

// Unused
MtxPtr TBathtub::getShineMtx() { return nullptr; }

// Unused
void TBathtub::liftMario(const JGeometry::TVec3<float>&) { }

// Unused
void TBathtub::trample(const JGeometry::TVec3<float>&) { }

TBathtub::~TBathtub() { }
