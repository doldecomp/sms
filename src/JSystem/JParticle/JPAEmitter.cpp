#include <JSystem/JParticle/JPAEmitter.hpp>

void JPAGetEmitterInfoPtr() { }

JPABaseEmitter::JPABaseEmitter() { }

void JPABaseEmitter::getFovy() { }

void JPABaseEmitter::getAspect() { }

void JPABaseEmitter::newParticle() { }

void JPABaseEmitter::deleteBaseParticle(JPABaseParticle*,
                                        JSUList<JPABaseParticle>*)
{
}

void JPABaseEmitter::deleteParticle(JPABaseParticle*) { }

void JPABaseEmitter::deleteAllParticle() { }

void JPABaseEmitter::createChildParticle(JPABaseParticle*) { }

void JPABaseEmitter::getEmitterGlobalTranslation(JGeometry::TVec3<f32>&) { }

void JPABaseEmitter::calcEmitterGlobalParams() { }

void JPABaseEmitter::loadBaseEmitterBlock(JPADataBlock*) { }

void JPABaseEmitter::executeBeforeCallBack() { }

void JPABaseEmitter::executeAfterCallBack() { }

void JPABaseEmitter::drawEmitterCallBack() { }

void JPABaseEmitter::createParticle() { }

void JPABaseEmitter::calcCurrentRateTimerStep() { }

void JPABaseEmitter::calcCreateParticle() { }

void JPABaseEmitter::checkStartFrame() { }

void JPABaseEmitter::checkMaxFrame() { }

void JPABaseEmitter::doParticle() { }

void JPABaseEmitter::doChildParticle() { }

void JPABaseEmitter::getKeyValue(f32, u16, f32*) { }

void JPABaseEmitter::calcKeyFrameAnime() { }

void JPABaseEmitter::calc() { }

void JPABaseEmitter::setGlobalRMatrix(MtxPtr) { }

void JPABaseEmitter::setGlobalRTMatrix(MtxPtr) { }

void JPABaseEmitter::setGlobalSRTMatrix(MtxPtr) { }

void JPABaseEmitter::getPivotX() { }

void JPABaseEmitter::getPivotY() { }
