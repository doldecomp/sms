#ifndef JASDVDTHREAD_HPP
#define JASDVDTHREAD_HPP

#include <dolphin/dvd.h>
#include <dolphin/os.h>
#include <dolphin/types.h>

namespace JASystem {

namespace Dvd {
	typedef void (*DvdCallback)(u32);

	void unpauseDvdT();

	u32 checkFileExtend(char*);
	s32 openDvd(char*, DVDFileInfo*);
	int registerFastOpen(char*);
	u32 loadToAramDvdT(u32, char*, void*, u32, u32, u32*, DvdCallback);
	int loadToAramDvdTMain(void*);

	struct TDvdCall {
		/* 0x0 */ u32 unk0;
		/* 0x4 */ char unk4[0x20];
		/* 0x24 */ void* unk24;
		/* 0x28 */ u32 unk28;
		/* 0x2C */ u32 unk2C;
		/* 0x30 */ u32* unk30;
		/* 0x34 */ DvdCallback unk34;
	};

	int loadToDramDvdT(u32, char*, void*, u32, u32, u32*, DvdCallback);
	int loadToDramDvdTMain(void*);
	int checkPassDvdT(u32, u32*, DvdCallback);
	void pauseDvdT();
	void init();

	void dvdProcInit();
	void* dvdProc(void*);

	void extendPath(char*, char*);
	int checkFile(char*);
	int loadFile(char*, void*);
	u32 loadFileDvdT(char*, void*);
	void setNumOfMsgs(s32);
	void setDvdBufSize(u32);
	void setRootPath(char*);
	void addTaskHigh(s32 (*)(void*), void*, u32);
	void addTask(s32 (*)(void*), void*, u32);
	void setBufferDvdT(u8*, u32, u32);
	void closeBufferDvdT(u8*);
	void getCurrentBufferDvdT(u8**);
	void* aramToDramDvdTMain(void*);
	void* dramToAramDvdTMain(void*);
	void aramToDramDvdT(u32, void*, void*, u32, u32*, void (*)(u32));
	void dramToAramDvdT(u32, void*, void*, u32, u32*, void (*)(u32));

	typedef void (*ErrorCallback)(char*, u8*);
	void registerDvdErrorCallback(ErrorCallback);
	void setFastOpenMaxDic(s32);
	void registerExtFastOpen(char*);
} // namespace Dvd

} // namespace JASystem

#endif // JASDVDTHREAD_HPP
