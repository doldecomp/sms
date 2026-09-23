#include <Enemy/Seal.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/Particles.hpp>
#include <M3DUtil/MActor.hpp>
#include <Map/Map.hpp>
#include <Strategic/Strategy.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Player/ModelWaterManager.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TSeal::TSeal(const char* name)
    : TSpineEnemy(name)
{
	unk150 = 0;
	mLiveFlag |= LIVE_FLAG_UNK10;
}

// The rotation wrap is MsWrap's inlined body (its named locals give retail's
// Mtx slot at 0x6c), and the raw mSpine drops the low-region reference slot.
void TSeal::init(TLiveManager* manager)
{
	mManager = manager;
	getManager()->manageActor(this);

	mMActorKeeper = new TMActorKeeper(getManager(), 2);
	mMActor       = mMActorKeeper->createMActor("gene_orange_model1.bmd", 0);
	getMActor()->offMakeDL();

	f32 radius = 100.0f * mScaling.x;
	initHitActor(0x10000024, 1, 0x81000000, radius, radius, radius, radius);
	mHitFlags &= ~HIT_FLAG_NO_COLLISION;

	TIdxGroupObj* group = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");
	JGadget::TList_pointer<THitActor*>& list = *group;
	list.push_back(this);

	mRotation.x = MsWrap(270.0f + mRotation.x, 0.0f, 360.0f);

	mMapCollisionManager = new TMapCollisionManager(1, "/scene/seal", this);
	mMapCollisionManager->init("gene_orange_col1.col", 2, nullptr);

	mMapCollisionManager->setUpUnk8TRS(mPosition, mRotation, mScaling);

	mHitPoints = getMaxHitPoints();

	mSpine->initWith(&TNerveSealSleep::theNerve());
}

BOOL TSeal::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->mActorType == 0x01000001 && message == 0xF) {
		gpMarioParticleManager->emit(0xE7, &sender->mPosition, 0, nullptr);
		gpMSound->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK, &sender->mPosition,
		                        0, 0.0f, 0, 0, 4);

		if (gpModelWaterManager->unk5D5F) {
			gpMSound->startSoundSet(MSD_SE_ERASE_SCRAWL, &sender->mPosition,
			                        0, 0.0f, 0, 0, 4);

			const TNerveBase<TLiveActor>* dieNerve
			    = &TNerveSealDie::theNerve();
			if (getSpine()->getLatestNerve() != dieNerve) {
				if (mMapCollisionManager->getUnk8())
					mMapCollisionManager->getUnk8()->remove();
				getSpine()->pushNerve(&TNerveSealDie::theNerve());
			}
			unk150++;
			return TRUE;
		}

		return TRUE;
	}

	return FALSE;
}

void TSeal::calcRootMatrix()
{
	J3DModel* model = getModel();
	MsMtxSetXYZRPH(model->getBaseTRMtx(), mPosition.x, mPosition.y, mPosition.z,
	               mRotation.x, mRotation.y, mRotation.z);
	model->setBaseScale(mScaling);
}

void TSeal::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!(mLiveFlag & (LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN)) && (cue & 1)) {
		for (int i = 0; i < getColNum(); ++i) {
			THitActor* other = getCollision(i);
			if (other->isActorType(0x80000001))
				other->receiveMessage(this, 0xE);
		}
	}

	TSpineEnemy::perform(cue, graphics);

	if (cue & 1) {
		updateSquareToMario();
		unk150 = 0;
	}

	if ((cue & 2) && !(mLiveFlag & (LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN))
	    && getDistToMarioSquared() < 2250000.0f
	    && gpMSound->gateCheck(MSD_SE_EN_ORANGESEAL_WAIT)) {
		MSoundSESystem::MSoundSE::startSoundActor(
		    MSD_SE_EN_ORANGESEAL_WAIT, &mPosition, 0, nullptr, 0, 4);
	}
}

TSealManager::TSealManager(const char* name)
    : TEnemyManager(name)
{
}

void TSealManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "gene_orange_model1.bmd", 0x11210000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TSealManager::initJParticle() { }

void TSealManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	initJParticle();
}

DEFINE_NERVE(TNerveSealSleep, TLiveActor)
{
	TSeal* seal = (TSeal*)spine->getBody();

	if (spine->getTime() == 0
	    && !seal->getMActor()->checkCurBckFromIndex(1)) {
		seal->getMActor()->setBckFromIndex(-1);
	}

	if (seal->getDistToMarioSquared() < 1000000.0f) {
		spine->pushAfterCurrent(&TNerveSealWait::theNerve());
		return TRUE;
	}

	if (seal->getMActor()->curAnmEndsNext(0, nullptr)
	    && seal->getMActor()->checkCurBckFromIndex(1)) {
		seal->getMActor()->setBckFromIndex(-1);
	}
	return FALSE;
}

// The named `time` and `distToMario` are a lever pair: each one alone is +0,
// together they are the last 8 bytes of frame (0x30 -> retail's 0x38) with no
// instruction change. A cast temporary for `spine->getBody()` and dropping the
// inner braces are +0 either way; a named `MActor*` or a named `bool` for the
// first `curAnmEndsNext` break the body.
DEFINE_NERVE(TNerveSealWait, TLiveActor)
{
	TSeal* seal = (TSeal*)spine->getBody();

	int time = spine->getTime();
	if (time == 0)
		seal->getMActor()->setBckFromIndex(3);

	if (seal->getMActor()->curAnmEndsNext(0, nullptr)
	    && seal->getMActor()->checkCurBckFromIndex(3)) {
		seal->getMActor()->setBckFromIndex(2);
	}

	f32 distToMario = seal->getDistToMarioSquared();
	if (distToMario > 2250000.0f
	    && seal->getMActor()->curAnmEndsNext(0, nullptr)) {
		seal->getMActor()->setBckFromIndex(1);
		spine->pushAfterCurrent(&TNerveSealSleep::theNerve());
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveSealDie, TLiveActor)
{
	TSeal* seal = (TSeal*)spine->getBody();

	if (spine->getTime() == 0) {
		seal->getMActor()->setBckFromIndex(0);
		MtxPtr mtx = seal->getMActor()->getModel()->getBaseTRMtx();
		JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(0xD1, mtx, 0, seal);
		if (emitter)
			emitter->setGlobalScale(seal->getScaling());
		// A distinct owner key so the second emitter is not treated as a
		// rebind of the first; retail passes seal + 1 byte.
		emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    0xD2, mtx, 0, (u8*)seal + 1);
		if (emitter)
			emitter->setGlobalScale(seal->getScaling());
	}

	if (gpMSound->gateCheck(MSD_SE_WT_BOSS_FADEAWAY)) {
		MSoundSESystem::MSoundSE::startSoundActor(
		    MSD_SE_WT_BOSS_FADEAWAY, &seal->mPosition, 0, nullptr, 0, 4);
	}

	if (seal->getMActor()->curAnmEndsNext(0, nullptr)) {
		seal->mHitFlags |= HIT_FLAG_NO_COLLISION;
		seal->kill();
		spine->pushAfterCurrent(&TNerveSealSleep::theNerve());
		return TRUE;
	}
	return FALSE;
}
