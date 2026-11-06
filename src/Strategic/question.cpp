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
	mFlags          = 0;
	mRequestNum     = 0;
	mRequestClipFar = 5000.0;
	ResTIMG* questionImg
	    = (ResTIMG*)JKRGetResource("/common/timg/question.bti");
	unk18        = new JUTTexture(questionImg);
	mRequests    = new TQuestionRequest[0x20];
	mDisplayList = new TDLTexQuad;
	mDisplayList->createBuffer(0x20);
	gpQuestionManager = this;
}

bool TQuestionManager::request(JGeometry::TVec3<f32> param_1, f32 param_2)
{
	if (mRequestNum < 0x20) {
		f32 dx = SMS_GetMarioPos().x - param_1.x;
		f32 dz = SMS_GetMarioPos().z - param_1.z;
		if (dx * dx + dz * dz < mRequestClipFar * mRequestClipFar) {
			mRequests[mRequestNum].mPosition = param_1;
			mRequests[mRequestNum].mRadius   = param_2;
			++mRequestNum;
			return true;
		}
	}

	return false;
}

void TQuestionManager::makeDL(JDrama::TGraphics* graphics) const
{
	for (int i = 0; i < mRequestNum; ++i) {
		TQuestionRequest& req          = mRequests[i];
		JGeometry::TVec3<f32> worldPos = req.mPosition;
		f32 rad                        = req.mRadius;
		worldPos.y += rad;
		JGeometry::TVec3<f32> screenPos;
		MTXMultVec(graphics->mViewMtx, &worldPos, &screenPos);
		JGeometry::TVec3<f32> v3[4];
		v3[0].set(screenPos.x - rad, screenPos.y + rad, screenPos.z + rad);
		v3[1].set(screenPos.x + rad, screenPos.y + rad, screenPos.z + rad);
		v3[2].set(screenPos.x + rad, screenPos.y - rad, screenPos.z + rad);
		v3[3].set(screenPos.x - rad, screenPos.y - rad, screenPos.z + rad);
		mDisplayList->request(v3);
	}
	mDisplayList->setEnd();
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
	mDisplayList->draw();
	GXSetClipMode(GX_CLIP_ENABLE);
}

void TQuestionManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_VIEW) {
		if (gpSilhouetteManager->isUnk48Positive()) {
			mDisplayList->reset();
			makeDL(graphics);
			mFlags |= FLAG_UNK2;
		} else {
			mFlags &= ~FLAG_UNK2;
		}
		mRequestNum = 0;
	}

	if (cue & CUE_DRAW && gpSilhouetteManager->isUnk48Positive()
	    && mFlags & FLAG_UNK2) {
		draw();
	}
}
