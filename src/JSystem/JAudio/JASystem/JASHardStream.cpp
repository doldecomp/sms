#include <JSystem/JAudio/JASystem/JASHardStream.hpp>

namespace JASystem {

namespace HardStream {

	TControl* strCtrl            = 0;
	char* rootDir                = "";
	THardStreamFile* streamFiles = 0;
	u32 streamFilesMax           = 0;
	u32 playPairsMax             = 0;
	u32 playListMax              = 0x20;
	bool useHardStreaming        = false;
	bool initFilenameHeapFlag    = false;
	TPlayPair* playPairs         = 0;
	u32 playPairsCount           = 0;
	TPlayList* playList          = 0;
	u32 qId                      = 0;

	void init() { }

	void main() { }

	void setMaxBgmPairs(u32) { }

	void setMaxFilenames(u32) { }

	void setRootDir(const char*) { }

	void setMasterVol(f32) { }

	void setFile(u32, const char*, f32) { }

	void setBgmPair(u32, u16, u16) { }

	void registBgmPair(u32, u32) { }

	void unregistBgmPair(u32) { }

	void unregistBgmAll() { }

	void playBgm(u32) { }

	void stopBgm(u32) { }

	bool checkPlaying() { return false; }

	void moveVolume(f32, u32) { }

	TPlayList* getFreeList() { return 0; }

	s32 firstBgmCallback(s32, DVDFileInfo*) { return 0; }

	s32 secondBgmCallback(s32, DVDFileInfo*) { return 0; }

	s32 getAddrCallback(s32, DVDCommandBlock*) { return 0; }

	// TControl implementation
	void TControl::setMasterVol(f32) { }
	u16 TControl::getIntroNum() { return 0; }
	u16 TControl::getLoopNum() { return 0; }
	void TControl::fileOpen(u16, DVDFileInfo*) { }
	void TControl::clearListOne() { }
	void TControl::setLastAddr(DVDFileInfo*) { }
	u32 TControl::getLastAddr() { return 0; }
	void TControl::startFirst(u16, DVDFileInfo*, u32*) { }
	void TControl::startSecond(u16, DVDFileInfo*, u32*) { }
	void TControl::resetFader() { }
	f32 TControl::getCurVol() { return 0.0f; }
	void TControl::calcCurVolume() { }
	void TControl::extendFilename(char*, char*) { }
	u32 TControl::msecToFrames(u32) { return 0; }
	u8 TControl::volFloatToU8(f32) { return 0; }
	TControl::TControl() { }
	TControl::~TControl() { }

	// THardStreamFile implementation
	THardStreamFile::THardStreamFile() { }
	THardStreamFile::~THardStreamFile() { }
	void THardStreamFile::setNameVol(const char*, f32) { }

	// TPlayPair implementation
	TPlayPair::TPlayPair() { }
	TPlayPair::~TPlayPair() { }
	bool TPlayPair::getRegisted() { return false; }

	// TPlayList implementation
	TPlayList::TPlayList() { }
	TPlayList::~TPlayList() { }

} // namespace HardStream

} // namespace JASystem
