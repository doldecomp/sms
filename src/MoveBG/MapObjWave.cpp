#include <Camera/CubeManagerBase.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MoveBG/MapObjWave.hpp>
#include <Player/MarioAccess.hpp>
#include <System/MarDirector.hpp>
#include <dolphin/gx.h>
#include <math.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static JUtility::TColor sColor;

static u8 sAlphaCompLarge = 0x55;
static u8 sAlphaCompSmall = 0x23;

void TMapObjWave::initDraw()
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
	GXLoadPosMtxImm(j3dSys.mViewMtx, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXSetNumTexGens(2);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
	                  GX_FALSE, GX_PTIDENTITY);
	GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY,
	                  GX_FALSE, GX_PTIDENTITY);

	JUTTexture texture(unk94);
	texture.load(GX_TEXMAP0);

	GXSetTevColorS10(GX_TEVREG0, unk7C);
	GXSetTevColorS10(GX_TEVREG1, unk84);
	GXSetTevColorS10(GX_TEVREG2, unk8C);
	GXSetNumTevStages(2);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_RASC, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_APREV,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2,
	                GX_TRUE, GX_TEVPREV);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_SRCCLR, GX_LO_NOOP);
	GXSetAlphaCompare(GX_GEQUAL, sAlphaCompLarge, GX_AOP_OR, GX_LEQUAL,
	                  sAlphaCompSmall);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
	GXSetCullMode(GX_CULL_NONE);
}

f32 TMapObjWave::getMoveTexPos1(f32 param_1) const
{
	f32 result = param_1 * unk78;
	return unk70 + result;
}

f32 TMapObjWave::getMoveTexPos0(f32 param_1) const
{
	return param_1 * unk78 * 0.8f;
}

f32 TMapObjWave::getStaticTexPos1(f32 param_1) const { return param_1 * unk74; }

f32 TMapObjWave::getStaticTexPos0(f32 param_1) const { return param_1 * unk74; }

f32 TMapObjWave::getWaveHeight(f32 param_1, f32 param_2) const
{
	if (unk94 == nullptr)
		return 0.0f;

	f32 a = unk3C * sinf(unk24 * (param_1 * (1.0f / 6.28318f)) + unk64);
	f32 b = unk40 * sinf(unk28 * (param_2 * (1.0f / 6.28318f)) + unk68);
	return a + b;
}

f32 TMapObjWave::getHeight(f32 param_1, f32 param_2, f32 param_3) const
{
	const TBGCheckData* ground;
	f32 height
	    = gpMap->checkGroundExactY(param_1, param_2 + 50.0f, param_3, &ground);
	if (ground->isWaterSurface()) {
		if (ground->isSea())
			return getWaveHeight(param_1, param_3);
		return height;
	}

	return param_2;
}

void TMapObjWave::noWave()
{
	unk34 = 0.0f;
	unk38 = 0.0f;
	unk2C = 0.0f;
	unk30 = 0.0f;
	unk3C = 0.0f;
	unk40 = 0.0f;
}

s32 TMapObjWave::getAlpha(f32 param_1, f32 param_2) const
{
	f32 absZ = fabsf(param_1);
	f32 absX = fabsf(param_2);
	if (absX > absZ)
		return (s32)(unk54 * (1.0f - unk18 * absX));
	return (s32)(unk54 * (1.0f - unk18 * absZ));
}

void TMapObjWave::draw()
{
	for (f32 zOffset = -unk14; zOffset <= unk14 - unk1C; zOffset += unk1C) {
		f32 z0 = zOffset + SMS_GetMarioPos().z;
		f32 z1 = z0 + unk1C;
		GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, unk20 * 2);

		for (f32 xOffset = -unk14; xOffset <= unk14 - unk1C; xOffset += unk1C) {
			f32 x      = xOffset + SMS_GetMarioPos().x;
			s32 alpha0 = getAlpha(zOffset, xOffset);
			s32 alpha1 = getAlpha(zOffset + unk1C, xOffset);

			GXPosition3f32(x, getWaveHeight(x, z0), z0);
			GXColor4u8(sColor.r, sColor.g, sColor.b, alpha0);
			GXTexCoord2f32(getStaticTexPos0(x) + unk6C, getStaticTexPos1(z0));
			GXTexCoord2f32(getMoveTexPos0(x), getMoveTexPos1(z0));

			GXPosition3f32(x, getWaveHeight(x, z1), z1);
			GXColor4u8(sColor.r, sColor.g, sColor.b, alpha1);
			GXTexCoord2f32(getStaticTexPos0(x) + unk6C, getStaticTexPos1(z1));
			GXTexCoord2f32(getMoveTexPos0(x), getMoveTexPos1(z1));
		}
		GXEnd();
	}
}

void TMapObjWave::updateHeightAndAlpha()
{
	const TBGCheckData* ground;
	const TBGCheckData* ground2;

	gpMap->checkGround(SMS_GetMarioPos(), &ground);
	gpMap->checkGroundExactY(SMS_GetMarioPos().x, 10.0f, SMS_GetMarioPos().z,
	                         &ground2);

	if (SMS_CheckMarioFlag(MARIO_FLAG_IN_SHALLOW_WATER)
	    || ground2->isWaterSurface() || ground->isWaterSurface()) {
		f32 height = gpMap->checkGroundIgnoreWaterSurface(
		    SMS_GetMarioPos().x, 0.0f, SMS_GetMarioPos().z, &ground2);
		f32 height2 = unk4C + height;
		if (height2 < 0.0f || ground2->isUnk700()) {
			unk3C = unk2C;
			unk40 = unk30;
		} else {
			f32 ratio = 1.0f - height2 / unk4C;
			unk3C     = ratio * (unk2C - unk34) + unk34;
			unk40     = ratio * (unk30 - unk38) + unk38;
		}

		f32 height3 = unk50 + height;
		if (height3 < 0.0f || ground2->isUnk700()) {
			unk54 = unk58;
		} else {
			unk54 = (1.0f - height3 / unk50) * (unk58 - unk5C) + unk5C;
		}
	} else {
		unk3C = unk34;
		unk40 = unk38;
		unk54 = unk5C;
	}

	if (gpMarDirector->mMap == 4 && -4950.0f < SMS_GetMarioPos().x
	    && -4340.0f > SMS_GetMarioPos().x && 7660.0f < SMS_GetMarioPos().z
	    && 8040.0f > SMS_GetMarioPos().z) {
		unk3C = unk34;
		unk40 = unk38;
		unk54 = unk5C;
	}

	int cubeNo = gpCubeStream->getInCubeNo(SMS_GetMarioPos());
	if (cubeNo != -1) {
		if (unk44 < ((TCubeStreamInfo&)(*gpCubeStream->unk14)[cubeNo]).unk3C)
			unk44 += unk48;
	} else if (unk44 > 0.0f) {
		unk44 -= unk48;
	} else {
		unk44 = 0.0f;
	}

	if (unk44 > 0.0f) {
		unk3C = unk2C + unk44;
		unk40 = unk30 + unk44;
	}
}

void TMapObjWave::updateTime()
{
	unk64 += unk24;
	if (unk64 > 6.28318f)
		unk64 -= 6.28318f;

	unk68 += unk28;
	if (unk68 > 6.28318f)
		unk68 -= 6.28318f;

	unk6C += unk60;
	if (unk6C > 1.0f)
		unk6C -= 1.0f;

	unk70 += unk60;
	if (unk70 > 1.0f)
		unk70 -= 1.0f;
}

void TMapObjWave::movement()
{
	updateTime();
	if (gpMarDirector->mMap == 4 || gpMarDirector->mMap == 6)
		updateHeightAndAlpha();
}

void TMapObjWave::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (unk94 != nullptr) {
		if (cue & CUE_MOVE)
			movement();
		if (cue & CUE_DRAW) {
			initDraw();
			draw();
		}
	}
}

void TMapObjWave::load(JSUMemoryInputStream& param_1)
{
	JDrama::TViewObj::load(param_1);
	unk10 = 5200.0f;
	unk1C = 200.0f;
	unk14 = unk10 / 2.0f;
	unk18 = 1.0f / unk14;
	unk20 = unk10 / unk1C;
	unk94 = (ResTIMG*)JKRGetResource("/scene/map/map/wave.bti");
	unk60 = 0.0015f;
	unk74 = 0.0012f;
	unk78 = 0.0015f;
	unk4C = 400.0f;
	unk50 = 150.0f;
	unk24 = 0.02f;
	unk28 = 0.03f;

	switch (gpMarDirector->mMap) {
	case 3:
	case 30:
		unk2C = 25.0f;
		unk30 = 20.0f;
		unk34 = 0.0f;
		unk38 = 0.0f;
		unk3C = unk2C;
		unk40 = unk30;
		break;
	case 4:
		unk2C = 40.0f;
		unk30 = 30.0f;
		unk34 = 5.0f;
		unk38 = 0.0f;
		break;
	case 13:
		unk2C = 30.0f;
		unk30 = 25.0f;
		unk34 = 5.0f;
		unk38 = 0.0f;
		break;
	case 9:
	case 52:
		unk2C = 10.0f;
		unk30 = 15.0f;
		unk34 = 0.0f;
		unk38 = 0.0f;
		break;
	default:
		unk2C = 30.0f;
		unk30 = 25.0f;
		unk34 = 0.0f;
		unk38 = 0.0f;
		break;
	}

	unk3C = unk2C;
	unk40 = unk30;
}

TMapObjWave* gpMapObjWave;

TMapObjWave::TMapObjWave(const char* param_1)
    : JDrama::TViewObj(param_1)
    , unk10(0.0f)
    , unk14(0.0f)
    , unk18(0.0f)
    , unk20(0)
    , unk24(0.0f)
    , unk28(0.0f)
    , unk2C(0.0f)
    , unk30(0.0f)
    , unk34(0.0f)
    , unk38(0.0f)
    , unk3C(0.0f)
    , unk40(0.0f)
    , unk44(0.0f)
    , unk48(0.1f)
    , unk4C(0.0f)
    , unk50(0.0f)
    , unk54(255.0f)
    , unk58(255.0f)
    , unk5C(0.0f)
    , unk60(0.0f)
    , unk64(MsRandF() * 360.0f)
    , unk68(MsRandF() * 360.0f)
    , unk6C(MsRandF())
    , unk70(MsRandF())
    , unk74(0.0f)
    , unk78(0.0f)
    , unk94(nullptr)
    , unk98(0)
{
	sColor.r     = 0xC8;
	sColor.g     = 0xC8;
	sColor.b     = 0xFF;
	sColor.a     = 0;
	unk7C.r      = 0xC2;
	unk7C.g      = 0xF2;
	unk7C.b      = 0xBE;
	unk7C.a      = 0;
	unk84.r      = 0;
	unk84.g      = 0;
	unk84.b      = 0;
	unk84.a      = 0x48;
	unk8C.r      = 0;
	unk8C.g      = 0;
	unk8C.b      = 0;
	unk8C.a      = 0x90;
	gpMapObjWave = this;
}
