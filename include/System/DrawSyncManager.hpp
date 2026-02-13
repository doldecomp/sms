#ifndef SYSTEM_DRAW_SYNC_MANAGER_HPP
#define SYSTEM_DRAW_SYNC_MANAGER_HPP

#include <JSystem/JGadget/std-vector.hpp>
#include <System/DrawSyncCallback.hpp>
#include <dolphin/os/OSThread.h>
#include <dolphin/os/OSMessage.h>

class TDrawSyncManager {
public:
	class TDrawSyncTokenRange {
	public:
		TDrawSyncTokenRange(u16 param_1 = 0, u16 param_2 = 0,
		                    TDrawSyncCallback* param_3 = nullptr)
		    : unk0(param_1)
		    , unk2(param_2)
		    , unk4(param_3)
		{
		}

	public:
		/* 0x0 */ u16 unk0;
		/* 0x2 */ u16 unk2;
		/* 0x4 */ TDrawSyncCallback* unk4;
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
	/* 0x0 */ JGadget::TVector<TDrawSyncTokenRange> unk0;
	/* 0x18 */ OSThread unk18;
	/* 0x328 */ OSMessageQueue unk328;
	/* 0x348 */ class TFifo* unk348;
	/* 0x34C */ u16 unk34C;
};

#endif
