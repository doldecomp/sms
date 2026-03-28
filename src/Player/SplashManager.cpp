#include <Player/SplashManager.hpp>
#include <dolphin/gx.h>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <MarioUtil/DLUtil.hpp>
#include <System/StageUtil.hpp>
#include <System/Application.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TSplashManager* gpSplashManager;

void TSplashManager::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	unk10 = 3;
	const ResTIMG* splashTex
	    = (const ResTIMG*)JKRGetResource("/mario/timg/splash.bti");
	unk14  = new JUTTexture(splashTex);
	unk630 = 50.0f;
	unk634 = 100.0f;
	unk638 = (SMSGetAnmFrameRate() * -0.5f) * SMSGetAnmFrameRate();
	unk63C = (GXColor) { 168, 203, 227, 255 };
	unk118.initiate();
	unk124.initiate();
	for (int i = 0; i < 64; ++i) {
		unk18[i] = new JSULink<TWaterSplash>(&unk130[i]);
		unk130[i].unk0.zero();
		unk130[i].unkC  = 0.0f;
		unk130[i].unk10 = 0;
		unk130[i].unk11 = i;
		unk124.append(unk18[i]);
	}

	unk640 = new TDLColorTexQuad;
	unk640->createBuffer(64);
	unk644 = 3000.0f;
	unk648 = 16;

	gpSplashManager = this;
}

void TSplashManager::newSplash(JGeometry::TVec3<f32> param_1, f32 param_2)
{
	if (unk124.getNumLinks() != 0 && !SMS_isDivingMap()) {
		JSULink<TWaterSplash>* link = unk124.getFirst();
		unk124.remove(link);
		unk118.append(link);

		TWaterSplash* splash = link->getObject();

		splash->unk0  = param_1;
		splash->unkC  = param_2;
		splash->unk10 = unk648;
	}
}

void TSplashManager::move()
{
	if (unk10 & 2) {
		for (JSULink<TWaterSplash>* link = unk118.getFirst(); link;) {
			TWaterSplash* splash = link->getObject();
			splash->unkC += unk638;
			splash->unk0.y += splash->unkC;

			if (splash->unk10 != 0)
				--splash->unk10;

			if (splash->unk10 == 0) {
				JSULink<TWaterSplash>* cur = link;

				link = link->getNext();

				unk118.remove(cur);
				unk124.append(cur);
			} else {
				link = link->getNext();
			}
		}
	}
}

void TSplashManager::makeDL(JDrama::TGraphics* param_1) const
{
	MtxPtr viewMtx = param_1->mViewMtx;

	JGeometry::TVec3<f32> thing[4];

	unk640->reset();
	for (JSULink<TWaterSplash>* link = unk118.getFirst(); link;
	     link                        = link->getNext()) {
		TWaterSplash* splash = link->getObject();

		JGeometry::TVec3<f32> pos;
		MTXMultVec(viewMtx, &splash->unk0, &pos);

		if (pos.z < -unk644 || -250.0f < pos.z) {
			splash->unk10 = 0;
			continue;
		}

		f32 fVar1 = ((f32)unk648 - splash->unk10) / unk648 * unk634 + unk630;

		GXColor color
		    = (GXColor) { 0xff, 0xff, 0xff, splash->unk10 * 255 / unk648 };

		thing[0].set(pos.x - fVar1, pos.y + fVar1, pos.z);
		thing[1].set(pos.x + fVar1, pos.y + fVar1, pos.z);
		thing[2].set(pos.x + fVar1, pos.y - fVar1, pos.z);
		thing[3].set(pos.x - fVar1, pos.y - fVar1, pos.z);

		unk640->requestCol(thing, color, splash->unk11);
	}
	unk640->setEnd();
}

void TSplashManager::draw() const
{
	Mtx afStack_38;

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
	GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
	GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U8, 7);
	MTXIdentity(afStack_38);
	GXSetCurrentMtx(GX_PNMTX0);
	GXLoadPosMtxImm(afStack_38, GX_PNMTX0);
	GXLoadNrmMtxImm(afStack_38, GX_PNMTX0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, 0, 125);
	GXSetCullMode(GX_CULL_NONE);
	unk14->load(GX_TEXMAP0);
	GXSetTevColor(GX_TEVREG0, unk63C);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
	                GX_TEVPREV);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
	GXSetClipMode(GX_CLIP_DISABLE);
	unk640->draw();
	GXSetClipMode(GX_CLIP_ENABLE);
}

void TSplashManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 2)
		move();

	if (unk10 & 1) {
		if (param_1 & 4)
			makeDL(param_2);

		if (param_1 & 8)
			draw();
	}
}
