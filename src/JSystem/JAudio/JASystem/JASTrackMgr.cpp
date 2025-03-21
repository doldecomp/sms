#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASTrack.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>

namespace JASystem {

struct FabricatedTrackListEntryStruct {
	TTrack* unk0;
	u32 unk4;
};

namespace TrackMgr {
	static FabricatedTrackListEntryStruct sTrackList[32];
}

TTrack* TrackMgr::sTrackPool             = 0;
TTrack::TOuterParam* TrackMgr::sSeqOuter = 0;
TTrack** TrackMgr::sRootTrack            = 0;
TTrack** TrackMgr::sFreeSeqpQueue        = 0;
s32 TrackMgr::sRootSeqCount              = 0;
u32 TrackMgr::sTrackCount                = 0;
u32 TrackMgr::sTLists                    = 0;
u32 TrackMgr::seqRemain                  = 0;
u32 TrackMgr::getP                       = 0;
u32 TrackMgr::backP                      = 0;

void TrackMgr::init(int param1, int param2)
{
	sTrackPool = new (JASDram, 0) TTrack[param1];
	sSeqOuter  = new (JASDram, 0) TTrack::TOuterParam[param1];

	sFreeSeqpQueue = new (JASDram, 0) TTrack*[param1];
	Calc::bzero(sFreeSeqpQueue, param2 * sizeof(TTrack*));

	sRootTrack = new (JASDram, 0) TTrack*[param2];
	Calc::bzero(sRootTrack, param2 * sizeof(TTrack*));

	sTrackCount   = param1;
	sRootSeqCount = param2;
}

TTrack* TrackMgr::handleToSeq(u32 idx)
{
	if (idx >= sRootSeqCount)
		return nullptr;

	return sRootTrack[idx];
}

u32 TrackMgr::getTrackHandle(u32 param) { return 0; }

void TrackMgr::setPause(Kernel::TPortArgs* args) { }

void TrackMgr::clearPause(Kernel::TPortArgs* args) { }

void TrackMgr::reset()
{
	for (int i = 0; i < sTrackCount; ++i) {
		TTrack* track = &sTrackPool[i];
		track->reset();
		sSeqOuter[i].initExtBuffer();
		track->assignExtBuffer(&sSeqOuter[i]);
		sFreeSeqpQueue[i] = track;
	}
	seqRemain = sTrackCount;
	backP     = 0;
	getP      = 0;
	initRegistTrack();
}

int TrackMgr::getRemainFreeTracks() { return seqRemain; }

void TrackMgr::initRegistTrack()
{
	sTLists = 0;
	for (int i = 0; i < 0x20; ++i)
		sTrackList[i].unk0 = nullptr;
}

TTrack* TrackMgr::getNewTrack()
{
	if (seqRemain == 0)
		return nullptr;

	TTrack* result = sFreeSeqpQueue[getP];
	--seqRemain;
	++getP;
	if (getP == sTrackCount)
		getP = 0;

	result->unk3C4 = 2;
	result->setInnerMemory(1);
	return result;
}

u32 TrackMgr::allocNewRoot(TTrack* track)
{
	for (u8 i = 0; i < sRootSeqCount; ++i) {
		if (!sRootTrack[(u8)i]) {
			sRootTrack[(u8)i] = track;
			return i;
		}
	}
	return -1;
}

u32 TrackMgr::deAllocRoot(TTrack* track)
{
	for (u8 i = 0; i < sRootSeqCount; ++i) {
		if (sRootTrack[(u8)i] == track) {
			sRootTrack[(u8)i] = nullptr;
			return i;
		}
	}
	return -1;
}

void TrackMgr::registTrack(u32 param, TTrack* track)
{
	for (int i = 0; i < sTLists; ++i) {
		if (param == sTrackList[i].unk4)
			return;
	}

	u32 i;
	if (sTLists == 32) {
		for (i = 0; i < 32; ++i) {
			if (sTrackList[i].unk0 == 0)
				break;
		}
		if (i == 32)
			return;
	} else {
		i = sTLists;
		++sTLists;
	}

	sTrackList[i].unk4 = param;
	sTrackList[i].unk0 = track;
}

void TrackMgr::unRegistTrack(TTrack* track)
{
	for (int i = 0; i < sTLists; ++i) {
		if (sTrackList[i].unk0 == track)
			sTrackList[i].unk0 = nullptr;
	}

	while (sTLists != 0) {
		if (sTrackList[sTLists - 1].unk0 != 0)
			break;
		--sTLists;
	}
}

bool TrackMgr::backTrack(TTrack* track)
{
	if (seqRemain == sTrackCount)
		return false;

	sFreeSeqpQueue[backP] = track;
	++seqRemain;
	++backP;
	if (backP == sTrackCount)
		backP = 0;

	return true;
}

void TrackMgr::registerTrackCallback(u16 (*callback)(TTrack*, u16))
{
	TTrack::registerTrackCallback(callback);
}

} // namespace JASystem
