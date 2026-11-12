#include <string.h>
#include <JSystem/JAudio/JASystem/JASHardStream.hpp>
#include <dolphin/ai.h>
#include <dolphin/dvd.h>

namespace JASystem {

namespace HardStream {

	static bool useHardStreaming = false;
	static char* streamFiles     = 0;
	static TPlayList* playList   = 0;
	static TControl strCtrl;

	static char rootDir[32] = "";

	static s32 playListMax = 0x20;

	static void firstBgmCallback(s32, DVDFileInfo*);
	static void secondBgmCallback(s32, DVDFileInfo*);
	static void getAddrCallback(s32, DVDCommandBlock*);

	static void dummy(f32* f) { *f = 0.0f; }

	void init() { }

	TPlayList::~TPlayList() { }

	TPlayList::TPlayList() { clear(); }

	TPlayPair::~TPlayPair() { }

	void setMaxBgmPairs(u32) { }

	void setMaxFilenames(u32) { }

	void setRootDir(const char*) { }

	void setMasterVol(f32) { }

	void setFile(u32, const char*, f32) { }

	THardStreamFile::~THardStreamFile() { }

	void setBgmPair(u32, u16, u16) { }

	void registBgmPair(u32, u32) { }

	void unregistBgmPair(u32) { }

	BOOL unregistBgmAll()
	{
		if (strCtrl.getList() == nullptr)
			return 0;

		strCtrl.setList(nullptr);
		for (int i = 0; i < playListMax; i++)
			playList[i].clear();

		return 1;
	}

	void playBgm(u32) { }

	void stopBgm(u32) { }

	bool checkPlaying() { return false; }

	void moveVolume(f32, u32) { }

	void main()
	{
		static DVDFileInfo finfo[3];
		static u32 cur_finfo   = 0;
		static u8 cur_addr_cmd = 0;
		static DVDCommandBlock get_addr_cmd[16];
		static DVDCommandBlock stop_cmd;
		static DVDCommandBlock finish_cmd;
		if (!useHardStreaming) {
			return;
		}
		while (true) {
			u16 introFileIndex;
			switch (strCtrl.getState()) {
			case TControl::STATE_START_REQUESTED:
				if (!strCtrl.getList())
					return;

				introFileIndex = strCtrl.getIntroNum();
				if (introFileIndex != 0xffff) {
					if (!strCtrl.startFirst(introFileIndex, finfo,
					                        &cur_finfo)) {
						strCtrl.setState(TControl::STATE_IDLE);
						return;
					}
					strCtrl.setPlayArea(TControl::PHASE_INTRO_SELECTED);
				} else {
					if (!strCtrl.startFirst(strCtrl.getLoopNum(), finfo,
					                        &cur_finfo)) {
						strCtrl.setState(TControl::STATE_IDLE);
						return;
					}
					strCtrl.clearListOne();
					strCtrl.setPlayArea(TControl::PHASE_PAIR_CONSUMED);
				}
				strCtrl.setState(TControl::STATE_WAITING_FIRST_PREPARE);
				return;
			case TControl::STATE_PREPARE_SECOND:
				if (strCtrl.getPlayArea() == TControl::PHASE_INTRO_SELECTED) {
					u16 loopFileIndex = strCtrl.getLoopNum();
					if (loopFileIndex != 0xffff) {
						if (!strCtrl.startSecond(loopFileIndex, finfo,
						                         &cur_finfo)) {
							strCtrl.setState(TControl::STATE_IDLE);
							return;
						}
						strCtrl.clearListOne();
						strCtrl.setPlayArea(TControl::PHASE_PAIR_CONSUMED);
						strCtrl.setState(TControl::STATE_POLL_PLAYBACK);
						return;
					}
				}
				strCtrl.clearListOne();
				if (!strCtrl.getList()) {
					if (strCtrl.getPlayArea()
					    == TControl::PHASE_INTRO_SELECTED) {
						DVDStopStreamAtEndAsync(&stop_cmd, 0);
						strCtrl.setState(TControl::STATE_POLL_PLAYBACK);
						strCtrl.setPlayArea(TControl::PHASE_DRAINING);
						return;
					}
					strCtrl.setPlayArea(TControl::PHASE_NO_NEXT_PAIR);
					strCtrl.setState(TControl::STATE_POLL_PLAYBACK);
					return;
				}
				introFileIndex = strCtrl.getIntroNum();
				if (introFileIndex != 0xffff) {
					if (!strCtrl.startSecond(introFileIndex, finfo,
					                         &cur_finfo)) {
						strCtrl.setState(TControl::STATE_IDLE);
						return;
					}
					strCtrl.setPlayArea(TControl::PHASE_INTRO_SELECTED);
				} else {
					if (!strCtrl.startSecond(strCtrl.getLoopNum(), finfo,
					                         &cur_finfo)) {
						strCtrl.setState(TControl::STATE_IDLE);
						return;
					}
					strCtrl.clearListOne();
					strCtrl.setPlayArea(TControl::PHASE_PAIR_CONSUMED);
				}
				strCtrl.setState(TControl::STATE_POLL_PLAYBACK);
				return;
			case TControl::STATE_POLL_PLAYBACK: {
				u8 currentVol = strCtrl.getCurVol();
				AISetStreamVolLeft(currentVol);
				AISetStreamVolRight(currentVol);
				DVDGetStreamPlayAddrAsync(&get_addr_cmd[cur_addr_cmd],
				                          getAddrCallback);
				cur_addr_cmd++;
				cur_addr_cmd %= 16;
				return;
			}
			case TControl::STATE_ADVANCE_PLAYLIST: {
				TPlayList* list = strCtrl.getList();
				if (!list) {
					if (strCtrl.getPlayArea() != TControl::PHASE_DRAINING) {
						strCtrl.setPlayArea(TControl::PHASE_NO_NEXT_PAIR);
					}
					strCtrl.setState(TControl::STATE_POLL_PLAYBACK);
					return;
				}
				if (strCtrl.getPlayArea() == TControl::PHASE_INTRO_SELECTED) {
					if (list->getPair()->getLoop() == 0xffff) {
						if (list->getNext() == nullptr) {
							DVDStopStreamAtEndAsync(&stop_cmd, 0);
							strCtrl.clearListOne();
							strCtrl.setPlayArea(TControl::PHASE_DRAINING);
						} else {
							strCtrl.clearListOne();
							strCtrl.setPlayArea(TControl::PHASE_PAIR_CONSUMED);
							break;
						}
					} else {
						if (!strCtrl.startSecond(strCtrl.getLoopNum(), finfo,
						                         &cur_finfo)) {
							strCtrl.setState(TControl::STATE_IDLE);
							return;
						}
						strCtrl.clearListOne();
						strCtrl.setPlayArea(TControl::PHASE_PAIR_CONSUMED);
					}
				} else if (!list) {
					strCtrl.setPlayArea(TControl::PHASE_NO_NEXT_PAIR);
					strCtrl.setState(TControl::STATE_POLL_PLAYBACK);
				} else {
					introFileIndex = strCtrl.getIntroNum();
					if (introFileIndex == 0xffff) {
						strCtrl.setPlayArea(TControl::PHASE_INTRO_SELECTED);
						break;
					}
					if (!strCtrl.startSecond(introFileIndex, finfo,
					                         &cur_finfo)) {
						strCtrl.setState(TControl::STATE_IDLE);
						return;
					}
					strCtrl.setPlayArea(TControl::PHASE_INTRO_SELECTED);
				}
				strCtrl.setState(TControl::STATE_POLL_PLAYBACK);
				return;
			}
			case TControl::STATE_STOP_REQUESTED:
				AISetStreamVolLeft(0);
				AISetStreamVolRight(0);
				AISetStreamPlayState(0);
				DVDCancelStreamAsync(&finish_cmd, nullptr);
				strCtrl.setState(TControl::STATE_IDLE);
				strCtrl.setPlayArea(TControl::PHASE_INTRO_SELECTED);
				strCtrl.resetFader();
				strCtrl.setRestart(0);
				return;
			case TControl::STATE_IDLE:
			case TControl::STATE_WAITING_FIRST_PREPARE:
			default:
				return;
			}
		}
	}

	TPlayList* getFreeList() { return 0; }

	static void firstBgmCallback(s32, DVDFileInfo*)
	{
		if (strCtrl.getRestart() == 0) {
			u8 vol = strCtrl.getCurVol();
			AISetStreamVolLeft(vol);
			AISetStreamVolRight(vol);
			AISetStreamSampleRate(1);
			AIResetStreamSampleCount();
			AISetStreamPlayState(1);
			strCtrl.setState(TControl::STATE_PREPARE_SECOND);
		} else {
			strCtrl.setState(TControl::STATE_POLL_PLAYBACK);
			strCtrl.setRestart(0);
		}
	}

	static void secondBgmCallback(s32, DVDFileInfo*)
	{
		strCtrl.setState(TControl::STATE_POLL_PLAYBACK);
	}

	static void getAddrCallback(s32 play_address, DVDCommandBlock*)
	{
		static BOOL last_frame = 0;
		if (strCtrl.getPlayArea() == TControl::PHASE_NO_NEXT_PAIR) {
			if (play_address == strCtrl.getLastAddrBefore()) {
				last_frame = true;
			} else if (last_frame) {
				last_frame = false;
				strCtrl.setState(TControl::STATE_ADVANCE_PLAYLIST);
			}
		} else if (strCtrl.getPlayArea() == TControl::PHASE_DRAINING) {
			if (play_address == strCtrl.getLastAddrBefore() - 0x8000) {
				last_frame = true;
			} else if (last_frame) {
				strCtrl.setState(TControl::STATE_STOP_REQUESTED);
				unregistBgmAll();
				last_frame = false;
			}
		} else {
			if (play_address == strCtrl.getLastAddr()) {
				last_frame = true;
			} else if (last_frame) {
				last_frame = false;
				strCtrl.setState(TControl::STATE_ADVANCE_PLAYLIST);
			}
		}
	}

	TControl::TControl()
	{
		unk0                     = 0;
		mPlaylistHead            = nullptr;
		mRestart                 = 0;
		mPlaybackPhase           = PHASE_INTRO_SELECTED;
		mState                   = STATE_IDLE;
		mMasterVolume            = 1.0f;
		mOutputVolume            = 0.0f;
		mFadeInDurationTicks     = 0;
		mFadeOutDurationTicks    = 0;
		mVolumeMoveDurationTicks = 0;
		mFadeInStep              = 0.0f;
		mFadeOutStep             = 0.0f;
		mVolumeMoveCurrent       = 1.0f;
		mVolumeMoveStart         = 1.0f;
		mVolumeMoveTarget        = 1.0f;
		mVolumeMoveStep          = 0.0f;
		mFadeInElapsedTicks      = 0;
		mFadeOutElapsedTicks     = 0;
		mVolumeMoveElapsedTicks  = 0;
		mNextBoundarySlot        = 0;
	}

	void TControl::setMasterVol(f32) { }

	u16 TControl::getIntroNum()
	{
		if (!mPlaylistHead)
			return 0xffff;

		if (mPlaylistHead->getPair() == NULL)
			return 0xffff;

		return mPlaylistHead->getPair()->getIntro();
	}

	u16 TControl::getLoopNum()
	{
		if (!mPlaylistHead)
			return 0xffff;

		if (mPlaylistHead->getPair() == NULL)
			return 0xffff;

		return mPlaylistHead->getPair()->getLoop();
	}

	BOOL TControl::fileOpen(u16 file_index, DVDFileInfo* file_info)
	{
		char buffer[64];
		char* ptr = streamFiles + file_index * 0x24;
		strcpy(buffer, rootDir);
		strcat(buffer, ptr);
		if (!DVDOpen(buffer, file_info))
			return false;

		return true;
	}

	BOOL TControl::clearListOne()
	{
		if (mPlaylistHead == NULL)
			return FALSE;

		TPlayList* oldList = mPlaylistHead;
		mPlaylistHead      = mPlaylistHead->getNext();
		oldList->clear();
		return TRUE;
	}

	void TControl::setLastAddr(DVDFileInfo* file_info)
	{
		mLastBlockAddresses[mNextBoundarySlot]
		    = file_info->startAddr + file_info->length - 0x8000;
		mNextBoundarySlot ^= 1;
	}

	u32 TControl::getLastAddr()
	{
		return mLastBlockAddresses[mNextBoundarySlot];
	}

	BOOL TControl::startFirst(u16 file_index, DVDFileInfo* file_infos,
	                          u32* next_file_info_index)
	{
		if (!fileOpen(file_index, file_infos + *next_file_info_index))
			return false;

		setLastAddr(file_infos + *next_file_info_index);
		DVDPrepareStreamAsync(file_infos + *next_file_info_index, 0, 0,
		                      firstBgmCallback);
		next_file_info_index[0]++;
		next_file_info_index[0] %= 3;
		return true;
	}

	BOOL TControl::startSecond(u16 file_index, DVDFileInfo* file_infos,
	                           u32* next_file_info_index)
	{
		if (!fileOpen(file_index, file_infos + *next_file_info_index))
			return false;

		setLastAddr(file_infos + *next_file_info_index);
		DVDPrepareStreamAsync(file_infos + *next_file_info_index, 0, 0,
		                      secondBgmCallback);
		next_file_info_index[0]++;
		next_file_info_index[0] %= 3;
		return true;
	}

	void TControl::resetFader()
	{
		mFadeInDurationTicks     = 0;
		mFadeInStep              = 0.0f;
		mFadeInElapsedTicks      = 0;
		mFadeOutDurationTicks    = 0;
		mFadeOutStep             = 0.0f;
		mFadeOutElapsedTicks     = 0;
		mVolumeMoveStart         = 1.0f;
		mVolumeMoveCurrent       = 1.0f;
		mVolumeMoveTarget        = 1.0f;
		mVolumeMoveDurationTicks = 0;
		mVolumeMoveElapsedTicks  = 0;
	}

	u8 TControl::getCurVol()
	{
		calcCurVolume();
		return volFloatToU8(mOutputVolume);
	}

	void TControl::calcCurVolume()
	{
		f32 outputVolume = 1.0f;
		f32 fadeInGain   = 1.0f;
		f32 fadeOutGain  = 1.0f;

		if (mFadeInDurationTicks != 0) {
			if (mFadeInElapsedTicks == 0)
				mFadeInStep = 1.0f / (f32)mFadeInDurationTicks;

			f32 fadeInValue = mFadeInStep * (f32)mFadeInElapsedTicks;

			fadeInGain = fadeInValue;
			if (fadeInValue >= 1.0f) {
				mFadeInDurationTicks = 0;
				mFadeInStep          = 0.0f;
				mFadeInElapsedTicks  = 0;
				fadeInGain           = 1.0f;
			} else {
				++mFadeInElapsedTicks;
			}
		}

		if (mFadeOutDurationTicks != 0) {
			if (mFadeOutElapsedTicks == 0)
				mFadeOutStep = 1.0f / (f32)mFadeOutDurationTicks;

			f32 fadeOutValue
			    = -(mFadeOutStep * (f32)mFadeOutElapsedTicks - 1.0f);

			fadeOutGain = fadeOutValue;
			if (fadeOutValue <= 0.0f) {
				fadeOutGain = 0.0f;

				mFadeInDurationTicks     = 0;
				mFadeInStep              = 0.0f;
				mFadeInElapsedTicks      = 0;
				mFadeOutDurationTicks    = 0;
				mFadeOutStep             = 0.0f;
				mFadeOutElapsedTicks     = 0;
				mVolumeMoveStart         = 1.0f;
				mVolumeMoveCurrent       = 1.0f;
				mVolumeMoveTarget        = 1.0f;
				mVolumeMoveDurationTicks = 0;
				mVolumeMoveElapsedTicks  = 0;
				mState                   = STATE_STOP_REQUESTED;

				unregistBgmAll();
			} else {
				++mFadeOutElapsedTicks;
			}
		}

		if (mVolumeMoveDurationTicks != 0) {
			if (mVolumeMoveElapsedTicks == 0)
				mVolumeMoveStep = (mVolumeMoveTarget - mVolumeMoveStart)
				                  / (f32)mVolumeMoveDurationTicks;

			f32 movedVolume = mVolumeMoveStep * (f32)mVolumeMoveElapsedTicks
			                  + mVolumeMoveStart;
			if (mVolumeMoveElapsedTicks >= mVolumeMoveDurationTicks) {
				movedVolume              = mVolumeMoveTarget;
				mVolumeMoveStart         = movedVolume;
				mVolumeMoveTarget        = mVolumeMoveStart;
				mVolumeMoveDurationTicks = 0;
				mVolumeMoveStep          = 0.0f;
				mVolumeMoveElapsedTicks  = 0;
			} else {
				++mVolumeMoveElapsedTicks;
			}
			mVolumeMoveCurrent = movedVolume;
		}

		outputVolume *= mVolumeMoveCurrent * (fadeInGain * fadeOutGain);
		outputVolume *= mMasterVolume;
		mOutputVolume = outputVolume;
	}

	void TControl::extendFilename(char*, char*) { }

	u32 TControl::msecToFrames(u32) { return 0; }

	u8 TControl::volFloatToU8(f32 volume)
	{
		if (volume > 1.0f)
			volume = 1.0f;

		if (volume < 0.0f)
			volume = 0.0f;

		return volume * 255.0f;
	}

	THardStreamFile::THardStreamFile() { }

	void THardStreamFile::setNameVol(const char*, f32) { }

	TPlayPair::TPlayPair() { }

	bool TPlayPair::getRegisted() { return false; }

} // namespace HardStream

} // namespace JASystem
