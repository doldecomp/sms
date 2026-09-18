#include <System/TalkCursor.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/MActor.hpp>
#include <NPC/NpcBase.hpp>

// rogue
#include <M3DUtil/InfectiousStrings.hpp>

// The 8 bytes of low region retail has here are one inline expansion: reading
// `unk10` through a level that binds its result. This stands in for a
// `TTalkCursor::getMActor()` accessor written as
// `MActor* actor = unk10; return actor;`, but `System/TalkCursor.hpp` is
// included by MarNameRefGen, MarDirectorEvent and MarDirectorSetup2, so it is
// parked here and reported.
// Also +8 and interchangeable with it (any one of them closes the function, and
// they do not stack usefully): a binding level on `new MActorAnmData`, on
// `new MActor(anmData)`, or on `JKRGetResource`. Worth zero: the same levels
// without the binding (a plain forwarder above `MActorAnmData::init`,
// `MActor::setModel/setBck/setBrk` or `unkC.on`, all of which sit above real
// `bl`s). Worse: a binding level on `J3DModelLoaderDataBase::load` (frame
// unmoved, 5 diffs).
// Superseded by the above (closure batch 103's 24-combination sweep): a
// non-binding `getMActor()`, naming the `new J3DModel` or loader results,
// `unkC.setBit()`, `MActor* actor = new MActor(anmData); unk10 = actor;`, a
// `setMActor()` setter, a named path string or loader-flag word.
static inline MActor* TalkCursorMActor(TTalkCursor* cursor)
{
	MActor* actor = cursor->unk10;
	return actor;
}

void TTalkCursor::loadAfter()
{
	MActorAnmData* anmData = new MActorAnmData;
	anmData->init("/common/cursor_b", nullptr);
	unk10 = new MActor(anmData);

	MActor* actor = TalkCursorMActor(this);
	void* bmd     = JKRGetResource("/common/cursor_b/default.bmd");
	actor->setModel(new J3DModel(J3DModelLoaderDataBase::load(
	                                 bmd, J3DMLF_MaterialPEFull
	                                          | (2 << J3DMLF_TevStageNumShift)),
	                             0, 1),
	                0);
	actor->setBck("icon_rot");
	actor->setBrk("icon_flash");
	unkC.on(CUE_CALC_VIEW | CUE_ENTRY);
}

void TTalkCursor::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_DRAW) {
		GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
	} else {
		unk10->perform(cue, graphics);
	}
}

// The 24 bytes of low region are two inline expansions: the model fetch
// through one level that binds its result (+0x10) and the NPC receiver through
// one more (+8). Both are placeholders for levels this unit cannot spell --
// `MActor::getModel()` and `TTalkCursor::getMActor()` written to bind, and
// whatever the caller side really hands `getCursorPos()` -- and their headers
// are shared, so they are parked here and reported. `TalkCursorTargetNPC` in
// particular is an identity forwarder: only its binding is load-bearing, and
// its real shape is not recovered.
// Measured over the 0x60 no-level base (retail 0x78, every slot 20 bytes low):
// a binding level on `unk10->getModel()` or on `unk10` alone +0x10 (slot 0x28,
// 8 short), two stacked binding levels on either +0x18 (slot 0x34, 4 long),
// and one binding level on the NPC pointer the missing +8 (slot 0x30 exactly).
// Worth zero, alone and in every pair: binding levels above `unkC.off`/`on`,
// `mtx.translation(...)`, `J3DModel::setBaseTRMtx`, a named or const-qualified
// `TBaseNPC*` local, a `(const TBaseNPC*)` cast at the call site, a helper
// returning the cursor position by value, and 4-, 8- or 12-byte named locals
// declared before `mtx` (they land above it and only move the frame).
// Rejected (instruction changes): `TPosition3f mtx(getCursorPos())`, a named
// `TVec3` for the cursor position, `identity33()` + `setTrans()` spelled out.
static inline J3DModel* TalkCursorModel(TTalkCursor* cursor)
{
	J3DModel* model = cursor->unk10->getModel();
	return model;
}

static inline TBaseNPC* TalkCursorTargetNPC(TBaseNPC* npc)
{
	TBaseNPC* target = npc;
	return target;
}

void TTalkCursor::associateNPC(TBaseNPC* param_1)
{
	if (param_1) {
		TPosition3f mtx;
		mtx.translation(TalkCursorTargetNPC(param_1)->getCursorPos());
		TalkCursorModel(this)->setBaseTRMtx(mtx);
		unkC.off(CUE_CALC_VIEW | CUE_ENTRY);
	} else {
		unkC.on(CUE_CALC_VIEW | CUE_ENTRY);
	}
}
