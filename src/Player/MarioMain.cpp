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

// TODO: the frame is now exact (0x168) and every instruction matches; the only
// residue left is a permutation of two slots in the named block. Retail lays
// it out, top down, f64 0x148 / `dir` 0x13c / startTimer's four-byte colour
// 0x138 / three scalars 0x134, 0x130, 0x12c / an 8-byte hole 0x124 / a 12-byte
// vector 0x118; we put the colour *below* the three scalars (0x128) and the
// vector 4 lower (0x114), with our own 8-byte hole at 0x120. The sizes all
// agree, so the colour temporary of the very first statement wants to be
// allocated before the two later colour conversions instead of after them.
// TTimeRec::startTimer has one overload and its temporary placement is a
// TimeRec.hpp question with its own trial table, so it is not a call-site fix.
// Closure batch 205 measured that table's remaining rows against *this*
// caller (frame / colour slot; the target is 0x168 / 0x138 and we are at
// 0x168 / 0x128).  Inert, still 0x168 / 0x128: declaring `color` before
// `inst` inside startTimer, and declaring both `color` and `col` before it.
// Worse: an unnamed `JUtility::TColor(r, g, b, a)` temporary is +8 of frame
// (0x170), and `TColor color; color.set(r, g, b, a);` costs two instructions.
// A TU-local level around the call (`MarioMainStartTimer()`) is not neutral
// here either -- it adds three instructions and drops the function to 98.4%.
// So the colour's rank among this function's four-byte pool temporaries is
// not reachable from startTimer's statement list at all: our descending pool
// is GXSetChanMatColor#1, #2, startTimer, retail's is startTimer, #1, #2,
// i.e. retail's is expansion order and ours puts the first expansion last.
//
// The 104 bytes of dead low region the frame needed were measured in closure
// batch 120: the parked MarioMainGetFludd binding level below is +16 of low
// region per expansion at eight of the nine `mWaterGun` sites (+0x60; all nine
// is +0x70 and overshoots, and dropping any one of the nine gives the same
// 0x160, so which site retail spelled differently is not determined), and
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
			if (MarioMainGetFludd(this) != nullptr)
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
			mWaterGun->perform(CUE_SEMITRANSPARENT_PRIO_1, graphics);

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
		GXSetChanMatColor(GX_COLOR0A0, gpSilhouetteManager->unk12);
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
