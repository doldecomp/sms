#include "JSystem/JAudio/JASystem/JASTrackMgr.hpp"

namespace JASystem {

TTrack* TrackMgr::sTrackList   = 0;
TTrack* TrackMgr::sTrackPool   = 0;
void* TrackMgr::sSeqOuter      = 0;
TTrack* TrackMgr::sRootTrack   = 0;
void* TrackMgr::sFreeSeqpQueue = 0;
u32 TrackMgr::sRootSeqCount    = 0;
u32 TrackMgr::sTrackCount      = 0;
void* TrackMgr::sTLists        = 0;
u32 TrackMgr::seqRemain        = 0;
void* TrackMgr::getP           = 0;
void* TrackMgr::backP          = 0;

void TrackMgr::init(int param1, int param2) { }

void TrackMgr::reset() { }

void TrackMgr::initRegistTrack() { }

TTrack* TrackMgr::getNewTrack() { return 0; }

int TrackMgr::allocNewRoot(TTrack* track) { return 0; }

void TrackMgr::deAllocRoot(TTrack* track) { }

void TrackMgr::registTrack(u32 param, TTrack* track) { }

void TrackMgr::unRegistTrack(TTrack* track) { }

void TrackMgr::backTrack(TTrack* track) { }

u32 TrackMgr::getTrackHandle(u32 param) { return 0; }

TTrack* TrackMgr::handleToSeq(u32 param) { return 0; }

void TrackMgr::setPause(Kernel::TPortArgs* args) { }

void TrackMgr::clearPause(Kernel::TPortArgs* args) { }

int TrackMgr::getRemainFreeTracks() { return 0; }

void TrackMgr::registerTrackCallback(u16 (*callback)(TTrack*, u16)) { }

} // namespace JASystem
