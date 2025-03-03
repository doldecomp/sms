#ifndef JASHARDSTREAM_HPP
#define JASHARDSTREAM_HPP

#include <dolphin/dvd.h>

namespace JASystem {

namespace HardStream {
	struct TControl;
	struct THardStreamFile;
	struct TPlayPair;
	struct TPlayList;

	void init();
	void main();
	void setMaxBgmPairs(unsigned long);
	void setMaxFilenames(unsigned long);
	void setRootDir(const char*);
	void setMasterVol(float);
	void setFile(unsigned long, const char*, float);
	void setBgmPair(unsigned long, unsigned short, unsigned short);
	void registBgmPair(unsigned long, unsigned long);
	void unregistBgmPair(unsigned long);
	void unregistBgmAll();
	void playBgm(unsigned long);
	void stopBgm(unsigned long);
	bool checkPlaying();
	void moveVolume(float, unsigned long);
	TPlayList* getFreeList();

	long firstBgmCallback(long, DVDFileInfo*);
	long secondBgmCallback(long, DVDFileInfo*);
	long getAddrCallback(long, DVDCommandBlock*);

	extern TControl* strCtrl;
	extern char* rootDir;
	extern THardStreamFile* streamFiles;
	extern unsigned long streamFilesMax;
	extern unsigned long playPairsMax;
	extern unsigned long playListMax;
	extern bool useHardStreaming;
	extern bool initFilenameHeapFlag;
	extern TPlayPair* playPairs;
	extern unsigned long playPairsCount;
	extern TPlayList* playList;
	extern unsigned long qId;
} // namespace HardStream

} // namespace JASystem

#endif // JASHARDSTREAM_HPP
