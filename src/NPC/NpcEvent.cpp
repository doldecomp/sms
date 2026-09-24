#include <NPC/NpcEvent.hpp>
#include <stdio.h>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <System/MarDirector.hpp>
#include <System/FlagManager.hpp>
#include <Strategic/Spine.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjHide.hpp>
#include <MSound/MSound.hpp>
#include <NPC/NpcBase.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Mario.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

extern const TNerveBase<TLiveActor>* NerveGetByIndex(int index);

int TNpcEvent::mDownSunflowerNum = 0;

// TODO: header item, measured (zero regressions tree-wide, DOL unchanged):
// include/Strategic/spcinterp.hpp's `push(int v)` must forward through the
// slice overload -- `void push(int v) { push(TSpcSlice(v)); }` instead of
// `mProcessStack.push(TSpcSlice(v))`. That extra level puts
// TSpcStack<TSpcSlice>::push at depth 4 for a builtin that reaches it through
// one of the two static helpers below, where the allowance is 2 and its body
// is five statements, so it becomes the `bl` the map wants: the missing
// `push__21TSpcStack<9TSpcSlice>FRC9TSpcSlice` (weak, 0x68, emitted between
// evIsNpcSinkBottom and evCheckLatestNerve4Npc) appears at 100%, symbol order
// goes from FAIL to PASS, evIsNpcSinkBottom 72.2 -> 100, evCheckCurNerve4Npc
// 72.6 -> 93.0, evCheckLatestNerve4Npc 71.1 -> 87.3. Builtins that push
// directly keep it at depth 3, where five statements still inline, so nothing
// in EventWatcher or Strategic moves.
static void IsNpcFlagOn_(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num,
                         u32 flag)
{
	interp->verifyArgNum(1, &arg_num);
	TBaseNPC* npc = (TBaseNPC*)(u32)interp->pop().getDataInt();
	int result    = 0;
	if (npc->checkLiveFlag(flag))
		result = 1;
	interp->push(result);
}

static inline TSpineBase<TLiveActor>* NpcEventGetSpine(const TBaseNPC* npc)
{
	TSpineBase<TLiveActor>* spine = npc->getSpine();
	return spine;
}

static inline const TNerveBase<TLiveActor>*
NpcEventGetLatestNerve(const TBaseNPC* npc)
{
	const TLiveActor* actor             = npc;
	const TNerveBase<TLiveActor>* nerve = actor->getLatestNerve();
	return nerve;
}

// getSpine binder (+0x10) lands evCheckCurNerve4Npc's frame at 0xa0;
// TLiveActor* upcast + named getLatestNerve (+0x10) lands
// evCheckLatestNerve4Npc at 0x98. Both then 99.96% / frame exact; leftover
// is arg_num 4 low and the pushed slice 8-12 low. NerveGetByIndex binder
// also lands the frame but drops the r28 save (ladder 353).
// 2026-09-22: pop levels for nerveId/npc, `push(TSpcSlice(result))` and
// `s32` locals all push this helper out of line in its callers (47-75%).
static void CheckNerve4Npc_(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num,
                            bool param_3)
{
	interp->verifyArgNum(2, &arg_num);
	int nerveId   = interp->pop().getDataInt();
	TBaseNPC* npc = (TBaseNPC*)(u32)interp->pop().getDataInt();
	int result = 0;

	const TNerveBase<TLiveActor>* expected = NerveGetByIndex(nerveId);
	const TNerveBase<TLiveActor>* actual   = param_3
	                                             ? NpcEventGetLatestNerve(npc)
	                                             : NpcEventGetSpine(npc)->getCurrentNerve();

	if (actual == expected)
		result = 1;
	interp->push(result);
}

// Pushing the search result straight (no named viewObj pointer) puts both
// slices at retail's slots; the named pointer was 4 bytes of low region
// short, a pop-string level +4 above, a direct-return search level +8.
static void evGetAddressFromViewObjName(TSpcTypedInterp<TEventWatcher>* interp,
                                        u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	const char* name = interp->pop().getDataString();
	interp->push((int)JDrama::TNameRefGen::search<JDrama::TViewObj>(name));
}

static void evCheckCurNerve4Npc(TSpcTypedInterp<TEventWatcher>* interp,
                                u32 arg_num)
{
	CheckNerve4Npc_(interp, arg_num, false);
}

static void evCheckLatestNerve4Npc(TSpcTypedInterp<TEventWatcher>* interp,
                                   u32 arg_num)
{
	CheckNerve4Npc_(interp, arg_num, true);
}

static void evIsNpcSinkBottom(TSpcTypedInterp<TEventWatcher>* interp,
                              u32 arg_num)
{
	IsNpcFlagOn_(interp, arg_num, TBaseNPC::LIVE_FLAG_SINK_BOTTOM);
}

// Binding level over a raw member read, worth +16 of low region in
// evIsGameModeNormal (batch 127).
static inline TMarDirector* NpcEventGetMarDirector()
{
	TMarDirector* marDirector = gpMarDirector;
	return marDirector;
}

static void evIsGameModeNormal(TSpcTypedInterp<TEventWatcher>* interp,
                               u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	int result = 0;
	if (NpcEventGetMarDirector()->unk124 == 0)
		result = 1;
	interp->push(TSpcSlice(result));
}

// TODO: 99.9%, frame exact (0x90) once all three gpMarDirector reads go
// through the TU-local binder (+32 over the raw global; SMSGetMarDirector() at
// one of the three is -8). Both slice slots are still 4 bytes high (push 0x4c
// vs 0x48, pop 0x70 vs 0x6c) against an exact stfd slot, i.e. one -4 of low
// region below the whole block. Naming the popped NPC is +8.
// 2026-09-22: SMSGetMarDirector()/raw gpMarDirector for the unk126 store
// lands the pushed slice but leaves the pop 4 low and the frame 8 short;
// pop levels (int/s32/pointer, direct or named) reorder the two slices;
// `(int)interp->pop()` and a named director local break instructions.
static void ev__ForceStartTalk(TSpcTypedInterp<TEventWatcher>* interp,
                               u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);

	int result = 0;

	if (!NpcEventGetMarDirector()->isTalkOrDemoModeNow() && SMS_IsMarioTouchGround4cm()
	    && !gpMarioOriginal->checkStatusType(MARIO_STATUS_FLAG_JUMPING)) {

		NpcEventGetMarDirector()->unkA0  = (TBaseNPC*)(u32)interp->pop().getDataInt();
		NpcEventGetMarDirector()->unk126 = 1;

		result = 1;
	} else {
		interp->pop();
	}

	interp->push(result);
}

// TODO: 92.8%. The frame is exact (0x90) since the three gpMarDirector reads
// went through the TU-local binder, and so is every slot but the discarded
// pop. The ROM copies only the *second* word of the popped slice
// (`addi r0, r3, 4; lwzx r0, r4, r0; stw r0, 0x70(r1)`) and then re-stores it
// into a second 4-byte slot at 0x7c that nothing ever reads, i.e. it binds the
// slice's data word to a named local it never uses. Every spelling that reads
// only `.mData` (`.mData.asInt`, `.mData.asString`, `getDataString()`) is
// dead-stripped by MWCC and loses both stores; binding the whole slice
// (`TSpcSlice exceptNpc = interp->pop();`) keeps them but copies both words.
// Open.
// 2026-09-22: `TSpcSlice exceptNpc; exceptNpc.mData = interp->pop().mData;`
// copies only the data word from the pop (93.5%) but keeps the default
// ctor's two zero stores and is +8 of frame; copy-init/assignment/ctor
// forms from the popped int or string are 80-91%.
// 2026-09-23: a reference bound to the pop, `TSpcSlice x(interp->pop())`,
// and a copy of the data union (union named in the header for the probe)
// are all dead-stripped whole (87.7%). Retail's pop temporary is at 0x6c
// (as in ev__ForceStartTalk, where ours is also 4 high), so both residues
// likely share one cause in the fabricated TSpcStack::pop.
static void ev__ForceStartTalkExceptNpc(TSpcTypedInterp<TEventWatcher>* interp,
                                        u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int result = 0;
	(void)interp->pop();

	if (!NpcEventGetMarDirector()->isTalkOrDemoModeNow() && SMS_IsMarioTouchGround4cm()
	    && !gpMarioOriginal->checkStatusType(MARIO_STATUS_FLAG_JUMPING)) {

		TBaseNPC* dummyNpc
		    = JDrama::TNameRefGen::search<TBaseNPC>("ダミーＮＰＣ");

		if (dummyNpc) {
			NpcEventGetMarDirector()->unkA0  = dummyNpc;
			NpcEventGetMarDirector()->unk126 = 1;

			result = 1;
		}
	}
	interp->push(result);
}

static void evConnectDummyNpc(TSpcTypedInterp<TEventWatcher>* interp,
                              u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);

	int result = 0;

	TBaseNPC* dummyNpc = (TBaseNPC*)JDrama::TNameRefGen::search2("ダミーＮＰＣ");
	if (dummyNpc != nullptr) {
		const JDrama::TActor* actor
		    = (const JDrama::TActor*)(u32)interp->pop().getDataInt();
		dummyNpc->setDummyConnectActor(actor);
		result = 1;
	} else {
		interp->pop();
	}

	interp->push(result);
}

static void evOnTalkToDummyNpc(TSpcTypedInterp<TEventWatcher>* interp,
                               u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	TBaseNPC* dummyNpc = (TBaseNPC*)JDrama::TNameRefGen::search2("ダミーＮＰＣ");
	if (dummyNpc != nullptr) {
		dummyNpc->offLiveFlag(LIVE_FLAG_DEAD);
		dummyNpc->offLiveFlag(LIVE_FLAG_UNK40000);
		dummyNpc->mTalkForbidCount = 120;
	}

	interp->push();
}

static void evSetNpcBalloonMessage(TSpcTypedInterp<TEventWatcher>* interp,
                                   u32 arg_num)
{
	interp->verifyArgNum(3, &arg_num);
	int fVar1     = interp->pop().getDataInt();
	int fVar2     = interp->pop().getDataInt();
	TBaseNPC* npc = (TBaseNPC*)(u32)interp->pop().getDataInt();
	npc->setBalloonMessage(fVar2, fVar1);
	interp->push();
}

static void evSetNpcTalkForbidCount(TSpcTypedInterp<TEventWatcher>* interp,
                                    u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	u16 count             = interp->pop().getDataInt();
	TBaseNPC* npc         = (TBaseNPC*)(u32)interp->pop().getDataInt();
	npc->mTalkForbidCount = count;
	interp->push();
}

static void evNpcDanceOn(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TBaseNPC* npc = (TBaseNPC*)(u32)interp->pop().getDataInt();
	npc->npcDanceIn();
	interp->push();
}

static void evNpcDanceOffHappyOn(TSpcTypedInterp<TEventWatcher>* interp,
                                 u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TBaseNPC* npc = (TBaseNPC*)(u32)interp->pop().getDataInt();
	npc->offActionFlag(TBaseNPC::NPC_ACTION_DANCE);
	npc->npcHappyIn(2);
	interp->push();
}

static void evResetFruitNum(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TFruitBasketEvent* basket = (TFruitBasketEvent*)(u32)interp->pop().getDataInt();
	basket->reset();
	interp->push();
}

static void evGetFruitNum(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	// `s32` (signed long), not `int`: an `int` local of the popped value
	// costs a 4-byte named slot below the float-to-int conversion buffer (+8
	// of frame); `u32` also lands it.
	s32 fVar4                 = interp->pop().getDataInt();
	TFruitBasketEvent* basket = (TFruitBasketEvent*)(u32)interp->pop().getDataInt();

	int iVar3 = 0;
	switch (fVar4) {
	case 0:
		iVar3 = 0;
		break;

	case 1:
		iVar3 = 4;
		break;

	case 2:
		iVar3 = 3;
		break;

	case 3:
		iVar3 = 1;
		break;
	}

	int num = basket->getFruitNum(iVar3);
	interp->push(num);
}

static void evSetFruitType(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(3, &arg_num);
	s32 fVar5                 = interp->pop().getDataInt();
	s32 fVar4                 = interp->pop().getDataInt();
	TFruitBasketEvent* basket = (TFruitBasketEvent*)(u32)interp->pop().getDataInt();

	if (fVar5 != 0) {
		int r28;
		switch (fVar4) {
		case 0:
			r28 = 0x40000394;
			break;

		case 1:
			r28 = 0x40000390;
			break;

		case 2:
			r28 = 0x40000392;
			break;

		case 3:
			r28 = 0x40000393;
			break;
		}
		basket->unk150 = r28;
	} else {
		basket->unk150 = 0;
	}
	interp->push();
}

static void evFireStartDemoCamera(TSpcTypedInterp<TEventWatcher>* interp,
                                  u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	const char* cameraName = interp->pop().getDataString();
	SMSGetMarDirector()->fireStartDemoCamera(cameraName, nullptr, -1, 0.0f,
	                                         true, nullptr, 0, nullptr,
	                                         JDrama::TFlagT<u16>());
	interp->push();
}

static void evIsDemoMode(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	int result = 0;
	if (SMSGetMarDirector()->isDemoModeNow())
		result = 1;
	interp->push(TSpcSlice(result));
}

// Binding level worth +8 of low region, landing evCheckMonteClear's frame at
// 0x90 (batch 124).
static inline bool NpcEventIsClean(const TBaseNPC* p)
{
	bool clean = p->isClean();
	return clean;
}

// TODO: 99.96%, frame exact (0x90) and the char[32] buffer now at 0x54: the
// `npc` pointer declared before it and assigned after is the +4 of named
// region above the buffer (`int b` declared there instead does nothing -- it
// stays in a register). The pushed slice is still 4 low (0x3c vs 0x40) and no
// lever moves it alone: `push(TSpcSlice(b))` is +4 without the pointer
// declaration but +8 with it, a named `TSpcSlice slice(b)` moves the popped
// slice instead, and dropping the NpcEventIsClean binder is -8 of frame.
// 2026-09-22: the if/else as a two-return TU-local predicate returning int
// (`push(TSpcSlice(pred(npc)))`, no npc pre-declaration, raw isClean())
// lands frame, buffer and pushed slice but leaves the popped slice 4 low;
// `isClean() != 0`, u32/bool flag wrappers, a pop level and a search level
// all move the three blocks together.
static void evCheckMonteClear(TSpcTypedInterp<TEventWatcher>* interp,
                              u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int fVar1 = interp->pop().getDataInt();

	TBaseNPC* npc;
	char buffer[32];
	snprintf(buffer, 32, "モンテ%d", fVar1);
	npc = JDrama::TNameRefGen::search<TBaseNPC>(buffer);

	int b;
	if (!npc->checkLiveFlag(LIVE_FLAG_UNK400000) && NpcEventIsClean(npc))
		b = true;
	else
		b = false;

	interp->push(b);
}

void TNpcEvent::initNpcBuiltin(TSpcTypedBinary<TEventWatcher>* param_1)
{
	// clang-format off
  param_1->bindSystemDataToSymbol("getAddressFromViewObjName", (u32)&evGetAddressFromViewObjName);
  param_1->bindSystemDataToSymbol("checkCurNerve4Npc", (u32)&evCheckCurNerve4Npc);
  param_1->bindSystemDataToSymbol("checkLatestNerve4Npc", (u32)&evCheckLatestNerve4Npc);
  param_1->bindSystemDataToSymbol("isNpcSinkBottom", (u32)&evIsNpcSinkBottom);
  param_1->bindSystemDataToSymbol("isGameModeNormal", (u32)&evIsGameModeNormal);
  param_1->bindSystemDataToSymbol("__forceStartTalk", (u32)&ev__ForceStartTalk);
  param_1->bindSystemDataToSymbol("__forceStartTalkExceptNpc", (u32)&ev__ForceStartTalkExceptNpc);
  param_1->bindSystemDataToSymbol("connectDummyNpc", (u32)&evConnectDummyNpc);
  param_1->bindSystemDataToSymbol("onTalkToDummyNpc", (u32)&evOnTalkToDummyNpc);
  param_1->bindSystemDataToSymbol("setNpcBalloonMessage", (u32)&evSetNpcBalloonMessage);
  param_1->bindSystemDataToSymbol("setNpcTalkForbidCount", (u32)&evSetNpcTalkForbidCount);
  param_1->bindSystemDataToSymbol("npcDanceOn", (u32)&evNpcDanceOn);
  param_1->bindSystemDataToSymbol("npcDanceOffHappyOn", (u32)&evNpcDanceOffHappyOn);
  param_1->bindSystemDataToSymbol("resetFruitNum", (u32)&evResetFruitNum);
  param_1->bindSystemDataToSymbol("getFruitNum", (u32)&evGetFruitNum);
  param_1->bindSystemDataToSymbol("setFruitType", (u32)&evSetFruitType);
  param_1->bindSystemDataToSymbol("fireStartDemoCamera", (u32)&evFireStartDemoCamera);
  param_1->bindSystemDataToSymbol("isDemoMode", (u32)&evIsDemoMode);
  param_1->bindSystemDataToSymbol("checkMonteClear", (u32)&evCheckMonteClear);
	// clang-format on
}

void TNpcEvent::initDownSunflowerNum()
{
	if (TFlagManager::getInstance()->getBool(0x50003))
		mDownSunflowerNum = 5;
	else
		mDownSunflowerNum = 0;
}

static inline MSound* NpcEventGetMSound()
{
	MSound* sound = SMSGetMSound();
	return sound;
}

static s32 ReviveSunflowerCallBack(u32 param_1, u32 param_2)
{
	if (param_2 == 0) {
		TBaseNPC* sunflower = (TBaseNPC*)param_1;
		sunflower->sunflowerReviveIn();
		u32 sound = MSD_SE_NPC_MONTE_C_CLEANUP;
		if (TNpcEvent::mDownSunflowerNum == 0)
			sound = MSD_SE_SY_PROBLEM_SOLVED;
		if (NpcEventGetMSound()->gateCheck(sound))
			MSoundSESystem::MSoundSE::startSoundNpcActor(
			    sound, &sunflower->mPosition, 0, nullptr, 0, 4);
	}

	return 1;
}

// A by-value read of the static member, worth the last +4 of low region in
// reviveOneSunflower (a named local inside it is +8 and overshoots).
static inline int NpcEventDownSunflowerNum()
{
	return TNpcEvent::mDownSunflowerNum;
}

void TNpcEvent::reviveOneSunflower()
{
	if (mDownSunflowerNum > 0) {
		static const char* sViewObjName = "ひまわり";

		char acStack_50[0x40];

		int idx = 5 - NpcEventDownSunflowerNum();
		snprintf(acStack_50, 0x40, "%s%d", sViewObjName, idx);

		TBaseNPC* npc = (TBaseNPC*)JDrama::TNameRefGen::search2(acStack_50);
		--mDownSunflowerNum;

		static const char* sCameraNames[] = {
			"ひまわりカメラ0", "ひまわりカメラ1", "ひまわりカメラ2",
			"ひまわりカメラ3", "ひまわりカメラ4",
		};

		JDrama::TFlagT<u16> demoFlag(0);
		const JGeometry::TVec3<f32>* npcPos = &npc->unk1B8;
		SMSGetMarDirector()->fireStartDemoCamera(sCameraNames[idx], npcPos, -1,
		                                         0.0f,
		                                   true, &ReviveSunflowerCallBack,
		                                   (u32)npc, nullptr, demoFlag);

		if (mDownSunflowerNum == 0) {
			gpItemManager->makeShineAppearWithDemo(
			    "ひまわり用シャイン", "ひまわりシャインカメラ", npcPos->x,
			    npcPos->y + 500.0f, npcPos->z);
			TFlagManager::getInstance()->setBool(false, 0x50003);
		}
	}
}
