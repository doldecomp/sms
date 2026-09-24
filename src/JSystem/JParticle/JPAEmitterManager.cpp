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
	    = ALIGN_NEXT(param_2 * sizeof(JPAParticle), 0x20)
	      + ALIGN_NEXT(sizeof(JKRSolidHeap), 0x10);
	unkC = JKRCreateSolidHeap(bytesForParticles, param_5, false);
	if (unkC) {
		for (int i = 0; i < param_2; ++i) {
			JPABaseParticle* particle = new (unkC, 0) JPAParticle;
			unk0.prepend(particle->getLinkBufferPtr());
			particle->init();
		}
	}

	u32 bytesForEmitters
	    = ALIGN_NEXT(param_3 * sizeof(JPABaseEmitter), 0x20)
	      + ALIGN_NEXT(sizeof(JKRSolidHeap), 0x10);
	unk20 = JKRCreateSolidHeap(bytesForEmitters, param_5, false);
	if (unk20) {
		for (int i = 0; i < param_3; ++i) {
			JPABaseEmitter* emitter = new (unk20, 0) JPABaseEmitter;
			unk14.prepend(emitter->getLinkBufferPtr());
		}
	}

	u32 bytesForFields
	    = ALIGN_NEXT(param_4 * sizeof(JPABaseField), 0x20)
	      + ALIGN_NEXT(sizeof(JKRSolidHeap), 0x10);
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

	// One flat 32-entry loop, which MWCC unrolls by 16 into retail's
	// two-iteration body; a 2 x 16 nested spelling allocates the first four
	// address temporaries differently. The member is probably a flat
	// `JPABaseEmitter* [32]` (header TODO: its users only read [0][0]).
	for (int i = 0; i < 32; ++i)
		(&unkC8[0][0])[i] = 0;
}

u32 JPAEmitterManager::getEmitterNumber()
{
	u32 num = 0;

	for (int i = 0; i < 8; ++i)
		num += unk44[i].getNumLinks();

	return num;
}

u32 JPAEmitterManager::getParticleNumber()
{
	u32 num = 0;

	for (int i = 0; i < 8; ++i) {
		JSUList<JPABaseEmitter>* list = &unk44[i];

		for (JSULink<JPABaseEmitter>* link = list->getFirst();
		     link != list->getEnd(); link  = link->getNext())
            num += link->getObject()->getParticleNumber();
	}

	return num;
}

u32 JPAEmitterManager::getFieldNumber()
{
	u32 num = 0;

	for (int i = 0; i < 8; ++i) {
		JSUList<JPABaseEmitter>* list = &unk44[i];

		for (JSULink<JPABaseEmitter>* link = list->getFirst();
		     link != list->getEnd(); link  = link->getNext())
            num += link->getObject()->getFieldManager()->unk0.getNumLinks();
	}

	return num;
}

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

void JPAEmitterManager::calc(u8 group_id)
{
	if (group_id >= 8)
		return;

	calcBase(group_id);
}

void JPAEmitterManager::drawBase(JPADrawInfo* info, u8 group_id)
{
	JPAGetEmitterInfoPtr()->mFovy   = info->getFovy();
	JPAGetEmitterInfoPtr()->mAspect = info->getAspect();
	if (!unk44[group_id].getNumLinks())
		return;

	JSUList<JPABaseEmitter>* list = &unk44[group_id];

	for (JSULink<JPABaseEmitter>* link = list->getFirst();
	     link != list->getEnd(); link  = link->getNext()) {

		JPABaseEmitter* emitter = link->getObject();

		if (emitter->isDraw())
			emitter->draw(info->getCameraMtxPtr());
	}
}

void JPAEmitterManager::draw(JPADrawInfo* info)
{
	for (int i = 0; i < 8; ++i)
		drawBase(info, i);
}

void JPAEmitterManager::draw(MtxPtr cam_mtx)
{
	JPADrawInfo info(cam_mtx);

	for (int i = 0; i < 8; ++i)
		drawBase(&info, i);
}

void JPAEmitterManager::draw(JPADrawInfo* info, u8 group_id)
{
	if (group_id >= 8)
		return;

	drawBase(info, group_id);
}

void JPAEmitterManager::draw(MtxPtr, u8) { }

JPABaseEmitter* JPAEmitterManager::createVolumeEmitter(JPADataBlock* block,
                                                       u8 group_id)
{
	JPABaseEmitter* emitter = nullptr;

	if (unk14.getNumLinks()) {
		emitter = (JPABaseEmitter*)unk14.getFirst()->getObjectPtr();
		unk14.remove(emitter->getLinkBufferPtr());
		new (emitter) JPABaseEmitter;
		unk44[group_id].append(emitter->getLinkBufferPtr());
		emitter->loadBaseEmitterBlock(block);
	}

	return emitter;
}

// Binding level worth +16 of low region, landing
// JPAEmitterManager::createEmitterBase's frame at 0xc8 (batch 124).
static inline JPADataBlock*
JPAEmitterManagerGetBaseEmitterBlock(JPADataBlockLinkInfo* p)
{
	JPADataBlock* baseEmitterBlock = p->getBaseEmitterBlock();
	return baseEmitterBlock;
}

// Binding level (with the one above) landing createEmitterBase's frame at
// 0xc8 (batch 124).
static inline u8 JPAEmitterManagerGetFieldNum(JPADataBlockLinkInfo* p)
{
	u8 fieldNum = p->getFieldNum();
	return fieldNum;
}

// A direct-return level over the link-info lookup: it ranks `linkInfo` below
// the `&unkA4[param_3]` base temp (retail's r29/r30), where the same read
// spelled inline in the caller ranks it above. A named-result or
// reference-out-parameter form of the same level costs 8/0x10 of frame.
static inline JPADataBlockLinkInfo*
JPAEmitterManagerGetLinkInfo(JPAEmitterData* p)
{
	return p->getLinkInfo()[0];
}

JPABaseEmitter* JPAEmitterManager::createEmitterBase(
    s32 param_1, u8 param_2, u8 param_3,
    JPACallBackBase<JPABaseEmitter*>* param_4,
    JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>* param_5)
{
	if (param_2 >= 8)
		return nullptr;

	if (getResourceManager(param_3)) {
		JPAEmitterData* emitterData
		    = getResourceManager(param_3)->getEmitterResource()->getByUserIndex(
		        param_1);
		if (!emitterData)
			return nullptr;

		JPADataBlockLinkInfo* linkInfo
		    = JPAEmitterManagerGetLinkInfo(emitterData);
		JPADataBlock* block
		    = JPAEmitterManagerGetBaseEmitterBlock(linkInfo);

		JPABaseEmitter* emitter = createVolumeEmitter(block, param_2);

		if (emitter) {
			emitter->unk173   = param_2;
			emitter->mManager = this;
			emitter->setGlobalPrmColor(0xff, 0xff, 0xff);
			emitter->setGlobalEnvColor(0xff, 0xff, 0xff);
			emitter->setGlobalAlpha(0xff);

			emitter->setFieldList(&unk28);

			int count             = JPAEmitterManagerGetFieldNum(linkInfo);
			JPADataBlock** blocks = linkInfo->getField();
			for (int i = 0; i < count; ++i) {
				JPADataBlock* block = blocks[i];
				JPABaseField* field = emitter->getFieldManager()->setField(
				    block->getRawData()[0xC]);
				if (field)
					field->loadFieldBlock(block);
			}
			emitter->mEmitterDataBlockInfo = linkInfo;
			if (linkInfo->getSweepShape())
				emitter->mParticlesCreateChildren = 1;

			emitter->calcCurrentRateTimerStep();
			emitter->getDraw()->initialize(
			    emitter, getResourceManager(param_3)->getTextureResource());
			emitter->setEmitterCallBackPtr(param_4);
			emitter->setParticleCallBackPtr(param_5);
		}
		return emitter;
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
		result->setGlobalTranslation(param_1);
		return result;
	}

	return nullptr;
}

JPABaseEmitter* JPAEmitterManager::createSimpleEmitter(
    const JGeometry::TVec3<f32>& param_1, s32 param_2,
    JPACallBackBase<JPABaseEmitter*>* param_3,
    JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*>* param_4)
{
	return createSimpleEmitterID(param_1, param_2, 0, 0, param_3, param_4);
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

void JPAEmitterManager::forceDeleteAllEmitter()
{
	for (int i = 0; i < 8; ++i) {
		JSUList<JPABaseEmitter>* list = &unk44[i];

		JSULink<JPABaseEmitter>* next = nullptr;
		for (JSULink<JPABaseEmitter>* link = list->getFirst();
		     link != list->getEnd(); link  = next) {
			next = link->getNext();
			forceDeleteEmitter(link->getObject());
		}
	}
}
