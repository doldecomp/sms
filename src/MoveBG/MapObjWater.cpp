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
	TScreenTexture* ref
	    = JDrama::TNameRefGen::search<TScreenTexture>("スクリーンテクスチャ");
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
	if (!unk44)
		return;
	if (gpMarDirector->unk124 != 0)
		return;

	// The camera position is re-read for every term instead of being bound
	// to a reference: a reference costs five fewer inline temporaries and
	// leaves the frame 36 bytes short, and folding the water test into this
	// same `||` is also what keeps the `bne`/`b` pair at 0x284 (a separate
	// `if (...) return;` collapses to one branch straight to the epilogue).
	if (gpCamera->isDemoCamera()
	    || (gpCamera->getUnk124().y > 0.0f
	        && gpCamera->getUnk124().y
	            >= gpMapObjWave->getHeight(gpCamera->getUnk124().x,
	                gpCamera->getUnk124().y, gpCamera->getUnk124().z)))
		return;

	if (cue & CUE_CALC_ANIM) {
		Mtx inverseView;
		Mtx translation;
		Mtx scale;
		J3DTransformInfo info;
		// TODO: only evidence is the frame: the original declared a
		// last local of 12 bytes here that no instruction touches,
		// presumably a scratch vector for a dropped code path.
		JGeometry::TVec3<f32> rotation;
		MtxPtr viewMtx    = graphics->getViewMtx();
		info.mScale.x     = 1.0f;
		info.mScale.y     = 1.0f;
		info.mScale.z     = 1.0f;
		info.mRotation.x  = 0.0f;
		info.mRotation.y  = 0.0f;
		info.mRotation.z  = 0.0f;
		info.mTranslate.x = mPosition.x;
		info.mTranslate.y = mPosition.y;
		info.mTranslate.z = mPosition.z;
		J3DGetTranslateRotateMtx(info, translation);
		MTXScale(scale, mScaling.x, mScaling.y, mScaling.z);
		MTXInverse(viewMtx, inverseView);
		MTXConcat(inverseView, translation, inverseView);
		MTXConcat(inverseView, scale, inverseView);
		unk44->getModel()->setBaseTRMtx(inverseView);
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
