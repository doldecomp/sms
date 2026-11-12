#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASTrack.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <macros.h>

namespace JASystem {

// fabricated
struct TrackRegistration {
	TTrack* mTrack;
	u32 mRegistrationId;
};

namespace TrackMgr {
	static TrackRegistration sTrackList[32];
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

void TrackMgr::init(int track_capacity, int root_sequence_capacity)
{
	sTrackPool = new (JASDram, 0) TTrack[track_capacity];
	sSeqOuter  = new (JASDram, 0) TTrack::TOuterParam[track_capacity];

	sFreeSeqpQueue = new (JASDram, 0) TTrack*[track_capacity];

	// BUG: uses root capacity, which can under-clear or overrun this queue.
	Calc::bzero(sFreeSeqpQueue, root_sequence_capacity * sizeof(TTrack*));

	sRootTrack = new (JASDram, 0) TTrack*[root_sequence_capacity];
	Calc::bzero(sRootTrack, root_sequence_capacity * sizeof(TTrack*));

	sTrackCount   = track_capacity;
	sRootSeqCount = root_sequence_capacity;
}

TTrack* TrackMgr::handleToSeq(u32 handle)
{
	if (handle >= sRootSeqCount)
		return nullptr;

	return sRootTrack[handle];
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
	for (int i = 0; i < ARRAY_COUNT(sTrackList); ++i)
		sTrackList[i].mTrack = nullptr;
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

	result->mSeqState = TTrack::SEQ_READY;
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

void TrackMgr::registTrack(u32 id, TTrack* track)
{
	// BUG: checks IDs even in empty slots, blocking reuse of stale IDs.
	for (int i = 0; i < sTLists; ++i)
		if (id == sTrackList[i].mRegistrationId)
			return;

	u32 index;
	if (sTLists == ARRAY_COUNT(sTrackList)) {
		for (index = 0; index < ARRAY_COUNT(sTrackList); ++index)
			if (sTrackList[index].mTrack == 0)
				break;

		if (index == ARRAY_COUNT(sTrackList))
			return;
	} else {
		index = sTLists;
		++sTLists;
	}

	sTrackList[index].mRegistrationId = id;
	sTrackList[index].mTrack          = track;
}

void TrackMgr::unRegistTrack(TTrack* track)
{
	for (int i = 0; i < sTLists; ++i)
		if (sTrackList[i].mTrack == track)
			sTrackList[i].mTrack = nullptr;

	while (sTLists != 0) {
		if (sTrackList[sTLists - 1].mTrack != 0)
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
