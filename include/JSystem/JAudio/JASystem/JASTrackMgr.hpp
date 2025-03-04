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
	void init(int, int);
	void reset();
	void initRegistTrack();
	TTrack* getNewTrack();
	TTrack* allocNewRoot(TTrack*);
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
