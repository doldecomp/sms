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
	static volatile u32 bufferFull2;

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
	static s32 dvdReadMutex(DVDFileInfo*, void*, s32, s32, char*);
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

void Dvd::setNumOfMsgs(s32 num) { dvdMsgsSize = num; }
void Dvd::setDvdBufSize(u32 size) { dvdBufSize = size; }
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
s32 Dvd::addTaskHigh(s32 (*func)(void*), void* param, u32 size)
{
	if (!mqInit)
		return 0;

	void* cs = getCallStack();
	Calc::bcopy(param, (void**)cs + 1, size);
	*(void**)cs = (void*)func;
	OSJamMessage(&mq, cs, 1);
	return 1;
}
s32 Dvd::addTask(s32 (*func)(void*), void* param, u32 size)
{
	if (!mqInit)
		return 0;

	void* cs = getCallStack();
	Calc::bcopy(param, (void**)cs + 1, size);
	*(void**)cs = (void*)func;
	OSSendMessage(&mq, cs, 1);
	return 1;
}
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

		(*(s32(**)(void*))cs)(((u8*)cs) + 4);
	}
}

s32 Dvd::loadToDramDvdTMain(void* param)
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

s32 Dvd::loadToDramDvdT(u32 param1, char* path, void* buffer, u32 size,
                        u32 param5, u32* param6, DvdCallback callback)
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

	addTask(&loadToDramDvdTMain, call, 0x38);

	return 0;
}

void Dvd::setBufferDvdT(u8* buffer, u32 size, u32 param3) { }
void Dvd::closeBufferDvdT(u8* buffer) { }
void Dvd::getCurrentBufferDvdT(u8** buffer) { }

s32 Dvd::loadToAramDvdTMain(void* param)
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
s32 Dvd::loadToAramDvdT(u32 param1, char* path, void* buffer, u32 size,
                        u32 param5, u32* param6, DvdCallback callback)
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

	addTask(&loadToAramDvdTMain, call, 0x38);

	return 0;
}

s32 Dvd::aramToDramDvdTMain(void* param)
{

	static DVDFileInfo finfo;
	static ARQRequest req;
	TDvdCall* call = (TDvdCall*)param;

	++bufferFull2;
	ARQPostRequest(&req, (u32)call, ARQ_TYPE_ARAM_TO_MRAM, ARQ_PRIORITY_HIGH,
	               call->unk28, (u32)call->unk24, call->unk2C, &aramDmaFinish2);
	while (bufferFull2 != 0)
		;

	doFinish(call, call->unk2C);
	return 0;
}
s32 Dvd::dramToAramDvdTMain(void* param)
{
	static DVDFileInfo finfo;
	static ARQRequest req;
	TDvdCall* call = (TDvdCall*)param;

	++bufferFull2;
	ARQPostRequest(&req, (u32)call, ARQ_TYPE_MRAM_TO_ARAM, ARQ_PRIORITY_HIGH,
	               (u32)call->unk24, call->unk28, call->unk2C, &aramDmaFinish2);
	while (bufferFull2 != 0)
		;

	doFinish(call, call->unk2C);
	return 0;
}
s32 Dvd::aramToDramDvdT(u32 param1, void* dest, void* src, u32 size,
                        u32* param5, DvdCallback callback)
{
	TDvdCall callData;
	TDvdCall* call = &callData;

	call->unk0  = param1;
	call->unk24 = dest;

	call->unk30 = param5;
	if (param5 != 0)
		*param5 = 0;

	call->unk34 = callback;
	call->unk28 = (u32)src;
	call->unk2C = size;

	addTaskHigh(&aramToDramDvdTMain, call, 0x38);

	return 0;
}
s32 Dvd::dramToAramDvdT(u32 param1, void* dest, void* src, u32 size,
                        u32* param5, DvdCallback callback)
{
	TDvdCall callData;
	TDvdCall* call = &callData;

	call->unk0  = param1;
	call->unk24 = dest;

	call->unk30 = param5;
	if (param5 != 0)
		*param5 = 0;

	call->unk34 = callback;
	call->unk28 = (u32)src;
	call->unk2C = size;

	addTaskHigh(&dramToAramDvdTMain, call, 0x38);

	return 0;
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
	volatile u32 done = 0;
	loadToDramDvdT(0, path, buffer, 0, 0, (u32*)&done, nullptr);

	while (!done)
		;

	if (done == 0xffffffff)
		return 0;
	else
		return done;
}
s32 Dvd::checkPassDvdT(u32 param1, u32* param2, void (*callback)(u32))
{
	TDvdCall callData;
	TDvdCall* call = &callData;

	callData.unk0  = param1;
	callData.unk30 = param2;
	callData.unk34 = callback;

	addTask((s32 (*)(void*))&dvdThreadCheckBack, call, 0x38);
	return 0;
}
s32 Dvd::checkFile(char* path)
{
	static DVDFileInfo finfo;
	if (!openDvd(path, &finfo))
		return 0;

	s32 result = finfo.length;
	DVDClose(&finfo);
	return result;
}
s32 Dvd::loadFile(char* path, void* buffer)
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

void Dvd::registerDvdErrorCallback(ErrorCallback callback)
{
	errorCallback = callback;
}
void Dvd::setFastOpenMaxDic(s32 max) { }
void Dvd::registerExtFastOpen(char* path)
{
	char realPath[64];
	extendPath(realPath, path);
	registerFastOpen(realPath);
}
s32 Dvd::registerFastOpen(char* path)
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

static s32 Dvd::dvdReadMutex(DVDFileInfo* fileInfo, void* buffer, s32 size,
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
	if (nextBuffers != 0) {
		writeBufferSize(nextBufferTop, nextBuffers, nextBufferSize);
		nextBuffers   = 0;
		nextBufferTop = 0;
	}
}

static void Dvd::aramDmaFinish(u32 param) { --bufferFull; }
static void Dvd::aramDmaFinish2(u32 param) { --bufferFull2; }

} // namespace JASystem
