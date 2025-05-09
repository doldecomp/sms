#ifndef SYSTEM_TIME_REC_HPP
#define SYSTEM_TIME_REC_HPP

#include <System/DrawSyncCallback.hpp>
#include <dolphin/os.h>
#include <dolphin/types.h>

class TTimeArray {
public:
	// fabricated
	struct Entry {
		u32 start, end;
	};

	enum { MAX_SIZE = 64 };

	TTimeArray()
	    : mSize(0)
	{
	}
	bool append(u32, u32);

	// fabricated
	int size() { return mSize; }
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
	static void snapGxTimeStatic(u32 v)
	{
		if (!_instance)
			return;
		_instance->snapGXTime(v);
	}

	static void startTimer(u32 v)
	{
		TTimeRec* inst = _instance;
		if (!inst)
			return;
		OSTick tick = OSGetTick();
		// TODO: uh oh, second arg isn't "end", it's some kind
		// of a 4-byte array?!
		inst->crTimeAry()[0].append(tick, v);
	}

	static void endTimer()
	{
		TTimeRec* inst = _instance;
		if (!inst)
			return;
		OSTick tick = OSGetTick();
		inst->crTimeAry()[0].append(tick, 0);
	}

public:
	/* 0x4 */ TTimeArray unk4[2][2];
	/* 0x814 */ u32 unk814;
	/* 0x818 */ u8 unk818;
	/* 0x81A */ u16 unk81A;
	/* 0x81C */ u16 unk81C;
};

#endif
