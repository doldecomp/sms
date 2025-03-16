#ifndef JASTRACKMGR_HPP
#define JASTRACKMGR_HPP

#include <JSystem/JAudio/JASystem/JASTrack.hpp>
#include <types.h>

namespace JASystem {

namespace Kernel {
	struct TPortArgs;
}

namespace TrackMgr {
	void init(int, int);
	void reset();
	void initRegistTrack();
	TTrack* getNewTrack();
	u32 allocNewRoot(TTrack*);
	u32 deAllocRoot(TTrack*);
	void registTrack(u32, TTrack*);
	void unRegistTrack(TTrack*);
	bool backTrack(TTrack*);
	u32 getTrackHandle(u32);
	void* handleToSeq(u32);
	void setPause(Kernel::TPortArgs*);
	void clearPause(Kernel::TPortArgs*);
	int getRemainFreeTracks();
	void registerTrackCallback(u16 (*)(TTrack*, u16));

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
