#include <MoveBG/MapObjWave.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <System/MarDirector.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <math.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

/// Alpha values outside [sAlphaCompSmall, sAlphaCompLarge] are drawn; the
/// band in between is punched out, which is what gives the sea its foam edge.
static u8 sAlphaCompLarge = 85;
static u8 sAlphaCompSmall = 35;

static JUtility::TColor sColor;

TMapObjWave* gpMapObjWave;

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
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);

	GXSetNumTexGens(2);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
	                  GX_FALSE, GX_PTIDENTITY);
	GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY,
	                  GX_FALSE, GX_PTIDENTITY);

	JUTTexture texture(mTexture);
	texture.load(GX_TEXMAP0);

	GXSetTevColorS10(GX_TEVREG0, mTevColor0);
	GXSetTevColorS10(GX_TEVREG1, mTevColor1);
	GXSetTevColorS10(GX_TEVREG2, mTevColor2);

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

	// The destination factor is 2, which is GX_BL_SRCCLR and GX_BL_DSTCLR
	// alike; the two spell the same byte, so which name the original used is
	// not recoverable.
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_SRCCLR, GX_LO_NOOP);
	GXSetAlphaCompare(GX_GEQUAL, sAlphaCompLarge, GX_AOP_OR, GX_LEQUAL,
	                  sAlphaCompSmall);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
	GXSetCullMode(GX_CULL_NONE);
}

f32 TMapObjWave::getMoveTexPos1(f32 v) const
{
	return mTexPos1 + getStaticTexPos1(v);
}

f32 TMapObjWave::getMoveTexPos0(f32 v) const
{
	return mTexPos0 + getStaticTexPos0(v);
}

f32 TMapObjWave::getStaticTexPos1(f32 v) const { return v * mTexScale1; }

f32 TMapObjWave::getStaticTexPos0(f32 v) const { return v * mTexScale0; }

f32 TMapObjWave::getWaveHeight(f32 x, f32 z) const
{
	if (mTexture == nullptr)
		return 0.0f;

	// 6.28318f is the unit's own rounded 2*pi; updateTime() wraps the phases
	// against the same literal. Both products have to stay in their own local
	// or MWCC contracts the second one into an fmadds.
	f32 heightX = mWaveHeightX
	              * sinf(mWaveFreqX * (x * (1.0f / 6.28318f)) + mWavePhaseX);
	f32 heightZ = mWaveHeightZ
	              * sinf(mWaveFreqZ * (z * (1.0f / 6.28318f)) + mWavePhaseZ);
	return heightX + heightZ;
}

f32 TMapObjWave::getHeight(f32 x, f32 y, f32 z) const
{
	const TBGCheckData* ground;
	f32 groundY = gpMap->checkGroundExactY(x, y + 50.0f, z, &ground);

	if (ground->isWaterSurface()) {
		if (ground->isSea())
			return getWaveHeight(x, z);
		else
			return groundY;
	} else {
		return y;
	}
}

void TMapObjWave::noWave()
{
	mWaveHeightMinX = 0.0f;
	mWaveHeightMinZ = 0.0f;
	mWaveHeightMaxX = 0.0f;
	mWaveHeightMaxZ = 0.0f;
	mWaveHeightX    = 0.0f;
	mWaveHeightZ    = 0.0f;
}

int TMapObjWave::getAlpha(f32 x, f32 z) const
{
	if (fabsf(x) > fabsf(z))
		return mAlpha * (1.0f - fabsf(x) * mInvHalfSize);
	else
		return mAlpha * (1.0f - fabsf(z) * mInvHalfSize);
}

void TMapObjWave::draw()
{
	for (f32 z = -mHalfSize; z <= mHalfSize - mGridSize; z += mGridSize) {
		f32 z0 = z + SMS_GetMarioZ();
		f32 z1 = z0 + mGridSize;

		GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mDivideNum * 2);
		for (f32 x = -mHalfSize; x <= mHalfSize - mGridSize; x += mGridSize) {
			f32 x0     = x + SMS_GetMarioX();
			int alpha0 = getAlpha(x, z);
			int alpha1 = getAlpha(x, z + mGridSize);

			GXPosition3f32(x0, getWaveHeight(x0, z0), z0);
			GXColor4u8(sColor.r, sColor.g, sColor.b, alpha0);
			GXTexCoord2f32(getMoveTexPos0(x0), getStaticTexPos0(z0));
			GXTexCoord2f32(0.8f * getStaticTexPos1(x0), getMoveTexPos1(z0));

			GXPosition3f32(x0, getWaveHeight(x0, z1), z1);
			GXColor4u8(sColor.r, sColor.g, sColor.b, alpha1);
			GXTexCoord2f32(getMoveTexPos0(x0), getStaticTexPos0(z1));
			GXTexCoord2f32(0.8f * getStaticTexPos1(x0), getMoveTexPos1(z1));
		}
		GXEnd();
	}
}

void TMapObjWave::updateHeightAndAlpha()
{
	const TBGCheckData* ground;
	gpMap->checkGround(SMS_GetMarioPos(), &ground);

	const TBGCheckData* surface;
	gpMap->checkGroundExactY(gpMarioPos->x, 10.0f, gpMarioPos->z, &surface);

	if (SMS_CheckMarioFlag(MARIO_FLAG_IN_SHALLOW_WATER)
	    || surface->isWaterSurface() || ground->isWaterSurface()) {
		f32 floorY = gpMap->checkGroundIgnoreWaterSurface(
		    gpMarioPos->x, 0.0f, gpMarioPos->z, &surface);

		f32 heightDepth = mHeightFadeDepth + floorY;
		if (heightDepth < 0.0f || surface->isSeaFloor()) {
			mWaveHeightX = mWaveHeightMaxX;
			mWaveHeightZ = mWaveHeightMaxZ;
		} else {
			f32 rate = 1.0f - heightDepth / mHeightFadeDepth;
			mWaveHeightX
			    = rate * (mWaveHeightMaxX - mWaveHeightMinX) + mWaveHeightMinX;
			mWaveHeightZ
			    = rate * (mWaveHeightMaxZ - mWaveHeightMinZ) + mWaveHeightMinZ;
		}

		f32 alphaDepth = mAlphaFadeDepth + floorY;
		if (alphaDepth < 0.0f || surface->isSeaFloor()) {
			mAlpha = mAlphaMax;
		} else {
			f32 rate = 1.0f - alphaDepth / mAlphaFadeDepth;
			mAlpha   = rate * (mAlphaMax - mAlphaMin) + mAlphaMin;
		}
	} else {
		mWaveHeightX = mWaveHeightMinX;
		mWaveHeightZ = mWaveHeightMinZ;
		mAlpha       = mAlphaMin;
	}

	// The Sirena Beach hotel lobby sits inside the sea area; kill the waves
	// so the mesh does not poke through the floor.
	u8 map = SMSGetMarDirector()->getCurrentMap();
	if (map == 4) {
		if (-4950.0f < SMS_GetMarioX() && -4340.0f > SMS_GetMarioX()
		    && 7660.0f < SMS_GetMarioZ() && 8040.0f > SMS_GetMarioZ()) {
			mWaveHeightX = mWaveHeightMinX;
			mWaveHeightZ = mWaveHeightMinZ;
			mAlpha       = mAlphaMin;
		}
	}

	int cubeNo = gpCubeStream->getInCubeNo(SMS_GetMarioPos());
	if (cubeNo != -1) {
		TCubeStreamInfo* info
		    = (TCubeStreamInfo*)gpCubeStream->unk14->getChildren()[cubeNo];
		if (mCubeWaveHeight < info->unk3C)
			mCubeWaveHeight += mCubeWaveHeightRate;
	} else {
		if (mCubeWaveHeight > 0.0f)
			mCubeWaveHeight -= mCubeWaveHeightRate;
		else
			mCubeWaveHeight = 0.0f;
	}

	if (mCubeWaveHeight > 0.0f) {
		mWaveHeightX = mWaveHeightMaxX + mCubeWaveHeight;
		mWaveHeightZ = mWaveHeightMaxZ + mCubeWaveHeight;
	}
}

void TMapObjWave::updateTime()
{
	mWavePhaseX += mWaveFreqX;
	if (mWavePhaseX > 6.28318f)
		mWavePhaseX -= 6.28318f;

	mWavePhaseZ += mWaveFreqZ;
	if (mWavePhaseZ > 6.28318f)
		mWavePhaseZ -= 6.28318f;

	mTexPos0 += mTexScrollSpeed;
	if (mTexPos0 > 1.0f)
		mTexPos0 -= 1.0f;

	mTexPos1 += mTexScrollSpeed;
	if (mTexPos1 > 1.0f)
		mTexPos1 -= 1.0f;
}

/**
 * @brief The CUE_MOVE half of perform().
 *
 * @details UNUSED in the map because perform() is its only caller and it is
 * inlined there. The indirection is load-bearing rather than cosmetic: with
 * this body written straight into perform(), updateTime() reaches inline
 * depth 1 and gets expanded, which the ROM does not do. Behind movement() it
 * sits at depth 2 and stays a call, exactly as in the ROM. The out-of-line
 * copy does inline updateTime(), which is why it compiles to the map's 0xd8.
 */
void TMapObjWave::movement()
{
	updateTime();
	if (SMSGetMarDirector()->getCurrentMap() == 4
	    || SMSGetMarDirector()->getCurrentMap() == 6)
		updateHeightAndAlpha();
}

// TODO: instruction-identical but the frame is 8 bytes short (0x38 vs 0x40),
// so the original declared one more two-word local here. The eight bytes are
// perform()'s own, not movement()'s: an uninitialised scalar in movement() is
// worth zero because movement() is an inlined callee (MWCC drops an inlined
// callee's trivial locals; only a non-trivial 8-byte class local there would
// count), while the same declaration in perform() is +8.
//
// The map read is the only lever and it is saturated. Each
// SMSGetMarDirector()->getCurrentMap() use is +8 and saturates at three uses;
// gpMarDirector->getCurrentMap() and ->mMap are +0. Measured frame/insns
// (target 0x40/34) over five spellings of the texture guard x seven of the map
// read, all 35 combinations:
//
//   two accessor uses (below)         0x38 / 34   <- best
//   one accessor + one bare global    0x30 / 34
//   two bare globals, or two ->mMap   0x28 / 34
//   !mTexture, nested guard, u32 c    no change from the above
//   named const ResTIMG* texture      +8 only where the map read is not
//                                     already saturated (0x30 with a bare
//                                     global, still 0x38 with two accessors)
//   three accessor uses               0x40 / 36   <- right frame, wrong body
//   switch (getCurrentMap()) 4/6      0x28 / 38
//   != 4 && != 6 with early return    0x38 / 34
//   a TU-static getMapNo()/getDirector() forwarder above SMSGetMarDirector()
//                                     0x38 / 34, plus a stray symbol
//
// So the residue is one 8-byte object at the bottom of the frame and there is
// no evidence for what it was: retail references no stack slot in perform, so
// there is no positional evidence either. See docs/catalog/frame-gaps.md,
// "The last 8 bytes". Left nonmatching rather than padded.
void TMapObjWave::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mTexture == nullptr)
		return;

	if (cue & CUE_MOVE)
		movement();

	if (cue & CUE_DRAW) {
		initDraw();
		draw();
	}
}

void TMapObjWave::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);

	mAreaSize = 5200.0f;
	mGridSize = 200.0f;

	mHalfSize    = mAreaSize / 2.0f;
	mInvHalfSize = 1.0f / mHalfSize;
	mDivideNum   = mAreaSize / mGridSize;

	mTexture = (const ResTIMG*)JKRGetResource("/scene/map/map/wave.bti");

	mTexScrollSpeed = 0.0015f;
	mTexScale0      = 0.0012f;
	mTexScale1      = 0.0015f;

	mHeightFadeDepth = 400.0f;
	mAlphaFadeDepth  = 150.0f;

	mWaveFreqX = 0.02f;
	mWaveFreqZ = 0.03f;

	switch (SMSGetMarDirector()->getCurrentMap()) {
	case 3:
	case 30:
		mWaveHeightMaxX = 25.0f;
		mWaveHeightMaxZ = 20.0f;
		mWaveHeightMinX = 0.0f;
		mWaveHeightMinZ = 0.0f;
		mWaveHeightX    = mWaveHeightMaxX;
		mWaveHeightZ    = mWaveHeightMaxZ;
		break;
	case 4:
		mWaveHeightMaxX = 40.0f;
		mWaveHeightMaxZ = 30.0f;
		mWaveHeightMinX = 5.0f;
		mWaveHeightMinZ = 0.0f;
		break;
	case 13:
		mWaveHeightMaxX = 30.0f;
		mWaveHeightMaxZ = 25.0f;
		mWaveHeightMinX = 5.0f;
		mWaveHeightMinZ = 0.0f;
		break;
	case 9:
	case 52:
		mWaveHeightMaxX = 10.0f;
		mWaveHeightMaxZ = 15.0f;
		mWaveHeightMinX = 0.0f;
		mWaveHeightMinZ = 0.0f;
		break;
	default:
		mWaveHeightMaxX = 30.0f;
		mWaveHeightMaxZ = 25.0f;
		mWaveHeightMinX = 0.0f;
		mWaveHeightMinZ = 0.0f;
		break;
	}

	mWaveHeightX = mWaveHeightMaxX;
	mWaveHeightZ = mWaveHeightMaxZ;
}

TMapObjWave::TMapObjWave(const char* name)
    : JDrama::TViewObj(name)
    , mAreaSize(0.0f)
    , mHalfSize(0.0f)
    , mInvHalfSize(0.0f)
    , mDivideNum(0)
    , mWaveFreqX(0.0f)
    , mWaveFreqZ(0.0f)
    , mWaveHeightMaxX(0.0f)
    , mWaveHeightMaxZ(0.0f)
    , mWaveHeightMinX(0.0f)
    , mWaveHeightMinZ(0.0f)
    , mWaveHeightX(0.0f)
    , mWaveHeightZ(0.0f)
    , mCubeWaveHeight(0.0f)
    , mCubeWaveHeightRate(0.1f)
    , mHeightFadeDepth(0.0f)
    , mAlphaFadeDepth(0.0f)
    , mAlpha(255.0f)
    , mAlphaMax(255.0f)
    , mAlphaMin(0.0f)
    , mTexScrollSpeed(0.0f)
    , mWavePhaseX(360.0f * MsRandF())
    , mWavePhaseZ(360.0f * MsRandF())
    , mTexPos0(MsRandF())
    , mTexPos1(MsRandF())
    , mTexScale0(0.0f)
    , mTexScale1(0.0f)
{
	mTexture = nullptr;
	unk98    = 0;

	// The vertex colour is a file static, but it is the instance constructor
	// that fills it in; the sinit only runs JUtility::TColor's own default
	// constructor (white).
	sColor.set(200, 200, 255, 0);

	mTevColor0.r = 194;
	mTevColor0.g = 242;
	mTevColor0.b = 190;
	mTevColor0.a = 0;

	mTevColor1.r = 0;
	mTevColor1.g = 0;
	mTevColor1.b = 0;
	mTevColor1.a = 72;

	mTevColor2.r = 0;
	mTevColor2.g = 0;
	mTevColor2.b = 0;
	mTevColor2.a = 144;

	gpMapObjWave = this;
}
