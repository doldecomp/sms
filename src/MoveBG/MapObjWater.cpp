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
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

// dummy: emits @2520 and @2522.
static void dummy(Vec* v)
{
	*v = (Vec) { 0.0f, 0.0f, 0.0f };
	*v = (Vec) { 1.0f, 1.0f, 1.0f };
}

void TMapObjSeaIndirect::perform(u32 cue, JDrama::TGraphics* graphics) { }

void TMapObjSeaIndirect::init()
{
	unk44 = SMS_MakeMActorWithAnmData(
	    "/common/map/UNDERwater.bmd", gpMapObjManager->getUnk40(), 3,
	    J3DMLF_MaterialPEFull | J3DMLF_MaterialUseIndirect
	        | J3DMLF_UseUniqueMaterials | (1 << J3DMLF_TevStageNumShift));

	unk44->setBtk("underwater");
	TScreenTexture* ref = static_cast<TScreenTexture*>(
	    JDrama::TNameRefGen::search("スクリーンテクスチャ"));
	const ResTIMG* img = ref->getTexture()->getTexInfo();
	unk44->getModel()->getModelData()->getTexture()->setResTIMG(1, *img);
}

TMapObjSeaIndirect::TMapObjSeaIndirect(const char* name)
    : JDrama::TActor(name)
    , unk44(nullptr)
{
}

void TMapObjWaterFilter::perform(u32 cue, JDrama::TGraphics* graphics)
{
	// TODO: mother of all intern codes...
	// Frame is 0xF8 rather than 0x128; the wave-height return also folds.

	if (!unk44)
		return;

	if (gpMarDirector->unk124 != 0)
		return;

	if (gpCamera->isDemoCamera())
		return;

	const JGeometry::TVec3<f32>& cameraPos = gpCamera->getUnk124();
	if (cameraPos.y > 0.0f
	    && cameraPos.y >= gpMapObjWave->getHeight(cameraPos.x, cameraPos.y,
	                                              cameraPos.z))
		return;

	if (cue & CUE_CALC_ANIM) {
		Mtx afStack_48;
		Mtx afStack_78;
		Mtx afStack_a8;
		MtxPtr viewMtx = graphics->mViewMtx;

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
		J3DGetTranslateRotateMtx(info, afStack_78);
		PSMTXScale(afStack_a8, mScaling.x, mScaling.y, mScaling.z);
		MTXInverse(viewMtx, afStack_48);
		MTXConcat(afStack_48, afStack_78, afStack_48);
		MTXConcat(afStack_48, afStack_a8, afStack_48);
		unk44->getModel()->setBaseTRMtx(afStack_48);
	}
	unk44->perform(cue, graphics);
}

void TMapObjWaterFilter::init()
{
	unk44 = SMS_MakeMActorWithAnmData(
	    "/common/map/UnderWaterFilter.bmd", gpMapObjManager->getUnk40(), 3,
	    J3DMLF_MaterialPEFull | J3DMLF_MaterialUseIndirect
	        | J3DMLF_UseUniqueMaterials | (1 << J3DMLF_TevStageNumShift));
}

TMapObjWaterFilter::TMapObjWaterFilter(const char* name)
    : JDrama::TActor(name)
    , unk44(nullptr)
{
}
