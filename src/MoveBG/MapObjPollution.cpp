#include <MoveBG/MapObjPollution.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Map/PollutionManager.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <stdio.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TPolluterBase::perform(u32 cue, JDrama::TGraphics*)
{
	if (cue & CUE_CALC_ANIM)
		unk138->calcAnm();

	if (cue & CUE_ENTRY)
		gpPollution->stampModel(unk138->getModel());
}

void TPolluterBase::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	char buffer[64];
	snprintf(buffer, 64, "/scene/mapObj/%s.bmd", unkF4);
	unk138 = SMS_MakeMActorWithAnmData(
	    buffer, gpMapObjManager->getMActorAnmData(), 3,
	    J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	        | (1 << J3DMLF_TevStageNumShift));
	makeLowerStr(unkF4, buffer);
	unk138->setBck(buffer);
	unk138->setBpk(buffer);
	unk138->setBtp(buffer);
	unk138->setBtk(buffer);

	MsMtxSetXYZRPH(unk138->getModel()->getBaseTRMtx(), mPosition.x, mPosition.y,
	               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
	unk138->getModel()->setBaseScale(mScaling);
}

TPolluterBase::TPolluterBase(const char* name)
    : TMapObjBase(name)
    , unk138(nullptr)
{
}

void TRevivalPolluter::pollute() { }

void TRevivalPolluter::registerPolluteTex()
{
	// TODO: inlines make me cry
	TPollutionLayer* layer = gpPollution->getLayer(mLayerIndex);
	unk8 = gpPollution->getCounterLayer().registerRevivalTexStamp(
	    mLayerIndex, 0, 0, layer->mPos.mWidth, layer->mPos.mHeight,
	    mStampInterval, mRevivalStampTex);
}

void TRevivalPolluter::loadInfo(JSUMemoryInputStream& stream)
{
	u32 value;
	stream >> value;
	mLayerIndex = value;
	stream >> mStampInterval;

	char buffer[64];
	snprintf(buffer, 64, "/scene/map/pollution/pollute%02d.bti", mLayerIndex);
	mRevivalStampTex = (ResTIMG*)JKRGetResource(buffer);
}

TRevivalPolluter::TRevivalPolluter()
    : mLayerIndex(0)
    , mRevivalStampTex(nullptr)
    , unk8(0)
    , unkC(0.0f)
    , unk10(0.0f)
    , unk14(0.0f)
    , mStampInterval(0)
{
}

void TMapObjRevivalPollution::perform(u32 cue, JDrama::TGraphics*)
{
	if (!(cue & CUE_CALC_ANIM))
		return;

	for (int i = 0; i < unk10; ++i)
		unk14[i].pollute();
}

void TMapObjRevivalPollution::loadAfter()
{
	JDrama::TViewObj::loadAfter();

	for (int i = 0; i < unk10; ++i)
		unk14[i].registerPolluteTex();
}

void TMapObjRevivalPollution::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	stream >> unk10;
	unk14 = new TRevivalPolluter[unk10];
	for (int i = 0; i < unk10; ++i)
		unk14[i].loadInfo(stream);
}

TMapObjRevivalPollution::TMapObjRevivalPollution(const char* name)
    : JDrama::TViewObj(name)
    , unk10(0)
    , unk14(0)
{
}
