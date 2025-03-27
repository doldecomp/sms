#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/os.h>
#include <dolphin/dvd.h>
#include <dolphin/ar.h>
#include <macros.h>

namespace JASystem {

namespace Dvd {

	static OSMessageQueue mq;

	static char audioRootPath[0x20] = "";

	static u32 mqInit = false;

	static u32 buffersize = 0;
	static u32 buffers    = 0;

	static u32 nextBufferSize = 0;
	static u8* nextBufferTop  = 0;
	static u32 nextBuffers    = 0;

	static u8* audioDvdBuffer[2];
	static u32 bufferLoad          = false;
	static volatile u32 bufferFull = false;

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

	static void* getCallStack();
	static int dvdReadMutex(DVDFileInfo*, void*, s32, s32, char*);
	static void doError(TDvdCall*, u32);
	static void doFinish(TDvdCall*, u32);
	static bool dvdThreadCheckBack(void*);
	static void allocDvdBuffer();
	static void writeBufferSize(u8*, u32, u32);
	static void updateBuffer();
	static void aramDmaFinish(u32);
	static void aramDmaFinish2(u32);
} // namespace Dvd

void Dvd::init()
{
	callStackArray = new (JASDram, 0) FabricatedCallstack[dvdMsgsSize];
	msgBuf         = new (JASDram, 0) void*[dvdMsgsSize];
}

void Dvd::setNumOfMsgs(s32 num) { }
void Dvd::setDvdBufSize(u32 size) { }
void Dvd::setRootPath(char* path) { }
void Dvd::extendPath(char* path1, char* path2)
{
	if (audioRootPath[0]) {
		strcpy(path1, audioRootPath);
		if (path2[0] == '/')
			strcat(path1, path2 + 1);
		else
			strcat(path1, path2);
	} else {
		strcpy(path1, path2);
	}
}
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
	void* cs;
	u8* buf;

	allocDvdBuffer();
	buf = (u8*)Kernel::allocFromSysDram(dvdBufSize * 2);
	OSInitThreadQueue(&dvdtSleep);
	while (true) {
		u32 message;
		OSReceiveMessage(&mq, &message, 1);
		cs = (void*)message;
		updateBuffer();
		if (buffersize == 0)
			writeBufferSize(buf, 2, dvdBufSize);

		if (cs == 0)
			continue;

		(*(int (**)(void*))cs)(((u8*)cs) + 4);
	}
}

int Dvd::loadToDramDvdTMain(void* param)
{
	s32 ret;
	TDvdCall* call = (TDvdCall*)param;
	DVDFileInfo finfo;
	char path[64];

	extendPath(path, call->unk4);

	if (!openDvd(path, &finfo)) {
		doError(call, 0);
		return -1;
	}

	if (finfo.length == 0) {
		doError(call, 1);
		return -1;
	}

	if (call->unk2C == 0) {
		call->unk2C = finfo.length;
		call->unk2C -= call->unk28;
	}

	DCInvalidateRange(call->unk24, call->unk2C);
	ret = dvdReadMutex(&finfo, call->unk24, call->unk2C, call->unk28,
	                   call->unk4);
	DVDClose(&finfo);
	doFinish(call, ret);
	return 0;
}

int Dvd::loadToDramDvdT(u32 param1, char* path, void* buffer, u32 size,
                        u32 param5, u32* param6, DvdCallback callback)
{
	TDvdCall callData;
	void* cs;
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
		cs = getCallStack();
		Calc::bcopy(call, (void**)cs + 1, 0x38);
		*(void**)cs = (void*)&loadToDramDvdTMain;
		OSSendMessage(&mq, cs, 1);
	}

	return 0;
}

void Dvd::setBufferDvdT(u8* buffer, u32 size, u32 param3) { }
void Dvd::closeBufferDvdT(u8* buffer) { }
void Dvd::getCurrentBufferDvdT(u8** buffer) { }

int Dvd::loadToAramDvdTMain(void* param)
{
	TDvdCall* call = (TDvdCall*)param;
	s32 ret;
	u32 batchSize;
	void* buf;
	char path[64];

	static DVDFileInfo finfo;
	static ARQRequest req[4];
	static u32 arq_index = 0;

	extendPath(path, call->unk4);

	if (!openDvd(path, &finfo)) {
		doError(call, 0);
		return -1;
	}

	ret = finfo.length;

	if (finfo.length == 0) {
		doError(call, 1);
		return -1;
	}

	if (call->unk2C == 0) {
		call->unk2C = ret;
		call->unk2C -= call->unk28;
	}

	OSTick time = OSGetTick();
	while (call->unk2C) {
		buf        = audioDvdBuffer[bufferLoad];
		bufferLoad = (bufferLoad + 1) % buffers;

		while (bufferFull == buffers)
			;

		if (call->unk2C < buffersize) {
			batchSize = ALIGN_NEXT(call->unk2C, 32);
			ret = dvdReadMutex(&finfo, buf, batchSize, call->unk28, call->unk4);
			call->unk2C = 0;
		} else {
			batchSize = buffersize;
			ret = dvdReadMutex(&finfo, buf, batchSize, call->unk28, call->unk4);
			call->unk28 += buffersize;
			call->unk2C -= buffersize;
		}
		ARQPostRequest(&req[arq_index], 0x12345678, 0, 1, (u32)buf,
		               (u32)call->unk24, batchSize, &aramDmaFinish);
		++bufferFull;
		++arq_index;
		arq_index %= 4;
		call->unk24 = (u8*)call->unk24 + batchSize;
	}
	time -= DVDClose(&finfo);
	while (bufferFull != 0)
		;
	OSGetTick();
	doFinish(call, ret);

	return 0;
}
int Dvd::loadToAramDvdT(u32 param1, char* path, void* buffer, u32 size,
                        u32 param5, u32* param6, DvdCallback callback)
{
	TDvdCall callData;
	void* cs;
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
		cs = getCallStack();
		Calc::bcopy(call, (void**)cs + 1, 0x38);
		*(void**)cs = (void*)&loadToAramDvdTMain;
		OSSendMessage(&mq, cs, 1);
	}

	return 0;
}

void* Dvd::aramToDramDvdTMain(void* param)
{

	static DVDFileInfo finfo;
	static ARQRequest req;
	return 0;
}
void* Dvd::dramToAramDvdTMain(void* param)
{
	static DVDFileInfo finfo;
	static ARQRequest req;
	return 0;
}
void Dvd::aramToDramDvdT(u32 param1, void* src, void* dest, u32 size,
                         u32* param5, void (*callback)(u32))
{
}
void Dvd::dramToAramDvdT(u32 param1, void* src, void* dest, u32 size,
                         u32* param5, void (*callback)(u32))
{
}

u32 Dvd::checkFileExtend(char* path)
{
	static DVDFileInfo finfo;
	char realPath[64];

	extendPath(realPath, path);

	if (!openDvd(realPath, &finfo))
		return 0;

	u32 extend = finfo.length;

	DVDClose(&finfo);

	return extend;
}

u32 Dvd::loadFileDvdT(char* path, void* buffer)
{
	// TODO: how do we make this inline? It becomes OK when it inlines...
	volatile u32 done = 0;
	loadToDramDvdT(0, path, buffer, 0, 0, (u32*)&done, nullptr);

	while (!done)
		;

	if (done == 0xffffffff)
		return 0;
	else
		return done;
}
int Dvd::checkPassDvdT(u32 param1, u32* param2, void (*callback)(u32))
{
	TDvdCall callData;
	void* cs;
	TDvdCall* call = &callData;

	callData.unk0  = param1;
	callData.unk30 = param2;
	callData.unk34 = callback;

	if (mqInit) {
		cs = getCallStack();
		Calc::bcopy(call, (void**)cs + 1, 0x38);
		*(void**)cs = (void*)&dvdThreadCheckBack;
		OSSendMessage(&mq, cs, 1);
	}
	return 0;
}
int Dvd::checkFile(char* path)
{
	static DVDFileInfo finfo;
	if (!openDvd(path, &finfo))
		return 0;

	int result = finfo.length;
	DVDClose(&finfo);
	return result;
}
int Dvd::loadFile(char* path, void* buffer)
{
	static DVDFileInfo finfo;

	if (openDvd(path, &finfo) == 0)
		return 0;

	s32 ret = finfo.length;
	while (true) {
		if (DVDReadPrio(&finfo, buffer, ret, 0, 2) != -1)
			break;

		if (errorCallback == nullptr)
			break;

		errorCallback(path, (u8*)buffer);
	}
	DVDClose(&finfo);

	return ret;
}
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

static void* Dvd::getCallStack()
{
	BOOL enable = OSDisableInterrupts();

	void* cs = &callStackArray[curQ++];
	if (curQ == 0x20)
		curQ = 0;

	OSRestoreInterrupts(enable);
	return cs;
}

static int Dvd::dvdReadMutex(DVDFileInfo* fileInfo, void* buffer, s32 size,
                             s32 offset, char* path)
{
	if (dvdThreadPauseFlag == 1)
		OSSleepThread(&dvdtSleep);

	s32 ret;
	while (true) {
		ret = DVDReadPrio(fileInfo, buffer, size, offset, 2);
		if (ret != -1)
			break;

		if (errorCallback == nullptr)
			break;

		errorCallback(path, (u8*)buffer);
	}
	return ret;
}

static void Dvd::doError(TDvdCall* call, u32 param)
{
	if (call->unk30 != nullptr)
		*call->unk30 = -1;

	if (call->unk34)
		call->unk34(-1);
}

static void Dvd::doFinish(TDvdCall* call, u32 param)
{
	if (call->unk30 != nullptr)
		*call->unk30 = param;

	if (call->unk34)
		call->unk34(call->unk0);
}

static bool Dvd::dvdThreadCheckBack(void* param)
{
	TDvdCall* call = (TDvdCall*)param;
	u32 thing      = call->unk0;
	if (call->unk30 != nullptr)
		*call->unk30 = thing;

	if (call->unk34)
		call->unk34(call->unk0);

	return false;
}

static void Dvd::allocDvdBuffer()
{
	if (buffersize != 0)
		return;

	for (u32 i = 0; i < buffers; ++i)
		audioDvdBuffer[i] = 0;
}

static void Dvd::writeBufferSize(u8* buffer, u32 buffer_count, u32 buffer_size)
{
	buffersize = buffer_size;
	buffers    = buffer_count;
	for (u32 i = 0; i < buffer_count; ++i) {
		audioDvdBuffer[i] = buffer;
		buffer += buffer_size;
	}
}
static void Dvd::updateBuffer()
{
	if (nextBuffers == 0)
		return;

	u8* nextBufTop = nextBufferTop;

	buffers    = nextBuffers;
	buffersize = nextBufferSize;

	for (u32 i = 0; i < nextBuffers; ++i) {
		audioDvdBuffer[i] = nextBufTop;
		nextBufTop += nextBufferSize;
	}

	nextBuffers   = 0;
	nextBufferTop = 0;
}

static void Dvd::aramDmaFinish(u32 param) { --bufferFull; }
static void Dvd::aramDmaFinish2(u32 param) { }

} // namespace JASystem
