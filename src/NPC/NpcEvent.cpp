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
#include <Player/MarioMain.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

extern const TNerveBase<TLiveActor>* NerveGetByIndex(int index);

int TNpcEvent::mDownSunflowerNum = 0;

static void IsNpcFlagOn_(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num,
                         u32 flag)
{
	interp->verifyArgNum(1, &arg_num);
	TBaseNPC* npc = (TBaseNPC*)interp->pop().getDataInt();
	int result    = 0;
	if (npc->checkLiveFlag(flag))
		result = 1;
	interp->push(result);
}

static void CheckNerve4Npc_(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num,
                            bool param_3)
{
	interp->verifyArgNum(2, &arg_num);
	int nerveId   = interp->pop().getDataInt();
	TBaseNPC* npc = (TBaseNPC*)interp->pop().getDataInt();
	const TNerveBase<TLiveActor>* expected = NerveGetByIndex(nerveId);
	const TNerveBase<TLiveActor>* actual   = param_3
	                                             ? npc->mSpine->getLatestNerve()
	                                             : npc->mSpine->getCurrentNerve();

	TSpcSlice result;
	if (actual == expected)
		result.setDataInt(1);
	interp->push(result);
}

static void evGetAddressFromViewObjName(TSpcTypedInterp<TEventWatcher>* interp,
                                        u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	const char* name = interp->pop().getDataString();
	JDrama::TViewObj* viewObj
	    = JDrama::TNameRefGen::search<JDrama::TViewObj>(name);
	interp->push((int)viewObj);
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
	IsNpcFlagOn_(interp, arg_num, LIVE_FLAG_UNK800000);
}

static void evIsGameModeNormal(TSpcTypedInterp<TEventWatcher>* interp,
                               u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	int result = 0;
	if (gpMarDirector->unk124 == 0)
		result = 1;
	interp->push(result);
}

static void ev__ForceStartTalkExceptNpc(TSpcTypedInterp<TEventWatcher>* interp,
                                        u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int result = 0;
	// TODO: uuuh...
	(void)interp->pop();

	if (!gpMarDirector->isTalkOrDemoModeNow() && SMS_IsMarioTouchGround4cm()
	    && !gpMarioOriginal->checkActionFlag(0x800)) {

		TBaseNPC* dummyNpc
		    = JDrama::TNameRefGen::search<TBaseNPC>("ダミーＮＰＣ");

		if (dummyNpc) {
			gpMarDirector->unkA0  = dummyNpc;
			gpMarDirector->unk126 = 1;

			result = 1;
		}
	}
	interp->push(result);
}

static void ev__ForceStartTalk(TSpcTypedInterp<TEventWatcher>* interp,
                               u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);

	int result = 0;

	if (!gpMarDirector->isTalkOrDemoModeNow() && SMS_IsMarioTouchGround4cm()
	    && !gpMarioOriginal->checkActionFlag(0x800)) {

		gpMarDirector->unkA0  = (TBaseNPC*)interp->pop().getDataInt();
		gpMarDirector->unk126 = 1;

		result = 1;
	} else {
		interp->pop();
	}

	interp->push(result);
}

static void evConnectDummyNpc(TSpcTypedInterp<TEventWatcher>* interp,
                              u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);

	int result = 0;

	TBaseNPC* dummyNpc = JDrama::TNameRefGen::search<TBaseNPC>("ダミーＮＰＣ");
	if (dummyNpc != nullptr) {
		const JDrama::TActor* actor
		    = (const JDrama::TActor*)interp->pop().getDataInt();
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
	TBaseNPC* dummyNpc = JDrama::TNameRefGen::search<TBaseNPC>("ダミーＮＰＣ");
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
	TBaseNPC* npc = (TBaseNPC*)interp->pop().getDataInt();
	npc->setBalloonMessage(fVar2, fVar1);
	interp->push();
}

static void evSetNpcTalkForbidCount(TSpcTypedInterp<TEventWatcher>* interp,
                                    u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	u16 count             = interp->pop().getDataInt();
	TBaseNPC* npc         = (TBaseNPC*)interp->pop().getDataInt();
	npc->mTalkForbidCount = count;
	interp->push();
}

static void evNpcDanceOn(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TBaseNPC* npc = (TBaseNPC*)interp->pop().getDataInt();
	npc->npcDanceIn();
	interp->push();
}

static void evNpcDanceOffHappyOn(TSpcTypedInterp<TEventWatcher>* interp,
                                 u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TBaseNPC* npc = (TBaseNPC*)interp->pop().getDataInt();
	npc->mActionFlag &= ~TBaseNPC::NPC_ACTION_DANCE;
	npc->npcHappyIn(2);
	interp->push();
}

static void evResetFruitNum(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	TFruitBasketEvent* basket = (TFruitBasketEvent*)interp->pop().getDataInt();
	basket->reset();
	interp->push();
}

static void evGetFruitNum(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(2, &arg_num);
	int fVar4                 = interp->pop().getDataInt();
	TFruitBasketEvent* basket = (TFruitBasketEvent*)interp->pop().getDataInt();

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
	int fVar5                 = interp->pop().getDataInt();
	int fVar4                 = interp->pop().getDataInt();
	TFruitBasketEvent* basket = (TFruitBasketEvent*)interp->pop().getDataInt();

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
	gpMarDirector->fireStartDemoCamera(cameraName, nullptr, -1, 0.0f, true,
	                                   nullptr, 0, nullptr,
	                                   JDrama::TFlagT<u16>());
	interp->push();
}

static void evIsDemoMode(TSpcTypedInterp<TEventWatcher>* interp, u32 arg_num)
{
	interp->verifyArgNum(0, &arg_num);
	int result = 0;
	if (gpMarDirector->isDemoModeNow())
		result = 1;
	interp->push(result);
}

static void evCheckMonteClear(TSpcTypedInterp<TEventWatcher>* interp,
                              u32 arg_num)
{
	interp->verifyArgNum(1, &arg_num);
	int fVar1 = interp->pop().getDataInt();

	char buffer[32];
	snprintf(buffer, 32, "モンテ%d", fVar1);
	TBaseNPC* npc = JDrama::TNameRefGen::search<TBaseNPC>(buffer);

	int b;
	if (!npc->checkLiveFlag(LIVE_FLAG_UNK400000) && npc->isClean())
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

static s32 ReviveSunflowerCallBack(u32 param_1, u32 param_2)
{
	if (param_2 == 0) {
		TBaseNPC* sunflower = (TBaseNPC*)param_1;
		sunflower->sunflowerReviveIn();
		u32 sound = 0x8808;
		if (TNpcEvent::mDownSunflowerNum == 0)
			sound = 0x4847;
		if (SMSGetMSound()->gateCheck(sound))
			MSoundSESystem::MSoundSE::startSoundNpcActor(
			    sound, &sunflower->mPosition, 0, nullptr, 0, 4);
	}

	return 1;
}

void TNpcEvent::reviveOneSunflower()
{
	if (mDownSunflowerNum > 0) {
		static const char* sViewObjName = "ひまわり";

		char acStack_50[0x40];

		int idx = 5 - mDownSunflowerNum;
		snprintf(acStack_50, 0x40, "%s%d", sViewObjName, idx);

		TBaseNPC* npc = JDrama::TNameRefGen::search<TBaseNPC>(acStack_50);
		--mDownSunflowerNum;

		static const char* sCameraNames[] = {
			"ひまわりカメラ0", "ひまわりカメラ1", "ひまわりカメラ2",
			"ひまわりカメラ3", "ひまわりカメラ4",
		};

		gpMarDirector->fireStartDemoCamera(sCameraNames[idx], &npc->unk1B8, -1,
		                                   0.0f, true, &ReviveSunflowerCallBack,
		                                   (u32)npc, nullptr, 0);

		if (mDownSunflowerNum == 0) {
			gpItemManager->makeShineAppearWithDemo(
			    "ひまわり用シャイン", "ひまわりシャインカメラ", npc->unk1B8.x,
			    npc->unk1B8.y + 500.0f, npc->unk1B8.z);
			TFlagManager::getInstance()->setBool(false, 0x50003);
		}
	}
}
