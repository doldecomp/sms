#include <Enemy/beam.hpp>
#include <dolphin/gx.h>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JMath.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Map/Map.hpp>

static void coneInPlane(const JGeometry::TVec3<f32>& origin, f32 angle,
                        const JGeometry::TVec3<f32>& axis,
                        const JGeometry::TVec3<f32>& offsetDir,
                        const JGeometry::TPartition3<f32>& plane,
                        JGeometry::TVec3<f32>* outPos)
{
	// Scale perpendicular component by cone opening
	JGeometry::TVec3<f32> dir = offsetDir;
	dir.scale(JMASin(angle));

	// Add the axis direction to get the final ray direction
	dir += axis;

	// Solve for intersection distance with the plane
	f32 t = -(plane.mDist + plane.mNormal.dot(origin)) / plane.mNormal.dot(dir);

	// Compute intersection point
	*outPos = dir;
	outPos->scale(t);
	*outPos += origin;
}

TConeBeam::TConeBeam()
    : mScale(0.0f)
    , unk1C(false)
    , unk1D(true)
{
	unk00.zero();
	unk0C.zero();
	mScale = 0.0f; // duplicate initialization
}

void TConeBeam::drawConeBeamAux(const GXColor& color, bool unk)
{
	// Draw outer?
	GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, mVtxCount + 2);

	GXPosition3f32(unk00.x, unk00.y, unk00.z);

	if (unk) {
		GXColor4u8(color.r, color.g, color.b, 0);
	} else {
		GXColor4u8(color.r, color.g, color.b, color.a);
	}

	for (int i = 0; i <= mVtxCount; i++) {
		const JGeometry::TVec3<f32>& vtx = mVtx[i];
		GXPosition3f32(vtx.x, vtx.y, vtx.z);
		GXColor4u8(color.r, color.g, color.b, color.a);
	}

	GXEnd();

	// Draw inner?
	GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, mVtxCount + 2);

	GXPosition3f32(unk0C.x, unk0C.y, unk0C.z);
	GXColor4u8(color.r, color.g, color.b, color.a);

	for (int i = 0; i <= mVtxCount; i++) {
		const JGeometry::TVec3<f32>& vtx = mVtx[i];
		GXPosition3f32(vtx.x, vtx.y, vtx.z);
		GXColor4u8(color.r, color.g, color.b, color.a);
	}

	GXEnd();
}

// TODO: @non-matching
void TConeBeam::calcVertices(int count)
{
	JGeometry::TVec3<f32> local_134(0.0f, 1.0f, 0.0f);

	JGeometry::TVec3<f32> local_128 = unk0C;
	JGeometry::TVec3<f32> local_140;

	mVtxCount = count;
	local_128.sub(unk00);
	local_140.cross(local_128, local_134);

	if (local_140.isZero()) {
		local_134.set(1.0f, 0.0f, 0.0f);
		local_140.set(0.0f, 0.0f, 1.0f);
	} else {
		local_134.cross(local_140, local_128);

		PSVECNormalize(&local_140, &local_140);
		PSVECNormalize(&local_134, &local_134);
	}

	if (mBGCheckData == nullptr) {
		for (int i = 0; i <= mVtxCount; i++) {
			f32 s = mScale * JMASin(i * (360.0f / mVtxCount)) / 2.0f;
			f32 c = mScale * JMACos(i * (360.0f / mVtxCount)) / 2.0f;

			JGeometry::TVec3<f32> local_11c;
			local_11c.zero();

			local_11c += local_140 * s;
			local_11c += local_134 * c;

			local_11c += unk0C;

			mVtx[i] = local_11c;
		}
	} else {
		JGeometry::TPartition3<f32> partition(mBGCheckData->getNormal(),
		                                      mBGCheckData->getPlaneDistance());
		f32 local_128Len = PSVECMag(&local_128);
		f32 angle        = matan(local_128Len, mScale)
		            * (360.0f / 65536.0f); // this is SHORT2DEGANGLE constant

		PSVECNormalize(&local_128, &local_128);

		for (int i = 0; i <= mVtxCount; i++) {
			f32 sinA = JMASin(i * (360.0f / mVtxCount));
			f32 cosA = JMACos(i * (360.0f / mVtxCount));

			JGeometry::TVec3<f32> local_ec;
			local_ec.zero();

			local_ec += local_140 * cosA;
			local_ec += local_134 * sinA;

			JGeometry::TVec3<f32> local_f8;
			coneInPlane(unk00, angle, local_128, local_ec, partition,
			            &local_f8);
			mVtx[i] = local_f8;
		}
	}
}

void TConeBeam::drawConeBeam(const GXColor& color)
{
	GXLoadPosMtxImm(j3dSys.mViewMtx, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetColorUpdate(GX_FALSE);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetDstAlpha(GX_TRUE, 0);
	GXSetCullMode(GX_CULL_NONE);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_ONE, GX_LO_NOOP);
	GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
	drawConeBeamAux(color, false);

	GXSetDstAlpha(GX_TRUE, 128);
	GXSetCullMode(GX_CULL_BACK);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
	drawConeBeamAux(color, false);

	GXSetColorUpdate(GX_TRUE);
	GXSetAlphaUpdate(GX_FALSE);
	GXSetDstAlpha(GX_FALSE, 0);
	GXSetCullMode(GX_CULL_FRONT);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_ONE, GX_LO_NOOP);
	GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
	drawConeBeamAux(color, false);

	if (unk1C) {
		GXSetCullMode(GX_CULL_BACK);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
		GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
		drawConeBeamAux(color, unk1D);
	}
}

TBeamManager::TBeamManager(const char* name)
    : JDrama::TViewObj(name)
    , mColor((GXColor) { 128, 128, 64, 255 })
    , mBeamVtxCount(10)
    , mBeamCount(0)
{
}

void TBeamManager::setupMaterial()
{
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGB, GX_RGBA8, 0);
	GXSetChanCtrl(GX_COLOR0A0, GX_TRUE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP,
	              GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, mColor);
}

void TBeamManager::drawAllBeam()
{
	setupMaterial();
	for (int i = 0; i < mBeamCount; i++) {
		mBeams[i].drawConeBeam(mColor);
	}
}

void TBeamManager::requestCone(const JGeometry::TVec3<f32>& pos,
                               const JGeometry::TVec3<f32>& dir, f32 scale,
                               bool param_4, bool param_5, bool param_6)
{
	TConeBeam& beam = mBeams[mBeamCount];
	const TBGCheckData* bgCheckData;
	JGeometry::TVec3<f32> intersect;

	beam.unk1C = param_4;
	mBeamCount++;
	if (param_6) {
		bgCheckData = gpMap->intersectLine(pos, dir, false, &intersect);
	} else {
		bgCheckData = nullptr;
	}

	if (bgCheckData == NULL) {
		intersect = dir;
	}

	beam.unk00        = pos;
	beam.unk1D        = param_5;
	beam.unk0C        = intersect;
	beam.mScale       = scale;
	beam.mBGCheckData = bgCheckData;
	beam.calcVertices(mBeamVtxCount);
}

void TBeamManager::perform(u32 param_1, JDrama::TGraphics* graphics)
{
	if (param_1 & 0x8) {
		setupMaterial();
		drawAllBeam();
		mBeamCount = 0;
	}
}
