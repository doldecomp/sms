#include <JSystem/JAudio/JALibrary/JALModSe.hpp>
#include <JSystem/JAudio/JALibrary/JALSystem.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>

JALSystem::TFlagManager* JALSystem::spFManager;

void JALSystem::init() { spFManager = new TFlagManager; }

bool JALSystem::gateCheckFunc(u32 param_1, f32 param_2)
{
	if (TFlagManager::get()->isRegistered(param_1, ModType_JALSeModVolFunk)
	    && JALSeModVolFunk::gateCheck(param_1, param_2))
		return true;
	if (TFlagManager::get()->isRegistered(param_1, ModType_JALSeModPitFunk)
	    && JALSeModPitFunk::gateCheck(param_1, param_2))
		return true;
	if (TFlagManager::get()->isRegistered(param_1, ModType_JALSeModEffFunk)
	    && JALSeModEffFunk::gateCheck(param_1, param_2))
		return true;
	if (TFlagManager::get()->isRegistered(param_1, ModType_JALSeModVolFGrp)
	    && JALSeModVolFGrp::gateCheckGrp(param_1, param_2))
		return true;
	if (TFlagManager::get()->isRegistered(param_1, ModType_JALSeModPitFGrp)
	    && JALSeModPitFGrp::gateCheckGrp(param_1, param_2))
		return true;
	if (TFlagManager::get()->isRegistered(param_1, ModType_JALSeModEffFGrp)
	    && JALSeModEffFGrp::gateCheckGrp(param_1, param_2))
		return true;

	return false;
}

void JALSystem::processModFunc(JAISound* param_1, f32 param_2, u32 param_3,
                               u8 param_4)
{
	if (!param_1)
		return;

	f32 val;

	u32 thing = param_1->getID();

	if (TFlagManager::get()->isRegistered(thing, ModType_JALSeModVolFunk)) {
		if (JALSeModVolFunk::calc(thing, param_2, &val))
			param_1->setVolume(val, param_3, param_4);
	} else if (TFlagManager::get()->isRegistered(thing,
	                                             ModType_JALSeModVolFGrp)) {
		if (JALSeModVolFGrp::calcGrp(thing, param_2, &val))
			param_1->setVolume(val, param_3, param_4);
	}

	if (TFlagManager::get()->isRegistered(thing, ModType_JALSeModPitFunk)) {
		if (JALSeModPitFunk::calc(thing, param_2, &val))
			param_1->setPitch(val, param_3, param_4);
	} else if (TFlagManager::get()->isRegistered(thing,
	                                             ModType_JALSeModPitFGrp)) {
		if (JALSeModPitFGrp::calcGrp(thing, param_2, &val))
			param_1->setPitch(val, param_3, param_4);
	}

	if (TFlagManager::get()->isRegistered(thing, ModType_JALSeModEffFunk)) {
		if (JALSeModEffFunk::calc(thing, param_2, &val))
			param_1->setFxmix(val, param_3, param_4);
	} else if (TFlagManager::get()->isRegistered(thing,
	                                             ModType_JALSeModEffFGrp)) {
		if (JALSeModEffFGrp::calcGrp(thing, param_2, &val))
			param_1->setFxmix(val, param_3, param_4);
	}
}

f32 JALSystem::processModDistVolume(u32 param_1, f32 param_2)
{
	f32 val;
	u32 id = param_1;
	if (TFlagManager::get()->isRegistered(id, ModType_JALSeModVolDist)) {
		if (JALSeModVolDist::calc(id, param_2, &val))
			return val;
	} else if (TFlagManager::get()->isRegistered(id, ModType_JALSeModVolDGrp)) {
		if (JALSeModVolDGrp::calcGrp(id, param_2, &val))
			return val;
	}

	return 1.0f;
}

f32 JALSystem::processModDistPitch(u32 param_1, f32 param_2)
{
	f32 val;
	u32 id = param_1;
	if (TFlagManager::get()->isRegistered(id, ModType_JALSeModPitDist)) {
		if (JALSeModPitDist::calc(id, param_2, &val))
			return val;
	} else if (TFlagManager::get()->isRegistered(id, ModType_JALSeModPitDGrp)) {
		if (JALSeModPitDGrp::calcGrp(id, param_2, &val))
			return val;
	}

	return 1.0f;
}

f32 JALSystem::processModDistFx(u32 param_1, f32 param_2)
{
	f32 val;
	u32 id = param_1;
	if (TFlagManager::get()->isRegistered(id, ModType_JALSeModEffDist)) {
		if (JALSeModEffDist::calc(id, param_2, &val))
			return val;
	} else if (TFlagManager::get()->isRegistered(id, ModType_JALSeModEffDGrp)) {
		if (JALSeModEffDGrp::calcGrp(id, param_2, &val))
			return val;
	}

	return 1.0f;
}

// TODO: 99.8%. Not one instruction is missing, extra or reordered; the only
// residue is 104 bytes (0x68) of temp pool, frame 0x130 vs 0xc8. The named
// block is already exact in shape and order -- set1 at frame-0x44/-0x40, prm at
// -0x48, set2 at -0x50/-0x4c, set3 at -0x58/-0x54 -- and so are the six `new`
// result temps at 0x58..0x6c, which sit at the *same* absolute offsets in both.
// So retail allocates 0x68 more pool between those temps and the named block.
// 0x68 is 13*8, and there are twelve in-class `JALSeModXxx` constructors
// expanded here, which makes "one 8-byte inline temp per expansion plus one"
// the obvious reading. Header round 23 tested the shared header and the
// arithmetic does not close:
//   * a binding inside `TFlagManager::get()` is +8 per expansion, i.e. +0x30
//     for the six arms that call `addUseFlag` -- but it also breaks
//     `gateCheckFunc`, `processModFunc` and `processModDistVolume`, which are
//     exact today, so it is a net loss even though it is the only lever found
//     that moves this frame at all.
//   * a named pointer local per arm (`JALSeModVolFunk* mod = new ...`) is +4
//     each, +0x30 for twelve arms; 0x30 + 0x30 still leaves 8.
//   * a dead uninitialised non-trivial 8-byte local in the shared
//     `JALSeModData<T>` ctor body is +0 (that ctor expansion is not a
//     dead-low-region carrier), and the same local in the twelve leaf ctor
//     bodies is not dead at all -- it emits 30 extra instructions.
// There is no `appendGrp` in the map, so the thirteenth expansion is not a
// sibling entry point. 0x68 is not divisible by twelve, so part of it is not
// per-arm; whatever is per-arm is worth 4 or 8 and there is a fixed remainder.
// Next lead: the gap sits *above* the six `__nw__` result temps at 0x58..0x6c,
// so it is produced by the ctor expansions themselves, and the levels inside
// them (`JALSeModDataGrp<T>` and `JALListHioNode<T,u32>`) are untested.
// Closure round 2026-09-18 -- the frame is now a measured ladder, but no rung
// lands it. A TU-local binder used only by `append` (so the TU's other four
// functions keep the plain accessor and stay byte-exact) moves the frame with
// no instruction change:
//   binder over `TFlagManager::get()`, n sites: 0xd0 0xe0 0xe8 0xf8 0x100 0x110
//     (alternating +8/+16, +0x48 over all six `addUseFlag` arms)
//   binder over `JALSystem::spFManager`, six sites: 0xf8 (a uniform +8 each)
//   a second binding level nested in the binder: +24 per site (0x128 at four
//     sites, 0x140 at five, 0x158 at six)
//   a direct-return fork nested in the binder: identical to the plain binder
//   a `void` wrapper doing the `addUseFlag` call itself: identical
//   a pass-through pointer binder on `&set1` or `&prm` at all twelve arms:
//     +12 each (0x158), and it perturbs the named block (131 operands)
//   copy-initialising the four prologue objects: +14 instructions
//   a named `JALSeModXxx* mod` per arm is +0 once a binder is present
// Six-site binder (0x110) plus 0x20 is the target, and 0x20 is 4 x 8 against
// the four named `JALPrmSet`/`JADPrmS<f32>` constructions in the prologue, so
// the parked need is a +8 binding level *inside* those constructors, i.e. in
// `JALModSe.hpp`/`JALPrmSet` -- shared headers (MSoundBGM, mameGesso and
// MSSetSound.hpp all include JALModSe.hpp), so it is reported, not made.
// Nothing here is committed: every rung above leaves the frame wrong and the
// binder alone takes the differing-operand count from 56 to 74.
// Closure round 2026-09-18 (2) -- the residue is now provably frame-only and
// the missing 0x68 is arithmetically solved, but no honest spelling produces
// it yet. Two facts settle where it lives:
//   * the ModType_JALSeModPitFunk arm passes `set3`, not `set2` (retail's
//     `addi r8, r1, 0xd8`), so all four `Pit` arms share the second pair.
//     Fixed below; with `volatile char trash[0x68]` declared after `set3` the
//     function is 100.0%, 371/371 instructions, zero differing operands.
//   * the six `new`-result temps keep their absolute offsets 0x58..0x6c in
//     both builds, so the 0x68 is one contiguous block allocated *above* them,
//     i.e. during the four prologue constructions and before any switch arm.
//     That rules the Grp/leaf constructors out by position: a reservation in
//     `JALSeModDataGrp<T>` or in the twelve leaf ctors is allocated after the
//     arm temps and would move them.
// Measured ladder for the prologue expansions (dead uninitialised non-trivial
// class local in the named constructor, zero instructions either way):
//   JALPrmSet ctor, 8-byte local   -> +0x18 (3 expansions x 8): frame 0xe0
//   JADPrmS<T> ctor, 8-byte local  -> +0x38 (7 expansions x 8): frame 0x100
//   JALPrmSet 16-byte + JADPrmS 8  -> +0x68 exactly: frame 0x130, 100.0%,
//                                     zero differing operands
// So the residue is 3 x 16 + 7 x 8 = 0x68, equivalently 8 bytes for the
// standalone `prm` plus 16 for each of the six `JADPrmS` subobjects nested
// inside a `JALPrmSet`. The open question is purely what real C++ reserves
// that: refuted this round are `JADPrmS(const T&, const char*)` (+2
// instructions, frame unmoved), `JALPrmSet(const f32&, const f32&)` (one
// opcode differs, frame unmoved) and a defaulted `const char* name = nullptr`
// on `JADPrmS` with the explicit nulls dropped at all seven sites (+0, exactly
// codegen-neutral). Probe structs are padding by another name and are not
// committed.
// Research round 2026-09-19 -- the *mechanism* is now named, the type is not.
// A defaulted argument of class type whose object is never read is exactly
// codegen-neutral and still reserves stack at every call site of the inline
// that declares it, at align8(sizeof) for the object plus the outgoing
// by-value copy area once the class no longer fits a register:
//   sizeof <= 4 -> +8 per site   sizeof 8 -> +16   sizeof 16 -> +32
// (This refines header round 19's "a defaulted argument is a codegen-neutral
// inline level": neutral in instructions, not in frame, once it is a class.)
// One such argument on `JADPrmS<T>`'s ctor (7 sites x 8 = 0x38) plus one
// 8-byte one on `JALPrmSet`'s (3 sites x 16 = 0x30) is 0x68 exactly: frame
// 0x130, 100.0%, 371/371, zero differing operands, and `ninja changes_all`
// moves nothing else in the tree -- MSoundBGM, mameGesso and MSSetSound are
// untouched, so the shape is surgical as well as exact. Two 4-byte arguments
// on `JALPrmSet` land the same total, as would a dead 12-16-byte class local
// in `JALPrmSet`'s ctor over a dead <=8-byte one in `JADPrmS`'s.
// Refuted this round, each measured alone: by-value `JADPrmS<f32>` parameters
// on `JALPrmSet` with a converting `JADPrmS(T, const char* = nullptr)` (frame
// 0x110, +27 instructions); copy-initialising the four prologue objects from
// same-type temporaries (0xe0, +14); a `JADPrmName` class carrying the
// `const char*` by value into `JADPrmS` (0x118, +24); and forwarding
// `JALPrmSet`'s two defaulted objects to its members instead of defaulting
// them again at the member sites (0x118, zero instructions but 0x50, not
// 0x68). A *live* by-value class argument always pays the store/reload, so
// only an argument nothing reads is free -- which is why the landing shape
// stays uncommitted: an empty class invented to hold the slot is a probe
// struct with extra steps, and neither marioUS.MAP nor any clean source
// (`gh search code JADPrm` returns only copies of this header) names a 4- or
// 8-byte JAudio debug type to put there. Name the type and the unit links in
// one edit.
// Sibling: `MSSetSoundTL`'s constructor is missing exactly the same 0x68 with
// seventeen `JADPrm` constructions, so "8 bytes per JADPrmS materialised" does
// not generalise; either the two are independent, or both bodies declare one
// dead object of the same unnamed ~104-byte audio-debug type.
// Unit round 2026-09-20 -- TU-local landing path confirmed and reverted:
//   struct JADPrmSF : JADPrmS<f32> { JADPrmSF(f32, const char*, Tag4=Tag4()); };
//   JALPrmSet(f32, f32, Tag4=Tag4(), Tag4=Tag4());  // or one Tag8
// with `JADPrmSF prm(...)` in append is frame 0x130, 100.0%, 371/371, zero
// operand diffs; `changes_all` moves only this unit (matched_code 86.93->100,
// fuzzy 99.98->100); DOL sha1 unchanged while unlinked. Tag4/Tag8 as invented
// empty classes, or `JGeometry::TUtil<f32>` as the research-214 stand-in, are
// still fakematches (no JAudio evidence for the type). Shared `JADPrm.hpp` is
// not required: the derived ctor keeps the unread arg off the emitted
// `JADPrm<T>` weak. Parked on naming the real type.
// Closure follow-up 2026-09-22 -- `JAInter::TDebugHeap` is the one existing
// empty JAudio class that reaches the landing: one ignored default on
// `JADPrmS` and two on `JALPrmSet` are 100.0% with no other changed unit.
// Its map-attested role is only a static debug-heap holder, though; no binary
// or source evidence connects it to JAD parameter constructors, so this
// spelling was tested only in shared headers and reverted rather than guessed.
void JALSystem::append(JALSystem::ModType param_1, const char* param_2,
                       u32 param_3, f32 param_4, f32 param_5, f32 param_6,
                       f32 param_7, f32 param_8, JALCalc::CurveSign param_9,
                       f32 param_10, f32 param_11, u8 param_12)
{
	JALPrmSet set1(param_4, param_5);
	JADPrmS<f32> prm(param_6, nullptr);
	JALPrmSet set2(param_7, param_8);
	JALPrmSet set3(param_7, param_8);

	switch (param_1) {
	case ModType_JALSeModVolFunk:
		new JALSeModVolFunk(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		TFlagManager::get()->addUseFlag(param_3, param_1);
		break;

	case ModType_JALSeModPitFunk:
		new JALSeModPitFunk(param_2, param_3, &set1, &set3, &prm, param_9,
		                    param_12);
		TFlagManager::get()->addUseFlag(param_3, param_1);
		break;

	case ModType_JALSeModEffFunk:
		new JALSeModEffFunk(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		TFlagManager::get()->addUseFlag(param_3, param_1);
		break;

	case ModType_JALSeModVolDist:
		new JALSeModVolDist(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		TFlagManager::get()->addUseFlag(param_3, param_1);
		break;

	case ModType_JALSeModEffDist:
		new JALSeModEffDist(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		TFlagManager::get()->addUseFlag(param_3, param_1);
		break;

	case ModType_JALSeModPitDist:
		new JALSeModPitDist(param_2, param_3, &set1, &set3, &prm, param_9,
		                    param_12);
		TFlagManager::get()->addUseFlag(param_3, param_1);
		break;

	case ModType_JALSeModVolFGrp:
		new JALSeModVolFGrp(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		break;

	case ModType_JALSeModPitFGrp:
		new JALSeModPitFGrp(param_2, param_3, &set1, &set3, &prm, param_9,
		                    param_12);
		break;

	case ModType_JALSeModEffFGrp:
		new JALSeModEffFGrp(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		break;

	case ModType_JALSeModVolDGrp:
		new JALSeModVolDGrp(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		break;

	case ModType_JALSeModEffDGrp:
		new JALSeModEffDGrp(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		break;

	case ModType_JALSeModPitDGrp:
		new JALSeModPitDGrp(param_2, param_3, &set1, &set3, &prm, param_9,
		                    param_12);
		break;
	}
}

void JALSystem::appendGrpMember(JALSystem::ModType param_1, u32 param_2,
                                u32 param_3)
{
	switch (param_1) {
	// Retail really does double-check `found` in the last four arms and not in
	// the first two; dropping the redundant tests loses eight instructions, and
	// spelling the first two arms the same way costs four.
	case ModType_JALSeModVolFGrp: {
		JALSeModDataGrp<JALSeModVolFGrp>* found
		    = JALListS<JALSeModVolFGrp, u32>::search(param_2);
		if (found) {
			JALSeModDataGrpMemb* memb
			    = new JALSeModDataGrpMemb(param_3, nullptr);
			found->append(memb);
		}
		spFManager->addUseFlag(param_3, param_1);
	} break;
	case ModType_JALSeModPitFGrp: {
		JALSeModDataGrp<JALSeModPitFGrp>* found
		    = JALListS<JALSeModPitFGrp, u32>::search(param_2);
		if (found) {
			JALSeModDataGrpMemb* memb
			    = new JALSeModDataGrpMemb(param_3, nullptr);
			found->append(memb);
		}
		spFManager->addUseFlag(param_3, param_1);
	} break;
	case ModType_JALSeModEffFGrp: {
		JALSeModDataGrp<JALSeModEffFGrp>* found
		    = JALListS<JALSeModEffFGrp, u32>::search(param_2);
		if (found) {
			JALSeModDataGrpMemb* memb
			    = new JALSeModDataGrpMemb(param_3, nullptr);
			if (found)
				found->append(memb);
		}
		spFManager->addUseFlag(param_3, param_1);
	} break;
	case ModType_JALSeModVolDGrp: {
		JALSeModDataGrp<JALSeModVolDGrp>* found
		    = JALListS<JALSeModVolDGrp, u32>::search(param_2);
		if (found) {
			JALSeModDataGrpMemb* memb
			    = new JALSeModDataGrpMemb(param_3, nullptr);
			if (found)
				found->append(memb);
		}
		spFManager->addUseFlag(param_3, param_1);
	} break;
	case ModType_JALSeModEffDGrp: {
		JALSeModDataGrp<JALSeModEffDGrp>* found
		    = JALListS<JALSeModEffDGrp, u32>::search(param_2);
		if (found) {
			JALSeModDataGrpMemb* memb
			    = new JALSeModDataGrpMemb(param_3, nullptr);
			if (found)
				found->append(memb);
		}
		spFManager->addUseFlag(param_3, param_1);
	} break;
	case ModType_JALSeModPitDGrp: {
		JALSeModDataGrp<JALSeModPitDGrp>* found
		    = JALListS<JALSeModPitDGrp, u32>::search(param_2);
		if (found) {
			JALSeModDataGrpMemb* memb
			    = new JALSeModDataGrpMemb(param_3, nullptr);
			if (found)
				found->append(memb);
		}
		spFManager->addUseFlag(param_3, param_1);
	} break;
	}
}

JALSystem::TFlagManager::TFlagManager()
{
	u16 size;
	u8 i;
	u16 j;
	for (i = 0; i < 16; ++i) {
		size    = JAIBasic::getInterface()->unk0->mSeTable.mSoundMax[i];
		unk0[i] = new u16[size];
		for (j = 0; j < size; ++j)
			unk0[i][j] = 0;
	}
}

void JALSystem::TFlagManager::addUseFlag(u32 param_1, u16 param_2)
{
	unk0[(u16)param_1 >> 12][param_1 & JAISoundID_IndexMask] += param_2;
}

u16 JALSystem::TFlagManager::getUseFlag(u32 param_1)
{
	return unk0[(u16)param_1 >> 12][param_1 & JAISoundID_IndexMask];
}

bool JALSystem::TFlagManager::isRegistered(u32 param_1, u16 param_2)
{
	if (param_2 & unk0[(u16)param_1 >> 12][param_1 & JAISoundID_IndexMask])
		return true;
	return false;
}
