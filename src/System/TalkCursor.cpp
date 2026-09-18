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
