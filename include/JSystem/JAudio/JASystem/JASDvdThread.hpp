#ifndef JASDVDTHREAD_HPP
#define JASDVDTHREAD_HPP

#include <dolphin/dvd.h>
#include <dolphin/os.h>
#include <dolphin/types.h>

namespace JASystem {

namespace Dvd {
	void unpauseDvdT();
	extern bool dvdThreadPauseFlag;
	extern bool dvdtSleep;
	char* checkFileExtend(char*);
	extern const char* audioRootPath;
	extern OSMessageQueue* mq;
	bool openDvd(char*, DVDFileInfo*);
	void registerFastOpen(char*);
	extern DVDFileInfo* dvdFile;
	extern s32 dvdEntryNum;
	extern void* dvdFileDics;
	extern s32 maxDics;
	void loadToAramDvdT(u32, char*, void*, u32, u32, u32*, void (*)(u32));
	extern bool mqInit;
	void* getCallStack();
	extern s32 curQ;
	extern void** callStackArray;
	void* loadToAramDvdTMain(void*);

	struct TDvdCall; // Forward declaration for the struct used in doError and
	                 // doFinish

	void doError(TDvdCall*, u32);
	void aramDmaFinish(u32);
	extern bool bufferFull;
	extern bool bufferLoad;
	extern void* audioDvdBuffer;
	extern void* buffers;
	extern u32 buffersize;
	bool dvdReadMutex(DVDFileInfo*, void*, s32, s32, char*);
	void doFinish(TDvdCall*, u32);
	void loadToDramDvdT(u32, char*, void*, u32, u32, u32*, void (*)(u32));
	void* loadToDramDvdTMain(void*);
	void checkPassDvdT(u32, u32*, void (*)(u32));
	void* dvdThreadCheckBack(void*);
	void pauseDvdT();
	void init();
	extern s32 dvdMsgsSize;
	extern void* msgBuf;
	void dvdProcInit();
	void* dvdProc(void*);
	void* allocDvdBuffer();
	extern u32 dvdBufSize;
	void updateBuffer();
	extern void* nextBuffers;
	extern u32 nextBufferSize;
	extern void* nextBufferTop;
	void writeBufferSize(u8*, u32, u32);
	void extendPath(char*, char*);
	bool checkFile(char*);
	bool loadFile(char*, void*);
	bool loadFileDvdT(char*, void*);
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
	void registerDvdErrorCallback(void (*)(char*, u8*));
	void setFastOpenMaxDic(s32);
	void registerExtFastOpen(char*);
	void aramDmaFinish2(u32);
	extern bool bufferFull2;
	extern void (*errorCallback)(char*, u8*);
} // namespace Dvd

} // namespace JASystem

#endif // JASDVDTHREAD_HPP
