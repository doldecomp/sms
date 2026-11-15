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
	char fullPath[64];
	JASystem::Dvd::extendPath(fullPath, path);
	if (DVDConvertPathToEntrynum(fullPath) != -1)
		return true;
	else
		return false;
}

BOOL JAISystemInterface::checkSeqActiveFlag(u32 seq_handle)
{
	JASystem::TTrack* rootTrack = JASystem::TrackMgr::handleToSeq(seq_handle);

	if (rootTrack != 0
	    && rootTrack->mSeqState != JASystem::TTrack::SEQ_INACTIVE) {
		if (rootTrack->getChild(0) || rootTrack->getChild(1)
		    || rootTrack->getChild(2) || rootTrack->getChild(3)
		    || rootTrack->getChild(4) || rootTrack->getChild(5)
		    || rootTrack->getChild(6) || rootTrack->getChild(7)
		    || rootTrack->getChild(8) || rootTrack->getChild(9)
		    || rootTrack->getChild(10) || rootTrack->getChild(11)
		    || rootTrack->getChild(12) || rootTrack->getChild(13)
		    || rootTrack->getChild(14) || rootTrack->getChild(15)) {
			return rootTrack->mSeqState;
		} else {
			return false;
		}
	}
	return false;
}

JASystem::TTrack* JAISystemInterface::trackToSeqp(JAISound* sound, u8 track_no)
{
	JASystem::TTrack* result = nullptr;
	if (sound->mSoundID & JAISoundID_SeqTwoLevelTracks) {
		JASystem::TTrack* rootTrack = JASystem::TrackMgr::handleToSeq(
		    sound->getSeqParameter()->mSeqHandle);
		if (rootTrack->getChild(track_no >> 4))
			result
			    = rootTrack->getChild(track_no >> 4)->getChild(track_no & 0xF);
	} else {
		JASystem::TTrack* rootTrack = JASystem::TrackMgr::handleToSeq(
		    sound->getSeqParameter()->mSeqHandle);
		result = rootTrack->getChild(track_no & 0xF);
	}
	return result;
}

void JAISystemInterface::setSeqPortargsF32(JAISeqUpdateData* update_data,
                                           u32 player_index, u8 arg_index,
                                           f32 value)
{
	update_data->mTrackCommands[player_index].mArgsAsF32[arg_index] = value;
}

void JAISystemInterface::setSeqPortargsPS16(JAISeqUpdateData* update_data,
                                            u32 player_index, u8 arg_index,
                                            s16* value)
{
	update_data->mTrackCommands[player_index].mArgsAsPS16[arg_index] = value;
}

void JAISystemInterface::setSeqPortargsU32(JAISeqUpdateData* update_data,
                                           u32 player_index, u8 arg_index,
                                           u32 value)
{
	update_data->mTrackCommands[player_index].mArgsAsU32[arg_index] = value;
}

JAISeqParameter* JAISystemInterface::rootInit(JAISeqUpdateData* update_data)
{
	JAISound* sound = update_data->mSound;
	JASystem::TTrack* rootTrack
	    = JASystem::TrackMgr::handleToSeq(sound->getSeqParameter()->mSeqHandle);
	outerInit(update_data, rootTrack, JAIGlobalParameter::getParamSeqTrackMax(),
	          0xffff, 0);
	return sound->getSeqParameter();
}

void JAISystemInterface::trackInit(JAISeqUpdateData* update_data)
{
	JAISound* sound = update_data->mSound;
	u32 trackCount  = 0x10;
	if (sound->mSoundID & JAISoundID_SeqTwoLevelTracks)
		trackCount = JAIGlobalParameter::getParamSeqTrackMax();

	for (u32 trackNo = 0; trackNo < trackCount; ++trackNo)
		if (!(update_data->mTrackInitFlags & (1 << trackNo))) {
			JASystem::TTrack* track = trackToSeqp(sound, trackNo);
			outerInit(update_data, track, trackNo, 0xffff, 0);
		}
}

void JAISystemInterface::outerInit(JAISeqUpdateData* update_data,
                                   void* track_ptr, u32 player_index,
                                   u16 outer_param_mask, u8 param_5)
{
	if (!track_ptr)
		return;

	JASystem::Kernel::TPortArgs* args
	    = &update_data->mTrackCommands[player_index].mArgs;
	JASystem::TTrack* track = (JASystem::TTrack*)track_ptr;

	update_data->mTrackCommands[player_index].mTrack = track;
	args->mTrack = (JASystem::TTrack*)track_ptr;
	update_data->mTrackCommands[player_index].mCmd.setPortCmd(
	    &setSePortParameter, args);

	JASystem::TTrack::TOuterParam* outer = track->getOuterParam();

	if (player_index == JAIGlobalParameter::getParamSeqTrackMax()) {
		args->mTrackVolume  = update_data->mSeqVolume;
		args->mTrackPitch   = update_data->mSeqPitch;
		args->mTrackFxmix   = update_data->mSeqFxmix;
		args->mTrackPan     = update_data->mSeqPan;
		args->mTrackDolby   = update_data->mSeqDolby;
		args->mTrackTempo   = update_data->mSeqTempo;
		args->mPendingFlags = 0xff;
		outer->onSwitch(JASystem::TTrack::UPDATE_TEMPO);
	} else {
		JAISeqParameter* seqParams = update_data->mSound->getSeqParameter();
		args->mTrackVolume
		    = seqParams->mTrackVolume[player_index].mCurrentValue;
		args->mTrackPitch = seqParams->mTrackPitch[player_index].mCurrentValue;
		args->mTrackFxmix = seqParams->mTrackFxmix[player_index].mCurrentValue;
		args->mTrackPan   = seqParams->mTrackPan[player_index].mCurrentValue;
		args->mTrackDolby = seqParams->mTrackDolby[player_index].mCurrentValue;
		args->mRequestApplicationInterrupt = 0;
		args->mPendingFlags                = 0x7f;
		track->muteTrack(seqParams->mMuteBits[player_index].mCurrent);
	}
	outer->onSwitch(JASystem::TTrack::UPDATE_VOLUME);
	outer->onSwitch(JASystem::TTrack::UPDATE_PITCH);
	outer->onSwitch(JASystem::TTrack::UPDATE_FXMIX);
	outer->onSwitch(JASystem::TTrack::UPDATE_PAN);
	outer->onSwitch(JASystem::TTrack::UPDATE_DOLBY);

	if ((outer_param_mask & 1) == 0)
		outer->setParam(JASystem::TTrack::UPDATE_VOLUME, 0.0);

	if ((outer_param_mask & 2) == 0)
		outer->setParam(JASystem::TTrack::UPDATE_PITCH, 0.0);

	if ((outer_param_mask & 4) == 0)
		outer->setParam(JASystem::TTrack::UPDATE_FXMIX, 0.0);

	if ((outer_param_mask & 8) == 0)
		outer->setParam(JASystem::TTrack::UPDATE_PAN, 0.0);

	if ((outer_param_mask & 0x10) == 0)
		outer->setParam(JASystem::TTrack::UPDATE_DOLBY, 0.0);

	update_data->mTrackCommands[player_index].mCmd.addPortCmdOnce();
}

void JAISystemInterface::setPortParameter(JASystem::Kernel::TPortArgs* args,
                                          JASystem::TTrack* track,
                                          u32 outer_param_mask, u32 value_index)
{
	if ((args->mPendingFlags & (1 << value_index)) != 0) {
		JASystem::TTrack::TOuterParam* outer = track->getOuterParam();
		outer->setParam(outer_param_mask, (&args->mTrackVolume)[value_index]);
		args->mPendingFlags ^= 1 << value_index;
	}
}

void JAISystemInterface::setSePortParameter(JASystem::Kernel::TPortArgs* args)
{
	JASystem::TTrack* track = args->mTrack;
	if (!track)
		return;

	setPortParameter(args, track, JASystem::TTrack::UPDATE_VOLUME, 0);
	setPortParameter(args, track, JASystem::TTrack::UPDATE_PITCH, 1);
	setPortParameter(args, track, JASystem::TTrack::UPDATE_PAN, 2);
	setPortParameter(args, track, JASystem::TTrack::UPDATE_FXMIX, 3);
	setPortParameter(args, track, JASystem::TTrack::UPDATE_TEMPO, 7);
	setPortParameter(args, track, JASystem::TTrack::UPDATE_DOLBY, 4);

	if ((args->mPendingFlags
	     & JASystem::Kernel::TPortArgs::UPDATE_APPLICATION_INTERRUPT)
	        != 0
	    && args->mRequestApplicationInterrupt != 0)
		track->setInterrupt(JASystem::TIntrMgr::INTR_APPLICATION);
}

void* JAISystemInterface::JAIouterP(void*) { return nullptr; }

void* JAISystemInterface::JAIouterSW(void*) { return nullptr; }

void JAISystemInterface::setAudioFrameParameter(JASystem::Kernel::TPortArgs*) {
}

int JAISystemInterface::setSeqData(JASystem::TTrack* track, u8* seq_data,
                                   s32 size,
                                   JASystem::Player::SEQ_PLAYMODE play_mode)
{
	if (track == nullptr) {
		BOOL interruptsWereEnabled = OSDisableInterrupts();
		track                      = JASystem::TrackMgr::getNewTrack();
		OSRestoreInterrupts(interruptsWereEnabled);
		if (track == nullptr)
			return -1;
	} else {
		track->setInnerMemory(0);
	}

	return track->setSeqData(seq_data, size, play_mode);
}

BOOL JAISystemInterface::startSeq(u32 seq_handle)
{
	JASystem::TTrack* track = JASystem::TrackMgr::handleToSeq(seq_handle);
	if (seq_handle == -1)
		return 0;
	else if (track == nullptr)
		return 0;
	else
		return track->startSeq();
}

BOOL JAISystemInterface::stopSeq(s32 seq_handle)
{
	if (seq_handle == -1)
		return 0;

	JASystem::TTrack* track = JASystem::TrackMgr::handleToSeq(seq_handle);

	if (track == nullptr)
		return 0;
	else
		return track->stopSeq();
}

BOOL JAISystemInterface::writePortApp(u32 seq_handle, u32 port_route, u16 value)
{
	JASystem::TTrack* track = JASystem::TrackMgr::handleToSeq(seq_handle);

	if (track == nullptr)
		return 0;
	else
		return track->writePortApp(port_route, value);
}

BOOL JAISystemInterface::readPortApp(u32 seq_handle, u32 port_route, u16* value)
{
	JASystem::TTrack* track = JASystem::TrackMgr::handleToSeq(seq_handle);

	if (track == nullptr)
		return 0;
	else
		return track->readPortApp(port_route, value);
}
