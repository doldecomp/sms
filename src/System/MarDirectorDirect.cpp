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
#include <GC2D/Guide.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JDrama/JDRCamera.hpp>
#include <dolphin/gx.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

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

			switch (mState) {
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
				if (mState == 4 || mState == 7) {
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
				mShinePfLstMov->perform(uVar4, &local_140);
			else
				mShinePfLstMov->perform(uVar4, &local_140);

			u32 uVar44 = 0;
			if (!(unk4C & 0x4000))
				uVar44 |= 2;
			unk30->perform(~uVar44, &local_140);
			movement();
			if (!(uVar8 & 2)) {
				if (unk4E & 1)
					mPerformListCalcAnim->perform(uVar11, &local_140);
				else
					mShinePfLstAnm->perform(uVar11, &local_140);
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
			mPerformListGX->perform(0xffffffff, &local_140);
			if ((gpSilhouetteManager->unk48 > 0.0f ? true : false)
			    || gpCamera->unk2C8 != -1) {
				mPerformListSilhouette->perform(0xffffffff, &local_140);
			}
			mPerformListGXPost->perform(0xffffffff, &local_140);
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
	int stage = SMS_getShineStage(gpApplication.mCurrArea.unk0);
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
	gpApplication.mNextArea = local_3C;
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
	u8 uVar5     = 1;
	u8 nextState = mState;
	switch (mState) {
	case 0:
		switch (gpApplication.mCurrArea.unk0) {
		case 0xf:
			nextState = 4;
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
			nextState = 1;
			unk50 |= 0x6;
			break;

		case 1:
			if (unk4E & 2) {
				nextState = 1;
				unk50 |= 6;
			} else {
				nextState = 2;
			}
			break;

		case 0:
		case 0x7:
		default:
			if (JKRGetResource("/scene/map/camera/startcamera.bck")) {
				nextState = 1;
				unk50 |= 10;
			} else {
				nextState = 4;
			}
			break;
		}
		break;

	case 1:
		if (unk4E & 4) {
			if (mConsole->unk94->unk2BC == 4) {
				nextState = 3;
				unk4E &= ~0x8;
			}
		} else {
			TConsoleStr* str = mConsole->unk94;
			f32 iVar2        = gpCamera->getRestDemoFrames() / 120.0f;
			if (iVar2 <= str->getWipeCloseTime()
			    || ((gpApplication.mCurrArea.unk0 != 1
			         || gpApplication.mCurrArea.unk1 != 1)
			        && (gpApplication.mCurrArea.unk0 != 1
			            || gpApplication.mCurrArea.unk1 != 9)
			        && (unk18[0]->mEnabledFrameMeaning & 0x61))) {
				unk4E |= 4;
				mConsole->unk94->startCloseWipe((unk50 & 8) != 0);
				unk50 &= ~0x4;
			}
		}
		break;

	case 3:
		if (gpApplication.mCurrArea.unk0 == 1) {
			if (mConsole->unk94->unk2BC == 6)
				nextState = 2;
		} else if (mConsole->unk94->unk2BC == 6
		           && gpApplication.mCurrArea.unk0 == 0) {
			nextState = 4;
		}
		break;

	case 2:
		if (!(gpMarioOriginal->mAction & 0x1000 ? true : false))
			nextState = 4;
		break;

	case 4:
		nextState = updateGameMode();
		break;

	case 5:
		switch (unkAC->getNextState()) {
		case 0:
			nextState = 4;
			break;
		case 1:
			unkE4     = 4;
			nextState = 12;
			unkB4     = 4;
			break;
		case 5:
			decideNextStage();
			unk4C &= ~0x100;
			moveStage();
			nextState = 9;
			break;
		}
		break;

	case 10:
		if (unk78 && gpApplication.mFader->mFadeStatus == 1)
			nextState = 4;
		break;

	case 11: {
		switch (unkAC->unk118->getNextState()) {
		case 0:
			if (unk261 == 7) {
				TFlagManager::smInstance->restore();
				TFlagManager::smInstance->setBool(true, 0x30001);
				gpApplication.mNextArea.unk0
				    = TFlagManager::smInstance->getFlag(0x40000);
				if (gpApplication.mNextArea.unk0)
					gpApplication.mNextArea.unk1 = 0xff;
				else
					gpApplication.mNextArea.unk1 = 0;
				gpApplication.mNextArea.unk2 = 0;
				unk4C &= ~0x100;
				moveStage();
				gpApplication.mFader->setFadeStatus(
				    TSMSFader::FADE_STATUS_UNK0);
				uVar5 = 5;
			} else {
				nextState = 4;
			}
			break;
		case 1:
			if (unk261 == 7) {
				gpApplication.mFader->setFadeStatus(
				    TSMSFader::FADE_STATUS_UNK0);
				uVar5 = 4;
			} else {
				unkE4     = 4;
				nextState = 12;
				unkB4     = 4;
			}
			break;
		}
		break;
	}

	case 7:
		if (gpApplication.mFader->mFadeStatus == 0
		    && (MSBgm::getHandle(2) == 0 || unk5C - unk60 > 0x4AF)) {
			if (TFlagManager::smInstance->getFlag(0x2001) < 0) {
				TFlagManager::smInstance->setBool(true, 0x30002);
				if (!SMS_isExMap() && gpApplication.mCurrArea.unk0 != 0
				    && gpApplication.mCurrArea.unk0 != 60) {
					decideNextStage();
				} else {
					gpApplication.mNextArea.set(gpApplication.mCurrArea.unk0, 0,
					                            0);
				}
				unk4C &= ~0x100;
				moveStage();
				unkE4 = 0xf;
				gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 0xff));
				nextState = 12;
			} else {
				gpApplication.mFader->startWipe(0xE, 0.3f, 0.0f);
				gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 0xff));
				unk261 = 7;
				mConsole->startDisappearStar();
				mConsole->startDisappearCoin();
			}
		}
		break;

	case 9:
	case 12:
		if (gpApplication.mFader->mFadeStatus == 0
		    && gpMSound->checkWaveOnAram(MS_WAVE_DEFAULT))
			uVar5 = unkB4;
		break;
	}

	if ((unk18[0]->mPortNum & 0x3FU & TMarioGamePad::mResetFlag)
	    && gpCardManager->getLastStatus() != -1 && (unk4C & 0x4000)
	    && !(unk50 & 0x10)) {
		nextState = 12;
		unk50 |= 0x10;
		unkE4 = 4;
		unkB4 = 4;
	}

	if (nextState != mState) {
		currentStateFinalize(nextState);
		nextStateInitialize(nextState);
		mState = nextState;
	}

	return uVar5;
}

void TMarDirector::currentStateFinalize(u8 next_state)
{
	switch (mState) {
	case 0:
		JDrama::TNameRefGen::search<JDrama::TViewObj>("Group 2D")->unkC.mValue
		    &= ~0xB;
		JDrama::TNameRefGen::search<JDrama::TViewObj>("Guide")->unkC.mValue
		    |= 0xB;

		gpApplication.mFader->startWipe(unkE4, 0.4f, 0.0f);
		SMSRumbleMgr->reset();
		if (gpApplication.mCurrArea.unk0 == 1)
			THPPlayerPlay();
		break;

	case 1:
		unk18[0]->mFlags &= ~0x1;
		gpCamera->endDemoCamera();
		mConsole->unk94->startOpenWipe();
		MSMainProc::endStageEntranceDemo(gpApplication.mCurrArea.unk0,
		                                 gpApplication.mCurrArea.unk1);
		break;

	case 4:
		if (unk124 == 0)
			OSStopStopwatch(&unkE8);
		unk18[0]->mFlags &= ~0x2;
		break;

	case 5:
		unk18[0]->mFlags &= ~0x1;
		SMSRumbleMgr->finishPause();
		if (gpApplication.mCurrArea.unk0 == 1)
			THPPlayerPlay();
		break;

	case 10:
		unk18[0]->mFlags &= ~0x1;
		SMSRumbleMgr->finishPause();

		JDrama::TNameRefGen::search<JDrama::TViewObj>("Group 2D")->unkC.mValue
		    &= ~0xB;
		JDrama::TNameRefGen::search<JDrama::TViewObj>("Guide")->unkC.mValue
		    |= 0xB;

		SMSSwitch2DArchive("guide", gArBkConsole);
		if (gpApplication.mCurrArea.unk0 == 1)
			THPPlayerPlay();
		break;

	case 11:
		unk18[0]->mFlags &= ~0x1;
		SMSRumbleMgr->finishPause();
		if (gpApplication.mCurrArea.unk0 == 1)
			THPPlayerPlay();
		switch (unk261) {
		case 3:
			mConsole->startAppearBalloon(0xE0048, true);
			break;

		case 4:
			mConsole->startAppearBalloon(0xE0049, true);
			break;
		}
		break;
	}
}

#pragma dont_inline on
void TMarDirector::setMario()
{
	bool cVar4 = TFlagManager::smInstance->getBool(0x30006);
	if (cVar4)
		TFlagManager::smInstance->setBool(false, 0x30006);

	u32 uVar11 = unkD0;

	JDrama::TNameRef* marioSetPosition
	    = JDrama::TNameRefGen::search<JDrama::TNameRef>("マリオセット位置");
	if (!marioSetPosition /* && marioSetPosition->unkD0 */)
		uVar11 = 0;

	switch (unkD1) {
		// TODO: requires knowing what marioSetPosition is and TWaterGun
	}
}
#pragma dont_inline off

void TMarDirector::nextStateInitialize(u8 next_state)
{
	TGameSequence& currSeq = gpApplication.mCurrArea;

	switch (next_state) {
	case 1: {
		const char* pcVar8 = "startcamera";
		unk18[0]->onFlag(0x1);
		unk68 = 0;
		if (gpApplication.mCurrArea.unk0 == 1 && (unk4E & 2)) {
			if (gpApplication.mCurrArea.unk1 == 8) {
				switch (TFlagManager::smInstance->getFlag(0x60003)) {
				case 0:
					if (TFlagManager::smInstance->getFlag(0x40000) >= 0x14)
						pcVar8 = "mareopen_startcamera";
					break;
				case 1:
					pcVar8 = "yoshi_startcamera";
					break;
				case 2:
					pcVar8 = "turbo_startcamera";
					break;
				case 3:
					pcVar8 = "rocket_startcamera";
					break;
				}
			} else {
				if (TFlagManager::smInstance->getBool(0x50001)) {
					pcVar8 = "sinkricco";
				} else {
					if (TFlagManager::smInstance->getBool(0x50002))
						pcVar8 = "sinkmamma";
				}
			}
		}
		gpCamera->startDemoCamera(pcVar8, nullptr, -1, 0.0f, true);
		if (unk50 & 4) {
			mConsole->unk94->startAppearScenario();
			unk50 &= ~0x4;
		}
		MSMainProc::startStageEntranceDemo(currSeq.unk0, currSeq.unk1);
		break;
	}

	case 3:
		unk68 = 0;
		if (!(unk50 & 1)) {
			MSMainProc::startStageBGM(currSeq.unk0, currSeq.unk1);
			setMario();
			unk50 |= 1;
		}
		break;

	case 2:
		if (!(unk50 & 1)) {
			MSMainProc::startStageBGM(currSeq.unk0, currSeq.unk1);
			setMario();
			unk50 |= 1;
		}
		if (mMap != 0xf)
			mConsole->unkC.mValue &= ~0xB;
		break;

	case 4:
		if (mState <= 3 && mMap != 0xf)
			mConsole->unkC.mValue &= ~0xB;
		if (unk50 & 2) {
			mConsole->unk94->startAppearGo();
			unk50 &= ~0x2;
		}
		if (!(unk50 & 1)) {
			MSMainProc::startStageBGM(currSeq.unk0, currSeq.unk1);
			setMario();
			unk50 |= 1;
		}
		if (!unk124)
			OSStartStopwatch(&unkE8);
		unk18[0]->onFlag(0x2);
		break;

	case 12:
		if (currSeq.unk0 == 1)
			THPPlayerStop();
	// !!!fallthrough!!!
	case 9: {
		gpApplication.mFader->startWipe(unkE4, 0.4f, 0.0f);
		if (unkE4 == 8)
			if (gpMSound->gateCheck(0x4859))
				MSoundSESystem::MSoundSE::startSoundSystemSE(0x4859, 0, nullptr,
				                                             0);
		MSound* sound = gpMSound;
		sound->fadeOutAllSound(SMSGetVSyncTimesPerSec() * 0.4f);
		SMSRumbleMgr->reset();
		for (int i = 0; i < 4; ++i)
			JUTGamePad::CRumble::stopMotor(unk18[i]->mPortNum);
		break;
	}

	case 5:
		if (currSeq.unk0 == 1)
			THPPlayerPause();
		SMSRumbleMgr->startPause();
		unkAC->setDrawStart();
		for (int i = 0; i < 4; ++i)
			JUTGamePad::CRumble::stopMotor(unk18[i]->mPortNum);
		unk18[0]->onFlag(0x1);
		break;

	case 10:
		if (currSeq.unk0 == 1)
			THPPlayerPause();
		SMSRumbleMgr->startPause();
		for (int i = 0; i < 4; ++i)
			JUTGamePad::CRumble::stopMotor(unk18[i]->mPortNum);
		unk18[0]->onFlag(0x1);
		JDrama::TNameRefGen::search<JDrama::TViewObj>("Group 2D")->unkC.mValue
		    |= 0xB;
		JDrama::TNameRefGen::search<JDrama::TViewObj>("Guide")->unkC.mValue
		    &= ~0xB;
		if (gpMSound->gateCheck(0x4817))
			MSoundSESystem::MSoundSE::startSoundSystemSE(0x4817, 0, nullptr, 0);
		gpApplication.mFader->startWipe(6, 1.0f, 0.0f);
		unk78->setup(nullptr);
		unk78->startMoveCursor();
		break;

	case 11:
		if (currSeq.unk0 == 1)
			THPPlayerPause();
		SMSRumbleMgr->startPause();
		unkAC->unk118->init(unk261);
		for (int i = 0; i < 4; ++i)
			JUTGamePad::CRumble::stopMotor(unk18[i]->mPortNum);
		unk18[0]->onFlag(0x1);
		break;

	case 7:
		gpMarDirector->mConsole->unk94->startAppearMiss();
		TFlagManager::smInstance->decFlag(0x20001, 1);
		unk60 = unk5C;
		gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 0xff));
		if (TFlagManager::smInstance->getFlag(0x20001) >= 0) {
			MSBgm::startBGM(0x8001000c);
			if (unk4E & 8)
				gpApplication.mFader->startWipe(2, 0.0f, 2.0f);
			else
				gpApplication.mFader->startWipe(10, 0.0f, 2.2f);
		} else {
			MSBgm::startBGM(0x80010028);
			gpApplication.mFader->startWipe(0xD, 0.0f, 2.0f);
		}
		break;
	}
}

#pragma dont_inline on
u8 TMarDirector::updateGameMode() { }
#pragma dont_inline off

void TMarDirector::moveStage()
{
	unkB4 = 5;
	unkE4 = 15;
	gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 0xff));

	u8 sVar4 = SMS_getShineStage(gpApplication.mNextArea.unk0);
	u8 sVar5 = SMS_getShineStage(gpApplication.mCurrArea.unk0);
	if (sVar4 != sVar5)
		TFlagManager::smInstance->setFlag(0x40002, 0);

	TGameSequence& nextArea = gpApplication.mNextArea;

	if (nextArea.unk1 == 0xff)
		switch (nextArea.unk0) {
		case 1:
			unkE4         = 2;
			nextArea.unk1 = decideNextScenario(nextArea.unk0);
			TFlagManager::smInstance->setFlag(0x40003, 0);
			break;

		case 13: {
			unkE4     = 2;
			u32 thing = 0;
			switch (TFlagManager::smInstance->getFlag(0x40003)) {
			case 0:
				thing = 0;
				break;
			case 2:
				thing = 1;
				break;
			case 4:
				thing = 2;
				break;
			case 5:
				thing = 3;
				break;
			case 6:
				thing = 4;
				break;
			case 7:
				thing = 5;
				break;
			}
			nextArea.unk1 = thing;
			break;
		}

		case 0x3A: {
			unkE4     = 2;
			u32 thing = 0;
			switch (TFlagManager::smInstance->getFlag(0x40003)) {
			case 0:
				thing = 1;
				break;
			case 7:
				thing = 0;
				break;
			}
			nextArea.unk1 = thing;
			break;
		}

		case 7: {
			unkE4     = 2;
			u32 thing = 0;
			switch (TFlagManager::smInstance->getFlag(0x40003)) {
			case 1:
				thing = 0;
				break;
			case 2:
				thing = 1;
				break;
			case 3:
			case 4:
				thing = 2;
				break;
			case 6:
				thing = 3;
				break;
			case 7:
				thing = 4;
				break;
			}
			nextArea.unk1 = thing;
			break;
		}

		case 14: {
			unkE4     = 2;
			u32 thing = 0;
			switch (TFlagManager::smInstance->getFlag(0x40003)) {
			case 3:
				thing = 0;
				break;
			case 4:
				thing = 1;
				break;
			}
			nextArea.unk1 = thing;
			break;
		}

		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 8:
			unkE4 = 2;
			unkB4 = 2;
			break;

		case 9:
			gpApplication.mFader->setColor(
			    JUtility::TColor(0xD2, 0xD2, 0xD2, 0xFF));
			unkB4 = 8;
			break;

		case 0x34:
			unkE4         = 8;
			nextArea.unk1 = 0;
			TFlagManager::smInstance->setFlag(0x40003, 0);
			break;

		case 0:
			nextArea.unk1 = 0;
			TFlagManager::smInstance->setFlag(0x40003, 0);
			break;

		default:
			unkE4         = 2;
			nextArea.unk1 = 0;
			break;
		}

	if (nextArea.unk1 != 0xff) {
		if (unk4C & 0x100) {
			unkE4 = 15;
			gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 0xff));
			unkB4 = 6;
		} else {
			unkB4 = 5;
		}
	}

	if (gpMarioOriginal->unk118 & 0x8000) {
		// TODO: water gun
		// u32 uVar1 = gpMarioOriginal->waterGun->unk1C85;
		TFlagManager::smInstance->setFlag(0x40004, 0);
	}
}

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
