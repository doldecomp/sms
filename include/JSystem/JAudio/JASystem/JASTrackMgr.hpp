#ifndef JASTRACKMGR_HPP
#define JASTRACKMGR_HPP

#include "types.h"

namespace JASystem {

class TTrack;

namespace Kernel {
	struct TPortArgs;
}

namespace TrackMgr {
	void init(int, int);
	void reset();
	void initRegistTrack();
	TTrack* getNewTrack();
	int allocNewRoot(TTrack*);
	void deAllocRoot(TTrack*);
	void registTrack(u32, TTrack*);
	void unRegistTrack(TTrack*);
	void backTrack(TTrack*);
	u32 getTrackHandle(u32);
	TTrack* handleToSeq(u32);
	void setPause(Kernel::TPortArgs*);
	void clearPause(Kernel::TPortArgs*);
	int getRemainFreeTracks();
	void registerTrackCallback(u16 (*)(TTrack*, u16));

	extern TTrack* sTrackList;
	extern TTrack* sTrackPool;
	extern void* sSeqOuter;
	extern TTrack* sRootTrack;
	extern void* sFreeSeqpQueue;
	extern u32 sRootSeqCount;
	extern u32 sTrackCount;
	extern void* sTLists;
	extern u32 seqRemain;
	extern void* getP;
	extern void* backP;
}; // namespace TrackMgr

} // namespace JASystem

#endif // JASTRACKMGR_HPP
