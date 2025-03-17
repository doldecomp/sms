#ifndef JASDVDTHREAD_HPP
#define JASDVDTHREAD_HPP

#include <dolphin/dvd.h>
#include <dolphin/os.h>
#include <dolphin/types.h>

namespace JASystem {

namespace Dvd {
	void unpauseDvdT();

	u32 checkFileExtend(char*);
	s32 openDvd(char*, DVDFileInfo*);
	int registerFastOpen(char*);
	u32 loadToAramDvdT(u32, char*, void*, u32, u32, u32*, void (*)(u32));
	void* getCallStack();
	void* loadToAramDvdTMain(void*);

	typedef void (*DvdCallback)(u32);
	struct TDvdCall {
		/* 0x0 */ u32 unk0;
		/* 0x4 */ char unk4[0x20];
		/* 0x24 */ void* unk24;
		/* 0x28 */ u32 unk28;
		/* 0x2C */ u32 unk2C;
		/* 0x30 */ u32* unk30;
		/* 0x34 */ DvdCallback unk34;
	};

	void doError(TDvdCall*, u32);
	void aramDmaFinish(u32);

	void dvdReadMutex(DVDFileInfo*, void*, s32, s32, char*);
	void doFinish(TDvdCall*, u32);
	int loadToDramDvdT(u32, char*, void*, u32, u32, u32*, void (*)(u32));
	void* loadToDramDvdTMain(void*);
	void checkPassDvdT(u32, u32*, void (*)(u32));
	bool dvdThreadCheckBack(void*);
	void pauseDvdT();
	void init();

	void dvdProcInit();
	void* dvdProc(void*);
	void allocDvdBuffer();

	void updateBuffer();
	void writeBufferSize(u8*, u32, u32);
	void extendPath(char*, char*);
	DVDCallback checkFile(char*);
	bool loadFile(char*, void*);
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
	void aramDmaFinish2(u32);
} // namespace Dvd

} // namespace JASystem

#endif // JASDVDTHREAD_HPP
