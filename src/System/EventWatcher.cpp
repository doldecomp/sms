#include <System/EventWatcher.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/MarioGamePad.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <Strategic/Spine.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <GC2D/Talk2D2.hpp>
#include <GC2D/GCConsole2.hpp>
#include <GC2D/ConsoleStr.hpp>
#include <GC2D/SunGlass.hpp>
#include <NPC/NpcBase.hpp>
#include <NPC/NpcEvent.hpp>
#include <Map/MapEventSink.hpp>
#include <Map/PollutionManager.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/ModelGate.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/MapObjItem2.hpp>
#include <MoveBG/MapObjBall.hpp>
#include <MoveBG/MapObjTown.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Emario.hpp>
#include <Player/Mario.hpp>
#include <Player/WaterGun.hpp>
#include <Camera/CubeManagerBase.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

// TODO: from M3UJoint or J3DJoint?
static void dummy()
{
	(Vec) { 0.0f, 0.0f, 0.0f };
	(Vec) { 1.0f, 1.0f, 1.0f };
}

static void evGetSystemFlag(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TSpcSlice idSlice = interp->pop();
	int id            = idSlice.getDataInt();

	interp->push((int)TFlagManager::getInstance()->getFlag(id));
}

static void evSetSystemFlag(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	TSpcSlice valueSlice = interp->pop();
	TSpcSlice flagSlice  = interp->pop();
	int flag             = flagSlice.getDataInt();
	int value            = valueSlice.getDataInt();

	TFlagManager::getInstance()->setFlag(flag, value);

	interp->push();
}

static void evGetNameRefHandle(TSpcTypedInterp<TEventWatcher>* interp,
                               u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);

	JDrama::TNameRef* ref = JDrama::TNameRefGen::search<JDrama::TNameRef>(
	    interp->pop().getDataString());

	interp->push((int)ref);
}

static void evGetNameRefName(TSpcTypedInterp<TEventWatcher>* interp,
                             u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);

	int ref = interp->pop().getDataInt();

	const char* name;
	if (ref)
		name = ((JDrama::TNameRef*)(u32)ref)->getName();
	else
		name = "";

	interp->push(name);
}

// The `name` local is necessary. It gives this function the compiled size that
// the symbol map records for it (0xe4), and it gives each of the ~15 callers
// the one extra stack object that they need. Do not remove it.
static JDrama::TNameRef* getNameRefPtr(TSpcSlice slice)
{
	JDrama::TNameRef* result = nullptr;

	switch (slice.typeof()) {
	case TSpcSlice::TYPE_STRING: {
		const char* name = slice.getDataString();
		result           = JDrama::TNameRefGen::search<JDrama::TNameRef>(name);
		break;
	}

	case TSpcSlice::TYPE_INT:
		result = (JDrama::TNameRef*)(u32)slice.getDataInt();
		break;
	}

	return result;
}

// Binding level worth +16 of low region, landing evGetNPCType's frame at
// 0x90 (batch 124).
static inline u32 EventWatcherGetActorType(const TBaseNPC* p)
{
	u32 actorType = p->getActorType();
	return actorType;
}

static void evGetNPCType(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int result    = -1;
	TBaseNPC* npc = (TBaseNPC*)getNameRefPtr(interp->pop());
	if (npc)
		result = EventWatcherGetActorType(npc) - 0x4000001;
	interp->push(result);
}

static void evSetFlagNPCDontTalk(TSpcTypedInterp<TEventWatcher>* interp,
                                 u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	bool enable   = interp->pop().getDataInt() != 0;
	TBaseNPC* npc = (TBaseNPC*)getNameRefPtr(interp->pop());
	if (npc) {
		if (enable)
			npc->onLiveFlag(TBaseNPC::LIVE_FLAG_DONT_TALK);
		else
			npc->offLiveFlag(TBaseNPC::LIVE_FLAG_DONT_TALK);
	}
	interp->push();
}

static void evSetFlagNPCDontThrow(TSpcTypedInterp<TEventWatcher>* interp,
                                  u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	bool enable   = interp->pop().getDataInt() != 0;
	TBaseNPC* npc = (TBaseNPC*)getNameRefPtr(interp->pop());
	if (npc) {
		if (enable)
			npc->onLiveFlag(TBaseNPC::LIVE_FLAG_DONT_THROW);
		else
			npc->offLiveFlag(TBaseNPC::LIVE_FLAG_DONT_THROW);
	}
	interp->push();
}

static void evSetFlagNPCDead(TSpcTypedInterp<TEventWatcher>* interp,
                             u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	bool enable   = interp->pop().getDataInt() != 0;
	TBaseNPC* npc = (TBaseNPC*)getNameRefPtr(interp->pop());
	if (npc) {
		if (enable)
			npc->onLiveFlag(LIVE_FLAG_DEAD);
		else
			npc->offLiveFlag(LIVE_FLAG_DEAD);
	}
	interp->push();
}

// TODO: 93.3%. Blocked on the same expanded-instead-of-called
// JGeometry::TUtil<f32>::sqrt as evIsNearActors; see the trial table there.
static void evIsNearSameActors(TSpcTypedInterp<TEventWatcher>* interp,
                               u32 arg_num)
{
	interp->verifyArgNum(3, &arg_num);
	THitActor* first = (THitActor*)getNameRefPtr(interp->pop());
	if (!first) {
		interp->push();
		return;
	}
	int type = first->getActorType();

	f32 dist          = interp->pop().getDataFloat();
	TLiveActor* which = (TLiveActor*)getNameRefPtr(interp->pop());
	if (!which) {
		interp->push();
		return;
	}

	int count = 0;
	int num   = gpMapObjManager->getObjNum();
	for (int i = 0; i < num; ++i) {
		TMapObjBase* obj = gpMapObjManager->getObj(i);
		if (type == obj->getActorType()) {
			JGeometry::TVec3<f32> diff = which->getPosition();
			diff -= obj->mPosition;
			if (diff.length() <= dist)
				count++;
		}
	}
	interp->push(count);
}

/// Counts how many of the actors named by arguments 2..n sit within a given
/// distance of the actor named by argument 0. The arguments stay on the process
/// stack until the end, because the count of them is only known at run time.
// TODO: 88.5%, and every remaining difference here and in evIsNearSameActors
// follows from one inline decision: retail *calls*
// JGeometry::TUtil<f32>::sqrt for `diff.length()` while we expand it. The call
// is what clobbers r3 (so retail parks the search result in r27 in
// evIsNearSameActors), what forces gpMapObjManager to be re-read every
// iteration there, and what stops MWCC from turning that loop into
// mtctr/bdnz.
//
// Measured, with `length()` reached from the builtin through n inlined
// wrappers (fabricated statics taking the two actors):
//   n=0 (this shape)                sqrt expanded          88.5%
//   n=1                             sqrt expanded          87.0%
//   n=2                             sqrt called, but the
//                                   `diff -= p` sub goes
//                                   out of line too        83.7%
//   n=2 + `diff.x -= p.x` spelled   sqrt called, but
//        out per component          TVec3::dot goes out
//                                   of line                90.2%
// So sqrt (4 statements) needs depth 4 to stay a call, while dot (1
// statement) must still be inlined at that depth -- impossible while
// `length()` reaches `dot()` through `squared()`, which adds a level.
//
// Two shared-header shapes in include/JSystem/JGeometry/JGVec3.hpp remove
// that level; both were measured over the whole tree and are NOT applied
// here:
//   `squared()` spelled `x * x + y * y + z * z`
//       this function 95.8%, but Total fuzzy 96.47% -> 96.44%, regressing
//       boid, BeeHive, four Camera units, ten Enemy units and more.
//   `length()` spelled `TUtil<f32>::sqrt(dot(*this))`
//       this function 97.3% with every register matching, Total fuzzy
//       96.47% -> 96.48% and only boid (98.04 -> 96.79) and Kazekun
//       (96.37 -> 96.07) regressing. This is the candidate worth revisiting
//       once boid is understood.
//
// Also still open: retail computes the loop's stack index as one subtraction
// (`mCount - (arg_num - i)`) while `arg_num - i - 1` lets MWCC hoist
// `arg_num - 1` and strength-reduce; `arg_num - (i + 1)` is inert, and
// TVec3::distance() in place of the diff/length() pair is worse in both.
static void evIsNearActors(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	int count = 0;

	if (arg_num >= 3) {
		THitActor* which = (THitActor*)getNameRefPtr(
		    interp->mProcessStack.getFromTop(arg_num - 1));
		if (which) {
			f32 dist
			    = interp->mProcessStack.getFromTop(arg_num - 2).getDataFloat();

			count = 1;
			for (u32 i = 2; i < arg_num; ++i) {
				THitActor* other = (THitActor*)getNameRefPtr(
				    interp->mProcessStack.getFromTop(arg_num - i - 1));
				if (other) {
					JGeometry::TVec3<f32> diff = which->mPosition;
					diff -= other->mPosition;
					if (diff.length() <= dist)
						count++;
				}
			}
		}
	}

	for (int i = 0; i < (int)arg_num; ++i)
		interp->pop();

	interp->push(count);
}

static inline TMarDirector* EventWatcherDirectorForTalkNPC()
{
	TMarDirector* marDirector = gpMarDirector;
	return marDirector;
}

static void evGetTalkNPC(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);

	TBaseNPC* npc = EventWatcherDirectorForTalkNPC()->getTalkingNPC();

	interp->push(!npc ? 0 : (int)npc);
}

static void evGetTalkNPCName(TSpcTypedInterp<TEventWatcher>* interp,
                             u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);

	TBaseNPC* npc = SMSGetMarDirector()->getTalkingNPC();

	// Named so push(const char*) cannot prove the pointer non-null and
	// drop setDataString's `if (!v)` — `push("")` constant-folds it
	// away (94.4% -> 98.1%). Merging both arms into one name local
	// collapses the two push expansions (60%). Residue is the 4-byte
	// slice-slot family.
	if (!npc) {
		const char* name = "";
		interp->push(name);
	} else
		interp->push(npc->getName());
}

// TODO: `TSpcSlice(interp->pop()).getDataInt()` is a placeholder for something
// this reconstruction has not identified.
//
// About half of the builtins that read an integer argument copy the popped
// slice into a *second* stack object and then read mType back out of memory,
// while the other half read the popped slice in place. Only a by-value
// TSpcSlice produces that copy, so those builtins must have gone through some
// helper that takes a slice by value -- the same way this file's own
// getNameRefPtr(TSpcSlice) does. The name is not recoverable: a helper that
// every translation unit inlines completely leaves no symbol in mario.MAP, and
// mario.MAP lists no other by-value slice function for this file.
//
// Writing the copy out by hand is what the explicit constructor call below
// does. It is spelled that way at every site whose disassembly shows the copy,
// and nowhere else -- an inline helper of our own does not work, because the
// extra call level pushes getDataInt out of line. Also rejected, by
// measurement: a named `TSpcSlice` local (fewer temporaries, not more), a
// popInt() on TSpcInterp, `operator int()`, and an extra copy inside
// TSpcInterp::pop() itself (that one makes the whole file worse).
// Bare-return fork over the raw global. Named at the call site it lands
// retail's 0x88 frame and the `addi r5, r30, 0` argument copy; the raw global
// named directly, or the binder form of this fork, stays 8 or 4 bytes off.
static inline TTalk2D2* EventWatcherTalk2DForMsgID() { return gpTalk2D; }

static void evSetTalkMsgID(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	int p1 = TSpcSlice(interp->pop()).getDataInt();
	int p2 = TSpcSlice(interp->pop()).getDataInt();
	TTalk2D2* talk2D = EventWatcherTalk2DForMsgID();
	talk2D->setMessageID(p2, p1);
	interp->push();
}

// evGetTalkMode matches with a direct-return fork and a named int result.
// TODO: frames now exact via TU-local binders (the
// director for getTalkNPC). Residue is load order: retail
// stores the slice type word, then the inlined member. A helper that
// both names the pointer and pushes made TSpcStack::push a `bl` (38%).
// evGetTalkNPCName is the same family at a 4-byte slice slot; evIsTalkModeNow
// is frame-exact with the slice 4 low.
static inline u32 EventWatcherTalkMode()
{
	return gpTalk2D->getTalkMode();
}

static void evGetTalkMode(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	int mode = EventWatcherTalkMode();
	interp->push(mode);
}

// Binding level over a raw member read, worth +8 of low region in
// evForceCloseTalk (batch 127); with a named int result it also closes
// evGetTalkSelectedValue.
static inline TTalk2D2* EventWatcherGetTalk2D()
{
	TTalk2D2* talk2D = gpTalk2D;
	return talk2D;
}

static void evGetTalkSelectedValue(TSpcTypedInterp<TEventWatcher>* interp,
                                   u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	int value = EventWatcherGetTalk2D()->getSelectedValue();
	interp->push(value);
}

static void evSetValue2TalkVariable(TSpcTypedInterp<TEventWatcher>* interp,
                                    u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	int value = TSpcSlice(interp->pop()).getDataInt();
	int index = TSpcSlice(interp->pop()).getDataInt();

	// not implemented?

	interp->push();
}

// Bare-return fork whose body calls the header accessor: +4 of pool.
static inline bool EventWatcherIsTalkModeNow()
{
	return SMSGetMarDirector()->isTalkModeNow();
}

static void evIsTalkModeNow(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	int value = EventWatcherIsTalkModeNow() ? 1 : 0;
	interp->push(value);
}

static void evSetFlagNPCCanTaken(TSpcTypedInterp<TEventWatcher>* interp,
                                 u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	s32 arg          = TSpcSlice(interp->pop()).getDataInt();
	const char* name = interp->pop().getDataString();
	TBaseNPC* npc    = (TBaseNPC*)JDrama::TNameRefGen::search2(name);
	if (npc) {
		if (arg)
			npc->onLiveFlag(LIVE_FLAG_UNK100000);
		else
			npc->offLiveFlag(LIVE_FLAG_UNK100000);
	}
	interp->push();
}

// TODO: removeme
extern const TNerveBase<TLiveActor>* NerveGetByIndex(int param_1);

static void evPushNerve4LiveActor(TSpcTypedInterp<TEventWatcher>* interp,
                                  u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	int nerveId                         = TSpcSlice(interp->pop()).getDataInt();
	const TNerveBase<TLiveActor>* nerve = NerveGetByIndex(nerveId);
	const char* actorName               = interp->pop().getDataString();

	TLiveActor* liveActor
	    = (TLiveActor*)JDrama::TNameRefGen::search2(actorName);
	if (liveActor && nerve)
		liveActor->mSpine->pushNerve(nerve);

	interp->push();
}

// TODO: 99.8%, instruction-exact at retail's 0xb0; every getNameRefPtr and
// push temporary sits 4 low. Inert: `result` or `liveActor` declared early,
// u32 flag, early return, `!= nullptr`; slice copies change the code.
static void evIsOnLiveActorFlag(TSpcTypedInterp<TEventWatcher>* interp,
                                u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	int flag = TSpcSlice(interp->pop()).getDataInt();

	TLiveActor* liveActor = (TLiveActor*)getNameRefPtr(interp->pop());

	int result = 0;
	if (liveActor)
		result = liveActor->mLiveFlag & flag;
	interp->push(result);
}

static void evSetHide4LiveActor(TSpcTypedInterp<TEventWatcher>* interp,
                                u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	s32 value             = TSpcSlice(interp->pop()).getDataInt();
	const char* actorName = interp->pop().getDataString();

	TLiveActor* liveActor
	    = (TLiveActor*)JDrama::TNameRefGen::search2(actorName);
	if (liveActor) {
		if (value) {
			liveActor->onLiveFlag(LIVE_FLAG_HIDDEN);
			liveActor->onHitFlag(HIT_FLAG_NO_COLLISION);
		} else {
			liveActor->offLiveFlag(LIVE_FLAG_HIDDEN);
			liveActor->offHitFlag(HIT_FLAG_NO_COLLISION);
		}
	}

	interp->push();
}

// TODO (also evSetHide4LiveActor and evSetFlagNPCCanTaken): 99.95%, every
// instruction and every referenced local slot exact, frame 8 bytes too big
// (0xa0 vs 0x98) -- the mirror of the "last 8 bytes" family: eight unreferenced
// bytes between our highest local and the register saves. Spelling the search
// out as `getInstance()->getRootNameRef()->search(name)` removes exactly those
// 8 (frame exact) but then shifts the slots 4-8 down, so the missing level is
// one below search2, not at the call site. Naming the popped string is
// load-bearing (the ROM keeps it in r27 and copies with `addi r3, r27, 0`);
// inlining it into the search argument costs 1.2 points.
static void evSetDead4LiveActor(TSpcTypedInterp<TEventWatcher>* interp,
                                u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	s32 value             = TSpcSlice(interp->pop()).getDataInt();
	const char* actorName = interp->pop().getDataString();

	TLiveActor* liveActor
	    = (TLiveActor*)JDrama::TNameRefGen::search2(actorName);
	if (liveActor) {
		if (value) {
			liveActor->onLiveFlag(LIVE_FLAG_DEAD);
			liveActor->onHitFlag(HIT_FLAG_NO_COLLISION);
		} else {
			liveActor->offLiveFlag(LIVE_FLAG_DEAD);
			liveActor->offHitFlag(HIT_FLAG_NO_COLLISION);
		}
	}

	interp->push();
}

// Binder over the director accessor; the raw-global
// EventWatcherGetMarDirector binder is 8 cheaper.
static inline TMarDirector* EventWatcherMarDirector()
{
	TMarDirector* marDirector = SMSGetMarDirector();
	return marDirector;
}

// A named director for the stopwatch and the binder for the store: retail's
// 0x68 frame with every slot in place.
static void evSetTimeLimit(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int time               = TSpcSlice(interp->pop()).getDataInt();
	TMarDirector* director = SMSGetMarDirector();
	OSResetStopwatch(&director->unkE8);
	EventWatcherMarDirector()->unk120 = time;
	interp->push();
}

// TODO (also evSetPollutionIncreaseCount): instruction-exact, frame 0x58 vs
// retail 0x70 with every slot 0x10 low. The dead getDataInt still reserves
// slots: TSpcSlice(interp->pop()) as in evSetTimeLimit is +8, a bare pop()
// or a named slice is smaller; getDataFloat, operator int are inert or worse.
// Also measured: an f32/u32/u8 local, a const reference to the slice, a
// double TSpcSlice copy (0x68 but +14 instructions), getDataString, push(0).
static void evSetAttentionTime(TSpcTypedInterp<TEventWatcher>* interp,
                               u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int tmp = interp->pop().getDataInt();

	// not implemented?

	interp->push();
}

static void evSetPollutionIncreaseCount(TSpcTypedInterp<TEventWatcher>* interp,
                                        u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int tmp = interp->pop().getDataInt();

	// not implemented?

	interp->push();
}

static void evGetRestTime(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	// TODO: frame exact with the raw global; the pushed slice sits 4 low
	// (0x1c vs 0x20), the evIsTalkModeNow class. Inert: SMSGetMarDirector(),
	// a named director or result, (int), an explicit TSpcSlice.
	// Header lever (not taken here, MarDirector.hpp is not this unit's):
	// binding the result inside TMarDirector::getRestTime
	// (`int rest = unk120 - time; return rest;`) closes this function and
	// moves nothing else in the unit; it is getRestTime's only caller.
	interp->verifyArgNum(0, &arg_num);
	interp->push(gpMarDirector->getRestTime());
}

static void evGetPollutionLevel(TSpcTypedInterp<TEventWatcher>* interp,
                                u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	interp->push((int)gpPollution->getPollutionDegree());
}

// TODO: both are UNUSED (map 0x174 and 0x148, i.e. 93 and 82 instructions), so
// there is no assembly to read and no string of their own left in the pool --
// this TU's .rodata is already exact with the stubs empty, so whatever they
// referenced was shared. 90-odd instructions is far more than any neighbouring
// builtin, so a body cannot be invented from the name; left empty on purpose
// rather than guessed. If evidence turns up it will be the event table these
// two write into, since `interp` and `arg_num` are all they are given.
static void evSetEventStart(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
}

static void evSetEventEnd(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
}

// Binding level over a raw member read, worth +8 of low region.
static inline TMarDirector* EventWatcherGetMarDirector()
{
	TMarDirector* marDirector = gpMarDirector;
	return marDirector;
}

static void evSetNextStage(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	int scenario = TSpcSlice(interp->pop()).getDataInt();
	int stage    = TSpcSlice(interp->pop()).getDataInt();

	// The director is a named local over the plain accessor: retail's 0x88
	// frame. Chaining the accessor or the raw global is 8 short, a binder 8
	// long.
	TMarDirector* director = SMSGetMarDirector();
	director->setNextStage((scenario & 0xff) + ((stage + 1) << 8), nullptr);

	interp->push();
}

static void evRegisterMovie(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int movieId = TSpcSlice(interp->pop()).getDataInt();
	TMarDirector* director = SMSGetMarDirector();
	director->fireStreamingMovie(movieId);
	interp->push();
}

// The nil push has two spellings and they are not interchangeable: writing the
// temporary at the call site (`push(TSpcSlice())`) puts the slice one 4-byte
// pool slot *above* where the zero-argument `push()` overload puts it, because
// that overload builds the same temporary one inlining level down. Both forms
// occur among this file's exact functions, so the choice is per function; here
// the accessor binder above plus the call-site temporary is what lands
// retail's 0x38 frame with the slice at 0x24 (the raw-global fork plus
// `push()` is 0x30/0x1c, and `push()` alone with the binder is 0x38/0x20).
static void evGameOver(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	EventWatcherMarDirector()->onUnk4CFlag(0x1);
	interp->push(TSpcSlice());
}

static void evIsGraffitoCoverage0(TSpcTypedInterp<TEventWatcher>* interp,
                                  u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	interp->push(gpPollution->cleanedAll() ? 1 : 0);
}

// TODO: 99.2%, frame exact; only the loop counter/layer registers differ
// (retail i in r7 with the layer in r5 in both loops).
static void evSetGraffitoMultiplied(TSpcTypedInterp<TEventWatcher>* interp,
                                    u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int enable = TSpcSlice(interp->pop()).getDataInt();

	int i                        = 0;
	TPollutionManager* pollution = gpPollution;
	if (enable) {
		for (; i < pollution->getJointModelNum(); ++i)
			((TPollutionLayer*)pollution->getJointModel(i))->startSpread();
	} else {
		for (; i < pollution->getJointModelNum(); ++i)
			((TPollutionLayer*)pollution->getJointModel(i))->stopSpread();
	}

	interp->push();
}

static void evIsBossDefeated(TSpcTypedInterp<TEventWatcher>* interp,
                             u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	interp->push(gpConductor->isBossDefeated() ? 1 : 0);
}

static inline TGCConsole2* EventWatcherConsoleForClearDemo()
{
	return SMSGetMarDirector()->getConsole();
}

static void evLaunchEventClearDemo(TSpcTypedInterp<TEventWatcher>* interp,
                                   u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	TGCConsole2* console = EventWatcherConsoleForClearDemo();
	console->unk94->startAppearShineGet();
	console->unk47 = 1;
	interp->push();
}

static void evIsEMarioReachedToGoal(TSpcTypedInterp<TEventWatcher>* interp,
                                    u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TEMario* emario = (TEMario*)getNameRefPtr(interp->pop());
	interp->push(emario->isReachedToGate());
}

static void evIsEMarioDownWaitingToTalk(TSpcTypedInterp<TEventWatcher>* interp,
                                        u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TEMario* emario = (TEMario*)getNameRefPtr(interp->pop());
	interp->push(emario->isDownWaitingToTalk());
}

static void evStartEMarioRunAway(TSpcTypedInterp<TEventWatcher>* interp,
                                 u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TEMario* emario = (TEMario*)getNameRefPtr(interp->pop());
	emario->startRunAway();
	interp->push();
}

static void evStartEMarioGateDrawing(TSpcTypedInterp<TEventWatcher>* interp,
                                     u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TEMario* emario = (TEMario*)getNameRefPtr(interp->pop());
	emario->startGateDrawing();
	interp->push();
}

static void evStartEMarioDisappear(TSpcTypedInterp<TEventWatcher>* interp,
                                   u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TEMario* emario = (TEMario*)getNameRefPtr(interp->pop());
	emario->forceDisappear();
	interp->push();
}

static void evStartOpenModelGate(TSpcTypedInterp<TEventWatcher>* interp,
                                 u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TModelGate* gate = (TModelGate*)getNameRefPtr(interp->pop());
	gate->startOpen();
	interp->push();
}

static void evIsMapEventFinishedAll(TSpcTypedInterp<TEventWatcher>* interp,
                                    u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TMapEvent* event = (TMapEvent*)getNameRefPtr(interp->pop());
	interp->push(event->isFinishedAll());
}

static void evRaiseBuilding(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);

	int id = TSpcSlice(interp->pop()).getDataInt();

	TMapEventSinkShadowMario* event
	    = (TMapEventSinkShadowMario*)JDrama::TNameRefGen::search2(
	        "イベント（カゲマリオゲート）");

	if (event)
		event->raiseBuilding(id);

	interp->push();
}

static void evForceCloseTalk(TSpcTypedInterp<TEventWatcher>* interp,
                             u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);

	EventWatcherGetTalk2D()->forceCloseTalk();

	interp->push();
}

// TODO: 99.9%, instruction-exact at retail's 0xa0; the popped slices sit 4
// low and the pushed one 8 low. All 216 combinations of accessor, raw member,
// raw global and both director binders over the three sites, times the two
// nil-push spellings, stay at >=17 markers (the best, 0xa8, puts the pops in
// place and the push 4 low). Inert or worse: a switch, a named console,
// TSpcSlice pops, C-style declarations.
static void evInsertTimer(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);

	int p1 = interp->pop().getDataInt();
	int p2 = interp->pop().getDataInt();

	if (p2 == 1)
		EventWatcherGetMarDirector()->getConsole()->startAppearTimer(0, p1);
	else if (p2 == 2)
		SMSGetMarDirector()->getConsole()->startAppearTimer(1, p1);
	else
		SMSGetMarDirector()->getConsole()->startDisappearTimer();

	interp->push();
}

static void evStartTimer(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);

	int time = interp->pop().getDataInt();

	EventWatcherGetMarDirector()->startTimer();
	SMSGetMarDirector()->getConsole()->startMoveTimer(time);

	interp->push();
}

static void evStartMonteman(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);

	TEMario* monteMan = (TEMario*)JDrama::TNameRefGen::search2("モンテマン");

	int id = TSpcSlice(interp->pop()).getDataInt();
	if (monteMan)
		monteMan->startMonteReplay(id);

	interp->push();
}

static void evStopTimer(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	TGCConsole2* console = SMSGetMarDirector()->getConsole();
	console->stopMoveTimer();
	interp->push();
}

static void evMonteManReachFlag(TSpcTypedInterp<TEventWatcher>* interp,
                                u32 arg_num)
{
	int result = 0;

	interp->verifyArgNum(0, &arg_num);

	TEMario* monteMan = JDrama::TNameRefGen::search<TEMario>("モンテマン");
	if (monteMan->isGoal())
		result = 1;

	interp->push(result);
}

static void evGetTime(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	TGCConsole2* console = SMSGetMarDirector()->getConsole();
	interp->push(console->getFinishedTime());
}

static void evKillShine(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TShine* shine = (TShine*)getNameRefPtr(interp->pop());
	shine->kill();
	interp->push();
}

static void evKillMushroom1up(TSpcTypedInterp<TEventWatcher>* interp,
                              u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TMushroom1up* mushroom = (TMushroom1up*)getNameRefPtr(interp->pop());
	mushroom->kill();
	interp->push();
}

// Binder over the raw global, named again at the call site: retail's 0x88
// frame with every pool slot in place. The SMSGetMSound() binder is 4 high,
// and naming the popped mushroom as well moves the pop slices 4 high.
static inline MSound* EventWatcherRawMSound()
{
	MSound* sound = gpMSound;
	return sound;
}

static void evAppearMushroom1up(TSpcTypedInterp<TEventWatcher>* interp,
                                u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	((TMushroom1up*)getNameRefPtr(interp->pop()))->appear();
	MSound* sound = EventWatcherRawMSound();
	sound->startSoundSystemSE(MSD_SE_SY_1UP_APPEAR, 0, nullptr, 0);
	interp->push();
}

static void evAppearShineFromNPC(TSpcTypedInterp<TEventWatcher>* interp,
                                 u32 arg_num)
{
	interp->verifyArgNum(3, &arg_num);
	const char* demoName  = interp->pop().getDataString();
	TSpcSlice npcSlice    = interp->pop();
	const char* shineName = interp->pop().getDataString();
	TBaseNPC* npc         = (TBaseNPC*)getNameRefPtr(npcSlice);
	if (strcmp(demoName, "") != 0) {
		gpItemManager->makeShineAppearWithDemo(
		    shineName, demoName, npc->mPosition.x, npc->mPosition.y,
		    npc->mPosition.z);
	} else {
		TShine* shine = (TShine*)JDrama::TNameRefGen::search2(shineName);
		shine->mInitialPosition = npc->mPosition;
		shine->mPosition        = npc->mPosition;
		shine->appearWithTime(1200, -1, -1, -1);
	}
	interp->push();
}

static void evAppearShine(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);

	const char* demoName  = interp->pop().getDataString();
	const char* shineName = interp->pop().getDataString();

	if (strcmp(demoName, "") != 0) {
		gpItemManager->makeShineAppearWithDemoOffset(shineName, demoName, 0.0f,
		                                             0.0f, 0.0f);
	} else {
		TShine* shine = JDrama::TNameRefGen::search<TShine>(shineName);
		shine->appearWithTime(1200, -1, -1, -1);
	}
	interp->push();
}

static void
evAppearShineFromNPCWithoutDemo(TSpcTypedInterp<TEventWatcher>* interp,
                                u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	TSpcSlice npcSlice    = interp->pop();
	const char* shineName = interp->pop().getDataString();
	TBaseNPC* npc         = (TBaseNPC*)getNameRefPtr(npcSlice);

	TShine* shine = (TShine*)JDrama::TNameRefGen::search2(shineName);
	shine->mPosition.set(npc->mPosition);
	shine->makeObjAppeared();

	interp->push();
}

static void evAppearShineFromKageMario(TSpcTypedInterp<TEventWatcher>* interp,
                                       u32 arg_num)
{
	interp->verifyArgNum(3, &arg_num);

	int arg1         = interp->pop().getDataInt();
	const char* arg2 = interp->pop().getDataString();
	const char* arg3 = interp->pop().getDataString();

	THitActor* uuuh = (THitActor*)JDrama::TNameRefGen::search2(arg2);
	TShine* shine   = (TShine*)JDrama::TNameRefGen::search2(arg3);

	shine->mPosition = uuuh->mPosition;
	shine->appearSimple(arg1);

	interp->push();
}

static void evAppearShineForWoodBox(TSpcTypedInterp<TEventWatcher>* interp,
                                    u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);

	int index = interp->pop().getDataInt();

	if (index) // huh?
		index = 1;

	static const char* sShineViewObjName[] = {
		"木箱ゲーム用シャイン１",
		"木箱ゲーム用シャイン２",
	};

	gpItemManager->makeShineAppearWithDemo(sShineViewObjName[index],
	                                       "木箱ゲーム用シャインカメラ",
	                                       -4010.0f, 9850.0f, -4040.0f);

	interp->push();
}

// Binding level over a raw member read, worth +8 of low region in
// evChangeNozzle (batch 127).
static inline TMario* EventWatcherGetMarioOriginal()
{
	TMario* marioOriginal = gpMarioOriginal;
	return marioOriginal;
}

static void evChangeNozzle(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TWaterGun::TNozzleType id
	    = (TWaterGun::TNozzleType)interp->pop().getDataInt();
	if (id == TWaterGun::DivingHelmet)
		EventWatcherGetMarioOriginal()->setDivHelm();
	else
		gpMarioOriginal->mWaterGun->changeNozzle(id, true);
	interp->push();
}

static void evStartMarioTalking(TSpcTypedInterp<TEventWatcher>* interp,
                                u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	gpMarioOriginal->startTalking();
	interp->push();
}

static void evCheckWoodBox(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	int p1 = interp->pop().getDataInt();
	int p2 = interp->pop().getDataInt();

	// p1 is the upper bound (the loop below runs p2..p1), so the span is
	// p1 - p2 + 1; retail's `subf r5, r6, r29` has the operands this way
	// round and the other order was a sign error.
	int count = p1 - p2 + 1;

	char buffer[] = "ゲーム木箱00";
	for (int i = p2; i <= p1; ++i) {
		if (i < 10) {
			buffer[10] = '0' + i;
			buffer[11] = 0;
		} else {
			buffer[10] = '0' + i / 10;
			buffer[11] = '0' + i % 10;
		}
		TMapObjBase* obj = (TMapObjBase*)JDrama::TNameRefGen::search2(buffer);
		if (obj && obj->checkLiveFlag(LIVE_FLAG_DEAD))
			--count;
	}

	interp->push(count);
}

// TODO: the three wood-box builtins are each exactly 4 bytes short in the low
// region (every instruction matches). They differ from the matching builtins
// only in reaching the actor through `TNameRefGen::search<T>` instead of
// `getNameRefPtr`. Making `search<T>` delegate to the neighbouring `search2`
// -- one more inline level, which is the obvious reason for both to exist --
// was tried in JDRNameRefGen.hpp and is NOT viable: it changes a
// source-linked object and breaks the DOL SHA-1.
static void evRefreshWoodBox(TSpcTypedInterp<TEventWatcher>* interp,
                             u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	int p1 = interp->pop().getDataInt();
	int p2 = interp->pop().getDataInt();

	char buffer[] = "ゲーム木箱00";
	for (int i = p2; i <= p1; ++i) {
		if (i < 10) {
			buffer[10] = '0' + i;
			buffer[11] = 0;
		} else {
			buffer[10] = '0' + i / 10;
			buffer[11] = '0' + i % 10;
		}
		TMapObjBase* obj = (TMapObjBase*)JDrama::TNameRefGen::search2(buffer);
		if (obj)
			obj->appear();
	}

	interp->push();
}

static void evKillWoodBox(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	int p1 = interp->pop().getDataInt();
	int p2 = interp->pop().getDataInt();

	char buffer[] = "ゲーム木箱00";
	for (int i = p2; i <= p1; ++i) {
		if (i < 10) {
			buffer[10] = '0' + i;
			buffer[11] = 0;
		} else {
			buffer[10] = '0' + i / 10;
			buffer[11] = '0' + i % 10;
		}
		TMapObjBase* obj = (TMapObjBase*)JDrama::TNameRefGen::search2(buffer);
		if (obj)
			obj->makeObjDead();
	}

	interp->push();
}

static void evIsInsideCube(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int cubeId = interp->pop().getDataInt();
	// Declared ahead of `pos`: the named result takes the slot above it, which
	// puts `pos` and every temporary below at retail's offsets.
	int result;

	// TODO: getPos10cmAbove or something like that?
	JGeometry::TVec3<f32> pos = gpMarioOriginal->mPosition;
	pos.y += 10.0f;

	result = gpCubeArea->isInCube(pos, cubeId) ? 1 : 0;
	interp->push(result);
}

static void evSetMarioWaiting(TSpcTypedInterp<TEventWatcher>* interp,
                              u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	gpMarioOriginal->changePlayerStatus(MARIO_STATUS_WAIT, 0, true);
	interp->push();
}

static void evStartMareBottleDemo(TSpcTypedInterp<TEventWatcher>* interp,
                                  u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);

	TMapObjBase* obj = (TMapObjBase*)JDrama::TNameRefGen::search2("ＥＸビン");
	obj->getMActor()->setBck("exbottle_bottle_in");

	// The original keeps Mario in a register across both statements: the
	// store to mPosition would otherwise force a reload of the global
	// (measured: the bare global adds an `lwz gpMarioOriginal` and 8 bytes of
	// frame, 98.0% -> 96.9%).
	// TODO: 98.0%. Two instructions are swapped: retail keeps the position's
	// x word in r3 and only materialises `this` afterwards, with
	// `addi r3, r7, 0`, while we emit `mr r3, r7` before the stores. A
	// `TMario&` local instead of the pointer changes nothing.
	// Also inert: `mPosition.set(...)`, per-component copies, and
	// SMSGetMarioOriginal() at either site.
	TMario* mario    = gpMarioOriginal;
	mario->mPosition = obj->mPosition;
	mario->changePlayerStatus(MARIO_STATUS_BOTTLE_IN, 0, true);

	interp->push();
}

static void evIsFinishMareBottleDemo(TSpcTypedInterp<TEventWatcher>* interp,
                                     u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);

	TMapObjBase* obj = JDrama::TNameRefGen::search<TMapObjBase>("ＥＸビン");

	int result;
	if (obj->getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
		result = 1;
	else
		result = 0;

	interp->push(result);
}

static void evIsInsideFastCube(TSpcTypedInterp<TEventWatcher>* interp,
                               u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	int p1 = interp->pop().getDataInt();
	int p2 = interp->pop().getDataInt();

	// TODO: getPos10cmAbove or something like that?
	JGeometry::TVec3<f32> pos = gpMarioOriginal->mPosition;
	pos.y += 10.0f;

	int value;
	switch (p2) {
	case 0:
		value = gpCubeFastA->isInCube(pos, p1) ? 1 : 0;
		break;
	case 1:
		value = gpCubeFastB->isInCube(pos, p1) ? 1 : 0;
		break;
	case 2:
		value = gpCubeFastC->isInCube(pos, p1) ? 1 : 0;
		break;
	default:
		value = 0;
		break;
	}

	interp->push(value);
}

static void evSetTransScale(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(7, &arg_num);
	f32 tz = interp->pop().getDataFloat();
	f32 ty = interp->pop().getDataFloat();
	f32 tx = interp->pop().getDataFloat();
	f32 sz = interp->pop().getDataFloat();
	f32 sy = interp->pop().getDataFloat();
	f32 sx = interp->pop().getDataFloat();

	TMapObjBase* obj = (TMapObjBase*)getNameRefPtr(interp->pop());

	obj->makeObjAppeared();
	JGeometry::TVec3<f32> scale(sx, sy, sz);
	JGeometry::TVec3<f32> rot(0.0f, 0.0f, 0.0f);
	JGeometry::TVec3<f32> trans(tx, ty, tz);
	obj->changeObjSRT(scale, rot, trans);

	interp->push();
}

static void evSetEventID(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	// The id is narrowed to 16 bits here, not at the setEventId call: the
	// original truncates it before the second pop.
	u16 p1 = interp->pop().getDataInt();

	TMapObjBase* event = (TMapObjBase*)getNameRefPtr(interp->pop());
	event->setEventId(p1);
	interp->push();
}

static void evManiCoinDown(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	TGCConsole2* console = SMSGetMarDirector()->getConsole();
	console->startAppearStar();
	interp->push();
}

static void evStartBGM(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	MSBgm::startBGM(interp->pop().getDataInt());
	interp->push(TSpcSlice());
}

// A by-value u32 predicate over checkLiveFlag: +4 of pool, retail's slots.
// The raw flag test, checkLiveFlag() itself and a bool-returning fork are all
// 4 short; a named bool inside the fork is 8 long.
static inline u32 EventWatcherIsDead(TLiveActor* actor)
{
	return actor->checkLiveFlag(LIVE_FLAG_DEAD);
}

static void evEggYoshiStartFruit(TSpcTypedInterp<TEventWatcher>* interp,
                                 u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TEggYoshi* egg = (TEggYoshi*)getNameRefPtr(interp->pop());
	if (!EventWatcherIsDead(egg))
		egg->startFruit();
	interp->push();
}

static void evPutNozzle(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TMapObjGeneral* obj = (TMapObjGeneral*)getNameRefPtr(interp->pop());
	obj->put();
	interp->push();
}

static void evStopBGM(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	MSBgm::stopBGM(interp->pop().getDataInt(), 10);
	interp->push(TSpcSlice());
}

static void evStartSE(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	SMSGetMSound()->startSoundSystemSE(interp->pop().getDataInt(), 0, nullptr,
	                                   0);
	interp->push();
}

static void evStartEventSE(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int se;
	switch (interp->pop().getDataInt()) {
	case 0:
		se = 0x485A;
		break;
	case 1:
		se = 0x485B;
		break;
	}
	MSound* sound = EventWatcherRawMSound();
	sound->startSoundSystemSE(se, 0, nullptr, 0);
	// TODO: 99.9%, all 98 instructions matching at retail's 0x58 frame. The
	// named receiver through the raw-global binder (as in
	// evAppearMushroom1up) puts the pushed slice in place; the popped slice
	// and its fctiwz reads remain 4 high (0x3c vs 0x38). The SMSGetMSound()
	// binder, the plain accessor and the raw global, named or not, with the
	// switch on a named int or a u32 `se`, are all 4-8 further off.
	interp->push();
}

static void evStartMiss(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	gpMarioOriginal->loserExec();
	interp->push();
}

static void evChangeSunglass(TSpcTypedInterp<TEventWatcher>* interp,
                             u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int arg = interp->pop().getDataInt();
	TSunGlass* sunglass
	    = JDrama::TNameRefGen::search<TSunGlass>("サングラスフェーダ");
	if (!arg) {
		sunglass->startFade(2, true);
		gpMarioOriginal->wearGlass();
		if (TFlagManager::getInstance()->getShineFlag(0x77))
			gpMarioOriginal->onFlag(MARIO_FLAG_HAS_SHIRT);
	} else {
		sunglass->startFade(3, true);
		gpMarioOriginal->takeOffGlass();
		if (TFlagManager::getInstance()->getShineFlag(0x77))
			gpMarioOriginal->offFlag(MARIO_FLAG_HAS_SHIRT);
	}
	interp->push();
}

static void evSetCollision(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	int value = interp->pop().getDataInt();

	THitActor* hitActor = (THitActor*)getNameRefPtr(interp->pop());

	if (!value)
		hitActor->onHitFlag(HIT_FLAG_NO_COLLISION);
	else
		hitActor->offHitFlag(HIT_FLAG_NO_COLLISION);

	interp->push();
}

// TODO: 99.9%, every instruction matching; our frame is 0xe8 against retail's
// 0xe0, with the whole 8-byte excess below the conversion temporaries.
// Measured: naming the vector (`TVec3<f32> pos(x, y, z)`) leaves the frame at
// 0xe8, and popping into `f32` locals instead of `int` takes it to 0xd8 but
// rewrites 23 instructions.
static void evWarpMario(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(4, &arg_num);
	s32 arg1 = interp->pop().getDataInt();
	s32 z    = interp->pop().getDataInt();
	s32 y    = interp->pop().getDataInt();
	s32 x    = interp->pop().getDataInt();
	SMS_MarioWarpRequest(JGeometry::TVec3<f32>(x, y, z), arg1);
	interp->push();
}

// Binding level worth +8 of low region, landing evStartAppearJetBalloon's
// frame at 0xa8 (batch 124).
static inline TGCConsole2* EventWatcherGetConsole(TMarDirector* p)
{
	TGCConsole2* console = p->getConsole();
	return console;
}

static inline TGCConsole2* EventWatcherConsoleForJet()
{
	return SMSGetMarDirector()->getConsole();
}

static void evStartAppearJetBalloon(TSpcTypedInterp<TEventWatcher>* interp,
                                    u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);

	int p1 = interp->pop().getDataInt();
	int p2 = interp->pop().getDataInt();

	switch (p2) {
	case 0:
		if (p1 == 1)
			EventWatcherGetConsole(SMSGetMarDirector())
			    ->startAppearJetBalloon(0, 8);
		break;

	case 1:
		if (p1 == 1)
			EventWatcherConsoleForJet()->startAppearJetBalloon(1, 10);
		break;

	case 2:
		if (p1 == 1)
			SMSGetMarDirector()->getConsole()->startAppearRedCoin();
		break;
	}

	interp->push();
}

static void evSetEventForWaterMelon(TSpcTypedInterp<TEventWatcher>* interp,
                                    u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TBigWatermelon* melon = (TBigWatermelon*)getNameRefPtr(interp->pop());
	melon->startEvent();
	interp->push();
}

static void evAppearReadyGo(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	EventWatcherGetConsole(EventWatcherMarDirector())->unk94->startAppearReady();
	interp->push();
}

static void evAppear8RedCoinsAndTimer(TSpcTypedInterp<TEventWatcher>* interp,
                                      u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);

	// The name has to be its own local. `search<T>` uses it twice, and with
	// the literal written inline MWCC spends a callee-saved register on a
	// .rodata base pointer and addresses both the name and the stack-overflow
	// string through it, which renumbers every register in the function
	// (92.2% -> 99.9%).
	// TODO: all 122 instructions match and the frame is now retail's 0xa0
	// (the director binder below at all three sites is worth the whole 24
	// bytes; at one or two sites it is only 8 each). The one residue is the
	// nil-push slice temporary: retail puts it at 0x54, `interp->push()`
	// puts it at 0x4c and `interp->push(TSpcSlice())` at 0x60, and nothing
	// on the caller side lands the 8 in between -- see the note above
	// evGameOver.
	// Ruled out (no frame change): a named `f32` for the timer seconds, a
	// named `MtxPtr` for the animation matrix, a named `int` for the
	// per-coin kill timer; a `getConsole()` binder overshoots to 0xb0.
	const char* switchName = "赤コイン用スイッチ";
	TRedCoinSwitch* swtch
	    = JDrama::TNameRefGen::search<TRedCoinSwitch>(switchName);

	int iVar9 = swtch->unk138;
	for (int i = 0; i < 8; ++i) {
		TCoinRed* coin = (TCoinRed*)gpItemManager->makeObjAppeared(0x2000000f);
		coin->killByTimer(iVar9 - coin->unk150);
		coin->unk158.set(coin->mPosition.x, coin->mPosition.y + 70.0f,
		                 coin->mPosition.z);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    MAPOBJ_MS_WATCOIN_KIRA, coin->getModel()->getAnmMtx(0), 0, coin);
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_DISAP_A_W,
		                             &coin->getUnk158(), 0, nullptr);
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_DISAP_B,
		                             &coin->getUnk158(), 0, nullptr);
	}
	EventWatcherGetMarDirector()->getConsole()->startAppearTimer(
	    1, iVar9 * 0.008333334f);
	EventWatcherGetMarDirector()->startTimer();
	EventWatcherGetMarDirector()->getConsole()->startMoveTimer(10);
	interp->push();
}

// fabricated: mario.MAP keeps no symbol for this helper, so the name is a
// guess, but the signature is read off evWarpFrontToMario. It rotates its
// argument in place through a `Vec&` -- retail builds the vector once and
// rewrites `x` and `z` in it, with no copy for the parameter -- and returns a
// `TVec3<f32>`, so that the `return` converts through `TVec3<f32>(const Vec&)`
// and leaves the `bl JGeometry::TVec3<float>::set(const Vec&)` retail calls
// here. `set(const Vec&)` only wins over the exact `set<TY>(const TVec3<TY>&)`
// when the source really is a `Vec`, which is what fixes the parameter type.
//
// `vec.x` is read into `x` before the first assignment: the store to `vec.x`
// invalidates every cached load of the struct, which is why retail reloads
// `vec.z` and both table entries for the second component but keeps the
// pre-store `-vec.x` in a register.
//
// Trials, all measured on evWarpFrontToMario:
//   Vec& param, TVec3<f32> return, bare gpMarioPos      99.8%  <- kept
//   Vec& param, TVec3<f32> return, SMS_GetMarioPos()    99.7%  (frame +8)
//   Vec by value, TVec3<f32> return                     86.0%  (argument copy)
//   Vec& param, const Vec& return                       76.2%
//   TVec3<f32> by value, Vec return                     78.5%
//   TVec3<f32> by value, const Vec& return              84.6%  (set inlined)
//   TVec3<f32> by value, TVec3<f32> return (original)   84.1%
static JGeometry::TVec3<f32> rotateY(Vec& vec, s16 angleY)
{
	f32 x = vec.x;
	vec.x = x * JMASCos(angleY) + vec.z * JMASSin(angleY);
	vec.z = -x * JMASSin(angleY) + vec.z * JMASCos(angleY);
	return vec;
}

static void evWarpFrontToMario(TSpcTypedInterp<TEventWatcher>* interp,
                               u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TLiveActor* actor = (TLiveActor*)(u32)interp->pop().getDataInt();

	s16 angleY = SMS_GetMarioAngleY();

	// TODO: every instruction and the frame size match; the only residue is
	// the position of operator+'s by-value `fst` temporary, which retail
	// allocates at the bottom of the low region (0x44) while we place it
	// third (0x74). Low-region temporaries are laid out in expansion order,
	// so retail expanded operator+ before the pop and the push.
	JGeometry::TVec3<f32> front(0.0f, 0.0f, 400.0f);
	actor->mPosition   = *gpMarioPos + rotateY(front, SMS_GetMarioAngleY());
	actor->mRotation.y = SHORTANGLE2DEG((s16)(angleY - 0x8000));

	interp->push();
}

static void evOnNeutralMarioKey(TSpcTypedInterp<TEventWatcher>* interp,
                                u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	SMSGetMarDirector()->getGamePad()->onNeutralMarioKey();
	interp->push(TSpcSlice());
}

// TODO: the pushed slice sits at 0x34 against retail's 0x38. Through the
// EventWatcherMarDirector accessor binder the slice and fctiwz slots land but
// the popped slice is 4 high (0x4c); push(TSpcSlice()) with SMSGetMarDirector
// lands the slice at frame 0x60; named pad/director locals move everything.
static void evInvalidatePad(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int frames = interp->pop().getDataInt();

	EventWatcherGetMarDirector()->getGamePad()->mDisabledFrames = frames;

	interp->push();
}

static void evIsWaterMelonIsReached(TSpcTypedInterp<TEventWatcher>* interp,
                                    u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TBigWatermelon* melon = (TBigWatermelon*)(u32)interp->pop().getDataInt();

	// The zero y component is real: retail starts the sum of squares from a
	// 0.0f literal (`fmadds f1, f3, f3, f1` with f1 loaded from the float
	// pool), which is what `squared()`'s y term folds to. Spelling the test
	// as `dx * dx + dz * dz` instead gives an `fmuls` and a 0x48 frame
	// against retail's 0x68.
	// Naming the two differences before building `diff` gives retail's float
	// registers.
	// TODO: 100% of instructions; the pushed slice sits 8 bytes high (0x34
	// vs retail's 0x2c). Inert: `result` declared after `diff`, a named
	// dist, `dot(diff)`, a named slice, the pop through an int local.
	int result = 0;
	f32 dx = -4660.0f - melon->mPosition.x;
	f32 dz = 12000.0f - melon->mPosition.z;
	JGeometry::TVec3<f32> diff(dx, 0.0f, dz);
	if (diff.squared() <= 90000.0f)
		result = 1;

	interp->push(result);
}

// TODO: 99.9%, every instruction matching, slice 4 high (0x18 vs 0x14) at
// retail's 0x28 frame. A TU-local push wrapper refuses TSpcStack::push
// (`bl`); raw gpMSound is -8 of frame and slice. The raw-global binder
// named as in evAppearMushroom1up lands the slice at 0x18 but the frame at
// 0x30; chained it is 0x30/0x1c. Inert: push(TSpcSlice()), push(0), direct
// and reference-returning forks, gateCheck spelled out, a named SE id.
static void evStartMontemanBGM(TSpcTypedInterp<TEventWatcher>* interp,
                               u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);

	MSBgm::stopTrackBGM(0, 10);
	MSBgm::startBGM(MSD_BGM_MONTEMAN_RACE);
	SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_RACE_START, 0, nullptr, 0);

	interp->push();
}

static void evStartMontemanFanfare(TSpcTypedInterp<TEventWatcher>* interp,
                                   u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);

	MSBgm::startBGM(MSD_BGM_FANFARE_RACE);

	interp->push();
}

template <> void TSpcTypedBinary<TEventWatcher>::initUserBuiltin()
{
	// clang-format off
  bindSystemDataToSymbol("getSystemFlag", (u32)&evGetSystemFlag);
  bindSystemDataToSymbol("setSystemFlag", (u32)&evSetSystemFlag);
  bindSystemDataToSymbol("getNameRefHandle", (u32)&evGetNameRefHandle);
  bindSystemDataToSymbol("getNameRefName", (u32)&evGetNameRefName);
  bindSystemDataToSymbol("getNPCType", (u32)&evGetNPCType);
  bindSystemDataToSymbol("setFlagNPCDontTalk", (u32)&evSetFlagNPCDontTalk);
  bindSystemDataToSymbol("setFlagNPCDontThrow", (u32)&evSetFlagNPCDontThrow);
  bindSystemDataToSymbol("setFlagNPCDead", (u32)&evSetFlagNPCDead);
  bindSystemDataToSymbol("isNearSameActors", (u32)&evIsNearSameActors);
  bindSystemDataToSymbol("isNearActors", (u32)&evIsNearActors);
  bindSystemDataToSymbol("getTalkNPC", (u32)&evGetTalkNPC);
  bindSystemDataToSymbol("getTalkNPCName", (u32)&evGetTalkNPCName);
  bindSystemDataToSymbol("setTalkMsgID", (u32)&evSetTalkMsgID);
  bindSystemDataToSymbol("getTalkMode", (u32)&evGetTalkMode);
  bindSystemDataToSymbol("getTalkSelectedValue", (u32)&evGetTalkSelectedValue);
  bindSystemDataToSymbol("setValue2TalkVariable", (u32)&evSetValue2TalkVariable);
  bindSystemDataToSymbol("isTalkModeNow", (u32)&evIsTalkModeNow);
  bindSystemDataToSymbol("setFlagNPCCanTaken", (u32)&evSetFlagNPCCanTaken);
  bindSystemDataToSymbol("pushNerve4LiveActor", (u32)&evPushNerve4LiveActor);
  bindSystemDataToSymbol("isOnLiveActorFlag", (u32)&evIsOnLiveActorFlag);
  bindSystemDataToSymbol("setHide4LiveActor", (u32)&evSetHide4LiveActor);
  bindSystemDataToSymbol("setDead4LiveActor", (u32)&evSetDead4LiveActor);
  bindSystemDataToSymbol("setTimeLimit", (u32)&evSetTimeLimit);
  bindSystemDataToSymbol("setAttentionTime", (u32)&evSetAttentionTime);
  bindSystemDataToSymbol("setPollutionIncreaseCount", (u32)&evSetPollutionIncreaseCount);
  bindSystemDataToSymbol("getRestTime", (u32)&evGetRestTime);
  bindSystemDataToSymbol("getPollutionLevel", (u32)&evGetPollutionLevel);
  bindSystemDataToSymbol("setNextStage", (u32)&evSetNextStage);
  bindSystemDataToSymbol("registerMovie", (u32)&evRegisterMovie);
  bindSystemDataToSymbol("gameOver", (u32)&evGameOver);
  bindSystemDataToSymbol("isGraffitoCoverage0", (u32)&evIsGraffitoCoverage0);
  bindSystemDataToSymbol("setGraffitoMultiplied", (u32)&evSetGraffitoMultiplied);
  bindSystemDataToSymbol("isBossDefeated", (u32)&evIsBossDefeated);
  bindSystemDataToSymbol("launchEventClearDemo", (u32)&evLaunchEventClearDemo);
  bindSystemDataToSymbol("isEMarioReachedToGoal", (u32)&evIsEMarioReachedToGoal);
  bindSystemDataToSymbol("isEMarioDownWaitingToTalk", (u32)&evIsEMarioDownWaitingToTalk);
  bindSystemDataToSymbol("startEMarioRunAway", (u32)&evStartEMarioRunAway);
  bindSystemDataToSymbol("startEMarioGateDrawing", (u32)&evStartEMarioGateDrawing);
  bindSystemDataToSymbol("startEMarioDisappear", (u32)&evStartEMarioDisappear);
  bindSystemDataToSymbol("startOpenModelGate", (u32)&evStartOpenModelGate);
  bindSystemDataToSymbol("isMapEventFinishedAll", (u32)&evIsMapEventFinishedAll);
  bindSystemDataToSymbol("raiseBuilding", (u32)&evRaiseBuilding);
  bindSystemDataToSymbol("forceCloseTalk", (u32)&evForceCloseTalk);
  bindSystemDataToSymbol("insertTimer", (u32)&evInsertTimer);
  bindSystemDataToSymbol("startTimer", (u32)&evStartTimer);
  bindSystemDataToSymbol("startMonteman", (u32)&evStartMonteman);
  bindSystemDataToSymbol("stopTimer", (u32)&evStopTimer);
  bindSystemDataToSymbol("monteManReachFlag", (u32)&evMonteManReachFlag);
  bindSystemDataToSymbol("getTime", (u32)&evGetTime);
  bindSystemDataToSymbol("killShine", (u32)&evKillShine);
  bindSystemDataToSymbol("killMushroom1up", (u32)&evKillMushroom1up);
  bindSystemDataToSymbol("appearMushroom1up", (u32)&evAppearMushroom1up);
  bindSystemDataToSymbol("appearShineFromNPC", (u32)&evAppearShineFromNPC);
  bindSystemDataToSymbol("appearShineFromNPCWithoutDemo", (u32)&evAppearShineFromNPCWithoutDemo);
  bindSystemDataToSymbol("appearShineFromKageMario", (u32)&evAppearShineFromKageMario);
  bindSystemDataToSymbol("appearShine", (u32)&evAppearShine);
  bindSystemDataToSymbol("appearShineForWoodBox", (u32)&evAppearShineForWoodBox);
  bindSystemDataToSymbol("changeNozzle", (u32)&evChangeNozzle);
  bindSystemDataToSymbol("startMarioTalking", (u32)&evStartMarioTalking);
  bindSystemDataToSymbol("isInsideCube", (u32)&evIsInsideCube);
  bindSystemDataToSymbol("setMarioWaiting", (u32)&evSetMarioWaiting);
  bindSystemDataToSymbol("setTransScale", (u32)&evSetTransScale);
  bindSystemDataToSymbol("setEventID", (u32)&evSetEventID);
  bindSystemDataToSymbol("startBGM", (u32)&evStartBGM);
  bindSystemDataToSymbol("stopBGM", (u32)&evStopBGM);
  bindSystemDataToSymbol("startMiss", (u32)&evStartMiss);
  bindSystemDataToSymbol("startSE", (u32)&evStartSE);
  bindSystemDataToSymbol("startEventSE", (u32)&evStartEventSE);
  bindSystemDataToSymbol("changeSunglass", (u32)&evChangeSunglass);
  bindSystemDataToSymbol("setCollision", (u32)&evSetCollision);
  bindSystemDataToSymbol("warpMario", (u32)&evWarpMario);
  bindSystemDataToSymbol("startAppearJetBalloon", (u32)&evStartAppearJetBalloon);
  bindSystemDataToSymbol("appear8RedCoinsAndTimer", (u32)&evAppear8RedCoinsAndTimer);
  bindSystemDataToSymbol("warpFrontToMario", (u32)&evWarpFrontToMario);
  bindSystemDataToSymbol("appearReadyGo", (u32)&evAppearReadyGo);
  bindSystemDataToSymbol("onNeutralMarioKey", (u32)&evOnNeutralMarioKey);
  bindSystemDataToSymbol("invalidatePad", (u32)&evInvalidatePad);
  bindSystemDataToSymbol("startMontemanBGM", (u32)&evStartMontemanBGM);
  bindSystemDataToSymbol("startMontemanFanfare", (u32)&evStartMontemanFanfare);
  bindSystemDataToSymbol("checkWoodBox", (u32)&evCheckWoodBox);
  bindSystemDataToSymbol("refreshWoodBox", (u32)&evRefreshWoodBox);
  bindSystemDataToSymbol("killWoodBox", (u32)&evKillWoodBox);
  bindSystemDataToSymbol("maniCoinFallDown", (u32)&evManiCoinDown);
  bindSystemDataToSymbol("eggYoshiStartFruit", (u32)&evEggYoshiStartFruit);
  bindSystemDataToSymbol("putNozzle", (u32)&evPutNozzle);
  bindSystemDataToSymbol("startMareBottleDemo", (u32)&evStartMareBottleDemo);
  bindSystemDataToSymbol("isFinishMareBottleDemo", (u32)&evIsFinishMareBottleDemo);
  bindSystemDataToSymbol("isInsideFastCube", (u32)&evIsInsideFastCube);
  bindSystemDataToSymbol("setEventForWaterMelon", (u32)&evSetEventForWaterMelon);
  bindSystemDataToSymbol("isWaterMelonIsReached", (u32)&evIsWaterMelonIsReached);
	// clang-format on
	TNpcEvent::initNpcBuiltin(this);
}

TEventWatcher::TEventWatcher(const char* name)
    : JDrama::TViewObj(name)
    , mBinary(nullptr)
    , mInterp(nullptr)
{
}

TEventWatcher::TEventWatcher(const char* name, const char* script)
    : JDrama::TViewObj(name)
    , mBinary(nullptr)
    , mInterp(nullptr)
{
	launchScript(script);
}

void TEventWatcher::launchScript(const char* script)
{
	if (void* res = JKRGetResource(script)) {
		mBinary = new TSpcTypedBinary<TEventWatcher>(res);
		mBinary->init();
		mInterp = new TSpcTypedInterp<TEventWatcher>(mBinary, this, 0x20, 0x20,
		                                             0x20, 0x20);
	}
}

void TEventWatcher::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if ((cue & CUE_MOVE) && mInterp)
		mInterp->update();
}
