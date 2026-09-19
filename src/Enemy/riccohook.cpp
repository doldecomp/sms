#include <Enemy/RiccoHook.hpp>
#include <Strategic/Spine.hpp>
#include <Enemy/Graph.hpp>
#include <JSystem/JMath.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// @non-matching -- the issue seems to stem from the JDrama TNameRefGen
// search/push_back calls.
THookTake::THookTake(TRiccoHook* owner, const char* name)
    : TTakeActor(name)
    , mOwner(owner)
{
	initHitActor(0x400000BB, 1, -0x80000000,
	             mOwner->getSaveLoadParam()->mSLHitRadius.get(),
	             mOwner->getSaveLoadParam()->mSLHitHeight.get(),
	             mOwner->getSaveLoadParam()->mSLHitRadius.get(),
	             mOwner->getSaveLoadParam()->mSLHitHeight.get());

	JDrama::TNameRefGen::search<TIdxGroupObj>("オブジェクトグループ")
	    ->getChildren()
	    .push_back(this);
}

MtxPtr THookTake::getTakingMtx() { return nullptr; }

f32 THookTake::getRadiusAtY(f32 y) const
{
	return mOwner->getSaveLoadParam()->mSLHangRadius.get();
}

BOOL THookTake::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->mActorType == 0x80000001) {
		if (message == HIT_MESSAGE_UNK5) {
			mHeldObject = (TTakeActor*)sender;
			return TRUE;
		}

		if (message == HIT_MESSAGE_THROWN || message == HIT_MESSAGE_UNK8) {
			mHeldObject = nullptr;
			return TRUE;
		}
	}

	return FALSE;
}

void THookTake::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if ((cue & CUE_MOVE) != 0) {
		mPosition = mOwner->getPosition();
		mPosition.y -= 900.0f;
	}

	THitActor::perform(cue, graphics);

	if ((cue & CUE_MOVE) != 0 && mHeldObject != nullptr) {
		moveHeldObject();
	}
}

THookParams::THookParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mSLHitHeight, 900.0f)
    , PARAM_INIT(mSLHitRadius, 120.0f)
    , PARAM_INIT(mSLHangRadius, 30.0f)
    , PARAM_INIT(mSLMoveSpeed, 4.0f)
{
	TParams::load(mPrmPath);
}

TRiccoHook::TRiccoHook(const char* name)
    : TSpineEnemy(name)
    , mHookTake(nullptr)
    , mTimer(0)
{
}

// TODO: 99.9%, frame 0xd0 vs 0xd8, all 151 instructions and registers match.
// Closure batch 136 located the 8 bytes exactly: the JGadget list-insert
// temps the `new THookTake(this)` expansion leaves fall in two groups, one
// anchored to the bottom of the local area (0x5c/0x60/0x68/0x6c/0x70, already
// exact) and one anchored to the top (0x84/0x88/0x8c, 8 low), with a 20-byte
// hole between them where retail has 12; the `&hookTake` slot above wants +4.
// So the 8 bytes belong *inside* that expansion, between two of its
// sub-expansions -- not at either end. Every TU-local binding lever adds them
// at the bottom instead and shifts the first group by the same 8 (`mSpine`,
// `unk124`, `getSaveLoadParam()` all reach frame 0xd8 with the first group
// then 8 high; `getTracer()` and a named `f32 speed` are +0). This is the
// JGadget iterator temp-pool grouping known-open class (docs/catalog/
// frame-gaps.md, "batch 133"), shared with TSeal::init and TMirrorActor::init.
// Closure batch 221 decomposes it into two independent residues and prices
// the first: R1 is 8 bytes missing *between* the two groups (group one,
// 0x5c/0x60/0x68/0x6c/0x70, is exact; group two is 0x84/0x88/0x8c retail and
// 0x7c/0x80/0x84 here), R2 is the `&hookTake` spill at 0xa4 for retail's
// 0xa8. A TU-local binder over `mSpine` feeding `initWith` adds its 8 bytes
// at the very bottom of the pool: frame 0xd8 exact and group two exactly
// retail's, but group one then reads 0x64..0x78 and the spill 0xac -- 13
// markers, all of them slot displacements. So the lever needed is 8 bytes
// that land between the two groups, plus 4 above them.
// Spellings measured on THookTake's inlined ctor (it has no out-of-line copy,
// so its body is free): a named `TIdxGroupObj* group` with
// `getChildren().push_back` packs group one another 4 (19 markers); a named
// `JGadget::TList_pointer<THitActor*>&` receiver does the same (19);
// `->add(this)` collapses to frame 0xc0 (22) and `->insert(this)` to 0xc8
// (23, +2 instructions); a named `THitActor*` for the pushed object costs a
// spill; a TU-local binder returning `&search(...)->getChildren()` hoists the
// string base out of the prologue (88.6). Every honest lever tried so far
// pads at the bottom or packs, never between.
void TRiccoHook::init(TLiveManager* manager)
{
	TSpineEnemy::init(manager);
	mSpine->initWith(&TNerveRHGraphWander::theNerve());
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mHookTake = new THookTake(this);
	unk124->reset();
	goToShortestNextGraphNode();
	mMarchSpeed = getSaveLoadParam()->mSLMoveSpeed.get();
	mTurnSpeed  = 10.0f;
	onLiveFlag(LIVE_FLAG_UNK10);
}

void TRiccoHook::kill() { }

BOOL TRiccoHook::receiveMessage(THitActor* sender, u32 message)
{
	return FALSE;
}

// Binding level over a raw member read, worth +8 of low region in
// TRiccoHook::perform (batch 127).
static inline TTakeActor* RiccohookHookTake(const TRiccoHook* p)
{
	TTakeActor* hookTake = p->mHookTake;
	return hookTake;
}

void TRiccoHook::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSpineEnemy::perform(cue, graphics);
	RiccohookHookTake(this)->perform(cue, graphics);
	if (cue & CUE_MOVE) {
		if (mTimer > 0) {
			mTimer--;
		} else if (mInstanceIndex & 1) {
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_CRANE_SIDEMOVE1,
			                                &mPosition, 0, nullptr, 0, 4);
		} else {
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_CRANE_SIDEMOVE2,
			                                &mPosition, 0, nullptr, 0, 4);
		}
	}
}

TRiccoHookManager::TRiccoHookManager(const char* name)
    : TEnemyManager(name)
{
}

void TRiccoHookManager::createModelData()
{
	static const TModelDataLoadEntry entry[2]
	    = { { "riccohook.bmd", 0x10000000, 0 }, { 0 } };
	createModelDataArray(entry);
}

void TRiccoHookManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new THookParams("/enemy/riccohook.prm");
	TEnemyManager::load(stream);
}

TSpineEnemy* TRiccoHookManager::createEnemyInstance() { return nullptr; }

// Same "fake" inline as seen in walkerEnemy.cpp's moveObject func
// Can't find any other way to get the * 1.0f's to emit
static inline JGeometry::TVec3<f32> polarXZ(f32 theta, f32 radius)
{
	f32 c = radius * JMACos(theta);
	f32 s = radius * JMASin(theta);
	return JGeometry::TVec3<f32>(s, 0.0f, c);
}

// Parked TU-local helper: retail's distance test copies the left operand into
// a stack vector, subtracts component-wise in place and then takes its length
// (a 12-byte copy at 0x9c followed by three fsubs with store-back).
// JGeometry::TVec3<f32>::distance() expands to a different shape and has no map
// symbol at all, so the site went through a copy-and-subtract helper.
// TODO: promote once the real helper is identified; batch 83 found the same
// shape in TEMario::perform and AnimalNerve::calcDist.
static inline f32 riccoHookDistance(const JGeometry::TVec3<f32>& a,
                                    const JGeometry::TVec3<f32>& b)
{
	JGeometry::TVec3<f32> diff = a;
	diff.sub(b);
	return diff.length();
}

DEFINE_NERVE(TNerveRHGraphWander, TLiveActor)
{
	TRiccoHook* self = (TRiccoHook*)spine->getBody();

	if (spine->getTime() == 0) {
		f32 y                              = self->getRotation().y;
		const JGeometry::TVec3<f32>& polar = polarXZ(y, 1.0f);

		self->goToDirectedNextGraphNode(polar);
	}

	if (riccoHookDistance(self->unk104.getPoint(), self->getPosition())
	    < 10.0f) {
		TGraphNode& node = self->unk124->getCurrent();

		if (node.checkFlag(0x800)) {
			self->mTimer = node.getRailNode()->mPitch;
		}

		spine->pushAfterCurrent(&TNerveRHGraphWander::theNerve());
		return true;
	} else {
		if (self->unk124->unk0 == nullptr || self->unk124->unk0->isDummy()) {
			return false;
		}

		if (self->mTimer > 0) {
			return false;
		}

		// TODO (closure batch 212): exactly one of this function's three
		// accessor reads is a raw member read in retail -- the dead
		// 4-byte temporary it drops is the last word of low region
		// (pool base 0x9c, not 0xa0). `self->mRotation.y`,
		// `riccoHookDistance(..., self->mPosition)` and this `sub`
		// argument each land the function byte-exact on their own and
		// nothing distinguishes them; this one is chosen because the
		// same block writes `self->mPosition.add(dPos)` raw two lines
		// below.
		JGeometry::TVec3<f32> dPos = self->getUnkF4().getPoint();
		dPos.sub(self->mPosition);
		PSVECNormalize(&dPos, &dPos);
		dPos.scale(self->getMarchSpeed());
		self->mPosition.add(dPos);
		return false;
	}
}
