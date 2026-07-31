#ifndef M3DUTIL_INFECTIOUS_STRINGS_HPP
#define M3DUTIL_INFECTIOUS_STRINGS_HPP

// Every retail TU carrying these mtx calc type names also carries the pair in
// System/DummyStrings.hpp, and always ahead of them, so that header comes
// first here.
#include <System/DummyStrings.hpp>

// TODO: this should live in some other header and infect various TUs, but we
// don't know which one yet
static const char* MtxCalcTypeName[] = {
	"MActorMtxCalcType_Basic クラシックスケールＯＮ",
	"MActorMtxCalcType_Softimage クラシックスケールＯＦＦ",
	"MActorMtxCalcType_MotionBlend モーションブレンド",
	"MActorMtxCalcType_User ユーザー定義",
};

#endif
