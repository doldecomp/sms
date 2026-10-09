#include <Enemy/BossHanachan.hpp>
#include <Enemy/BossHanachanChangeSaveParams.hpp>
#include <GC2D/GCConsole2.hpp>
#include <MSound/MSModBgm.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <Strategic/Spine.hpp>
#include <System/MarDirector.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>

DEFINE_NERVE(TNerveBossHanachanGraphWander, TLiveActor)
{
	TBossHanachan* hanachan = (TBossHanachan*)spine->getBody();
	if (spine->getTime() == 0) {
		hanachan->setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK0,
		                            BOSS_HANACHAN_STOP_MOTION_BLEND_ON);
	}
	hanachan->execWalk(true);
	if (hanachan->checkFallDecideAndSetup()) {
		spine->pushAfterCurrent(&TNerveBossHanachanTumble::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBossHanachanTumble, TLiveActor)
{
	TBossHanachan* hanachan = (TBossHanachan*)spine->getBody();
	if (spine->getTime() == 0) {
		hanachan->setTumbleAnm(BOSS_HANACHAN_STOP_MOTION_BLEND_ON);
	} else {
		hanachan->considerSetAnm(BOSS_HANACHAN_NERVE_ANM_UNK0);
	}
	hanachan->execSlip();
	if (hanachan->getMarchSpeed() == 0.0f
	    && hanachan->isTumbleCompletelyAllBody()) {
		SMSGetMarDirector()->getConsole()->startAppearBalloon(0xE0007, true);
		spine->pushAfterCurrent(&TNerveBossHanachanDown::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBossHanachanDown, TLiveActor)
{
	TBossHanachan* hanachan = (TBossHanachan*)spine->getBody();
	hanachan->considerSetAnm(BOSS_HANACHAN_NERVE_ANM_UNK1);
	if (spine->getTime() >= hanachan->unk1C0->mSLDownFrames.get()) {
		hanachan->setAnmTimerWhenGetUp();
		spine->pushAfterCurrent(&TNerveBossHanachanGetUp::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBossHanachanGetUp, TLiveActor)
{
	TBossHanachan* hanachan = (TBossHanachan*)spine->getBody();
	hanachan->considerSetAnm(BOSS_HANACHAN_NERVE_ANM_UNK2);
	if (hanachan->isFinishedGetUp()) {
		hanachan->setRandomWeakBodyIndex();
		hanachan->setAnmTimerWhenSnort();
		spine->pushAfterCurrent(&TNerveBossHanachanSnort::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBossHanachanDamage, TLiveActor)
{
	TBossHanachan* hanachan = (TBossHanachan*)spine->getBody();
	hanachan->considerSetAnm(BOSS_HANACHAN_NERVE_ANM_UNK3);
	hanachan->execSlip();
	if (hanachan->getMarchSpeed() == 0.0f
	    && spine->getTime() >= hanachan->unk1C0->mSLDamageFrames.get()) {
		hanachan->setAnmTimerWhenGetUp();
		spine->pushAfterCurrent(&TNerveBossHanachanGetUp::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBossHanachanSnort, TLiveActor)
{
	TBossHanachan* hanachan = (TBossHanachan*)spine->getBody();
	if (spine->getTime() == 0xC8
	    && hanachan->checkLiveFlag(LIVE_FLAG_UNK20000)) {
		hanachan->offLiveFlag(LIVE_FLAG_UNK20000);
		MSBgm::startBGM(MSD_BGM_BOSSGESO_2DN3RD);
		switch (hanachan->mHitPoints) {
		case 2:
			SMSGetMSound()->unk98->changeTempo(0, 1);
			break;
		case 1:
			SMSGetMSound()->unk98->changeTempo(1, 1);
			break;
		}
	}
	hanachan->considerSetAnm(BOSS_HANACHAN_NERVE_ANM_UNK4);
	if (hanachan->isAllBckAlreadyEnd(BOSS_HANACHAN_ANM_UNK14)) {
		hanachan->goToInitialRecoverGraphNode();
		spine->pushAfterCurrent(&TNerveBossHanachanGraphWander::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveBossHanachanDead, TLiveActor)
{
	TBossHanachan* hanachan = (TBossHanachan*)spine->getBody();
	hanachan->considerSetAnm(BOSS_HANACHAN_NERVE_ANM_UNK5);
	if (!hanachan->checkLiveFlag(LIVE_FLAG_UNK40000)
	    && hanachan->isAllBckAlreadyEnd(BOSS_HANACHAN_ANM_UNK15)) {
		hanachan->onLiveFlag(LIVE_FLAG_UNK40000);
		hanachan->removeAllMapCollision();
	}
	return false;
}
