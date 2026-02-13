#include <System/DrawSyncManager.hpp>
#include <dolphin/gx.h>

TDrawSyncManager* TDrawSyncManager::smInstance;

class TFifo {
public:
	TFifo(int param_1)
	    : unk4(param_1)
	    , unk8(0)
	    , unkC(0)
	{
		unk0 = new void*[unk4 + 1];
	}

	int getLoopIdx(u32 param_1)
	{
		if (param_1 >= unk4 + 1)
			param_1 = 0;
		return param_1;
	}

	// fabricated
	int getThing()
	{
		if (unk8 <= unkC)
			return unkC - unk8;
		else
			return unkC + unk4 + 1 - unk8;
	}

	// fabricated
	void skip() { unk8 = getLoopIdx(unkC + 1); }

	// fabricated
	void* read() { return unk0[getLoopIdx(unk8 + 1)]; }

	// fabricated
	void push(void* param_1)
	{
		unk0[unkC] = param_1;
		unkC       = getLoopIdx(unkC + 1);
	}

private:
	/* 0x0 */ void** unk0;
	/* 0x4 */ int unk4;
	/* 0x8 */ int unk8;
	/* 0xC */ u32 unkC;
};

TDrawSyncManager* TDrawSyncManager::start(u32 param_1, u32 param_2, s32 param_3)
{
	if (smInstance == nullptr)
		smInstance = new TDrawSyncManager(param_1, param_2, param_3);
	return smInstance;
}

void TDrawSyncManager::end() { }

void TDrawSyncManager::drawSyncCallback(u16 param_1)
{
	if (smInstance != nullptr)
		smInstance->drawSyncCallbackSub(param_1);
}

void* TDrawSyncManager::threadFunc(void* param_1)
{
	TDrawSyncManager* self = (TDrawSyncManager*)param_1;
	for (;;) {
		void* msg;
		OSReceiveMessage(&self->unk328, &msg, 1);
		if ((size_t)msg >= 0x80000000) {
			self->unk348->push(msg);

			u32 iVar1 = self->unk348->getThing();

			if (iVar1 == 2)
				GXEnableBreakPt(msg);
		} else {
			if ((size_t)msg >= 0x10000)
				break;

			self->unk348->skip();

			u32 iVar1 = self->unk348->getThing();

			if (iVar1 != 0) {
				if (iVar1 == 1)
					GXDisableBreakPt();
				else if (iVar1 >= 2)
					GXEnableBreakPt(self->unk348->read());
			}
		}
	}
	return 0;
}

TDrawSyncManager::TDrawSyncManager(u32 param_1, u32 param_2, s32 param_3)
    : unk0(param_1)
{
	unk34C    = 0;
	u8* stack = new u8[0x1000];
	OSCreateThread(&unk18, threadFunc, this, stack + 0x1000, 0x1000, param_3,
	               0);
	OSInitMessageQueue(&unk328, new u8[0x50], 0x14);
	unk348 = new TFifo(param_2);
	OSResumeThread(&unk18);
}

TDrawSyncManager::~TDrawSyncManager() { }

void TDrawSyncManager::setCallback(u32 param_1, u16 param_2, u16 param_3,
                                   TDrawSyncCallback* param_4)
{
	unk0[param_1] = TDrawSyncTokenRange(param_2, param_3, param_4);
}

void TDrawSyncManager::drawSyncCallbackSub(u16 param_1)
{
	if (param_1 == 0) {
		if (!(unk34C & 2))
			OSSendMessage(&unk328, (void*)(size_t)param_1, 1);
		return;
	}

	for (TDrawSyncTokenRange* it = unk0.begin(); it != unk0.end(); ++it)
		if (it->unk4 != nullptr && it->unk0 <= param_1 && param_1 <= it->unk2) {
			it->unk4->drawSyncCallback(param_1);
			if (!(unk34C & 2))
				OSSendMessage(&unk328, (void*)(size_t)param_1, 1);
			return;
		}
}

void TDrawSyncManager::pushBreakPoint()
{
	if (unk34C & 3)
		return;

	GXFlush();
	void* readPtr;
	void* writePtr;
	GXGetFifoPtrs(GXGetCPUFifo(), &readPtr, &writePtr);
	OSSendMessage(&unk328, writePtr, 1);
}
