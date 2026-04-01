#include <System/MSoundMainSide.hpp>
#include <System/MarDirector.hpp>
#include <System/FlagManager.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MSound/MSModBgm.hpp>
#include <MSound/MSHandle.hpp>
#include <MarioUtil/MapUtil.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Camera/CubeMapTool.hpp>
#include <Player/MarioAccess.hpp>

MSStage* MSStage::smMSStage;

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
	MSStageInfo::demoBgm                   = 0x80010017;
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
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x8001001a);
		MSStageInfo::stageBgm = 0x8001001a;
		break;
	case 1:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010001);
		MSStageInfo::stageBgm = 0x80010001;
		if (true) {
			switch (param_2) {
			case 0:
				MSStageInfo::fadeEvent     = 2;
				MSStageInfo::switchBgm     = 0xfffffff0;
				MSStageInfo::switchBgm2    = 0xfffffff0;
				MSStageInfo::cubeFadeRatio = 0.22f;
				break;
			case 1:
				MSStageInfo::demoBgm = 0x80010016;
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
					MSStageInfo::demoBgm = 0x80010016;
					MSStageInfo::flags   = 6;
					MSStageInfo::volOffCategory -= 0x83;
					MSStageInfo::fadeEvent  = 1;
					MSStageInfo::switchBgm  = 0xfffffff0;
					MSStageInfo::switchBgm2 = 0xfffffff0;
				}
				break;
			case 9:
				MSStageInfo::demoBgm = 0x80010016;
				MSStageInfo::flags   = 0;
				MSStageInfo::volOffCategory -= 0x82;
			}
		}
		break;
	case 2:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010002);
		MSStageInfo::stageBgm = 0x80010002;
		switch (param_2) {
		case 0:
			MSStageInfo::demoBgm       = 0x80010027;
			MSStageInfo::flags         = 10;
			MSStageInfo::fadeEvent     = 2;
			MSStageInfo::switchBgm     = 0xfffffff0;
			MSStageInfo::switchBgm2    = 0xfffffff0;
			MSStageInfo::cubeFadeRatio = 0.28f;
			break;

		case 6:
			MSStageInfo::demoBgm = 0x80010016;
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
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010009);
		MSStageInfo::stageBgm = 0x80010009;
		if (param_2 == 6) {
			MSStageInfo::demoBgm = 0x80010016;
			MSStageInfo::flags   = 3;
			MSStageInfo::volOffCategory -= 0x83;
			MSStageInfo::fadeEvent           = 1;
			MSStageInfo::switchBgm           = 0xfffffff0;
			MSStageInfo::switchBgm2          = 0xfffffff0;
			MSStageInfo::distFadeStageToKage = 0;
		}
		if (param_2 == 4) {
			MSStageInfo::switchBgm     = 0x8001000d;
			MSStageInfo::fadeEvent     = 3;
			MSStageInfo::switchBgm2    = 0x8001002a;
			MSStageInfo::cubeFadeRatio = 0.28f;
		}
		break;
	case 4:
		if (param_2 != 2) {
			MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010003);
			MSStageInfo::stageBgm = 0x80010003;
		} else {
			MSStageInfo::msStg    = MSBgm::getSceneNo(0x8001000d);
			MSStageInfo::stageBgm = 0x8001000d;
		}
		if (param_2 == 6) {
			MSStageInfo::switchBgm = 0xfffffff0;
			MSStageInfo::demoBgm   = 0x80010016;
			MSStageInfo::flags     = 3;
			MSStageInfo::volOffCategory -= 0x83;
			MSStageInfo::fadeEvent           = 1;
			MSStageInfo::switchBgm2          = 0xfffffff0;
			MSStageInfo::distFadeStageToKage = 0;
		} else if (param_2 == 2) {
			MSStageInfo::demoBgm = 0x8001000d;
			MSStageInfo::flags   = 0;
		}
		break;
	case 5:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010004);
		MSStageInfo::stageBgm = 0x80010004;
		switch (param_2) {
		case 0:
			MSStageInfo::flags   = 10;
			MSStageInfo::demoBgm = 0x80010027;
			break;
		case 1:
			MSStageInfo::volOffCategory -= 4;
			break;
		}
		break;
	case 6:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010008);
		MSStageInfo::stageBgm = 0x80010008;
		break;
	case 7:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010013);
		MSStageInfo::stageBgm = 0x80010013;
		if (param_2 == 3) {
			MSStageInfo::fadeEvent                 = 1;
			MSStageInfo::switchBgm                 = 0xfffffff0;
			MSStageInfo::switchBgm2                = 0xfffffff0;
			MSStageInfo::stageBgm                  = 0x80010016;
			MSStageInfo::stageBgmSilent            = 0x80010013;
			MSStageInfo::stageBgmSilentStartStatus = 2;
			MSStageInfo::distFadeStageToKage       = 0;
		}
		break;
	case 8:
		switch (param_2) {
		case 5:
			MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010022);
			MSStageInfo::stageBgm = 0x80010022;
			break;
		default:
			MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010007);
			MSStageInfo::stageBgm = 0x80010007;
			break;
		}
		if (param_2 == 6) {
			MSStageInfo::switchBgm = 0xfffffff0;
			MSStageInfo::demoBgm   = 0x80010016;
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
				MSStageInfo::stageBgmSilent            = 0x8001002c;
				MSStageInfo::stageBgmSilentStartStatus = 2;
			} else {
				MSStageInfo::stageBgmSilent            = 0x80010018;
				MSStageInfo::stageBgmSilentStartStatus = 0;
			}
		}
		break;
	case 9:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010014);
		MSStageInfo::stageBgm = 0x80010014;
		if (param_2 == 6) {
			MSStageInfo::demoBgm = 0x80010016;
			MSStageInfo::flags   = 3;
			MSStageInfo::volOffCategory -= 0x83;
			MSStageInfo::fadeEvent           = 1;
			MSStageInfo::switchBgm           = 0xfffffff0;
			MSStageInfo::switchBgm2          = 0xfffffff0;
			MSStageInfo::distFadeStageToKage = 0;
		} else if (param_2 == 1) {
			MSStageInfo::switchBgm  = 0x8001000b;
			MSStageInfo::fadeEvent  = 3;
			MSStageInfo::switchBgm2 = 0x8001000b;
		}
		break;
	case 13:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010005);
		MSStageInfo::stageBgm = 0x80010005;
		if (param_2 == 4) {
			MSStageInfo::fadeEvent                 = 1;
			MSStageInfo::switchBgm                 = 0xfffffff0;
			MSStageInfo::switchBgm2                = 0xfffffff0;
			MSStageInfo::stageBgm                  = 0x80010016;
			MSStageInfo::stageBgmSilent            = 0x80010005;
			MSStageInfo::stageBgmSilentStartStatus = 2;
			MSStageInfo::distFadeStageToKage       = 0;
		} else if (param_2 == 0) {
			MSStageInfo::fadeEvent                 = 1;
			MSStageInfo::switchBgm                 = 0xfffffff0;
			MSStageInfo::switchBgm2                = 0xfffffff0;
			MSStageInfo::stageBgm                  = 0x80010016;
			MSStageInfo::stageBgmSilent            = 0x80010005;
			MSStageInfo::stageBgmSilentStartStatus = 2;
			MSStageInfo::distFadeStageToKage       = 0;
		} else {
			MSStageInfo::stageBgmSilent            = 0x80010023;
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
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x8001001e);
		MSStageInfo::stageBgm = 0x8001001e;
		break;
	case 15:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010010);
		MSStageInfo::stageBgm = 0x80010010;
		SMSGetMSound()->loadArcSeqData(0x80010010, false);
		break;
	case 16:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010006);
		bVar2                 = true;
		MSStageInfo::stageBgm = 0x80010006;
		break;
	case 20:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010001);
		MSStageInfo::stageBgm = 0x80010001;
		MSStageInfo::demoBgm  = 0x80010001;
		MSStageInfo::flags    = 0;
		break;
	case 21:
	case 22:
	case 23:
	case 24:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010021);
		MSStageInfo::stageBgm = 0x80010021;
		MSStageInfo::demoBgm  = 0x80010021;
		MSStageInfo::flags    = 0;
		break;
	case 28:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010012);
		MSStageInfo::stageBgm = 0x80010012;
		MSStageInfo::demoBgm  = 0x80010012;
		MSStageInfo::flags    = 0;
		break;
	case 29:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010021);
		MSStageInfo::stageBgm = 0x80010021;
		MSStageInfo::demoBgm  = 0x80010021;
		MSStageInfo::flags    = 0;
		break;
	case 30:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010012);
		MSStageInfo::stageBgm = 0x80010012;
		break;
	case 31:
	case 32:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010012);
		MSStageInfo::stageBgm = 0x80010012;
		MSStageInfo::demoBgm  = 0x80010012;
		MSStageInfo::flags    = 0;
		break;
	case 33:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010021);
		MSStageInfo::stageBgm = 0x80010021;
		MSStageInfo::demoBgm  = 0x80010021;
		MSStageInfo::flags    = 0;
		break;
	case 34:
	case 35:
	case 40:
	case 41:
	case 42:
	case 43:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010012);
		MSStageInfo::stageBgm = 0x80010012;
		MSStageInfo::demoBgm  = 0x80010012;
		MSStageInfo::flags    = 0;
		break;
	case 44:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010021);
		bVar2                 = true;
		MSStageInfo::stageBgm = 0x80010021;
		MSStageInfo::demoBgm  = 0x80010021;
		MSStageInfo::flags    = 0;
		break;
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010012);
		MSStageInfo::stageBgm = 0x80010012;
		MSStageInfo::demoBgm  = 0x80010012;
		MSStageInfo::flags    = 0;
		break;
	case 52:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010015);
		MSStageInfo::stageBgm = 0x80010015;
		MSStageInfo::demoBgm  = 0x80010015;
		MSStageInfo::flags    = 0;
		break;
	case 55:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x8001000d);
		MSStageInfo::stageBgm = 0x8001000d;
		break;
	case 57:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x8001000d);
		MSStageInfo::stageBgm = 0x8001000d;
		break;
	case 58:
		switch (param_2) {
		case 0:
			MSStageInfo::msStg    = MSBgm::getSceneNo(0x8001001f);
			MSStageInfo::stageBgm = 0x8001001f;
			break;
		case 1:
			MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010019);
			MSStageInfo::stageBgm = 0x80010019;
			break;
		}
		break;
	case 59:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x80010009);
		MSStageInfo::stageBgm = 0x80010009;
		MSBgm::startBGM(0x8001000d);
		MSStageInfo::fadeEvent  = 2;
		MSStageInfo::flags      = 3;
		MSStageInfo::switchBgm  = 0xfffffff0;
		MSStageInfo::switchBgm2 = 0xfffffff0;
		break;
	case 60:
		MSStageInfo::msStg    = MSBgm::getSceneNo(0x8001002e);
		MSStageInfo::stageBgm = 0x8001002e;
		MSStageInfo::demoBgm  = 0x8001002e;
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

MSStage* MSStage::init(u8 param_1, u8)
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
			    gpMSound->unkAC[1].unk0, fVar1, fVar2, uVar6,
			    MSMainProc::MSStageInfo::distFadeStageToKage);
		} else {
			smMSStage = new MSStageDistFadeMonte(
			    gpMSound->unkAC[1].unk0, 6000.0f, 1600.0f, 90,
			    MSMainProc::MSStageInfo::distFadeStageToKage);
		}
		break;

	case 2:
		if (gpCubeSoundChange->unk10 != 0) {
			if (param_1 == 8)
				smMSStage = new MSStageCubeFadeMonte;
			else
				smMSStage = new MSStageCubeFade;
		}
		break;

	case 3:
		if (gpCubeSoundChange->unk10 == 1)
			smMSStage = new MSStageCubeSwitch(param_1);
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

	Vec local_68 = gpMSound->unk8->toCamSpace(*unk10);

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
		MSoundSESystem::MSoundSE::startSoundSystemSE(0x5023, 0, nullptr, 0);
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

		Vec local_88 = gpMSound->unk8->toCamSpace(*unk10);
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
}

void MSStageCubeFade::proc()
{
	JAISound* sound1 = MSBgm::getHandle(1);
	JAISound* sound0 = MSBgm::getHandle(0);
	if (sound1 == nullptr)
		return;

	Vec local_2c = SMS_GetMarioPos();
	local_2c.y   = gpCubeSoundChange->unk14->begin()[0]->unkC.y + 75.0f;

	unk4 = gpCubeSoundChange->getInCubeNo(local_2c);
	if (unk4 == -1) {
		if (unk8 != -1) {
			gpMSound->unk9C->unk0 = 0.0f;
		}
	} else {
		Vec local_74;
		local_74.x = 0.0f;
		local_74.y = 0.0f;
		local_74.z = 0.0f;

		Vec local_68 = SMS_GetMarioPos();
		local_68.y   = gpCubeSoundChange->unk14->begin()[unk4]->unkC.y + 65.0f;

		gpCubeSoundChange->calcPointInCubeRatio(local_68, unk4, &local_74.x,
		                                        &local_74.y, &local_74.z);

		f32 local_ac = local_74.x - 0.5f;
		f32 local_b0 = local_74.z - 0.5f;
		local_ac     = std::fabs(local_ac);
		local_b0     = std::fabs(local_b0);
		f32 fVar2    = local_ac > local_b0 ? local_ac : local_b0;

		f32 fVar1;
		if (fVar2 < unkC)
			fVar1 = 1.0f;
		else
			fVar1 = (0.5f - fVar2) / (0.5f - unkC);

		gpMSound->unk9C->xFadeBgm(fVar1);
		if (MSMainProc::MSStageInfo::cubeFadeUsePan != 0) {
			TCubeGeneralInfo* info = gpCubeSoundChange->unk14->begin()[unk4];

			Vec local_d0 = info->getUnkC();
			Vec marioPos = SMS_GetMarioPos();

			f32 d = vec_dist(local_d0, marioPos);

			Vec local_98 = gpMSound->unk8->toCamSpace(local_2c);
			f32 dVar6    = MSHandle::calcPan(local_98, d, 10000.0f);
			f32 dVar7    = MSHandle::calcDolby(local_98, d);
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

	Vec local_2c = SMS_GetMarioPos();
	local_2c.y   = gpCubeSoundChange->unk14->begin()[0]->unkC.y + 75.0f;

	unk4  = gpCubeSoundChange->getInCubeNo(local_2c);
	unk10 = SMS_GetMonteVillageAreaInMario();

	f32 fVar2;
	if (unk4 != -1) {

		// TODO: this is probably calcParamRatioInCube?
		Vec local_74;
		local_74.x = 0.0f;
		local_74.y = 0.0f;
		local_74.z = 0.0f;

		Vec local_68 = SMS_GetMarioPos();
		local_68.y   = gpCubeSoundChange->unk14->begin()[unk4]->unkC.y + 65.0f;

		gpCubeSoundChange->calcPointInCubeRatio(local_68, unk4, &local_74.x,
		                                        &local_74.y, &local_74.z);

		f32 local_ac = local_74.x - 0.5f;
		f32 local_b0 = local_74.z - 0.5f;
		local_ac     = std::fabs(local_ac);
		local_b0     = std::fabs(local_b0);
		f32 fVar3    = local_ac > local_b0 ? local_ac : local_b0;

		if (fVar3 < unkC)
			fVar2 = 1.0f;
		else
			fVar2 = (0.5f - fVar3) / (0.5f - unkC);

	} else {
		fVar2 = 0.0f;
	}

	if (unk10 == 0) {
		MSoundSESystem::MSoundSE::startSoundSystemSE(0x5023, 0, nullptr, 0);
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

			Vec local_e8           = SMS_GetMarioPos();
			TCubeGeneralInfo* info = gpCubeSoundChange->unk14->begin()[unk4];

			Vec local_d0        = info->getUnkC();
			const Vec& marioPos = SMS_GetMarioPos();

			f32 d        = vec_dist(local_d0, marioPos);
			Vec local_a4 = gpMSound->unk8->toCamSpace(local_e8);

			f32 dVar6 = MSHandle::calcPan(local_a4, d, 10000.0f);
			f32 dVar7 = MSHandle::calcDolby(local_a4, d);
			MSBgm::setPan(1, dVar6, 1, 0);
			MSBgm::setDolby(1, dVar7, 1, 0);
		}
	}

	unk8  = unk4;
	unk14 = unk10;
}

f32 MSStageCubeFade::calcParamRatioInCube(s32 id) { }

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

void MSSTageSimpleEnvironmentMonte::proc() { }
