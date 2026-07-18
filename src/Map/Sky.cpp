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

void TSky::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		MtxPtr mtx = gpCamera->unk1EC;

		Mtx local_EC;
		MTXInverse(mtx, local_EC);

		Mtx local_BC;
		MTXIdentity(local_BC);

		local_BC[0][3] = -local_EC[0][0] * mtx[0][3]
		                 - local_EC[0][1] * mtx[1][3]
		                 - local_EC[0][2] * mtx[2][3];
		local_BC[1][3] = -local_EC[1][0] * mtx[0][3]
		                 - local_EC[1][1] * mtx[1][3]
		                 - local_EC[1][2] * mtx[2][3];
		local_BC[2][3] = -local_EC[2][0] * mtx[0][3]
		                 - local_EC[2][1] * mtx[1][3]
		                 - local_EC[2][2] * mtx[2][3];

		if (SMSGetMarDirector()->getCurrentMap() == 15) {
			Mtx local_8C;
			f32 fVar2 = sinf(unk48 * 0.017453294f);
			f32 fVar3 = cosf(unk48 * 0.017453294f);

			local_8C[0][0] = fVar3;
			local_8C[0][1] = 0.0f;
			local_8C[0][2] = fVar2;
			local_8C[0][3] = 0.0f;

			local_8C[1][0] = 0.0f;
			local_8C[1][1] = 1.0f;
			local_8C[1][2] = 0.0f;
			local_8C[1][3] = 0.0f;

			local_8C[2][0] = -fVar2;
			local_8C[2][1] = 0.0f;
			local_8C[2][2] = fVar3;
			local_8C[2][3] = 0.0f;
			MTXConcat(local_8C, local_BC, local_BC);
			unk48 += unk4C;
			unk48 = MsWrap<f32>(unk48, 0.0f, 360.0f);
		}
		unk44->getModel()->setBaseTRMtx(local_BC);
	}
	unk44->perform(cue, graphics);
	if ((cue & CUE_DRAW) != 0) {
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
		GXSetCurrentMtx(GX_PNMTX0);
		Mtx afStack_dc;
		MTXScale(afStack_dc, 100000.0f, 100000.0f, 100000.0f);
		GXLoadPosMtxImm(afStack_dc, GX_PNMTX0);
		GXLoadNrmMtxImm(afStack_dc, GX_PNMTX0);
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
	    3,
	    J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	        | (2 << J3DMLF_TevStageNumShift));

	if (gpMapObjManager->unk68) {
		unk44->getModel()->getModelData()->setMaterialTable(
		    gpMapObjManager->getUnk68(), J3DMatCopyFlag_All);
		unk44->initDL();
	}

	if (gpMarDirector->mMap != 15)
		TMapObjBase::startAllAnim(unk44, "sky");
}

TSky::TSky(const char* name)
    : JDrama::TActor(name)
    , unk44(nullptr)
    , unk48(-30.0f)
    , unk4C(0.035f)
{
}
