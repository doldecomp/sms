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
#include <Player/Mario.hpp>
#include <Player/WaterGun.hpp>
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
	int dt = 600 / (int)SMSGetVSyncTimesPerSec();

	if (!mSetupDone) {
		if (!OSIsThreadTerminated(&gSetupThread))
			return 0;

		void* local_40;
		OSJoinThread(&gSetupThread, &local_40);
		if (local_40)
			return 4;

		setupObjects();
		mSetupDone = true;
	}

	u32 desiredAppState = TApplication::APP_STATE_DEFAULT;

	JDrama::TGraphics local_140;

	u8 prevSeGateMask = SMSGetMSound()->mSeGateMask;
	mPendingSimulationTime += dt;

	int i = 0;
	for (;;) {
		if (!checkFlag(DIRECTOR_FLAG_LAST_SIMULATION_TICK)) {
			++i;
			if (i == 1)
				onFlag(DIRECTOR_FLAG_FIRST_SIMULATION_TICK);
			mPendingSimulationTime -= 5;
			if (mPendingSimulationTime < 5)
				onFlag(DIRECTOR_FLAG_LAST_SIMULATION_TICK);

			// inline?
			u32 blacklistedCues = 0;
			u8 seGateMask       = prevSeGateMask;
			if (checkFlag(DIRECTOR_FLAG_LAST_SIMULATION_TICK)) {
				if (unk258)
					unk258->stageLoop();
			} else {
				blacklistedCues |= CUE_CALC_ANIM;
				seGateMask &= ~MSSeGate_Continuous;
			}
			SMSGetMSound()->mSeGateMask = seGateMask;

			switch (mState) {
			case STATE_PAUSE_MENU:
			case STATE_CARD_SAVE:
			case STATE_UNK12:
				blacklistedCues |= CUE_CALC_ANIM;
				blacklistedCues |= CUE_MOVE;
				break;

			case STATE_GUIDE:
				blacklistedCues |= CUE_CALC_ANIM;
				blacklistedCues |= CUE_MOVE;
				break;
			}

			if (!(blacklistedCues & CUE_MOVE))
				++mMoveTickCount;
			++mTickCount;
			if (checkFlag(DIRECTOR_FLAG_FIRST_SIMULATION_TICK)) {
				if (mState == STATE_UNK4 || mState == STATE_UNK7)
					SMSRumbleMgr->update();
			} else {
				for (int i = 0; i < 4; ++i) {
					TMarioGamePad* pad = unk18[i];
					pad->resetButtons();

					unk18[i]->updateMeaning();
					unk18[i]->offFlag(TMarioGamePad::PAD_FLAG_0x40);
				}
			}

			u32 tmp = 0;
			if (checkFlag(DIRECTOR_FLAG_FIRST_SIMULATION_TICK))
				tmp |= 1;
			if (checkFlag(DIRECTOR_FLAG_LAST_SIMULATION_TICK))
				tmp |= 2;
			local_140.unk2 = tmp;

			// inline
			bool bVar1 = true;
			if ((mMoveTickCount & 1) || (mMoveTickCount & 2))
				bVar1 = false;

			if (bVar1)
				gpObjHitCheck->checkActorsHit();
			else
				gpObjHitCheck->clearHitNum();

			u32 movementCue = ~blacklistedCues;
			if (mMoveTickCount & 1)
				movementCue &= ~CUE_MOVEMENT_GATE_A;
			if (mMoveTickCount & 2)
				movementCue &= ~CUE_MOVEMENT_GATE_B;
			if (checkDemoFlag(DEMO_FLAG_SHINE_GET_STOP_THE_WORLD))
				mShinePfLstMov->perform(movementCue, &local_140);
			else
				mPerformListMovement->perform(movementCue, &local_140);

			u32 someCue = 0;
			if (!checkFlag(DIRECTOR_FLAG_LAST_SIMULATION_TICK))
				someCue |= CUE_CALC_ANIM;
			u32 unk30Cue = ~someCue;
			unk30->perform(unk30Cue, &local_140);
			movement();
			if (!(blacklistedCues & CUE_CALC_ANIM)) {
				if (checkDemoFlag(DEMO_FLAG_SHINE_GET_STOP_THE_WORLD))
					mShinePfLstAnm->perform(~blacklistedCues, &local_140);
				else
					mPerformListCalcAnim->perform(~blacklistedCues, &local_140);
			}

			if (checkFlag(DIRECTOR_FLAG_LAST_SIMULATION_TICK)) {
				local_140.unk2 = 0;
				unk34->perform(CUE_ALL, &local_140);
				break;
			}
		} else {
			local_140.unk2 = 0;
			unk40->perform(CUE_ALL, &local_140);
			unk38->perform(CUE_ALL, &local_140);
			unk3C->perform(CUE_ALL, &local_140);
			mPerformListGX->perform(CUE_ALL, &local_140);
			if ((gpSilhouetteManager->unk48 > 0.0f ? true : false)
			    || gpCamera->unk2C8 != -1) {
				mPerformListSilhouette->perform(CUE_ALL, &local_140);
			}
			mPerformListGXPost->perform(CUE_ALL, &local_140);
			GXInvalidateTexAll();
		}
		desiredAppState = changeState();
		offFlag(DIRECTOR_FLAG_LAST_SIMULATION_TICK
		        | DIRECTOR_FLAG_FIRST_SIMULATION_TICK);
	}

	gpMSound->mSeGateMask = prevSeGateMask;
	return desiredAppState;
}

static void decideNextStage()
{
	TGameSequence local_3C;

	int stage = SMS_getShineStage(SMSGetApplication()->mCurrArea.getStage());
	switch (stage) {
	case 0:
		local_3C.set(1, 0xff, JDrama::TFlagT<u16>());
		break;
	case 1:
	case 2:
	default:
		local_3C.set(1, 0xff, JDrama::TFlagT<u16>());
		break;
	}
	SMSGetApplication()->setNextArea(local_3C);
}

static void decideNextStageOfMiss()
{
	const TGameSequence& curArea = SMSGetApplication()->mCurrArea;
	if (SMS_isExMap() || curArea.getStage() == 0 || curArea.getStage() == 60)
		SMSGetApplication()->mNextArea.set(curArea.getStage(), 0, 0);
	else
		decideNextStage();
}

static bool checkDefeatShadowMarioAll()
{
	static u8 stages[] = { 0x6, 0x10, 0x1A, 0x24, 0x2E, 0x38, 0x42 };

	for (int i = 0; i < 7; ++i)
		if (!TFlagManager::getInstance()->getShineFlag(stages[i]))
			return false;

	return true;
}

static int decideNextScenario(u8 param_1)
{
	int result = 0;
	switch (param_1) {
	case 1:
		if (TFlagManager::getInstance()->getBool(0x103AE)) {
			result = 2;
		} else if (checkDefeatShadowMarioAll()) {
			result = 9;
		} else if (TFlagManager::getInstance()->getBool(0x10389)) {
			result = 8;
		} else if (TFlagManager::getInstance()->getBool(0x10386)
		           && TFlagManager::getInstance()->getBool(0x10387)) {
			if (TFlagManager::getInstance()->getFlag(0x40000) >= 10)
				result = 7;
			else
				result = 6;
		} else if (TFlagManager::getInstance()->getBool(0x10385)) {
			result = 5;
		} else if (TFlagManager::getInstance()->getBool(0x10384)) {
			result = 1;
		} else {
			result = 0;
		}
		break;
	}
	return result;
}

int TMarDirector::changeState()
{
	int desiredAppState = TApplication::APP_STATE_DEFAULT;
	u8 nextState        = mState;
	switch (mState) {
	case STATE_UNK0:
		switch (SMSGetApplication()->mCurrArea.getStage()) {
		case 0xf:
			nextState = STATE_UNK4;
			onTransitionFlag(TRANSITION_FLAG_STAGE_BGM_STARTED);
			break;

		case 0x2:
		case 0x3:
		case 0x4:
		case 0x5:
		case 0x6:
		case 0x8:
		case 0x9:
		case 0x34:
			nextState = STATE_UNK1;
			onTransitionFlag(TRANSITION_FLAG_GO_BANNER_PENDING
			                 | TRANSITION_FLAG_SCENARIO_NAME_BANNER_PENDING);
			break;

		case 1:
			if (checkDemoFlag(DEMO_FLAG_CAMERA_DEMO_ON_START)) {
				nextState = STATE_UNK1;
				onTransitionFlag(
				    TRANSITION_FLAG_GO_BANNER_PENDING
				    | TRANSITION_FLAG_SCENARIO_NAME_BANNER_PENDING);
			} else {
				nextState = STATE_UNK2;
			}
			break;

		default:
			if (JKRGetResource("/scene/map/camera/startcamera.bck")) {
				nextState = STATE_UNK1;
				onTransitionFlag(TRANSITION_FLAG_GO_BANNER_PENDING
				                 | TRANSITION_FLAG_OPENING_WIPE_PENDING);
			} else {
				nextState = STATE_UNK4;
			}
			break;
		}
		break;

	case STATE_UNK1:
		if (checkDemoFlag(DEMO_FLAG_CAMERA_DEMO_WIPE_STARTED)) {
			if (getConsole()->unk94->unk2BC == 4) {
				nextState = STATE_UNK3;
				offDemoFlag(DEMO_FLAG_CAMERA_DEMO_WIPE_STARTED);
			}
		} else {
			TGameSequence& curArea = SMSGetApplication()->mCurrArea;
			TConsoleStr* str       = getConsole()->unk94;
			f32 iVar2              = gpCamera->getRestDemoFrames() / 120.0f;
			if (iVar2 <= str->getWipeCloseTime()
			    || ((curArea.getStage() != 1 || curArea.getScenario() != 1)
			        && (curArea.getStage() != 1 || curArea.getScenario() != 9)
			        && unk18[0]->checkFrameMeaning(
			            TMarioGamePad::MEANING_START
			            | TMarioGamePad::MEANING_MENU_A
			            | TMarioGamePad::MEANING_MENU_B))) {
				onDemoFlag(DEMO_FLAG_CAMERA_DEMO_WIPE_STARTED);
				getConsole()->unk94->startCloseWipe(
				    checkTransitionFlag(TRANSITION_FLAG_OPENING_WIPE_PENDING)
				    != FALSE);
				offTransitionFlag(TRANSITION_FLAG_OPENING_WIPE_PENDING);
			}
		}
		break;

	case STATE_UNK3:
		if (SMSGetApplication()->mCurrArea.getStage() == 1) {
			if (getConsole()->unk94->unk2BC == 6)
				nextState = STATE_UNK2;
		} else if (getConsole()->unk94->unk2BC == 6
		           && !gpMarioOriginal->checkStatusType(
		               MARIO_STATUS_FLAG_UNK1000)) {
			nextState = STATE_UNK4;
		}
		break;

	case STATE_UNK2:
		if (!unkE0->unk26
		    && !gpMarioOriginal->checkStatusType(MARIO_STATUS_FLAG_UNK1000))
			nextState = STATE_UNK4;
		break;

	case STATE_UNK4:
		nextState = updateGameMode();
		break;

	case STATE_PAUSE_MENU:
		switch (unkAC->getNextState()) {
		case 0:
			nextState = STATE_UNK4;
			break;
		case 1:
			unkE4     = 4;
			nextState = STATE_UNK12;
			unkB4     = TApplication::APP_STATE_DONE;
			break;
		case 5:
			decideNextStage();
			mFlags &= ~DIRECTOR_FLAG_MOVIE_PENDING;
			moveStage();
			unkE4     = 2;
			nextState = STATE_UNK9;
			break;
		}
		break;

	case STATE_GUIDE:
		if (unk78->unkC4 && SMSGetApplication()->getFader()->isFullyFadedIn())
			nextState = STATE_UNK4;
		break;

	case STATE_CARD_SAVE: {
		switch (unkAC->mCardSave->getNextState()) {
		case 0:
			if (unk261 == 7) {
				TFlagManager::getInstance()->restore();
				TFlagManager::getInstance()->setBool(true, 0x30001);
				if (!TFlagManager::getInstance()->getFlag(0x40000)) {
					SMSGetApplication()->mNextArea.set(0, 0, 0);
				} else {
					SMSGetApplication()->mNextArea.set(1, 0xff, 0);
				}
				mFlags &= ~DIRECTOR_FLAG_MOVIE_PENDING;
				moveStage();
				SMSGetApplication()->getFader()->setFadeStatus(
				    TSMSFader::FADE_STATUS_FULLY_FADED_OUT);
				desiredAppState = TApplication::APP_STATE_GAMEPLAY;
			} else {
				nextState = STATE_UNK4;
			}
			break;
		case 1:
			if (unk261 == 7) {
				SMSGetApplication()->getFader()->setFadeStatus(
				    TSMSFader::FADE_STATUS_FULLY_FADED_OUT);
				desiredAppState = TApplication::APP_STATE_DONE;
			} else {
				unkE4     = 4;
				nextState = STATE_UNK12;
				unkB4     = TApplication::APP_STATE_DONE;
			}
			break;
		}
		break;
	}

	case STATE_UNK7:
		if (SMSGetApplication()->getFader()->isFullyFadedOut()
		    && (MSBgm::getHandle(2) == 0 || mTickCount - unk60 >= 1200)) {
			if (TFlagManager::getInstance()->getFlag(0x20001) >= 0) {
				TFlagManager::getInstance()->setBool(true, 0x30002);
				decideNextStageOfMiss();
				mFlags &= ~DIRECTOR_FLAG_MOVIE_PENDING;
				moveStage();
				unkE4 = 0xf;
				SMSGetApplication()->getFader()->setColor(
				    JUtility::TColor(0, 0, 0, 0xff));
				nextState = STATE_UNK12;
			} else {
				SMSGetApplication()->getFader()->startWipe(0xE, 0.3f, 0.0f);
				SMSGetApplication()->getFader()->setColor(
				    JUtility::TColor(0, 0, 0, 0xff));
				unk261    = 7;
				nextState = STATE_CARD_SAVE;
				getConsole()->startDisappearStar();
				getConsole()->startDisappearCoin();
			}
		}
		break;

	case STATE_UNK9:
	case STATE_UNK12:
		if (SMSGetApplication()->getFader()->isFullyFadedOut()
		    && gpMSound->checkWaveOnAram(MS_WAVE_DEFAULT))
			desiredAppState = unkB4;
		break;
	}

	if (unk18[0]->isSomethingPushed()
	    && gpCardManager->getLastStatus() != CARD_RESULT_BUSY
	    && (mFlags & DIRECTOR_FLAG_LAST_SIMULATION_TICK)
	    && !checkTransitionFlag(TRANSITION_FLAG_RESET_HANDLED)) {
		nextState = STATE_UNK12;
		onTransitionFlag(TRANSITION_FLAG_RESET_HANDLED);
		unkE4 = 4;
		unkB4 = TApplication::APP_STATE_DONE;
	}

	if (nextState != mState) {
		currentStateFinalize(nextState);
		nextStateInitialize(nextState);
		mState = nextState;
	}

	return desiredAppState;
}

void TMarDirector::currentStateFinalize(u8 next_state)
{
	switch (mState) {
	case STATE_UNK0:
		static_cast<JDrama::TViewObj*>(JDrama::TNameRefGen::search("Group 2D"))
		    ->unkC.off(CUE_MOVE | CUE_CALC_ANIM | CUE_DRAW);
		static_cast<JDrama::TViewObj*>(JDrama::TNameRefGen::search("Guide"))
		    ->unkC.on(CUE_MOVE | CUE_CALC_ANIM | CUE_DRAW);

		SMSGetApplication()->getFader()->startWipe(unkE4, 0.4f, 0.0f);
		SMSRumbleMgr->reset();
		if (SMSGetApplication()->mCurrArea.getStage() == 1)
			THPPlayerPlay();
		break;

	case STATE_UNK1:
		unk18[0]->offFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
		gpCamera->endDemoCamera();
		getConsole()->unk94->startOpenWipe();
		MSMainProc::endStageEntranceDemo(
		    SMSGetApplication()->mCurrArea.getStage(),
		    SMSGetApplication()->mCurrArea.getScenario());
		break;

	case STATE_UNK4:
		if (unk124 == 0)
			OSStopStopwatch(&unkE8);
		unk18[0]->offFlag(TMarioGamePad::PAD_FLAG_GAME_INPUT);
		break;

	case STATE_PAUSE_MENU:
		unk18[0]->offFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
		SMSRumbleMgr->finishPause();
		if (SMSGetApplication()->mCurrArea.getStage() == 1)
			THPPlayerPlay();
		break;

	case STATE_GUIDE:
		unk18[0]->offFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
		SMSRumbleMgr->finishPause();

		static_cast<JDrama::TViewObj*>(JDrama::TNameRefGen::search("Group 2D"))
		    ->unkC.off(CUE_MOVE | CUE_CALC_ANIM | CUE_DRAW);
		static_cast<JDrama::TViewObj*>(JDrama::TNameRefGen::search("Guide"))
		    ->unkC.on(CUE_MOVE | CUE_CALC_ANIM | CUE_DRAW);

		SMSSwitch2DArchive("guide", gArBkConsole);
		if (SMSGetApplication()->mCurrArea.getStage() == 1)
			THPPlayerPlay();
		break;

	case STATE_CARD_SAVE:
		unk18[0]->offFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
		SMSRumbleMgr->finishPause();
		if (SMSGetApplication()->mCurrArea.getStage() == 1)
			THPPlayerPlay();
		switch (unk261) {
		case 3:
			getConsole()->startAppearBalloon(0xE0048, true);
			break;

		case 4:
			getConsole()->startAppearBalloon(0xE0049, true);
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

	TMarioPositionObj* marioSetPosition = static_cast<TMarioPositionObj*>(
	    JDrama::TNameRefGen::search("マリオセット位置"));
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
		switch (SMS_getShineStage(SMSGetApplication()->mPrevArea.getStage())) {
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

	case 0:
	default: {
		const JGeometry::TVec3<f32>* pos = nullptr;
		if (uVar10)
			pos = &marioSetPosition->getUnk10(uVar10 - 1);
		gpMarioOriginal->waitingStart(pos, 0.0f);
	} break;
	}

	TMario* mario                = gpMarioOriginal;
	const TGameSequence& curArea = SMSGetApplication()->mCurrArea;
	if (mario->checkFlag(MARIO_FLAG_HAS_FLUDD)) {
		switch (curArea.getStage()) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 8:
		case 9:
		case 0x10:
		case 0x2C:
		case 0x34:
		case 0x39:
			break;

		case 0x3C:
			mario->mWaterGun->changeNozzle(TWaterGun::Rocket, true);
			break;

		default: {
			TWaterGun* gun = mario->mWaterGun;
			gun->changeNozzle(
			    (TWaterGun::TNozzleType)TFlagManager::getInstance()->getFlag(
			        0x40004),
			    true);
			gpMarioOriginal->mWaterGun->changeNozzle(TWaterGun::Spray, true);
		} break;
		}
	}

	u8 uVar6 = SMS_getShineIDofExStage(curArea.getStage());
	if (uVar6 != 0xff && TFlagManager::getInstance()->getShineFlag(uVar6) == 0)
		gpMarioOriginal->offFlag(MARIO_FLAG_HAS_FLUDD);
}

void TMarDirector::nextStateInitialize(u8 next_state)
{
	TGameSequence& currSeq = SMSGetApplication()->mCurrArea;

	switch (next_state) {
	case STATE_UNK1: {
		const char* demoName = "startcamera";
		unk18[0]->onFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
		unk68 = 0;
		if (currSeq.getStage() == 1
		    && checkDemoFlag(DEMO_FLAG_CAMERA_DEMO_ON_START)) {
			if (currSeq.getScenario() == 8) {
				switch (TFlagManager::getInstance()->getFlag(0x60003)) {
				case 0:
					if (TFlagManager::getInstance()->getFlag(0x40000) >= 0x14)
						demoName = "mareopen_startcamera";
					break;
				case 1:
					demoName = "yoshi_startcamera";
					break;
				case 2:
					demoName = "turbo_startcamera";
					break;
				case 3:
					demoName = "rocket_startcamera";
					break;
				}
			} else {
				if (TFlagManager::getInstance()->getBool(0x50001)) {
					demoName = "sinkricco";
				} else {
					if (TFlagManager::getInstance()->getBool(0x50002))
						demoName = "sinkmamma";
				}
			}
		}
		gpCamera->startDemoCamera(demoName, nullptr, -1, 0.0f, true);
		if (checkTransitionFlag(TRANSITION_FLAG_SCENARIO_NAME_BANNER_PENDING)) {
			getConsole()->unk94->startAppearScenario();
			offTransitionFlag(TRANSITION_FLAG_SCENARIO_NAME_BANNER_PENDING);
		}
		MSMainProc::startStageEntranceDemo(currSeq.getStage(),
		                                   currSeq.getScenario());
		break;
	}

	case STATE_UNK3:
		unk68 = 0;
		if (!checkTransitionFlag(TRANSITION_FLAG_STAGE_BGM_STARTED)) {
			MSMainProc::startStageBGM(currSeq.getStage(),
			                          currSeq.getScenario());
			setMario();
			onTransitionFlag(TRANSITION_FLAG_STAGE_BGM_STARTED);
		}
		break;

	case STATE_UNK2:
		if (!checkTransitionFlag(TRANSITION_FLAG_STAGE_BGM_STARTED)) {
			MSMainProc::startStageBGM(currSeq.getStage(),
			                          currSeq.getScenario());
			setMario();
			onTransitionFlag(TRANSITION_FLAG_STAGE_BGM_STARTED);
		}
		if (mMap != 0xf)
			getConsole()->unkC.off(CUE_MOVE | CUE_CALC_ANIM | CUE_DRAW);
		break;

	case STATE_UNK4:
		if (mState <= STATE_UNK3 && mMap != 0xf)
			getConsole()->unkC.off(CUE_MOVE | CUE_CALC_ANIM | CUE_DRAW);
		if (checkTransitionFlag(TRANSITION_FLAG_GO_BANNER_PENDING)) {
			getConsole()->unk94->startAppearGo();
			offTransitionFlag(TRANSITION_FLAG_GO_BANNER_PENDING);
		}
		if (!checkTransitionFlag(TRANSITION_FLAG_STAGE_BGM_STARTED)) {
			MSMainProc::startStageBGM(currSeq.getStage(),
			                          currSeq.getScenario());
			setMario();
			onTransitionFlag(TRANSITION_FLAG_STAGE_BGM_STARTED);
		}
		if (!unk124)
			OSStartStopwatch(&unkE8);
		unk18[0]->onFlag(TMarioGamePad::PAD_FLAG_GAME_INPUT);
		break;

	case STATE_UNK12:
		if (currSeq.getStage() == 1)
			THPPlayerStop();
	// !!!fallthrough!!!
	case STATE_UNK9: {
		SMSGetApplication()->getFader()->startWipe(unkE4, 0.4f, 0.0f);
		if (unkE4 == 8)
			SMSGetMSound()->startSoundSystemSE(MSD_SE_MA_INTO_DOKAN, 0, nullptr,
			                                   0);
		MSound* sound = gpMSound;
		sound->fadeOutAllSound(SMSGetVSyncTimesPerSec() * 0.4f);
		SMSRumbleMgr->reset();
		for (int i = 0; i < 4; ++i)
			JUTGamePad::CRumble::stopMotor(unk18[i]->mPortNum);
		break;
	}

	case STATE_PAUSE_MENU:
		if (currSeq.getStage() == 1)
			THPPlayerPause();
		SMSRumbleMgr->startPause();
		unkAC->setDrawStart();
		for (int i = 0; i < 4; ++i)
			JUTGamePad::CRumble::stopMotor(unk18[i]->mPortNum);
		unk18[0]->onFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
		break;

	case STATE_GUIDE:
		if (currSeq.getStage() == 1)
			THPPlayerPause();
		SMSRumbleMgr->startPause();
		for (int i = 0; i < 4; ++i)
			JUTGamePad::CRumble::stopMotor(unk18[i]->mPortNum);
		unk18[0]->onFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
		static_cast<JDrama::TViewObj*>(JDrama::TNameRefGen::search("Group 2D"))
		    ->unkC.on(CUE_MOVE | CUE_CALC_ANIM | CUE_DRAW);
		static_cast<JDrama::TViewObj*>(JDrama::TNameRefGen::search("Guide"))
		    ->unkC.off(CUE_MOVE | CUE_CALC_ANIM | CUE_DRAW);
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_WIPE_IN, 0, nullptr, 0);
		SMSGetApplication()->getFader()->startWipe(6, 1.0f, 0.0f);
		unk78->setup(nullptr);
		unk78->startMoveCursor();
		break;

	case STATE_CARD_SAVE:
		if (currSeq.getStage() == 1)
			THPPlayerPause();
		SMSRumbleMgr->startPause();
		unkAC->mCardSave->init(unk261);
		for (int i = 0; i < 4; ++i)
			JUTGamePad::CRumble::stopMotor(unk18[i]->mPortNum);
		unk18[0]->onFlag(TMarioGamePad::PAD_FLAG_MENU_INPUT);
		break;

	case STATE_UNK7:
		SMSGetMarDirector()->getConsole()->unk94->startAppearMiss();
		TFlagManager::getInstance()->decFlag(0x20001, 1);
		unk60 = mTickCount;
		SMSGetApplication()->getFader()->setColor(
		    JUtility::TColor(0, 0, 0, 0xff));
		if (TFlagManager::getInstance()->getFlag(0x20001) >= 0) {
			MSBgm::startBGM(MSD_BGM_BOSS);
			if (checkDemoFlag(DEMO_FLAG_HELL_DEAD))
				SMSGetApplication()->getFader()->startWipe(2, 0.0f, 2.0f);
			else
				SMSGetApplication()->getFader()->startWipe(10, 0.0f, 2.2f);
		} else {
			MSBgm::startBGM(MSD_BGM_BOSSHANA_2ND3RD);
			SMSGetApplication()->getFader()->startWipe(0xD, 0.0f, 2.0f);
		}
		break;
	}
}

u8 TMarDirector::updateGameMode()
{
	u8 r29 = mState;

	switch (unk124) {
	case 0:
		if (!checkFlag(~(DIRECTOR_FLAG_SHINE_TAKEN
		                 | DIRECTOR_FLAG_LAST_SIMULATION_TICK
		                 | DIRECTOR_FLAG_FIRST_SIMULATION_TICK))) {
			if (SMS_CheckMarioFlag(MARIO_FLAG_GAME_OVER)) {
				mFlags |= DIRECTOR_FLAG_GAME_OVER_PENDING;
				break;
			}

			if (mMap != 15) {
				if (unk18[0]->testTrigger(0x10)) {
					r29 = STATE_GUIDE;
					break;
				}

				if (unk18[0]->checkFrameMeaning(TMarioGamePad::MEANING_START)) {
					if (gpMarioOriginal->checkActionThing3()) {
						r29 = STATE_PAUSE_MENU;
						break;
					}

					SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_NOT_COLLECT, 0,
					                                   nullptr, 0);
				}
			}
		} else {
			if (checkFlag(DIRECTOR_FLAG_GAME_OVER_PENDING)) {
				offFlag(DIRECTOR_FLAG_GAME_OVER_PENDING);
				r29 = STATE_UNK7;
				TFlagManager::getInstance()->setFlag(0x40002, 0);
				break;
			}

			if (checkFlag(DIRECTOR_FLAG_SHINE_GET_PENDING)) {
				offFlag(DIRECTOR_FLAG_SHINE_GET_PENDING);
				unk126 = 3;

				TGCConsole2* console = gpMarDirector->mConsole;
				console->unk94->startAppearShineGet();
				console->unk47 = 1;
				MSBgm::startBGM(MSD_BGM_CHUBOSS);
				TFlagManager::getInstance()->setBool(true, 0x30006);
				TFlagManager::getInstance()->setShineFlag(unk25C->getEventId());
				f32 fVar3     = unkDC->mRate;
				f32 fadeInSec = 1.0f;
				unkDC->registFadeout(fadeInSec * fVar3, fVar3 * 5.3333333f);
				onFlag(DIRECTOR_FLAG_SHINE_TAKEN
				       | DIRECTOR_FLAG_CARD_SAVE_PENDING
				       | DIRECTOR_FLAG_STAGE_TRANSITION_PENDING);
				unk261 = 6;
				decideNextStage();
				break;
			}

			if (checkFlag(DIRECTOR_FLAG_DEMO_PENDING)) {
				unk126 = 3;
				break;
			}

			if (checkFlag(DIRECTOR_FLAG_CARD_SAVE_PENDING)) {
				offFlag(DIRECTOR_FLAG_CARD_SAVE_PENDING);
				r29 = STATE_CARD_SAVE;
				break;
			}

			if (checkFlag(DIRECTOR_FLAG_GATE_DEMO_STAGE_TRANSITION_PENDING)) {
				offFlag(DIRECTOR_FLAG_GATE_DEMO_STAGE_TRANSITION_PENDING);
				onFlag(DIRECTOR_FLAG_STAGE_TRANSITION_PENDING);
				unk126                        = 3;
				const TGameSequence& nextArea = SMSGetApplication()->mNextArea;
				if (nextArea.getStage() == 5) {
					fireStartDemoCamera("hodai_dpt_pinna1", nullptr, -1, 0.0f,
					                    false, nullptr, 0, nullptr, 0);
					if (unk254 != nullptr)
						unk254->startDemo();
					break;
				}

				if (nextArea.getStage() == 6) {
					fireStartDemoCamera("camera_sirena_gate_in", nullptr, -1,
					                    0.0f, false, nullptr, 0, nullptr, 0);
					break;
				}

				if (nextArea.getStage() == 8) {
					fireStartDemoCamera("camera_monte_gate_in", nullptr, -1,
					                    0.0f, false, nullptr, 0, nullptr, 0);
					break;
				}

				break;
			}

			if (checkFlag(DIRECTOR_FLAG_ACTOR_DEMO_STAGE_TRANSITION_PENDING)) {
				offFlag(DIRECTOR_FLAG_ACTOR_DEMO_STAGE_TRANSITION_PENDING);
				onFlag(DIRECTOR_FLAG_STAGE_TRANSITION_PENDING);
				unk126 = 3;
				fireStartDemoCamera(nullptr, nullptr, -1, 0.0f, false, nullptr,
				                    0, unk250, 0);
				break;
			}

			if (checkFlag(DIRECTOR_FLAG_STAGE_TRANSITION_PENDING)) {
				moveStage();
				r29 = STATE_UNK9;
				break;
			}
		}
		break;

	case 2:
		if (checkFlag(DIRECTOR_FLAG_DEMO_PENDING)) {
			unk126 = 4;
		} else {
			if (unkB0->unk248 == 0)
				unk126 = 0;
		}
		break;

	case 3:
	case 4: {
		bool bVar5  = false;
		bool uVar15 = 0;
		if (checkFlag(DIRECTOR_FLAG_END_DEMO_PENDING)) {
			uVar15 = 1;
			bVar5  = true;
			offFlag(DIRECTOR_FLAG_END_DEMO_PENDING);
		} else {
			if (!gpCamera->getRestDemoFrames()) {
				if (!MSBgm::getHandle(2) || mTickCount - unk60 >= 720) {
					bVar5  = true;
					uVar15 = mDemoQueue[mDemoQueueHead].unk10;
				}
			}
		}

		if (bVar5) {
			u32 prev = mDemoQueueHead++;
			mDemoQueueHead &= 0x7;
			TDemoInfo* info = &mDemoQueue[prev];
			if (mDemoQueueHead != mDemoQueueTail) {
				gpCamera->endDemoCamera();
				if (info->unk14 != nullptr)
					(*info->unk14)(info->unk18, 1);

				TDemoInfo* next = &mDemoQueue[mDemoQueueHead];
				gpCamera->startDemoCamera(next->unk0, next->unk4, next->unk8,
				                          next->unkC, next->unk10);
				if (next->unk14 != nullptr)
					(*next->unk14)(next->unk18, 0);
			} else {
				offFlag(DIRECTOR_FLAG_DEMO_PENDING);
				unk126 = unk124 == 4 ? 2 : 0;
				if (uVar15 != 0)
					gpCamera->endDemoCamera();
				if (info->unk14 != nullptr)
					(*info->unk14)(info->unk18, 1);
			}
		}
	} break;
	}

	if (mDemoQueueHead == mDemoQueueTail)
		offFlag(DIRECTOR_FLAG_END_DEMO_PENDING);

	unk125 = unk124;

	if (unk124 != unk126) {
		switch (unk124) {
		case 2:
			if (unk126 == 0) {
				unkA0 = 0;
				unkA4 = 0;
				unk18[0]->offFlag(TMarioGamePad::PAD_FLAG_TALK_SELECT);
				OSStartStopwatch(&unkE8);
			}
			break;

		case 3:
		case 4:
			if (unk124 == 4)
				MSMainProc::fromTalkingCameraDemo(unk124 == 4);
			else
				MSMainProc::fromInnerCameraDemo();
			unk18[0]->offFlag(TMarioGamePad::PAD_FLAG_NO_INPUT);
			OSStartStopwatch(&unkE8);
			break;
		}

		switch (unk126) {
		case 0:
			break;

		case 1:
			unkA0->onLiveFlag(LIVE_FLAG_UNK40000);
			unkA0->unkC.off(CUE_MOVE | CUE_CALC_ANIM);
			unk18[0]->onFlag(TMarioGamePad::PAD_FLAG_TALK_SELECT);
			OSStopStopwatch(&unkE8);
			break;

		case 2:
			if (unk124 == 1)
				unkB0->openTalkWindow(unkA0);
			break;

		case 3:
		case 4:
			if (unk126 == 4)
				MSMainProc::toTalkingCameraDemo();
			else
				MSMainProc::toInnerCameraDemo();
			unk18[0]->onFlag(TMarioGamePad::PAD_FLAG_NO_INPUT);
			if ((int)mDemoQueue[mDemoQueueHead].unk20.get() == 1) {
				gpCamera->startGateDemoCamera(mDemoQueue[mDemoQueueHead].unk1C);
			} else {
				gpCamera->startDemoCamera(mDemoQueue[mDemoQueueHead].unk0,
				                          mDemoQueue[mDemoQueueHead].unk4,
				                          mDemoQueue[mDemoQueueHead].unk8,
				                          mDemoQueue[mDemoQueueHead].unkC,
				                          mDemoQueue[mDemoQueueHead].unk10);
				if (mDemoQueue[mDemoQueueHead].unk14 != nullptr)
					(*mDemoQueue[mDemoQueueHead].unk14)(
					    mDemoQueue[mDemoQueueHead].unk18, 0);
			}
			OSStopStopwatch(&unkE8);
			unk60 = mTickCount;
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
	unkB4 = TApplication::APP_STATE_GAMEPLAY;
	unkE4 = 15;
	SMSGetApplication()->getFader()->setColor(JUtility::TColor(0, 0, 0, 0xff));

	u8 sVar4 = SMS_getShineStage(SMSGetApplication()->mNextArea.getStage());
	u8 sVar5 = SMS_getShineStage(SMSGetApplication()->mCurrArea.getStage());
	if (sVar4 != sVar5)
		TFlagManager::getInstance()->setFlag(0x40002, 0);

	TGameSequence& nextArea = SMSGetApplication()->mNextArea;

	if (nextArea.getScenario() == 0xff)
		switch (nextArea.getStage()) {
		case 1:
			unkE4         = 2;
			nextArea.unk1 = decideNextScenario(nextArea.getStage());
			TFlagManager::getInstance()->setFlag(0x40003, 0);
			break;

		case 13: {
			unkE4     = 2;
			u32 thing = 0;
			switch (TFlagManager::getInstance()->getFlag(0x40003)) {
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
			switch (TFlagManager::getInstance()->getFlag(0x40003)) {
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
			switch (TFlagManager::getInstance()->getFlag(0x40003)) {
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
			switch (TFlagManager::getInstance()->getFlag(0x40003)) {
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
			unkB4 = TApplication::APP_STATE_BOOT;
			break;

		case 9:
			SMSGetApplication()->getFader()->setColor(
			    JUtility::TColor(0xD2, 0xD2, 0xD2, 0xFF));
			unkB4 = TApplication::APP_STATE_TITLE;
			break;

		case 0x34:
			unkE4         = 8;
			nextArea.unk1 = 0;
			TFlagManager::getInstance()->setFlag(0x40003, 0);
			break;

		case 0:
			nextArea.unk1 = 0;
			TFlagManager::getInstance()->setFlag(0x40003, 0);
			break;

		default:
			unkE4         = 2;
			nextArea.unk1 = 0;
			break;
		}

	if (nextArea.getScenario() != 0xff) {
		if (mFlags & DIRECTOR_FLAG_MOVIE_PENDING) {
			unkE4 = 15;
			SMSGetApplication()->getFader()->setColor(
			    JUtility::TColor(0, 0, 0, 0xff));
			unkB4 = TApplication::APP_STATE_MOVIE;
		} else {
			unkB4 = TApplication::APP_STATE_GAMEPLAY;
		}
	}

	if (gpMarioOriginal->checkFlag(MARIO_FLAG_HAS_FLUDD)) {
		u32 r5 = 0;
		if ((int)gpMarioOriginal->mWaterGun->mSecondNozzle == 3)
			r5 = 4;
		TFlagManager::getInstance()->setFlag(0x40004, r5);
	}
}

JStage::TObject* TMarDirector::JSGFindObject(const char* param_1,
                                             JStage::TEObject param_2) const
{
	if (strcmp("cam_int1", param_1) == 0) {
		TMarDirector* casted = const_cast<TMarDirector*>(this);
		JDrama::TCamera* cam = (JDrama::TCamera*)casted->search("camera 1");
		return cam;
	}

	if (strcmp("mario", param_1) == 0) {
		TMarDirector* casted  = const_cast<TMarDirector*>(this);
		JDrama::TActor* mario = (JDrama::TActor*)casted->search("マリオ");
		return mario;
	}

	return JDrama::TDirector::JSGFindObject(param_1, param_2);
}
