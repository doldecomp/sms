#include <System/TalkCursor.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/MActor.hpp>
#include <NPC/NpcBase.hpp>

// rogue
#include <M3DUtil/InfectiousStrings.hpp>

void TTalkCursor::loadAfter()
{
	MActorAnmData* anmData = new MActorAnmData;
	anmData->init("/common/cursor_b", nullptr);
	unk10 = new MActor(anmData);

	MActor* actor = unk10;
	void* bmd     = JKRGetResource("/common/cursor_b/default.bmd");
	actor->setModel(
	    new J3DModel(J3DModelLoaderDataBase::load(bmd, 0x10020000), 0, 1), 0);
	actor->setBck("icon_rot");
	actor->setBrk("icon_flash");
	unkC.on(0x204);
}

void TTalkCursor::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 8) {
		GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
	} else {
		unk10->perform(param_1, param_2);
	}
}

void TTalkCursor::associateNPC(TBaseNPC* param_1)
{
	if (param_1) {
		TPosition3f mtx(param_1->getCursorPos());
		unk10->getModel()->setBaseTRMtx(mtx);
		unkC.off(0x204);
	} else {
		unkC.on(0x204);
	}
}
