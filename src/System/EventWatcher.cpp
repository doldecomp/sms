#include <System/EventWatcher.hpp>
#include <NPC/NpcEvent.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>

static void evGetSystemFlag(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetSystemFlag(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evGetNameRefHandle(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evGetNameRefName(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void getNameRefPtr(TSpcSlice) { }
static void evGetNPCType(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetFlagNPCDontTalk(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetFlagNPCDontThrow(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetFlagNPCDead(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evIsNearSameActors(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evIsNearActors(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evGetTalkNPC(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evGetTalkNPCName(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetTalkMsgID(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evGetTalkMode(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evGetTalkSelectedValue(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetValue2TalkVariable(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evIsTalkModeNow(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetFlagNPCCanTaken(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evPushNerve4LiveActor(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evIsOnLiveActorFlag(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetHide4LiveActor(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetDead4LiveActor(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetTimeLimit(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetAttentionTime(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetPollutionIncreaseCount(TSpcTypedInterp<TEventWatcher>*, u32) {
}
static void evGetRestTime(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evGetPollutionLevel(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetEventStart(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetEventEnd(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetNextStage(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evRegisterMovie(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evGameOver(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evIsGraffitoCoverage0(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetGraffitoMultiplied(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evIsBossDefeated(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evLaunchEventClearDemo(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evIsEMarioReachedToGoal(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evIsEMarioDownWaitingToTalk(TSpcTypedInterp<TEventWatcher>*, u32) {
}
static void evStartEMarioRunAway(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evStartEMarioGateDrawing(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evStartEMarioDisappear(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evStartOpenModelGate(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evIsMapEventFinishedAll(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evRaiseBuilding(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evForceCloseTalk(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evInsertTimer(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evStartTimer(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evStartMonteman(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evStopTimer(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evMonteManReachFlag(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evGetTime(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evKillShine(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evKillMushroom1up(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evAppearMushroom1up(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evAppearShineFromNPC(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evAppearShine(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evAppearShineFromNPCWithoutDemo(TSpcTypedInterp<TEventWatcher>*,
                                            u32)
{
}
static void evAppearShineFromKageMario(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evAppearShineForWoodBox(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evChangeNozzle(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evStartMarioTalking(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evCheckWoodBox(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evRefreshWoodBox(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evKillWoodBox(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evIsInsideCube(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetMarioWaiting(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evStartMareBottleDemo(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evIsFinishMareBottleDemo(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evIsInsideFastCube(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetTransScale(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetEventID(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evManiCoinDown(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evStartBGM(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evEggYoshiStartFruit(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evPutNozzle(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evStopBGM(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evStartSE(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int se = interp->pop().getDataInt();
	if (gpMSound->gateCheck(se))
		MSoundSESystem::MSoundSE::startSoundSystemSE(se, 0, nullptr, 0);
	interp->push(TSpcSlice());
}
static void evStartEventSE(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evStartMiss(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evChangeSunglass(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetCollision(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evWarpMario(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evStartAppearJetBalloon(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evSetEventForWaterMelon(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evAppearReadyGo(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evAppear8RedCoinsAndTimer(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evWarpFrontToMario(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evOnNeutralMarioKey(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evInvalidatePad(TSpcTypedInterp<TEventWatcher>*, u32) { }
static void evIsWaterMelonIsReached(TSpcTypedInterp<TEventWatcher>*, u32) { }

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

void TEventWatcher::perform(u32 param_1, JDrama::TGraphics*)
{
	if ((param_1 & 1) && mInterp)
		mInterp->update();
}
