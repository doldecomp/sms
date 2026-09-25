#include <THPPlayer/THPVideoDecode.h>

#include <dolphin/thp.h>
#include <THPPlayer/THPDraw.h>
#include <THPPlayer/THPRead.h>
#include <THPPlayer/THPPlayer.h>

#pragma opt_strength_reduction off

#define STACK_SIZE 4096

static OSThread VideoDecodeThread;
static u8 VideoDecodeThreadStack[STACK_SIZE];
static OSMessageQueue FreeTextureSetQueue;
static OSMessageQueue DecodedTextureSetQueue;
static OSMessage FreeTextureSetMessage[THP_AUDIO_BUFFER_COUNT];
static OSMessage DecodedTextureSetMessage[THP_AUDIO_BUFFER_COUNT];

static BOOL VideoDecodeThreadCreated;
static BOOL First;

static void* VideoDecoderForOnMemory(void* arg);
static void* VideoDecoder(void* arg);
static void VideoDecode(THPReadBuffer* readBuffer);

BOOL CreateVideoDecodeThread(OSPriority prio, void* arg)
{
	BOOL res;
	if (arg) {
		res = OSCreateThread(&VideoDecodeThread, VideoDecoderForOnMemory, arg,
		                     VideoDecodeThreadStack + STACK_SIZE, STACK_SIZE,
		                     prio, 1);
		if (res == FALSE)
			return FALSE;
	} else {
		res = OSCreateThread(&VideoDecodeThread, VideoDecoder, NULL,
		                     VideoDecodeThreadStack + STACK_SIZE, STACK_SIZE,
		                     prio, 1);
		if (res == FALSE)
			return FALSE;
	}

	OSInitMessageQueue(&FreeTextureSetQueue, FreeTextureSetMessage,
	                   THP_AUDIO_BUFFER_COUNT);
	OSInitMessageQueue(&DecodedTextureSetQueue, DecodedTextureSetMessage,
	                   THP_AUDIO_BUFFER_COUNT);
	VideoDecodeThreadCreated = TRUE;
	First                    = TRUE;
	return TRUE;
}

void VideoDecodeThreadStart(void)
{
	if (VideoDecodeThreadCreated)
		OSResumeThread(&VideoDecodeThread);
}

void VideoDecodeThreadCancel(void)
{
	if (VideoDecodeThreadCreated) {
		OSCancelThread(&VideoDecodeThread);
		VideoDecodeThreadCreated = FALSE;
	}
}

void* VideoDecoder(void* arg)
{
	THPReadBuffer* thpBuffer;

	while (TRUE) {
		if (ActivePlayer.audioExist) {
			for (; ActivePlayer.videoDecodeCount < 0;) {
				thpBuffer = (THPReadBuffer*)PopReadedBuffer2();
				s32 remaining
				    = ((thpBuffer->frameNumber + ActivePlayer.initReadFrame)
				       % ActivePlayer.header.numFrames);
				if (remaining == (ActivePlayer.header.numFrames - 1)
				    && (ActivePlayer.playFlag & 1) == 0)
					VideoDecode(thpBuffer);

				PushFreeReadBuffer(thpBuffer);
				BOOL enable = OSDisableInterrupts();
				ActivePlayer.videoDecodeCount++;
				OSRestoreInterrupts(enable);
			}
		}

		if (ActivePlayer.audioExist)
			thpBuffer = (THPReadBuffer*)PopReadedBuffer2();
		else
			thpBuffer = (THPReadBuffer*)PopReadedBuffer();

		VideoDecode(thpBuffer);
		PushFreeReadBuffer(thpBuffer);
	}
}

void* VideoDecoderForOnMemory(void* arg)
{
	THPReadBuffer readBuffer;
	s32 readSize;
	s32 frame;
	s32 remaining;

	readSize       = ActivePlayer.initReadSize;
	frame          = 0;
	readBuffer.ptr = (u8*)arg;

	while (TRUE) {
		if (ActivePlayer.audioExist) {
			while (ActivePlayer.videoDecodeCount < 0) {
				BOOL enable = OSDisableInterrupts();
				ActivePlayer.videoDecodeCount++;
				OSRestoreInterrupts(enable);
				remaining = (frame + ActivePlayer.initReadFrame)
				            % ActivePlayer.header.numFrames;
				if (remaining == ActivePlayer.header.numFrames - 1) {
					if ((ActivePlayer.playFlag & 1) == 0)
						break;

					readSize       = *(s32*)readBuffer.ptr;
					readBuffer.ptr = ActivePlayer.movieData;
				} else {
					s32 size = *(s32*)readBuffer.ptr;
					readBuffer.ptr += readSize;
					readSize = size;
				}
				frame++;
			}
		}

		readBuffer.frameNumber = frame;

		VideoDecode(&readBuffer);

		remaining = (frame + ActivePlayer.initReadFrame)
		            % ActivePlayer.header.numFrames;
		if (remaining == ActivePlayer.header.numFrames - 1) {
			if ((ActivePlayer.playFlag & 1)) {
				readSize       = *(s32*)readBuffer.ptr;
				readBuffer.ptr = ActivePlayer.movieData;
			} else {
				OSSuspendThread(&VideoDecodeThread);
			}
		} else {
			s32 size = *(s32*)readBuffer.ptr;
			readBuffer.ptr += readSize;
			readSize = size;
		}

		frame++;
	}
}

void VideoDecode(THPReadBuffer* readBuffer)
{
	THPTextureSet* textureSet;
	s32 i;
	u32* tileOffsets;
	u8* tile;

	tileOffsets = (u32*)(readBuffer->ptr + 8);
	tile        = &readBuffer->ptr[ActivePlayer.compInfo.numComponents * 4] + 8;
	textureSet  = (THPTextureSet*)PopFreeTextureSet();

	for (i = 0; i < ActivePlayer.compInfo.numComponents; i++) {
		switch (ActivePlayer.compInfo.frameComp[i]) {
		case 0: {
			if ((ActivePlayer.videoError = THPVideoDecode(
			         tile, textureSet->ytexture, textureSet->utexture,
			         textureSet->vtexture, ActivePlayer.thpWork))) {
				if (First) {
					PrepareReady(FALSE);
					First = FALSE;
				}
				OSSuspendThread(&VideoDecodeThread);
			}
			textureSet->frameNumber = readBuffer->frameNumber;
			PushDecodedTextureSet(textureSet);
			BOOL enable = OSDisableInterrupts();
			ActivePlayer.videoDecodeCount++;
			OSRestoreInterrupts(enable);
		}
		}

		tile += *tileOffsets;
		tileOffsets++;
	}

	if (First) {
		PrepareReady(TRUE);
		First = FALSE;
	}
}

void* PopFreeTextureSet(void)
{
	void* tex;
	OSReceiveMessage(&FreeTextureSetQueue, &tex, 1);
	return tex;
}

void PushFreeTextureSet(void* tex)
{
	OSSendMessage(&FreeTextureSetQueue, tex, 0);
}

void* PopDecodedTextureSet(s32 flags)
{
	void* tex;
	if (OSReceiveMessage(&DecodedTextureSetQueue, &tex, flags) == TRUE) {
		return tex;
	}
	return NULL;
}

void PushDecodedTextureSet(void* tex)
{
	OSSendMessage(&DecodedTextureSetQueue, tex, OS_MESSAGE_BLOCK);
}
