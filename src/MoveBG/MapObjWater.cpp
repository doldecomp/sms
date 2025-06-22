#include <MoveBG/MapObjWater.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjWave.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <System/MarDirector.hpp>
#include <Camera/Camera.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static void dummy(Vec* v)
{
	*v = (Vec) { 0.0f, 0.0f, 0.0f };
	*v = (Vec) { 1.0f, 1.0f, 1.0f };
}

void TMapObjSeaIndirect::perform(u32, JDrama::TGraphics*) { }

void TMapObjSeaIndirect::init()
{
	unk44
	    = SMS_MakeMActorWithAnmData("/common/map/UNDERwater.bmd",
	                                gpMapObjManager->getUnk40(), 3, 0x11210000);

	unk44->setBtk("underwater");
	TScreenTexture* ref
	    = JDrama::TNameRefGen::search<TScreenTexture>("スクリーンテクスチャ");
	const ResTIMG* img = ref->getTexture()->getTexInfo();
	unk44->getUnk4()->getModelData()->getTexture()->setResTIMG(1, *img);
}

TMapObjSeaIndirect::TMapObjSeaIndirect(const char* name)
    : JDrama::TActor(name)
    , unk44(nullptr)
{
}

void TMapObjWaterFilter::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	// TODO: mother of all intern codes...

	if (!unk44 || gpMarDirector->unk124 != 0)
		return;

	bool bVar1 = true;
	if (!gpCamera->isSimpleDemoCamera()
	    && !(gpCamera->mMode == 0x49 ? true : false)) {
		bVar1 = false;
	}

	if (bVar1 ? true : false)
		return;

	if (gpCamera->unk124.y > 0.0f
	    && gpCamera->unk124.y >= gpMapObjWave->getHeight(
	           gpCamera->unk124.x, gpCamera->unk124.y, gpCamera->unk124.z))
		return;

	if (param_1 & 2) {
		J3DTransformInfo info;
		info.mScale.x     = 1.0f;
		info.mScale.y     = 1.0f;
		info.mScale.z     = 1.0f;
		info.mRotation.x  = 0.0f;
		info.mRotation.y  = 0.0f;
		info.mRotation.z  = 0.0f;
		info.mTranslate.x = mPosition.x;
		info.mTranslate.y = mPosition.y;
		info.mTranslate.z = mPosition.z;
		Mtx afStack_78;
		J3DGetTranslateRotateMtx(info, afStack_78);
		Mtx afStack_a8;
		PSMTXScale(afStack_a8, mScaling.x, mScaling.y, mScaling.z);
		Mtx afStack_48;
		MTXInverse(param_2->unkB4, afStack_48);
		MTXConcat(afStack_48, afStack_78, afStack_48);
		MTXConcat(afStack_48, afStack_a8, afStack_48);
		unk44->getUnk4()->setBaseTRMtx(afStack_48);
	}
	unk44->perform(param_1, param_2);
}

void TMapObjWaterFilter::init()
{
	unk44
	    = SMS_MakeMActorWithAnmData("/common/map/UnderWaterFilter.bmd",
	                                gpMapObjManager->getUnk40(), 3, 0x11210000);
}

TMapObjWaterFilter::TMapObjWaterFilter(const char* name)
    : JDrama::TActor(name)
    , unk44(nullptr)
{
}
