#include <Enemy/BossHanachan.hpp>
#include <Strategic/Spine.hpp>
#include <System/MarDirector.hpp>
#include <GC2D/GCConsole2.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSModBgm.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

DEFINE_NERVE(TNerveBossHanachanGraphWander, TLiveActor)
{
	TBossHanachan* boss = (TBossHanachan*)spine->getBody();
	if (spine->getTime() == 0)
		boss->setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK0,
		                        BOSS_HANACHAN_STOP_MOTION_BLEND_ON);
	boss->execWalk(true);
	if (boss->checkFallDecideAndSetup()) {
		spine->pushAfterCurrent(&TNerveBossHanachanTumble::theNerve());
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveBossHanachanTumble, TLiveActor)
{
	TBossHanachan* boss = (TBossHanachan*)spine->getBody();
	if (spine->getTime() == 0)
		boss->setTumbleAnm(BOSS_HANACHAN_STOP_MOTION_BLEND_ON);
	else
		boss->considerSetAnm(BOSS_HANACHAN_NERVE_ANM_UNK0);
	boss->execSlip();
	if (0.0f == boss->mMarchSpeed && boss->isTumbleCompletelyAllBody()) {
		SMSGetMarDirector()->getConsole()->startAppearBalloon(7, true);
		spine->pushAfterCurrent(&TNerveBossHanachanDown::theNerve());
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveBossHanachanDown, TLiveActor)
{
	TBossHanachan* boss = (TBossHanachan*)spine->getBody();
	boss->considerSetAnm(BOSS_HANACHAN_NERVE_ANM_UNK1);
	if (spine->getTime() >= boss->mChangeParams->mSLDownFrames.get()) {
		boss->setAnmTimerWhenGetUp();
		spine->pushAfterCurrent(&TNerveBossHanachanGetUp::theNerve());
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveBossHanachanGetUp, TLiveActor)
{
	TBossHanachan* boss = (TBossHanachan*)spine->getBody();
	boss->considerSetAnm(BOSS_HANACHAN_NERVE_ANM_UNK2);
	if (boss->isFinishedGetUp()) {
		boss->setRandomWeakBodyIndex();
		boss->setAnmTimerWhenSnort();
		spine->pushAfterCurrent(&TNerveBossHanachanSnort::theNerve());
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveBossHanachanDamage, TLiveActor)
{
	TBossHanachan* boss = (TBossHanachan*)spine->getBody();
	boss->considerSetAnm(BOSS_HANACHAN_NERVE_ANM_UNK3);
	boss->execSlip();
	if (0.0f == boss->mMarchSpeed
	    && spine->getTime() >= boss->mChangeParams->mSLDamageFrames.get()) {
		boss->setAnmTimerWhenGetUp();
		spine->pushAfterCurrent(&TNerveBossHanachanGetUp::theNerve());
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveBossHanachanSnort, TLiveActor)
{
	TBossHanachan* boss = (TBossHanachan*)spine->getBody();
	if (spine->getTime() == 200 && boss->checkLiveFlag(LIVE_FLAG_UNK20000)) {
		boss->offLiveFlag(LIVE_FLAG_UNK20000);
		MSBgm::startBGM(0x80010029);
		// TODO: the US binary puts the tempo controller at 0x9C;
		// the current shared MSound layout places it at 0x98.
		switch (boss->mHitPoints) {
		case 2:
			gpMSound->unk98->changeTempo(0, 1);
			break;
		case 1:
			gpMSound->unk98->changeTempo(1, 1);
			break;
		}
	}
	boss->considerSetAnm(BOSS_HANACHAN_NERVE_ANM_UNK4);
	if (boss->isAllBckAlreadyEnd(BOSS_HANACHAN_ANM_UNK14)) {
		boss->goToInitialRecoverGraphNode();
		spine->pushAfterCurrent(&TNerveBossHanachanGraphWander::theNerve());
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveBossHanachanDead, TLiveActor)
{
	TBossHanachan* boss = (TBossHanachan*)spine->getBody();
	boss->considerSetAnm(BOSS_HANACHAN_NERVE_ANM_UNK5);
	if (!boss->checkLiveFlag(LIVE_FLAG_UNK40000)
	    && boss->isAllBckAlreadyEnd(BOSS_HANACHAN_ANM_UNK15)) {
		boss->onLiveFlag(LIVE_FLAG_UNK40000);
		boss->removeAllMapCollision();
	}
	return FALSE;
}
