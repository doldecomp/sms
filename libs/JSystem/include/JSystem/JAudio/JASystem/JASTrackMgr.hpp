#ifndef JASTRACKMGR_HPP
#define JASTRACKMGR_HPP

#include <JSystem/JAudio/JASystem/JASTrack.hpp>
#include <dolphin/types.h>

namespace JASystem {

namespace Kernel {
	struct TPortArgs;
}

namespace TrackMgr {
	void init(int track_capacity, int root_sequence_capacity);
	void reset();
	void initRegistTrack();
	TTrack* getNewTrack();
	u32 allocNewRoot(TTrack* track);
	u32 deAllocRoot(TTrack* track);
	void registTrack(u32 id, TTrack* track);
	void unRegistTrack(TTrack* track);
	bool backTrack(TTrack* track);
	u32 getTrackHandle(u32);
	TTrack* handleToSeq(u32 handle);
	void setPause(Kernel::TPortArgs*);
	void clearPause(Kernel::TPortArgs*);
	int getRemainFreeTracks();
	void registerTrackCallback(u16 (*callback)(TTrack*, u16));

	extern TTrack* sTrackPool;
	extern TTrack::TOuterParam* sSeqOuter;
	extern TTrack** sRootTrack;
	extern TTrack** sFreeSeqpQueue;
	extern s32 sRootSeqCount;
	extern u32 sTrackCount;
	extern u32 sTLists;
	extern u32 seqRemain;
	extern u32 getP;
	extern u32 backP;
}; // namespace TrackMgr

} // namespace JASystem

#endif // JASTRACKMGR_HPP
