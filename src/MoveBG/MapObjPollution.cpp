#include <MoveBG/MapObjPollution.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Map/PollutionManager.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <stdio.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TPolluterBase::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 2)
		unk138->calcAnm();

	if (param_1 & 0x200)
		gpPollution->stampModel(unk138->getUnk4());
}

void TPolluterBase::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	char buffer[64];
	snprintf(buffer, 64, "/scene/mapObj/%s.bmd", unkF4);
	unk138 = SMS_MakeMActorWithAnmData(
	    buffer, gpMapObjManager->getMActorAnmData(), 3, 0x10210000);
	makeLowerStr(unkF4, buffer);
	unk138->setBck(buffer);
	unk138->setBpk(buffer);
	unk138->setBtp(buffer);
	unk138->setBtk(buffer);

	MsMtxSetXYZRPH(unk138->getUnk4()->getBaseTRMtx(), mPosition.x, mPosition.y,
	               mPosition.z, mRotation.x * (65536.0f / 360.0f),
	               mRotation.y * (65536.0f / 360.0f),
	               mRotation.z * (65536.0f / 360.0f));
	unk138->getUnk4()->unk14 = mScaling;
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
	TPollutionLayer* layer = gpPollution->getLayer(unk0);
	unk8 = gpPollution->getCounterLayer().registerRevivalTexStamp(
	    unk0, 0, 0, layer->unk5C.mWidth, layer->unk5C.mHeight, unk18, unk4);
}

void TRevivalPolluter::loadInfo(JSUMemoryInputStream& stream)
{
	u32 value;
	stream.read(&value, 4);
	unk0 = value;
	stream.read(&unk18, 4);

	char buffer[64];
	snprintf(buffer, 64, "/scene/map/pollution/pollute%02d.bti", unk0);
	unk4 = (ResTIMG*)JKRGetResource(buffer);
}

TRevivalPolluter::TRevivalPolluter()
    : unk0(0)
    , unk4(nullptr)
    , unk8(0)
    , unkC(0.0f)
    , unk10(0.0f)
    , unk14(0.0f)
    , unk18(0)
{
}

void TMapObjRevivalPollution::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!(param_1 & 2))
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
	stream.read(&unk10, 4);
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
