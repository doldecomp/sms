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
	if (gpSunMgr->isThing() != false)
		return;

	// TODO: nonmatching .rodata offset: retail strips the duplicate volume
	// strings from SunModel.hpp (mario.MAP: @1633, @1634).
	char buf[0x100];
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
			f32 hiddenCount  = gpSunModel->calcHiddenRatio();
			f32 visibleRatio = 1.0f - hiddenCount;

			unk28 = CLBEaseOutInbetween<f32>(unk48 * visibleRatio, 255.0f,
			                                 gpSunModel->getUnk194());
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
		// TODO: nonmatching camera/vector inline boundaries and stack frame.
		Vec sunWorldPos = gpSunModel->unk198;

		JGeometry::TVec3<f32> near9grid[9];
		S16Vec euler;
		CLBCalcNearNinePos(near9grid, &euler, gpCamera->unk124,
		                   gpCamera->unk148, gpCamera->getFinalAngleZ(),
		                   gpCamera->getNear(), gpCamera->getFovy(),
		                   gpCamera->getAspect());

		f32 tx = unk3C * -gpSunModel->unkF8[0].x;
		f32 ty = unk3C * -gpSunModel->unkF8[0].y;
		JGeometry::TVec3<f32> right;
		right.sub(near9grid[5], near9grid[4]);
		right.scale(tx);
		JGeometry::TVec3<f32> up;
		up.sub(near9grid[1], near9grid[4]);
		up.scale(ty);
		JGeometry::TVec3<f32> lensPos;
		lensPos.add(near9grid[4], right);
		lensPos.add(up);

		JGeometry::TVec3<f32> finalPos;
		finalPos.set(sunWorldPos);

		JGeometry::TVec3<f32> dir;
		dir.sub(lensPos, finalPos);

		JGeometry::TVec3<f32> rot = MsGetRotFromZaxis(dir);
		s16 angleX                = CLBDegToShortAngle(rot.x);
		s16 angleY                = CLBDegToShortAngle(rot.y);
		Mtx mtx;
		MsMtxSetTRS(mtx, sunWorldPos.x, sunWorldPos.y, sunWorldPos.z, angleX,
		            angleY, 0, unk18.x, unk18.y, unk18.z);
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
