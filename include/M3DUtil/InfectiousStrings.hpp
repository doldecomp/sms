#ifndef M3DUTIL_INFECTIOUS_STRINGS_HPP
#define M3DUTIL_INFECTIOUS_STRINGS_HPP

// Every retail TU carrying these mtx calc type names also carries the pair in
// System/DummyStrings.hpp, and always ahead of them, so that header comes
// first here.
#include <System/DummyStrings.hpp>

// TODO: this should live in some other header and infect various TUs, but we
// don't know which one yet. Ruled out so far, because a header-level static is
// emitted in every TU that includes it -- and a deadstripped one still shows in
// the map as UNUSED, so absence from a TU is proof:
//   M3DUtil/MActor.hpp and M3DUtil/MActorAnm.hpp -- MActorUtil.cpp and
//     LodAnm.cpp construct and call MActor, so they must include these, yet
//     neither TU carries MtxCalcTypeName at all.
//   Strategic/LiveActor.hpp -- same argument through LodAnm.cpp.
// The 181 carrier TUs are a wide, mixed set (DrawUtil, MarDirector, GCConsole2,
// NpcSound, most Enemy and MoveBG files), so the home is something broad that
// those three TUs happen not to include.
static const char* MtxCalcTypeName[] = {
	"MActorMtxCalcType_Basic クラシックスケールＯＮ",
	"MActorMtxCalcType_Softimage クラシックスケールＯＦＦ",
	"MActorMtxCalcType_MotionBlend モーションブレンド",
	"MActorMtxCalcType_User ユーザー定義",
};

#endif
