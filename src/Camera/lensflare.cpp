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
	const Vec& camPos = gpCamera->unk124;
	const Vec& camAt  = gpCamera->unk148;
	CLBCalcNearNinePos(out_grid, out_euler, camPos, camAt,
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
			f32 hiddenCount = gpSunModel->calcHiddenRatio();

			unk28 = CLBEaseOutInbetween<f32>(unk48 * (1.0f - hiddenCount),
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
		const Vec& sunPosSrc              = gpSunModel->unk198;
		JGeometry::TVec3<f32> sunWorldPos = sunPosSrc;

		S16Vec camEuler;
		JGeometry::TVec3<f32> near9grid[9];
		CalcLensNearNinePos(near9grid, &camEuler);

		f32 tx = unk3C * -gpSunModel->unkF8[0].x;
		f32 ty = unk3C * -gpSunModel->unkF8[0].y;
		f32 lx = near9grid[4].x + (near9grid[5].x - near9grid[4].x) * tx
		         + (near9grid[1].x - near9grid[4].x) * ty;
		f32 ly = near9grid[4].y + (near9grid[5].y - near9grid[4].y) * tx
		         + (near9grid[1].y - near9grid[4].y) * ty;
		f32 lz = near9grid[4].z + (near9grid[5].z - near9grid[4].z) * tx
		         + (near9grid[1].z - near9grid[4].z) * ty;

		// TODO (header round 25): the three-deep chain above is doing its
		// job -- two of retail's three `bl TVec3<f32>::set(const Vec&)`
		// sites, plus both `bl JMAS{Cos,Sin}`, reproduce exactly. The third
		// `bl set` is this one, and it is lost to the lerp above, not to a
		// depth problem: retail computes the three components with separate
		// `fmuls`/`fadds` pairs (six adds, f29/f30/f31 as three named scalar
		// locals) and only then calls `set`, while this spelling lets
		// fp_contract fuse every product into `fmadds` and then folds the
		// `set` away with it. The multiplicands retail uses are reloaded from
		// the stack between the subtraction and the product, so the deltas
		// were stored -- i.e. retail built the two difference *vectors* as
		// objects and scaled them, rather than writing nine scalar
		// expressions. That is the next thing to try here, and it should also
		// account for the four extra callee-saved FPRs (we name seven float
		// locals in this body, retail three) and most of the 0x90 frame gap.
		JGeometry::TVec3<f32> finalPos;
		finalPos.set(sunWorldPos);

		JGeometry::TVec3<f32> dir(lx - finalPos.x, ly - finalPos.y,
		                          lz - finalPos.z);

		JGeometry::TVec3<f32> rot = MsGetRotFromZaxis(dir);
		Mtx mtx;
		// Wrong! Need a different inline wrapper!
		MsMtxSetTRS(mtx, sunWorldPos.x, sunWorldPos.y, sunWorldPos.z,
		            CLBDegToShortAngle(rot.x), CLBDegToShortAngle(rot.y), 0,
		            unk18.x, unk18.y, unk18.z);
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
