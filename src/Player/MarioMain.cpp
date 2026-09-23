#include <Player/Mario.hpp>
#include <Player/MarioCap.hpp>
#include <Player/WaterGun.hpp>
#include <Player/Yoshi.hpp>
#include <System/Resolution.hpp>
#include <System/MarDirector.hpp>
#include <System/TimeRec.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <M3DUtil/M3UModelMario.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <dolphin/mtx.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static JGeometry::TVec3<f32> cDeformedTerrainCenter(0.0f, 5000.0f, 0.0f);

TMario* gpMarioOriginal;

BOOL TMario::isMario() { return gpMarioOriginal == this ? TRUE : FALSE; }

void TMario::thinkFreeze()
{
	if (mFreezeTimer > 0) {
		mFreezeTimer -= 1;
		if (mFreezeTimer <= 0)
			mFreezeImmunityTimer = mDeParams.mNoFreezeTime.get();
	}

	if (mFreezeImmunityTimer > 0)
		mFreezeImmunityTimer -= 1;
}

void TMario::thinkCube()
{
	if (!isMario())
		return;

	JGeometry::TVec3<f32> pos = mPosition;
	pos.y += 75.0f;
	TCubeManagerArea* mgrArea = gpCubeArea;
	mgrArea->unk1C            = mgrArea->getInCubeNo(pos);
	TCubeManagerFast* mgrA    = gpCubeFastA;
	mgrA->unk1C               = mgrA->getInCubeNo(pos);
	TCubeManagerFast* mgrB    = gpCubeFastB;
	mgrB->unk1C               = mgrB->getInCubeNo(pos);
	TCubeManagerFast* mgrC    = gpCubeFastC;
	mgrC->unk1C               = mgrC->getInCubeNo(pos);
}

void TMario::thinkAloha()
{
	if (checkFlag(MARIO_FLAG_HAS_SHIRT)) {
		mModel->getModel()->getModelData()->getShapeNodePointer(10)->offFlag(
		    J3DShpFlag_Visible);
	} else {
		mModel->getModel()->getModelData()->getShapeNodePointer(10)->onFlag(
		    J3DShpFlag_Visible);
	}
}

// TODO: frame exact (0x168), every instruction exact, and since the silhouette
// colour is read through a named `TSilhouette* sil` every slot but two is
// retail's: `dir` 0x13c and the three GXSetChanMatColor argument temporaries
// 0x134/0x130/0x12c now land.  Left: startTimer's inlined TColor sits at 0x128
// (below those temporaries) where retail has it at 0x138 (above them, where we
// leave a dead word). thinkCube's `pos` (4 low, 0x114 vs 0x118) landed when the
// one unbound fludd site moved from the CUE_SEMITRANSPARENT_PRIO_1 call to the
// first CUE_MOVE null test (the first CUE_MOVE call site is equivalent; every
// later site keeps `pos` low).
// Retail allocates the colour first among the function's argument/compiler
// temporaries; ours allocates it last.  Inert or worse against this caller
// (the colour never rises above the GX temporaries): `Vec dir`, dir at
// function scope, `dir.set(...)`/ctor (+4 instructions), named, static or
// TColor-built GX colours (the named `GXColor color = unk12` and a
// `const GXColor&` are equivalent to `sil`), checkUnk114 at either profile
// test (+5), a block around startTimer, and in TimeRec.hpp: colour declared
// before `inst`, `toUInt32()`, an unnamed TColor (+0x10 frame), copy-init
// (+4), dropping `col`, a u32-built colour, a TU-local level around the call
// (+3 instructions). Our dead word at 0x138 is exactly retail's colour slot and
// survives raw `mWaterGun` (frame 0x108), a union colour, and dropping
// `timeArray`/`tick`/`col` or declaring the colour first (pairs too).
//
// Diagnosed (header research c-tcolor): this is not a JUTColor stride.  The
// dead word at 0x138 is the named slot of `sil` (a caller-level pointer takes
// a named slot even when register-held), and retail's colour is a *call-site*
// temporary, created while the body is parsed and so allocated above the GX
// argument temporaries, not an inlined local in the deferred-inline pool.
// With `sil` dropped and a TU-local `startTimer(const JUtility::TColor&)`
// called as `(JUtility::TColor(0xff, 0x00, 0x00, 0x80))`, every slot is
// retail's shifted 4 low (dir 0x138, colour 0x134): retail has 4 more bytes
// between thinkCube's `pos` and the last argument temporary.  Two ways to
// supply them, both refused:
//   - a copy alias `TTimeArray* ary = timeArray;` in that helper (+4 at the
//     top of the pool): 100.0%, frame and every slot exact -- a fabricated
//     local;
//   - the endTimer site as a call-site temporary too, `(JUtility::TColor(0))`:
//     every slot lands but the temporary's stw/lwz survives (+3 instructions,
//     99.3%) where retail has a bare `li r5, 0`.
// Inert (layout = the 4-low one): a by-value TColor parameter (0x170), the
// u32 overload fed a TColor (0x170/0x180), `instance()` (0x170), forwarding
// to appendTime/a u32 helper (pos moves instead), `inst == nullptr`, `const
// u32 col`, timeArray declared early, as a reference, or before OSGetTick.
// So retail's endTimer (or something parsed after the silhouette colour)
// owns one dead 4-byte temporary whose value folds to a constant; its
// spelling is the open lead.
//
// The 104 bytes of dead low region the frame needed were measured in closure
// batch 120: the parked MarioMainGetFludd binding level below is +16 of low
// region per expansion at eight of the nine `mWaterGun` sites (+0x60; all nine
// is +0x70 and overshoots; the frame does not say which site is raw, but the
// slot of thinkCube's `pos` does -- see above), and
// `getM3UModel()` at the setBaseTRMtx site supplies the last +8. The real
// accessor `getFludd()` at all nine sites is only +16 in total, so the missing
// level is a binding one, not the accessor.
// Equivalent to the getM3UModel rung and also landing 0x168: a parked yoshi
// binding level at the `mYoshi->movement()` site alone. Overshoot: a parked
// cap binding (0x178), a parked yoshi binding at all ten sites (0x1b0).
// checkUnk114() for the raw `unk114 &` tests is wrong -- it materialises a
// bool retail does not have. A getShadow() wrapper over unk390 is worth zero.
static inline TWaterGun* MarioMainGetFludd(TMario* mario)
{
	TWaterGun* fludd = mario->getFludd();
	return fludd;
}

void TMario::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (unk114 & UNK114_FLAG_PROFILE)
		TTimeRec::startTimer(0xff, 0x00, 0x00, 0x80);

	if (checkFlag(MARIO_FLAG_IS_PERFORMING))
		return;

	if (cue & CUE_MOVE) {
		thinkFreeze();

		if (mFreezeTimer <= 0) {
			playerControl(graphics);
			setPositions();
			if (mCap != nullptr)
				mCap->perform(CUE_MOVE, graphics);
			if (mWaterGun != nullptr)
				MarioMainGetFludd(this)->perform(CUE_MOVE, graphics);
			if (mYoshi != nullptr)
				mYoshi->movement();
			moveParticle();
		}

		thinkCube();
		soundMovement();
	}

	if ((cue & CUE_MOVE) && mFreezeTimer <= 0) {
		thinkAloha();
		calcAnim(CUE_CALC_ANIM, graphics);
		animSound();

		if (MarioMainGetFludd(this) != nullptr) {
			MarioMainGetFludd(this)->setBaseTRMtx(
			    getM3UModel()->getModel()->getAnmMtx(mJointIdChest));
			MarioMainGetFludd(this)->perform(CUE_CALC_ANIM, graphics);
		}

		if (mYoshi != nullptr)
			mYoshi->calcAnim();
	}

	if (cue & CUE_CALC_VIEW) {
		calcView(graphics);
		if (MarioMainGetFludd(this) != nullptr)
			MarioMainGetFludd(this)->perform(CUE_CALC_VIEW, graphics);
		if (mYoshi != nullptr)
			mYoshi->viewCalc();

		if (this == gpMarioOriginal) {
			JGeometry::TVec3<f32> dir;
			dir.x = JMASSin(mFaceAngle.y);
			dir.y = 0.0f;
			dir.z = JMASCos(mFaceAngle.y);
			PSMTXMultVecSR(graphics->mViewMtx, &dir, &unk4F0);
		}
	}

	if (cue & CUE_ENTRY) {
		// TODO: inline?
		BOOL doEntry = TRUE;
		if (!(unk114 & UNK114_FLAG_VISIBLE))
			doEntry = FALSE;
		if (checkFlag(MARIO_FLAG_UNK4))
			doEntry = FALSE;

		if (doEntry == TRUE) {
			addDirty();
			addDamageFog(graphics);

			if (checkFlag(MARIO_FLAG_HAS_FLUDD))
				MarioMainGetFludd(this)->perform(CUE_ENTRY, graphics);

			entryModels(graphics);
			mYoshi->entry();

			if (isSinking() == FALSE)
				unk390->entryDrawShadow();
		} else if (!onYoshi()) {
			mYoshi->entry();
		}
	}

	if (cue & CUE_SEMITRANSPARENT_PRIO_1)
		if (checkFlag(MARIO_FLAG_HAS_FLUDD))
			MarioMainGetFludd(this)->perform(CUE_SEMITRANSPARENT_PRIO_1, graphics);

	if (cue & CUE_UNK10000000) {
		unk394->frameInit();
		unk398->frameInit();

		unk39C = j3dSys.mDrawBuffer[0];
		unk3A0 = j3dSys.mDrawBuffer[1];

		j3dSys.mDrawBuffer[0] = unk394;
		j3dSys.mDrawBuffer[1] = unk398;

		mTrembleModelEffect->movement();
		mCap->perform(CUE_UNK10000000, graphics);
	}

	if (cue & CUE_UNK8000000) {
		j3dSys.mDrawBuffer[0] = unk39C;
		j3dSys.mDrawBuffer[1] = unk3A0;
	}

	if (cue & CUE_UNK40000000) {
		if (checkUnk114(UNK114_FLAG_UNK10)) {
			j3dSys.setUnk4C(3);
			unk394->draw();
			mYoshi->unkA8->draw();
		}
	}

	if (cue & CUE_UNK20000000) {
		if (checkUnk114(UNK114_FLAG_UNK10)) {
			j3dSys.setUnk4C(4);
			unk398->draw();
			mYoshi->unkAC->draw();
		}
	}

	if (cue & CUE_UNK1000000)
		drawSpecial(graphics);

	if (cue & CUE_SEMITRANSPARENT_PRIO_2) {
		if (checkUnk114(UNK114_FLAG_DO_OCCLUSION_PROBE)) {
			boxDrawPrepare(graphics->mViewMtx);
			GXSetColorUpdate(GX_FALSE);
			GXSetAlphaUpdate(GX_TRUE);
			GXSetDstAlpha(GX_ENABLE, 0x10);
			GXDrawCube();
			GXSetColorUpdate(GX_TRUE);
			GXSetAlphaUpdate(GX_FALSE);
			GXSetDstAlpha(GX_DISABLE, 0);
		}
	}

	if (cue & CUE_UNK800000) {
		if (checkUnk114(UNK114_FLAG_DO_OCCLUSION_PROBE)) {
			boxDrawPrepare(graphics->mViewMtx);
			GXSetColorUpdate(GX_FALSE);
			GXSetAlphaUpdate(GX_TRUE);
			GXSetDstAlpha(GX_ENABLE, 0);
			GXDrawCube();
			GXSetColorUpdate(GX_TRUE);
			GXSetAlphaUpdate(GX_FALSE);
			GXSetDstAlpha(GX_DISABLE, 0);
		}
	}

	if ((cue & CUE_UNK80000000) && (unk114 & UNK114_FLAG_VISIBLE)) {
		j3dSys.onFlag(0x2);
		GXSetChanMatColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		              GX_COLOR0A0);
		GXSetZCompLoc(GX_TRUE);
		GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
		GXSetColorUpdate(GX_FALSE);
		GXSetAlphaUpdate(GX_TRUE);
		GXSetDstAlpha(GX_ENABLE, gpSilhouetteManager->unk48);
		j3dSys.setUnk4C(3);
		unk394->draw();
		j3dSys.setUnk4C(4);
		unk398->draw();
		boxDrawPrepare(graphics->mViewMtx);
		TSilhouette* sil = gpSilhouetteManager;
		GXSetChanMatColor(GX_COLOR0A0, sil->unk12);
		GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_INVDSTALPHA,
		               GX_LO_NOOP);
		GXSetColorUpdate(GX_TRUE);
		GXSetAlphaUpdate(GX_TRUE);
		GXSetDstAlpha(GX_ENABLE, 0);
		GXDrawCube();
		j3dSys.offFlag(0x2);
	}

	if (unk114 & UNK114_FLAG_PROFILE)
		TTimeRec::endTimer();
}

void TMario::drawSyncCallback(u16)
{
	if (!checkUnk114(UNK114_FLAG_DO_OCCLUSION_PROBE))
		return;

	if (mMarioScreenPos.x < 0.0f || mMarioScreenPos.y < 0.0f
	    || mMarioScreenPos.x >= (u16)SMSGetGameRenderWidth()
	    || mMarioScreenPos.y >= (u16)SMSGetGameRenderHeight()) {
		offFlag(MARIO_FLAG_OCCLUDED);
		return;
	}

	// Dead 4-byte local: retail's peek target is at 0x14(r1) with the word at
	// 0x10 reserved below it, which only a local declared after it can do.
	u32 local_1c;
	u32 unusedPeek;

	GXPeekARGB(mMarioScreenPos.x, mMarioScreenPos.y, &local_1c);
	if ((local_1c & 0xff000000) == 0x10000000) {
		offFlag(MARIO_FLAG_OCCLUDED);
	} else {
		onFlag(MARIO_FLAG_OCCLUDED);
	}
}
