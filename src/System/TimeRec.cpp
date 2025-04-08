#include <System/TimeRec.hpp>
#include <System/DrawSyncManager.hpp>
#include <dolphin/gx/GXManage.h>
#include <dolphin/os.h>
#include <types.h>

TTimeRec* TTimeRec::_instance;

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

TTimeRec* TTimeRec::start(u16 param_1)
{
	if (_instance == nullptr)
		_instance = new TTimeRec(param_1);
	return _instance;
}

void TTimeRec::end() { }

void TTimeRec::drawSyncCallbackSt(u16) { }

TTimeRec::TTimeRec(u16 param_1)
    : unk814(0)
    , unk818(0)
    , unk81A(param_1)
    , unk81C(0)
{
}

void TTimeRec::flip()
{
	TTimeArray& array = _instance->crTimeAry()[1];
	int size          = array.mSize;
	if (size >= 3) {
		int i    = size - 1;
		u32 curr = array.mEntries[i].start;
		while (i > 0) {
			--i;
			if (array.mEntries[i].start == 0) {
				array.mEntries[i].start = curr;
			} else {
				curr = array.mEntries[i].start;
			}
		}
	}

	unk814 ^= 1;
	crTimeAry()[0].mSize = 0;
	crTimeAry()[1].mSize = 0;
}

void TTimeRec::snapGXTime(u32 param_1)
{
	if (crTimeAry()[1].append(0, param_1)) {
		if ((unk81C & 1) == 0) {
			u16 token = unk81A + crTimeAry()[1].size() - 1;
			if (TDrawSyncManager::smInstance)
				TDrawSyncManager::smInstance->pushBreakPoint();
			GXSetDrawSync(token);
		}
	}
}

void TTimeRec::suppleGXTime() { }

void TTimeRec::drawSyncCallback(u16 param_1)
{
	unk4[unk814][1][param_1 - unk81A].start = OSGetTick();
}
