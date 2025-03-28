#include <JSystem/JParticle/JPADraw.hpp>

void JPADraw::initialize(JPABaseEmitter*, JPATextureResource*) { }

void JPADraw::draw(MtxPtr) { }

void JPADraw::calc() { }

void JPADraw::calcParticle(JPABaseParticle*) { }

void JPADraw::calcChild(JPABaseParticle*) { }

void JPADraw::initParticle(JPABaseParticle*) { }

void JPADraw::initChild(JPABaseParticle*, JPABaseParticle*) { }

void JPADraw::swapImage(const ResTIMG*, short) { }

void JPADraw::loadTexture(unsigned char, _GXTexMapID) { }

void JPADraw::setDrawExecVisitorsBeforeCB(
    const JPADraw::JPADrawVisitorDefFlags&)
{
}

void JPADraw::setDrawExecVisitorsAfterCB(const JPADraw::JPADrawVisitorDefFlags&)
{
}

void JPADraw::setDrawCalcVisitors(const JPADraw::JPADrawVisitorDefFlags&) { }

void JPADraw::setParticleClipBoard() { }

void JPADraw::setChildClipBoard() { }

void JPADraw::drawParticle() { }

void JPADraw::drawChild() { }

void JPADraw::zDraw() { }

void JPADraw::zDrawParticle() { }

void JPADraw::zDrawChild() { }

void JPADraw::getMainTextureID(unsigned char) { }

void JPADraw::getIndTextureID() { }

void JPADraw::getIndSubTextureID() { }

void JPADraw::getSecondTextureID() { }

void JPADraw::loadYBBMtx(MtxPtr) { }
