#include <Enemy/Seal.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/Particles.hpp>
#include <M3DUtil/MActor.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Strategic/Strategy.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Player/ModelWaterManager.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TSeal::TSeal(const char* name)
    : TSpineEnemy(name)
{
	unk150 = 0;
	mLiveFlag |= LIVE_FLAG_UNK10;
}

// TODO: every instruction and the frame (0xc0) are exact. Two residues remain.
// Update: residue (1), the r30/r31 rotation, is CLOSED by reading the manager
// through `getManager()` at both sites (99.9%, 6 markers: the group binding
// and the Mtx slot below). On that base the unnamed `search(...)->push_back`
// brings the rotation back (43), and `getChildren()`/`insert`/named list
// references all go to frame 0xc8 or 192 instructions.
//
// (1) A two-register rotation: retail keeps the .rodata string base in r31 and
// the `this` reloads in r30 where we do the reverse. Measured, all with the
// frame right and 182 instructions: `->push_back(this)` is the baseline 0xb8,
// `getChildren().push_back(this)` +16, `getSpine()->initWith` +8, spelling
// `getMaxHitPoints()` out -8; four combinations reach 0xc0 and every one keeps
// the rotation, as do a named group local, `getMActor()->offMakeDL()` and
// splitting `radius`' declaration. So the lever is not an accessor level on
// `this` (batch 63's register-priority rule does not apply here).
// Closure batch 129 adds six more inert spellings (byte-identical objects):
// split declarations for the TMapCollisionManager, the TMActorKeeper, the
// MActor and `radius`, and `search2` with the cast at the site.
//
// (2) One extra 4-byte inline temporary between the iterator-copy pair and
// `insert`'s argument group: retail's referenced slots are 0x4c, 0x50 |
// 0x5c, 0x60 | 0x6c and ours are 0x4c, 0x50 | 0x60, 0x64 | 0x70, so the first
// pair is exact and everything above it is 4 bytes high, with 12 dead bytes
// between 0x50 and the insert group where retail has 8. That is the same
// JGadget push_back temp-pool grouping that blocks SDLModel::entry and
// TMirrorActor::init (std-list.hpp research item); the scratch Mtx of the
// inlined setUpUnk8TRS rides along on it.
// Research batch 133: the extra word is not a header spelling. The isolated
// model (`TList_pointer<T*>::insert` forced out of line) had its own 16-byte
// frame excess from an implicit derived-from-base conversion on its `return`,
// which is now fixed and exact, and that change leaves this function
// byte-identical. Also rejected here: `getChildren().push_back(this)` and
// `insert(this)` at the site, and every `const&`/named-temporary/implicit
// begin-end spelling in std-list.hpp (docs/catalog/frame-gaps.md, batch 133).
//
// Closure batch 215 applies research 211's rule (the JGadget pool packs when
// the container receiver is a named local pointer and pads when it is an
// unnamed expression): naming the searched group takes the push_back pool from
// 0x4c,0x50 | 0x60,0x64 to retail's 0x4c,0x50 | 0x5c,0x60 exactly, so residue
// (2) is now one 4-byte hole *below* the setUpUnk8TRS scratch Mtx (0x70 here,
// 0x6c retail) with everything above it -- 0xa4, 0xa8 and the save area -- at
// retail's offsets. `group->getChildren().push_back`, `group->add`, a named
// `TList_pointer<THitActor*>` reference or pointer on top of the named group,
// and a TU-local forwarder around `setUpUnk8TRS` are all worse. `mSpine->`
// instead of `getSpine()->` and writing `getMaxHitPoints()` out each remove 8
// bytes, 4 of them exactly the hole under the Mtx and 4 from the top of the
// named region, so the hole is an allocation *order* difference in a pinned
// low region, not a missing 4-byte item.
//
// Closure batch 221 splits the residue into three independent pieces and
// prices two of them.
// (b) The group *binding*: retail binds the converted list base (`addi rD,
// r3, 0x10` after the search, then `+8` and `+0` at the two uses) where our
// named `group` binds `r3 + 0` and pays `+0x18`/`+0x10` per use. Dropping the
// local -- `search<TIdxGroupObj>("...")->push_back(this)` -- reproduces
// retail's binding and its two uses byte for byte (and is the same spelling
// THookTake's ctor already proves in riccohook.cpp), but research 211's pad
// comes back: the second iterator group goes 0x60/0x64 for retail's
// 0x5c/0x60, 43 markers against the named local's 41. Named group and
// retail's binding are so far mutually exclusive.
// (c) The Mtx hole is now priced exactly: `mSpine->initWith` (in place of
// `getSpine()->`) puts the scratch Mtx at retail's 0x6c with the whole
// push_back pool still exact, and the *only* thing left below the save area
// is 4 bytes of dead low region -- our iterator copies land at 0xa0/0xa4 for
// retail's 0xa4/0xa8, frame 0xb8 for 0xc0. So retail allocates that one
// 4-byte accessor temporary *above* the Mtx and we allocate it below; a
// `volatile char trash[4]` declared last restores the frame but parks its
// word above the copies (0xa4/0xa8 unmoved), so the missing item sits at
// 0xa0, between the Mtx's end and the iterator copies.
// Also rejected this batch: the group local in an inner block (byte-identical
// to the baseline, so the callee-saved ladder's inner-block rung does not
// reach it), `search(...)->getChildren().push_back(this)` (frame 0xc8, 62
// markers), a named `JGadget::TList_pointer<THitActor*>&` receiver (0xc8, 62)
// and a named `TList_pointer<THitActor*>*` (the implicit derived-to-base
// pointer conversion adds a null test, 94.9).
//
// The rotation (1) has one measured lever: `MActor* actor = createMActor(...);
// mMActor = actor; actor->offMakeDL();` puts the .rodata base in r31 and the
// `this`/group chain in r30 exactly as retail does -- 6 markers left, 99.3%,
// frame 0xc0, both remaining clusters being the group binding and the Mtx.
// It is not committable: retail reloads `mMActor` for `offMakeDL` (`stw r3,
// 0x74(r29); lwz r3, 0x74(r29)`) and the named local elides that load. Every
// spelling that keeps the reload (dead named result, `MActor* a = mMActor;`,
// `getMActor()->offMakeDL()` with or without the named result) is back at 41
// markers, and the same named-local trick on the TMActorKeeper or the
// TMapCollisionManager does not move the rotation at all. So the caller-local
// that outranks `this` in retail is a fourth value somewhere else in the body.
void TSeal::init(TLiveManager* manager)
{
	mManager = manager;
	getManager()->manageActor(this);

	mMActorKeeper = new TMActorKeeper(getManager(), 2);
	mMActor       = mMActorKeeper->createMActor("gene_orange_model1.bmd", 0);
	mMActor->offMakeDL();

	f32 radius = 100.0f * mScaling.x;
	initHitActor(0x10000024, 1, 0x81000000, radius, radius, radius, radius);
	mHitFlags &= ~HIT_FLAG_NO_COLLISION;

	TIdxGroupObj* group = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");
	group->push_back(this);

	f32 angle = 270.0f + mRotation.x;
	while (angle >= 360.0f)
		angle -= 360.0f;
	while (angle < 0.0f)
		angle += 360.0f;
	mRotation.x = angle;

	mMapCollisionManager = new TMapCollisionManager(1, "/scene/seal", this);
	mMapCollisionManager->init("gene_orange_col1.col", 2, nullptr);

	mMapCollisionManager->setUpUnk8TRS(mPosition, mRotation, mScaling);

	mHitPoints = getMaxHitPoints();

	getSpine()->initWith(&TNerveSealSleep::theNerve());
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
