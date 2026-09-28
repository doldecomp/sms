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
	s32 registerFastOpen(char*);
	s32 loadToAramDvdT(u32, char*, void*, u32, u32, u32*, DvdCallback);
	s32 loadToAramDvdTMain(void*);

	struct TDvdCall {
		/* 0x0 */ u32 unk0;
		/* 0x4 */ char unk4[0x20];
		/* 0x24 */ void* unk24;
		/* 0x28 */ u32 unk28;
		/* 0x2C */ u32 unk2C;
		/* 0x30 */ u32* unk30;
		/* 0x34 */ DvdCallback unk34;
	};

	s32 loadToDramDvdT(u32, char*, void*, u32, u32, u32*, DvdCallback);
	s32 loadToDramDvdTMain(void*);
	s32 checkPassDvdT(u32, u32*, DvdCallback);
	void pauseDvdT();
	void init();

	void dvdProcInit();
	void* dvdProc(void*);

	void extendPath(char*, char*);
	s32 checkFile(char*);
	s32 loadFile(char*, void*);
	u32 loadFileDvdT(char*, void*);
	void setNumOfMsgs(s32);
	void setDvdBufSize(u32);
	void setRootPath(char*);
	s32 addTaskHigh(s32 (*)(void*), void*, u32);
	s32 addTask(s32 (*)(void*), void*, u32);
	void setBufferDvdT(u8*, u32, u32);
	void closeBufferDvdT(u8*);
	void getCurrentBufferDvdT(u8**);
	s32 aramToDramDvdTMain(void*);
	s32 dramToAramDvdTMain(void*);
	s32 aramToDramDvdT(u32, void*, void*, u32, u32*, void (*)(u32));
	s32 dramToAramDvdT(u32, void*, void*, u32, u32*, void (*)(u32));

	typedef void (*ErrorCallback)(char*, u8*);
	void registerDvdErrorCallback(ErrorCallback);
	void setFastOpenMaxDic(s32);
	void registerExtFastOpen(char*);
} // namespace Dvd

} // namespace JASystem

#endif // JASDVDTHREAD_HPP
