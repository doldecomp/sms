#include <Enemy/EffectObj.hpp>
#include <Enemy/Conductor.hpp>
#include <System/Particles.hpp>
#include <System/EmitterViewObj.hpp>
#include <Player/MarioAccess.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <stdlib.h>

// rogue include
#include <M3DUtil/InfectiousStrings.hpp>

TEffectObjManager* gpEffectObjManager;

// JGadget::TList<TEffectObjBase*,
// JGadget::TAllocator<TEffectObjBase*>>::iterator::iterator(JGadget::TList<TEffectObjBase*,
// JGadget::TAllocator<TEffectObjBase*>>::TNode_*)
// JGadget::operator==(JGadget::TList<TEffectObjBase*,
// JGadget::TAllocator<TEffectObjBase*>>::iterator,
// JGadget::TList<TEffectObjBase*,
// JGadget::TAllocator<TEffectObjBase*>>::iterator) void
// JGadget::TList<TEffectObjBase*,
// JGadget::TAllocator<TEffectObjBase*>>::erase(JGadget::TList<TEffectObjBase*,
// JGadget::TAllocator<TEffectObjBase*>>::iterator) {} void
// JGadget::TList<TEffectObjBase*,
// JGadget::TAllocator<TEffectObjBase*>>::~TList() {} void
// JGadget::TList<TEffectObjBase*,
// JGadget::TAllocator<TEffectObjBase*>>::insert(JGadget::TList<TEffectObjBase*,
// JGadget::TAllocator<TEffectObjBase*>>::iterator, TEffectObjBase* const&) {}

void TSimpleEffect::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 2)
		emitEffect();
}

void TEffectPinnaFunsui::loadAfter()
{
	SMS_LoadParticle("/scene/map/map/ms_pinna_funsui.jpa", 0x1A8);
	gpConductor->registerOtherObj(this);
}

void TEffectPinnaFunsui::emitEffect()
{
	MsMtxSetXYZRPH(getUnk48(), mPosition.x, mPosition.y, mPosition.z,
	               mRotation.x, mRotation.y, mRotation.z);

	gpMarioParticleManager->emitAndBindToMtxPtr(0x1A8, getUnk48(), 1, this);

	if (gpMSound->gateCheck(0x5006))
		MSoundSESystem::MSoundSE::startSoundActor(0x5006, &mPosition, 0,
		                                          nullptr, 0, 4);
}

void TEffectBiancoFunsui::loadAfter()
{
	SMS_LoadParticle("/scene/map/map/ms_bia_funsui.jpa", 0x1A9);
	gpConductor->registerOtherObj(this);
}

void TEffectBiancoFunsui::emitEffect()
{
	MsMtxSetXYZRPH(getUnk48(), mPosition.x, mPosition.y, mPosition.z,
	               mRotation.x, mRotation.y, mRotation.z);

	gpMarioParticleManager->emitAndBindToMtxPtr(0x1A9, getUnk48(), 1, this);

	if (gpMSound->gateCheck(0x5006))
		MSoundSESystem::MSoundSE::startSoundActor(0x5006, &mPosition, 0,
		                                          nullptr, 0, 4);
}

TEffectObjBase::TEffectObjBase(const char* name)
    : THitActor(name)
    , unk68(0)
    , unk6C(0.0f)
    , unk70(0.0f)
{
}

void TEffectObjBase::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);
	init();
	if (gpEffectObjManager)
		gpEffectObjManager->addListEffectObj(this);
}

void TEffectObjBase::init()
{
	initHitActor(0x10000005, 5, -0x80000000, 30.0f, 150.0f, 30.0f, 150.0f);
	reset();
}

void TEffectObjBase::reset()
{
	unk68 = 1;
	unk6C = 200.0f;
	unk70 = 200.0f;
	offHitFlag(HIT_FLAG_NO_COLLISION);
	unk74 = 0;
}

void TEffectObjBase::perform(u32 param_1, JDrama::TGraphics*)
{
	if (param_1 & 1) {
		if (unk68 == 1) {
			unk74 = 0;
			unk68 = 2;
		}

		moveObject();
	}
}

BOOL TEffectObjBase::receiveMessage(THitActor* sender, u32 message)
{
	if (message == 0xf) {
		behaveToWater(sender);
		return true;
	}

	return false;
}

void TEffectObjBase::moveObject()
{
	if (unk68 == 2) {
		unk74 += 1;
		JGeometry::TVec3<f32> local_1c;
		VECScale(&mScaling, &local_1c, unk70 / unk6C);

		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emit(0x1F5, &mPosition, 3, this)) {
			// TODO: making this a setUnk154AndUnk174 makes it match less
			// because loads happen in z x y order instead of x y z, what was
			// the real inlines?
			f32 x = local_1c.x;
			f32 y = local_1c.y;
			f32 z = local_1c.z;
			emitter->unk154.set(x, y, z);
			emitter->unk174.set(x, y, z);
		}

		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emit(0x1C4, &mPosition, 1, this)) {
			f32 x = local_1c.x;
			f32 y = local_1c.y;
			f32 z = local_1c.z;
			emitter->unk154.set(x, y, z);
			emitter->unk174.set(x, y, z);
		}

		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emit(0x1C5, &mPosition, 1, this)) {
			f32 x = local_1c.x;
			f32 y = local_1c.y;
			f32 z = local_1c.z;
			emitter->unk154.set(x, y, z);
			emitter->unk174.set(x, y, z);
		}

		if (gpMSound->gateCheck(0x30B2))
			MSoundSESystem::MSoundSE::startSoundActor(0x30B2, &mPosition, 0,
			                                          nullptr, 0, 4);
	}

	if (unk68 == 3)
		unk68 = 0;

	for (int i = 0; i < mColCount; ++i)
		if (mCollisions[i]->isActorType(0x80000001))
			SMS_SendMessageToMario(this, 0xA);
}

void TEffectObjBase::behaveToWater(THitActor* param_1)
{
	if (unk68 != 2)
		return;

	if (unk70 > 0.0f) {
		unk70 -= 1.0f;
		return;
	}

	if (gpMSound->gateCheck(0x28C5))
		MSoundSESystem::MSoundSE::startSoundActor(0x28C5, &mPosition, 0,
		                                          nullptr, 0, 4);
	unk68 = 3;
	onHitFlag(HIT_FLAG_NO_COLLISION);

	if (JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
	        0x8B, &mPosition, 0, nullptr)) {
		emitter->unk154.set(mScaling);
		emitter->unk174.set(mScaling);
	}
}

void TEffectObjBase::forceKill() { }

TEffectObjManager::TEffectObjManager(const char* name)
    : JDrama::TViewObj(name)
    , unk10(0)
{
	gpConductor->registerOtherObj(this);
	gpEffectObjManager = this;

	unk24 = new TEffectObjBase*[COOL_EFFECT_OBJ_NUM];
	for (int i = 0; i < COOL_EFFECT_OBJ_NUM; ++i)
		unk24[i] = new TEffectObjBase;
}

void TEffectObjManager::generateEffect(Vec) { }

void TEffectObjManager::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
}

void TEffectObjManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 0x9) {
		JGadget::TList<TEffectObjBase*>::iterator it = unk14.begin();
		for (; it != unk14.end(); ++it)
			(*it)->perform(param_1, param_2);

		for (int i = 0; i < COOL_EFFECT_OBJ_NUM; ++i)
			unk24[i]->perform(param_1, param_2);
	}
}

void TEffectObjManager::addListEffectObj(TEffectObjBase* param_1)
{
	unk14.push_back(param_1);
	++unk10;
}

void TEffectModel::init(TLiveManager* param_1)
{
	TSpineEnemy::init(param_1);

	onLiveFlag(LIVE_FLAG_DEAD);
	mScaling.set(0.0f, 0.0f, 0.0f);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mActorType = 0x10000020;
	mRotation.zero();
}

void TEffectModel::reset()
{
	TSpineEnemy::reset();
	mRotation.y = MsRandF(0.0f, 360.0f);
	onLiveFlag(LIVE_FLAG_UNK8);
	onLiveFlag(LIVE_FLAG_UNK10);
	offLiveFlag(LIVE_FLAG_DEAD);
	offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TEffectModel::moveObject()
{
	if (!checkLiveFlag(LIVE_FLAG_DEAD) && mMActor->curAnmEndsNext()) {
		onLiveFlag(LIVE_FLAG_DEAD);
		onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
	}
}

void TEffectModel::calcRootMatrix()
{
	TPosition3f mtx;
	MsMtxSetXYZRPH(mtx, mPosition.x, mPosition.y, mPosition.z, mRotation.x,
	               mRotation.y, mRotation.z);
	mtx.translation(mPosition.x, mPosition.y, mPosition.z);
	mMActor->getModel()->setBaseTRMtx(mtx);
	mMActor->getModel()->setBaseScale(mScaling);
}

TEffectColumWaterManager::TEffectColumWaterManager(const char* name)
    : TEffectModelManager(name)
{
}

void TEffectColumWaterManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TSpineEnemyParams("/enemy/watercolum.prm");
	TEnemyManager::load(stream);
}

void TEffectColumWaterManager::loadAfter()
{
	SMS_LoadParticle("/scene/columWater/jpa/ms_enehamon_a.jpa", 0x89);
	SMS_LoadParticle("/scene/columWater/jpa/ms_enehamon_b.jpa", 0x8A);
}

TEffectModel* TEffectColumWaterManager::createEnemyInstance()
{
	return new TEffectColumWater;
}

void TEffectColumWaterManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "06_enem_tobikomi.bmd", 0x10220000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TEffectColumWater::TEffectColumWater(const char* name)
    : TEffectModel(name)
{
}

void TEffectColumWater::init(TLiveManager* param_1)
{
	TEffectModel::init(param_1);
	onLiveFlag(LIVE_FLAG_DEAD);
	mScaling.set(0.0f, 0.0f, 0.0f);
	mActorType = 0x10000020;
	mMActor->setBck("06_enem_tobikomi");
	mMActor->setBrk("06_enem_tobikomi");
	mMActor->setBtk("06_enem_tobikomi");
	mMActor->setBpk("06_enem_tobikomi");
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TEffectColumWater::reset()
{
	TEffectModel::reset();
	mMActor->setBck("06_enem_tobikomi");
	mMActor->setBrk("06_enem_tobikomi");
	mMActor->setBtk("06_enem_tobikomi");
	mMActor->setBpk("06_enem_tobikomi");
	mMActor->getFrameCtrl(0)->setFrame(0.0f);
	mMActor->getFrameCtrl(5)->setFrame(0.0f);
	mMActor->getFrameCtrl(4)->setFrame(0.0f);
	mMActor->getFrameCtrl(2)->setFrame(0.0f);
}

void TEffectColumWater::generate(JGeometry::TVec3<f32>& param_1,
                                 JGeometry::TVec3<f32>& param_2)
{
	reset();
	JGeometry::TVec3<f32> local_28 = param_2;
	local_28.scale(1.3f);
	mScaling = local_28;

	if (JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(0x89, &param_1, 0, nullptr)) {
		emitter->unk154.set(mScaling);
		emitter->unk174.set(mScaling);
	}

	if (JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(0x8A, &param_1, 0, nullptr)) {
		emitter->unk154.set(mScaling);
		emitter->unk174.set(mScaling);
	}

	onHitFlag(HIT_FLAG_NO_COLLISION);
}

TEffectBombColumWaterManager::TEffectBombColumWaterManager(const char* name)
    : TEffectModelManager(name)
{
}

void TEffectBombColumWaterManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TSpineEnemyParams("/enemy/bombwatercolum.prm");
	TEnemyManager::load(stream);
}

void TEffectBombColumWaterManager::loadAfter()
{
	SMS_LoadParticle("/scene/bombColumWater/jpa/ms_m_tobikomi_a.jpa", 0x2F);
	SMS_LoadParticle("/scene/bombColumWater/jpa/ms_m_tobikomi_b.jpa", 0x30);
	SMS_LoadParticle("/scene/bombColumWater/jpa/ms_m_tobikomi_c.jpa", 0x1D4);
}

TEffectModel* TEffectBombColumWaterManager::createEnemyInstance()
{
	return new TEffectBombColumWater;
}

void TEffectBombColumWaterManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "04_tobikomi.bmd", 0x10220000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TEffectBombColumWater::TEffectBombColumWater(const char* name)
    : TEffectModel(name)
{
}
void TEffectBombColumWater::init(TLiveManager* param_1)
{
	TEffectModel::init(param_1);
	onLiveFlag(LIVE_FLAG_DEAD);
	mScaling.set(0.0f, 0.0f, 0.0f);
	mActorType = 0x10000020;
	mMActor->setBck("04_tobikomi");
	mMActor->setBrk("04_tobikomi");
	mMActor->setBtk("04_tobikomi");
	mMActor->setBpk("04_tobikomi");
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TEffectBombColumWater::reset()
{
	TEffectModel::reset();
	mMActor->setBck("04_tobikomi");
	mMActor->setBrk("04_tobikomi");
	mMActor->setBtk("04_tobikomi");
	mMActor->setBpk("04_tobikomi");
	mMActor->getFrameCtrl(0)->setFrame(0.0f);
	mMActor->getFrameCtrl(5)->setFrame(0.0f);
	mMActor->getFrameCtrl(4)->setFrame(0.0f);
	mMActor->getFrameCtrl(2)->setFrame(0.0f);
}

void TEffectBombColumWater::generate(JGeometry::TVec3<f32>& param_1,
                                     JGeometry::TVec3<f32>& param_2)
{
	reset();
	JGeometry::TVec3<f32> local_28 = param_2;
	local_28.scale(1.3f);
	mScaling = local_28;

	if (JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(0x2F, &param_1, 0, nullptr)) {
		emitter->unk154.set(mScaling);
		emitter->unk174.set(mScaling);
	}

	if (JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(0x30, &param_1, 0, nullptr)) {
		emitter->unk154.set(mScaling);
		emitter->unk174.set(mScaling);
	}

	if (JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(0x1D4, &param_1, 0, nullptr)) {
		emitter->unk154.set(mScaling);
		emitter->unk174.set(mScaling);
	}

	onHitFlag(HIT_FLAG_NO_COLLISION);
}

TEffectColumSandManager::TEffectColumSandManager(const char* name)
    : TEffectModelManager(name)
{
}

void TEffectColumSandManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TSpineEnemyParams("/enemy/sandcolum.prm");
	TEnemyManager::load(stream);
}

void TEffectColumSandManager::loadAfter()
{
	SMS_LoadParticle("/scene/columSand/jpa/ms_boha_crash_a.jpa", 0x7E);
}

TEffectModel* TEffectColumSandManager::createEnemyInstance()
{
	return new TEffectColumSand;
}

void TEffectColumSandManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "08_sunabashira.bmd", 0x10240000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TEffectColumSand::TEffectColumSand(const char* name)
    : TEffectModel(name)
{
}

void TEffectColumSand::init(TLiveManager* param_1)
{
	TEffectModel::init(param_1);
	onLiveFlag(LIVE_FLAG_DEAD);
	mScaling.set(0.0f, 0.0f, 0.0f);
	mActorType = 0x10000020;
	mMActor->setBck("08_sunabashira");
	mMActor->setBrk("08_sunabashira");
	mMActor->setBtk("08_sunabashira");
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TEffectColumSand::reset()
{
	TEffectModel::reset();
	mMActor->setBck("08_sunabashira");
	mMActor->setBrk("08_sunabashira");
	mMActor->setBtk("08_sunabashira");
	mMActor->getFrameCtrl(0)->setFrame(0.0f);
	mMActor->getFrameCtrl(5)->setFrame(0.0f);
	mMActor->getFrameCtrl(4)->setFrame(0.0f);
}

void TEffectColumSand::generate(JGeometry::TVec3<f32>& param_1,
                                JGeometry::TVec3<f32>& param_2)
{
	reset();
	mScaling  = param_2;
	f32 fVar1 = param_2.y;

	if (JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(0x7E, &param_1, 0, nullptr)) {
		emitter->unk154.set(fVar1, fVar1, fVar1);
		emitter->unk174.set(fVar1, fVar1, fVar1);
	}

	onHitFlag(HIT_FLAG_NO_COLLISION);
}

TEffectExplosionManager::TEffectExplosionManager(const char* name)
    : TEffectModelManager(name)
{
}

void TEffectExplosionManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TSpineEnemyParams("/enemy/explosion.prm");
	TEnemyManager::load(stream);
}

void TEffectExplosionManager::loadAfter()
{
	SMS_LoadParticle("/scene/explosion/jpa/ms_bomb_smoke.jpa", 0xC4);
	SMS_LoadParticle("/scene/explosion/jpa/ms_bomb_bomb.jpa", 0xC5);
	SMS_LoadParticle("/scene/explosion/jpa/ms_bomb_hahen.jpa", 0xC6);
}

TEffectModel* TEffectExplosionManager::createEnemyInstance()
{
	return new TEffectExplosion;
}

void TEffectExplosionManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "10_bomb.bmd", 0x10240000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TEffectExplosion::TEffectExplosion(const char* name)
    : TEffectModel(name)
{
}

void TEffectExplosion::init(TLiveManager* param_1)
{
	TEffectModel::init(param_1);
	onLiveFlag(LIVE_FLAG_DEAD);
	mScaling.set(0.0f, 0.0f, 0.0f);
	mActorType = 0x10000020;
	mMActor->setBck("10_bomb");
	mMActor->setBrk("10_bomb");
	mMActor->setBtk("10_bomb");
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TEffectExplosion::reset()
{
	TEffectModel::reset();
	mMActor->setBck("10_bomb");
	mMActor->setBrk("10_bomb");
	mMActor->setBtk("10_bomb");
	mMActor->getFrameCtrl(0)->setFrame(0.0f);
	mMActor->getFrameCtrl(5)->setFrame(0.0f);
	mMActor->getFrameCtrl(4)->setFrame(0.0f);
}

void TEffectExplosion::generate(JGeometry::TVec3<f32>& param_1,
                                JGeometry::TVec3<f32>& param_2)
{
	reset();
	mScaling  = param_2;
	f32 fVar1 = param_2.y;

	if (JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(0xC4, &param_1, 0, nullptr)) {
		emitter->unk154.set(fVar1, fVar1, fVar1);
		emitter->unk174.set(fVar1, fVar1, fVar1);
	}

	if (JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(0xC5, &param_1, 0, nullptr)) {
		emitter->unk154.set(fVar1, fVar1, fVar1);
		emitter->unk174.set(fVar1, fVar1, fVar1);
	}

	if (JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(0xC6, &param_1, 0, nullptr)) {
		emitter->unk154.set(fVar1, fVar1, fVar1);
		emitter->unk174.set(fVar1, fVar1, fVar1);
	}

	if (JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(0xA2, &param_1, 0, nullptr)) {
		emitter->unk154.set(fVar1, fVar1, fVar1);
		emitter->unk174.set(fVar1, fVar1, fVar1);
	}

	onHitFlag(HIT_FLAG_NO_COLLISION);
}
