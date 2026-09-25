#include <System/MarDirector.hpp>
#include <System/TalkCursor.hpp>
#include <System/MarioGamePad.hpp>
#include <System/FlagManager.hpp>
#include <System/Application.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MoveBG/Item.hpp>
#include <NPC/NpcBase.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Mario.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

const char* cCameraBckNameShineGetInside
    = "/common/camera/camera_demo_shine_get_inside";
const char* cCameraBckNameShineGetOutside
    = "/common/camera/camera_demo_shine_get_outside";
const char* cCameraBckNameGate = "/common/camera/camera_demo_gate_in";

void TMarDirector::entryNPC(TBaseNPC* npc) { unk88.push_back(npc); }

void isNearMapObj(const JDrama::TActor&, const char*, f32) { }

void is3BarrelNear(const TBaseNPC&) { }

void TMarDirector::getTalkMsgID(TBaseNPC*) { }

void TMarDirector::updateFlag(TBaseNPC*, u32, u32) { }

TBaseNPC* TMarDirector::findNearestTalkNPC()
{
	TBaseNPC* result = nullptr;
	if (gpMarioOriginal->mStatus == MARIO_STATUS_WAIT) {
		f32 bestDist                   = 5000000.0f;
		JGeometry::TVec3<f32> marioPos = *gpMarioPos;
		JGadget::TVector_pointer<TBaseNPC>::iterator it;

		for (it = unk88.begin(); it != unk88.end(); ++it) {
			TBaseNPC* npc = *it;
			if (npc->checkLiveFlag(LIVE_FLAG_UNK100000)
			    || !npc->checkLiveFlag(LIVE_FLAG_UNK20000))
				continue;

			f32 dist = (npc->mPosition.x - marioPos.x)
			               * (npc->mPosition.x - marioPos.x)
			           + (npc->mPosition.y - marioPos.y)
			                 * (npc->mPosition.y - marioPos.y)
			           + (npc->mPosition.z - marioPos.z)
			                 * (npc->mPosition.z - marioPos.z);
			if (dist < bestDist) {
				bestDist = dist;
				result   = npc;
			}
		}
	}
	return result;
}

TBaseNPC* TMarDirector::findNearestTakeNPC()
{
	TBaseNPC* result = nullptr;
	JGadget::TVector_pointer<TBaseNPC>::iterator it;

	for (it = unk88.begin(); it != unk88.end(); ++it) {
		TBaseNPC* npc = *it;

		if (npc->isNowCanTaken() && gpMarioOriginal->isTakeSituation(npc))
			result = npc;
	}
	return result;
}

void TMarDirector::movement_game()
{
	unk84->associateNPC(nullptr);
	switch (unk124) {
	case 0:
		unk18[0]->offFlag(TMarioGamePad::PAD_FLAG_TALK_NPC);
		if (gpMarioOriginal->isHolding() || gpCamera->isLButtonCamera())
			break;

		if (!gpCamera->isDemoCamera()) {
			if (TBaseNPC* takeNpc = findNearestTakeNPC()) {
				unk84->associateNPC(takeNpc);
			} else {
				TBaseNPC* talkNpc = findNearestTalkNPC();
				if (talkNpc != nullptr) {
					unkA0 = talkNpc;
					unk84->associateNPC(talkNpc);
					unk18[0]->onFlag(TMarioGamePad::PAD_FLAG_TALK_NPC);
					unk128 |= 0x1;
					if ((unk128 & 2)
					    && (unk18[0]->checkFrameMeaning(
					        TMarioGamePad::MEANING_TALK_B)))
						unk126 = 1;
				}
			}
		}
		break;
	}
}

void TMarDirector::fireGetBlueCoin(TCoin* coin)
{
	if (!coin)
		return;

	TFlagManager::getInstance()->setBlueCoinFlag(
	    SMSGetApplication()->mCurrArea.getStage(), coin->getEventId());
	onFlag(DIRECTOR_FLAG_CARD_SAVE_PENDING);
	unk261 = 1;
	SMSGetMSound()->startSoundActor(MSD_SE_SY_BLUE_COIN_GET, &coin->mPosition,
	                                0, nullptr, 0, 4);
}

void TMarDirector::fireGetNozzle(TItemNozzle* nozzle)
{
	if (!nozzle)
		return;

	u8 stage = SMSGetApplication()->mCurrArea.getStage();
	if (nozzle->isActorType(0x20000022)
	    && !TFlagManager::getInstance()->getNozzleRight(stage, 0)) {
		TFlagManager::getInstance()->setNozzleRight(stage, 0);
		onFlag(DIRECTOR_FLAG_CARD_SAVE_PENDING);
		unk261 = 3;
	} else if (nozzle->isActorType(0x2000002A)
	           && !TFlagManager::getInstance()->getNozzleRight(stage, 1)) {
		TFlagManager::getInstance()->setNozzleRight(stage, 1);
		onFlag(DIRECTOR_FLAG_CARD_SAVE_PENDING);
		unk261 = 4;
	}
}

void TMarDirector::fireGetStar(TShine* shine)
{
	unk25C = shine;
	onFlag(DIRECTOR_FLAG_SHINE_GET_PENDING);
	JGeometry::TVec3<f32>& v = shine->mInitialRotation;
	fireStartDemoCamera(!shine->unk190 ? cCameraBckNameShineGetInside
	                                   : cCameraBckNameShineGetOutside,
	                    &gpMarioOriginal->mPosition, -1, v.y, false, nullptr, 0,
	                    nullptr, JDrama::TFlagT<u16>(0));
}

void TMarDirector::fireRideYoshi(TYoshi* yoshi)
{
	if (!yoshi)
		return;

	if (SMSGetApplication()->mCurrArea.getStage() == 1
	    && !TFlagManager::getInstance()->getBool(0x1038F)) {
		TFlagManager::getInstance()->setBool(true, 0x1038F);
		onFlag(DIRECTOR_FLAG_CARD_SAVE_PENDING);
		unk261 = 5;
	}
}

void TMarDirector::fireDefeatEnemy(TSpineEnemy*) { }

void TMarDirector::fireDemoMovie(u32, TLiveActor*) { }

void TMarDirector::movement()
{
	switch (mState) {
	case STATE_UNK4:
		movement_game();
		break;
	}
}

void TMarDirector::setNextStage(u16 param_1, JDrama::TActor* param_2)
{
	if (checkFlag(DIRECTOR_FLAG_STAGE_TRANSITION_PENDING))
		return;

	TGameSequence local;
	int stage = param_1;
	if (param_1 >= 0x100) {
		local.unk0 = (stage >> 8) - 1;
		local.unk1 = stage;
	} else {
		local.unk0 = param_1;
		local.unk1 = 0xff;
	}
	SMSGetApplication()->setNextArea(local);

	const TGameSequence& curArea = SMSGetApplication()->mCurrArea;
	if (param_2) {
		onFlag(DIRECTOR_FLAG_ACTOR_DEMO_STAGE_TRANSITION_PENDING);
		unk250 = param_2;
	} else {
		if ((curArea.getStage() == 1 && local.getStage() == 5)
		    || (curArea.getStage() == 1 && local.getStage() == 6)
		    || (curArea.getStage() == 1 && local.getStage() == 8))
			onFlag(DIRECTOR_FLAG_GATE_DEMO_STAGE_TRANSITION_PENDING);
		else
			onFlag(DIRECTOR_FLAG_STAGE_TRANSITION_PENDING);
	}

	switch (local.getStage()) {
	case 0x37:
		onFlag(DIRECTOR_FLAG_MOVIE_PENDING);
		SMSGetApplication()->setMovie(6);
		break;
	}
}

void TMarDirector::fireStageEvent(TMapObjBase*) { }

void TMarDirector::fireStartDemoCamera(const char* param_1,
                                       const JGeometry::TVec3<f32>* param_2,
                                       s32 param_3, f32 param_4, bool param_5,
                                       s32 (*param_6)(u32, u32), u32 param_7,
                                       JDrama::TActor* param_8,
                                       JDrama::TFlagT<u16> param_9)
{
	s32 used = (mDemoQueueTail - mDemoQueueHead) & 7;
	if (used >= 7)
		return;

	onFlag(DIRECTOR_FLAG_DEMO_PENDING);
	mDemoQueue[mDemoQueueTail].unk0  = param_1;
	mDemoQueue[mDemoQueueTail].unk4  = param_2;
	mDemoQueue[mDemoQueueTail].unk8  = param_3;
	mDemoQueue[mDemoQueueTail].unkC  = param_4;
	mDemoQueue[mDemoQueueTail].unk10 = param_5;
	mDemoQueue[mDemoQueueTail].unk14 = param_6;
	mDemoQueue[mDemoQueueTail].unk18 = param_7;
	mDemoQueue[mDemoQueueTail].unk1C = param_8;
	mDemoQueue[mDemoQueueTail].unk20 = param_9;

	mDemoQueueTail += 1;
	mDemoQueueTail &= 7;
}

void TMarDirector::fireEndDemoCamera()
{
	onFlag(DIRECTOR_FLAG_END_DEMO_PENDING);
}

void TMarDirector::fireStreamingMovie(u8 param_1)
{
	switch (param_1) {
	case 0:
		if (!checkFlag(DIRECTOR_FLAG_MOVIE_PENDING)) {
			onFlag(DIRECTOR_FLAG_MOVIE_PENDING);
			setNextStage(0x1, nullptr);
			TFlagManager::getInstance()->setBool(true, 0x10389);
			TFlagManager::getInstance()->setBool(true, 0x30004);
			SMSGetApplication()->setMovie(param_1);
		}
		break;

	case 10:
		if (!checkFlag(DIRECTOR_FLAG_MOVIE_PENDING)) {
			onFlag(DIRECTOR_FLAG_MOVIE_PENDING);
			setNextStage(0x3B, nullptr);
			SMSGetApplication()->setMovie(param_1);
		}
		break;

	case 7:
		if (!checkFlag(DIRECTOR_FLAG_MOVIE_PENDING)) {
			onFlag(DIRECTOR_FLAG_MOVIE_PENDING);
			setNextStage(0xE06, nullptr);
			SMSGetApplication()->setMovie(param_1);
		}
		break;

	case 8:
		if (!checkFlag(DIRECTOR_FLAG_MOVIE_PENDING)) {
			onFlag(DIRECTOR_FLAG_MOVIE_PENDING);
			setNextStage(0xE07, nullptr);
			SMSGetApplication()->setMovie(param_1);
		}
		break;

	case 11:
		if (!checkFlag(DIRECTOR_FLAG_MOVIE_PENDING)) {
			onFlag(DIRECTOR_FLAG_MOVIE_PENDING);
			setNextStage(0x3C, nullptr);
			SMSGetApplication()->setMovie(param_1);
		}
		break;

	case 2:
		if (!checkFlag(DIRECTOR_FLAG_MOVIE_PENDING)) {
			onFlag(DIRECTOR_FLAG_MOVIE_PENDING);
			setNextStage(0x101, nullptr);
			SMSGetApplication()->setMovie(param_1);
		}
		break;

	case 12:
	default:
		if (!checkFlag(DIRECTOR_FLAG_MOVIE_PENDING)) {
			onFlag(DIRECTOR_FLAG_MOVIE_PENDING);
			setNextStage(0xF, nullptr);
			SMSGetApplication()->setMovie((u8)param_1);
		}
		break;
	}
}
