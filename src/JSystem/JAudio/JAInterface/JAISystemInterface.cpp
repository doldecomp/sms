#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>
#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>
#include <types.h>

JASystem::Kernel::TPortCmd JAISystemInterface::systemPortCmd;

bool JAISystemInterface::checkFileExsistence(char* path)
{
	char buf[64];
	JASystem::Dvd::extendPath(buf, path);
	if (DVDConvertPathToEntrynum(buf) != -1)
		return true;
	else
		return false;
}

BOOL JAISystemInterface::checkSeqActiveFlag(u32 param_1)
{
	JASystem::TTrack* track = JASystem::TrackMgr::handleToSeq(param_1);

	if (track != 0 && track->unk3C4) {
		if (track->unk2C4[0] || track->unk2C4[1] || track->unk2C4[2]
		    || track->unk2C4[3] || track->unk2C4[4] || track->unk2C4[5]
		    || track->unk2C4[6] || track->unk2C4[7] || track->unk2C4[8]
		    || track->unk2C4[9] || track->unk2C4[10] || track->unk2C4[11]
		    || track->unk2C4[12] || track->unk2C4[13] || track->unk2C4[14]
		    || track->unk2C4[15]) {
			return track->unk3C4;
		} else {
			return false;
		}
	}
	return false;
}

JASystem::TTrack* JAISystemInterface::trackToSeqp(JAISound* param_1, u8 param_2)
{
	JASystem::TTrack* result = nullptr;
	if (param_1->unk8 & 0x800) {
		JASystem::TTrack* track
		    = JASystem::TrackMgr::handleToSeq(param_1->getSeqParameter()->unk0);
		if (track->unk2C4[param_2 >> 4])
			result = track->unk2C4[param_2 >> 4]->unk2C4[param_2 & 0xF];
	} else {
		JASystem::TTrack* track
		    = JASystem::TrackMgr::handleToSeq(param_1->getSeqParameter()->unk0);
		result = track->unk2C4[param_2 & 0xF];
	}
	return result;
}

void JAISystemInterface::setSeqPortargsF32(JAISeqUpdateData*, u32, u8, f32) { }

void JAISystemInterface::setSeqPortargsPS16(JAISeqUpdateData*, u32, u8, s16*) {
}

void JAISystemInterface::setSeqPortargsU32(JAISeqUpdateData*, u32, u8, u32) { }

JAISeqParameter* JAISystemInterface::rootInit(JAISeqUpdateData* param_1)
{
	JAISound* sound = param_1->unk48;
	JASystem::TTrack* track
	    = JASystem::TrackMgr::handleToSeq(sound->getSeqParameter()->unk0);
	outerInit(param_1, track, JAIGlobalParameter::getParamSeqTrackMax(), 0xffff,
	          0);
	return sound->getSeqParameter();
}

void JAISystemInterface::trackInit(JAISeqUpdateData* param_1)
{
	JAISound* sound = param_1->unk48;
	u32 trackCnt    = 0x10;
	if (sound->unk8 & 0x800)
		trackCnt = JAIGlobalParameter::getParamSeqTrackMax();

	for (u32 i = 0; i < trackCnt; ++i)
		if (!(param_1->unk4 & (1 << i))) {
			JASystem::TTrack* track = trackToSeqp(sound, i);
			outerInit(param_1, track, i, 0xffff, 0);
		}
}

void JAISystemInterface::outerInit(JAISeqUpdateData*, void*, u32, u16, u8) { }

void JAISystemInterface::setPortParameter(JASystem::Kernel::TPortArgs*,
                                          JASystem::TTrack*, u32, u32)
{
}

void JAISystemInterface::setSePortParameter(
    JASystem::Kernel::TPortArgs* param_1)
{
	JASystem::TTrack* track = param_1->mTrack;
	if (track != nullptr) {
		if ((param_1->mFlags & 1) != 0) {
			track->mOuterParam->setParam(0x1, param_1->mTrackVolume);
			param_1->mFlags ^= 1;
		}
		if ((param_1->mFlags & 2) != 0) {
			track->mOuterParam->setParam(0x2, param_1->mTrackPitch);
			param_1->mFlags ^= 2;
		}
		if ((param_1->mFlags & 4) != 0) {
			track->mOuterParam->setParam(0x8, param_1->mTrackPan);
			param_1->mFlags ^= 4;
		}
		if ((param_1->mFlags & 8) != 0) {
			track->mOuterParam->setParam(0x4, param_1->mTrackFxmix);
			param_1->mFlags ^= 8;
		}
		if ((param_1->mFlags & 0x80) != 0) {
			track->mOuterParam->setParam(0x40, param_1->mTrackTempo);
			param_1->mFlags ^= 0x80;
		}
		if ((param_1->mFlags & 0x10) != 0) {
			track->mOuterParam->setParam(0x10, param_1->mTrackDolby);
			param_1->mFlags ^= 0x10;
		}
		if ((param_1->mFlags & 0x40) != 0 && param_1->unk20 != 0) {
			track->setInterrupt(5);
		}
	}
}

void* JAISystemInterface::JAIouterP(void*) { return nullptr; }

void* JAISystemInterface::JAIouterSW(void*) { return nullptr; }

void JAISystemInterface::setAudioFrameParameter(JASystem::Kernel::TPortArgs*) {
}

int JAISystemInterface::setSeqData(JASystem::TTrack* param_1, u8* param_2,
                                   s32 param_3,
                                   JASystem::Player::SEQ_PLAYMODE param_4)
{
	if (param_1 == nullptr) {
		BOOL enable = OSDisableInterrupts();
		param_1     = JASystem::TrackMgr::getNewTrack();
		OSRestoreInterrupts(enable);
		if (param_1 == nullptr)
			return -1;
	} else {
		param_1->setInnerMemory(0);
	}

	return param_1->setSeqData(param_2, param_3, param_4);
}

BOOL JAISystemInterface::startSeq(u32 param_1)
{
	JASystem::TTrack* track = JASystem::TrackMgr::handleToSeq(param_1);
	if (param_1 == -1)
		return 0;
	else if (track == nullptr)
		return 0;
	else
		return track->startSeq();
}

BOOL JAISystemInterface::stopSeq(s32 param_1)
{
	if (param_1 == -1)
		return 0;

	JASystem::TTrack* track = JASystem::TrackMgr::handleToSeq(param_1);

	if (track == nullptr)
		return 0;
	else
		return track->stopSeq();
}

BOOL JAISystemInterface::writePortApp(u32 param_1, u32 param_2, u16 param_3)
{
	JASystem::TTrack* track = JASystem::TrackMgr::handleToSeq(param_1);

	if (track == nullptr)
		return 0;
	else
		return track->writePortApp(param_2, param_3);
}

BOOL JAISystemInterface::readPortApp(u32 param_1, u32 param_2, u16* param_3)
{
	JASystem::TTrack* track = JASystem::TrackMgr::handleToSeq(param_1);

	if (track == nullptr)
		return 0;
	else
		return track->readPortApp(param_2, param_3);
}
