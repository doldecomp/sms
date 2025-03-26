#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>
#include <JSystem/JAudio/JASystem/JASCmdStack.hpp>
#include <JSystem/JAudio/JASystem/JASVload.hpp>

void JAIBasic::stopSeq(JAISound* param_1)
{
	if (param_1->getSwBit() & 1) {
		for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
			JAISound* sound = unk0->unk180[i].unk48;
			if (sound != param_1 && sound) {
				if (sound->unk1 >= 3 && !(sound->getSwBit() & 2)) {
					sound->setSeqInterVolume(10, 1.0f, 10);
					JASystem::TrackMgr::handleToSeq(
					    sound->getSeqParameter()->unk0)
					    ->unPauseTrackAll();
				}
			}
		}
	}

	param_1->unk34                      = nullptr;
	param_1->getSeqParameter()->unk1850 = nullptr;
	if (param_1->unk1 >= 3) {
		unk0->releaseAutoHeapPointer(param_1->getSeqParameter()->unk1754);
	}
	param_1->unk1 = 0;
	releaseSeqParameterPointer(param_1->getSeqParameter());
	releaseControllerHandle(&unk0->unk210, param_1);
	unk0->unk180[param_1->unk0].unk48 = nullptr;
}

void JAIBasic::checkEntriedSeq() { }

#pragma dont_inline on
void JAIBasic::checkPlayingSeqTrack(unsigned long trackID) { }
#pragma dont_inline off

void JAIBasic::checkPlayingSeq()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISound* sound = unk0->unk180[i].unk48;
		if (sound && sound->unk1 >= 4) {
			checkPlayingSeqTrack(i);
			for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax + 1; ++j) {
				if (unk0->unk180[i].unk44[j] != 0) {
					JAISystemInterface::setSeqPortargsU32(
					    &unk0->unk180[i], j, 1, unk0->unk180[i].unk44[j]);

					unk0->unk180[i].unk4C[j].unk0 = 0;
					unk0->unk180[i].unk4C[j].unk2C.addPortCmdOnce();
				}
			}
		}
	}
}

void JAIBasic::checkStoppedSeq()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISound** sound = &unk0->unk180[i].unk48;

		if (!*sound)
			continue;

		if ((*sound)->unk1 == 4 || (*sound)->unk1 == 5) {
			u8 flag = JAISystemInterface::checkSeqActiveFlag(
			    (*sound)->getSeqParameter()->unk0);
			if (flag == 0) {
				(*sound)->clearMainSoundPPointer();
				stopSeq(*sound);
				unk0->unk180[i].unk8 = 0;
			}
		}
	}
}

void JAIBasic::checkStartedSeq()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISound** sound = &unk0->unk180[i].unk48;

		if (!*sound)
			continue;

		if ((*sound)->unk1 == 3) {
			u8 flag = JAISystemInterface::checkSeqActiveFlag(
			    (*sound)->getSeqParameter()->unk0);
			if (flag != 0) {
				(*sound)->unk1 = 4;
				JAISystemInterface::trackInit(&unk0->unk180[i]);
			}
		}
	}
}

void JAIBasic::checkFadeoutSeq()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISound** sound = &unk0->unk180[i].unk48;

		if (!*sound)
			continue;

		if ((*sound)->unk1 == 5 && (*sound)->getSeqInterVolume(6) == 0.0f) {
			JAISystemInterface::stopSeq((*sound)->getSeqParameter()->unk0);
			(*sound)->clearMainSoundPPointer();
			stopSeq(*sound);
			unk0->unk180[i].unk8 = 0;
		}
	}
}

void JAIBasic::checkReadSeq()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISeqUpdateData* sud = &unk0->unk180[i];
		JAISound** sound      = &sud->unk48;
		if (!*sound)
			continue;
		if ((*sound)->unk1 != 2)
			continue;
		if ((*sound)->getSeqParameter()->unk1758 != -1)
			continue;
		if ((*sound)->getSeqParameter()->unk1850->unk2 != 0)
			continue;

		u32 lVar2
		    = JASystem::Vload::checkSize(unk2C + ((*sound)->unk8 & 0x3FF));
		int uVar3 = JAISystemInterface::setSeqData(
		    nullptr, sud->unk40, lVar2, JASystem::Player::SEQ_PLAYMODE_UNK_0);

		(*sound)->getSeqParameter()->unk0 = uVar3;
		(*sound)->getSeqParameter();
		if ((*sound)->getSeqParameter()->unk0 != -1) {
			unk0->initSeqTrackInfoParameter((*sound)->unk0);
			(*sound)->unk1 = 3;
			if ((*sound)->unk10 > 1) {
				(*sound)->setSeqInterVolume(6, 0.0f, 0);
				(*sound)->setSeqInterVolume(6, 1.0f, (*sound)->unk10);
			}
			if (sud->unk0 != 0) {
				(*sound)->setPauseMode(sud->unk0, sud->unk1);
				sud->unkC = 1.1f;
			}
			setSeExtParameter(*sound);
			checkPlayingSeqTrack(i);
			if (*sound != nullptr) {
				JAISystemInterface::rootInit(sud);
				JAISystemInterface::startSeq((*sound)->getSeqParameter()->unk0);
			}
		} else {
			JAISound* snd = *sound;
			stopSeq(snd);
		}
	}
}

void JAIBasic::checkSeqWave()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISeqUpdateData* sud = &unk0->unk180[i];
		JAISound** sound      = &sud->unk48;
		if (!*sound)
			continue;
		if (!(*sound)->getSeqParameter())
			continue;
		if ((*sound)->getSeqParameter()->unk1758 == 0xffffffff)
			continue;
		if (unk34 == 0xffffffff)
			continue;

		u32 uVar3 = (*sound)->getSeqParameter()->unk1758;

		JAISound* snd = *sound;
		if (unk34 == uVar3 || uVar3 == 0xff00ff00
		    || (((unk34 & 0xffff0000) == (uVar3 & 0xffff0000)
		         && uVar3 == 0xffff))
		    || (((unk34 & 0xffff) == (uVar3 & 0xffff)
		         && uVar3 == 0xffff0000))) {
			snd->getSeqParameter()->unk1758 = 0xffffffff;
		}
	}
}

void JAIBasic::checkDvdLoadArc(u32 param_1)
{
	u8 hi   = param_1 >> 8;
	u8 lo   = param_1 & 0xff;
	u16 hi2 = ((param_1 >> 16) & 0x3FF);
	if (hi != 0xff)
		basic->unk0->setAutoHeapLoadedFlag(hi, 0);

	if (lo < 0xFE) {
		JAISound* sound              = basic->unk0->unk180[lo].unk48;
		basic->unk0->unk180[lo].unk3 = 0;
		if (sound && sound->unk1 == 1 && hi2 == (sound->unk8 & 0x3FF))
			sound->unk1 = 2;
		else
			basic->unk0->releaseAutoHeapPointer(hi);
	} else if (lo == 0xFE) {
		basic->unk0->releaseAutoHeapPointer(hi);
	}
}
