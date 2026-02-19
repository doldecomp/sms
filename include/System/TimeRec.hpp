#ifndef SYSTEM_TIME_REC_HPP
#define SYSTEM_TIME_REC_HPP

#include <System/DrawSyncCallback.hpp>
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

		union {
			u8 asAry[4];
			u32 asUint;
		} color;
		color.asAry[0] = r;
		color.asAry[1] = g;
		color.asAry[2] = b;
		color.asAry[3] = a;
		u32 col        = color.asUint;

		if (!inst)
			return;
		OSTick tick = OSGetTick();
		inst->crTimeAry()[0].append(tick, col);
	}

	static void startTimer(u32 param_1)
	{
		TTimeRec* inst = _instance;

		// TODO: there must be some kind of a trick to unify
		// this with the overload above that does 4 separate byte
		// writes....
		volatile u32 tmp = param_1;
		u32 col          = tmp;
		if (!inst)
			return;
		OSTick tick = OSGetTick();
		inst->crTimeAry()[0].append(tick, param_1);
	}

	static void startTimerTwice(u32 tick, u32 param_1)
	{
		TTimeRec* inst = _instance;
		if (!inst)
			return;
		inst->crTimeAry()[0].append(tick, param_1);
		inst->crTimeAry()[1].append(tick, param_1);
	}

	static void endTimer()
	{
		TTimeRec* inst = _instance;
		if (!inst)
			return;
		OSTick tick = OSGetTick();
		inst->crTimeAry()[0].append(tick, 0);
	}

	static TTimeRec* instance() { return _instance; }

public:
	/* 0x4 */ TTimeArray unk4[2][2];
	/* 0x814 */ u32 unk814;
	/* 0x818 */ u8 unk818;
	/* 0x81A */ u16 unk81A;
	/* 0x81C */ u16 unk81C;
};

#endif
