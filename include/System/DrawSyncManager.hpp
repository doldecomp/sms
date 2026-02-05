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
		TDrawSyncTokenRange();
		TDrawSyncTokenRange(u16, u16, TDrawSyncCallback*);
	};

	static void start(u32, u32, s32);
	void end();
	static void drawSyncCallback(u16);
	void threadFunc(void*);
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
	/* 0x348 */ void* unk348;
	/* 0x34C */ u16 unk34C;
};

#endif
