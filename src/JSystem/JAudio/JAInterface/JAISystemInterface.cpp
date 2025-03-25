#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>
#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>
#include <types.h>

JASystem::Kernel::TPortCmd JAISystemInterface::systemPortCmd;

BOOL JAISystemInterface::checkFileExsistence(char* path)
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
		if (track->getChild(0) || track->getChild(1) || track->getChild(2)
		    || track->getChild(3) || track->getChild(4) || track->getChild(5)
		    || track->getChild(6) || track->getChild(7) || track->getChild(8)
		    || track->getChild(9) || track->getChild(10) || track->getChild(11)
		    || track->getChild(12) || track->getChild(13) || track->getChild(14)
		    || track->getChild(15)) {
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
		if (track->getChild(param_2 >> 4))
			result = track->getChild(param_2 >> 4)->getChild(param_2 & 0xF);
	} else {
		JASystem::TTrack* track
		    = JASystem::TrackMgr::handleToSeq(param_1->getSeqParameter()->unk0);
		result = track->getChild(param_2 & 0xF);
	}
	return result;
}

void JAISystemInterface::setSeqPortargsF32(JAISeqUpdateData* param_1,
                                           u32 param_2, u8 param_3, f32 param_4)
{
	JAISeqUpdateData::FabricatedUnk4CStruct* s = &param_1->unk4C[param_2];

	((f32*)&s->unk4)[param_3] = param_4;
}

void JAISystemInterface::setSeqPortargsPS16(JAISeqUpdateData*, u32, u8, s16*) {
}

void JAISystemInterface::setSeqPortargsU32(JAISeqUpdateData* param_1,
                                           u32 param_2, u8 param_3, u32 param_4)
{
	JAISeqUpdateData::FabricatedUnk4CStruct* s = &param_1->unk4C[param_2];

	((u32*)&s->unk4)[param_3] = param_4;
}

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

void JAISystemInterface::outerInit(JAISeqUpdateData* param_1, void* param_2,
                                   u32 param_3, u16 param_4, u8 param_5)
{
	if (!param_2)
		return;

	JASystem::Kernel::TPortArgs* args = &param_1->unk4C[param_3].unk4;
	JASystem::TTrack* track           = ((JASystem::TTrack*)param_2);

	param_1->unk4C[param_3].unk0 = track;
	args->mTrack                 = ((JASystem::TTrack*)param_2);
	param_1->unk4C[param_3].unk2C.setPortCmd(&setSePortParameter, args);

	JASystem::TTrack::TOuterParam* outer = track->getOuterParam();

	if (param_3 == JAIGlobalParameter::getParamSeqTrackMax()) {
		args->mTrackVolume = param_1->unkC;
		args->mTrackPitch  = param_1->unk10;
		args->mTrackFxmix  = param_1->unk14;
		args->mTrackPan    = param_1->unk18;
		args->mTrackDolby  = param_1->unk1C;
		args->mTrackTempo  = param_1->unk20;
		args->mFlags       = 0xff;
		outer->onSwitch(0x40);
	} else {
		JAISeqParameter* pJVar3 = param_1->unk48->getSeqParameter();
		args->mTrackVolume      = pJVar3->unk754[param_3].unk4;
		args->mTrackPitch       = pJVar3->unkB54[param_3].unk4;
		args->mTrackFxmix       = pJVar3->unkD54[param_3].unk4;
		args->mTrackPan         = pJVar3->unk954[param_3].unk4;
		args->mTrackDolby       = pJVar3->unkF54[param_3].unk4;
		args->unk20             = 0;
		args->mFlags            = 0x7f;
		track->muteTrack(pJVar3->unk1830[param_3].flag1);
	}
	outer->onSwitch(0x1);
	outer->onSwitch(0x2);
	outer->onSwitch(0x4);
	outer->onSwitch(0x8);
	outer->onSwitch(0x10);

	if ((param_4 & 1) == 0)
		outer->setParam(1, 0.0);

	if ((param_4 & 2) == 0)
		outer->setParam(2, 0.0);

	if ((param_4 & 4) == 0)
		outer->setParam(4, 0.0);

	if ((param_4 & 8) == 0)
		outer->setParam(8, 0.0);

	if ((param_4 & 0x10) == 0)
		outer->setParam(16, 0.0);

	param_1->unk4C[param_3].unk2C.addPortCmdOnce();
}

void JAISystemInterface::setPortParameter(JASystem::Kernel::TPortArgs*,
                                          JASystem::TTrack*, u32, u32)
{
}

void JAISystemInterface::setSePortParameter(
    JASystem::Kernel::TPortArgs* param_1)
{
	JASystem::TTrack* track = param_1->mTrack;
	if (!track)
		return;

	// TODO: some weird stuff is happening here and with setSeqPortargsF32/U32,
	// they seem to have a union somewhere making the params accessible by index
	// and these 6 ifs seem to be setPortParameter called w/ different args and
	// accessing the union
	if ((param_1->mFlags & 1) != 0) {
		track->getOuterParam()->setParam(0x1, param_1->mTrackVolume);
		param_1->mFlags ^= 1;
	}
	if ((param_1->mFlags & 2) != 0) {
		track->getOuterParam()->setParam(0x2, param_1->mTrackPitch);
		param_1->mFlags ^= 2;
	}
	if ((param_1->mFlags & 4) != 0) {
		track->getOuterParam()->setParam(0x8, param_1->mTrackPan);
		param_1->mFlags ^= 4;
	}
	if ((param_1->mFlags & 8) != 0) {
		track->getOuterParam()->setParam(0x4, param_1->mTrackFxmix);
		param_1->mFlags ^= 8;
	}
	if ((param_1->mFlags & 0x80) != 0) {
		track->getOuterParam()->setParam(0x40, param_1->mTrackTempo);
		param_1->mFlags ^= 0x80;
	}
	if ((param_1->mFlags & 0x10) != 0) {
		track->getOuterParam()->setParam(0x10, param_1->mTrackDolby);
		param_1->mFlags ^= 0x10;
	}
	if ((param_1->mFlags & 0x40) != 0 && param_1->unk20 != 0) {
		track->setInterrupt(5);
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
