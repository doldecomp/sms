#include <System/MarDirector.hpp>
#include <System/Application.hpp>
#include <System/MSoundMainSide.hpp>
#include <System/MarioGamePad.hpp>
#include <System/PerformList.hpp>
#include <System/StageUtil.hpp>
#include <System/FlagManager.hpp>
#include <System/CardManager.hpp>
#include <Player/MarioMain.hpp>
#include <Strategic/ObjHitCheck.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <THPPlayer/THPPlayer.h>
#include <GC2D/GCConsole2.hpp>
#include <GC2D/ConsoleStr.hpp>
#include <GC2D/ScrnFader.hpp>
#include <GC2D/PauseMenu2.hpp>
#include <GC2D/CardSave.hpp>
#include <MSound/MSound.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JDrama/JDRCamera.hpp>
#include <dolphin/gx.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

extern OSThread gSetupThread;

int TMarDirector::direct()
{
	int vsyncRate = 600 / (int)SMSGetVSyncTimesPerSec();

	if (unk260 == 0) {
		if (!OSIsThreadTerminated(&gSetupThread))
			return 0;

		u32 local_40;
		OSJoinThread(&gSetupThread, &local_40);
		if (local_40)
			return 4;

		setupObjects();
		unk260 = 1;
	}

	u32 result = 1;

	JDrama::TGraphics local_140;

	u8 bVar2 = gpMSound->unkA8;
	unk54 += vsyncRate;

	int i = 0;
	for (;;) {
		if (!(unk4C & 0x4000)) {
			++i;
			if (i == 1)
				unk4C |= 0x2000;
			unk54 -= 5;
			if (unk54 < 5)
				unk4C |= 0x4000;

			// inline?
			u32 uVar8 = 0;
			u8 bVar7  = bVar2;
			if (unk4C & 0x4000) {
				if (unk258)
					unk258->stageLoop();
			} else {
				uVar8 |= 2;
				bVar7 &= ~0x1;
			}
			gpMSound->unkA8 = bVar7;

			switch (unk64) {
			case 5:
			case 11:
			case 12:
				uVar8 |= 2;
				uVar8 |= 1;
				break;

			case 10:
				uVar8 |= 2;
				uVar8 |= 1;
				break;
			}

			if (!(uVar8 & 1))
				++unk58;
			++unk5C;
			if (unk4C & 0x2000) {
				if (unk64 == 4 || unk64 == 7) {
					SMSRumbleMgr->update();
				}
			} else {
				for (int i = 0; i < 4; ++i) {
					// TODO: inline?
					TMarioGamePad* pad    = unk18[i];
					pad->mButton.mTrigger = 0;
					pad->mButton.mRelease = 0;

					unk18[i]->updateMeaning();
					unk18[i]->offFlag(0x10);
				}
			}

			u32 tmp = 0;
			if (unk4C & 0x2000)
				tmp |= 1;
			if (unk4C & 0x4000)
				tmp |= 2;
			local_140.unk2 = tmp;

			// inline
			bool bVar1 = true;
			if ((unk58 & 1) || (unk58 & 2))
				bVar1 = false;

			if (bVar1)
				gpObjHitCheck->checkActorsHit();
			else
				gpObjHitCheck->clearHitNum();

			u32 uVar11 = ~uVar8;
			u32 uVar4  = uVar11;
			if (unk58 & 1)
				uVar4 &= ~0x100;
			if (unk58 & 2)
				uVar4 &= 0x200;
			if (unk4E & 1)
				unk28->perform(uVar4, &local_140);
			else
				unk44->perform(uVar4, &local_140);

			u32 uVar44 = 0;
			if (!(unk4C & 0x4000))
				uVar44 |= 2;
			unk30->perform(~uVar44, &local_140);
			movement();
			if (!(uVar8 & 2)) {
				if (unk4E & 1)
					unk2C->perform(uVar11, &local_140);
				else
					unk48->perform(uVar11, &local_140);
			}

			if (unk4C & 0x4000) {
				local_140.unk2 = 0;
				unk34->perform(0xffffffff, &local_140);
				break;
			}
		} else {
			local_140.unk2 = 0;
			unk40->perform(0xffffffff, &local_140);
			unk38->perform(0xffffffff, &local_140);
			unk3C->perform(0xffffffff, &local_140);
			unk1C->perform(0xffffffff, &local_140);
			if ((gpSilhouetteManager->unk48 > 0.0f ? true : false)
			    || gpCamera->unk2C8 != -1) {
				unk20->perform(0xffffffff, &local_140);
			}
			unk24->perform(0xffffffff, &local_140);
			GXInvalidateTexAll();
		}
		result = changeState();
		unk4C &= 0x8000;
	}

	gpMSound->unkA8 = bVar2;
	return result;
}

static void decideNextStage()
{
	// TODO: inline hell. TFlagT is my mortal enemy
	TGameSequence local_3C;
	int stage = SMS_getShineStage(gpApplication.currArea.unk0);
	switch (stage) {
	case 0:
		local_3C.set(1, 0xff, JDrama::TFlagT<u16>(0));
		break;
	case 1:
	case 2:
	default:
		local_3C.set(1, 0xff, JDrama::TFlagT<u16>(0));
		break;
	}
	gpApplication.nextArea = local_3C;
}

static void decideNextStageOfMiss() { }

static void checkDefeatShadowMarioAll() { }

// fabricated
static inline bool decideSomething()
{
	static u8 stages[] = { 0x00 };

	int i = 0;
	do {
		if (!TFlagManager::smInstance->getShineFlag(stages[i]))
			return false;
		++i;
	} while (i < 7);

	return true;
}

static int decideNextScenario(u8 param_1)
{
	if ((int)param_1 != 1)
		return 0;

	if (TFlagManager::smInstance->getBool(0x103AE))
		return 2;

	if (decideSomething())
		return 9;

	if (TFlagManager::smInstance->getBool(0x10389))
		return 8;

	if (TFlagManager::smInstance->getBool(0x10386)
	    && TFlagManager::smInstance->getBool(0x10387)) {
		if (TFlagManager::smInstance->getFlag(0x40000) >= 10)
			return 7;
		else
			return 6;
	}

	if (TFlagManager::smInstance->getBool(0x10385))
		return 5;

	if (TFlagManager::smInstance->getBool(0x10384))
		return 1;

	return 0;
}

int TMarDirector::changeState()
{
	u8 uVar5 = 1;
	u8 uVar3 = unk64;
	switch (unk64) {
	case 0:
		switch (gpApplication.currArea.unk0) {
		case 0xf:
			uVar3 = 4;
			unk50 |= 1;
			break;

		case 0x2:
		case 0x3:
		case 0x4:
		case 0x5:
		case 0x6:
		case 0x8:
		case 0x9:
		case 0x34:
			uVar3 = 1;
			unk50 |= 0x6;
			break;

		case 1:
			if (unk4E & 2) {
				uVar3 = 1;
				unk50 |= 6;
			} else {
				uVar3 = 2;
			}
			break;

		case 0:
		case 0x7:
		default:
			if (JKRGetResource("/scene/map/camera/startcamera.bck")) {
				uVar3 = 1;
				unk50 |= 10;
			} else {
				uVar3 = 4;
			}
			break;
		}
		break;

	case 1:
		if (unk4E & 4) {
			if (console->unk94->unk2BC == 4) {
				uVar3 = 3;
				unk4E &= ~0x8;
			}
		} else {
			TConsoleStr* str = console->unk94;
			f32 iVar2        = gpCamera->getRestDemoFrames() / 120.0f;
			if (iVar2 <= str->getWipeCloseTime()
			    || ((gpApplication.currArea.unk0 != 1
			         || gpApplication.currArea.unk1 != 1)
			        && (gpApplication.currArea.unk0 != 1
			            || gpApplication.currArea.unk1 != 9)
			        && ((*unk18)->mEnabledFrameMeaning & 0x61))) {
				unk4E |= 4;
				console->unk94->startCloseWipe((unk50 & 8) != 0);
				unk50 &= ~0x4;
			}
		}
		break;

	case 3:
		if (gpApplication.currArea.unk0 == 1) {
			if (console->unk94->unk2BC == 6)
				uVar3 = 2;
		} else if (console->unk94->unk2BC == 6
		           && gpApplication.currArea.unk0 == 0) {
			uVar3 = 4;
		}
		break;

	case 2:
		if (!(gpMarioOriginal->action & 0x1000 ? true : false))
			uVar3 = 4;
		break;

	case 4:
		uVar3 = updateGameMode();
		break;

	case 5:
		switch (unkAC->getNextState()) {
		case 0:
			uVar3 = 4;
			break;
		case 1:
			unkE4 = 4;
			uVar3 = 12;
			unkB4 = 4;
			break;
		case 5:
			decideNextStage();
			unk4C &= ~0x100;
			moveStage();
			uVar3 = 9;
			break;
		}
		break;

	case 10:
		if (unk78 && gpApplication.unkFader->unk20 == 1)
			uVar3 = 4;
		break;

	case 11: {
		switch (unkAC->unk118->getNextState()) {
		case 0:
			if (unk261 == 7) {
				TFlagManager::smInstance->restore();
				TFlagManager::smInstance->setBool(true, 0x30001);
				gpApplication.nextArea.unk0
				    = TFlagManager::smInstance->getFlag(0x40000);
				if (gpApplication.nextArea.unk0)
					gpApplication.nextArea.unk1 = 0xff;
				else
					gpApplication.nextArea.unk1 = 0;
				gpApplication.nextArea.unk2 = 0;
				unk4C &= ~0x100;
				moveStage();
				gpApplication.unkFader->setFadeStatus(
				    TSMSFader::FADE_STATUS_UNK0);
				uVar5 = 5;
			} else {
				uVar3 = 4;
			}
			break;
		case 1:
			if (unk261 == 7) {
				gpApplication.unkFader->setFadeStatus(
				    TSMSFader::FADE_STATUS_UNK0);
				uVar5 = 4;
			} else {
				unkE4 = 4;
				uVar3 = 12;
				unkB4 = 4;
			}
			break;
		}
		break;
	}

	case 7:
		if (gpApplication.unkFader->unk20 == 0
		    && (MSBgm::getHandle(2) == 0 || unk5C - unk60 > 0x4AF)) {
			if (TFlagManager::smInstance->getFlag(0x2001) < 0) {
				TFlagManager::smInstance->setBool(true, 0x30002);
				if (!SMS_isExMap() && gpApplication.currArea.unk0 != 0
				    && gpApplication.currArea.unk0 != 60) {
					decideNextStage();
				} else {
					gpApplication.nextArea.set(gpApplication.currArea.unk0, 0,
					                           0);
				}
				unk4C &= ~0x100;
				moveStage();
				unkE4 = 0xf;
				gpApplication.unkFader->setColor(
				    JUtility::TColor(0, 0, 0, 0xff));
				uVar3 = 12;
			} else {
				gpApplication.unkFader->startWipe(0xE, 0.3f, 0.0f);
				gpApplication.unkFader->setColor(
				    JUtility::TColor(0, 0, 0, 0xff));
				unk261 = 7;
				console->startDisappearStar();
				console->startDisappearCoin();
			}
		}
		break;

	case 9:
	case 12:
		if (gpApplication.unkFader->unk20 == 0
		    && gpMSound->checkWaveOnAram(MS_WAVE_DEFAULT))
			uVar5 = unkB4;
		break;
	}

	if (((*unk18)->mPortNum & 0x3FU & TMarioGamePad::mResetFlag)
	    && gpCardManager->getLastStatus() != -1 && (unk4C & 0x4000)
	    && !(unk50 & 0x10)) {
		uVar3 = 12;
		unk50 |= 0x10;
		unkE4 = 4;
		unkB4 = 4;
	}

	if (uVar3 != unk64) {
		currentStateFinalize(uVar3);
		nextStateInitialize(uVar3);
		unk64 = uVar3;
	}

	return uVar5;
}

void TMarDirector::currentStateFinalize(u8 param_1)
{
	switch (unk64) {
	case 0:
		JDrama::TNameRefGen::search<JDrama::TViewObj>("Group 2D")->unkC.mValue
		    &= ~0xB;
		JDrama::TNameRefGen::search<JDrama::TViewObj>("Guide")->unkC.mValue
		    |= 0xB;

		gpApplication.unkFader->startWipe(unkE4, 0.4f, 0.0f);
		SMSRumbleMgr->reset();
		if (gpApplication.currArea.unk0 == 1)
			THPPlayerPlay();
		break;

	case 1:
		(*unk18)->mFlags &= ~0x1;
		gpCamera->endDemoCamera();
		console->unk94->startOpenWipe();
		MSMainProc::endStageEntranceDemo(gpApplication.currArea.unk0,
		                                 gpApplication.currArea.unk1);
		break;

	case 4:
		if (unk124 == 0)
			OSStopStopwatch(&unkE8);
		(*unk18)->mFlags &= ~0x2;
		break;

	case 5:
		(*unk18)->mFlags &= ~0x1;
		SMSRumbleMgr->finishPause();
		if (gpApplication.currArea.unk0 == 1)
			THPPlayerPlay();
		break;

	case 10:
		(*unk18)->mFlags &= ~0x1;
		SMSRumbleMgr->finishPause();

		JDrama::TNameRefGen::search<JDrama::TViewObj>("Group 2D")->unkC.mValue
		    &= ~0xB;
		JDrama::TNameRefGen::search<JDrama::TViewObj>("Guide")->unkC.mValue
		    |= 0xB;

		SMSSwitch2DArchive("guide", gArBkConsole);
		if (gpApplication.currArea.unk0 == 1)
			THPPlayerPlay();
		break;

	case 11:
		(*unk18)->mFlags &= ~0x1;
		SMSRumbleMgr->finishPause();
		if (gpApplication.currArea.unk0 == 1)
			THPPlayerPlay();
		switch (unk261) {
		case 3:
			console->startAppearBalloon(0xE0048, true);
			break;

		case 4:
			console->startAppearBalloon(0xE0049, true);
			break;
		}
		break;
	}
}

#pragma dont_inline on
void TMarDirector::setMario() { }

void TMarDirector::nextStateInitialize(u8) { }

u8 TMarDirector::updateGameMode() { }

void TMarDirector::moveStage() { }
#pragma dont_inline off

JStage::TObject* TMarDirector::JSGFindObject(const char* param_1,
                                             JStage::TEObject param_2) const
{
	if (strcmp("cam_int1", param_1) == 0) {
		JDrama::TCamera* cam
		    = (JDrama::TCamera*)const_cast<TMarDirector*>(this)->search(
		        "camera 1");
		return cam;
	}

	if (strcmp("mario", param_1) == 0) {
		JDrama::TActor* mario
		    = (JDrama::TActor*)const_cast<TMarDirector*>(this)->search(
		        "マリオ");
		return mario;
	}

	return JDrama::TDirector::JSGFindObject(param_1, param_2);
}
