#ifndef SYSTEM_TIME_REC_HPP
#define SYSTEM_TIME_REC_HPP

#include <System/DrawSyncCallback.hpp>
#include <JSystem/JUtility/JUTColor.hpp>
#include <dolphin/os.h>
#include <dolphin/types.h>

class TTimeArray {
public:
	// fabricated
	struct Entry {
		u32 time;
		// This being a color is a wild guess, but would make sense for a
		// instrumented profiler which this seems to be
		u32 color;
	};

	enum { MAX_SIZE = 64 };

	TTimeArray()
	    : mSize(0)
	{
	}
	bool append(u32 time, u32 color);

	// fabricated
	int size() const { return mSize; }
	Entry& operator[](int i) { return mEntries[i]; }

public:
	/* 0x0 */ Entry mEntries[MAX_SIZE];
	/* 0x200 */ int mSize;
};

class TTimeRec : public TDrawSyncCallback {
public:
	static TTimeRec* start(u16);
	void end();

	void drawSyncCallbackSt(u16);

	TTimeRec(u16);
	void flip();
	void snapGXTime(u32);
	void suppleGXTime();
	virtual void drawSyncCallback(u16);

	static TTimeRec* _instance;

	TTimeArray* crTimeAry() { return unk4[unk814]; }

	// fabricated
	static void snapGxTimeStart(u8 r, u8 g, u8 b, u8 a)
	{
		union {
			u8 asAry[4];
			u32 asUint;
		} color;
		color.asAry[0] = r;
		color.asAry[1] = g;
		color.asAry[2] = b;
		color.asAry[3] = a;
		snapGxTimeStatic(color.asUint);
	}

	// fabricated
	static void snapGxTimeEnd() { snapGxTimeStatic(0); }

	static void snapGxTimeStatic(u32 param_1)
	{
		if (!_instance)
			return;
		_instance->snapGXTime(param_1);
	}

	static void startTimer(u8 r = 0xff, u8 g = 0xff, u8 b = 0xff, u8 a = 0xff)
	{
		TTimeRec* inst = _instance;
		JUtility::TColor color(r, g, b, a);
		u32 col = color;

		if (!inst)
			return;
		OSTick tick          = OSGetTick();
		TTimeArray* timeArray = inst->crTimeAry();
		timeArray->append(tick, col);
	}

	// Only TSnapTimeObj::perform reaches this overload, and there it is one
	// stack word short: retail puts the reloaded colour at 0x38(r1), we put it
	// at 0x34, inside an identically sized 0x50 frame and with identical
	// instructions. Header round 11 mapped the frame instead of guessing:
	// locals start at 0x28 (0x8..0x28 is MWCC's fixed 32-byte outgoing
	// parameter area), the inlined `endTimer()` owns 0x28..0x34, and the
	// colour is the next temporary. Both inlined `snapGxTimeStatic` calls
	// contribute nothing (dropping either leaves 0x50/0x34). The region is
	// 28 bytes and the colour's offset inside it is what moves, so the fix is
	// +4 at constant region -- but every lever measured either moves the
	// colour and the region together or neither:
	//
	//   named `TTimeArray* timeArray` (any spelling: pointer, reference,
	//     `&crTimeAry()[0]`, hoisted above the null test)   0x58 / 0x38
	//   `TColor color2(color)` copy                        0x58 / 0x38
	//   named `u32` in `snapGxTimeStatic`                   0x58 / 0x38
	//   `TTimeArray::Entry` temporary                       0x68 / --
	//   `TColor color; color.set(p);`                      0x50 / 0x30
	//   `_instance` instead of `instance()`                 0x50 / 0x34 (-8/-4
	//     on top of the named timeArray, so the two cancel)
	//   unused leading `f64`, `toUInt32()`, `instance()`
	//     twice, declaration order, an extra level in
	//     `snapGxTimeStatic`                                0x50 / 0x34
	//
	// Nothing measured is worth (-8, 0) or (0, +4). 0x38 is the first
	// 8-byte-aligned slot above `endTimer`'s block, so retail's temporary is
	// probably 8-byte aligned (the catalog's "one 8-byte object" family), but
	// no 8-byte object in this body reproduces it.
	//
	// Header round 18 measured a second batch of levers on the same two
	// numbers (frame / colour slot; the target is 0x50 / 0x38 at 57
	// instructions), and all of them move the pair together:
	//
	//   dead non-trivial 4-byte local, first in this body   0x58 / 0x38
	//   the same local 8 bytes wide                         0x58 / 0x3c
	//   `JUtility::TColor` as a by-value *parameter* of
	//     this overload (the caller's u32 conversion)        +5 instructions
	//   the colour built one level deeper (an inlined
	//     `makeColorBits(u32)` returning the bits)          0x58 / 0x30
	//   an unnamed `JUtility::TColor(param_1)` temporary     0x58 / 0x40
	//   a `TTimeArray::Entry` local with the colour in its
	//     second word (trivial POD: scalar-replaced, the
	//     store/reload disappears entirely)                 0x58 / --
	//   dropping the named `col`                            store moves below
	//                                                       the null test
	//   an unnamed `OSGetTick()` at the `append` call        +5 instructions
	//   raw `inst->unk4[inst->unk814][0]` for `crTimeAry()`  0x48 / 0x2c, +1
	//   a named `inst` inside `snapGxTimeStatic` (+4 per
	//     expansion, and perform has two)                    0x58 / 0x3c
	//
	// Two results worth keeping:
	//
	// 1. An 8-byte dead non-trivial local here *plus* `_instance` instead of
	//    `instance()` lands both numbers exactly (0x50 / 0x38, 57
	//    instructions) and leaves only an r29/r30 swap: with `_instance` the
	//    caller's `cue` takes r30 and `inst` r29, the other way round from
	//    retail. So `instance()` is load-bearing for the *caller's* register
	//    assignment, which is why the (-8, -4) it is worth cannot be spent.
	// 2. The old conclusion that `endTimer` is untouchable is retired.
	//    Giving `endTimer` the named `timeArray` puts the colour at 0x38, and
	//    the damage to the two source-linked callers is *exactly* cancelled
	//    by removing the same named `timeArray` from the four-argument
	//    overload above -- TLiveManager::perform and TObjManager::perform
	//    stay byte-exact, because both call one `startTimer` and one
	//    `endTimer` and only the sum matters. What is left over is the +8 of
	//    frame, which the compensating pair does not touch. A reference
	//    (`TTimeArray& timeArray = inst->crTimeAry()[0]`) in `endTimer` is
	//    the same +8 and does *not* cancel (both callers 99.8%).
	//
	// So the remaining need is a pure (-8, 0) inside this overload, whose only
	// caller is TSnapTimeObj::perform. Left as it stands.
	static void startTimer(u32 param_1)
	{
		JUtility::TColor color(param_1);
		TTimeRec* inst = instance();
		u32 col = color;
		if (!inst)
			return;
		OSTick tick = OSGetTick();
		inst->crTimeAry()[0].append(tick, col);
	}

	static void startTimerTwice(u32 tick, u32 param_1)
	{
		TTimeRec* inst = _instance;
		if (!inst)
			return;
		inst->crTimeAry()[0].append(tick, param_1);
		inst->crTimeAry()[1].append(tick, param_1);
	}

	// fabricated name (no map symbol: every TTimeRec timer helper is fully
	// inlined). `endTimer()` forwarding to this shared body is the inline
	// level TSnapTimeObj::perform's parked `SnapTimeObjEndTimer` supplied,
	// promoted in header round 20: codegen-identical for the two source-linked
	// callers (TLiveManager::perform, TObjManager::perform) and for
	// TSnapTimeObj::perform itself, and it takes TEnemyManager::performShared
	// 94.82 -> 94.84. `startTimer(u32)` keeps its own copy of the body -- see
	// the trial table at its declaration.
	static void appendTime(u32 col)
	{
		TTimeRec* inst = instance();
		if (!inst)
			return;
		OSTick tick = OSGetTick();
		inst->crTimeAry()[0].append(tick, col);
	}

	static void endTimer() { appendTime(0); }

	static TTimeRec* instance() { return _instance; }

public:
	/* 0x4 */ TTimeArray unk4[2][2];
	/* 0x814 */ u32 unk814;
	/* 0x818 */ u8 unk818;
	/* 0x81A */ u16 unk81A;
	/* 0x81C */ u16 unk81C;
};

#endif
