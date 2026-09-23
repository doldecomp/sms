#include <MoveBG/MapObjPlane.hpp>
#include <Map/MapCollisionPlane.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <dolphin/gx.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

f32 TMapObjPlane::mHipDropDownRate = 50.0f;
f32 TMapObjPlane::mWaterDownRate   = 2.0f;
f32 TMapObjPlane::mMudDownRate     = 0.0f;
f32 TMapObjPlane::mTexScale        = 0.6f;
f32 TMapObjPlane::mWeatherRate     = 0.0f;
u8 TMapObjPlane::mAmbColor         = 0x46;

void TMapObjPlane::initDraw()
{
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

	GXLoadPosMtxImm(j3dSys.getViewMtx(), GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);

	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_TRUE, GX_SRC_REG, GX_SRC_REG, 1, GX_DF_CLAMP,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE,
	              GX_AF_NONE);

	GXSetChanAmbColor(GX_COLOR0A0,
	                  (GXColor) { mAmbColor, mAmbColor, mAmbColor, 0xff });
	GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });

	GXLightObj light;
	JGeometry::TVec3<f32> pos(20000.0f, 20000.0f, 20000.0f);
	GXInitLightPos(&light, pos.x, pos.y, pos.z);
	GXInitLightColor(&light, (GXColor) { 0xff, 0xff, 0xff, 0xff });
	GXLoadLightObjImm(&light, GX_LIGHT0);

	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
	JUTTexture JStack_a8(mAlbedo);
	JStack_a8.load(GX_TEXMAP0);

	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_KONST, GX_CA_ZERO, GX_CA_ZERO,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);

	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
	GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_GREATER, 0);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetCullMode(GX_CULL_NONE);
}

// TODO: frame is 0xb0, retail 0xb8, every slot shifted by 8 (one more low
// inline temporary); split worldZNext, s32 counters and named texcoords were inert.
void TMapObjPlane::draw()
{
	for (int z = 0; z < mExtents - 1; ++z) {
		f32 worldZ     = mCollision->gridToWorld(z);
		f32 worldZNext = worldZ + unkFC;

		GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mExtents * 2);
		for (int x = 0; x < mExtents; ++x) {
			f32 worldX = mCollision->gridToWorld(x);

			f32& height = heightAt(x, z);
			GXPosition3f32(worldX, height, worldZ);
			GXNormal3f32(normalAt(x, z).x, normalAt(x, z).y, normalAt(x, z).z);
			GXTexCoord2f32(getTexPos(x), getTexPos(z));

			GXPosition3f32(worldX, heightAt(x, z + 1), worldZNext);
			GXNormal3f32(normalAt(x, z + 1).x, normalAt(x, z + 1).y,
			             normalAt(x, z + 1).z);
			GXPosition2f32(getTexPos(x), getTexPos(z + 1));
		}
		GXEnd();
	}
}

f32 TMapObjPlane::getTexPos(f32 v) const { return v * mTexScale; }

void TMapObjPlane::updateCheckData(int x, int z)
{
	if (x < 0 || mExtents <= x || z < 0 || mExtents <= z)
		return;

	f32 x1 = mCollision->gridToWorld(x);
	f32 x2 = mCollision->gridToWorld(x + 1);
	f32 z1 = mCollision->gridToWorld(z);
	f32 z2 = mCollision->gridToWorld(z + 1);

	JGeometry::TVec3<f32> local_64(x1, heightAt(x, z) + 2.0f, z1);
	JGeometry::TVec3<f32> local_58(x2, heightAt(x + 1, z) + 2.0f, z1);
	JGeometry::TVec3<f32> local_4c(x1, heightAt(x, z + 1) + 2.0f, z2);
	JGeometry::TVec3<f32> local_40(x2, heightAt(x + 1, z + 1) + 2.0f, z2);

	mCollision->getCheckData(x, z, 0)->setVertex(local_64, local_4c, local_58);
	mCollision->getCheckData(x, z, 1)->setVertex(local_58, local_4c, local_40);
}

// One inline level above normalize() is what reaches the map's out-of-line
// JGeometry::TVec3<f32>::dot, ::scale(f, const&) and TUtil<f32>::inv_sqrt at
// all four sites: it puts setLength at depth 3, scale and squared at 4 (the
// three-statement allowance is 2 there) and dot at 5. Parked here because the
// map has no symbol for it.
static inline void MapObjPlaneNormalize(JGeometry::TVec3<f32>& v)
{
	v.normalize();
}

void TMapObjPlane::calcNrm(int x, int z)
{
	if (x < 0 || mExtents <= x || z < 0 || mExtents <= z)
		return;

	// TODO: 92.8%, frame 0x240 vs 0x230. The FPRs rank differently (retail
	// gives h00..hP0 f31..f27, then w, nw), retail re-subtracts h0N - h00 and
	// h0P - hP0 where ours reuses them, and its normal sum copies each partial
	// sum through a temporary before the next operator+ (a by-value return).
	// Inert: C-style top declarations, w read before h0N, s1/s2 sum locals.
	// The extra FPR (f23) is ours carrying face 1's `0 - nw` into face 2 (a
	// cross-call CSE retail does not do). Retail's four normals are named and
	// contiguous (0x194..0x170) with no edge slots between them. Inert (c-mix1):
	// a face-normal helper taking three points (edges as its locals), normals
	// declared together, cross over operator- temporaries (48%).
	f32 h00 = heightAt(x, z);
	f32 h0N = heightAt(x, MsWrap(z - 1, 0, mExtents));
	f32 w = unkFC;
	f32 nw = -w;
	f32 h0P = heightAt(x, MsWrap(z + 1, 0, mExtents));
	f32 hN0 = heightAt(MsWrap(x - 1, 0, mExtents), z);
	f32 hP0 = heightAt(MsWrap(x + 1, 0, mExtents), z);

	JGeometry::TVec3<f32> p00(0.0f, h00, 0.0f);
	JGeometry::TVec3<f32> p0N(0.0f, h0N, nw);
	JGeometry::TVec3<f32> p0P(0.0f, h0P, w);
	JGeometry::TVec3<f32> pN0(nw, hN0, 0.0f);
	JGeometry::TVec3<f32> pP0(w, hP0, 0.0f);

	JGeometry::TVec3<f32> e0a;
	e0a.sub(p0N, pN0);
	JGeometry::TVec3<f32> e0b;
	e0b.sub(pN0, p00);
	JGeometry::TVec3<f32> local_9c;
	local_9c.cross(e0a, e0b);
	MapObjPlaneNormalize(local_9c);

	JGeometry::TVec3<f32> e1a;
	e1a.sub(pP0, p0N);
	JGeometry::TVec3<f32> e1b;
	e1b.sub(p0N, p00);
	JGeometry::TVec3<f32> local_a8;
	local_a8.cross(e1a, e1b);
	MapObjPlaneNormalize(local_a8);

	JGeometry::TVec3<f32> e2a;
	e2a.sub(pN0, p0P);
	JGeometry::TVec3<f32> e2b;
	e2b.sub(p0P, p00);
	JGeometry::TVec3<f32> local_b4;
	local_b4.cross(e2a, e2b);
	MapObjPlaneNormalize(local_b4);

	JGeometry::TVec3<f32> e3a;
	e3a.sub(p0P, pP0);
	JGeometry::TVec3<f32> e3b;
	e3b.sub(pP0, p00);
	JGeometry::TVec3<f32> local_c0;
	local_c0.cross(e3a, e3b);
	MapObjPlaneNormalize(local_c0);

	JGeometry::TVec3<f32>& nrm = mNormalMap[x + z * mExtents];
	JGeometry::TVec3<f32> sum = local_9c + local_a8 + local_b4 + local_c0;
	nrm = sum;
	nrm.scale(0.25f);
}

void TMapObjPlane::movement() { }

void TMapObjPlane::depress(f32 x, f32 z, f32 rate)
{
	f32 x_ = mCollision->worldToGrid(x);
	f32 z_ = mCollision->worldToGrid(z);

	int x_00 = x_;
	int z_00 = z_;

	f32 xrem = x_ - x_00;
	f32 zrem = z_ - z_00;

	f32& h00 = heightAt(x_00, z_00);
	h00      = h00 - rate * ((1.0f - xrem) + (1.0f - zrem));
	f32& h10 = heightAt(x_00 + 1, z_00);
	h10      = h10 - rate * (xrem + (1.0f - zrem));
	f32& h01 = heightAt(x_00, z_00 + 1);
	h01      = h01 - rate * ((1.0f - xrem) + zrem);
	f32& h11 = heightAt(x_00 + 1, z_00 + 1);
	h11      = h11 - rate * (xrem + zrem);

	calcNrm(x_00, z_00 - 1);
	calcNrm(x_00 + 1, z_00 - 1);
	calcNrm(x_00 - 1, z_00);
	calcNrm(x_00, z_00);
	calcNrm(x_00 + 1, z_00);
	calcNrm(x_00 + 2, z_00);
	calcNrm(x_00 - 1, z_00 + 1);
	calcNrm(x_00, z_00 + 1);
	calcNrm(x_00 + 1, z_00 + 1);
	calcNrm(x_00 + 1, z_00 + 2);
	calcNrm(x_00, z_00 + 2);
	calcNrm(x_00 + 1, z_00 + 2);
	updateCheckData(x_00 - 1, z_00 - 1);
	updateCheckData(x_00, z_00 - 1);
	updateCheckData(x_00 + 1, z_00 - 1);
	updateCheckData(x_00 - 1, z_00);
	updateCheckData(x_00, z_00);
	updateCheckData(x_00 + 1, z_00);
	updateCheckData(x_00 - 1, z_00 + 1);
	updateCheckData(x_00, z_00 + 1);
	updateCheckData(x_00 + 1, z_00 + 1);
}

void TMapObjPlane::weather() { }

BOOL TMapObjPlane::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_HIP_DROP) {
		depress(sender->mPosition.x, sender->mPosition.z, mHipDropDownRate);
		return true;
	}

	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		depress(sender->mPosition.x, sender->mPosition.z, mWaterDownRate);
		return true;
	}

	return false;
}

void TMapObjPlane::perform(u32 cue, JDrama::TGraphics*)
{
	if (mAlbedo != nullptr && (cue & CUE_DRAW)) {
		initDraw();
		draw();
	}
}

// TODO: frame 0x38 short (0x28 vs 0x60) in the low region and the header adds
// are scheduled differently. Inert: height first, |, reassociated or reversed
// sums; a little-endian read helper adds only 8 per call. TU-local pixel
// and pixel-to-height helpers (c-mix1) add 8 each with no instruction change
// (0x30..0x50 in combination); the map has no symbol for any of them.
void TMapObjPlane::makeMountain()
{
	int width = (unk118[0x15] << 24) + (unk118[0x14] << 16)
	            + (unk118[0x13] << 8) + unk118[0x12];

	int height = (unk118[0x19] << 24) + (unk118[0x18] << 16)
	             + (unk118[0x17] << 8) + unk118[0x16];

	for (int z = 0; z < mExtents; z = z + 1) {
		for (int x = 0; x < mExtents; x = x + 1) {

			u8 byte = unk118[width * (height - 1 - z) + x + 0x436];
			mHeightMap[x + z * mExtents]
			    = (byte * 3000.0f) / 255.0f + mPosition.y;
		}
	}

	for (int z = 0; z < mExtents; ++z)
		for (int x = 0; x < mExtents; ++x)
			calcNrm(x, z);
}

void TMapObjPlane::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);

	unkF4 = unkFC * mExtents;
	unkF8 = unkF4 / 2;

	mHeightMap = new f32[mExtents * mExtents];
	mNormalMap = new JGeometry::TVec3<f32>[mExtents * mExtents];

	makeMountain();

	mCollision = new TMapCheckGroundPlane;
	mCollision->init(mExtents, mExtents, unkFC);

	for (int z = 0; z < mExtents - 1; ++z) {
		for (int x = 0; x < mExtents - 1; ++x) {
			mCollision->getCheckData(x, z, 0)->mActor = this;
			mCollision->getCheckData(x, z, 1)->mActor = this;

			updateCheckData(x, z);
		}
	}
}

TMapObjPlane::TMapObjPlane(const char* name)
    : TLiveActor(name)
    , unkF4(0.0f)
    , unkF8(0.0f)
    , unkFC(0.0f)
    , mExtents(0)
    , mHeightMap(nullptr)
    , mNormalMap(nullptr)
    , mCollision(nullptr)
    , unk110(0)
    , mAlbedo(nullptr)
    , unk11C(0)
{
}

void TRockPlane::load(JSUMemoryInputStream& stream)
{
	unkFC    = 100.0f;
	mExtents = 64;
	mAlbedo  = (ResTIMG*)JKRGetResource("/scene/map/map/RockPlane.bti");
	if (!mAlbedo)
		return;

	unk118 = (u8*)JKRGetResource("/scene/map/map/RockPlane.bmp");
	TMapObjPlane::load(stream);
}

void TSandPlane::load(JSUMemoryInputStream& stream)
{
	unkFC    = 100.0f;
	mExtents = 64;
	mAlbedo  = (ResTIMG*)JKRGetResource("/scene/map/map/SandPlane.bti");
	if (!mAlbedo)
		return;

	unk118 = (u8*)JKRGetResource("/scene/map/map/SandPlane.bmp");
	TMapObjPlane::load(stream);
}
