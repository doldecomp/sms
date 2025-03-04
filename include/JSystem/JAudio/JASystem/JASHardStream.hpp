#ifndef JASHARDSTREAM_HPP
#define JASHARDSTREAM_HPP

#include <dolphin/dvd.h>
#include <dolphin/types.h>

namespace JASystem {

namespace HardStream {
	struct TControl {
		void setMasterVol(f32);
		u16 getIntroNum();
		u16 getLoopNum();
		void fileOpen(u16, DVDFileInfo*);
		void clearListOne();
		void setLastAddr(DVDFileInfo*);
		u32 getLastAddr();
		void startFirst(u16, DVDFileInfo*, u32*);
		void startSecond(u16, DVDFileInfo*, u32*);
		void resetFader();
		f32 getCurVol();
		void calcCurVolume();
		void extendFilename(char*, char*);
		u32 msecToFrames(u32);
		u8 volFloatToU8(f32);
		TControl();
		~TControl();
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
	};

	struct TPlayList {
		TPlayList();
		~TPlayList();
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
	void unregistBgmAll();
	void playBgm(u32);
	void stopBgm(u32);
	bool checkPlaying();
	void moveVolume(f32, u32);
	TPlayList* getFreeList();

	s32 firstBgmCallback(s32, DVDFileInfo*);
	s32 secondBgmCallback(s32, DVDFileInfo*);
	s32 getAddrCallback(s32, DVDCommandBlock*);

	extern TControl* strCtrl;
	extern char* rootDir;
	extern THardStreamFile* streamFiles;
	extern u32 streamFilesMax;
	extern u32 playPairsMax;
	extern u32 playListMax;
	extern bool useHardStreaming;
	extern bool initFilenameHeapFlag;
	extern TPlayPair* playPairs;
	extern u32 playPairsCount;
	extern TPlayList* playList;
	extern u32 qId;
} // namespace HardStream

} // namespace JASystem

#endif // JASHARDSTREAM_HPP
