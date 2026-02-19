#include <Strategic/question.hpp>
#include <MarioUtil/DLUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <dolphin/gx.h>

TQuestionManager* gpQuestionManager;

void TQuestionManager::load(JSUMemoryInputStream& param_1)
{
	JDrama::TViewObj::load(param_1);
	unk10           = 0;
	unk12           = 0;
	unk14           = 5000.0;
	ResTIMG* pRVar1 = (ResTIMG*)JKRGetResource("/common/timg/question.bti");
	unk18           = new JUTTexture(pRVar1);
	unk1C           = new TQuestionRequest[0x20];
	unk20           = new TDLTexQuad;
	unk20->createBuffer(0x20);
	gpQuestionManager = this;
}

bool TQuestionManager::request(JGeometry::TVec3<f32> param_1, f32 param_2)
{
	// TODO: inline for horizontal distance?
	if (unk12 < 0x20
	    && (gpMarioPos->x - param_1.x) * (gpMarioPos->x - param_1.x)
	               + (gpMarioPos->z - param_1.z) * (gpMarioPos->z - param_1.z)
	           < unk14 * unk14) {
		unk1C[unk12].unk0 = param_1;
		unk1C[unk12].unkC = param_2;
		++unk12;
		return true;
	}

	return false;
}

#pragma dont_inline on
void TQuestionManager::makeDL(JDrama::TGraphics* param_1) const
{
	for (int i = 0; i < unk12; ++i) {
		TQuestionRequest& req    = unk1C[i];
		JGeometry::TVec3<f32> v1 = req.unk0;
		f32 f                    = req.unkC;
		v1.y += f;
		JGeometry::TVec3<f32> v2;
		MTXMultVec(param_1->mViewMtx, &v1, &v2);
		JGeometry::TVec3<f32> v3(v2.x - f, v2.y + f, v2.z + f);
		unk20->request(&v3);
	}
	unk20->setEnd();
}
#pragma dont_inline off

void TQuestionManager::draw() const
{
	Mtx mtx;

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
	GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);

	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U8, 7);

	MTXIdentity(mtx);
	GXSetCurrentMtx(0);
	GXLoadPosMtxImm(mtx, 0);
	GXLoadNrmMtxImm(mtx, 0);
	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
	GXSetCullMode(GX_CULL_NONE);
	unk18->load(GX_TEXMAP0);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
	GXSetZCompLoc(1);
	GXSetZMode(1, GX_GREATER, 0);
	GXSetColorUpdate(1);
	GXSetAlphaUpdate(0);
	GXSetDstAlpha(0, 0);
	GXSetClipMode(GX_CLIP_DISABLE);
	unk20->draw();
	GXSetClipMode(GX_CLIP_ENABLE);
}

void TQuestionManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if ((param_1 & 4) != 0) {
		if (gpSilhouetteManager->isUnk48Positive()) {
			unk20->reset();
			makeDL(param_2);
			unk10 |= 2;
		} else {
			unk10 &= ~2;
		}
		unk12 = 0;
	}

	if ((param_1 & 8) != 0 && gpSilhouetteManager->isUnk48Positive()
	    && (unk10 & 2) != 0) {
		draw();
	}
}
