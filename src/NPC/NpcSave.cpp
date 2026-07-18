#include <NPC/NpcSave.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <NPC/NpcBase.hpp>
#include <NPC/NpcManager.hpp>
#include <NPC/NpcEvent.hpp>

TNpcSaveStageFarClip::TNpcSaveStageFarClip()
    : TParams("/Npc/npcFarClip.prm")
    , PARAM_INIT(mSLFarAirport, 15000.0f)
    , PARAM_INIT(mSLFarDolpicTown, 15000.0f)
    , PARAM_INIT(mSLFarBiancoHills, 15000.0f)
    , PARAM_INIT(mSLFarRiccoHarbor, 15000.0f)
    , PARAM_INIT(mSLFarMammaBeach, 15000.0f)
    , PARAM_INIT(mSLFarPinnaBeach, 15000.0f)
    , PARAM_INIT(mSLFarPinnaParco, 15000.0f)
    , PARAM_INIT(mSLFarSirenaBeach, 15000.0f)
    , PARAM_INIT(mSLFarHotelDelfino, 15000.0f)
    , PARAM_INIT(mSLFarMareVillage, 15000.0f)
    , PARAM_INIT(mSLFarMonteVillage, 15000.0f)
    , PARAM_INIT(mSLFarCoronaMountain, 15000.0f)
    , PARAM_INIT(mSLFarOthers, 15000.0f)
{
	TParams::load(mPrmPath);
}

TNpcSaveNormal::TNpcSaveNormal()
    : TParams("/Npc/npcNormal.prm")
    , PARAM_INIT(mSLMarioTalkAcceptDegree, 150.0f)
    , PARAM_INIT(mTalkAcceptDist, 200.0f)
    , PARAM_INIT(mTalkAcceptHeight, 150.0f)
    , PARAM_INIT(mTalkAcceptDegree, 150.0f)
    , PARAM_INIT(mSLSitTalkAcceptDegree, 100.0f)
    , PARAM_INIT(mSLSunflowerLTalkDist, 600.0f)
    , PARAM_INIT(mSLThrowTalkAcceptDist, 200.0f)
    , PARAM_INIT(mSLThrowTalkAcceptHeight, 150.0f)
    , PARAM_INIT(mSLThrowStartFrame, 40)
    , PARAM_INIT(mSLTrampleShakeFrames, 40)
    , PARAM_INIT(mSLTrampleAmplitude, 0.75f)
    , PARAM_INIT(mSLTrampleVelocity, 1500)
    , PARAM_INIT(mSLTrampleToMadFrames, 60)
    , PARAM_INIT(mSLHeadHeightNormal, 100.0f)
    , PARAM_INIT(mSLHeadHeightSandBomb, 200.0f)
    , PARAM_INIT(mSLBlownVelocity, 30.0f)
    , PARAM_INIT(mGravityY, 0.3f)
    , PARAM_INIT(mMoveWalkAnmRateChase, 0.9f)
    , PARAM_INIT(mStopWalkAnmRateChase, 0.01f)
    , PARAM_INIT(mStopWalkAnmRateFrame, 10)
    , PARAM_INIT(mMotionBlendFrame, 20)
    , PARAM_INIT(mSLDanceAnmOffDist, 6500.0f)
    , PARAM_INIT(mThrowSpeedXZ, 3.5f)
    , PARAM_INIT(mThrowSpeedY, 2.5f)
    , PARAM_INIT(mPosInbetweenFrame, 6)
    , PARAM_INIT(mSLGraphWanderMinFrame, 300)
    , PARAM_INIT(mSLGraphWanderMaxFrame, 3000)
    , PARAM_INIT(mSLGraphWaitMinFrame, 100)
    , PARAM_INIT(mSLGraphWaitMaxFrame, 1200)
    , PARAM_INIT(mSLCleanEffectScale, 3.0f)
    , PARAM_INIT(mSLSmokeRunMagnif, 1.5f)
    , PARAM_INIT(mSLFireDecSpeed, 0.002f)
{
	TParams::load(mPrmPath);
}

TNpcSaveIndividual::TNpcSaveIndividual(const char* param_1)
    : TParams(param_1)
    , PARAM_INIT(mSLBodyHeight, 200.0f)
    , PARAM_INIT(mSLCursorHeight, 30.0f)
    , PARAM_INIT(mSLLookatHeight, 40.0f)
    , PARAM_INIT(mCircleShadowSize, 60.0f)
    , PARAM_INIT(mSLDamageRadiusSmall, 10.0f)
    , PARAM_INIT(mSLMinWalkAnmRate, 0.1f)
    , PARAM_INIT(mSLMaxWalkAnmRate, 1.5f)
    , PARAM_INIT(mSLMaxRunAnmRate, 3.0f)
    , PARAM_INIT(mSLMinMarchSpeed, 0.2f)
    , PARAM_INIT(mMaxMarchSpeed, 3.0f)
    , PARAM_INIT(mSLMaxRunSpeed, 6.0f)
    , PARAM_INIT(mSLRunAccel, 0.08f)
    , PARAM_INIT(mMarchAccel, 0.02f)
    , PARAM_INIT(mMarchDecrease, 0.1f)
    , PARAM_INIT(mWalkTurnSpeed, 5.0f)
    , PARAM_INIT(mWaitTurnSpeed, 1.0f)
    , PARAM_INIT(mTurnAnmMinRate, 1.0f)
    , PARAM_INIT(mTurnAnmMaxRate, 1.0f)
    , PARAM_INIT(mTurnAnmRate, 1.0f)
    , PARAM_INIT(mWaitAnmOffDist0, 3500.0f)
    , PARAM_INIT(mWaitAnmOffDist1, 7500.0f)
    , PARAM_INIT(mLodChangeDist, 5500.0f)
    , PARAM_INIT(mAllDLLockDist, 2000.0f)
    , PARAM_INIT(mNeckTurnSearchDist, 500.0f)
    , PARAM_INIT(mNeckTurnSearchHeight, 600.0f)
    , PARAM_INIT(mBodyTurnSearchDist, 300.0f)
    , PARAM_INIT(mBodyTurnSearchHeight, 250.0f)
    , PARAM_INIT(mBodyTurnSearchDegree, 240.0f)
    , PARAM_INIT(mBodyTurnSearchAware, 160.0f)
    , PARAM_INIT(mMadSearchDist, 800.0f)
    , PARAM_INIT(mMadSearchHeight, 400.0f)
    , PARAM_INIT(mMadSearchDegree, 240.0f)
    , PARAM_INIT(mMadSearchAware, 500.0f)
    , PARAM_INIT(mMadTurnSpeed, 20.0f)
    , PARAM_INIT(mFirstStateTurnSpeed, 0.5f)
    , PARAM_INIT(mUTurnSpeed, 3.0f)
    , PARAM_INIT(mSinkSpeed, 0.3f)
    , PARAM_INIT(mSinkHeight, 220.0f)
    , PARAM_INIT(mPollutionMax, 150)
    , PARAM_INIT(mPollutionCleanSpeed, 0.004f)
    , PARAM_INIT(mNeckMinAngleX, -8192)
    , PARAM_INIT(mNeckMaxAngleX, 0)
    , PARAM_INIT(mNeckMaxAngleY, 15000)
    , PARAM_INIT(mNeckAngleXSpeed, 500)
    , PARAM_INIT(mNeckAngleYSpeed, 700)
{
	TParams::load(mPrmPath);
}

TNpcParams::TNpcParams()
{
	TMonteMBaseManager::mStaticCommonKeeper = nullptr;
	TMonteWBaseManager::mStaticCommonKeeper = nullptr;
	TMareMBaseManager::mStaticCommonKeeper  = nullptr;
	TMareWBaseManager::mStaticCommonKeeper  = nullptr;
	TMareBaseManager::mStaticBmtNormal      = nullptr;
	TMareBaseManager::mStaticBmtPollution   = nullptr;
	TNpcEvent::initDownSunflowerNum();
	gpMareJellyFishManager = nullptr;

	unk0 = new TNpcSaveStageFarClip;
	unk4 = new TNpcSaveNormal;

	TBaseNPC::mPtrSaveNormal = unk4;

	static const char* sSaveFileName[] = {
		"/Npc/npcMonteM.prm",     "/Npc/npcMonteM.prm",
		"/Npc/npcMonteM.prm",     "/Npc/npcMonteM.prm",
		"/Npc/npcMonteM.prm",     "/Npc/npcMonteME.prm",
		"/Npc/npcMonteMF.prm",    "/Npc/npcMonteMG.prm",
		"/Npc/npcMonteMH.prm",    "/Npc/npcMonteW.prm",
		"/Npc/npcMonteW.prm",     "/Npc/npcMonteW.prm",
		"/Npc/npcMonteWC.prm",    "/Npc/npcMareM.prm",
		"/Npc/npcMareMA.prm",     "/Npc/npcMareMB.prm",
		"/Npc/npcMareMC.prm",     "/Npc/npcMareMD.prm",
		"/Npc/npcMareW.prm",      "/Npc/npcMareWA.prm",
		"/Npc/npcMareWB.prm",     "/Npc/npcKinopio.prm",
		"/Npc/npcKinojii.prm",    "/Npc/npcPeach.prm",
		"/Npc/npcRaccoonDog.prm", "/Npc/npcSunflowerL.prm",
		"/Npc/npcSunflowerS.prm", "/Npc/npcDummy.prm",
		"/Npc/npcBoard.prm",
	};

	for (int i = 0; i < 29; ++i) {
		switch (i) {
		case 1:
		case 2:
		case 3:
		case 4:
			unk8[i] = unk8[0];
			break;

		case 10:
		case 11:
			unk8[i] = unk8[9];
			break;

		default:
			unk8[i] = new TNpcSaveIndividual(sSaveFileName[i]);
			break;
		}
	}
}
