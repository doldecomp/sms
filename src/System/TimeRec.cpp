#include <System/TimeRec.hpp>
#include <System/DrawSyncManager.hpp>
#include <dolphin/gx/GXManage.h>
#include <types.h>

TTimeRec* TTimeRec::_instance;

void TTimeRec::drawSyncCallback(u16) { }

void TTimeRec::suppleGXTime() { }

void TTimeRec::snapGXTime(u32 param_1)
{
	if (unk4[unk814][1].append(0, param_1)) {
		if ((unk81C & 1) == 0) {
			u32 uVar1 = unk81A;
			u32 uVar2 = unk4[unk814][1].mSize;
			if (TDrawSyncManager::smInstance)
				TDrawSyncManager::smInstance->pushBreakPoint();
			GXSetDrawSync(uVar1 + (uVar2 - 1));
		}
	}
}

void TTimeRec::flip()
{
	unk814 ^= 1;
	unk4[unk814][0].mSize = 0;
	unk4[unk814][1].mSize = 0;
}

TTimeRec::TTimeRec(u16 param_1)
    : unk814(0)
    , unk818(0)
    , unk81A(param_1)
    , unk81C(0)
{
}

void TTimeRec::drawSyncCallbackSt(u16) { }

void TTimeRec::end() { }

TTimeRec* TTimeRec::start(u16 param_1)
{
	if (_instance == nullptr)
		_instance = new TTimeRec(param_1);
	return _instance;
}

bool TTimeArray::append(u32 start, u32 end)
{
	bool result = false;
	if (mSize < MAX_SIZE) {
		Entry& entry = mEntries[mSize++];
		entry.start  = start;
		entry.end    = end;
		result       = true;
	}
	return result;
}
