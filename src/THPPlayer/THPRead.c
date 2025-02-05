#include <THPPlayer/THPRead.h>
#include <THPPlayer/THPPlayer.h>

#define STACK_SIZE        4096

static OSMessageQueue FreeReadBufferQueue;
static OSMessageQueue ReadedBufferQueue;
static OSMessageQueue ReadedBufferQueue2;

static OSMessage FreeReadBufferMessage[THP_READ_BUFFER_COUNT];
static OSMessage ReadedBufferMessage[THP_READ_BUFFER_COUNT];
static OSMessage ReadedBufferMessage2[THP_READ_BUFFER_COUNT];

static OSThread ReadThread;
static u8 ReadThreadStack[STACK_SIZE];

static void* Reader(void* arg);

static BOOL ReadThreadCreated;

BOOL CreateReadThread(OSPriority prio)
{
	if (OSCreateThread(&ReadThread, Reader, NULL, ReadThreadStack + STACK_SIZE,
	                   STACK_SIZE, prio, 1)
	    == FALSE)
		return FALSE;

	OSInitMessageQueue(&FreeReadBufferQueue, FreeReadBufferMessage,
	                   THP_READ_BUFFER_COUNT);
	OSInitMessageQueue(&ReadedBufferQueue, ReadedBufferMessage,
	                   THP_READ_BUFFER_COUNT);
	OSInitMessageQueue(&ReadedBufferQueue2, ReadedBufferMessage2,
	                   THP_READ_BUFFER_COUNT);
	ReadThreadCreated = TRUE;

	return TRUE;
}

void ReadThreadStart(void)
{
	if (ReadThreadCreated)
		OSResumeThread(&ReadThread);
}

void ReadThreadCancel(void)
{
	if (ReadThreadCreated) {
		OSCancelThread(&ReadThread);
		ReadThreadCreated = FALSE;
	}
}

static void* Reader(void* arg)
{
	THPReadBuffer* buf;
	s32 curFrame;
	s32 status;
	s32 offset       = ActivePlayer.initOffset;
	s32 initReadSize = ActivePlayer.initReadSize;
	s32 frame        = 0;

	while (TRUE) {
		buf    = (THPReadBuffer*)PopFreeReadBuffer();
		status = DVDReadPrio(&ActivePlayer.fileInfo, buf->ptr, initReadSize,
		                     offset, 2);
		if (status != initReadSize) {
			if (status == -1)
				ActivePlayer.dvdError = -1;
			if (frame == 0)
				PrepareReady(FALSE);

			OSSuspendThread(&ReadThread);
		}

		buf->frameNumber = frame;
		PushReadedBuffer(buf);
		offset += initReadSize;
		initReadSize = *(s32*)buf->ptr;

		curFrame = (frame + ActivePlayer.initReadFrame)
		           % ActivePlayer.header.numFrames;
		if (curFrame == ActivePlayer.header.numFrames - 1) {
			if ((ActivePlayer.playFlag & 1))
				offset = ActivePlayer.header.movieDataOffsets;
			else
				OSSuspendThread(&ReadThread);
		}

		frame++;
	}
}

void* PopReadedBuffer(void)
{
	void* buffer;
	OSReceiveMessage(&ReadedBufferQueue, &buffer, OS_MESSAGE_BLOCK);
	return buffer;
}

void* PushReadedBuffer(void* buffer)
{
	OSSendMessage(&ReadedBufferQueue, buffer, 1);
}

void* PopFreeReadBuffer(void)
{
	void* buffer;
	OSReceiveMessage(&FreeReadBufferQueue, &buffer, OS_MESSAGE_BLOCK);
	return buffer;
}

void PushFreeReadBuffer(void* buffer)
{
	OSSendMessage(&FreeReadBufferQueue, buffer, 1);
}

void* PopReadedBuffer2(void)
{
	void* buffer;
	OSReceiveMessage(&ReadedBufferQueue2, &buffer, OS_MESSAGE_BLOCK);
	return buffer;
}

void PushReadedBuffer2(void* buffer)
{
	OSSendMessage(&ReadedBufferQueue2, buffer, 1);
}
