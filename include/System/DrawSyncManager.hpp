#ifndef SYSTEM_DRAW_SYNC_MANAGER_HPP
#define SYSTEM_DRAW_SYNC_MANAGER_HPP

#include <JSystem/JGadget/std-vector.hpp>
#include <System/DrawSyncCallback.hpp>
#include <dolphin/os/OSThread.h>
#include <dolphin/os/OSMessage.h>

/**
 * This class is responsible for managing GPU -> CPU sync.
 * GX has a feature where you can call GXSetDrawSync with a token value,
 * and then when that call reaches the end of pipe (asynchronously), the
 * OS will call a callback function with the token value via an interrupt.
 *
 * This class allows for callbacks to be easily bound to ranges of token values.
 */
class TDrawSyncManager {
public:
	class TDrawSyncTokenRange {
	public:
		TDrawSyncTokenRange(u16 param_1 = 0, u16 param_2 = 0,
		                    TDrawSyncCallback* param_3 = nullptr)
		    : mRangeStart(param_1)
		    , mRangeEnd(param_2)
		    , mCallback(param_3)
		{
		}

	public:
		/* 0x0 */ u16 mRangeStart;
		/* 0x2 */ u16 mRangeEnd;
		/* 0x4 */ TDrawSyncCallback* mCallback;
	};

	static TDrawSyncManager* start(u32, u32, s32);
	static void end();
	static void drawSyncCallback(u16);
	static void* threadFunc(void*);

	TDrawSyncManager(u32, u32, s32);
	~TDrawSyncManager();
	void setCallback(u32, u16, u16, TDrawSyncCallback*);
	void drawSyncCallbackSub(u16);
	void pushBreakPoint();

	static TDrawSyncManager* smInstance;

public:
	/* 0x0 */ JGadget::TVector<TDrawSyncTokenRange> mCallbacks;
	/* 0x18 */ OSThread mProcessingThread;
	/* 0x328 */ OSMessageQueue mMessageQueue;
	/* 0x348 */ class TFifo* mFifo;
	/* 0x34C */ u16 mFlags;
};

#endif
