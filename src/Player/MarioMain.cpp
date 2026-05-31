#include <Player/Mario.hpp>
#include <Player/MarioCap.hpp>
#include <Player/WaterGun.hpp>
#include <Player/Yoshi.hpp>
#include <System/Resolution.hpp>
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
	if (unk14E > 0) {
		unk14E -= 1;
		if (unk14E <= 0)
			unk150 = mDeParams.mNoFreezeTime.get();
	}

	if (unk150 > 0)
		unk150 -= 1;
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
		    0x1);
	} else {
		mModel->getModel()->getModelData()->getShapeNodePointer(10)->onFlag(
		    0x1);
	}
}

void TMario::perform(u32 param_1, JDrama::TGraphics* graphics)
{
	if (unk114 & UNK114_FLAG_PROFILE)
		TTimeRec::startTimer(0xff, 0x00, 0x00, 0x80);

	if (checkFlag(MARIO_FLAG_IS_PERFORMING))
		return;

	if (param_1 & 1) {
		thinkFreeze();

		if (unk14E <= 0) {
			playerControl(graphics);
			setPositions();
			if (mCap != nullptr)
				mCap->perform(1, graphics);
			if (mWaterGun != nullptr)
				mWaterGun->perform(1, graphics);
			if (mYoshi != nullptr)
				mYoshi->movement();
			moveParticle();
		}

		thinkCube();
		soundMovement();
	}

	if ((param_1 & 1) && unk14E <= 0) {
		thinkAloha();
		calcAnim(2, graphics);
		animSound();

		if (mWaterGun != nullptr) {
			mWaterGun->setBaseTRMtx(mModel->unk8->mNodeMatrices[mBoneIDs[0]]);
			mWaterGun->perform(2, graphics);
		}

		if (mYoshi != nullptr)
			mYoshi->calcAnim();
	}

	if (param_1 & 0x4) {
		calcView(graphics);
		if (mWaterGun != nullptr)
			mWaterGun->perform(0x4, graphics);
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

	if (param_1 & 0x200) {
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
				mWaterGun->perform(0x200, graphics);

			entryModels(graphics);
			mYoshi->entry();

			BOOL hasUnk368 = unk368 > 0.0f ? TRUE : FALSE;
			if (hasUnk368 == FALSE)
				((TMBindShadowBody*)unk390)->entryDrawShadow();
		} else if (!onYoshi()) {
			mYoshi->entry();
		}
	}

	if (param_1 & 0x04000000)
		if (checkFlag(MARIO_FLAG_HAS_FLUDD))
			mWaterGun->perform(0x04000000, graphics);

	if (param_1 & 0x10000000) {
		unk394->frameInit();
		unk398->frameInit();

		unk39C = j3dSys.mDrawBuffer[0];
		unk3A0 = j3dSys.mDrawBuffer[1];

		j3dSys.mDrawBuffer[0] = unk394;
		j3dSys.mDrawBuffer[1] = unk398;

		mTrembleModelEffect->movement();
		mCap->perform(0x10000000, graphics);
	}

	if (param_1 & 0x08000000) {
		j3dSys.mDrawBuffer[0] = unk39C;
		j3dSys.mDrawBuffer[1] = unk3A0;
	}

	if (param_1 & 0x40000000) {
		if (checkUnk114(UNK114_FLAG_UNK10)) {
			j3dSys.setUnk4C(3);
			unk394->draw();
			mYoshi->unkA8->draw();
		}
	}

	if (param_1 & 0x20000000) {
		if (checkUnk114(UNK114_FLAG_UNK10)) {
			j3dSys.setUnk4C(4);
			unk398->draw();
			mYoshi->unkAC->draw();
		}
	}

	if (param_1 & 0x01000000)
		drawSpecial(graphics);

	if (param_1 & 0x02000000) {
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

	if (param_1 & 0x00800000) {
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

	if ((param_1 & 0x80000000) && (unk114 & UNK114_FLAG_VISIBLE)) {
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

	u32 local_1c;
	GXPeekARGB(mMarioScreenPos.x, mMarioScreenPos.y, &local_1c);
	if ((local_1c & 0xff000000) == 0x10000000) {
		offFlag(MARIO_FLAG_OCCLUDED);
	} else {
		onFlag(MARIO_FLAG_OCCLUDED);
	}
}
