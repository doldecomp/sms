#include <System/TalkCursor.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/MActor.hpp>
#include <NPC/NpcBase.hpp>

// rogue
#include <M3DUtil/InfectiousStrings.hpp>

// TODO: every instruction matches, the stack frame is 0x28 instead of 0x30.
// Locals in this function are inert for frame size; only a named local inside
// an inlined callee moves it. Giving JKRGetResource one, as in
// `void* r = getGlbResource(name); return r;`, lands on 0x30 exactly, but that
// header is shared with every other resource user.
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

// TODO: every instruction matches, the stack frame is 0x60 instead of 0x78.
// Passing a temporary instead, as in setBaseTRMtx(TPosition3f(getCursorPos())),
// lands on 0x78 exactly but adds a real call to SMatrix34C<f32>::SMatrix34C(),
// and mario.MAP says TalkCursor.cpp only ever emitted TViewObj's vtable, its
// destructor and identity33 as weak symbols. In a standalone repro, dropping
// the empty SMatrix34C<f32> default constructor removes that call and leaves
// the frame at 0x78, but that would make the type trivial repo-wide.
// TODO: nonmatching, frame 0x60 vs target 0x78
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
