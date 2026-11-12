#ifndef JASHARDSTREAM_HPP
#define JASHARDSTREAM_HPP

#include <dolphin/dvd.h>
#include <dolphin/types.h>
#include <dolphin/types.h>

namespace JASystem {

namespace HardStream {
	class TPlayList;

	class TControl {
	public:
		enum State {
			STATE_IDLE                  = 0,
			STATE_START_REQUESTED       = 1,
			STATE_PREPARE_SECOND        = 2,
			STATE_WAITING_FIRST_PREPARE = 3,
			STATE_POLL_PLAYBACK         = 4,
			STATE_ADVANCE_PLAYLIST      = 5,
			STATE_STOP_REQUESTED        = 6,
		};

		enum PlaybackPhase {
			PHASE_INTRO_SELECTED = 0,
			PHASE_PAIR_CONSUMED  = 1,
			PHASE_DRAINING       = 2,
			PHASE_NO_NEXT_PAIR   = 3,
		};

		void setMasterVol(f32);
		u16 getIntroNum();
		u16 getLoopNum();
		BOOL fileOpen(u16 file_index, DVDFileInfo* file_info);
		BOOL clearListOne();
		void setLastAddr(DVDFileInfo* file_info);
		u32 getLastAddr();
		BOOL startFirst(u16 file_index, DVDFileInfo* file_infos,
		                u32* next_file_info_index);
		BOOL startSecond(u16 file_index, DVDFileInfo* file_infos,
		                 u32* next_file_info_index);
		void resetFader();
		u8 getCurVol();
		void calcCurVolume();
		void extendFilename(char*, char*);
		u32 msecToFrames(u32);
		u8 volFloatToU8(f32 volume);
		TControl();
		~TControl() { }

		TPlayList* getList() { return mPlaylistHead; }
		void setList(TPlayList* list) { mPlaylistHead = list; }

		u16 getPlayArea() { return mPlaybackPhase; }
		void setPlayArea(u16 phase) { mPlaybackPhase = phase; }

		u8 getRestart() { return mRestart; }
		void setRestart(u8 restart) { mRestart = restart; }

		u8 getState() { return mState; }
		void setState(u8 state) { mState = state; }

		u32 getLastAddrBefore()
		{
			return mLastBlockAddresses[mNextBoundarySlot ^ 1];
		}

	public:
		/* 0x00 */ int unk0;
		/* 0x04 */ TPlayList* mPlaylistHead;
		/* 0x08 */ u16 mPlaybackPhase;
		/* 0x0A */ u8 mRestart;
		/* 0x0B */ u8 mState;
		/* 0x0C */ f32 mMasterVolume;
		/* 0x10 */ f32 mOutputVolume;
		/* 0x14 */ u32 mFadeInDurationTicks;
		/* 0x18 */ u32 mFadeOutDurationTicks;
		/* 0x1C */ u32 mVolumeMoveDurationTicks;
		/* 0x20 */ f32 mFadeInStep;
		/* 0x24 */ f32 mFadeOutStep;
		/* 0x28 */ f32 mVolumeMoveTarget;
		/* 0x2C */ f32 mVolumeMoveCurrent;
		/* 0x30 */ f32 mVolumeMoveStart;
		/* 0x34 */ f32 mVolumeMoveStep;
		/* 0x38 */ u32 mFadeInElapsedTicks;
		/* 0x3C */ u32 mFadeOutElapsedTicks;
		/* 0x40 */ u32 mVolumeMoveElapsedTicks;
		/* 0x44 */ u32 mLastBlockAddresses[2];
		/* 0x4C */ u8 mNextBoundarySlot;
	};

	struct THardStreamFile {
		THardStreamFile();
		~THardStreamFile();
		void setNameVol(const char*, f32);
	};

	struct TPlayPair {
		TPlayPair();
		~TPlayPair();
		bool getRegisted();

		u16 getIntro() { return mIntroFileIndex; }
		u16 getLoop() { return mLoopFileIndex; }

	public:
		/* 0x00 */ u16 unk0;
		/* 0x02 */ u16 unk2;
		/* 0x04 */ u16 mIntroFileIndex;
		/* 0x06 */ u16 mLoopFileIndex;
	};

	class TPlayList {
	public:
		TPlayList();
		~TPlayList();

		void clear()
		{
			mPair = nullptr;
			mNext = nullptr;
			unk8  = 0;
		}

		TPlayPair* getPair() { return mPair; }
		TPlayList* getNext() { return mNext; }

		/* 0x00 */ TPlayPair* mPair;
		/* 0x04 */ TPlayList* mNext;
		/* 0x08 */ u32 unk8;
	};

	void init();
	void main();
	void setMaxBgmPairs(u32);
	void setMaxFilenames(u32);
	void setRootDir(const char*);
	void setMasterVol(f32);
	void setFile(u32, const char*, f32);
	void setBgmPair(u32, u16, u16);
	void registBgmPair(u32, u32);
	void unregistBgmPair(u32);
	BOOL unregistBgmAll();
	void playBgm(u32);
	void stopBgm(u32);
	bool checkPlaying();
	void moveVolume(f32, u32);
	TPlayList* getFreeList();
} // namespace HardStream

} // namespace JASystem

#endif // JASHARDSTREAM_HPP
