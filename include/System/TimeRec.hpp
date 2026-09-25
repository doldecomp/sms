#ifndef SYSTEM_TIME_REC_HPP
#define SYSTEM_TIME_REC_HPP

#include <JSystem/JDrama/JDRFlag.hpp>
#include <System/DrawSyncCallback.hpp>
#include <dolphin/os.h>
#include <dolphin/types.h>

class TTimeArray {
public:
	// fabricated
	struct Entry {
		u32 time;
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
	void clear() { mSize = 0; }

public:
	/* 0x0 */ Entry mEntries[MAX_SIZE];
	/* 0x200 */ int mSize;
};

class TTimeRec : public TDrawSyncCallback {
public:
	TTimeRec(u16 tokenBase);
	virtual void drawSyncCallback(u16 token);

	static TTimeRec* start(u16 tokenBase);
	static void end();
	static void drawSyncCallbackSt(u16 token);
	static void suppleGXTime();

	void flip();
	void snapGXTime(u32 color);

	TTimeArray* crTimeAry() { return mTimeAry[mCrIdx]; }

	bool appendGX(u32 tick, u32 color)
	{
		return crTimeAry()[1].append(tick, color);
	}

	void startGXFrame(u32 tick) { appendGX(tick, 0); }

	void startFrame(u32 tick)
	{
		crTimeAry()[0].append(tick, 0);
		startGXFrame(tick);
	}

	static void startFrameSt(u32 tick)
	{
		if (_instance)
			_instance->startFrame(tick);
	}

	static void snapCPUTime(u32 color)
	{
		TTimeRec* rec = _instance;
		if (rec) {
			OSTick tick = OSGetTick();
			rec->crTimeAry()[0].append(tick, color);
		}
	}

	static void snapGXTimeSt(u32 color)
	{
		if (_instance)
			_instance->snapGXTime(color);
	}

	static TTimeRec* instance() { return _instance; }

	static TTimeRec* _instance;

public:
	/* 0x4 */ TTimeArray mTimeAry[2][2];
	/* 0x814 */ u32 mCrIdx;
	/* 0x818 */ u8 unk818;
	/* 0x81A */ u16 mTokenBase;
	/* 0x81C */ JDrama::TFlagT<u16> mFlags;
};

#endif
