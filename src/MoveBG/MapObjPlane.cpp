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

void TMapObjPlane::draw()
{
	for (int z = 0; z < mExtents - 1; ++z) {
		f32 fVar1 = unkFC;

		f32 worldZ     = mCollision->gridToWorld(z);
		f32 nextWorldZ = worldZ + fVar1;

		GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mExtents * 2);
		for (int x = 0; x < mExtents; ++x) {
			f32 worldX = mCollision->gridToWorld(x);

			GXPosition3f32(worldX, heightAt(x, z), worldZ);
			GXNormal3f32(normalAt(x, z).x, normalAt(x, z).y, normalAt(x, z).z);
			GXTexCoord2f32(getTexPos(x), getTexPos(z));

			GXPosition3f32(worldX, heightAt(x, z + 1), nextWorldZ);
			GXNormal3f32(normalAt(x, z + 1).x, normalAt(x, z + 1).y,
			             normalAt(x, z + 1).z);
			GXTexCoord2f32(getTexPos(x), getTexPos(z + 1));
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

// fabricated: recover the repeated triangle-normal inline boundary in calcNrm.
static inline void calcPlaneNormal(JGeometry::TVec3<f32>& normal,
                                   const JGeometry::TVec3<f32>& first,
                                   const JGeometry::TVec3<f32>& second,
                                   const JGeometry::TVec3<f32>& center)
{
	JGeometry::TVec3<f32> edge1;
	JGeometry::TVec3<f32> edge2;
	edge2.sub(second, center);
	edge1.sub(first, second);
	normal.cross(edge1, edge2);
	normal.normalize();
}

void TMapObjPlane::calcNrm(int x, int z)
{
	if (x < 0 || mExtents <= x || z < 0 || mExtents <= z)
		return;

	f32 h00 = heightAt(x, z);
	JGeometry::TVec3<f32> center(0.0f, h00, 0.0f);

	f32 h0N   = heightAt(x, MsWrap(z - 1, 0, mExtents));
	f32 fVar1 = unkFC;
	JGeometry::TVec3<f32> north(0.0f, h0N, -unkFC);

	f32 h0P = heightAt(x, MsWrap(z + 1, 0, mExtents));
	JGeometry::TVec3<f32> south(0.0f, h0P, fVar1);

	f32 hN0 = heightAt(MsWrap(x - 1, 0, mExtents), z);
	JGeometry::TVec3<f32> west(-unkFC, hN0, 0.0f);

	f32 hP0 = heightAt(MsWrap(x + 1, 0, mExtents), z);
	JGeometry::TVec3<f32> east(unkFC, hP0, 0.0f);

	JGeometry::TVec3<f32> local_9c;
	calcPlaneNormal(local_9c, north, west, center);

	JGeometry::TVec3<f32> local_a8;
	calcPlaneNormal(local_a8, east, north, center);

	JGeometry::TVec3<f32> local_b4;
	calcPlaneNormal(local_b4, west, south, center);

	JGeometry::TVec3<f32> local_c0;
	calcPlaneNormal(local_c0, south, east, center);

	// TODO: JGVec3's fabricated operators omit intermediate value copies.
	JGeometry::TVec3<f32>& normal = mNormalMap[x + z * mExtents];
	normal                        = local_9c + local_a8 + local_b4 + local_c0;
	normal.scale(0.25f);
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

	heightAt(x_00, z_00)
	    = heightAt(x_00, z_00) - rate * ((1.0f - xrem) + (1.0f - zrem));
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

// fabricated: shared BMP dimension decode recovers the target's load order.
static inline int readBmpInt(const u8* bmp, int offset)
{
	int byte0 = bmp[offset];
	int byte1 = bmp[offset + 1];
	int byte2 = bmp[offset + 2];
	int byte3 = bmp[offset + 3];
	return (byte3 << 24) + (byte2 << 16) + (byte1 << 8) + byte0;
}

void TMapObjPlane::makeMountain()
{
	int width  = readBmpInt(unk118, 0x12);
	int height = readBmpInt(unk118, 0x16);

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
