#include <NPC/NpcParts.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DGXColorS10.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/SharedParts.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/LightUtil.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <NPC/NpcColor.hpp>
#include <NPC/NpcInitData.hpp>
#include <NPC/NpcBase.hpp>
#include <NPC/NpcManager.hpp>

// rogue
#include <M3DUtil/InfectiousStrings.hpp>

// TODO: figure out the odr violations with this symbol
const char* cNpcPartsNameRootJoint = "__ROOT_JOINT__";
const char* cPeachPartsTextureName = "H_peach_main_dummy";
const char* cPeachHostTextureName  = "H_peach_main_s3tc";

void SetMActorAnmFrame(MActor* actor, f32 frame, bool set_bck, bool set_btp)
{
	if (actor == nullptr)
		return;

	J3DFrameCtrl* ctrl;

	if (set_bck) {
		ctrl = actor->getFrameCtrl(ANM_TYPE_BCK);
		if (ctrl)
			ctrl->setFrame(frame);
	}

	if (set_btp) {
		ctrl = actor->getFrameCtrl(ANM_TYPE_BTP);
		if (ctrl)
			ctrl->setFrame(frame);
	}
}

// TODO: 94.2% (84.8% before batch 136 fixed the clearing loop below), frame
// 0x1f8 vs 0x190 and eight instructions still missing. The missing ones are
// one construct: at both defaulted `initSimpleMotionBlend` sites retail emits a
// dead `li r4,-1; cmpwi r4,-1; bne` and only then reads
// `mPtrSaveNormal->mMotionBlendFrame`, i.e. the frame count is a **defaulted
// argument** of a wrapper around MActor::initSimpleMotionBlend that resolves -1
// to the saved value. Spelling that wrapper as a TU-local `static inline
// (MActor*, int frame = -1)` does not reproduce it -- MWCC folds the default
// through one inline level -- so the wrapper is probably reached through a
// second inlined level (a per-part setup helper taking the frame), which is
// also where the remaining 104 low bytes would live.
TNpcParts::TNpcParts(u32 param_1, const J3DGXColorS10* param_2,
                     TBaseNPC* param_3)
    : unk60(param_3)
{
	const TNpcInitInfo* initInfo
	    = SMSGetNpcInitData(unk60->getActorType() - 0x4000001);

	// The clearing loop is a flat 24-trip pointer walk with the increment in
	// the `for`'s third clause: that is the only spelling MWCC unrolls by
	// eight into a `bdnz` with ctr = 3 and no remainder path, as retail does.
	// `*slot++ = nullptr` as the body, `slot[i] = nullptr`, and a nested
	// 2x12 walk are all different (82.0%, 82.0%, 84.8%); a pointer-compare
	// `while` is 90.5%.
	TSharedParts** slot = unk0[0];
	for (int i = 0; i < 24; ++i, ++slot)
		*slot = nullptr;

	for (int i = 0; i < 12; ++i) {
		const TNpcModelData* iVar10 = initInfo->unk4[i];
		if (iVar10 == nullptr || !(param_1 & (1 << i)))
			continue;

		u32 param3 = (&param_2->color.r)[iVar10->unk28];

		const GXColor* param4 = nullptr;
		if (iVar10->unk2A)
			param4 = unk60->getPtrInitPollutionColor();

		for (int j = 0; j < 2; ++j) {
			if (j >= unk60->getManager()->unk28)
				break;

			const char* puVar3 = initInfo->unk4[i]->unk8[j];
			if (puVar3 == nullptr)
				continue;

			int iVar6;
			if (strcmp(initInfo->unk4[i]->unk0[j], cNpcPartsNameRootJoint)
			    == 0) {
				iVar6 = -1;
			} else {
				iVar6 = unk60->mMActorKeeper->getMActor(j)
				            ->getModel()
				            ->getModelData()
				            ->getJointName()
				            ->getIndex(initInfo->unk4[i]->unk0[j]);
			}

			TNPCManager* manager    = (TNPCManager*)unk60->getManager();
			SDLModelData* modelData = manager->getPartsSDLModelData(puVar3);
			unk0[j][i] = new TSharedParts(unk60, iVar6, modelData, 3);
			if (initInfo->unk4[i]->unk2B)
				SMS_UnifyMaterial(unk0[j][i]->getMActor()->getModel());

			switch (unk60->getActorType()) {
			case 0x4000018:
				if (j != 0 || (i != 3 && i != 4)) {
					TSharedParts* parts = unk0[j][i];

					J3DModelData* pJVar17
					    = parts->getMActor()->getModel()->getModelData();
					J3DModelData* pJVar15 = unk60->getModel()->getModelData();

					int uVar9 = pJVar15->getTextureName()->getIndex(
					    cPeachHostTextureName);
					SMS_ChangeTextureAll(
					    pJVar17, cPeachPartsTextureName,
					    *pJVar15->getTexture()->getResTIMG(uVar9));
					parts->getMActor()->initDL();
				}

				if (j == 0) {
					switch (i) {
					case 0:
					case 3:
					case 4:
						int iVar6 = -1;
						if (iVar6 == -1)
							iVar6 = TBaseNPC::mPtrSaveNormal->mMotionBlendFrame
							            .get();
						unk0[j][i]->getMActor()->initSimpleMotionBlend(iVar6);
						break;
					}
				}
				break;

			case 0x4000010:
				if (i == 0 && j == 9)
					unk0[j][i]->getMActor()->initSimpleMotionBlend(20);
				break;

			case 0x4000015:
				if (j == 0 && i == 10) {
					int iVar6 = -1;
					if (iVar6 == -1)
						iVar6
						    = TBaseNPC::mPtrSaveNormal->mMotionBlendFrame.get();
					unk0[j][i]->getMActor()->initSimpleMotionBlend(iVar6);
				}
				break;
			}

			for (int k = 0; k < 3; ++k) {
				const TColorChangeInfo* ccInfo
				    = initInfo->unk4[i]->unk10[k].unk0;
				if (ccInfo != nullptr)
					SMS_InitChangeNpcColor(unk0[j][i]->getMActor(), ccInfo,
					                       param3, param4);
			}

			if (param4 != nullptr) {
				J3DModel* pJVar18     = unk0[j][i]->getMActor()->getModel();
				J3DModelData* pJVar15 = pJVar18->getModelData();
				u16 matNum            = pJVar15->getMaterialNum();
				for (u16 k = 0; k < matNum; ++k) {
					int shapeIdx = pJVar15->getMaterialNodePointer(k)
					                   ->getShape()
					                   ->getIndex();
					if (!pJVar18->getShapePacket(shapeIdx)->getUserArea()) {
						SMS_InitPacket_OneTevKColor(pJVar18, k, GX_KCOLOR0,
						                            param4);
					}
				}
			}

			unk0[j][i]->getMActor()->setLightType(LIGHT_TYPE_OBJECT);
		}
	}
}

void TNpcParts::addJellyFishParts(f32 param_1)
{
	TSharedParts** slot = &unk0[0][11];

	int iVar2 = gpMareJellyFishManager->getModelDataKeeper()->getModelDataNum();
	f32 fVar1 = MsRandF() * iVar2;
	int iVar3 = fVar1;

	SDLModelData* data
	    = gpMareJellyFishManager->getModelDataKeeper()->getNthData(iVar3);

	SDLModel* model = new SDLModel(data, 0, 1);
	MActor* actor   = new MActor(gpMareJellyFishManager->getMActorAnmData());
	actor->setModel(model, 0);

	*slot = new TSharedParts(unk60, -1, actor);

	actor->setBckFromIndex(0);
	actor->setBrkFromIndex(iVar3);
	actor->getFrameCtrl(ANM_TYPE_BCK)->setFrame(param_1);
	actor->getFrameCtrl(ANM_TYPE_BRK)->setFrame(param_1);
	actor->setLightType(LIGHT_TYPE_INDIRECT);
}

void TNpcParts::setPartsAnmFrame(f32 param_1)
{
	switch (unk60->getActorType()) {
	case 0x4000010:
		SetMActorAnmFrame(getPartsMActor(9, 0), param_1, true, false);
		break;

	case 0x4000015:
		SetMActorAnmFrame(getPartsMActor(10, 0), param_1, true, true);
		break;

	case 0x4000018:
		SetMActorAnmFrame(getPartsMActor(0, 0), param_1, true, false);
		SetMActorAnmFrame(getPartsMActor(3, 0), param_1, true, false);
		SetMActorAnmFrame(getPartsMActor(4, 0), param_1, true, false);
		break;
	}
}

MActor* TNpcParts::getPartsMActor(int param_1, int param_2)
{
	MActor* result = nullptr;
	if (unk0[param_2][param_1])
		result = unk0[param_2][param_1]->getMActor();
	return result;
}

void TNpcParts::partsFrameUpdate()
{
	int i = 0;

	TLodAnm* lodAnm   = unk60->getLodAnm();
	int lod           = lodAnm->unk8;
	TSharedParts** it = unk0[lod];

	for (; i < 12; i++, ++it)
		if (*it) {
			MActor* mactor = (*it)->getMActor();
			mactor->frameUpdate();
		}
}

void TNpcParts::partsPerform(u32 param_1, JDrama::TGraphics* param_2)
{
	int i = 0;

	TSharedParts** it = unk0[unk60->getLodAnm()->unk8];

	for (; i < 12; ++i, ++it) {
		if (*it == nullptr)
			continue;

		if (unk60->getActorType() == 0x4000018) {
			// Peach stuff
			bool r4 = true;
			if (unk60->checkUnk1D8(TBaseNPC::UNK1D8_FLAG_UNK4)) {
				switch (i) {
				case 1:
				case 2:
				case 4:
					r4 = false;
					break;
				}
			} else if (unk60->checkUnk1D8(TBaseNPC::UNK1D8_FLAG_UNK1)) {
				switch (i) {
				case 1:
				case 2:
					r4 = false;
					break;
				}
			} else {
				switch (i) {
				case 4:
				case 5:
				case 6:
					r4 = false;
					break;
				}
			}

			if (!r4)
				continue;
		}

		if (param_1 & 2) {
			if (unk60->isJellyFishMare() && i == 11) {
				MActor* mactor = (*it)->getMActor();
				// TODO: still 40 bytes of frame short of the ROM after
				// the 4x4 fix (0xd0 vs 0xf8).
				Mtx44 mtx;
				SMS_GetLightPerspectiveForEffectMtx(mtx);
				J3DModelData* data = mactor->getModel()->getModelData();
				int starglowMatIdx
				    = data->getMaterialName()->getIndex("_starglow1");
				int matNum = data->getMaterialNum();
				for (u16 j = 0; j < matNum; ++j) {
					if (j != starglowMatIdx)
						data->getMaterialNodePointer(j)
						    ->getTexGenBlock()
						    ->getTexMtx(0)
						    ->setEffectMtx(mtx);
				}
			}
		}

		(*it)->perform(param_1, param_2);
	}
}
