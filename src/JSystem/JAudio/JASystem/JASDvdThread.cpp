#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/os.h>
#include <dolphin/dvd.h>

namespace JASystem {

namespace Dvd {

	static OSMessageQueue mq;

	static char audioRootPath[0x20] = "";

	static u32 mqInit = false;

	static u32 buffersize = 0;
	static void* buffers  = 0;

	static u32 nextBufferSize  = 0;
	static void* nextBufferTop = 0;
	static void* nextBuffers   = 0;

	static u8 audioDvdBuffer[8];
	static u32 bufferLoad = false;
	static u32 bufferFull = false;

	typedef u8 FabricatedCallstack[0x40];
	static FabricatedCallstack* callStackArray = 0;
	static void** msgBuf                       = 0;
	static u32 curQ                            = 0;

	static OSThreadQueue dvdtSleep;
	static BOOL dvdThreadPauseFlag = false;

	static ErrorCallback errorCallback = 0;
	static u32 dvdFileDics             = 0;
	static char** dvdFile              = 0;
	static s32* dvdEntryNum            = 0;

	static s32 dvdMsgsSize = 0x20;
	static u32 dvdBufSize  = 0x8000;
	static s32 maxDics     = 0x20;

} // namespace Dvd

void Dvd::init()
{
	callStackArray = new (JASDram, 0) FabricatedCallstack[dvdMsgsSize];
	msgBuf         = new (JASDram, 0) void*[dvdMsgsSize];
}

void Dvd::setNumOfMsgs(s32 num) { }
void Dvd::setDvdBufSize(u32 size) { }
void Dvd::setRootPath(char* path) { }
void Dvd::extendPath(char* path1, char* path2) { }
void Dvd::addTaskHigh(s32 (*func)(void*), void* param, u32 param3) { }
void Dvd::addTask(s32 (*func)(void*), void* param, u32 param3) { }
void Dvd::dvdProcInit()
{
	OSInitMessageQueue(&mq, msgBuf, 0x20);
	mqInit  = 1;
	dvdFile = new (JASDram, 0) char*[maxDics];
	for (int i = 0; i < maxDics; ++i)
		dvdFile[i] = new (JASDram, 0) char[64];
	dvdEntryNum = new (JASDram, 0) s32[maxDics];
}
void* Dvd::dvdProc(void* param)
{
	u32 message[2];
	allocDvdBuffer();
	u8* buf = (u8*)Kernel::allocFromSysDram(dvdBufSize * 2);
	OSInitThreadQueue(&dvdtSleep);
	while (true) {
		OSReceiveMessage(&mq, &message, 1);
		u32 r30 = message[0];
		updateBuffer();
		if (buffersize == 0)
			writeBufferSize(buf, 2, dvdBufSize);

		if (r30 == 0)
			continue;

		typedef void (*Func)(u32);
		(*(Func*)r30)(r30);
	}
}

void* Dvd::loadToDramDvdTMain(void* param) { return 0; }
int Dvd::loadToDramDvdT(u32 param1, char* path, void* buffer, u32 size,
                        u32 param5, u32* param6, void (*callback)(u32))
{
	TDvdCall callData;
	TDvdCall* call = &callData;

	call->unk0 = param1;
	strcpy(call->unk4, path);
	call->unk24 = buffer;

	call->unk30 = param6;
	if (param6 != 0)
		*param6 = 0;

	call->unk34 = callback;
	call->unk28 = size;
	call->unk2C = param5;

	if (mqInit) {
		void* cs = getCallStack();
		Calc::bcopy(call, (void**)cs + 1, 0x38);
		*(void**)cs = (void*)&loadToDramDvdTMain;
		OSSendMessage(&mq, cs, 1);
	}

	return 0;
}

void Dvd::setBufferDvdT(u8* buffer, u32 size, u32 param3) { }
void Dvd::closeBufferDvdT(u8* buffer) { }
void Dvd::getCurrentBufferDvdT(u8** buffer) { }

void* Dvd::loadToAramDvdTMain(void* param) { return 0; }
u32 Dvd::loadToAramDvdT(u32 param1, char* path, void* buffer, u32 size,
                        u32 param5, u32* param6, void (*callback)(u32))
{
}

void* Dvd::aramToDramDvdTMain(void* param) { return 0; }
void* Dvd::dramToAramDvdTMain(void* param) { return 0; }
void Dvd::aramToDramDvdT(u32 param1, void* src, void* dest, u32 size,
                         u32* param5, void (*callback)(u32))
{
}
void Dvd::dramToAramDvdT(u32 param1, void* src, void* dest, u32 size,
                         u32* param5, void (*callback)(u32))
{
}

u32 Dvd::checkFileExtend(char* path) { return 0; }

u32 Dvd::loadFileDvdT(char* path, void* buffer) { return false; }
void Dvd::checkPassDvdT(u32 param1, u32* param2, void (*callback)(u32)) { }
DVDCallback Dvd::checkFile(char* path)
{
	static DVDFileInfo finfo;
	if (!openDvd(path, &finfo)) {
		return nullptr;
	} else {
		DVDCallback result = finfo.callback;
		DVDClose(&finfo);
		return result;
	}
}
bool Dvd::loadFile(char* path, void* buffer) { return false; }
void Dvd::pauseDvdT() { dvdThreadPauseFlag = 1; }
void Dvd::unpauseDvdT()
{
	if (dvdThreadPauseFlag == 1)
		OSWakeupThread(&dvdtSleep);
	dvdThreadPauseFlag = 0;
}

void Dvd::registerDvdErrorCallback(void (*callback)(char*, u8*)) { }
void Dvd::setFastOpenMaxDic(s32 max) { }
void Dvd::registerExtFastOpen(char* path) { }
int Dvd::registerFastOpen(char* path)
{
	if (strlen(path) > 63)
		return -1;

	for (int i = 0; i < dvdFileDics; ++i) {
		if (strcmp(dvdFile[i], path) == 0)
			return dvdEntryNum[i];
	}

	if (dvdFileDics == maxDics)
		return -1;

	s32 entryNum = DVDConvertPathToEntrynum(path);
	if (entryNum != -1) {
		strcpy(dvdFile[dvdFileDics], path);
		dvdEntryNum[dvdFileDics] = entryNum;
		++dvdFileDics;
	}
	return entryNum;
}

s32 Dvd::openDvd(char* path, DVDFileInfo* fileInfo)
{
	s32 entryNum = registerFastOpen(path);
	if (entryNum == -1)
		DVDOpen(path, fileInfo);
	else
		DVDFastOpen(entryNum, fileInfo);
	// TODO: is this a bug that they forgot to return entryNum
	// but the assembly HAPPENS to work out in such a way that
	// r3 still contains entryNum?
	// Or is this a fakematch?
}

void* Dvd::getCallStack()
{
	BOOL enable = OSDisableInterrupts();

	void* cs = &callStackArray[curQ++];
	if (curQ == 0x20)
		curQ = 0;

	OSRestoreInterrupts(enable);
	return cs;
}

void Dvd::dvdReadMutex(DVDFileInfo* fileInfo, void* buffer, s32 size,
                       s32 offset, char* path)
{
	if (dvdThreadPauseFlag == 1)
		OSSleepThread(&dvdtSleep);

	while (true) {
		s32 sz = DVDReadPrio(fileInfo, buffer, size, offset, 2);
		if (sz != -1)
			break;

		if (errorCallback == nullptr)
			break;

		errorCallback(path, (u8*)buffer);
	}
}
void Dvd::doError(TDvdCall* call, u32 param)
{
	if (call->unk30 != nullptr)
		*call->unk30 = -1;

	if (call->unk34)
		call->unk34(-1);
}

void Dvd::doFinish(TDvdCall* call, u32 param)
{
	if (call->unk30 != nullptr)
		*call->unk30 = param;

	if (call->unk34)
		call->unk34(call->unk0);
}
bool Dvd::dvdThreadCheckBack(void* param)
{
	TDvdCall* call = (TDvdCall*)param;
	u32 thing      = call->unk0;
	if (call->unk30 != nullptr)
		*call->unk30 = thing;

	if (call->unk34)
		call->unk34(call->unk0);

	return false;
}
void Dvd::allocDvdBuffer() { }
void Dvd::writeBufferSize(u8* buffer, u32 size, u32 param3) { }
void Dvd::updateBuffer() { }

void Dvd::aramDmaFinish(u32 param) { --bufferFull; }
void Dvd::aramDmaFinish2(u32 param) { }

} // namespace JASystem
