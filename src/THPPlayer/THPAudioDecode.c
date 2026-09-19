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
// Closure batch 103 added: all declarations at the top of the body in the
// file's own C89 style (89.1, and any permutation of the five declarations only
// ever swaps frame with readSize: 89.3 or 86.2), every order of the three
// initialising statements (89.3 with readSize first, 84.5 with readBuffer.ptr
// before frame), and named `OSThread* thread`/`THPPlayer* player` locals
// initialised at the top in retail's register order (83.0 -- the ranking is not
// declaration order either: it comes out player > frame > thread). So the
// ranking is neither use count, nor declaration order, nor first-definition
// order. Retail interleaves a source local and the two global-address
// temporaries (frame > &AudioDecodeThread > &ActivePlayer > readSize) where we
// always put both temporaries above both locals; the same shape blocks
// TMapObjRevivalPollution::loadAfter, so this is one cross-unit question, not
// a spelling problem here.
// TODO: 89.3%, 46 instructions, every instruction exact; the whole residue is
// a callee-saved rotation. Retail ranks the source counter above the two
// global-address bases (frame r31, &AudioDecodeThread r30, &ActivePlayer r29,
// readSize r28); we rank the two bases above it (r31/r30 for the bases, r29
// for frame, r28 for readSize).
//
// Header round 18 found the mechanism: **a source local outranks a
// compiler-generated global-address temporary only if it is live before that
// temporary is materialised.** Spelling the first statement
// `readBuffer.frameNumber = frame = 0;` -- a use of `frame` ahead of the first
// ActivePlayer read -- reproduces retail's whole ranking exactly (89.3 ->
// 97.7%, all four registers right, `li r31, 0` in retail's slot); the only
// difference left is the pre-loop store that the probe itself adds, so the
// real source got `frame` live early some other way.
// Inert (all leave the rotation untouched): `s32 frame = 0;` at the
// declaration, `register s32 frame;`, a `for (frame = 0;; frame++)` induction
// variable, and all six permutations of the three initialising statements.
// Worse: declaring `frame` last, which drops it to r28 -- so declaration order
// ranks the source locals among themselves and never lifts the group above
// the temporaries. Hoisting the `readBuffer.frameNumber` store out of the loop
// (store before it, store after the increment) is +2 instructions.
// The same group swap is open in TMapObjRevivalPollution::loadAfter
// (src/MoveBG/MapObjPollution.cpp).
//
// Closure batch 115 measured the probe's cost exactly and looked for a
// zero-instruction use. With `readBuffer.frameNumber = frame = 0;` the diff is
// **one** instruction: an extra `stw r31, 0x14(r1)` emitted where the `frame =
// 0` statement sits, i.e. before the `lwz r28, 0xbc(r29)` readSize load; the
// other 44 instructions and the frame are exact. Retail has *no* pre-loop use
// of `frame` at all (its pre-loop block is `li r31, 0`, the two `addi` hoists,
// the readSize load and `stw r3, 0x10(r1)`), so the probe is a symptom, not
// the source shape.
// A use that folds away does not rank the local: `readSize =
// ActivePlayer.initReadSize + frame;` and `readBuffer.ptr = (u8*)arg + frame;`
// are both constant-folded to the baseline object (46 instructions, rotation
// unchanged), so MWCC's ranking is computed after folding.
// Position matters: moving `readBuffer.frameNumber = frame = 0;` *after* the
// ActivePlayer read is 85.8% and 47 instructions (the `li` then lands between
// the two address hoists), confirming that only a use ahead of the first
// ActivePlayer access lifts the local.
// Naming the two globals as source locals (`THPPlayer* player`, `OSThread*
// thread`) never reaches retail's order either: three declaration orders give
// 83.0% at 47 instructions and the three with `frame` declared after both give
// 90.8% at 45, but none is instruction-exact, so the address temporaries are
// genuinely compiler-generated in retail.
// Batch 151 tried batch 144's one new knob, the number of **named scalar
// locals** the frame holds: hoisting the two inner-block locals (`remaining`,
// `size`) to function scope, which takes the count from two to four, is worse
// (13 -> 19 markers, still 46 instructions) and does not move `frame` above the
// two address bases. With no third load-bearing scalar to add and no aggregate
// to group, the knob is exhausted here too; the site stays in the pool-vs-local
// group-swap class with `checkNextFrameSe` and `loadAfter`.
// Closure batch 211 tried research 210's inlined-call form, which is the only
// one available in a C TU: a `static inline s32 GetFrameInBuffer(THPPlayer*
// player, s32 frame)` wrapping the `(frame + initReadFrame) % numFrames`
// expression, so that `frame` and the `&ActivePlayer` base are the two
// parameters of one inlined call. Byte-for-byte inert (13 markers, 46
// instructions, same rotation): the helper folds away and MWCC regenerates the
// same global-address temporary, so a C helper cannot put a local into the
// parameter bucket the way an inlined member's argument does in C++.
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
