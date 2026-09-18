#include <System/TalkCursor.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/MActor.hpp>
#include <NPC/NpcBase.hpp>

// rogue
#include <M3DUtil/InfectiousStrings.hpp>

// TODO: frame 0x28 vs retail 0x30; all 60 instructions match, so retail's
// low (inline-temporary) region is 8 bytes bigger. Measured as zero here: a
// TTalkCursor::getMActor() accessor over unk10 (it is +8 in associateNPC but
// nothing here), naming the `new J3DModel` result, naming the
// J3DModelLoaderDataBase::load result (also -9%), unkC.setBit() over on().
// Closure batch 103 swept 24 combinations of {`MActor* actor = unk10` local /
// raw unk10 / getMActor()} x {named J3DModel*} x {named J3DModelData*} x
// {named `void* bmd`}: the `actor` local plus the named `bmd` is the only 60-
// instruction shape and it is 0x28; nothing reaches 0x30. Also zero or worse:
// `MActor* actor = new MActor(anmData); unk10 = actor;` (97.9), a setMActor()
// setter (97.9), `new MActorAnmData()` with parens, a named `const char*` for
// the anm-data path (94.1), a named `u32` for the loader flags (95.8), a
// TU-local `static inline` that makes the anm data (84.2) or the J3DModel
// (99.6) or forwards JKRGetResource (99.9), and a parked
// `static inline MActor* f(TTalkCursor*)` (the batch-103 free-function binding
// lever -- it is worth +8 only when its argument is a fresh *global* load, so
// `this` buys nothing here).
void TTalkCursor::loadAfter()
{
	MActorAnmData* anmData = new MActorAnmData;
	anmData->init("/common/cursor_b", nullptr);
	unk10 = new MActor(anmData);

	MActor* actor = unk10;
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

// TODO: frame 0x60 vs retail 0x78; all 35 instructions match and every slot
// (the getCursorPos() return temporary, mtx, the three stores) is exactly 20
// bytes low, so the whole residue is the low inline-temporary region: 24 bytes
// there, which is also exactly two TVec3 temporaries.
// Measured ladder over the 0x50 no-accessor base: MActor::getModel() +16, a
// TTalkCursor::getMActor() accessor over unk10 +8 (and the chain saturates at
// two levels: a third forwarder above getMActor() adds nothing), spelling
// JGeometry::TPosition3::translation's body as
// `setTrans(t.x, t.y, t.z)` instead of `setTrans(t)` +8 with no instruction
// change. Those three together reach 0x70 and nothing reaches 0x78.
// Measured as zero: translation() taking the vector by value, mtx.mMtx over
// the operator ArrType*() conversion, unkC.setBit() over off()/on(), a
// J3DModel* named local, a getCursorModel() wrapper folding both links.
// Batch 69: the +24 is a dead non-trivial local in an inlined callee (see
// docs/catalog/frame-gaps.md, "The dead low region"), but every candidate here
// is a shared header -- JGeometry::TPosition3::translation, MActor::getModel,
// TFlagT::on/off -- since TBaseNPC::getCursorPos returns its 12 bytes by value
// through a real bl. Nothing in this unit's own files can carry it.
// Closure batch 103: the same parked-free-function lever (a `static inline`
// taking the cursor by pointer and returning unk10 or unk10->getModel()) is
// +0 at either site and in combination -- it binds only for a fresh global
// load, and this function reads no global.
// Rejected (instruction changes): TPosition3f mtx(getCursorPos()) (-6%),
// a named TVec3 for the cursor position (+8 frame but six extra instructions,
// the copy is not elided), mtx.identity33() + mtx.setTrans() spelled out
// (-29%), translation() calling TRotation3<T>::identity33() directly (-35%).
void TTalkCursor::associateNPC(TBaseNPC* param_1)
{
	if (param_1) {
		TPosition3f mtx;
		mtx.translation(param_1->getCursorPos());
		unk10->getModel()->setBaseTRMtx(mtx);
		unkC.off(CUE_CALC_VIEW | CUE_ENTRY);
	} else {
		unkC.on(CUE_CALC_VIEW | CUE_ENTRY);
	}
}
