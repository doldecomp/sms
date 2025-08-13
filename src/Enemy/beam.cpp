#include <Enemy/beam.hpp>
#include <dolphin/gx.h>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JMath.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Map/Map.hpp>

// these names are just guesses
bool coneInPlane(
    const JGeometry::TVec3<float>& apex,
    float radiusOrAngle,
    const JGeometry::TVec3<float>& dir,
    const JGeometry::TVec3<float>& upDir,
    const JGeometry::TPartition3<float>& plane,
    JGeometry::TVec3<float>* outIntersection
)
{
    float t = -(plane.mDist
                + plane.mNormal.x * apex.x
                + plane.mNormal.y * apex.y
                + plane.mNormal.z * apex.z)
              / (plane.mNormal.x * dir.x
                 + plane.mNormal.y * dir.y
                 + plane.mNormal.z * dir.z);

    outIntersection->scaleAdd(t, dir, apex);
    return true;
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

void TConeBeam::calcVertices(int count)
{
	JGeometry::TVec3<f32> rightVec;
	JGeometry::TVec3<f32> frontVec;
	JGeometry::TVec3<f32> upVec;

	upVec.set(0.0f, 1.0f, 0.0f);
	mVtxCount = count;

	JGeometry::TVec3<f32> dPos = unk0C;
	dPos.sub(unk00);
	rightVec.cross(dPos, upVec);
	if (rightVec.isZero()) {
		frontVec.set(1.0f, 0.0f, 0.0f);
		rightVec.set(0.0f, 0.0f, 1.0f);
	} else {
		frontVec.cross(rightVec, dPos);

		PSVECNormalize(&rightVec, &rightVec);
		PSVECNormalize(&frontVec, &frontVec);
	}

	if (mBGCheckData == nullptr) {
		for (int i = 0; i <= count; i++) {
			f32 scale = mScale;
			f32 s     = scale * JMASin(360.0f / mVtxCount);
			f32 c     = scale * JMACos(360.0f / mVtxCount);

			JGeometry::TVec3<f32> v0 = rightVec;
			v0.scale(s);
			// v0.add(v0);

			// JGeometry::TVec3<f32> v1;
			// v1.zero();
			// tmpScaleAdd(v1, s, frontVec);
			JGeometry::TVec3<f32> v1 = frontVec;
			v1.scale(c);
			v1.add(v1);

			mVtx[i] = unk0C;
			mVtx[i].add(v0);
			mVtx[i].add(v1);
		}
	} else {
		JGeometry::TPartition3<f32> plane(mBGCheckData->mNormal, mBGCheckData->mPlaneDistance);

		// Magnitude of dPos used for angle calc
		f32 dPosLen = PSVECMag(&dPos);

		// Some pitch angle? (atan2-like)
		s16 pitchIndex = matan(dPosLen, mScale);

		// Lookup scale factor from pitch
		f32 angle = pitchIndex * (360.0f / (f32)mVtxCount);

		PSVECNormalize(&dPos, &dPos);

		// f32 pitchScale = JMASin(angle);

		// Loop over ring vertices
		for (int i = 0; i <= mVtxCount; i++) {
			f32 sinA = JMASin(i * (360.0f / mVtxCount)) * 0.5f;
			f32 cosA = JMACos(i * (360.0f / mVtxCount)) * 0.5f;

			JGeometry::TVec3<f32> rightOffset = rightVec;
			rightOffset.scale(cosA);

			JGeometry::TVec3<f32> frontOffset = frontVec;
			frontOffset.scale(sinA);

			JGeometry::TVec3<f32> ringDir = rightOffset;
			ringDir.add(frontOffset);

			f32 pitchScale = JMASin(angle);

			// Apply pitch scale and offset along dPos
			JGeometry::TVec3<f32> point;
			point.set(ringDir.x * pitchScale + dPos.x,
					ringDir.y * pitchScale + dPos.y,
					ringDir.z * pitchScale + dPos.z);

			// Plane intersection
			coneInPlane(unk00, pitchScale, point, upVec, plane, &mVtx[i]);
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
