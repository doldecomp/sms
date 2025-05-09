#include <Map/Sky.hpp>
#include <Map/Map.hpp>
#include <Map/MapModel.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <Camera/Camera.hpp>
#include <System/MarDirector.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TSky::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 2) {
		Mtx local_4c;
		MTXInverse(gpCamera->unk1EC, local_4c);

		Mtx afStack_7c;
		MTXIdentity(afStack_7c);

		// TODO: match this awfulness
		afStack_7c[0][3] = -(local_4c[0][2] * gpCamera->unk1EC[2][3]
		                     - (-local_4c[0][0] * gpCamera->unk1EC[0][3]
		                        - local_4c[0][1] * gpCamera->unk1EC[1][3]));
		afStack_7c[1][3] = -(local_4c[1][2] * gpCamera->unk1EC[2][3]
		                     - (-local_4c[1][0] * gpCamera->unk1EC[0][3]
		                        - local_4c[1][1] * gpCamera->unk1EC[1][3]));
		afStack_7c[2][3] = -(local_4c[2][2] * gpCamera->unk1EC[2][3]
		                     - (-local_4c[2][0] * gpCamera->unk1EC[0][3]
		                        - local_4c[2][1] * gpCamera->unk1EC[1][3]));

		if (gpMarDirector->map == 15) {
			Mtx local_ac;
			f32 fVar2 = sinf(unk48 * 0.017453294f);
			f32 fVar3 = cosf(unk48 * 0.017453294f);

			local_ac[0][0] = fVar3;
			local_ac[2][0] = -fVar2;
			local_ac[0][1] = 0.0f;
			local_ac[0][3] = 0.0f;
			local_ac[1][0] = 0.0f;
			local_ac[1][1] = 1.0f;
			local_ac[1][2] = 0.0f;
			local_ac[1][3] = 0.0f;
			local_ac[2][1] = 0.0f;
			local_ac[2][3] = 0.0f;
			local_ac[0][2] = fVar2;
			local_ac[2][2] = fVar3;
			MTXConcat(local_ac, afStack_7c, afStack_7c);
			unk48 += unk4C;
			unk48 = MsWrap<f32>(unk48, 0.0f, 360.0f);
		}
		unk44->getUnk4()->setBaseTRMtx(afStack_7c);
	}
	unk44->perform(param_1, param_2);
	if ((param_1 & 8) != 0) {
		GXClearVtxDesc();
		GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
		GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
		GXSetCullMode(GX_CULL_FRONT);
		GXSetNumChans(1);
		GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 1,
		              GX_DF_CLAMP, GX_AF_NONE);
		GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0,
		              GX_DF_NONE, GX_AF_NONE);
		GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0x0, 0x12, 0xEE, 0x80 });
		GXSetNumTexGens(0);
		GXSetCurrentMtx(0);
		Mtx afStack_dc;
		MTXScale(afStack_dc, 100000.0f, 100000.0f, 100000.0f);
		GXLoadPosMtxImm(afStack_dc, 0);
		GXLoadNrmMtxImm(afStack_dc, 0);
		GXSetNumTevStages(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR0A0);
		GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
		GXSetZCompLoc(1);
		GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
		GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
		GXDrawSphere(8, 0x10);
	}
}

void TSky::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);
	unk44 = SMS_MakeMActorWithAnmData(
	    "/scene/map/map/sky.bmd", gpMap->getModelManager()->getMActorAnmData(),
	    3, 0x10220000);

	if (gpMapObjManager->unk68) {
		unk44->getUnk4()->getModelData()->setMaterialTable(
		    gpMapObjManager->getUnk68(), J3DMatCopyFlag_All);
		unk44->initDL();
	}

	if (gpMarDirector->map != 15)
		TMapObjBase::startAllAnim(unk44, "sky");
}

TSky::TSky(const char* name)
    : JDrama::TActor(name)
    , unk44(nullptr)
    , unk48(-30.0f)
    , unk4C(0.035f)
{
}
