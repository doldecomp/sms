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

void SetMActorAnmFrame(MActor* param_1, f32 param_2, bool param_3, bool param_4)
{
}

TNpcParts::TNpcParts(u32 param_1, const J3DGXColorS10* param_2,
                     TBaseNPC* param_3)
    : unk60(param_3)
{
	const TNpcInitInfo* initInfo
	    = SMSGetNpcInitData(unk60->getActorType() - 0x4000001);

	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 12; ++j)
			unk0[i][j] = nullptr;

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

			const TNpcModelData* puVar6 = &initInfo->unk4[i][j];
			const char* puVar3          = puVar6->unk8[0];
			if (puVar3 == nullptr)
				continue;

			int iVar6 = strcmp(puVar6->unk0, cNpcPartsNameRootJoint) == 0
			                ? -1
			                : unk60->mMActorKeeper->getMActor(j)
			                      ->getModel()
			                      ->getModelData()
			                      ->getJointName()
			                      ->getIndex(initInfo->unk4[i + j]->unk0);

			TNPCManager* manager    = (TNPCManager*)unk60->getManager();
			SDLModelData* modelData = manager->getPartsSDLModelData(puVar3);
			unk0[i][j] = new TSharedParts(unk60, iVar6, modelData, 3);
			if (initInfo->unk4[j]->unk2B)
				SMS_UnifyMaterial(unk0[i][j]->getMActor()->getModel());

			switch (unk60->getActorType()) {
			case 0x4000018:
				if (j != 0 || (i != 3 && i != 4)) {
					TSharedParts* parts = unk0[i][j];

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
						unk0[i][j]->getMActor()->initSimpleMotionBlend(iVar6);
						break;
					}
				}
				break;

			case 0x4000010:
				if (i == 0 && j == 9)
					unk0[i][j]->getMActor()->initSimpleMotionBlend(20);
				break;

			case 0x4000015:
				if (j == 0 && i == 10) {
					int iVar6 = -1;
					if (iVar6 == -1)
						iVar6
						    = TBaseNPC::mPtrSaveNormal->mMotionBlendFrame.get();
					unk0[i][j]->getMActor()->initSimpleMotionBlend(iVar6);
				}
				break;
			}

			for (int k = 0; k < 3; ++k) {
				const TColorChangeInfo* ccInfo
				    = initInfo->unk4[i][j].unk10[k].unk0;
				if (ccInfo != nullptr)
					SMS_InitChangeNpcColor(unk0[i][j]->getMActor(), ccInfo,
					                       param3, param4);
			}

			if (param4 != nullptr) {
				J3DModel* pJVar18     = unk0[i][j]->getMActor()->getModel();
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

			unk0[i][j]->getMActor()->setLightType(1);
		}
	}
}

void TNpcParts::addJellyFishParts(f32 param_1)
{
	TSharedParts** slot = &unk0[5][1];

	int iVar2 = gpMareJellyFishManager->getModelDataKeeper()->getModelDataNum();
	int iVar3 = MsRandF() * iVar2;

	SDLModelData* data
	    = gpMareJellyFishManager->getModelDataKeeper()->getNthData(iVar3);

	SDLModel* model = new SDLModel(data, 0, 1);
	MActor* actor   = new MActor(gpMareJellyFishManager->getMActorAnmData());
	actor->setModel(model, 0);

	*slot = new TSharedParts(unk60, -1, actor);

	actor->setBckFromIndex(0);
	actor->setBrkFromIndex(iVar3);
	actor->getFrameCtrl(0)->setFrame(param_1);
	actor->getFrameCtrl(5)->setFrame(param_1);
	actor->setLightType(3);
}

void TNpcParts::setPartsAnmFrame(f32 param_1)
{
	switch (unk60->getActorType()) {
	case 0x4000010: {
		if (MActor* mactor = getPartsMActor(9, 0))
			if (J3DFrameCtrl* ctrl = mactor->getFrameCtrl(0))
				ctrl->setFrame(param_1);
	} break;

	case 0x4000015: {
		if (MActor* mactor = getPartsMActor(10, 0)) {
			if (J3DFrameCtrl* ctrl = mactor->getFrameCtrl(0))
				ctrl->setFrame(param_1);
			if (J3DFrameCtrl* ctrl = mactor->getFrameCtrl(3))
				ctrl->setFrame(param_1);
		}
	} break;

	case 0x4000018:
		if (MActor* mactor = getPartsMActor(0, 0))
			if (J3DFrameCtrl* ctrl = mactor->getFrameCtrl(0))
				ctrl->setFrame(param_1);
		if (MActor* mactor = getPartsMActor(3, 0))
			if (J3DFrameCtrl* ctrl = mactor->getFrameCtrl(0))
				ctrl->setFrame(param_1);
		if (MActor* mactor = getPartsMActor(4, 0))
			if (J3DFrameCtrl* ctrl = mactor->getFrameCtrl(0))
				ctrl->setFrame(param_1);
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

	TSharedParts** it = unk0[unk60->getLodAnm()->unk8];

	for (; i < 12; i++, ++it)
		if (*it)
			(*it)->getMActor()->frameUpdate();
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
				Mtx mtx;
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
