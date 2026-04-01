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
		if ((param == 0x8001000a) || (param == 0x8001000c)
		    || (param == 0x80010028)) {
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
	case 0x80010001:
		return MS_WAVE_DOLPIC;
	case 0x80010002:
		return MS_WAVE_BIANCO;
	case 0x80010003:
		return MS_WAVE_MANMA;
	case 0x80010004:
		return MS_WAVE_PINNAPACO_S;
	case 0x80010005:
		return MS_WAVE_PINNAPACO;
	case 0x80010006:
		return MS_WAVE_MARE_SEA;
	case 0x80010007:
		return MS_WAVE_MONTEVILLAGE;
	case 0x80010008:
		return MS_WAVE_SHILENA;
	case 0x80010009:
		return MS_WAVE_RICO;
	case 0x8001000a:
		return MS_WAVE_UNK210;
	case 0x8001000b:
		return MS_WAVE_UNK210;
	case 0x8001000c:
		return MS_WAVE_UNK210;
	case 0x8001000d:
		return MS_WAVE_UNK210;
	case 0x8001000e:
		return MS_WAVE_UNK20A;
	case 0x8001000f:
		return MS_WAVE_CLEAR;
	case 0x80010010:
		return MS_WAVE_UNK20A;
	case 0x80010011:
		return MS_WAVE_INVALID;
	case 0x80010012:
		return MS_WAVE_UNK20C;
	case 0x80010013:
		return MS_WAVE_UNK20D;
	case 0x80010014:
		return MS_WAVE_UNK20E;
	case 0x80010015:
		return MS_WAVE_UNK20F;
	case 0x80010016:
		return MS_WAVE_UNK210;
	case 0x80010017:
		return MS_WAVE_UNK210;
	case 0x80010018:
		return MS_WAVE_MONTEVILLAGE;
	case 0x80010019:
		return MS_WAVE_UNK211;
	case 0x8001001a:
		return MS_WAVE_UNK212;
	case 0x8001001b:
		return MS_WAVE_UNK210;
	case 0x8001001c:
		return MS_WAVE_UNK20A;
	case 0x8001001d:
		return MS_WAVE_MONTEVILLAGE;
	case 0x8001001e:
		return MS_WAVE_UNK20D;
	case 0x8001001f:
		return MS_WAVE_UNK210;
	case 0x80010020:
		return MS_WAVE_UNK210;
	case 0x80010021:
		return MS_WAVE_UNK214;
	case 0x80010022:
		return MS_WAVE_MONTEVILLAGE;
	case 0x80010023:
		return MS_WAVE_PINNAPACO_S;
	case 0x80010024:
		return MS_WAVE_UNK210;
	case 0x80010025:
		return MS_WAVE_UNK20D;
	case 0x80010026:
		return MS_WAVE_UNK210;
	case 0x80010027:
		return MS_WAVE_UNK210;
	case 0x80010028:
		return MS_WAVE_UNK210;
	case 0x80010029:
		return MS_WAVE_UNK210;
	case 0x8001002a:
		return MS_WAVE_UNK210;
	case 0x8001002b:
		return MS_WAVE_UNK210;
	case 0x8001002c:
		return MS_WAVE_MONTEVILLAGE;
	case 0x8001002e:
		return MS_WAVE_UNK215;
	case 0x8001002f:
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
