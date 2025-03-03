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
	static void registTrack(unsigned long, TTrack*);
	static void unRegistTrack(TTrack*);
	static void backTrack(TTrack*);
	static unsigned long getTrackHandle(unsigned long);
	static TTrack* handleToSeq(unsigned long);
	static void setPause(Kernel::TPortArgs*);
	static void clearPause(Kernel::TPortArgs*);
	static int getRemainFreeTracks();
	static void registerTrackCallback(unsigned short (*)(TTrack*,
	                                                     unsigned short));

	static TTrack* sTrackList;
	static TTrack* sTrackPool;
	static void* sSeqOuter;
	static TTrack* sRootTrack;
	static void* sFreeSeqpQueue;
	static unsigned long sRootSeqCount;
	static unsigned long sTrackCount;
	static void* sTLists;
	static unsigned long seqRemain;
	static void* getP;
	static void* backP;
};

} // namespace JASystem

#endif // JASTRACKMGR_HPP
