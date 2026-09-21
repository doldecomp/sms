#include <System/TimeRec.hpp>
#include <System/DrawSyncManager.hpp>
#include <dolphin/gx/GXManage.h>
#include <dolphin/os.h>
#include <types.h>

TTimeRec* TTimeRec::_instance;

bool TTimeArray::append(u32 time, u32 color)
{
	bool result = false;
	if (mSize < MAX_SIZE) {
		Entry& entry = mEntries[mSize++];
		entry.time   = time;
		entry.color  = color;
		result       = true;
	}
	return result;
}

TTimeRec* TTimeRec::start(u16 tokenBase)
{
	if (_instance == nullptr)
		_instance = new TTimeRec(tokenBase);
	return _instance;
}

void TTimeRec::end()
{
	if (_instance) {
		delete _instance;
		_instance = nullptr;
	}
}

void TTimeRec::drawSyncCallbackSt(u16 token)
{
	if (_instance)
		_instance->TTimeRec::drawSyncCallback(token);
}

TTimeRec::TTimeRec(u16 tokenBase)
    : mCrIdx(0)
    , unk818(0)
    , mTokenBase(tokenBase)
    , mFlags(0)
{
}

void TTimeRec::flip()
{
	suppleGXTime();
	mCrIdx ^= 1;
	crTimeAry()[0].clear();
	crTimeAry()[1].clear();
}

void TTimeRec::snapGXTime(u32 color)
{
	if (appendGX(0, color)) {
		if (!mFlags.check(1)) {
			u16 token = mTokenBase + crTimeAry()[1].size() - 1;
			if (TDrawSyncManager::smInstance)
				TDrawSyncManager::smInstance->pushBreakPoint();
			GXSetDrawSync(token);
		}
	}
}

void TTimeRec::suppleGXTime()
{
	TTimeArray& array = _instance->crTimeAry()[1];
	if (array.size() < 3)
		return;
	u32 curr = array[array.size() - 1].time;
	int i    = array.size() - 1;
	while (i > 0) {
		--i;
		if (array[i].time == 0)
			array[i].time = curr;
		else
			curr = array[i].time;
	}
}

void TTimeRec::drawSyncCallback(u16 token)
{
	crTimeAry()[1][token - mTokenBase].time = OSGetTick();
}
