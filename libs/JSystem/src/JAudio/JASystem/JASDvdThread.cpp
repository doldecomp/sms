#include <string.h>
#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/os.h>
#include <dolphin/dvd.h>
#include <dolphin/arq.h>
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

	typedef u8 TaskStorage[0x40];
	static TaskStorage* callStackArray = 0;
	static void** msgBuf               = 0;
	static u32 curQ                    = 0;

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
	static s32 dvdThreadCheckBack(void*);
	static void allocDvdBuffer();
	static void writeBufferSize(u8*, u32, u32);
	static void updateBuffer();
	static void aramDmaFinish(uintptr_t);
	static void aramDmaFinish2(uintptr_t);
} // namespace Dvd

void Dvd::init()
{
	callStackArray = new (JASDram, 0) TaskStorage[dvdMsgsSize];
	msgBuf         = new (JASDram, 0) void*[dvdMsgsSize];
}

void Dvd::setNumOfMsgs(s32 num) { dvdMsgsSize = num; }
void Dvd::setDvdBufSize(u32 size) { dvdBufSize = size; }
void Dvd::setRootPath(char*) { }
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
s32 Dvd::addTaskHigh(s32 (*func)(void*), void* data, u32 size)
{
	if (!mqInit)
		return 0;

	void* cs = getCallStack();
	Calc::bcopy(data, (void**)cs + 1, size);
	*(void**)cs = (void*)func;
	OSJamMessage(&mq, cs, 1);
	return 1;
}
s32 Dvd::addTask(s32 (*func)(void*), void* data, u32 size)
{
	if (!mqInit)
		return 0;

	void* cs = getCallStack();
	Calc::bcopy(data, (void**)cs + 1, size);
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
void* Dvd::dvdProc(void*)
{
	void* cs;
	u8* buf;

	allocDvdBuffer();
	buf = (u8*)Kernel::allocFromSysDram(dvdBufSize * 2);
	OSInitThreadQueue(&dvdtSleep);
	while (true) {
		OSMessage message;
		OSReceiveMessage(&mq, &message, 1);
		cs = message;
		updateBuffer();
		if (buffersize == 0)
			writeBufferSize(buf, 2, dvdBufSize);

		if (cs == 0)
			continue;

		(*(s32(**)(void*))cs)(((u8*)cs) + 4);
	}
}

s32 Dvd::loadToDramDvdTMain(void* task_data)
{
	s32 ret;
	TDvdCall* call = (TDvdCall*)task_data;
	DVDFileInfo finfo;
	char path[64];

	extendPath(path, call->mPath);

	if (!openDvd(path, &finfo)) {
		doError(call, 0);
		return -1;
	}

	if (finfo.length == 0) {
		doError(call, 1);
		return -1;
	}

	if (call->mTransferSize == 0) {
		call->mTransferSize = finfo.length;
		call->mTransferSize -= call->mOffsetOrAramAddress;
	}

	DCInvalidateRange(call->mDramBuffer, call->mTransferSize);
	ret = dvdReadMutex(&finfo, call->mDramBuffer, call->mTransferSize,
	                   call->mOffsetOrAramAddress, call->mPath);
	DVDClose(&finfo);
	doFinish(call, ret);
	return 0;
}

s32 Dvd::loadToDramDvdT(u32 user_data, char* path, void* buffer, u32 size,
                        u32 param5, u32* result_ptr, DvdCallback callback)
{
	TDvdCall callData;
	TDvdCall* call = &callData;

	call->mCbUserData = user_data;
	strcpy(call->mPath, path);
	call->mDramBuffer = buffer;

	call->mResult = result_ptr;
	if (result_ptr != 0)
		*result_ptr = 0;

	call->mCallback            = callback;
	call->mOffsetOrAramAddress = size;
	call->mTransferSize        = param5;

	addTask(&loadToDramDvdTMain, call, sizeof(TDvdCall));

	return 0;
}

void Dvd::setBufferDvdT(u8*, u32, u32) { }
void Dvd::closeBufferDvdT(u8*) { }
void Dvd::getCurrentBufferDvdT(u8**) { }

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

	extendPath(path, call->mPath);

	if (!openDvd(path, &finfo)) {
		doError(call, 0);
		return -1;
	}

	ret = finfo.length;

	if (finfo.length == 0) {
		doError(call, 1);
		return -1;
	}

	if (call->mTransferSize == 0) {
		call->mTransferSize = ret;
		call->mTransferSize -= call->mOffsetOrAramAddress;
	}

	OSTick time = OSGetTick();
	while (call->mTransferSize) {
		buf        = audioDvdBuffer[bufferLoad];
		bufferLoad = (bufferLoad + 1) % buffers;

		while (bufferFull == buffers)
			;

		if (call->mTransferSize < buffersize) {
			batchSize           = ALIGN_NEXT(call->mTransferSize, 32);
			ret                 = dvdReadMutex(&finfo, buf, batchSize,
			                                   call->mOffsetOrAramAddress, call->mPath);
			call->mTransferSize = 0;
		} else {
			batchSize = buffersize;
			ret       = dvdReadMutex(&finfo, buf, batchSize,
			                         call->mOffsetOrAramAddress, call->mPath);
			call->mOffsetOrAramAddress += buffersize;
			call->mTransferSize -= buffersize;
		}
		ARQPostRequest(&req[arq_index], 0x12345678, 0, 1, (uintptr_t)buf,
		               (uintptr_t)call->mDramBuffer, batchSize, &aramDmaFinish);
		++bufferFull;
		++arq_index;
		arq_index %= 4;
		call->mDramBuffer = (u8*)call->mDramBuffer + batchSize;
	}
	time -= DVDClose(&finfo);
	while (bufferFull != 0)
		;
	OSGetTick();
	doFinish(call, ret);

	return 0;
}
s32 Dvd::loadToAramDvdT(u32 user_data, char* path, void* buffer, u32 offset,
                        u32 size, u32* result_ptr, DvdCallback callback)
{
	TDvdCall callData;
	TDvdCall* call = &callData;

	call->mCbUserData = user_data;
	strcpy(call->mPath, path);
	call->mDramBuffer = buffer;

	call->mResult = result_ptr;
	if (result_ptr != 0)
		*result_ptr = 0;

	call->mCallback            = callback;
	call->mOffsetOrAramAddress = offset;
	call->mTransferSize        = size;

	addTask(&loadToAramDvdTMain, call, sizeof(TDvdCall));

	return 0;
}

s32 Dvd::aramToDramDvdTMain(void* task_data)
{

	static DVDFileInfo finfo;
	static ARQRequest req;
	TDvdCall* call = (TDvdCall*)task_data;

	++bufferFull2;
	ARQPostRequest(&req, (u32)(uintptr_t)call, ARQ_TYPE_ARAM_TO_MRAM,
	               ARQ_PRIORITY_HIGH, call->mOffsetOrAramAddress,
	               (uintptr_t)call->mDramBuffer, call->mTransferSize,
	               &aramDmaFinish2);
	while (bufferFull2 != 0)
		;

	doFinish(call, call->mTransferSize);
	return 0;
}
s32 Dvd::dramToAramDvdTMain(void* task_data)
{
	static DVDFileInfo finfo;
	static ARQRequest req;
	TDvdCall* call = (TDvdCall*)task_data;

	++bufferFull2;
	ARQPostRequest(&req, (u32)(uintptr_t)call, ARQ_TYPE_MRAM_TO_ARAM,
	               ARQ_PRIORITY_HIGH, (uintptr_t)call->mDramBuffer,
	               call->mOffsetOrAramAddress, call->mTransferSize,
	               &aramDmaFinish2);
	while (bufferFull2 != 0)
		;

	doFinish(call, call->mTransferSize);
	return 0;
}
s32 Dvd::aramToDramDvdT(u32 user_data, void* dst, void* src, u32 size,
                        u32* result_ptr, DvdCallback callback)
{
	TDvdCall callData;
	TDvdCall* call = &callData;

	call->mCbUserData = user_data;
	call->mDramBuffer = dst;

	call->mResult = result_ptr;
	if (result_ptr != 0)
		*result_ptr = 0;

	call->mCallback            = callback;
	call->mOffsetOrAramAddress = (uintptr_t)src;
	call->mTransferSize        = size;

	addTaskHigh(&aramToDramDvdTMain, call, sizeof(TDvdCall));

	return 0;
}
s32 Dvd::dramToAramDvdT(u32 user_data, void* src, void* dst, u32 size,
                        u32* result_ptr, DvdCallback callback)
{
	TDvdCall callData;
	TDvdCall* call = &callData;

	call->mCbUserData = user_data;
	call->mDramBuffer = src;

	call->mResult = result_ptr;
	if (result_ptr != 0)
		*result_ptr = 0;

	call->mCallback            = callback;
	call->mOffsetOrAramAddress = (uintptr_t)dst;
	call->mTransferSize        = size;

	addTaskHigh(&dramToAramDvdTMain, call, sizeof(TDvdCall));

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
s32 Dvd::checkPassDvdT(u32 user_data, u32* result_ptr, DvdCallback callback)
{
	TDvdCall callData;
	TDvdCall* call = &callData;

	callData.mCbUserData = user_data;
	callData.mResult     = result_ptr;
	callData.mCallback   = callback;

	addTask(&dvdThreadCheckBack, call, sizeof(TDvdCall));
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
void Dvd::setFastOpenMaxDic(s32) { }
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

static void Dvd::doError(TDvdCall* call, u32)
{
	if (call->mResult != nullptr)
		*call->mResult = -1;

	if (call->mCallback)
		call->mCallback(-1);
}

static void Dvd::doFinish(TDvdCall* call, u32 result)
{
	if (call->mResult != nullptr)
		*call->mResult = result;

	if (call->mCallback)
		call->mCallback(call->mCbUserData);
}

static s32 Dvd::dvdThreadCheckBack(void* param)
{
	TDvdCall* call = (TDvdCall*)param;
	doFinish(call, call->mCbUserData);

	return false;
}

static void Dvd::allocDvdBuffer()
{
	if (buffersize != 0)
		return;

	for (u32 i = 0; i < buffers; ++i)
		audioDvdBuffer[i] = nullptr;
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

static void Dvd::aramDmaFinish(uintptr_t) { --bufferFull; }
static void Dvd::aramDmaFinish2(uintptr_t) { --bufferFull2; }

} // namespace JASystem
