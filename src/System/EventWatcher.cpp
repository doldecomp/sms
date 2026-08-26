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
		name = ((JDrama::TNameRef*)ref)->getName();
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
		result = (JDrama::TNameRef*)slice.getDataInt();
		break;
	}

	return result;
}

static void evGetNPCType(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int result    = -1;
	TBaseNPC* npc = (TBaseNPC*)getNameRefPtr(interp->pop());
	if (npc)
		result = npc->getActorType() - 0x4000001;
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
			JGeometry::TVec3<f32> diff = which->mPosition;
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
				    interp->mProcessStack.getFromTop(arg_num - 1 - i));
				if (other) {
					JGeometry::TVec3<f32> diff = which->mPosition;
					diff -= other->mPosition;
					if (diff.length() <= dist)
						count++;
				}
			}
		}
	}

	for (int i = 0; i < arg_num; ++i)
		interp->pop();

	interp->push(count);
}

static void evGetTalkNPC(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);

	TBaseNPC* npc = SMSGetMarDirector()->getTalkingNPC();

	interp->push(!npc ? 0 : (int)npc);
}

static void evGetTalkNPCName(TSpcTypedInterp<TEventWatcher>* interp,
                             u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);

	TBaseNPC* npc = SMSGetMarDirector()->getTalkingNPC();

	if (!npc)
		interp->push("");
	else
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
static void evSetTalkMsgID(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	int p1 = TSpcSlice(interp->pop()).getDataInt();
	int p2 = TSpcSlice(interp->pop()).getDataInt();
	gpTalk2D->setMessageID(p2, p1);
	interp->push();
}

static void evGetTalkMode(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	interp->push((int)gpTalk2D->getTalkMode());
}

static void evGetTalkSelectedValue(TSpcTypedInterp<TEventWatcher>* interp,
                                   u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	interp->push((int)gpTalk2D->getSelectedValue());
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

static void evIsTalkModeNow(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	int value = SMSGetMarDirector()->isTalkModeNow() ? 1 : 0;
	interp->push(value);
}

static void evSetFlagNPCCanTaken(TSpcTypedInterp<TEventWatcher>* interp,
                                 u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	int arg          = TSpcSlice(interp->pop()).getDataInt();
	const char* name = interp->pop().getDataString();
	TBaseNPC* npc    = JDrama::TNameRefGen::search<TBaseNPC>(name);
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

	TLiveActor* liveActor = JDrama::TNameRefGen::search<TLiveActor>(actorName);
	if (liveActor && nerve)
		liveActor->mSpine->pushNerve(nerve);

	interp->push();
}

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
	int value             = TSpcSlice(interp->pop()).getDataInt();
	const char* actorName = interp->pop().getDataString();

	TLiveActor* liveActor = JDrama::TNameRefGen::search<TLiveActor>(actorName);
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

static void evSetDead4LiveActor(TSpcTypedInterp<TEventWatcher>* interp,
                                u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	int value             = TSpcSlice(interp->pop()).getDataInt();
	const char* actorName = interp->pop().getDataString();

	TLiveActor* liveActor = JDrama::TNameRefGen::search<TLiveActor>(actorName);
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

static void evSetTimeLimit(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int time = TSpcSlice(interp->pop()).getDataInt();
	OSResetStopwatch(&SMSGetMarDirector()->unkE8);
	SMSGetMarDirector()->unk120 = time;
	interp->push();
}

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
	interp->verifyArgNum(0, &arg_num);
	interp->push(SMSGetMarDirector()->getRestTime());
}

static void evGetPollutionLevel(TSpcTypedInterp<TEventWatcher>* interp,
                                u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	interp->push((int)gpPollution->getPollutionDegree());
}

static void evSetEventStart(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
}

static void evSetEventEnd(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
}

static void evSetNextStage(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	int scenario = TSpcSlice(interp->pop()).getDataInt();
	int stage    = TSpcSlice(interp->pop()).getDataInt();

	// This function reads the global directly. The rest of the file goes
	// through SMSGetMarDirector(), but here the accessor makes the match worse
	// (94.8% -> 92.4%), so the original must have had the bare global.
	gpMarDirector->setNextStage((scenario & 0xff) + ((stage + 1) << 8),
	                            nullptr);

	interp->push();
}

static void evRegisterMovie(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int movieId = TSpcSlice(interp->pop()).getDataInt();
	SMSGetMarDirector()->fireStreamingMovie(movieId);
	interp->push();
}

static void evGameOver(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	SMSGetMarDirector()->onUnk4CFlag(0x1);
	interp->push();
}

static void evIsGraffitoCoverage0(TSpcTypedInterp<TEventWatcher>* interp,
                                  u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	interp->push(gpPollution->cleanedAll() ? 1 : 0);
}

static void evSetGraffitoMultiplied(TSpcTypedInterp<TEventWatcher>* interp,
                                    u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int enable = TSpcSlice(interp->pop()).getDataInt();

	TPollutionManager* pollution = gpPollution;
	int i                        = 0;
	if (enable) {
		for (; i < pollution->getJointModelNum(); ++i)
			pollution->getLayer(i)->startSpread();
	} else {
		for (; i < pollution->getJointModelNum(); ++i)
			pollution->getLayer(i)->stopSpread();
	}

	interp->push();
}

static void evIsBossDefeated(TSpcTypedInterp<TEventWatcher>* interp,
                             u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	interp->push(gpConductor->isBossDefeated() ? 1 : 0);
}

static void evLaunchEventClearDemo(TSpcTypedInterp<TEventWatcher>* interp,
                                   u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	TGCConsole2* console = SMSGetMarDirector()->getConsole();
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
	    = JDrama::TNameRefGen::search<TMapEventSinkShadowMario>(
	        "イベント（カゲマリオゲート）");

	if (event)
		event->raiseBuilding(id);

	interp->push();
}

static void evForceCloseTalk(TSpcTypedInterp<TEventWatcher>* interp,
                             u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);

	gpTalk2D->forceCloseTalk();

	interp->push();
}

static void evInsertTimer(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);

	int p1 = interp->pop().getDataInt();
	int p2 = interp->pop().getDataInt();

	if (p2 == 0)
		SMSGetMarDirector()->getConsole()->startAppearTimer(0, p1);
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

	SMSGetMarDirector()->startTimer();
	SMSGetMarDirector()->getConsole()->startMoveTimer(time);

	interp->push();
}

static void evStartMonteman(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);

	TEMario* monteMan = JDrama::TNameRefGen::search<TEMario>("モンテマン");

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
	((TMushroom1up*)getNameRefPtr(interp->pop()))->kill();
	interp->push();
}

static void evAppearMushroom1up(TSpcTypedInterp<TEventWatcher>* interp,
                                u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TMushroom1up* mushroom = (TMushroom1up*)getNameRefPtr(interp->pop());
	mushroom->appear();
	SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_1UP_APPEAR, 0, nullptr, 0);
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
		TShine* shine = JDrama::TNameRefGen::search<TShine>(shineName);
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

	TShine* shine = JDrama::TNameRefGen::search<TShine>(shineName);
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

	THitActor* uuuh = JDrama::TNameRefGen::search<THitActor>(arg2);
	TShine* shine   = JDrama::TNameRefGen::search<TShine>(arg3);

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

static void evChangeNozzle(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TWaterGun::TNozzleType id
	    = (TWaterGun::TNozzleType)interp->pop().getDataInt();
	if (id == TWaterGun::DivingHelmet)
		gpMarioOriginal->setDivHelm();
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

	int count = p2 - p1 + 1;

	char buffer[] = "ゲーム木箱00";
	for (int i = p2; i <= p1; ++i) {
		if (i < 10) {
			buffer[10] = '0' + i;
			buffer[11] = 0;
		} else {
			buffer[10] = '0' + i / 10;
			buffer[11] = '0' + i % 10;
		}
		TMapObjBase* obj = JDrama::TNameRefGen::search<TMapObjBase>(buffer);
		if (obj && obj->checkLiveFlag(LIVE_FLAG_DEAD))
			--count;
	}

	interp->push(count);
}

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
		TMapObjBase* obj = JDrama::TNameRefGen::search<TMapObjBase>(buffer);
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
		TMapObjBase* obj = JDrama::TNameRefGen::search<TMapObjBase>(buffer);
		if (obj)
			obj->makeObjDead();
	}

	interp->push();
}

static void evIsInsideCube(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int cubeId = interp->pop().getDataInt();

	// TODO: getPos10cmAbove or something like that?
	JGeometry::TVec3<f32> pos = gpMarioOriginal->mPosition;
	pos.y += 10.0f;

	interp->push(gpCubeArea->isInCube(pos, cubeId) ? 1 : 0);
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

	TMapObjBase* obj = JDrama::TNameRefGen::search<TMapObjBase>("ＥＸビン");
	obj->getMActor()->setBck("exbottle_bottle_in");

	// The original keeps Mario in a register across both statements: the
	// store to mPosition would otherwise force a reload of the global.
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
	if (obj->getMActor()->curAnmEndsNext(0, nullptr))
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
	obj->changeObjSRT(JGeometry::TVec3<f32>(sx, sy, sz),
	                  JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
	                  JGeometry::TVec3<f32>(tx, ty, tz));

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

static void evEggYoshiStartFruit(TSpcTypedInterp<TEventWatcher>* interp,
                                 u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TEggYoshi* egg = (TEggYoshi*)getNameRefPtr(interp->pop());
	if (!egg->checkLiveFlag(LIVE_FLAG_DEAD))
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
		se = 0x4842;
		break;
	case 1:
		se = 0x484f;
		break;
	}
	SMSGetMSound()->startSoundSystemSE(se, 0, nullptr, 0);
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

static void evWarpMario(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(4, &arg_num);
	int arg1 = interp->pop().getDataInt();
	int z    = interp->pop().getDataInt();
	int y    = interp->pop().getDataInt();
	int x    = interp->pop().getDataInt();
	SMS_MarioWarpRequest(JGeometry::TVec3<f32>(x, y, z), arg1);
	interp->push();
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
			SMSGetMarDirector()->getConsole()->startAppearJetBalloon(0, 8);
		break;

	case 1:
		if (p1 == 1)
			SMSGetMarDirector()->getConsole()->startAppearJetBalloon(1, 10);
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
	SMSGetMarDirector()->getConsole()->unk94->startAppearReady();
	interp->push();
}

static void evAppear8RedCoinsAndTimer(TSpcTypedInterp<TEventWatcher>* interp,
                                      u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	TRedCoinSwitch* swtch
	    = JDrama::TNameRefGen::search<TRedCoinSwitch>("赤コイン用スイッチ");

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
	SMSGetMarDirector()->getConsole()->startAppearTimer(1,
	                                                    iVar9 * 0.008333334f);
	SMSGetMarDirector()->startTimer();
	SMSGetMarDirector()->getConsole()->startMoveTimer(10);
	interp->push();
}

// fabricated and wrong
JGeometry::TVec3<f32> rotateY(JGeometry::TVec3<f32> vec, s16 angleY)
{
	f32 x = vec.x * JMASCos(angleY) + vec.z * JMASSin(angleY);
	f32 z = -vec.x * JMASSin(angleY) + vec.z * JMASCos(angleY);
	vec.x = x;
	vec.z = z;
	return JGeometry::TVec3<f32>(vec.x, vec.y, vec.z);
}

static void evWarpFrontToMario(TSpcTypedInterp<TEventWatcher>* interp,
                               u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TLiveActor* actor = (TLiveActor*)interp->pop().getDataInt();

	s16 angleY = SMS_GetMarioAngleY();

	// TODO: codegen very wrong
	actor->mPosition = SMS_GetMarioPos()
	                   + rotateY(JGeometry::TVec3<f32>(0.0f, 0.0f, 400.0f),
	                             SMS_GetMarioAngleY());
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

static void evInvalidatePad(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int frames = interp->pop().getDataInt();

	SMSGetMarDirector()->getGamePad()->mDisabledFrames = frames;

	interp->push();
}

static void evIsWaterMelonIsReached(TSpcTypedInterp<TEventWatcher>* interp,
                                    u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TBigWatermelon* melon = (TBigWatermelon*)interp->pop().getDataInt();

	int result = 0;
	f32 dx     = -4660.0f - melon->mPosition.x;
	f32 dz     = 12000.0f - melon->mPosition.z;
	if (dx * dx + dz * dz <= 90000.0f)
		result = 1;

	interp->push(result);
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
