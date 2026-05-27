#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MSound/MSound.hpp>
#include <JSystem/JAudio/JALibrary/JALModSe.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>

MSBgm* MSBgm::smBgmInTrack[3];
f32 MSBgm::smMainVolume = 0.75f;

void MSBgm::init()
{
	u16 count = MSGMSound->unk0->unk88.unk2[16];
	for (u32 i = 1; i < count; ++i)
		new MSBgm(i);

	smBgmInTrack[0] = nullptr;
	smBgmInTrack[1] = nullptr;
	smBgmInTrack[2] = nullptr;
}

JAISound* MSBgm::startBGM(u32 param)
{
	MSBgm* iVar1 = JALListS<MSBgm, u32>::search(param & 0x3FF);
	if (iVar1) {
		if ((param == MSD_BGM_CHUBOSS) || (param == MSD_BGM_BOSS)
		    || (param == MSD_BGM_BOSSHANA_2ND3RD)) {
			for (u8 i = 0; i < 3; i++) {
				if ((3 >> i & 1) != 0) {
					stopTrackBGM(i, 0);
				}
			}
			MSGMSound->demoModeIn(0x16f, false);
		}
		MSGBasic->startSoundActor(param, &iVar1->unk14, nullptr, 0, 4);
		if (iVar1->unk14) {
			iVar1->unk14->setVolume(smMainVolume, 0, 8);

			if (iVar1->unk14->unk0 < 3) {
				smBgmInTrack[iVar1->unk14->unk0] = iVar1;
			}
			int uVar2 = getSceneNo(param);
			if (uVar2 != -1 && uVar2 != 0x210) {
				u8 temp  = uVar2 >> 8;
				u8 temp2 = uVar2;
				iVar1->unk14->setWaveReadMode(temp, temp2);
			}
			return iVar1->unk14;
		}
	}

	return 0;
}

void MSBgm::stopBGM(u32 param1, u32 param2)
{
	MSBgm* track = smBgmInTrack[param1];
	u32 stack; /* Unused */
	MSBgm* iVar2;
	JAISound* audio; /*Unused*/
	if (param1 == -1) {
		if (smBgmInTrack[0] && smBgmInTrack[0]->unk14) {
			smBgmInTrack[0]->unk14->stop(param2);
			smBgmInTrack[0] = nullptr;
		}
	} else {
		iVar2 = JALListS<MSBgm, u32>::search(param1 & 0x3FF);
		if (iVar2 && iVar2->unk14) {
			u8 a = iVar2->unk14->unk0;
			if (a == 0xff) {
				stopBGM(0xffffffff, 10);
			} else if (a < 3) {
				iVar2 = smBgmInTrack[a];
				if (iVar2 && iVar2->unk14) {
					iVar2->unk14->stop(param2);
					smBgmInTrack[a] = 0;
				}
			}
		}
	}
}

void MSBgm::stopTrackBGM(u8 param1, u32 param2)
{
	// Todo: Disable Inlining properly.
	(void)0;
	(void)0;
	(void)0;
	(void)0;
	(void)0;
	if (param1 == 0xff) {
		stopTrackBGM(0, 10);
	} else if (param1 < 3) {
		MSBgm* track = smBgmInTrack[param1];
		if (track) {
			JAISound* sound = track->unk14;
			if (sound) {
				sound->stop(param2);
				smBgmInTrack[param1] = 0;
			}
		}
	}
}

void MSBgm::stopTrackBGMs(u8 param1, u32 param2)
{
	for (u8 i = 0; i < 3; i++) {
		if ((param1 >> i) & 1) {
			if (i == 0xff) {
				stopBGM(0xffffffff, 10);
			} else if (i < 3) {
				MSBgm* iVar2 = smBgmInTrack[i];
				if (iVar2 && iVar2->unk14) {
					iVar2->unk14->stop(param2);
					smBgmInTrack[i] = 0;
				}
			}
		}
	}
}

void MSBgm::setVolume(u32 param1, f32 param2, u32 param3, u8 param4)
{
	MSBgm* bgm = JALListS<MSBgm, u32>::search(param1 & 0x3FF);
	if (bgm && bgm->unk14) {
		bgm->unk14->setVolume(param2, param3, param4);
	}
}

void MSBgm::setTrackVolume(u8 param1, f32 param2, u32 param3, u8 param4)
{
	MSBgm* track = smBgmInTrack[param1];
	if (track && track->unk14) {
		track->unk14->setVolume(param2, param3, param4);
	}
}

void MSBgm::setSeqTrackVolume(u8 param1, u8 param2, f32 param3, u32 param4) { }

void MSBgm::setPan(u8 param1, f32 param2, u32 param3, u8 param4)
{
	MSBgm* track = smBgmInTrack[param1];
	if (track && track->unk14) {
		track->unk14->setPan(param2, param3, param4);
	}
}

void MSBgm::setDolby(u8 param1, f32 param2, u32 param3, u8 param4)
{
	MSBgm* track = smBgmInTrack[param1];
	if (track && track->unk14) {
		track->unk14->setDolby(param2, param3, param4);
	}
}

void MSBgm::setPause(u8 param1, bool param2) { }

void MSBgm::setSeqTrackMute(u8 param1, u8 param2, bool param3) { }

void MSBgm::setSeqTRACKsMute(u8 param1, bool param2, u16 param3) { }

void MSBgm::setSeqTRACKsMuteH(JAISound* param1, bool param2, u16 param3) { }

void MSBgm::setStageBgmYoshiPercussion(bool param)
{
	JAISound* sound = smBgmInTrack[0] ? smBgmInTrack[0]->unk14 : nullptr;

	if (sound == nullptr)
		return;

	if (MSGMSound->getBstSwitch(sound->unk8) & 0x10000000) {
		JASystem::TTrack* pTVar3 = getJASTrack(sound, 15);

		if (pTVar3 != nullptr)
			pTVar3->unk3C2 = param == 1 ? 0 : 1;
	}
}

bool MSBgm::checkPlaying(u32 param) { return false; }

MS_SCENE_WAVE MSBgm::getSceneNo(u32 param)
{
	switch (param) {
	case MSD_BGM_BIANCO:
		return MS_WAVE_DOLPIC;
	case MSD_BGM_MAMMA:
		return MS_WAVE_BIANCO;
	case MSD_BGM_PINNAPACO_SEA:
		return MS_WAVE_MANMA;
	case MSD_BGM_PINNAPACO:
		return MS_WAVE_PINNAPACO_S;
	case MSD_BGM_MARE_SEA:
		return MS_WAVE_PINNAPACO;
	case MSD_BGM_MONTEVILLAGE:
		return MS_WAVE_MARE_SEA;
	case MSD_BGM_SHILENA:
		return MS_WAVE_MONTEVILLAGE;
	case MSD_BGM_RICCO:
		return MS_WAVE_SHILENA;
	case MSD_BGM_GET_SHINE:
		return MS_WAVE_RICO;
	case MSD_BGM_CHUBOSS:
		return MS_WAVE_UNK210;
	case MSD_BGM_MISS:
		return MS_WAVE_UNK210;
	case MSD_BGM_BOSS:
		return MS_WAVE_UNK210;
	case MSD_BGM_MAP_SELECT:
		return MS_WAVE_UNK210;
	case MSD_BGM_BOSSPAKU_DEMO:
		return MS_WAVE_UNK20A;
	case MSD_BGM_MAIN_TITLE:
		return MS_WAVE_CLEAR;
	case MSD_BGM_CHUBOSS2:
		return MS_WAVE_UNK20A;
	case MSD_BGM_EXTRA:
		return MS_WAVE_INVALID;
	case MSD_BGM_DELFINO:
		return MS_WAVE_UNK20C;
	case MSD_BGM_MAREVILLAGE:
		return MS_WAVE_UNK20D;
	case MSD_BGM_CORONA:
		return MS_WAVE_UNK20E;
	case MSD_BGM_KAGEMARIO:
		return MS_WAVE_UNK20F;
	case MSD_BGM_CAMERA:
		return MS_WAVE_UNK210;
	case MSD_BGM_MONTE_ONSEN:
		return MS_WAVE_UNK210;
	case MSD_BGM_MECHAKUPPA:
		return MS_WAVE_MONTEVILLAGE;
	case MSD_BGM_AIRPORT:
		return MS_WAVE_UNK211;
	case MSD_BGM_UNDERGROUND:
		return MS_WAVE_UNK212;
	case MSD_BGM_TITLEBACK:
		return MS_WAVE_UNK210;
	case MSD_BGM_MONTE_NIGHT:
		return MS_WAVE_UNK20A;
	case MSD_BGM_CASINO:
		return MS_WAVE_MONTEVILLAGE;
	case MSD_BGM_EVENT:
		return MS_WAVE_UNK20D;
	case MSD_BGM_TIME_IVENT:
		return MS_WAVE_UNK210;
	case MSD_BGM_SKY_AND_SEA:
		return MS_WAVE_UNK210;
	case MSD_BGM_MONTE_RESCUE:
		return MS_WAVE_UNK214;
	case MSD_BGM_MERRY_GO_ROUND:
		return MS_WAVE_MONTEVILLAGE;
	case MSD_BGM_SCENARIO_SELECT:
		return MS_WAVE_PINNAPACO_S;
	case MSD_BGM_FANFARE_CASINO:
		return MS_WAVE_UNK210;
	case MSD_BGM_FANFARE_RACE:
		return MS_WAVE_UNK20D;
	case MSD_BGM_CAMERA_KAGE:
		return MS_WAVE_UNK210;
	case MSD_BGM_GAMEOVER:
		return MS_WAVE_UNK210;
	case MSD_BGM_BOSSHANA_2ND3RD:
		return MS_WAVE_UNK210;
	case MSD_BGM_BOSSGESO_2DN3RD:
		return MS_WAVE_UNK210;
	case MSD_BGM_CHUBOSS_MANTA:
		return MS_WAVE_UNK210;
	case MSD_BGM_MONTE_LAST:
		return MS_WAVE_UNK210;
	case MSD_BGM_SHINE_APPEAR:
		return MS_WAVE_MONTEVILLAGE;
	case MSD_BGM_MONTEMAN_RACE:
		return MS_WAVE_UNK215;
	case MSD_STR_SPACEWORLD:
		return MS_WAVE_UNK210;
	default:
		return MS_WAVE_INVALID;
	}
}

JAISound* MSBgm::getHandle(u8 param)
{
	MSBgm* track = smBgmInTrack[param];
	if (track) {
		return track->unk14;
	}
	return nullptr;
}

JASystem::TTrack* MSBgm::getJASTrack(JAISound* sound, u8 param_2)
{
	JASystem::TTrack* pTVar3
	    = JASystem::TrackMgr::handleToSeq(sound->getSeqParameter()->unk0);
	if (!pTVar3)
		return nullptr;

	return pTVar3->unk2C4[param_2];
}
