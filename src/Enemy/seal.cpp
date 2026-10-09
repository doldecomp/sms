#include <Enemy/Seal.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TSeal::TSeal(const char* name)
    : TSpineEnemy(name)
    , unk150(0)
{
	onLiveFlag(LIVE_FLAG_UNK10);
}

void TSeal::init(TLiveManager* param_1)
{
	mManager = param_1;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 2);
	mMActor       = mMActorKeeper->createMActor("gene_orange_model1.bmd", 0);
	mMActor->offMakeDL();

	f32 scale = 100.0f * mScaling.x;
	initHitActor(ACTOR_TYPE_SEAL, 1, HIT_CATEGORY_PLAYER | HIT_CATEGORY_WATER,
	             scale, scale, scale, scale);
	offHitFilter(HIT_FILTER_NO_COLLISION);

	static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"))
	    ->getChildren()
	    .push_back(this);

	mRotation.x = MsWrap(mRotation.x + 270.0f, 0.0f, 360.0f);

	mMapCollisionManager = new TMapCollisionManager(1, "/scene/seal", this);
	mMapCollisionManager->init("gene_orange_col1.col", 2, nullptr);

	mMapCollisionManager->setUpActiveCollisionTRS(mPosition, mRotation,
	                                              mScaling);

	mHitPoints = getMaxHitPoints();
	mSpine->initWith(&TNerveSealSleep::theNerve());
}

BOOL TSeal::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->getActorType() == ACTOR_TYPE_WATER
	    && message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                             0, nullptr);
		SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK,
		                              &sender->mPosition, 0, 0.0f, 0, 0, 4);
		if (gpModelWaterManager->unk5D5F != 0) {
			SMSGetMSound()->startSoundSet(MSD_SE_ERASE_SCRAWL,
			                              &sender->mPosition, 0, 0.0f, 0, 0, 4);
			if (!mSpine->isNerve(&TNerveSealDie::theNerve())) {
				if (mMapCollisionManager->getActiveCollision() != nullptr)
					mMapCollisionManager->getActiveCollision()->remove();
				const TNerveBase<TLiveActor>* nextNerve
				    = &TNerveSealDie::theNerve();
				mSpine->pushNerve(nextNerve);
			}
			unk150++;
			return true;
		}
		return true;
	}
	return false;
}

void TSeal::calcRootMatrix()
{
	J3DModel* model = getModel();
	MsMtxSetXYZRPH(model->getBaseTRMtx(), mPosition.x, mPosition.y, mPosition.z,
	               mRotation.x, mRotation.y, mRotation.z);
	model->setBaseScale(mScaling);
}

void TSeal::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN) && (param_1 & 1)) {
		for (int i = 0; i < getColNum(); ++i) {
			THitActor* actor = getCollision(i);
			if (actor->isActorType(ACTOR_TYPE_MARIO))
				actor->receiveMessage(this, HIT_MESSAGE_ATTACK);
		}
	}

	TSpineEnemy::perform(param_1, param_2);
	if (param_1 & 1) {
		updateSquareToMario();
		unk150 = 0;
	}
	if ((param_1 & 2) && !checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN)
	    && getDistToMarioSquared() < 2250000.0f)
		SMSGetMSound()->startSoundActor(MSD_SE_EN_ORANGESEAL_WAIT, &mPosition,
		                                0, nullptr, 0, 4);
}

TSealManager::TSealManager(const char* name)
    : TEnemyManager(name)
{
}

void TSealManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "gene_orange_model1.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_MaterialUseIndirect
		      | J3DMLF_UseUniqueMaterials | (1 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TSealManager::initJParticle() { }

void TSealManager::load(JSUMemoryInputStream& param_1)
{
	TEnemyManager::load(param_1);
}

DEFINE_NERVE(TNerveSealSleep, TLiveActor)
{
	TSeal* seal = static_cast<TSeal*>(spine->getBody());
	if (spine->getTime() == 0 && !seal->getMActor()->checkCurBckFromIndex(1))
		seal->getMActor()->setBckFromIndex(-1);

	if (seal->getDistToMarioSquared() < 1000000.0f) {
		spine->pushAfterCurrent(&TNerveSealWait::theNerve());
		return true;
	}

	if (seal->getMActor()->curAnmEndsNext()
	    && seal->getMActor()->checkCurBckFromIndex(1))
		seal->getMActor()->setBckFromIndex(-1);

	return false;
}

DEFINE_NERVE(TNerveSealWait, TLiveActor)
{
	TSeal* seal = static_cast<TSeal*>(spine->getBody());
	if (spine->getTime() == 0)
		seal->getMActor()->setBckFromIndex(3);

	if (seal->getMActor()->curAnmEndsNext()
	    && seal->getMActor()->checkCurBckFromIndex(3))
		seal->getMActor()->setBckFromIndex(2);

	if (seal->getDistToMarioSquared() > 2250000.0f
	    && seal->getMActor()->curAnmEndsNext()) {
		seal->getMActor()->setBckFromIndex(1);
		spine->pushAfterCurrent(&TNerveSealSleep::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveSealDie, TLiveActor)
{
	TSeal* seal = static_cast<TSeal*>(spine->getBody());
	if (spine->getTime() == 0) {
		seal->getMActor()->setBckFromIndex(0);

		MtxPtr mtx = seal->getMActor()->getModel()->getBaseTRMtx();
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(
		        PARTICLE_MS_SL_OR_MELT1, mtx, 0, seal))
			emitter->setGlobalScale(seal->mScaling);
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(
		        PARTICLE_MS_SL_OR_MELT2, mtx, 0, (u8*)seal + 1))
			emitter->setGlobalScale(seal->mScaling);
	}

	SMSGetMSound()->startSoundActor(MSD_SE_WT_BOSS_FADEAWAY, &seal->mPosition,
	                                0, nullptr, 0, 4);

	if (seal->getMActor()->curAnmEndsNext()) {
		seal->onHitFilter(HIT_FILTER_NO_COLLISION);
		seal->kill();
		spine->pushAfterCurrent(&TNerveSealSleep::theNerve());
		return true;
	}

	return false;
}
