#include <Camera/Camera.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DTevStage.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JGadget/std-list.hpp>
#include <JSystem/JGeometry/JGVec2.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <M3DUtil/SampleCtrlModel.hpp>
#include <M3DUtil/SampleCtrlNode.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <MoveBG/ModelGate.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/LiveActor.hpp>
#include <System/FlagManager.hpp>
#include <System/Particles.hpp>
#include <THPPlayer/THPPlayer.h>
#include <stdio.h>
#include <string.h>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static const char* gateMActorNames[] = {
	"05_gate01",      "05_gate02rico", "05_gate03manma",
	"05_gate04monte", "05_gate05mare",
};

void TModelGate::loadAfter()
{
	static const char* gateNames[] = {
		"Gate",        "GateToRicco", "GateToMamma",
		"GateToMonte", "GateToMare",  nullptr,
	};
	initHitActor(ACTOR_TYPE_MODEL_GATE, 5, HIT_CATEGORY_PLAYER, 300.0f, 400.0f,
	             300.0f, 400.0f);
	onHitFilter(HIT_FILTER_NO_COLLISION);
	unk70 = 0;
	unk71 = 0;
	for (u8 i = 0; i < 5; ++i) {
		if (strcmp(gateNames[i], getName()) == 0) {
			unk71 = i;
			break;
		}
	}

	char modelName[0x100];
	u32 modelFlags = 0x11100000;
	snprintf(modelName, sizeof(modelName), "/scene/map/map/gate/%s.bmd",
	         gateMActorNames[unk71]);
	unk78 = SMS_MakeMActor("/scene/map/map/gate", modelName, 0, modelFlags);
	unk72
	    = unk78->getModel()->getModelData()->getJointName()->getIndex("center");
	if (ActivePlayer.open != 0) {
		THPVideoInfo videoInfo;
		THPPlayerGetVideoInfo(&videoInfo);
		u16 width            = videoInfo.xSize;
		u16 height           = videoInfo.ySize;
		J3DTexture* textures = unk78->getModel()->getModelData()->getTexture();
		ResTIMG* image       = textures->getResTIMG(0);
		image->format        = GX_TF_I8;
		image->width         = width;
		image->height        = height;
		image                = textures->getResTIMG(1);
		image->format        = GX_TF_I8;
		image->width         = width >> 1;
		image->height        = height >> 1;
		image                = textures->getResTIMG(2);
		image->format        = GX_TF_I8;
		image->width         = width >> 1;
		image->height        = height >> 1;
	}

	unkB8 = 0;
	unkB9 = 0;
	unkBA = 0;
	unkBE = 0xF0;
	unkBC = unkBE;
	unk78->setBtk(gateMActorNames[unk71]);
	unk78->setBrk(gateMActorNames[unk71]);
	unk78->getFrameCtrl(ANM_TYPE_BRK)->setRate(0.0f);
	unkC0 = new SampleCtrlModelData(unk78->getModel()->getModelData());
	unkC5 = 0x20;
	unkC6 = 0xFF;
	unkC4 = STATE_UNK1;
	unkC8 = 0x168;
	unkCA = 0;
	unkCC = 0;
	unkCE = 0x3C;
	unkD0 = 0.0f;
	unkD4 = 0.1f;
	unkD8 = 0.02f;
	unkDC = 0.025f;
	mScaling.setAll(1.0f);
	static_cast<JDrama::TViewObjPtrListT<THitActor>*>(
	    JDrama::TNameRefGen::search("マップグループ"))
	    ->getChildren()
	    .push_back(this);

	Mtx mtx;
	SMS_GetActorMtx(*this, mtx);
	MTXCopy(mtx, unk78->getModel()->getBaseTRMtx());
	unk78->getModel()->calc();
	MTXTrans(mtx, 0.0f, 0.0f, 250.0f);
	MTXConcat(unk78->getModel()->getAnmMtx(unk72), mtx, mtx);
	unkAC.set(0.0f, 0.0f, 0.0f);
	MTXMultVec(mtx, &unkAC, &unkAC);
	MTXInverse(unk78->getModel()->getAnmMtx(unk72), unk7C);
	unk74  = matan(unk78->getModel()->getAnmMtx(unk72)[2][2],
	               unk78->getModel()->getAnmMtx(unk72)[0][2]);
	unkE0  = 0;
	unkE4  = 0.0f;
	unkE8  = 0.01f;
	unkEC  = 0.02f;
	unkF0  = 500.0f;
	unkF4  = 1000.0f;
	unkF8  = 0.7f;
	unkFC  = 0.0f;
	unk100 = 200.0f;
	unk104 = 150.0f;
	unk108 = -1000.0f;
	unk10C = 150.0f;
	unk110 = 0.0f;
	unk114 = 300.0f;
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_body.jpa",
	                 MAP_MAP_GATE_MS_MARIOWP_BODY);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_head.jpa",
	                 MAP_MAP_GATE_MS_MARIOWP_HEAD);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_cap.jpa",
	                 MAP_MAP_GATE_MS_MARIOWP_CAP);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_rhand.jpa",
	                 MAP_MAP_GATE_MS_MARIOWP_RHAND);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_lhand.jpa",
	                 MAP_MAP_GATE_MS_MARIOWP_LHAND);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_rleg.jpa",
	                 MAP_MAP_GATE_MS_MARIOWP_RLEG);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_rfoot.jpa",
	                 MAP_MAP_GATE_MS_MARIOWP_RFOOT);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_lleg.jpa",
	                 MAP_MAP_GATE_MS_MARIOWP_LLEG);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_lfoot.jpa",
	                 MAP_MAP_GATE_MS_MARIOWP_LFOOT);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_watgun.jpa",
	                 MAP_MAP_GATE_MS_MARIOWP_WATGUN);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_dust.jpa",
	                 MAP_MAP_GATE_MS_MARIOWP_DUST);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_senko.jpa",
	                 MAP_MAP_GATE_MS_MARIOWP_SENKO);
	SMS_LoadParticle("/scene/map/map/gate/ms_gatewind_a.jpa",
	                 MAP_MAP_GATE_MS_GATEWIND_A);
	SMS_LoadParticle("/scene/map/map/gate/ms_gatewind_a2.jpa",
	                 MAP_MAP_GATE_MS_GATEWIND_A2);
	SMS_LoadParticle("/scene/map/map/gate/ms_gatewind_a3.jpa",
	                 MAP_MAP_GATE_MS_GATEWIND_A3);
	SMS_LoadParticle("/scene/map/map/gate/ms_gatewind_b.jpa",
	                 MAP_MAP_GATE_MS_GATEWIND_B);
	SMS_LoadParticle("/scene/map/map/gate/ms_gatehit_a.jpa",
	                 MAP_MAP_GATE_MS_GATEHIT_A);
	SMS_LoadParticle("/scene/map/map/gate/ms_gatehit_b.jpa",
	                 MAP_MAP_GATE_MS_GATEHIT_B);
	bool opened = false;
	switch (unk71) {
	case 0:
		opened = TFlagManager::getInstance()->getBool(MSF_M_PAINTED_ON_STATUE);
		break;
	case 1:
		opened
		    = TFlagManager::getInstance()->getBool(MSF_M_PAINTED_ON_BOATHOUSE);
		break;
	case 2:
		opened
		    = TFlagManager::getInstance()->getBool(MSF_M_PAINTED_ON_LIGHTHOUSE);
		break;
	case 3:
		opened
		    = TFlagManager::getInstance()->getBool(MSF_M_PAINTED_ON_LIGHTHOUSE);
		break;
	case 4:
		opened
		    = TFlagManager::getInstance()->getBool(MSF_M_PAINTED_ON_LIGHTHOUSE);
		break;
	}
	if (opened == true) {
		unk70 |= 1;
		offHitFilter(HIT_FILTER_NO_COLLISION);
	} else {
		unk70 &= ~1;
		unk70 |= 2;
		onHitFilter(HIT_FILTER_NO_COLLISION);
	}
}

void TModelGate::startOpen()
{
	unk70 |= 1;
	unkC4 = STATE_UNK0;
	unk78->setBpk(gateMActorNames[unk71]);
	offHitFilter(HIT_FILTER_NO_COLLISION);
	unk70 |= 2;
}

void TModelGate::screenBlur(JDrama::TGraphics* graphics)
{
	JGeometry::TVec3<f32> viewDirection;
	JGeometry::TVec3<f32> direction;
	JGeometry::TVec3<f32> position;

	direction.x = SMS_GetMarioX() - mPosition.x;
	direction.y = 0.0f;
	direction.z = SMS_GetMarioZ() - mPosition.z;
	VECNormalize(&direction, &direction);
	MTXMultVecSR(graphics->getViewMtx(), &direction, &viewDirection);

	JGeometry::TVec3<f32> marioPosition = SMS_GetMarioPos();
	MTXMultVec(unk7C, &marioPosition, &position);

	f32 factor = 0.0f;
	if (-250.0f <= position.x && position.x <= 250.0f && -250.0f <= position.y
	    && position.y <= 250.0f && 0.0f <= position.z && position.z <= unkF4) {
		if (position.z < unkF0)
			factor = 1.0f;
		if (unkF0 <= position.z && position.z <= unkF4)
			factor = 1.0f - (position.z - unkF0) / (unkF4 - unkF0);
		if (unkF4 < position.z)
			factor = 0.0f;
	}

	f32 blurRate = (f32)unkE0 * factor;
	s16 angle    = (s16)(DEG2SHORTANGLE(mRotation.y) - gpCamera->getUnk258());
	if (angle < -0x2AAA || angle > 0x2AAA)
		blurRate = 0.0f;

	unkE4 += unkE8 * (blurRate - unkE4);
	u8 blurAlpha = (u8)(unkE4 * (1.0f - gpCamera->unk270));

	gpAfterEffect->unk15 = 2;
	gpAfterEffect->unk1C = blurAlpha;
	gpAfterEffect->unk50 = unkEC;
	gpAfterEffect->unk5C = viewDirection.x;
	gpAfterEffect->unk60 = viewDirection.y;
	gpAfterEffect->unk64 = viewDirection.z;
}

BOOL TModelGate::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->getActorType() == ACTOR_TYPE_MARIO
	    && message == HIT_MESSAGE_ATTACK) {
		unkC8 = 0;
		unkC4 = STATE_UNK2;
		return TRUE;
	}
	if (sender->getActorType() == ACTOR_TYPE_WATER) {
		Vec pos;
		MTXMultVec(unk7C, &sender->mPosition, &pos);

		Mtx mtx;
		MTXCopy(unk78->getModel()->getAnmMtx(unk72), mtx);
		f32 dist = (pos.x * pos.x) + (pos.y * pos.y);

		if (dist < 40000.0f && -100.0f < pos.z && pos.z < unkFC) {
			if (unk70 & 2) {
				unkD0 += unkD4;
				if (unkD0 > 1.0f) {
					unkCA = unkC8;
					unkD0 = 1.0f;
					unk70 &= ~2;
				}
			}
			if (MsRandF() < unkF8) {
				gpMarioParticleManager->emitWithRotate(
				    MAP_MAP_GATE_MS_GATEHIT_A, &sender->mPosition, 0, unk74, 0,
				    2, nullptr);
				gpMarioParticleManager->emitWithRotate(
				    MAP_MAP_GATE_MS_GATEHIT_B, &sender->mPosition, 0, unk74, 0,
				    2, nullptr);
			}
			return TRUE;
		}
	}
	return FALSE;
}

void TModelGate::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!(unk70 & 1))
		return;

	if ((param_1 & 8) && ActivePlayer.open != 0) {
		THPTextureSet* textureSet = ActivePlayer.dispTextureSet;
		if (textureSet != nullptr) {
			J3DTexture* textures
			    = unk78->getModel()->getModelData()->getTexture();
			ResTIMG* image = textures->getResTIMG(0);
			image->imageDataOffset
			    = (uintptr_t)textureSet->ytexture - (uintptr_t)image;
			image = textures->getResTIMG(1);
			image->imageDataOffset
			    = (uintptr_t)textureSet->utexture - (uintptr_t)image;
			image = textures->getResTIMG(2);
			image->imageDataOffset
			    = (uintptr_t)textureSet->vtexture - (uintptr_t)image;
		}
	}
	unk78->perform(param_1, param_2);
	if (param_1 & 1) {
		if (!(unk70 & 2)) {
			if (SMS_DistanceFromMario(mPosition) < 1000.0f) {
				unkD0 += 0.01f;
				if (unkD0 > 1.0f) {
					unkD0 = 1.0f;
					unkCA = unkC8;
				}
			} else {
				--unkCA;
				if (unkCA <= 0)
					unkCA = 0;
				unkD0 = unkCA / 1000.0f;
			}
		}
		JGeometry::TVec3<f32> localPos;
		MTXMultVec(unk7C, &SMS_GetMarioPos(), &localPos);
		if (-unk104 < localPos.x && localPos.x < unk104 && unk108 < localPos.y
		    && localPos.y < unk10C && unk110 < localPos.z
		    && localPos.z < unk114) {
			if (unkCA > 0) {
				if (SMS_IsMarioStatusTypeJumping()
				    && SMS_GetMarioLiveActor()->receiveMessage(this,
				                                               HIT_MESSAGE_TAKE)
				           == TRUE)
					mHeldObject = SMS_GetMarioLiveActor();
			} else {
				JGeometry::TVec2<f32> direction(SMS_GetMarioX() - mPosition.x,
				                                SMS_GetMarioZ() - mPosition.z);
				f32 distance = direction.length();
				if (distance < unk100) {
					f32 nx                    = direction.x / distance;
					f32 nz                    = direction.y / distance;
					JGeometry::TVec3<f32> pos = SMS_GetMarioPos();
					pos.x += 10.0f * nx;
					pos.z += 10.0f * nz;
					SMS_MarioMoveRequest(pos);
				}
			}
		}
	}
	if (param_1 & 2) {
		switch (unkC4) {
		case STATE_UNK0:
			if (unk78->getFrameCtrl(ANM_TYPE_BPK)
			        ->checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE
			                     | J3DFrameCtrl::STATE_LOOPED_ONCE))
				unkC4 = STATE_UNK1;
			break;
		case STATE_UNK1:
			if (unkCA > 0) {
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    MAP_MAP_GATE_MS_GATEWIND_A,
				    unk78->getModel()->getAnmMtx(unk72), 1, this);
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    MAP_MAP_GATE_MS_GATEWIND_A2,
				    unk78->getModel()->getAnmMtx(unk72), 1, this);
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    MAP_MAP_GATE_MS_GATEWIND_A3,
				    unk78->getModel()->getAnmMtx(unk72), 1, this);
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    MAP_MAP_GATE_MS_GATEWIND_B,
				    unk78->getModel()->getAnmMtx(unk72), 1, this);
				--unkCA;
				++unkCC;
				SMSGetMSound()->startSoundActor(MSD_SE_OBJ_GATE_LIGHT,
				                                &mPosition, 0, nullptr, 0, 4);
				SMSGetMSound()->startSoundActor(MSD_SE_OBJ_GATE_WAVE,
				                                &mPosition, 0, nullptr, 0, 4);
			} else {
				unkCA = 0;
				unkCC = 0;
				unkD0 -= unkD8;
				if (unkD0 < 0.0f)
					unkD0 = 0.0f;
			}
			break;
		case STATE_UNK2:
		default:
			unkD0 -= unkDC;
			break;
		}
		if (unkD0 > 1.0f)
			unkD0 = 1.0f;
		if (unkD0 < 0.0f)
			unkD0 = 0.0f;

		J3DTevBlock* tevBlock = unk78->getModel()
		                            ->getModelData()
		                            ->getMaterialNodePointer(0)
		                            ->getTevBlock();
		if (unkB8 == 1) {
			unkBA = unkB9;
			--unkBC;
			if (unkBC == 0) {
				++unkB9;
				unkBC = unkBE;
				if (unkB9 >= 8)
					unkB9 = 0;
			}
			SampleCtrlMaterial* material = unkC0->mMaterials[0];
			J3DTevStageInfo& stage0      = material->unk3C[0];
			J3DTevStageInfo& stage2      = material->unk3C[2];
			J3DTevStageInfo& stage3      = material->unk3C[3];
			J3DTevStageInfo& stage5      = material->unk3C[5];
			stage0.field_0x5             = GX_TEV_ADD;
			stage2.field_0x5             = GX_TEV_ADD;
			stage3.field_0x5             = GX_TEV_ADD;
			stage3.field_0x6             = GX_TB_ZERO;
			stage3.field_0x7             = GX_CS_SCALE_1;
			stage3.field_0x8             = GX_TRUE;
			stage5.field_0x11            = GX_TRUE;
			switch (unkB9) {
			case 0:
				break;
			case 1:
				stage3.field_0x5 = GX_TEV_SUB;
				stage3.field_0x8 = GX_FALSE;
				break;
			case 2:
				stage0.field_0x5 = GX_TEV_COMP_R8_GT;
				break;
			case 3:
				stage3.field_0x5 = GX_TEV_COMP_R8_GT;
				break;
			case 4:
				stage3.field_0x7 = GX_CS_SCALE_2;
				break;
			case 5:
				stage5.field_0x11 = GX_FALSE;
				break;
			case 6:
				stage2.field_0x5 = GX_TEV_SUB;
				break;
			case 7:
				stage3.field_0x5 = GX_TEV_ADD;
				stage3.field_0x6 = GX_TB_ADDHALF;
				stage3.field_0x8 = GX_FALSE;
				break;
			}
			tevBlock->getTevStage(0)->setTevStageInfo(stage0);
			tevBlock->getTevStage(2)->setTevStageInfo(stage2);
			tevBlock->getTevStage(3)->setTevStageInfo(stage3);
			tevBlock->getTevStage(5)->setTevStageInfo(stage5);
		}
		f32 frame = unkD0 * unk78->getFrameCtrl(ANM_TYPE_BRK)->getEnd();
		unk78->getFrameCtrl(ANM_TYPE_BRK)->setFrame(frame);
	}
	if (param_1 & 4)
		screenBlur(param_2);
	THitActor::perform(param_1, param_2);
}
