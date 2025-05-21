#include <Player/ModelWaterManager.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TModelWaterManager::perform(unsigned long, JDrama::TGraphics*) { }

void TModelWaterManager::drawRefracAndSpec() const { }

void TModelWaterManager::drawShineShadowVolume(MtxPtr) { }

void init_sphere_glist() { }

void TModelWaterManager::drawMirror(MtxPtr) { }

void TModelWaterManager::drawWaterVolume(MtxPtr) { }

void TModelWaterManager::drawSilhouette(MtxPtr) { }

void TModelWaterManager::drawTouchingMask() { }

void TModelWaterManager::drawTouching() { }

void TModelWaterManager::calcVMAll(MtxPtr) { }

void TModelWaterManager::calcVMMtxWall(MtxPtr, f32,
                                       const JGeometry::TVec3<f32>&,
                                       const JGeometry::TVec3<f32>&, MtxPtr)
{
}

void TModelWaterManager::calcVMMtxGround(MtxPtr, f32,
                                         const JGeometry::TVec3<f32>&,
                                         const JGeometry::TVec3<f32>&, MtxPtr)
{
}

void TModelWaterManager::calcDrawVtx(MtxPtr) { }

void TModelWaterManager::calcWorldMinMax() { }

void TModelWaterManager::move() { }

void TModelWaterManager::garbageCollect() { }

void TModelWaterManager::wind(const JGeometry::TVec3<f32>&) { }

void TModelWaterManager::askDoWaterHitCheck() { }

void TModelWaterManager::getPlaneVanishSpeed(const TBGCheckData*) { }

void TModelWaterManager::getPlaneFall(const TBGCheckData*) { }

void TModelWaterManager::getPlaneFriction(const TBGCheckData*) { }

void TModelWaterManager::splashWallPosSize(const JGeometry::TVec3<f32>&, f32) {
}

void TModelWaterManager::splashWall(int) { }

void TModelWaterManager::touchingExec(int) { }

void TModelWaterManager::splashGround(int) { }

void TModelWaterManager::splashSound(const JGeometry::TVec3<f32>&, f32) const {
}

void TModelWaterManager::emitRequest(const TWaterEmitInfo&) { }

void TModelWaterManager::makeEmit(const TWaterEmitInfo&) { }

void TModelWaterManager::askHitWaterParticleOnGround(
    const JGeometry::TVec3<f32>&)
{
}

void TModelWaterManager::getWaterAlpha() const { }

void TModelWaterManager::getWPGravity(int) const { }

void TModelWaterManager::loadAfter() { }

void TModelWaterManager::load(JSUMemoryInputStream&) { }

TWaterParticleType::TWaterParticleType(const char*) { }

TWaterEmitInfo::TWaterEmitInfo(const char* name)
    : TParams(name)
    , PARAM_INIT(mNum, 1)
    , PARAM_INIT(mAlive, 0xff)
    , PARAM_INIT(mAttack, 1)
    , PARAM_INIT(mDir, JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f))
    , PARAM_INIT(mPos, JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f))
    , PARAM_INIT(mV, JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f))
    , PARAM_INIT(mDirTremble, 0.0f)
    , PARAM_INIT(mPow, 0.0f)
    , PARAM_INIT(mPowTremble, 0.0f)
    , PARAM_INIT(mSize, 0.0f)
    , PARAM_INIT(mSizeTremble, 0.0f)
    , PARAM_INIT(mHitRadius, 0.0f)
    , PARAM_INIT(mHitHeight, 0.0f)
    , PARAM_INIT(mFlag, 0)
    , PARAM_INIT(mType, 0)
    , PARAM_INIT(__padding, 0)
{
	TParams::load(mPrmPath);
}
