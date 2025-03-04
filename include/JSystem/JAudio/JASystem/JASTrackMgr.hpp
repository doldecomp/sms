#ifndef JASTRACKMGR_HPP
#define JASTRACKMGR_HPP

#include "types.h"

namespace JASystem {

class TTrack;

namespace Kernel {
	struct TPortArgs;
}

class TrackMgr {
public:
	static void init(int, int);
	static void reset();
	static void initRegistTrack();
	static TTrack* getNewTrack();
	static TTrack* allocNewRoot(TTrack*);
	static void deAllocRoot(TTrack*);
	static void registTrack(u32, TTrack*);
	static void unRegistTrack(TTrack*);
	static void backTrack(TTrack*);
	static u32 getTrackHandle(u32);
	static TTrack* handleToSeq(u32);
	static void setPause(Kernel::TPortArgs*);
	static void clearPause(Kernel::TPortArgs*);
	static int getRemainFreeTracks();
	static void registerTrackCallback(u16 (*)(TTrack*, u16));

	static TTrack* sTrackList;
	static TTrack* sTrackPool;
	static void* sSeqOuter;
	static TTrack* sRootTrack;
	static void* sFreeSeqpQueue;
	static u32 sRootSeqCount;
	static u32 sTrackCount;
	static void* sTLists;
	static u32 seqRemain;
	static void* getP;
	static void* backP;
};

} // namespace JASystem

#endif // JASTRACKMGR_HPP
