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

// rogue includes needed for matching __sinit: the JAL sound lists register in
// reverse declaration order, and the target registers MSBgm first, then
// MSSetSoundGrp/MSSetSound, so these two must be the last includes with no
// earlier copy of MSoundBGM.hpp.
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

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

MSStage* MSStage::smMSStage;
MSStageCubeFade* MSStageCubeFade::smInstance;
MSStageCubeFadeDouble* MSStageCubeFadeDouble::smInstance;

void MSMainProc::setGateKeeperBGMPlayFlag(u32 param_1, bool param_2)
{
	if (MSStageCubeFadeDouble::smInstance == nullptr)
		return;

	if (param_1 == 3)
		MSStageCubeFadeDouble::smInstance->unk10[0] = param_2;
	else if (param_1 == 4)
		MSStageCubeFadeDouble::smInstance->unk10[1] = param_2;
}

bool MSMainProc::getGateKeeperBGMStopFlag()
{
	if (MSStageCubeFadeDouble::smInstance != nullptr) {
		for (u8 i = 0; i < 2; i++) {
			if (MSStageCubeFadeDouble::smInstance->unk10[i] != 0)
				return false;
		}
	}
	return true;
}

int MSMainProc::getMonteVillageActorArea(const Vec& param_1)
{
	int result = 4;
	if (MSGMSound->unkCD == 8) {
		// inline?
		Vec local_10 = param_1;
		local_10.y += 75.0f;
		Vec copy = local_10;
		switch (gpCubeFastC->getInCubeNo(copy)) {
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

// UNUSED, 0x10: four instructions, and the only flag of the array nothing
// else raises is the one endStageEntranceDemo clears.
void MSMainProc::entranceDemoWipeInEnd() { gpMSound->unkC8[4] = 1; }

void MSMainProc::toInnerCameraDemo() { gpMSound->unkC8[2] = 1; }

void MSMainProc::fromInnerCameraDemo() { gpMSound->unkC8[2] = 0; }

// UNUSED, 0x38 and 0x34: the demo enter/leave pair of
// startStageEntranceDemo/endStageEntranceDemo on the one free flag slot.
void MSMainProc::toTHPDemo()
{
	gpMSound->demoModeIn(MSStageInfo::volOffCategory, false);
	gpMSound->unkC8[3] = 1;
}

void MSMainProc::fromTHPDemo()
{
	gpMSound->demoModeOut(false);
	gpMSound->unkC8[3] = 0;
}

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

	MSStageCubeFadeDouble::smInstance = nullptr;

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
		MSoundSESystem::MSRandPlay::createRandPlayVec(
		    MSD_SE_OBJ_BIRD_BIA_1, 8);
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
	case 8: {
		int scenario = param_2;

		// Even episodes get the night ambience, odd ones the day set.
		switch (scenario) {
		case 0:
		case 2:
		case 4:
		case 6:
			MSoundSESystem::MSRandPlay::createRandPlayVec(
			    MSD_SE_OBJ_MONTE_NIGHT_A1, 5);
			break;
		default:
			MSoundSESystem::MSRandPlay::createRandPlayVec(
			    MSD_SE_OBJ_MONTE_DAY_A1, 5);
			break;
		}
		switch (scenario) {
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
		} else if (param_2 == 1) {
			break;
		} else {
			MSStageInfo::switchBgm  = 0xfffffff0;
			MSStageInfo::fadeEvent  = 2;
			MSStageInfo::switchBgm2 = 0xfffffff0;

			switch (scenario) {
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
	}
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
		} else if (param_2 == 6) {
			// Episode 6 keeps the plain Mare Sea setup.
		} else {
			MSStageInfo::stageBgmSilent            = MSD_BGM_SCENARIO_SELECT;
			MSStageInfo::cubeFadeRatio             = 0.12f;
			MSStageInfo::stageBgmSilentStartStatus = 2;
			MSStageInfo::fadeEvent                 = 2;
			MSStageInfo::switchBgm                 = 0xfffffff0;
			MSStageInfo::switchBgm2                = 0xfffffff0;
			MSStageInfo::cubeFadeUsePan            = 1;
		}
		if (param_2 == 6 || param_2 == 7)
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
		bVar2                 = true;
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
#if defined(VERSION_GMSE01)
	gpMSound->mWaterFilterOverride = bVar2;
#endif
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
			else if (param_1 == 1 && param_2 == 5)
				smMSStage = new MSStageCubeFadeDouble;
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

void MSStageProc::setBgmPosition(const Vec&, f32, bool, u32, u32) { }

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

static f32 vec_dist(const Vec& a, const Vec& b)
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

	// TODO: inline?
	u32 r29 = unk14;
	u32 r30 = unk4;

	Vec local_68 = gpMSound->mAudioCameras->toCamSpace(*unk10);

	f32 dVar6 = MSHandle::calcPan(local_68, fVar8, 10000.0f);
	f32 dVar7 = MSHandle::calcDolby(local_68, fVar8);
	if (r29 < r30) {
		dVar6 = (dVar6 - 0.5f) * r30 / r29;
		dVar6 += 0.5f;
		dVar7 = dVar7 * r30 / r29;
	}

	MSBgm::setPan(1, dVar6, 1, 0);
	MSBgm::setDolby(1, dVar7, 1, 0);
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

	Vec marioPos = SMS_GetMarioPos();
	marioPos.y += 75.0f;
	Vec marioPos2 = marioPos;
	f32 fVar12    = vec_dist(*unk10, marioPos2);
	f32 fVar2     = 0.0f;
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

		// TODO: inline?
		u32 uVar4 = unk14;
		u32 uVar8 = unk4;

		Vec local_88 = gpMSound->mAudioCameras->toCamSpace(*unk10);
		f32 dVar10   = MSHandle::calcPan(local_88, fVar12, 10000.0f);
		f32 dVar11   = MSHandle::calcDolby(local_88, fVar12);
		if (uVar8 < uVar4) {
			dVar10 = 0.5f + (dVar10 - 0.5f) * uVar8 / uVar4;
			dVar11 = dVar11 * uVar8 / uVar4;
		}
		MSBgm::setPan(1, dVar10, 1, 0);
		MSBgm::setDolby(1, dVar11, 1, 0);
	}
	unk20 = unk1C;
	unk4 += 1;
}

MSStageCubeFade::MSStageCubeFade()
    : unk4(-1)
    , unk8(-1)
    , unkC(MSMainProc::MSStageInfo::cubeFadeRatio)
{
	smInstance = this;
}

void MSStageCubeFade::proc()
{
	JAISound* sound1 = MSBgm::getHandle(1);
	JAISound* sound0 = MSBgm::getHandle(0);
	if (sound1 == nullptr)
		return;

	Vec local_164 = *gpMarioPos;
	local_164.y   = 75.0f + gpCubeSoundChange->unk14->begin()[0]->unkC.y;

	unk4 = gpCubeSoundChange->getInCubeNo(local_164);
	if (unk4 == -1) {
		if (unk8 != -1) {
			gpMSound->unk9C->unk0 = 0.0f;
		}
	} else {
		gpMSound->unk9C->xFadeBgm(calcParamRatioInCube(unk4));
		if (MSMainProc::MSStageInfo::cubeFadeUsePan != 0) {
			Vec local_158 = gpCubeSoundChange->unk14->begin()[unk4]->unkC;
			Vec local_14c = *gpMarioPos;

			// The pan is taken at Mario's own height, so the fade only
			// reacts to the horizontal distance to the cube's centre.
			local_158.y = local_14c.y;

			f32 d = vec_dist(local_158, local_14c);

			Vec local_c0 = gpMSound->mAudioCameras->toCamSpace(local_158);
			f32 dVar6    = MSHandle::calcPan(local_c0, d, 10000.0f);
			f32 dVar7    = MSHandle::calcDolby(local_c0, d);
			MSBgm::setPan(1, dVar6, 1, 0);
			MSBgm::setDolby(1, dVar7, 1, 0);
		}
	}
	unk8 = unk4;
}

void MSStageCubeFade::setBgmVolumeForce()
{
	Vec local_7c = *gpMarioPos;
	local_7c.y += 75.0f;
	Vec local_88 = local_7c;

	s32 r30 = gpCubeSoundChange->getInCubeNo(local_88);

	f32 fVar1;
	if (r30 != -1)
		fVar1 = calcParamRatioInCube(r30);
	else
		fVar1 = 0.0f;

	gpMSound->unk9C->xFadeBgmForce(fVar1);
}

MSStageCubeFadeDouble::MSStageCubeFadeDouble()
{
	unk10[0] = 0;
	unk10[1] = 0;
	smInstance = this;
}

void MSStageCubeFadeDouble::proc()
{
	JAISound* sound1 = MSBgm::getHandle(1);
	JAISound* sound0 = MSBgm::getHandle(0);
	if (sound1 == nullptr)
		return;

	Vec local_170 = *gpMarioPos;
	local_170.y   = 75.0f + gpCubeSoundChange->unk14->begin()[0]->unkC.y;

	unk4 = gpCubeSoundChange->getInCubeNo(local_170);

	bool bVar1 = false;
	if ((unk4 == 0 || unk4 == 1) && unk10[unk4] != 0)
		bVar1 = true;

	if (!bVar1) {
		if (unk8 != -1) {
			gpMSound->unk9C->unk0 = 0.0f;
			gpMSound->unk9C->xFadeBgmForce(0.0f);
		}
	} else {
		gpMSound->unk9C->xFadeBgm(calcParamRatioInCube(unk4));
		if (MSMainProc::MSStageInfo::cubeFadeUsePan != 0) {
			Vec local_160 = gpCubeSoundChange->unk14->begin()[unk4]->unkC;
			Vec local_154 = *gpMarioPos;

			local_160.y = local_154.y;

			f32 d = vec_dist(local_160, local_154);

			Vec local_c8 = gpMSound->mAudioCameras->toCamSpace(local_160);
			f32 dVar6    = MSHandle::calcPan(local_c8, d, 10000.0f);
			f32 dVar7    = MSHandle::calcDolby(local_c8, d);
			MSBgm::setPan(1, dVar6, 1, 0);
			MSBgm::setDolby(1, dVar7, 1, 0);
		}
	}
	unk8 = unk4;
}

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

	Vec local_19c = *gpMarioPos;
	local_19c.y   = 75.0f + gpCubeSoundChange->unk14->begin()[0]->unkC.y;

	unk4  = gpCubeSoundChange->getInCubeNo(local_19c);
	unk10 = SMS_GetMonteVillageAreaInMario();

	f32 fVar2;
	if (unk4 != -1)
		fVar2 = calcParamRatioInCube(unk4);
	else
		fVar2 = 0.0f;

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
			Vec local_190 = gpCubeSoundChange->unk14->begin()[unk4]->unkC;
			Vec local_184 = *gpMarioPos;

			local_190.y = local_184.y;

			f32 d = vec_dist(local_190, local_184);

			Vec local_e0 = gpMSound->mAudioCameras->toCamSpace(local_190);
			f32 dVar6    = MSHandle::calcPan(local_e0, d, 10000.0f);
			f32 dVar7    = MSHandle::calcDolby(local_e0, d);
			MSBgm::setPan(1, dVar6, 1, 0);
			MSBgm::setDolby(1, dVar7, 1, 0);
		}
	}

	unk8  = unk4;
	unk14 = unk10;
}

f32 MSStageCubeFade::calcParamRatioInCube(s32 id)
{
	Vec local_74;
	local_74.x = 0.0f;
	local_74.y = 0.0f;
	local_74.z = 0.0f;

	Vec local_68 = *gpMarioPos;
	local_68.y = 75.0f
	             + gpCubeSoundChange->unk14->getChildren().begin()[id]->unkC.y;

	gpCubeSoundChange->calcPointInCubeRatio(local_68, id, &local_74.x,
	                                        &local_74.y, &local_74.z);

	f32 local_ac = local_74.x - 0.5f;
	f32 local_b0 = local_74.z - 0.5f;
	local_ac     = std::fabs(local_ac);
	local_b0     = std::fabs(local_b0);
	f32 fVar2    = local_ac > local_b0 ? local_ac : local_b0;

	if (fVar2 < unkC)
		return 1.0f;
	return (0.5f - fVar2) / (0.5f - unkC);
}

void MSStageCubeSwitch::proc()
{
	Vec tmp = SMS_GetMarioPos();
	tmp.y += 75.0f;
	Vec local_18 = tmp;

	unk4 = gpCubeSoundChange->getInCubeNo(local_18);

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
