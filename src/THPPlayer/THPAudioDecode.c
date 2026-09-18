#include <THPPlayer/THPAudioDecode.h>

#include <THPPlayer/THPPlayer.h>
#include <THPPlayer/THPRead.h>
#include <THPPlayer/THPPlayer.h>
#include <dolphin/thp.h>
#include <dolphin/os.h>

#pragma opt_strength_reduction off

#define STACK_SIZE 4096

static OSThread AudioDecodeThread;
static u8 AudioDecodeThreadStack[STACK_SIZE];
static OSMessageQueue FreeAudioBufferQueue;
static OSMessageQueue DecodedAudioBufferQueue;
static OSMessage FreeAudioBufferMessage[THP_AUDIO_BUFFER_COUNT];
static OSMessage DecodedAudioBufferMessage[THP_AUDIO_BUFFER_COUNT];

static BOOL AudioDecodeThreadCreated;

static void* AudioDecoderForOnMemory(void* arg);
static void* AudioDecoder(void* arg);
static void AudioDecode(THPReadBuffer* readBuffer);

BOOL CreateAudioDecodeThread(OSPriority prio, void* arg)
{
	BOOL res;
	if (arg) {
		res = OSCreateThread(&AudioDecodeThread, AudioDecoderForOnMemory, arg,
		                     AudioDecodeThreadStack + STACK_SIZE, STACK_SIZE,
		                     prio, 1);
		if (res == FALSE)
			return FALSE;
	} else {
		res = OSCreateThread(&AudioDecodeThread, AudioDecoder, NULL,
		                     AudioDecodeThreadStack + STACK_SIZE, STACK_SIZE,
		                     prio, 1);
		if (res == FALSE)
			return FALSE;
	}

	OSInitMessageQueue(&FreeAudioBufferQueue, FreeAudioBufferMessage,
	                   THP_AUDIO_BUFFER_COUNT);
	OSInitMessageQueue(&DecodedAudioBufferQueue, DecodedAudioBufferMessage,
	                   THP_AUDIO_BUFFER_COUNT);
	AudioDecodeThreadCreated = TRUE;
	return TRUE;
}

void AudioDecodeThreadStart()
{
	if (AudioDecodeThreadCreated)
		OSResumeThread(&AudioDecodeThread);
}

void AudioDecodeThreadCancel()
{
	if (AudioDecodeThreadCreated) {
		OSCancelThread(&AudioDecodeThread);
		AudioDecodeThreadCreated = FALSE;
	}
}

static void* AudioDecoder(void* arg)
{
	THPReadBuffer* buf;
	while (TRUE) {
		buf = (THPReadBuffer*)PopReadedBuffer();
		AudioDecode(buf);
		PushReadedBuffer2(buf);
	}
}

// TODO: 89.3%. Every instruction is right; the residue is a pure rotation of
// the callee-saved set. Retail ranks frame (r31) > &AudioDecodeThread (r30) >
// &ActivePlayer (r29) > readSize (r28) and emits `li r31, 0` before the two
// address hoists; we rank &AudioDecodeThread (r31) > &ActivePlayer (r30) >
// frame (r29) > readSize (r28) and initialise frame after them. Measured, all
// no better: `s32 frame = 0` in the declaration (+0), `for (frame = 0;;
// frame++)` (+0), readBuffer declared first (+0), `remaining` declared at the
// top (+0), readSize declared before frame (86.2), the `remaining` expression
// written inline in the if (84.3), `frame = 0` moved after the other two
// initialisations (84.5), caching header.numFrames in a local (73.9), a named
// OSThread* in the suspend branch (82.6).
static void* AudioDecoderForOnMemory(void* arg)
{
	s32 frame;
	s32 readSize;
	THPReadBuffer readBuffer;

	frame          = 0;
	readSize       = ActivePlayer.initReadSize;
	readBuffer.ptr = (u8*)arg;

	while (TRUE) {
		readBuffer.frameNumber = frame;
		AudioDecode(&readBuffer);

		s32 remaining = (frame + ActivePlayer.initReadFrame)
		                % ActivePlayer.header.numFrames;

		if (remaining == ActivePlayer.header.numFrames - 1) {
			if ((ActivePlayer.playFlag & 1)) {
				readSize       = *(s32*)readBuffer.ptr;
				readBuffer.ptr = ActivePlayer.movieData;
			} else {
				OSSuspendThread(&AudioDecodeThread);
			}
		} else {
			s32 size = *(s32*)readBuffer.ptr;
			readBuffer.ptr += readSize;
			readSize = size;
		}
		frame++;
	}
}

#pragma dont_inline on
static void AudioDecode(THPReadBuffer* readBuffer)
{
	THPAudioBuffer* audioBuf;
	s32 i;
	u32* offsets;
	u8* audioData;

	offsets   = (u32*)(readBuffer->ptr + 8);
	audioData = &readBuffer->ptr[ActivePlayer.compInfo.numComponents * 4] + 8;
	audioBuf  = (THPAudioBuffer*)PopFreeAudioBuffer();

	for (i = 0; i < ActivePlayer.compInfo.numComponents; i++) {
		switch (ActivePlayer.compInfo.frameComp[i]) {
		case 1: {
			audioBuf->validSample = THPAudioDecode(
			    audioBuf->buffer,
			    (audioData + *offsets * ActivePlayer.curAudioTrack), 0);
			audioBuf->curPtr = audioBuf->buffer;
			PushDecodedAudioBuffer(audioBuf);
			return;
		}
		}

		audioData += *offsets;
		offsets++;
	}
}
#pragma dont_inline off

void* PopFreeAudioBuffer()
{
	void* buf;
	OSReceiveMessage(&FreeAudioBufferQueue, &buf, 1);
	return buf;
}

void PushFreeAudioBuffer(void* buf)
{
	OSSendMessage(&FreeAudioBufferQueue, buf, OS_MESSAGE_NOBLOCK);
}

void* PopDecodedAudioBuffer(s32 flags)
{
	void* buf;
	s32 res = OSReceiveMessage(&DecodedAudioBufferQueue, &buf, flags);
	if (res == 1) {
		return buf;
	}
	return NULL;
}

void PushDecodedAudioBuffer(void* buf)
{
	OSSendMessage(&DecodedAudioBufferQueue, buf, 1);
}
