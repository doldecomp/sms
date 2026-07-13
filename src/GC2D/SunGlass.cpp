#include <GC2D/SunGlass.hpp>
#include <Camera/SunMgr.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <Player/MarioAccess.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

extern JPAEmitterManager* gpEmitterManager4D2;

// UNUSED (getShineAlpha__9TSunGlassFv, changeAlpha__9TSunGlassFPUc, mario.MAP,
// 0xA4 each): both fully inlined at every call site in retail, so neither is
// emitted standalone. MWCC keeps a standalone copy of each here instead
// (168B/180B) - the extraction is structurally right (matches the MAP
// signatures and the duplicated inline blocks it replaces) but something
// about this reconstruction isn't tripping the compiler's inline-away
// heuristic yet. TODO: find what's blocking the inline.
void TSunGlass::changeAlpha(u8* out)
{
	u8 alpha = 0;
	if (gpMarDirector->mMap == 1)
		alpha = (u8)((f32)(unk1E - unk1F)
		             * (1.0f
		                - (f32)TFlagManager::getInstance()->getFlag(0x40000)
		                      / 120.0f));
	*out = alpha;
}

void TSunGlass::startFade(int type, bool arg1)
{
	TFlagManager::getInstance()->getFlag(0x40000);

	if (type == 2) {
		changeAlpha(&unk1D);
		unk1C = 0x64;
	} else {
		unk1D = 0x64;
		changeAlpha(&unk1C);
	}

	unk26 = arg1;
	unk24 = 0;
}

void TSunGlass::draw(const JDrama::TRect& rect, JUtility::TColor color)
{
	if (color.a == 0)
		return;

	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX,
	              GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG,
	              GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);

	Mtx m;
	MTXTrans(m, 0.0f, 0.0f, 0.0f);
	GXLoadPosMtxImm(m, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
	                  GX_FALSE, GX_PTIDENTITY);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetNumChans(1);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);

	if (unk1A & 2)
		GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
	else
		GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
		               GX_LO_NOOP);

	u32 col = color;
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3s16(rect.x1, rect.y1, 0);
	GXColor1u32(col);
	GXPosition3s16(rect.x2, rect.y1, 0);
	GXColor1u32(col);
	GXPosition3s16(rect.x2, rect.y2, 0);
	GXColor1u32(col);
	GXPosition3s16(rect.x1, rect.y2, 0);
	GXColor1u32(col);
	GXEnd();
}

void TSunGlass::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if ((flags & 1) && unk26 != 0) {
		// TODO: 81.9% - logic exact; residual is int->float conversion
		// scheduling in the alpha interp.
		u8 from = unk1D;
		unk14.a
		    = (u8)((f32)from + (f32)(unk24 * (unk1C - from)) / (f32)(s16)unk22);
		if (unk24 < (s16)unk22)
			unk24++;
		else
			unk26 = 0;
	}

	if (flags & 8)
		draw(graphics->getViewport(), unk14);
}

void TSunGlass::getShineAlpha()
{
	u8 alpha = 0;
	if (gpMarDirector->mMap == 1)
		alpha = (u8)((f32)(unk1E - unk1F)
		             * (1.0f
		                - (f32)TFlagManager::getInstance()->getFlag(0x40000)
		                      / 120.0f));
	unk14.a = alpha;
}

void TSunGlass::loadAfter() { getShineAlpha(); }

void TSunGlass::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	unk10 = gpMarDirector->unk18[1];
}

void TSunShine::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if (flags & 8)
		draw(graphics->getViewport(), unk14);

	if (flags & 1) {
		unk14.a = gpSunMgr->getAddColor();
		if (unk28 != 0) {
			if (!SMS_IsMarioStatusElecDamage())
				unk28 = 0;
		} else if (SMS_IsMarioStatusElecDamage()) {
			unk28 = 1;
			JGeometry::TVec3<f32> pos(300.0f, 224.0f, 0.0f);
			gpEmitterManager4D2->createEmitter(pos, 0x200, nullptr, nullptr);
		}
	}
}

void TSunShine::loadAfter()
{
	JDrama::TViewObj::loadAfter();
	if (gpMarDirector->mMap == 6) {
		unk14.r = 0x48;
		unk14.g = 0x30;
		unk14.b = 0;
		unk14.a = 0xFF;
	}
}
