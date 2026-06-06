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
	if (gpSunMgr->isThing())
		return;

	char buf[0x100];
	snprintf(buf, 0x100, "%s/%s", cSunVolumeName, "sun_lensfx.bmd");

	unk10 = J3DModelLoaderDataBase::load(JKRGetResource(buf), 0x10020000);
	unk14 = new J3DModel(unk10, 0, 1);
}

void TLensFlare::perform(u32 param_1, JDrama::TGraphics*)
{
	if (gpSunMgr->isThing())
		return;

	bool sunInBounds;
	if (gpCameraMario->isMarioIndoor()) {
		sunInBounds = false;
	} else {
		sunInBounds = gpSunModel->isInBounds(unk40);
	}

	if (param_1 & 1) {
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

	if (param_1 & 2) {
		JGeometry::TVec3<f32> sunWorldPos = gpSunModel->unk198;

		// TODO: a mystery is happening here with the args, but it's definitely
		// this inline (maybe one more inlining layer?)
		JGeometry::TVec3<f32> near9grid[9];
		CLBCalcNearNinePos(near9grid, nullptr, gpCamera->unk124,
		                   gpCamera->mTarget, gpCamera->getFinalAngleZ(),
		                   gpCamera->getNear(), gpCamera->getFovy(),
		                   gpCamera->getAspect());

		f32 tx = unk3C * -gpSunModel->unkF8[0].x;
		f32 ty = unk3C * -gpSunModel->unkF8[0].y;
		f32 lx = near9grid[4].x + (near9grid[5].x - near9grid[4].x) * tx
		         + (near9grid[1].x - near9grid[4].x) * ty;
		f32 ly = near9grid[4].y + (near9grid[5].y - near9grid[4].y) * tx
		         + (near9grid[1].y - near9grid[4].y) * ty;
		f32 lz = near9grid[4].z + (near9grid[5].z - near9grid[4].z) * tx
		         + (near9grid[1].z - near9grid[4].z) * ty;

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

	if (param_1 & 0x200) {
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

	if (param_1 & 4)
		unk14->viewCalc();
}
