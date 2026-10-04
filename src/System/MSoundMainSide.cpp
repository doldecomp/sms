#include <System/MSoundMainSide.hpp>
#include <System/MarDirector.hpp>
#include <System/FlagManager.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSModBgm.hpp>
#include <MSound/MSHandle.hpp>
#include <MarioUtil/MapUtil.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Camera/CubeMapTool.hpp>
#include <Player/MarioAccess.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

MSStage* MSStage::smMSStage;
#ifdef VERSION_GMSP01
MSStageCubeFade* MSStageCubeFade::smInstance             = nullptr;
MSStageCubeFadeDouble* MSStageCubeFadeDouble::smInstance = nullptr;
#endif

namespace MSMainProc {

namespace MSStageInfo {

	MS_SCENE_WAVE msStg;
	u32 demoBgm;
	u32 stageBgm;
	u32 stageBgmSilent;
	u8 stageBgmSilentStartStatus;
	u8 flags;
	u16 volOffCategory;
	u8 fadeEvent;
	u32 switchBgm;
	u32 switchBgm2;
	f32 cubeFadeRatio;
	u8 cubeFadeUsePan;
	bool bossLives;
	bool bossLives2;
	u8 bossNotDamaged;
	bool distFadeStageToKage;

} // namespace MSStageInfo

} // namespace MSMainProc

#ifdef VERSION_GMSP01
void MSMainProc::setGateKeeperBGMPlayFlag(u32 param_1, bool param_2)
{
	MSStageCubeFadeDouble* stage = MSStageCubeFadeDouble::smInstance;
	if (stage == nullptr)
		return;

	if (param_1 == 3)
		stage->mPlayFlag[0] = param_2;
	else if (param_1 == 4)
		stage->mPlayFlag[1] = param_2;
}

bool MSMainProc::getGateKeeperBGMStopFlag()
{
	MSStageCubeFadeDouble* stage = MSStageCubeFadeDouble::smInstance;
	if (stage != nullptr) {
		for (u8 i = 0; i < 2; ++i) {
			if (stage->mPlayFlag[i])
				return false;
		}
	}
	return true;
}
#endif

inline Vec getCubeCheckPos(const Vec& pos)
{
	Vec result = pos;
	result.y += 75.0f;
	return result;
}

// TODO: fake and wrong, figure this out
inline TCubeGeneralInfo* getSoundCubeInfo(s32 no)
{
	return gpCubeSoundChange->getCubeInfo(no);
}

int MSMainProc::getMonteVillageActorArea(const Vec& param_1)
{
	int result = 4;
	if (MSGMSound->unkCD == 8) {
		switch (gpCubeFastC->getInCubeNo(getCubeCheckPos(param_1))) {
		case 0:
			result = 2;
			break;
		case 1:
			result = 0;
			break;
		default:
			result = 3;
			break;
		}
	}
	return result;
}

void MSMainProc::entranceDemoWipeInEnd() { }

void MSMainProc::toInnerCameraDemo() { gpMSound->unkC8[2] = 1; }

void MSMainProc::fromInnerCameraDemo() { gpMSound->unkC8[2] = 0; }

void MSMainProc::toTHPDemo() { }

void MSMainProc::fromTHPDemo() { }

void MSMainProc::toTalkingCameraDemo()
{
	u16 tmp = 0;
	switch (gpMarDirector->mMap) {
	case 9:
		tmp = 4;
		break;

	case 4:
		tmp = 4;
		break;

	case 1:
		tmp = 4;
		break;
	}

	gpMSound->setCategoryVOLs(tmp, 1.0f);
}

void MSMainProc::fromTalkingCameraDemo(bool) { gpMSound->talkModeIn(false); }

void MSMainProc::setBossLivesFlag(bool param_1)
{
	MSBgm::stopTrackBGM(1, 10);
	MSBgm::setTrackVolume(0, 1.0f, 18, 0);
	MSStageInfo::bossLives = param_1;
}

void MSMainProc::setBossLivesFlagOnlyFlag(bool param_1)
{
	MSStageInfo::bossLives = param_1;
}

void MSMainProc::setBossLivesFlag2(bool param_1)
{
	MSBgm::stopTrackBGM(1, 10);
	MSBgm::setTrackVolume(0, 1.0f, 18, 0);
	MSStageInfo::bossLives2 = param_1;
}

bool MSMainProc::getBossLivesFlag() { return MSStageInfo::bossLives; }

bool MSMainProc::getBossLivesFlag2() { return MSStageInfo::bossLives2; }

void MSMainProc::setBossNotDamagedFlag(bool param_1)
{
	MSStageInfo::bossNotDamaged = param_1;
}

void MSMainProc::setMSoundEnterStage(u8 param_1, u8 param_2)
{

	MSStageInfo::msStg                     = MSBgm::getSceneNo(0xfffffff0);
	MSStageInfo::stageBgm                  = 0xfffffff0;
	MSStageInfo::demoBgm                   = MSD_BGM_MONTE_ONSEN;
	MSStageInfo::flags                     = 10;
	MSStageInfo::stageBgmSilent            = 0xfffffff0;
	MSStageInfo::stageBgmSilentStartStatus = 2;
	MSStageInfo::fadeEvent                 = 0;
	MSStageInfo::switchBgm                 = 0xfffffff0;
	MSStageInfo::switchBgm2                = 0xfffffff0;
	MSStageInfo::cubeFadeRatio             = 0.15f;
	MSStageInfo::cubeFadeUsePan            = 1;
	MSStageInfo::bossLives                 = 1;
	MSStageInfo::bossLives2                = 1;
	MSStageInfo::bossNotDamaged            = 1;
	MSStageInfo::volOffCategory            = 0x1c7;
	MSStageInfo::distFadeStageToKage       = 1;

	bool bVar2 = false;

	switch (param_1) {
	case 0:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_UNDERGROUND);
		MSStageInfo::stageBgm = MSD_BGM_UNDERGROUND;
		break;
	case 1:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_BIANCO);
		MSStageInfo::stageBgm = MSD_BGM_BIANCO;
		if (true) {
			switch (param_2) {
			case 0:
				MSStageInfo::fadeEvent     = 2;
				MSStageInfo::switchBgm     = 0xfffffff0;
				MSStageInfo::switchBgm2    = 0xfffffff0;
				MSStageInfo::cubeFadeRatio = 0.22f;
				break;
			case 1:
				MSStageInfo::demoBgm = MSD_BGM_CAMERA;
				MSStageInfo::flags   = 0;
				MSStageInfo::volOffCategory -= 0x183;
				break;
			case 5:
				MSStageInfo::fadeEvent     = 2;
				MSStageInfo::switchBgm     = 0xfffffff0;
				MSStageInfo::switchBgm2    = 0xfffffff0;
				MSStageInfo::cubeFadeRatio = 0.34f;
				break;
			case 8:
				if (TFlagManager::getInstance()->getFlag(0x60003) > 0) {
					MSStageInfo::demoBgm = MSD_BGM_CAMERA;
					MSStageInfo::flags   = 6;
					MSStageInfo::volOffCategory -= 0x83;
					MSStageInfo::fadeEvent  = 1;
					MSStageInfo::switchBgm  = 0xfffffff0;
					MSStageInfo::switchBgm2 = 0xfffffff0;
				}
				break;
			case 9:
				MSStageInfo::demoBgm = MSD_BGM_CAMERA;
				MSStageInfo::flags   = 0;
				MSStageInfo::volOffCategory -= 0x82;
			}
		}
		break;
	case 2:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_MAMMA);
		MSStageInfo::stageBgm = MSD_BGM_MAMMA;
		switch (param_2) {
		case 0:
			MSStageInfo::demoBgm       = MSD_BGM_GAMEOVER;
			MSStageInfo::flags         = 10;
			MSStageInfo::fadeEvent     = 2;
			MSStageInfo::switchBgm     = 0xfffffff0;
			MSStageInfo::switchBgm2    = 0xfffffff0;
			MSStageInfo::cubeFadeRatio = 0.28f;
			break;

		case 6:
			MSStageInfo::demoBgm = MSD_BGM_CAMERA;
			MSStageInfo::flags   = 3;
			MSStageInfo::volOffCategory -= 0x83;
			MSStageInfo::fadeEvent           = 1;
			MSStageInfo::switchBgm           = 0xfffffff0;
			MSStageInfo::switchBgm2          = 0xfffffff0;
			MSStageInfo::distFadeStageToKage = 0;
			break;
		}
		break;
	case 3:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_GET_SHINE);
		MSStageInfo::stageBgm = MSD_BGM_GET_SHINE;
		if (param_2 == 6) {
			MSStageInfo::demoBgm = MSD_BGM_CAMERA;
			MSStageInfo::flags   = 3;
			MSStageInfo::volOffCategory -= 0x83;
			MSStageInfo::fadeEvent           = 1;
			MSStageInfo::switchBgm           = 0xfffffff0;
			MSStageInfo::switchBgm2          = 0xfffffff0;
			MSStageInfo::distFadeStageToKage = 0;
		}
		if (param_2 == 4) {
			MSStageInfo::switchBgm     = MSD_BGM_MAP_SELECT;
			MSStageInfo::fadeEvent     = 3;
			MSStageInfo::switchBgm2    = MSD_BGM_CHUBOSS_MANTA;
			MSStageInfo::cubeFadeRatio = 0.28f;
		}
		break;
	case 4:
		if (param_2 != 2) {
			MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_PINNAPACO_SEA);
			MSStageInfo::stageBgm = MSD_BGM_PINNAPACO_SEA;
		} else {
			MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_MAP_SELECT);
			MSStageInfo::stageBgm = MSD_BGM_MAP_SELECT;
		}
		if (param_2 == 6) {
			MSStageInfo::switchBgm = 0xfffffff0;
			MSStageInfo::demoBgm   = MSD_BGM_CAMERA;
			MSStageInfo::flags     = 3;
			MSStageInfo::volOffCategory -= 0x83;
			MSStageInfo::fadeEvent           = 1;
			MSStageInfo::switchBgm2          = 0xfffffff0;
			MSStageInfo::distFadeStageToKage = 0;
		} else if (param_2 == 2) {
			MSStageInfo::demoBgm = MSD_BGM_MAP_SELECT;
			MSStageInfo::flags   = 0;
		}
		break;
	case 5:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_PINNAPACO);
		MSStageInfo::stageBgm = MSD_BGM_PINNAPACO;
		switch (param_2) {
		case 0:
			MSStageInfo::flags   = 10;
			MSStageInfo::demoBgm = MSD_BGM_GAMEOVER;
			break;
		case 1:
			MSStageInfo::volOffCategory -= 4;
			break;
		}
		break;
	case 6:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_RICCO);
		MSStageInfo::stageBgm = MSD_BGM_RICCO;
		break;
	case 7:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_MAREVILLAGE);
		MSStageInfo::stageBgm = MSD_BGM_MAREVILLAGE;
		if (param_2 == 3) {
			MSStageInfo::fadeEvent                 = 1;
			MSStageInfo::switchBgm                 = 0xfffffff0;
			MSStageInfo::switchBgm2                = 0xfffffff0;
			MSStageInfo::stageBgm                  = MSD_BGM_CAMERA;
			MSStageInfo::stageBgmSilent            = MSD_BGM_MAREVILLAGE;
			MSStageInfo::stageBgmSilentStartStatus = 2;
			MSStageInfo::distFadeStageToKage       = 0;
		}
		break;
	case 8:
		switch (param_2) {
		case 5:
			MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_MERRY_GO_ROUND);
			MSStageInfo::stageBgm = MSD_BGM_MERRY_GO_ROUND;
			break;
		default:
			MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_SHILENA);
			MSStageInfo::stageBgm = MSD_BGM_SHILENA;
			break;
		}
		if (param_2 == 6) {
			MSStageInfo::switchBgm = 0xfffffff0;
			MSStageInfo::demoBgm   = MSD_BGM_CAMERA;
			MSStageInfo::flags     = 6;
			MSStageInfo::volOffCategory -= 0x83;
			MSStageInfo::fadeEvent  = 1;
			MSStageInfo::switchBgm2 = 0xfffffff0;
		} else {
			MSStageInfo::switchBgm  = 0xfffffff0;
			MSStageInfo::fadeEvent  = 2;
			MSStageInfo::switchBgm2 = 0xfffffff0;

			switch (param_2) {
			case 7:
				MSStageInfo::cubeFadeUsePan = 0;
				MSStageInfo::cubeFadeRatio  = 0.28f;
				break;
			default:
				MSStageInfo::cubeFadeUsePan = 1;
				MSStageInfo::cubeFadeRatio  = 0.15f;
				break;
			}

			if (param_2 == 7) {
				MSStageInfo::stageBgmSilent            = MSD_BGM_SHINE_APPEAR;
				MSStageInfo::stageBgmSilentStartStatus = 2;
			} else {
				MSStageInfo::stageBgmSilent            = MSD_BGM_MECHAKUPPA;
				MSStageInfo::stageBgmSilentStartStatus = 0;
			}
		}
		break;
	case 9:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_CORONA);
		MSStageInfo::stageBgm = MSD_BGM_CORONA;
		if (param_2 == 6) {
			MSStageInfo::demoBgm = MSD_BGM_CAMERA;
			MSStageInfo::flags   = 3;
			MSStageInfo::volOffCategory -= 0x83;
			MSStageInfo::fadeEvent           = 1;
			MSStageInfo::switchBgm           = 0xfffffff0;
			MSStageInfo::switchBgm2          = 0xfffffff0;
			MSStageInfo::distFadeStageToKage = 0;
		} else if (param_2 == 1) {
			MSStageInfo::switchBgm  = MSD_BGM_MISS;
			MSStageInfo::fadeEvent  = 3;
			MSStageInfo::switchBgm2 = MSD_BGM_MISS;
		}
		break;
	case 13:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_MARE_SEA);
		MSStageInfo::stageBgm = MSD_BGM_MARE_SEA;
		if (param_2 == 4) {
			MSStageInfo::fadeEvent                 = 1;
			MSStageInfo::switchBgm                 = 0xfffffff0;
			MSStageInfo::switchBgm2                = 0xfffffff0;
			MSStageInfo::stageBgm                  = MSD_BGM_CAMERA;
			MSStageInfo::stageBgmSilent            = MSD_BGM_MARE_SEA;
			MSStageInfo::stageBgmSilentStartStatus = 2;
			MSStageInfo::distFadeStageToKage       = 0;
		} else if (param_2 == 0) {
			MSStageInfo::fadeEvent                 = 1;
			MSStageInfo::switchBgm                 = 0xfffffff0;
			MSStageInfo::switchBgm2                = 0xfffffff0;
			MSStageInfo::stageBgm                  = MSD_BGM_CAMERA;
			MSStageInfo::stageBgmSilent            = MSD_BGM_MARE_SEA;
			MSStageInfo::stageBgmSilentStartStatus = 2;
			MSStageInfo::distFadeStageToKage       = 0;
		} else {
			MSStageInfo::stageBgmSilent            = MSD_BGM_SCENARIO_SELECT;
			MSStageInfo::cubeFadeRatio             = 0.12f;
			MSStageInfo::stageBgmSilentStartStatus = 2;
			MSStageInfo::fadeEvent                 = 2;
			MSStageInfo::switchBgm                 = 0xfffffff0;
			MSStageInfo::switchBgm2                = 0xfffffff0;
			MSStageInfo::cubeFadeUsePan            = 1;
		}
		if (param_2 == 7)
			MSStageInfo::stageBgm = 0xfffffff0;
		break;
	case 14:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_EVENT);
		MSStageInfo::stageBgm = MSD_BGM_EVENT;
		break;
	case 15:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_CHUBOSS2);
		MSStageInfo::stageBgm = MSD_BGM_CHUBOSS2;
		SMSGetMSound()->loadArcSeqData(MSD_BGM_CHUBOSS2, false);
		break;
	case 16:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_MONTEVILLAGE);
		bVar2                 = true;
		MSStageInfo::stageBgm = MSD_BGM_MONTEVILLAGE;
		break;
	case 20:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_BIANCO);
		MSStageInfo::stageBgm = MSD_BGM_BIANCO;
		MSStageInfo::demoBgm  = MSD_BGM_BIANCO;
		MSStageInfo::flags    = 0;
		break;
	case 21:
	case 22:
	case 23:
	case 24:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_MONTE_RESCUE);
		MSStageInfo::stageBgm = MSD_BGM_MONTE_RESCUE;
		MSStageInfo::demoBgm  = MSD_BGM_MONTE_RESCUE;
		MSStageInfo::flags    = 0;
		break;
	case 28:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_DELFINO);
		MSStageInfo::stageBgm = MSD_BGM_DELFINO;
		MSStageInfo::demoBgm  = MSD_BGM_DELFINO;
		MSStageInfo::flags    = 0;
		break;
	case 29:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_MONTE_RESCUE);
		MSStageInfo::stageBgm = MSD_BGM_MONTE_RESCUE;
		MSStageInfo::demoBgm  = MSD_BGM_MONTE_RESCUE;
		MSStageInfo::flags    = 0;
		break;
	case 30:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_DELFINO);
		MSStageInfo::stageBgm = MSD_BGM_DELFINO;
		break;
	case 31:
	case 32:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_DELFINO);
		MSStageInfo::stageBgm = MSD_BGM_DELFINO;
		MSStageInfo::demoBgm  = MSD_BGM_DELFINO;
		MSStageInfo::flags    = 0;
		break;
	case 33:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_MONTE_RESCUE);
		MSStageInfo::stageBgm = MSD_BGM_MONTE_RESCUE;
		MSStageInfo::demoBgm  = MSD_BGM_MONTE_RESCUE;
		MSStageInfo::flags    = 0;
		break;
	case 34:
	case 35:
	case 40:
	case 41:
	case 42:
	case 43:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_DELFINO);
		MSStageInfo::stageBgm = MSD_BGM_DELFINO;
		MSStageInfo::demoBgm  = MSD_BGM_DELFINO;
		MSStageInfo::flags    = 0;
		break;
	case 44:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_MONTE_RESCUE);
		bVar2                 = true;
		MSStageInfo::stageBgm = MSD_BGM_MONTE_RESCUE;
		MSStageInfo::demoBgm  = MSD_BGM_MONTE_RESCUE;
		MSStageInfo::flags    = 0;
		break;
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_DELFINO);
		MSStageInfo::stageBgm = MSD_BGM_DELFINO;
		MSStageInfo::demoBgm  = MSD_BGM_DELFINO;
		MSStageInfo::flags    = 0;
		break;
	case 52:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_KAGEMARIO);
		MSStageInfo::stageBgm = MSD_BGM_KAGEMARIO;
		MSStageInfo::demoBgm  = MSD_BGM_KAGEMARIO;
		MSStageInfo::flags    = 0;
		break;
	case 55:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_MAP_SELECT);
		MSStageInfo::stageBgm = MSD_BGM_MAP_SELECT;
		break;
	case 57:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_MAP_SELECT);
		MSStageInfo::stageBgm = MSD_BGM_MAP_SELECT;
		break;
	case 58:
		switch (param_2) {
		case 0:
			MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_TIME_IVENT);
			MSStageInfo::stageBgm = MSD_BGM_TIME_IVENT;
			break;
		case 1:
			MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_AIRPORT);
			MSStageInfo::stageBgm = MSD_BGM_AIRPORT;
			break;
		}
		break;
	case 59:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_GET_SHINE);
		MSStageInfo::stageBgm = MSD_BGM_GET_SHINE;
		MSBgm::startBGM(MSD_BGM_MAP_SELECT);
		MSStageInfo::fadeEvent  = 2;
		MSStageInfo::flags      = 3;
		MSStageInfo::switchBgm  = 0xfffffff0;
		MSStageInfo::switchBgm2 = 0xfffffff0;
		break;
	case 60:
		MSStageInfo::msStg    = MSBgm::getSceneNo(MSD_BGM_MONTEMAN_RACE);
		MSStageInfo::stageBgm = MSD_BGM_MONTEMAN_RACE;
		MSStageInfo::demoBgm  = MSD_BGM_MONTEMAN_RACE;
		MSStageInfo::flags    = 0;
		MSStageInfo::volOffCategory -= 0x104;
	}

	SMSGetMSound()->initSound();
	if (MSStageInfo::msStg != MS_WAVE_INVALID) {
		SMSGetMSound()->enterStage(MSStageInfo::msStg, param_1, param_2);
	}
	MSSeCallBack::setWaterCameraFir(bVar2);
	if (MSStageInfo::stageBgmSilent != 0xfffffff0
	    && MSStageInfo::stageBgmSilentStartStatus == 0
	    && SMSGetMSound()->unkCF != 0) {
		JAISound* sound = MSBgm::startBGM(MSStageInfo::stageBgmSilent);
		if (sound)
			sound->setVolume(0.0f, 0, 0);
	}
	SMSGetMSound()->unkC8[0] = 1;
}

void MSMainProc::startStageEntranceDemo(u8, u8)
{
	if (MSStageInfo::demoBgm != 0xfffffff0)
		MSBgm::startBGM(MSStageInfo::demoBgm);

	if (MSStageInfo::stageBgmSilent != 0xfffffff0
	    && MSStageInfo::stageBgmSilentStartStatus == 1
	    && gpMSound->unkCF != 0) {
		JAISound* sound = MSBgm::startBGM(MSStageInfo::stageBgmSilent);
		if (sound)
			sound->setVolume(0.0f, 0, 0);
	}

	gpMSound->demoModeIn(MSStageInfo::volOffCategory, false);
	gpMSound->unkA4    = 0;
	gpMSound->unkC8[1] = 1;
}

void MSMainProc::entranceDemoLoop(u32) { }

void MSMainProc::endStageEntranceDemo(u8, u8)
{
	u32 bgm = MSStageInfo::demoBgm;
	if (bgm != 0xfffffff0)
		if (MSStageInfo::flags & 8 ? true : false)
			MSBgm::stopBGM(bgm, 20);

	if (MSStageInfo::flags & 4 ? true : false)
		MSBgm::setVolume(MSStageInfo::demoBgm, 0.0, 20, 0);

	gpMSound->demoModeOut(false);
	gpMSound->unkC8[4] = 0;
}

void MSMainProc::startStageBGM(u8, u8)
{
	gpMSound->initSound();

	gpMSound->unkA0    = 0;
	gpMSound->unkC8[1] = 0;

	u32 bgm = MSStageInfo::stageBgm;
	if (bgm != 0xfffffff0) {
		if (MSStageInfo::flags & 2 ? true : false) {
			if (MSStageInfo::flags & 1) {
				if (gpMSound->unkCF != 0) {
					JAISound* sound = MSBgm::startBGM(bgm);
					if (sound)
						sound->setVolume(0.0f, 0, 0);
				}
			} else {
				MSBgm::startBGM(bgm);
			}
		}
	}

	if (MSStageInfo::stageBgmSilent != 0xfffffff0
	    && MSStageInfo::stageBgmSilentStartStatus == 2
	    && gpMSound->unkCF != 0) {
		JAISound* sound = MSBgm::startBGM(MSStageInfo::stageBgmSilent);
		if (sound)
			sound->setVolume(0.0f, 0, 0);
	}
}

MSStage* MSStage::init(u8 param_1, u8 param_2)
{
	smMSStage = nullptr;
#ifdef VERSION_GMSP01
	MSStageCubeFade::smInstance       = nullptr;
	MSStageCubeFadeDouble::smInstance = nullptr;
#endif

	gpMSound->unk9C->unk0 = 0.0f;

	switch (MSMainProc::MSStageInfo::fadeEvent) {
	case 1:
		if (param_1 != 8) {
			f32 fVar1 = 6000.0f;
			f32 fVar2 = 1600.0f;
			u32 uVar6 = 260;

			if (param_1 == 7) {
				fVar1 = 3000.0f;
				uVar6 = 260;
				fVar2 = 800.0f;
			} else if (param_1 == 2) {
				uVar6 = 260;
			} else if (param_1 == 4) {
				uVar6 = 340;
			}

			smMSStage = new MSStageDistFade(
			    gpMSound->unkAC[1].mPosition, fVar1, fVar2, uVar6,
			    MSMainProc::MSStageInfo::distFadeStageToKage);
		} else {
			smMSStage = new MSStageDistFadeMonte(
			    gpMSound->unkAC[1].mPosition, 6000.0f, 1600.0f, 90,
			    MSMainProc::MSStageInfo::distFadeStageToKage);
		}
		break;

	case 2:
		if (gpCubeSoundChange->unk10 != 0) {
			if (param_1 == 8)
				smMSStage = new MSStageCubeFadeMonte;
#ifdef VERSION_GMSP01
			else if (param_1 == 1 && param_2 == 5)
				smMSStage = new MSStageCubeFadeDouble;
#endif
			else
				smMSStage = new MSStageCubeFade;
		}
		break;

	case 3:
		if (gpCubeSoundChange->unk10 == 1)
			smMSStage = new MSStageCubeSwitch;
		break;
	}

	switch (param_1) {
	case 0x3C:
	case 0x34:
		smMSStage = new MSSTageSimpleEnvironment;
		break;
	}

	return smMSStage;
}

void MSStage::stageLoop() { proc(); }

void MSStageProc::setBgmPosition(const Vec& pos, f32 dist, bool fade, u32 frame,
                                 u32 fadeFrame)
{
	Vec camPos = gpMSound->mAudioCameras->toCamSpace(pos);

	f32 pan   = MSHandle::calcPan(camPos, dist, 10000.0f);
	f32 dolby = MSHandle::calcDolby(camPos, dist);
	if (fade && frame < fadeFrame) {
		pan = (pan - 0.5f) * frame / fadeFrame;
		pan += 0.5f;
		dolby = dolby * frame / fadeFrame;
	}

	MSBgm::setPan(1, pan, 1, 0);
	MSBgm::setDolby(1, dolby, 1, 0);
}

MSStageDistFade::MSStageDistFade(const Vec* param_1, f32 param_2, f32 param_3,
                                 u32 param_4, bool param_5)
    : unk4(0)
    , unk8(param_2 == 0.0f ? 2000.0f : param_2)
    , unkC(param_3 == 0.0f ? 500.0f : param_3)
    , unk10(param_1)
    , unk14(param_4)
    , unk18(param_5)
{
	// TODO: hack, remove this!!!!
	(void)0;
	(void)0;
}

// TODO: fake and wrong
inline f32 vec_dist(const Vec& a, const Vec& b)
{
	return std::sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)
	                  + (a.z - b.z) * (a.z - b.z));
}

void MSStageDistFade::proc()
{
	JAISound* sound1 = MSBgm::getHandle(1);
	JAISound* sound2 = MSBgm::getHandle(0);
	if (sound1 == nullptr || sound2 == nullptr)
		return;

	f32 fVar8 = vec_dist(*unk10, SMS_GetMarioPos());
	f32 fVar1 = 0.0f;
	if (fVar8 < unkC) {
		fVar1 = 1.0f;
	} else if (fVar8 < unk8) {
		fVar1 = (unk8 - fVar8) / (unk8 - unkC);
	}

	if (unk4 <= unk14) {
		f32 fVar4;
		if (unk18 == 1) {
			fVar4 = fVar1 * unk4 / unk14;
		} else {
			fVar4 = (fVar1 - 1.0f) * unk4 / unk14 + 1.0f;
		}

		MSBgm::setTrackVolume(0, 1.0f - fVar4, 2, 0);
		MSBgm::setTrackVolume(1, fVar4, 2, 0);
		gpMSound->unk9C->unk0 = fVar4;
	} else {
		gpMSound->unk9C->xFadeBgm(fVar1);
	}

	MSStageProc::setBgmPosition(*unk10, fVar8, true, unk4, unk14);
	unk4 += 1;
}

MSStageDistFadeMonte::MSStageDistFadeMonte(const Vec* param_1, f32 param_2,
                                           f32 param_3, u32 param_4,
                                           bool param_5)
    : MSStageDistFade(param_1, param_2, param_3, param_4, param_5)
    , unk1C(3)
    , unk20(3)
{
}

void MSStageDistFadeMonte::proc()
{
	JAISound* sound1 = MSBgm::getHandle(1);
	JAISound* sound2 = MSBgm::getHandle(0);
	if (sound1 == nullptr || sound2 == nullptr)
		return;

	f32 fVar12 = vec_dist(*unk10, getCubeCheckPos(SMS_GetMarioPos()));
	f32 fVar2  = 0.0f;
	if (fVar12 < unkC) {
		fVar2 = 1.0f;
	} else if (fVar12 < unk8) {
		fVar2 = (unk8 - fVar12) / (unk8 - unkC);
	}

	if (unk4 < unk14)
		fVar2 = fVar2 * unk4 / unk14;

	unk1C = SMS_GetMonteVillageAreaInMario();
	if (unk1C == 0) {
		MSoundSESystem::MSoundSE::startSoundSystemSE(
		    MSD_SE_ENV_MONTE_UPPER_WIND, 0, nullptr, 0);
		if (unk20 != 0) {
			MSBgm::setTrackVolume(1, 0.0f, 10, 0);
			MSBgm::setTrackVolume(0, 1.0f, 10, 0);
		}
		gpMSound->unk9C->unk0 = fVar2;
	} else {
		if (unk20 == 0) {
			if (fVar2 == 1.0f) {
				MSBgm::setTrackVolume(1, 1.0f, 10, 0);
				MSBgm::setTrackVolume(0, 0.0f, 10, 0);
				gpMSound->unk9C->unk0 = fVar2;
			} else if (fVar2 == 0.0f) {
				MSBgm::setTrackVolume(1, 0.0f, 10, 0);
				MSBgm::setTrackVolume(0, 1.0f, 10, 0);
				gpMSound->unk9C->unk0 = fVar2;
			} else {
				gpMSound->unk9C->xFadeBgmForce(fVar2);
			}
		} else {
			gpMSound->unk9C->xFadeBgm(fVar2);
		}

		MSStageProc::setBgmPosition(*unk10, fVar12, true, unk4, unk14);
	}
	unk20 = unk1C;
	unk4 += 1;
}

MSStageCubeFade::MSStageCubeFade()
    : unk4(-1)
    , unk8(-1)
    , unkC(MSMainProc::MSStageInfo::cubeFadeRatio)
{
#ifdef VERSION_GMSP01
	smInstance = this;
#endif
}

void MSStageCubeFade::proc()
{
	JAISound* sound1 = MSBgm::getHandle(1);
	JAISound* sound0 = MSBgm::getHandle(0);
	if (sound1 == nullptr)
		return;

	Vec local_2c = SMS_GetMarioPos();
	local_2c.y   = getSoundCubeInfo(0)->unkC.y + 75.0f;

	unk4 = gpCubeSoundChange->getInCubeNo(local_2c);
	if (unk4 == -1) {
		if (unk8 != -1) {
			gpMSound->unk9C->unk0 = 0.0f;
		}
	} else {
		f32 ratio = calcParamRatioInCube(unk4);
		gpMSound->unk9C->xFadeBgm(ratio);
		if (MSMainProc::MSStageInfo::cubeFadeUsePan != 0) {
			Vec cubePos  = getSoundCubeInfo(unk4)->unkC;
			Vec marioPos = SMS_GetMarioPos();
			cubePos.y    = marioPos.y;
			f32 dist     = vec_dist(cubePos, marioPos);
			MSStageProc::setBgmPosition(cubePos, dist, false, 0, 0);
		}
	}
	unk8 = unk4;
}

#ifdef VERSION_GMSP01
void MSStageCubeFade::setBgmVolumeForce()
{
	s32 cubeNo
	    = gpCubeSoundChange->getInCubeNo(getCubeCheckPos(SMS_GetMarioPos()));
	f32 ratio;
	if (cubeNo != -1)
		ratio = calcParamRatioInCube(cubeNo);
	else
		ratio = 0.0f;

	gpMSound->unk9C->xFadeBgmForce(ratio);
}

MSStageCubeFadeDouble::MSStageCubeFadeDouble()
{
	mPlayFlag[0] = false;
	mPlayFlag[1] = false;
	smInstance   = this;
}

void MSStageCubeFadeDouble::proc()
{
	JAISound* sound1 = MSBgm::getHandle(1);
	JAISound* sound0 = MSBgm::getHandle(0);
	if (sound1 == nullptr)
		return;

	Vec local_2c = SMS_GetMarioPos();
	local_2c.y   = getSoundCubeInfo(0)->unkC.y + 75.0f;

	unk4 = gpCubeSoundChange->getInCubeNo(local_2c);

	bool play = false;
	if ((unk4 == 0 || unk4 == 1) && mPlayFlag[unk4])
		play = true;

	if (!play) {
		if (unk8 != -1) {
			gpMSound->unk9C->unk0 = 0.0f;
			gpMSound->unk9C->xFadeBgmForce(0.0f);
		}
	} else {
		gpMSound->unk9C->xFadeBgm(calcParamRatioInCube(unk4));
		if (MSMainProc::MSStageInfo::cubeFadeUsePan != 0) {
			Vec cubePos  = getSoundCubeInfo(unk4)->unkC;
			Vec marioPos = SMS_GetMarioPos();
			cubePos.y    = marioPos.y;
			f32 dist     = vec_dist(cubePos, marioPos);
			MSStageProc::setBgmPosition(cubePos, dist, false, 0, 0);
		}
	}
	unk8 = unk4;
}
#endif

MSStageCubeFadeMonte::MSStageCubeFadeMonte()
    : unk10(3)
    , unk14(3)
{
}

void MSStageCubeFadeMonte::proc()
{
	JAISound* sound1 = MSBgm::getHandle(1);
	JAISound* sound0 = MSBgm::getHandle(0);
	if (sound1 == nullptr)
		return;

	Vec local_2c = SMS_GetMarioPos();
	local_2c.y   = getSoundCubeInfo(0)->unkC.y + 75.0f;

	unk4  = gpCubeSoundChange->getInCubeNo(local_2c);
	unk10 = SMS_GetMonteVillageAreaInMario();

	f32 fVar2 = unk4 != -1 ? calcParamRatioInCube(unk4) : 0.0f;

	if (unk10 == 0) {
		MSoundSESystem::MSoundSE::startSoundSystemSE(
		    MSD_SE_ENV_MONTE_UPPER_WIND, 0, nullptr, 0);
		if (unk14 != 0) {
			MSBgm::setTrackVolume(1, 0.0f, 10, 0);
			MSBgm::setTrackVolume(0, 1.0f, 10, 0);
		}
		gpMSound->unk9C->unk0 = fVar2;
	} else {
		if (unk14 == 0) {
			if (fVar2 == 1.0f) {
				MSBgm::setTrackVolume(1, 1.0f, 10, 0);
				MSBgm::setTrackVolume(0, 0.0f, 10, 0);
				gpMSound->unk9C->unk0 = fVar2;
			} else if (fVar2 == 0.0f) {
				MSBgm::setTrackVolume(1, 0.0f, 10, 0);
				MSBgm::setTrackVolume(0, 1.0f, 10, 0);
				gpMSound->unk9C->unk0 = fVar2;
			} else {
				gpMSound->unk9C->xFadeBgmForce(fVar2);
			}
		} else if (unk4 == -1) {
			if (unk8 != -1) {
				MSBgm::setTrackVolume(1, 0.0f, 10, 0);
				MSBgm::setTrackVolume(0, 1.0f, 10, 0);
				gpMSound->unk9C->unk0 = 0.0f;
			}
		} else {
			gpMSound->unk9C->xFadeBgm(fVar2);
		}

		if (unk4 != -1 && MSMainProc::MSStageInfo::cubeFadeUsePan) {
			Vec cubePos  = getSoundCubeInfo(unk4)->unkC;
			Vec marioPos = SMS_GetMarioPos();
			cubePos.y    = marioPos.y;
			f32 dist     = vec_dist(cubePos, marioPos);
			MSStageProc::setBgmPosition(cubePos, dist, false, 0, 0);
		}
	}

	unk8  = unk4;
	unk14 = unk10;
}

f32 MSStageCubeFade::calcParamRatioInCube(s32 id)
{
	f32 x = 0.0f;
	f32 y = 0.0f;
	f32 z = 0.0f;

	Vec pos = SMS_GetMarioPos();
	pos.y   = getSoundCubeInfo(id)->unkC.y + 75.0f;

	gpCubeSoundChange->calcPointInCubeRatio(pos, id, &x, &y, &z);

	f32 dx = std::fabs(x - 0.5f);
	f32 dz = std::fabs(z - 0.5f);
	f32 d  = dx > dz ? dx : dz;

	return d < unkC ? 1.0f : (0.5f - d) / (0.5f - unkC);
}

void MSStageCubeSwitch::proc()
{
	unk4 = gpCubeSoundChange->getInCubeNo(getCubeCheckPos(SMS_GetMarioPos()));

	switch (unk4) {
	case -1:
		if (unk8 == 0) {
			unk10 = 0;
			unk11 = 0;
		} else {
			unk10 = !unk10 ? !SMS_IsMarioStatusTypeJumping() : true;
			if (unk10 == true && unk11 == 0)
				toStageBgm();
			unk11 = unk10;
		}
		break;
	case 0:
		if (unk8 == -1) {
			unk10 = 0;
			unk11 = 0;
		} else {
			unk10 = !unk10 ? !SMS_IsMarioStatusTypeJumping() : true;
			if (unk10 == true && unk11 == 0)
				toBossBgm();

			unk11 = unk10;
		}
		break;
	}
	unk8 = unk4;
}

void MSStageCubeSwitch::toStageBgm()
{
	if (!MSMainProc::MSStageInfo::bossLives)
		return;

	MSBgm::stopTrackBGM(1, 10);
	MSBgm::setTrackVolume(0, 1.0f, 15, 0);
}

void MSStageCubeSwitch::toBossBgm()
{
	if (!MSMainProc::MSStageInfo::bossLives)
		return;

	MSBgm::setTrackVolume(0, 0.0f, 15, 0);
	if (!MSMainProc::MSStageInfo::bossNotDamaged)
		MSBgm::startBGM(MSMainProc::MSStageInfo::switchBgm2);
	else
		MSBgm::startBGM(MSMainProc::MSStageInfo::switchBgm);
}

void MSSTageSimpleEnvironment::proc()
{
	if (unk4 != 0xffffffff)
		MSoundSESystem::MSoundSE::startSoundSystemSE(unk4, 0, nullptr, 0);
}

void MSSTageSimpleEnvironmentMonte::proc()
{
	if (unk4 != 0xffffffff)
		MSoundSESystem::MSoundSE::startSoundSystemSE(unk4, 0, nullptr, 0);
}
