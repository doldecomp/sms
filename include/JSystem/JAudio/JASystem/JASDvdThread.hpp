#ifndef JASDVDTHREAD_HPP
#define JASDVDTHREAD_HPP

#include <dolphin/dvd.h>

namespace JASystem {

namespace Dvd {
	void unpauseDvdT();
	extern bool dvdThreadPauseFlag;
	extern bool dvdtSleep;
	char* checkFileExtend(char*);
	extern char* audioRootPath;
	extern void* mq;
	bool openDvd(char*, DVDFileInfo*);
	void registerFastOpen(char*);
	extern DVDFileInfo* dvdFile;
	extern int dvdEntryNum;
	extern void* dvdFileDics;
	extern long maxDics;
	void loadToAramDvdT(unsigned long, char*, void*, unsigned long,
	                    unsigned long, unsigned long*, void (*)(unsigned long));
	extern bool mqInit;
	void* getCallStack();
	extern int curQ;
	extern void* callStackArray;
	void* loadToAramDvdTMain(void*);
	void doError(TDvdCall*, unsigned long);
	void aramDmaFinish(unsigned long);
	extern bool bufferFull;
	extern bool bufferLoad;
	extern void* audioDvdBuffer;
	extern void* buffers;
	extern unsigned long buffersize;
	bool dvdReadMutex(DVDFileInfo*, void*, long, long, char*);
	void doFinish(TDvdCall*, unsigned long);
	void loadToDramDvdT(unsigned long, char*, void*, unsigned long,
	                    unsigned long, unsigned long*, void (*)(unsigned long));
	void* loadToDramDvdTMain(void*);
	void checkPassDvdT(unsigned long, unsigned long*, void (*)(unsigned long));
	void* dvdThreadCheckBack(void*);
	void pauseDvdT();
	void init();
	extern long dvdMsgsSize;
	extern void* msgBuf;
	void dvdProcInit();
	void* dvdProc(void*);
	void* allocDvdBuffer();
	extern unsigned long dvdBufSize;
	void updateBuffer();
	extern void* nextBuffers;
	extern unsigned long nextBufferSize;
	extern void* nextBufferTop;
	void writeBufferSize(unsigned char*, unsigned long, unsigned long);
	void extendPath(char*, char*);
	bool checkFile(char*);
	bool loadFile(char*, void*);
	bool loadFileDvdT(char*, void*);
	void setNumOfMsgs(long);
	void setDvdBufSize(unsigned long);
	void setRootPath(char*);
	void addTaskHigh(long (*)(void*), void*, unsigned long);
	void addTask(long (*)(void*), void*, unsigned long);
	void setBufferDvdT(unsigned char*, unsigned long, unsigned long);
	void closeBufferDvdT(unsigned char*);
	void getCurrentBufferDvdT(unsigned char**);
	void* aramToDramDvdTMain(void*);
	void* dramToAramDvdTMain(void*);
	void aramToDramDvdT(unsigned long, void*, void*, unsigned long,
	                    unsigned long*, void (*)(unsigned long));
	void dramToAramDvdT(unsigned long, void*, void*, unsigned long,
	                    unsigned long*, void (*)(unsigned long));
	void registerDvdErrorCallback(void (*)(char*, unsigned char*));
	void setFastOpenMaxDic(long);
	void registerExtFastOpen(char*);
	void aramDmaFinish2(unsigned long);
	extern bool bufferFull2;

	struct TDvdCall; // Forward declaration for the struct used in doError and
	                 // doFinish
} // namespace Dvd

} // namespace JASystem

#endif // JASDVDTHREAD_HPP
