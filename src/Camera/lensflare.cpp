#include <System/DummyStrings.hpp>
#include <Camera/LensFlare.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JMath.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CameraMarioData.hpp>
#include <Camera/SunMgr.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/SunModel.hpp>
#include <stdio.h>

const char* cSunVolumeName    = "/scene/sun";
const char* cSunsetVolumeName = "/scene/sunset";

// fabricated. Retail calls JMASCos, JMASSin and TVec3<f32>::set(const Vec&)
// out of line from the calc-anim block below, which only happens if the
// CLBCalcNearNinePos fovy/aspect wrapper sits at inline depth 3: the wrapper
// then expands fakeTan at depth 4 but leaves the two table lookups at depth 5,
// and the TVec3 temporaries its arguments need put set() at depth 4 as well.
// Two levels between perform() and the wrapper reproduce that; their names and
// split are guesses, since retail inlined both away completely.
static inline void CalcLensNearNinePosFromCamera(JGeometry::TVec3<f32>* out_grid,
                                                S16Vec* out_euler)
{
	// Both vectors pass as their `Vec` base so the `const TVec3&` parameters
	// take the converting constructor: a stack temporary plus the out-of-line
	// `TVec3::set(const Vec&)` retail calls here (at depth 4), with gpCamera
	// re-read for each. Binding them to named `const Vec&` locals caches the
	// two addresses in callee-saved registers instead (-1.4%), and the
	// TVec3-typed members bind directly with no temporary (-5%).
	CLBCalcNearNinePos(out_grid, out_euler, (const Vec&)gpCamera->unk124,
	                   (const Vec&)gpCamera->unk148,
	                   gpCamera->getFinalAngleZ(), gpCamera->getNear(),
	                   gpCamera->getFovy(), gpCamera->getAspect());
}

static inline void CalcLensNearNinePos(JGeometry::TVec3<f32>* out_grid,
                                       S16Vec* out_euler)
{
	CalcLensNearNinePosFromCamera(out_grid, out_euler);
}

TLensFlare::TLensFlare(const char* name)
    : JDrama::TViewObj(name)
    , unk10(nullptr)
    , unk14(nullptr)
    , unk18(60.0f, 60.0f, 80.0f)
    , unk24(0.0f)
    , unk28(0.0f)
    , unk2C(0.04f)
    , unk30(0.005f)
    , unk34(0.04f)
    , unk38(0.04f)
    , unk3C(30000.0f)
    , unk40(3.0f)
    , unk44(1.75f)
    , unk48(75.0f)
{
	if (gpSunMgr->isThing())
		return;

	// Dead 4-byte local. Retail puts `buf` at 0x18 and leaves one word between
	// it and the outgoing-argument area; without a local declared after `buf`
	// ours lands at 0x14 (every other instruction is identical). The same +4
	// shows up in TLensGlow::TLensGlow and TSunModel::load, the two sibling
	// functions that build a path out of cSunVolumeName, so this is a leftover
	// declaration in that idiom rather than something the function uses.
	// TODO: the original name is unrecoverable; an assigned local is register
	// allocated and does not reserve the slot, so it was never written to.
	char buf[0x100];
	int pathLen;

	snprintf(buf, 0x100, "%s/%s", cSunVolumeName, "sun_lensfx.bmd");

	unk10 = J3DModelLoaderDataBase::load(JKRGetResource(buf),
	                                     J3DMLF_MaterialPEFull
	                                         | (2 << J3DMLF_TevStageNumShift));
	unk14 = new J3DModel(unk10, 0, 1);
}

static inline void LensDirTo(JGeometry::TVec3<f32>& dir, const Vec& from,
                             const JGeometry::TVec3<f32>& to)
{
	JGeometry::TVec3<f32> f(from);
	dir.x = to.x - f.x;
	dir.y = to.y - f.y;
	dir.z = to.z - f.z;
}

static inline JGeometry::TVec3<f32> LensRotTo(const Vec& from,
                                             const JGeometry::TVec3<f32>& to)
{
	JGeometry::TVec3<f32> dir;
	LensDirTo(dir, from, to);
	return MsGetRotFromZaxis(dir);
}

static inline void LensSetTRS(Mtx mtx, const Vec& t,
                              const JGeometry::TVec3<f32>& r,
                              const JGeometry::TVec3<f32>& s)
{
	s16 rx = CLBDegToShortAngle(r.x);
	s16 ry = CLBDegToShortAngle(r.y);
	MsMtxSetTRS(mtx, t.x, t.y, t.z, rx, ry, 0, s.x, s.y, s.z);
}

void TLensFlare::perform(u32 cue, JDrama::TGraphics*)
{
	if (gpSunMgr->isThing())
		return;

	bool sunInBounds;
	if (gpCameraMario->isMarioIndoor()) {
		sunInBounds = false;
	} else {
		sunInBounds = gpSunModel->isInBounds(unk40);
	}

	if (cue & CUE_MOVE) {
		if (!gpSunModel->isInBounds(unk44)) {
			unk28 = 0.0f;
		} else {
			int hiddenCount = 0;
			const JGeometry::TVec2<s16>* zBuffer = gpSunModel->unkB4;
			const bool* visible               = gpSunModel->unk180;
			for (int i = 0; i < 17; ++i, ++zBuffer, ++visible) {
				if (zBuffer->x != -1 && zBuffer->y != -1 && !*visible)
					++hiddenCount;
			}
			f32 hiddenRatio = hiddenCount * (1.0f / 17.0f);

			unk28 = CLBEaseOutInbetween<f32>(unk48 * (1.0f - hiddenRatio),
			                                 255.0f, gpSunModel->getUnk194());
		}

		f32 chase;
		if (unk24 < unk28) {
			if (gpSunModel->unk194 == 0.0f)
				chase = unk30;
			else
				chase = unk2C;
		} else {
			if (gpSunModel->unk194 == 0.0f)
				chase = unk38;
			else
				chase = unk34;
		}
		CLBChaseDecrease(&unk24, unk28, chase, 0.0f);
	}

	if (!sunInBounds)
		return;

	if (cue & CUE_CALC_ANIM) {
		Vec sunWorldPos = gpSunModel->unk198;

		S16Vec camEuler;
		JGeometry::TVec3<f32> near9grid[9];
		CalcLensNearNinePos(near9grid, &camEuler);

		const JGeometry::TVec2<f32>& sp = gpSunModel->unkF8[0];
		f32 tx = unk3C * -sp.x;
		f32 ty = unk3C * -sp.y;
		JGeometry::TVec3<f32> d5;
		d5.sub(near9grid[5], near9grid[4]);
		d5.scale(tx);
		JGeometry::TVec3<f32> d1;
		d1.sub(near9grid[1], near9grid[4]);
		d1.scale(ty);
		JGeometry::TVec3<f32> l;
		l.add(near9grid[4], d5);
		l.add(d1);
		// TODO: 94.8%. The lerp is two scaled difference vectors added to
		// grid[4] (products and sums as separate fmuls/fadds, no fmadds);
		// the sun position is a plain `Vec` copy (lwz/stw), and the direction
		// helpers put its conversion to `TVec3` at depth 4 so `set(const
		// Vec&)` is the third retail `bl`. Left: (a) the isInBounds f0/f1
		// swap shared with TLensGlow::perform; (b) r3/r4/r5 rotation in the
		// hidden-count loop and the float regs of hiddenRatio; (c) in the
		// MsMtxSetTRS tail retail loads its constants first and `unk18`/the
		// sun position last; (d) the frame is 0xb0 short (0x208 vs 0x2b8)
		// and `this` is r28 instead of r29.
		JGeometry::TVec3<f32> rot = LensRotTo(sunWorldPos, l);
		Mtx mtx;
		LensSetTRS(mtx, sunWorldPos, rot, unk18);
		unk14->setBaseTRMtx(mtx);
		unk14->calc();
	}

	if (cue & CUE_ENTRY) {
		int matCount = unk10->getMaterialNum();
		for (u16 i = 0; i < matCount; ++i) {
			unk10->getMaterialNodePointer(i)->change();
			J3DGXColorS10 c;
			c         = *unk10->getMaterialNodePointer(i)->getTevColor(0);
			c.color.a = unk24;
			unk10->getMaterialNodePointer(i)->setTevColor(0, &c);
		}
		unk14->entry();
	}

	if (cue & CUE_CALC_VIEW)
		unk14->viewCalc();
}
