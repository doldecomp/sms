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
		param_5 = JKRHeap::getCurrentHeap();

	u32 bytesForParticles
	    = ALIGN_NEXT(param_2 * sizeof(JPABaseParticle), 0x20) + 0x80;
	unkC = JKRCreateSolidHeap(bytesForParticles, param_5, false);
	if (unkC) {
		for (int i = 0; i < param_2; ++i) {
			JPABaseParticle* particle = new (unkC, 0) JPAParticle;
			unk0.prepend(particle->getLinkBufferPtr());
			particle->init();
		}
	}

	u32 bytesForEmitters
	    = ALIGN_NEXT(param_3 * sizeof(JPABaseEmitter), 0x20) + 0x80;
	unk20 = JKRCreateSolidHeap(bytesForEmitters, param_5, false);
	if (unk20) {
		for (int i = 0; i < param_3; ++i) {
			JPABaseEmitter* emitter = new (unk20, 0) JPABaseEmitter;
			unk14.prepend(emitter->getLinkBufferPtr());
		}
	}

	u32 bytesForFields
	    = ALIGN_NEXT(param_4 * sizeof(JPABaseField), 0x20) + 0x80;
	unk34 = JKRCreateSolidHeap(bytesForFields, param_5, false);
	if (unk34) {
		for (int i = 0; i < param_4; ++i) {
			JPABaseField* field = new (unk34, 0) JPABaseField;
			unk28.prepend(field->getLinkBufferPtr());
		}
	}

	unk10 = param_2;
	unk24 = param_3;
	unk38 = param_4;

	unkA4[0] = param_1;

	for (int i = 1; i < 8; ++i)
		unkA4[i] = 0;

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

u32 JPAEmitterManager::getEmitterNumber() { }

u32 JPAEmitterManager::getParticleNumber() { }

u32 JPAEmitterManager::getFieldNumber() { }

void JPAEmitterManager::calcBase(u8 group_id)
{
	if (!unk44[group_id].getNumLinks())
		return;

	JSUList<JPABaseEmitter>* list = &unk44[group_id];

	JSULink<JPABaseEmitter>* next = nullptr;
	for (JSULink<JPABaseEmitter>* link = list->getFirst();
	     link != list->getEnd(); link  = next) {
		next = link->getNext();

		JPABaseEmitter* emitter = link->getObject();

		if (!emitter->checkStartFrame())
			continue;

		if (emitter->checkMaxFrame() != 0 && emitter->getParticleNumber() == 0)
			deleteEmitter(emitter);
		else
			emitter->calc();
	}
	unk3C += unk40;
	if (unk3C < 0.0f)
		unk3C = 0.0f;
}

void JPAEmitterManager::calc()
{
	for (int gid = 0; gid < 8; ++gid)
		calcBase(gid);
}

void JPAEmitterManager::calc(u8) { }

void JPAEmitterManager::drawBase(JPADrawInfo* info, u8 group_id)
{
	JPAGetEmitterInfoPtr()->mFovy   = info->getFovy();
	JPAGetEmitterInfoPtr()->mAspect = info->getAspect();
	if (!unk44[group_id].getNumLinks())
		return;

	JSUList<JPABaseEmitter>* list = &unk44[group_id];

	for (JSULink<JPABaseEmitter>* link = list->getFirst();
	     link != list->getEnd(); link  = link->getNext()) {

		if (link->getObject()->checkStatus(JPABaseEmitter::STATUS_STOP_DRAW))
			continue;

		link->getObject()->mDraw.draw(info->getCameraMtxPtr());
	}
}

void JPAEmitterManager::draw(JPADrawInfo* info)
{
	for (int i = 0; i < 8; ++i)
		drawBase(info, i);
}

void JPAEmitterManager::draw(MtxPtr) { }

void JPAEmitterManager::draw(JPADrawInfo* info, u8 group_id)
{
	if (group_id >= 8)
		return;

	drawBase(info, group_id);
}

void JPAEmitterManager::draw(MtxPtr, u8) { }

JPABaseEmitter* JPAEmitterManager::createVolumeEmitter(JPADataBlock*, u8) { }

JPABaseEmitter* JPAEmitterManager::createEmitterBase(
    s32 param_1, u8 param_2, u8 param_3,
    JPACallBackBase<JPABaseEmitter*>* param_4,
    JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>* param_5)
{
	if (param_2 >= 8)
		return nullptr;

	if (unkA4[param_3]) {
		JPAEmitterData* emitterData
		    = unkA4[param_3]->unk4->getByUserIndex(param_1);
		if (!emitterData)
			return nullptr;

		JPABaseEmitter* emitter = nullptr;

		JPADataBlockLinkInfo* linkInfo = emitterData->unk0[0];
		JPADataBlock* block            = linkInfo->getBaseEmitterBlock();

		if (unk14.getNumLinks()) {
			emitter = unk14.getFirst()->getObject();
			unk14.remove(emitter->getLinkBufferPtr());
			new (emitter) JPABaseEmitter;
			unk44[param_2].append(emitter->getLinkBufferPtr());
			emitter->loadBaseEmitterBlock(block);
		}

		if (emitter) {
			emitter->unk173   = param_2;
			emitter->mManager = this;
			emitter->unk180.r = 0xff;
			emitter->unk180.g = 0xff;
			emitter->unk180.b = 0xff;
			emitter->unk184.r = 0xff;
			emitter->unk184.g = 0xff;
			emitter->unk184.b = 0xff;
			emitter->unk180.a = 0xff;

			emitter->mFieldManager.unkC = &unk28;

			JPAFieldManager* fieldMgr = emitter->getFieldManager();

			int count             = linkInfo->getFieldNum();
			JPADataBlock** blocks = linkInfo->getField();
			for (int i = 0; i < count; ++i) {
				JPADataBlock* block2 = blocks[i];
				JPABaseField* field
				    = fieldMgr->setField(((u8*)blocks[i]->mRawData)[0xC]);
				if (field)
					field->loadFieldBlock(block2);
			}
			emitter->mEmitterDataBlockInfo = linkInfo;
			if (linkInfo->getSweepShape())
				emitter->unk172 = 1;

			emitter->calcCurrentRateTimerStep();
			emitter->mDraw.initialize(emitter, unkA4[param_3]->unk8);
			emitter->unk110 = param_4;
			emitter->unk114 = param_5;
			return emitter;
		}
	}

	return nullptr;
}

JPABaseEmitter* JPAEmitterManager::createSimpleEmitterID(
    const JGeometry::TVec3<f32>& param_1, s32 param_2, u8 param_3, u8 param_4,
    JPACallBackBase<JPABaseEmitter*>* param_5,
    JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>* param_6)
{
	JPABaseEmitter* result
	    = createEmitterBase(param_2, param_3, param_4, param_5, param_6);

	if (result) {
		result->unk160.set(param_1);
		return result;
	}

	return nullptr;
}

JPABaseEmitter* JPAEmitterManager::createSimpleEmitter(
    const JGeometry::TVec3<f32>& param_1, s32 param_2,
    JPACallBackBase<JPABaseEmitter*>* param_3,
    JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>* param_4)
{
}

bool JPAEmitterManager::createEmitter(
    const JGeometry::TVec3<f32>& param_1, s32 param_2,
    JPACallBackBase<JPABaseEmitter*>* param_3,
    JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>* param_4)
{
	JPABaseEmitter* result
	    = createSimpleEmitterID(param_1, param_2, 0, 0, param_3, param_4);

	if (result) {
		unkC4       = 1;
		unkC8[0][0] = result;
		return true;
	}

	return false;
}

void JPAEmitterManager::deleteEmitter(JPABaseEmitter* emitter)
{
	emitter->getFieldManager()->deleteAllField();
	unk44[emitter->getGroupID()].remove(emitter->getLinkBufferPtr());
	unk14.prepend(emitter->getLinkBufferPtr());
}

void JPAEmitterManager::forceDeleteEmitter(JPABaseEmitter* emitter)
{
	emitter->deleteAllParticle();
	deleteEmitter(emitter);
}

void JPAEmitterManager::forceDeleteAllEmitter() { }
