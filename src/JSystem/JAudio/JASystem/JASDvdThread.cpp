#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>

namespace JASystem {

namespace Dvd {

	bool dvdThreadPauseFlag           = false;
	bool dvdtSleep                    = false;
	const char* audioRootPath         = "";
	OSMessageQueue* mq                = 0;
	DVDFileInfo* dvdFile              = 0;
	s32 dvdEntryNum                   = 0;
	void* dvdFileDics                 = 0;
	s32 maxDics                       = 0x20;
	bool mqInit                       = false;
	s32 curQ                          = 0;
	void** callStackArray             = 0;
	bool bufferFull                   = false;
	bool bufferLoad                   = false;
	void* audioDvdBuffer              = 0;
	void* buffers                     = 0;
	u32 buffersize                    = 0;
	s32 dvdMsgsSize                   = 0x20;
	void* msgBuf                      = 0;
	u32 dvdBufSize                    = 0x8000;
	void* nextBuffers                 = 0;
	u32 nextBufferSize                = 0;
	void* nextBufferTop               = 0;
	bool bufferFull2                  = false;
	void (*errorCallback)(char*, u8*) = 0;

	void unpauseDvdT() { }
	u32 checkFileExtend(char* path) { return 0; }
	bool openDvd(char* path, DVDFileInfo* fileInfo) { return false; }
	void registerFastOpen(char* path) { }
	u32 loadToAramDvdT(u32 param1, char* path, void* buffer, u32 size,
	                   u32 param5, u32* param6, void (*callback)(u32))
	{
	}
	void* getCallStack() { return 0; }
	void* loadToAramDvdTMain(void* param) { return 0; }
	void doError(TDvdCall* call, u32 param) { }
	void aramDmaFinish(u32 param) { }
	bool dvdReadMutex(DVDFileInfo* fileInfo, void* buffer, s32 size, s32 offset,
	                  char* path)
	{
		return false;
	}
	void doFinish(TDvdCall* call, u32 param) { }
	void loadToDramDvdT(u32 param1, char* path, void* buffer, u32 size,
	                    u32 param5, u32* param6, void (*callback)(u32))
	{
	}
	void* loadToDramDvdTMain(void* param) { return 0; }
	void checkPassDvdT(u32 param1, u32* param2, void (*callback)(u32)) { }
	void* dvdThreadCheckBack(void* param) { return 0; }
	void pauseDvdT() { }
	void init() { }
	void dvdProcInit() { }
	void* dvdProc(void* param) { return 0; }
	void* allocDvdBuffer() { return 0; }
	void updateBuffer() { }
	void writeBufferSize(u8* buffer, u32 size, u32 param3) { }
	void extendPath(char* path1, char* path2) { }
	bool checkFile(char* path) { return false; }
	bool loadFile(char* path, void* buffer) { return false; }
	bool loadFileDvdT(char* path, void* buffer) { return false; }
	void setNumOfMsgs(s32 num) { }
	void setDvdBufSize(u32 size) { }
	void setRootPath(char* path) { }
	void addTaskHigh(s32 (*func)(void*), void* param, u32 param3) { }
	void addTask(s32 (*func)(void*), void* param, u32 param3) { }
	void setBufferDvdT(u8* buffer, u32 size, u32 param3) { }
	void closeBufferDvdT(u8* buffer) { }
	void getCurrentBufferDvdT(u8** buffer) { }
	void* aramToDramDvdTMain(void* param) { return 0; }
	void* dramToAramDvdTMain(void* param) { return 0; }
	void aramToDramDvdT(u32 param1, void* src, void* dest, u32 size,
	                    u32* param5, void (*callback)(u32))
	{
	}
	void dramToAramDvdT(u32 param1, void* src, void* dest, u32 size,
	                    u32* param5, void (*callback)(u32))
	{
	}
	void registerDvdErrorCallback(void (*callback)(char*, u8*)) { }
	void setFastOpenMaxDic(s32 max) { }
	void registerExtFastOpen(char* path) { }
	void aramDmaFinish2(u32 param) { }

} // namespace Dvd

} // namespace JASystem
