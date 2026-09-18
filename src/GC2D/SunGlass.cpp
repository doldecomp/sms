#include <GC2D/SunGlass.hpp>
#include <Camera/SunMgr.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <Player/MarioAccess.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

extern JPAEmitterManager* gpEmitterManager4D2;

void TSunGlass::startFade(int type, bool arg1)
{
	// The shine count is read and thrown away here in retail: the read is the
	// only reason startFade's frame carries a named 4-byte slot at 0x58.
	// TODO: dead in the shipped build; presumably fed a removed debug print.
	s32 shineCount = SMSGetFlagManager()->getFlag(0x40000);

	if (type == 2) {
		unk1D = getShineAlpha();
		unk1C = 100;
	} else {
		unk1D = 100;
		unk1C = getShineAlpha();
	}

	unk26 = arg1;
	unk24 = 0;
}

void TSunGlass::changeAlpha(u8*) { }

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

void TSunGlass::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if ((cue & CUE_MOVE) && unk26 != 0) {
		unk14.a = (u8)((f32)unk1D
		               + (f32)(unk24 * (unk1C - unk1D)) / (f32)(s16)unk22);
		if (unk24 < (s16)unk22)
			unk24++;
		else
			unk26 = 0;
	}

	if (cue & CUE_DRAW)
		draw(graphics->getViewport(), unk14);
}

// Both this and changeAlpha(u8*) are UNUSED at 0xa4 in the map, and this body
// reproduces 0xa4 exactly, so the two really are same-sized siblings.
// TODO: changeAlpha's body is still unknown. `changeAlpha(u8* p) { *p =
// getShineAlpha(); }` is the only shape that explains the equal sizes, but it
// compiles to 0xb4 and buys none of startFade's frame, so it is not written
// here. Writing the computation into `*p` directly is 0xb4 too.
//
// TODO: this expansion is 8 bytes of low region short at every site. A dead
// 8-byte non-trivial local here (a `struct { ~T(); u32 a, b; }` probe) takes
// both TSunGlass::loadAfter and startFade to 100% at unchanged instruction
// counts, and unlike the accessor forks below it does not saturate across the
// two expansions in startFade. Nothing in the body wants an 8-byte object, so
// it is left out; see docs/catalog/frame-gaps.md "The dead low region".
u8 TSunGlass::getShineAlpha()
{
	u8 alpha = 0;
	if (SMSGetMarDirector()->getCurrentMap() == 1)
		alpha = (u8)((f32)(unk1E - unk1F)
		             * (1.0f
		                - (f32)SMSGetFlagManager()->getFlag(0x40000)
		                      / 120.0f));
	return alpha;
}

void TSunGlass::loadAfter() { unk14.a = getShineAlpha(); }

void TSunGlass::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	TMarioGamePad* gamePad = SMSGetMarDirector()->getGamePad(1);
	unk10 = gamePad;
}

// TODO: frame 0x30 is exact but every temporary sits 4 bytes low: retail's
// `draw` colour copy is at 0x18 and the emitter position at 0x1c-0x24, ours at
// 0x14 and 0x18-0x20. One 4-byte low-region item below the colour copy is
// missing; accessor levels come in 8-byte steps, so it is not an accessor.
void TSunShine::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_DRAW)
		draw(graphics->getViewport(), unk14);

	if (cue & CUE_MOVE) {
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

// TODO: frame 0x20 vs 0x28. Instructions are exact; 8 bytes of locals are
// missing with no positional evidence (the function references no stack slot).
// An uninitialised 8- or 12-byte local anywhere in the body lands 0x28 at 23
// instructions. `unk14.set(0x48, 0x30, 0, 0xFF)` and an unnamed
// `JUtility::TColor(...)` temporary are both worth 0 (the temporary also costs
// two instructions); a named `u8 map` and SMSGetMarDirector() are worth 0.
void TSunShine::loadAfter()
{
	JDrama::TViewObj::loadAfter();
	if (gpMarDirector->getCurrentMap() == 6) {
		unk14.r = 0x48;
		unk14.g = 0x30;
		unk14.b = 0;
		unk14.a = 0xFF;
	}
}
