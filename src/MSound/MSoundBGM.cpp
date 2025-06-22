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
	JAISound* sound;
	JAISeqParameter* pJVar2;
	u32 uVar1;
	u32 uVar2;
	JASystem::TTrack* pTVar3;
	// Inlined function in MSoundBGM.hpp TODO: Found original inline function
	// placement.
	sound = someInline();
	if (sound == nullptr)
		return;

	uVar1 = MSGMSound->getBstSwitch(sound->unk8);

	if ((uVar1 & 0x10000000)) {
		pJVar2 = sound->getSeqParameter();
		pTVar3 = JASystem::TrackMgr::handleToSeq(pJVar2->unk0);
		if (pTVar3 == nullptr) {
			pTVar3 = nullptr;
		} else {
			pTVar3 = pTVar3->unk2C4[0xf];
		}
		if (pTVar3 != nullptr) {
			if (param == 1) {
				uVar2 = 0;
			} else {
				uVar2 = 1;
			}
			pTVar3->unk3C2 = uVar2;
		}
	}
}

bool MSBgm::checkPlaying(u32 param) { return false; }

u32 MSBgm::getSceneNo(u32 param)
{
	switch (param) {
	case 0x80010001:
		return 0x201;
	case 0x80010002:
		return 0x202;
	case 0x80010003:
		return 0x203;
	case 0x80010004:
		return 0x204;
	case 0x80010005:
		return 0x204;
	case 0x80010006:
		return 0x205;
	case 0x80010007:
		return 0x206;
	case 0x80010008:
		return 0x207;
	case 0x80010009:
		return 0x208;
	case 0x8001000a:
		return 0x210;
	case 0x8001000b:
		return 0x210;
	case 0x8001000c:
		return 0x210;
	case 0x8001000d:
		return 0x210;
	case 0x8001000e:
		return 0x20a;
	case 0x8001000f:
		return 0x209;
	case 0x80010010:
		return 0x20a;
	case 0x80010011:
		return 0xffffffff;
	case 0x80010012:
		return 0x20c;
	case 0x80010013:
		return 0x20d;
	case 0x80010014:
		return 0x20e;
	case 0x80010015:
		return 0x20f;
	case 0x80010016:
		return 0x210;
	case 0x80010017:
		return 0x210;
	case 0x80010018:
		return 0x206;
	case 0x80010019:
		return 0x211;
	case 0x8001001a:
		return 0x212;
	case 0x8001001b:
		return 0x210;
	case 0x8001001c:
		return 0x20a;
	case 0x8001001d:
		return 0x206;
	case 0x8001001e:
		return 0x20d;
	case 0x8001001f:
		return 0x210;
	case 0x80010020:
		return 0x210;
	case 0x80010021:
		return 0x214;
	case 0x80010022:
		return 0x206;
	case 0x80010023:
		return 0x204;
	case 0x80010024:
		return 0x210;
	case 0x80010025:
		return 0x20d;
	case 0x80010026:
		return 0x210;
	case 0x80010027:
		return 0x210;
	case 0x80010028:
		return 0x210;
	case 0x80010029:
		return 0x210;
	case 0x8001002a:
		return 0x210;
	case 0x8001002b:
		return 0x210;
	case 0x8001002c:
		return 0x206;
	case 0x8001002e:
		return 0x215;
	case 0x8001002f:
		return 0x210;
	default:
		return 0xffffffff;
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

JAISound* MSBgm::getJASTrack(JAISound* param1, u8 param2) { return nullptr; }