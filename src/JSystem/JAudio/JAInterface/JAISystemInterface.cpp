#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>
#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>
#include <dolphin/types.h>

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

	if (track != 0 && track->mSeqState) {
		if (track->getChild(0) || track->getChild(1) || track->getChild(2)
		    || track->getChild(3) || track->getChild(4) || track->getChild(5)
		    || track->getChild(6) || track->getChild(7) || track->getChild(8)
		    || track->getChild(9) || track->getChild(10) || track->getChild(11)
		    || track->getChild(12) || track->getChild(13) || track->getChild(14)
		    || track->getChild(15)) {
			return track->mSeqState;
		} else {
			return false;
		}
	}
	return false;
}

JASystem::TTrack* JAISystemInterface::trackToSeqp(JAISound* param_1, u8 param_2)
{
	JASystem::TTrack* result = nullptr;
	if (param_1->mSoundID & 0x800) {
		JASystem::TTrack* track = JASystem::TrackMgr::handleToSeq(
		    param_1->getSeqParameter()->mSeqHandle);
		if (track->getChild(param_2 >> 4))
			result = track->getChild(param_2 >> 4)->getChild(param_2 & 0xF);
	} else {
		JASystem::TTrack* track = JASystem::TrackMgr::handleToSeq(
		    param_1->getSeqParameter()->mSeqHandle);
		result = track->getChild(param_2 & 0xF);
	}
	return result;
}

void JAISystemInterface::setSeqPortargsF32(JAISeqUpdateData* sud, u32 track_no,
                                           u8 arg_no, f32 value)
{
	sud->mPlayerParams[track_no].mArgsAsF32[arg_no] = value;
}

void JAISystemInterface::setSeqPortargsPS16(JAISeqUpdateData* sud, u32 track_no,
                                            u8 arg_no, s16* value)
{
	sud->mPlayerParams[track_no].mArgsAsPS16[arg_no] = value;
}

void JAISystemInterface::setSeqPortargsU32(JAISeqUpdateData* sud, u32 track_no,
                                           u8 arg_no, u32 value)
{
	sud->mPlayerParams[track_no].mArgsAsU32[arg_no] = value;
}

JAISeqParameter* JAISystemInterface::rootInit(JAISeqUpdateData* param_1)
{
	JAISound* sound = param_1->mSound;
	JASystem::TTrack* track
	    = JASystem::TrackMgr::handleToSeq(sound->getSeqParameter()->mSeqHandle);
	outerInit(param_1, track, JAIGlobalParameter::getParamSeqTrackMax(), 0xffff,
	          0);
	return sound->getSeqParameter();
}

void JAISystemInterface::trackInit(JAISeqUpdateData* sud)
{
	JAISound* sound = sud->mSound;
	u32 trackCnt    = 0x10;
	if (sound->mSoundID & 0x800)
		trackCnt = JAIGlobalParameter::getParamSeqTrackMax();

	for (u32 i = 0; i < trackCnt; ++i)
		if (!(sud->mTrackInitFlags & (1 << i))) {
			JASystem::TTrack* track = trackToSeqp(sound, i);
			outerInit(sud, track, i, 0xffff, 0);
		}
}

void JAISystemInterface::outerInit(JAISeqUpdateData* sud, void* track,
                                   u32 track_no, u16 param_4, u8 param_5)
{
	if (!track)
		return;

	JASystem::Kernel::TPortArgs* args = &sud->mPlayerParams[track_no].mArgs;
	JASystem::TTrack* trackCasted     = (JASystem::TTrack*)track;

	sud->mPlayerParams[track_no].mTrack = trackCasted;
	args->mTrack                        = (JASystem::TTrack*)track;
	sud->mPlayerParams[track_no].mCmd.setPortCmd(&setSePortParameter, args);

	JASystem::TTrack::TOuterParam* outer = trackCasted->getOuterParam();

	if (track_no == JAIGlobalParameter::getParamSeqTrackMax()) {
		args->mTrackVolume = sud->mSeqVolume;
		args->mTrackPitch  = sud->mSeqPitch;
		args->mTrackFxmix  = sud->mSeqFxmix;
		args->mTrackPan    = sud->mSeqPan;
		args->mTrackDolby  = sud->mSeqDolby;
		args->mTrackTempo  = sud->mSeqTempo;
		args->mFlags       = 0xff;
		outer->onSwitch(JASystem::TTrack::UPDATE_Tempo);
	} else {
		JAISeqParameter* pJVar3 = sud->mSound->getSeqParameter();
		args->mTrackVolume      = pJVar3->mTrackVolume[track_no].mCurrentValue;
		args->mTrackPitch       = pJVar3->mTrackPitch[track_no].mCurrentValue;
		args->mTrackFxmix       = pJVar3->mTrackFxmix[track_no].mCurrentValue;
		args->mTrackPan         = pJVar3->mTrackPan[track_no].mCurrentValue;
		args->mTrackDolby       = pJVar3->mTrackDolby[track_no].mCurrentValue;
		args->unk20             = 0;
		args->mFlags            = 0x7f;
		trackCasted->muteTrack(pJVar3->mMuteBits[track_no].mCurrent);
	}
	outer->onSwitch(JASystem::TTrack::UPDATE_Volume);
	outer->onSwitch(JASystem::TTrack::UPDATE_Pitch);
	outer->onSwitch(JASystem::TTrack::UPDATE_Fxmix);
	outer->onSwitch(JASystem::TTrack::UPDATE_Pan);
	outer->onSwitch(JASystem::TTrack::UPDATE_Dolby);

	if ((param_4 & 1) == 0)
		outer->setParam(JASystem::TTrack::UPDATE_Volume, 0.0);

	if ((param_4 & 2) == 0)
		outer->setParam(JASystem::TTrack::UPDATE_Pitch, 0.0);

	if ((param_4 & 4) == 0)
		outer->setParam(JASystem::TTrack::UPDATE_Fxmix, 0.0);

	if ((param_4 & 8) == 0)
		outer->setParam(JASystem::TTrack::UPDATE_Pan, 0.0);

	if ((param_4 & 0x10) == 0)
		outer->setParam(JASystem::TTrack::UPDATE_Dolby, 0.0);

	sud->mPlayerParams[track_no].mCmd.addPortCmdOnce();
}

void JAISystemInterface::setPortParameter(JASystem::Kernel::TPortArgs* args,
                                          JASystem::TTrack* track, u32 param_3,
                                          u32 param_4)
{
	if ((args->mFlags & (1 << param_4)) != 0) {
		JASystem::TTrack::TOuterParam* outer = track->getOuterParam();
		outer->setParam(param_3, (&args->mTrackVolume)[param_4]);
		args->mFlags ^= 1 << param_4;
	}
}

void JAISystemInterface::setSePortParameter(JASystem::Kernel::TPortArgs* args)
{
	JASystem::TTrack* track = args->mTrack;
	if (!track)
		return;

	setPortParameter(args, track, JASystem::TTrack::UPDATE_Volume, 0);
	setPortParameter(args, track, JASystem::TTrack::UPDATE_Pitch, 1);
	setPortParameter(args, track, JASystem::TTrack::UPDATE_Pan, 2);
	setPortParameter(args, track, JASystem::TTrack::UPDATE_Fxmix, 3);
	setPortParameter(args, track, JASystem::TTrack::UPDATE_Tempo, 7);
	setPortParameter(args, track, JASystem::TTrack::UPDATE_Dolby, 4);

	if ((args->mFlags & 0x40) != 0 && args->unk20 != 0)
		track->setInterrupt(5);
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
