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
#include <Player/MarioMain.hpp>

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
	if (gpMarioOriginal->mAction == 0xC400201) {
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
	if ((int)unk124 == 0)
		return;

	(*unk18)->offFlag(0x2);
	if (!gpMarioOriginal->isHolding()
	    && gpCamera->isLButtonCameraSpecifyMode(gpCamera->mMode))
		return;

	bool bVar1 = true;
	if (gpCamera->isSimpleDemoCamera() && gpCamera->mMode != 0x49) {
		bVar1 = false;
	}

	if (!bVar1) {
		TBaseNPC* takeNpc = findNearestTakeNPC();
		if (takeNpc != nullptr) {
			unk84->associateNPC(takeNpc);
		} else {
			TBaseNPC* talkNpc = findNearestTalkNPC();
			if (talkNpc != nullptr) {
				unkA0 = talkNpc;
				unk84->associateNPC(talkNpc);
				(*unk18)->onFlag(4);
				unk128 |= 0x1;
				if ((unk128 & 2) && ((*unk18)->mEnabledFrameMeaning & 0x800))
					unk126 = 1;
			}
		}
	}
}

void TMarDirector::fireGetBlueCoin(TCoin* coin)
{
	if (!coin)
		return;

	TFlagManager::smInstance->setBlueCoinFlag(gpApplication.mCurrArea.unk0,
	                                          coin->unk134);
	unk4C |= 0x200;
	unk261 = 1;
	if (gpMSound->gateCheck(0x4845))
		MSoundSESystem::MSoundSE::startSoundActor(0x4845, coin->mPosition, 0,
		                                          nullptr, 0, 4);
}

void TMarDirector::fireGetNozzle(TItemNozzle* nozzle)
{
	if (!nozzle)
		return;

	if (nozzle->isActorType(0x20000022)
	    && TFlagManager::smInstance->getNozzleRight(
	        gpApplication.mCurrArea.unk0, 0)) {
		TFlagManager::smInstance->setNozzleRight(gpApplication.mCurrArea.unk0,
		                                         0);
		unk4C |= 0x200;
		unk261 = 3;
	} else if (nozzle->isActorType(0x2000002A)
	           && TFlagManager::smInstance->getNozzleRight(
	               gpApplication.mCurrArea.unk0, 1)) {
		TFlagManager::smInstance->setNozzleRight(gpApplication.mCurrArea.unk0,
		                                         1);
		unk4C |= 0x200;
		unk261 = 4;
	}
}

void TMarDirector::fireGetStar(TShine* shine)
{
	unk25C = shine;
	unk4C |= 1;
	JGeometry::TVec3<f32>& v = shine->mInitialRotation;
	fireStartDemoCamera(!shine->unk190 ? cCameraBckNameShineGetInside
	                                   : cCameraBckNameShineGetOutside,
	                    &gpMarioOriginal->mPosition, -1, v.y, false, nullptr, 0,
	                    nullptr, JDrama::TFlagT<u16>(0));
}

void TMarDirector::fireRideYoshi(TYoshi*) { }

void TMarDirector::fireDefeatEnemy(TSpineEnemy*) { }

void TMarDirector::fireDemoMovie(u32, TLiveActor*) { }

void TMarDirector::movement()
{
	if ((int)mState != 4)
		movement_game();
}

#pragma dont_inline on
void TMarDirector::setNextStage(u16 param_1, JDrama::TActor* param_2)
{
	// TODO: wtf is happening in this function it's cursed
}
#pragma dont_inline off

void TMarDirector::fireStageEvent(TMapObjBase*) { }

void TMarDirector::fireStartDemoCamera(const char* param_1,
                                       const JGeometry::TVec3<f32>* param_2,
                                       s32 param_3, f32 param_4, bool param_5,
                                       s32 (*param_6)(u32, u32), u32 param_7,
                                       JDrama::TActor* param_8,
                                       JDrama::TFlagT<u16> param_9)
{
	if (((unk24C - unk24D) & 7) >= 7)
		return;

	unk4C |= 0x40;
	unk12C[unk24C].unk0  = param_1;
	unk12C[unk24C].unk4  = param_2;
	unk12C[unk24C].unk8  = param_3;
	unk12C[unk24C].unkC  = param_4;
	unk12C[unk24C].unk10 = param_5;
	unk12C[unk24C].unk14 = param_6;
	unk12C[unk24C].unk18 = param_7;
	unk12C[unk24C].unk1C = param_8;
	unk12C[unk24C].unk20 = param_9;

	unk24C += 1;
	unk24C &= 7;
}

void TMarDirector::fireEndDemoCamera() { unk4C |= 0x80; }

void TMarDirector::fireStreamingMovie(u8 param_1)
{
	switch (param_1) {
	case 0:
		if (!(unk4C & 0x100)) {
			unk4C |= 0x100;
			setNextStage(0x1, nullptr);
			TFlagManager::smInstance->setBool(true, 0x10389);
			TFlagManager::smInstance->setBool(true, 0x30004);
			gpApplication.mMovie = param_1;
		}
		break;

	case 10:
		if (!(unk4C & 0x100)) {
			unk4C |= 0x100;
			setNextStage(0x3B, nullptr);
			gpApplication.mMovie = param_1;
		}
		break;

	case 7:
		if (!(unk4C & 0x100)) {
			unk4C |= 0x100;
			setNextStage(0xE06, nullptr);
			gpApplication.mMovie = param_1;
		}
		break;

	case 8:
		if (!(unk4C & 0x100)) {
			unk4C |= 0x100;
			setNextStage(0xE07, nullptr);
			gpApplication.mMovie = param_1;
		}
		break;

	case 11:
		if (!(unk4C & 0x100)) {
			unk4C |= 0x100;
			setNextStage(0x3C, nullptr);
			gpApplication.mMovie = param_1;
		}
		break;

	case 2:
		if (!(unk4C & 0x100)) {
			unk4C |= 0x100;
			setNextStage(0x101, nullptr);
			gpApplication.mMovie = param_1;
		}
		break;

	default:
		if (!(unk4C & 0x100)) {
			unk4C |= 0x100;
			setNextStage(0xF, nullptr);
			gpApplication.mMovie = (u8)param_1;
		}
		break;
	}
}
