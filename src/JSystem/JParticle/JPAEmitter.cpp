#include <JSystem/JParticle/JPAEmitter.hpp>

static JPAEmitterInfo JPAEmitterInfoObj;

JPAEmitterInfo* JPAGetEmitterInfoPtr() { return &JPAEmitterInfoObj; }

JPABaseEmitter::JPABaseEmitter()
    : unk0(this)
    , unk10(0.0f)
    , unk14(1.0f)
    , unk18(0.0f)
    , unk1C(1.0f)
    , unk21C(0)
{
	PSMTXIdentity(unk124);

	// TODO: vectors?

	unk154 = 1.0f;
	unk158 = 1.0f;
	unk15C = 1.0f;

	unk174 = 1.0f;
	unk178 = 1.0f;
	unk17C = 1.0f;

	unk170 = 0;
	unk16E = 0;
	unk16C = 0;

	unk160.x = unk160.y = unk160.z = 0.0f;

	unk1B8 = 0.0f;
	unk1BC = 0.0f;
	unk1C0 = 1.0f;

	unk110 = nullptr;
	unk114 = nullptr;
	unk118 = nullptr;

	unk172 = 0;
	unk11C = 0;
	unk11C |= 0x30;
	unk21C.value = JPAEmitterInfoObj.unk8.next();
}

f32 JPABaseEmitter::getFovy() { return JPAEmitterInfoObj.unk15C; }

f32 JPABaseEmitter::getAspect() { return JPAEmitterInfoObj.unk160; }

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

bool JPABaseEmitter::checkStartFrame() { }

bool JPABaseEmitter::checkMaxFrame() { }

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
