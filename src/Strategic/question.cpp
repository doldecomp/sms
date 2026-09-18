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
	if (unk12 < 0x20) {
		f32 dx = gpMarioPos->x - param_1.x;
		f32 dz = gpMarioPos->z - param_1.z;
		if (dx * dx + dz * dz < unk14 * unk14) {
			unk1C[unk12].unk0 = param_1;
			unk1C[unk12].unkC = param_2;
			++unk12;
			return true;
		}
	}

	return false;
}

void TQuestionManager::makeDL(JDrama::TGraphics* param_1) const
{
	JGeometry::TVec3<f32> vtx[4];

	for (int i = 0; i < getRequestNum(); ++i) {
		JGeometry::TVec3<f32> viewPos;
		TQuestionRequest& req     = unk1C[i];
		JGeometry::TVec3<f32> pos = req.unk0;
		f32 size                  = req.unkC;
		pos.y += size;
		MTXMultVec(param_1->mViewMtx, &pos, &viewPos);
		vtx[0].set(viewPos.x - size, viewPos.y + size, viewPos.z + size);
		vtx[1].set(viewPos.x + size, viewPos.y + size, viewPos.z + size);
		vtx[2].set(viewPos.x + size, viewPos.y - size, viewPos.z + size);
		vtx[3].set(viewPos.x - size, viewPos.y - size, viewPos.z + size);
		unk20->request(vtx);
	}
	unk20->setEnd();
}

void TQuestionManager::draw() const
{
	Mtx mtx;

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
	GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);

	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U8, 7);

	MTXIdentity(mtx);
	GXSetCurrentMtx(GX_PNMTX0);
	GXLoadPosMtxImm(mtx, GX_PNMTX0);
	GXLoadNrmMtxImm(mtx, GX_PNMTX0);
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

void TQuestionManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if ((cue & CUE_CALC_VIEW) != 0) {
		bool silhouetteOn = gpSilhouetteManager->isUnk48Positive();
		if (silhouetteOn) {
			unk20->reset();
			makeDL(graphics);
			unk10 |= 2;
		} else {
			unk10 &= ~2;
		}
		unk12 = 0;
	}

	if ((cue & CUE_DRAW) != 0) {
		bool silhouetteOn = gpSilhouetteManager->isUnk48Positive();
		if (silhouetteOn && (unk10 & 2) != 0)
			draw();
	}
}
