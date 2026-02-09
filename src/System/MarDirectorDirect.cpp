#include <System/MarDirector.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JDrama/JDRCamera.hpp>
#include <System/Application.hpp>
#include <System/MSoundMainSide.hpp>
#include <System/MarioGamePad.hpp>
#include <System/PerformList.hpp>
#include <System/StageUtil.hpp>
#include <System/FlagManager.hpp>
#include <System/CardManager.hpp>
#include <Player/MarioMain.hpp>
#include <Player/Watergun.hpp>
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
#include <GC2D/SunGlass.hpp>
#include <GC2D/Talk2D2.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/MarioPositionObj.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/MapObjDolpic.hpp>
#include <NPC/NpcBase.hpp>
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

	int stage = SMS_getShineStage(gpApplication.mCurrArea.getStage());
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
	gpApplication.setNextArea(local_3C);
}

static void decideNextStageOfMiss() { }

static void checkDefeatShadowMarioAll() { }

// fabricated
static inline bool decideSomething()
{
	static u8 stages[] = { 0x6, 0x10, 0x1A, 0x24, 0x2E, 0x38, 0x42 };

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
				unk4E &= ~0x4;
			}
		} else {
			TGameSequence& curArea = gpApplication.mCurrArea;
			TConsoleStr* str       = mConsole->unk94;
			f32 iVar2              = gpCamera->getRestDemoFrames() / 120.0f;
			if (iVar2 <= str->getWipeCloseTime()
			    || ((curArea.getStage() != 1 || curArea.getScenario() != 1)
			        && (curArea.getStage() != 1 || curArea.getScenario() != 9)
			        && (unk18[0]->mEnabledFrameMeaning & 0x61))) {
				unk4E |= 4;
				mConsole->unk94->startCloseWipe((unk50 & 8) != 0);
				unk50 &= ~0x8;
			}
		}
		break;

	case 3:
		if (gpApplication.mCurrArea.unk0 == 1) {
			if (mConsole->unk94->unk2BC == 6)
				nextState = 2;
		} else if (mConsole->unk94->unk2BC == 6
		           && !gpMarioOriginal->checkActionThing()) {
			nextState = 4;
		}
		break;

	case 2:
		if (!unkE0->unk26 && !gpMarioOriginal->checkActionThing())
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
			unkE4     = 2;
			nextState = 9;
			break;
		}
		break;

	case 10:
		if (unk78->unkC4 && gpApplication.mFader->mFadeStatus == 1)
			nextState = 4;
		break;

	case 11: {
		switch (unkAC->unk118->getNextState()) {
		case 0:
			if (unk261 == 7) {
				TFlagManager::smInstance->restore();
				TFlagManager::smInstance->setBool(true, 0x30001);
				if (!TFlagManager::smInstance->getFlag(0x40000)) {
					gpApplication.mNextArea.set(0, 0, 0);
				} else {
					gpApplication.mNextArea.set(1, 0xff, 0);
				}
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
		    && (MSBgm::getHandle(2) == 0 || unk5C - unk60 >= 1200)) {
			if (TFlagManager::smInstance->getFlag(0x20001) >= 0) {
				TFlagManager::smInstance->setBool(true, 0x30002);
				const TGameSequence& curArea = gpApplication.mCurrArea;
				if (SMS_isExMap() || curArea.getStage() == 0
				    || curArea.getStage() == 60) {
					gpApplication.mNextArea.set(curArea.getStage(), 0, 0);
				} else {
					decideNextStage();
				}
				unk4C &= ~0x100;
				moveStage();
				unkE4 = 0xf;
				gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 0xff));
				nextState = 12;
			} else {
				gpApplication.mFader->startWipe(0xE, 0.3f, 0.0f);
				gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 0xff));
				unk261    = 7;
				nextState = 11;
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

	if (unk18[0]->isSomethingPushed() && gpCardManager->getLastStatus() != -1
	    && (unk4C & 0x4000) && !(unk50 & 0x10)) {
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
		JDrama::TNameRefGen::search<JDrama::TViewObj>("Group 2D")
		    ->unkC.off(0xB);
		JDrama::TNameRefGen::search<JDrama::TViewObj>("Guide")->unkC.on(0xB);

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

		JDrama::TNameRefGen::search<JDrama::TViewObj>("Group 2D")
		    ->unkC.off(0xB);
		JDrama::TNameRefGen::search<JDrama::TViewObj>("Guide")->unkC.on(0xB);

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

void TMarDirector::setMario()
{
	bool cVar4 = false;
	if (TFlagManager::getInstance()->getBool(0x30006)) {
		TFlagManager::getInstance()->setBool(false, 0x30006);
		cVar4 = true;
	}

	u8 uVar10 = unkD0;

	TMarioPositionObj* marioSetPosition
	    = JDrama::TNameRefGen::search<TMarioPositionObj>("マリオセット位置");
	if (!marioSetPosition || marioSetPosition->unkD0 == 0)
		uVar10 = 0;

	switch (unkD1) {
	case 1: {
		f32 fVar1 = 0.0f;

		const JGeometry::TVec3<f32>* pos = nullptr;

		if (uVar10) {
			fVar1 = marioSetPosition->getUnk70(uVar10 - 1).y;
			pos   = &marioSetPosition->getUnk10(uVar10 - 1);
		}
		gpMarioOriginal->rollingStart(pos, fVar1);
	} break;

	case 2: {
		int iVar9 = 0;
		switch (SMS_getShineStage(gpApplication.mPrevArea.getStage())) {
		case 5:
		case 6:
		case 7:
			iVar9 = 1;
			break;
		case 8:
			iVar9 = 2;
			break;
		}
		f32 fVar1 = 0.0f;

		const JGeometry::TVec3<f32>* pos = nullptr;

		if (uVar10) {
			fVar1 = marioSetPosition->getUnk70(uVar10 - 1).y;
			pos   = &marioSetPosition->getUnk10(uVar10 - 1);
		}
		gpMarioOriginal->returnStart(pos, fVar1, cVar4, iVar9);
	} break;

	case 4:
		gpMarioOriginal->toroccoStart();
		break;

	case 3:
		const JGeometry::TVec3<f32>* pos = nullptr;
		if (uVar10)
			pos = &marioSetPosition->getUnk10(uVar10 - 1);
		gpMarioOriginal->waitingStart(pos, 0.0f);
		break;
	}

	switch (gpApplication.mCurrArea.getStage()) {
	case 0x3C:
		gpMarioOriginal->mWaterGun->changeNozzle(1, true);
		break;

		// TODO: crazy cases
	case 0:
	case 7:
		gpMarioOriginal->mWaterGun->changeNozzle(
		    TFlagManager::getInstance()->getFlag(0x40004), true);
		gpMarioOriginal->mWaterGun->changeNozzle(0, true);
		break;
	}

	u32 uVar6 = SMS_getShineIDofExStage(gpApplication.mCurrArea.getStage());
	if (uVar6 != 0xff && TFlagManager::getInstance()->getShineFlag(uVar6) == 0)
		gpMarioOriginal->unk118 &= ~0x8000;
}

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
			mConsole->unkC.off(0xB);
		break;

	case 4:
		if (mState <= 3 && mMap != 0xf)
			mConsole->unkC.off(0xB);
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
		JDrama::TNameRefGen::search<JDrama::TViewObj>("Group 2D")->unkC.on(0xB);
		JDrama::TNameRefGen::search<JDrama::TViewObj>("Guide")->unkC.off(0xB);
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

u8 TMarDirector::updateGameMode()
{
	u8 r29 = mState;

	switch (unk124) {
	case 0:
		if (!(unk4C & 0x1FFF)) {
			if (SMS_CheckMarioFlag(0x400)) {
				unk4C |= 0x20;
				break;
			}

			if (mMap != 15) {
				if (unk18[0]->mButton.mTrigger & 0x10) {
					r29 = 10;
					break;
				}

				if (unk18[0]->mEnabledFrameMeaning & 0x1) {
					if (gpMarioOriginal->checkActionThing3()) {
						r29 = 5;
						break;
					}

					if (gpMSound->gateCheck(0x483D))
						MSoundSESystem::MSoundSE::startSoundSystemSE(
						    0x483D, 0, nullptr, 0);
				}
			}
		} else {
			if (unk4C & 0x20) {
				unk4C &= ~0x20;
				r29 = 7;
				TFlagManager::getInstance()->setFlag(0x40002, 0);
				break;
			}

			if (unk4C & 0x1) {
				unk4C &= ~0x1;
				unk126 = 3;

				TGCConsole2* console = gpMarDirector->mConsole;
				console->unk94->startAppearShineGet();
				console->unk34[19] = 1;
				MSBgm::startBGM(0x8001000a);
				TFlagManager::getInstance()->setBool(true, 0x30006);
				TFlagManager::getInstance()->setShineFlag(unk25C->unk134);
				f32 fVar3 = unkDC->mRate;
				unkDC->registFadeout(fVar3 * 1.0f, fVar3 * 5.3333333f);
				unk4C |= 0x8202;
				unk261 = 6;
				decideNextStage();
				break;
			}

			if (unk4C & 0x40) {
				unk126 = 3;
				break;
			}

			if (unk4C & 0x200) {
				unk4C &= ~0x200;
				r29 = 11;
				break;
			}

			if (unk4C & 0x8) {
				unk4C &= ~0x8;
				unk4C |= 0x2;
				unk126 = 3;
				if (gpApplication.mNextArea.getStage() == 5) {
					fireStartDemoCamera("hodai_dpt_pinna1", nullptr, -1, 0.0f,
					                    false, nullptr, 0, nullptr, 0);
					if (unk254 != nullptr)
						unk254->startDemo();
					break;
				}

				if (gpApplication.mNextArea.getStage() == 6) {
					fireStartDemoCamera("camera_sirena_gate_in", nullptr, -1,
					                    0.0f, false, nullptr, 0, nullptr, 0);
					break;
				}

				if (gpApplication.mNextArea.getStage() == 8) {
					fireStartDemoCamera("camera_monte_gate_in", nullptr, -1,
					                    0.0f, false, nullptr, 0, nullptr, 0);
					break;
				}

				break;
			}

			if (unk4C & 0x4) {
				unk4C &= ~0x4;
				unk4C |= 0x2;
				unk126 = 3;
				fireStartDemoCamera(nullptr, nullptr, -1, 0.0f, false, nullptr,
				                    0, unk250, 0);
				break;
			}

			if (unk4C & 0x2) {
				moveStage();
				r29 = 9;
				break;
			}
		}
		break;

	case 2:
		if (unk4C & 0x40) {
			unk126 = 4;
		} else {
			if (unkB0->unk248 == 0)
				unk126 = 0;
		}
		break;

	case 4: {
		bool bVar5  = false;
		bool uVar15 = 0;
		if (unk4C & 0x80) {
			uVar15 = 1;
			bVar5  = true;
			unk4C &= ~0x80;
		} else {
			if (!gpCamera->getRestDemoFrames()) {
				if (!MSBgm::getHandle(2) || unk5C - unk60 >= 1200) {
					bVar5  = true;
					uVar15 = unk12C[unk24D].unk10;
				}
			}
		}

		if (bVar5) {
			u32 prev = unk24D++;
			unk24D &= 0x7;
			TDemoInfo* info = &unk12C[prev];
			if (unk24D != unk24C) {
				gpCamera->endDemoCamera();
				if (info->unk14 != nullptr)
					(*info->unk14)(info->unk18, 1);

				gpCamera->startDemoCamera(info->unk0, info->unk4, info->unk8,
				                          info->unkC, info->unk10);
				if (info->unk14 != nullptr)
					(*info->unk14)(info->unk18, 0);
			} else {
				unk4C &= ~0x40;
				unk126 = unk124 == 4 ? 2 : 0;
				if (uVar15 != 0)
					gpCamera->endDemoCamera();
				if (unk12C[prev].unk14 != nullptr)
					(*unk12C[prev].unk14)(unk12C[prev].unk18, 1);
			}
		}
	} break;
	}

	if (unk24D == unk24C)
		unk4C &= ~0x80;

	unk125 = unk124;

	if (unk124 != unk126) {
		switch (unk124) {
		case 2:
			if (unk126 == 0) {
				unkA0 = 0;
				unkA4 = 0;
				unk18[0]->mFlags &= ~0x2;
				OSStartStopwatch(&unkE8);
			}
			break;

		case 3:
		case 4:
			if (unk124 == 4)
				MSMainProc::fromTalkingCameraDemo(unk124 == 4);
			else
				MSMainProc::fromInnerCameraDemo();
			unk18[0]->mFlags &= ~0x80;
			OSStartStopwatch(&unkE8);
			break;
		}

		switch (unk126) {
		case 0:
			break;

		case 1:
			unkA0->onLiveFlag(LIVE_FLAG_UNK40000);
			unkA0->unkC.off(0x3);
			unk18[0]->mFlags |= 0x8;
			OSStopStopwatch(&unkE8);
			break;

		case 2:
			if (unk124 == 1)
				unkB0->openTalkWindow(unkA0);
			break;

		case 3:
		case 4:
			if (unk124 == 4)
				MSMainProc::toTalkingCameraDemo();
			else
				MSMainProc::toInnerCameraDemo();
			unk18[0]->mFlags |= 0x10;
			if (unk12C[unk24D].unk20.mValue == 1) {
				gpCamera->startGateDemoCamera(unk12C[unk24D].unk1C);
			} else {
				TDemoInfo* info = &unk12C[unk24D];
				gpCamera->startDemoCamera(info->unk0, info->unk4, info->unk8,
				                          info->unkC, info->unk10);
				if (info->unk14 != nullptr)
					(*info->unk14)(info->unk18, 0);
			}
			OSStopStopwatch(&unkE8);
			unk60 = unk5C;
			break;
		}

		unk124 = unk126;
	}

	if (unk128 & 0x1) {
		unk128 &= ~0x1;
		unk128 |= 0x2;
	} else {
		unk128 &= ~0x2;
	}

	return r29;
}

void TMarDirector::moveStage()
{
	unkB4 = 5;
	unkE4 = 15;
	gpApplication.mFader->setColor(JUtility::TColor(0, 0, 0, 0xff));

	u8 sVar4 = SMS_getShineStage(gpApplication.mNextArea.getStage());
	u8 sVar5 = SMS_getShineStage(gpApplication.mCurrArea.getStage());
	if (sVar4 != sVar5)
		TFlagManager::smInstance->setFlag(0x40002, 0);

	TGameSequence& nextArea = gpApplication.mNextArea;

	if (nextArea.unk1 == 0xff)
		switch (nextArea.unk0) {
		case 1:
			unkE4         = 2;
			nextArea.unk1 = decideNextScenario(nextArea.getStage());
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

	if (gpMarioOriginal->checkFlag(0x8000)) {
		u32 r5 = 0;
		if ((int)gpMarioOriginal->mWaterGun->mSecondNozzle == 3)
			r5 = 4;
		TFlagManager::smInstance->setFlag(0x40004, r5);
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
