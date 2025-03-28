#include <JSystem/JParticle/JPAEmitterManager.hpp>

JPAEmitterManager::JPAEmitterManager(JPAResourceManager*, s32, s32, s32,
                                     JKRHeap*)
{
}

void JPAEmitterManager::getEmitterNumber() { }

void JPAEmitterManager::getParticleNumber() { }

void JPAEmitterManager::getFieldNumber() { }

void JPAEmitterManager::calcBase(u8) { }

void JPAEmitterManager::calc() { }

void JPAEmitterManager::calc(u8) { }

void JPAEmitterManager::drawBase(JPADrawInfo*, u8) { }

void JPAEmitterManager::draw(JPADrawInfo*) { }

void JPAEmitterManager::draw(MtxPtr) { }

void JPAEmitterManager::draw(JPADrawInfo*, u8) { }

void JPAEmitterManager::draw(MtxPtr, u8) { }

void JPAEmitterManager::createVolumeEmitter(JPADataBlock*, u8) { }

void JPAEmitterManager::createEmitterBase(
    s32, u8, u8, JPACallBackBase<JPABaseEmitter*>*,
    JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>*)
{
}

void JPAEmitterManager::createSimpleEmitterID(
    const JGeometry::TVec3<f32>&, s32, u8, u8,
    JPACallBackBase<JPABaseEmitter*>*,
    JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>*)
{
}

void JPAEmitterManager::createSimpleEmitter(
    const JGeometry::TVec3<f32>&, s32, JPACallBackBase<JPABaseEmitter*>*,
    JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>*)
{
}

void JPAEmitterManager::createEmitter(
    const JGeometry::TVec3<f32>&, s32, JPACallBackBase<JPABaseEmitter*>*,
    JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>*)
{
}

void JPAEmitterManager::deleteEmitter(JPABaseEmitter*) { }

void JPAEmitterManager::forceDeleteEmitter(JPABaseEmitter*) { }

void JPAEmitterManager::forceDeleteAllEmitter() { }
