static float const cDefaultScaleA[3] = { 1.0f, 1.0f, 1.0f };
static float const cDefaultScaleB[3] = { 1.0f, 1.0f, 1.0f };
static int const cMtxCalcOrder[4] = { 0, 2, 1, 3 };

static const char* MtxCalcTypeName[] = {
	"MActorMtxCalcType_Basic クラシックスケールＯＮ",
	"MActorMtxCalcType_Softimage クラシックスケールＯＦＦ",
	"MActorMtxCalcType_MotionBlend モーションブレンド",
	"MActorMtxCalcType_User ユーザー定義",
};

static const char* bwanwan_bastable[] = {
	"/scene/bwanwan/bas/bwanwan_bark.bas",
	0,
	"/scene/bwanwan/bas/bwanwan_shake.bas",
	0,
	"/scene/bwanwan/bas/bwanwan_wait.bas",
	"/scene/bwanwan/bas/bwanwan_wait2.bas",
	0,
};
