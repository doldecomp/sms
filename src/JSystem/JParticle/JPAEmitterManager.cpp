#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <JSystem/JParticle/JPAParticle.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAField.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <macros.h>

JPAEmitterManager::JPAEmitterManager(JPAResourceManager* param_1, s32 param_2,
                                     s32 param_3, s32 param_4, JKRHeap* param_5)
    : unk3C(0.0f)
    , unk40(1.0f)
{
	if (!param_5)
		param_5 = JKRHeap::sCurrentHeap;

	unkC = JKRSolidHeap::create(
	    ALIGN_NEXT(param_2 * sizeof(JPABaseParticle), 0x20) + 0x80, param_5,
	    false);
	if (unkC) {
		for (int i = 0; i < param_2; ++i) {
			JPABaseParticle* particle = new (unkC, 0) JPAParticle;
			unk0.prepend(&particle->unk0);
			particle->init();
		}
	}
	unk20 = JKRSolidHeap::create(
	    ALIGN_NEXT(param_3 * sizeof(JPABaseEmitter), 0x20) + 0x80, param_5,
	    false);
	if (unk20) {
		for (int i = 0; i < param_3; ++i) {
			JPABaseEmitter* emitter = new (unk20, 0) JPABaseEmitter;
			unk14.prepend(&emitter->unk0);
		}
	}

	unk34 = JKRSolidHeap::create(
	    ALIGN_NEXT(param_4 * sizeof(JPABaseField), 0x20) + 0x80, param_5,
	    false);
	if (unk34) {
		for (int i = 0; i < param_4; ++i) {
			JPABaseField* field = new (unk34, 0) JPABaseField;
			unk28.prepend(&field->unk0);
		}
	}

	unk10 = param_2;
	unk24 = param_3;
	unk38 = param_4;

	unkA4[0] = param_1;

	unkA8 = 0;
	unkAC = 0;
	unkB0 = 0;
	unkB4 = 0;
	unkB8 = 0;
	unkBC = 0;
	unkC0 = 0;
	unkC4 = 0;

	for (int i = 0; i < 2; ++i) {
		unkC8[i][0]  = 0;
		unkC8[i][1]  = 0;
		unkC8[i][2]  = 0;
		unkC8[i][3]  = 0;
		unkC8[i][4]  = 0;
		unkC8[i][5]  = 0;
		unkC8[i][6]  = 0;
		unkC8[i][7]  = 0;
		unkC8[i][8]  = 0;
		unkC8[i][9]  = 0;
		unkC8[i][10] = 0;
		unkC8[i][11] = 0;
		unkC8[i][12] = 0;
		unkC8[i][13] = 0;
		unkC8[i][14] = 0;
		unkC8[i][15] = 0;
	}
}

void JPAEmitterManager::getEmitterNumber() { }

void JPAEmitterManager::getParticleNumber() { }

void JPAEmitterManager::getFieldNumber() { }

void JPAEmitterManager::calcBase(u8 param_1)
{
	if (!unk44[param_1].getNumLinks())
		return;

	JSUListIterator<JPABaseEmitter> it = unk44[param_1].getFirst();
	while (it != unk44[param_1].getEnd()) {
		JSUListIterator<JPABaseEmitter> next = it.mLink->getNext();
		JPABaseEmitter* emitter              = it.getObject();
		if (!emitter->checkStartFrame())
			continue;
		if (emitter->checkMaxFrame() != 0
		    && emitter->unkF4.getNumLinks() + emitter->unk100.getNumLinks()
		           == 0)
			deleteEmitter(emitter);
		else
			emitter->calc();
		it = next;
	}
	unk3C += unk40;
	if (unk3C < 0.0f)
		unk3C = 0.0f;
}

void JPAEmitterManager::calc()
{
	for (int i = 0; i < 8; ++i)
		calcBase(i);
}

void JPAEmitterManager::calc(u8) { }

void JPAEmitterManager::drawBase(JPADrawInfo* info, u8 i)
{
	JPAGetEmitterInfoPtr()->unk15C = info->unk4;
	JPAGetEmitterInfoPtr()->unk160 = info->unk8;
	if (!unk44[i].getNumLinks())
		return;

	JSUListIterator<JPABaseEmitter> it = unk44[i].getFirst();
	for (; it != unk44[i].getEnd(); ++it) {
		if (!((it->unk11C & 4) ? true : false))
			it->unk30.draw(info->unk0);
	}
}

void JPAEmitterManager::draw(JPADrawInfo* info)
{
	for (int i = 0; i < 8; ++i)
		drawBase(info, i);
}

void JPAEmitterManager::draw(MtxPtr) { }

void JPAEmitterManager::draw(JPADrawInfo* info, u8 i)
{
	if (i >= 8)
		return;

	drawBase(info, i);
}

void JPAEmitterManager::draw(MtxPtr, u8) { }

void JPAEmitterManager::createVolumeEmitter(JPADataBlock*, u8) { }

JPABaseEmitter* JPAEmitterManager::createEmitterBase(
    s32 param_1, u8 param_2, u8 param_3,
    JPACallBackBase<JPABaseEmitter*>* param_4,
    JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>* param_5)
{
	if (param_2 >= 8)
		return nullptr;

	if (!unkA4[param_3])
		return nullptr;

	void* thing = unkA4[param_3]->unk4->getByUserIndex(param_1);
	if (!thing)
		return nullptr;

	void* thing2        = **(void***)thing;
	JPADataBlock* block = *(JPADataBlock**)thing2;

	JPABaseEmitter* emitter = nullptr;
	if (unk14.getNumLinks()) {
		emitter = unk14.getFirst()->getObject();
		unk14.remove(&emitter->unk0);
		new (emitter) JPABaseEmitter;
		unk44[param_2].append(&emitter->unk0);
		emitter->loadBaseEmitterBlock(block);
	}

	if (emitter) {
		emitter->unk173     = param_2;
		emitter->unk10C     = this;
		emitter->unk180     = 0xff;
		emitter->unk181     = 0xff;
		emitter->unk182     = 0xff;
		emitter->unk184     = 0xff;
		emitter->unk185     = 0xff;
		emitter->unk186     = 0xff;
		emitter->unk183     = 0xff;
		emitter->unk20.unkC = &unk28;

		u32 count = *((u8*)thing2 + 8);
		for (int i = 0; i < count; ++i) {
			// TODO:
		}
		emitter->unk118 = 0.0f;
		if (0) // TODO:
			emitter->unk172 = 1;

		emitter->calcCurrentRateTimerStep();
		emitter->unk30.initialize(emitter, unkA4[param_3]->unk8);
	}

	return emitter;
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
