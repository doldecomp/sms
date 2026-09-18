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

// TODO: promote to MSound::getModBgm() in include/MSound/MSound.hpp. The
// inline level it adds is what gives TNerveBossHanachanSnort::execute its
// 0x40 frame (raw gpMSound->unk98 is 0x28, this level +8 per site, the
// SMSGetMSound() level another +8 over the two sites); parked here as a
// TU-prefixed static inline because a shared header may not be edited in a
// unit batch.
static inline MSModBgm* BossHanachanNerve_getModBgm(MSound* sound)
{
	return sound->unk98;
}

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
	// TODO: frame 0x38 vs retail 0x40; the body is instruction-exact (8 bytes
	// of padding gives 100%) and every r1 displacement in retail, the LR slot
	// included, is exactly 8 higher, i.e. one 8-byte object below every local
	// (see docs/catalog/frame-gaps.md, "The last 8 bytes"). The accessor
	// ladder saturates at +0x10 here: measured 0x28 for gpMarDirector
	// ->mConsole, 0x30 for one level (either ->getConsole() or
	// boss->getMarchSpeed()), 0x38 for any two or three of
	// {SMSGetMarDirector(), getConsole(), getMarchSpeed()}. Also +0: a
	// TU-static console forwarder (with and without a director parameter),
	// nested ifs instead of &&, a named TLiveActor* before the cast, and a
	// named bool for startAppearBalloon's discarded result.
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
		switch (boss->mHitPoints) {
		case 2:
			BossHanachanNerve_getModBgm(SMSGetMSound())->changeTempo(0, 1);
			break;
		case 1:
			BossHanachanNerve_getModBgm(SMSGetMSound())->changeTempo(1, 1);
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
