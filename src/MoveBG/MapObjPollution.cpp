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

// TODO: 88.6%, and the only function keeping MapObjPollution.cpp from being
// source-linked is its only caller, TMapObjRevivalPollution::loadAfter(), which
// inlines this body. The out-of-line copy stays at the map's 0x68 = 26
// instructions in every variant tried, so that size is no discriminator.
//
// 1. Argument evaluation order, SOLVED and retained (batch 151). The ROM
//    evaluates the argument list right to left, so the accessor pair
//    getStampInterval()/getRevivalStampTex() loads the texture (arg 7) before
//    the interval (arg 6), and `int height` declared *before* `int width`
//    swaps the dimension pair while keeping the layer pointer in r8. The whole
//    loop body is now instruction-exact (22 markers, all r1 displacements or
//    register numbers). A plain `mStampInterval, mRevivalStampTex` pair always
//    loads left to right, and layer->getTexWidth()/getTexHeight() or
//    getPos().getWidth()/getHeight() as expressions move the layer into r7, so
//    they are not what the ROM did.
// 2. Frame size and the callee-saved group swap, UNSOLVED. loadAfter's frame is
//    0x38 against the ROM's 0x60 (+0x28), and the two blocks trade places
//    wholesale: retail is [i r31, this r30, offset r29, element r28] and we are
//    [offset r31, element r30, i r29, this r28]. Batch 145 named this class --
//    the pool/base block versus the local block, as in `checkNextFrameSe` and
//    `partsPerform` -- and its only known mover is the number of **named scalar
//    locals** the frame holds (batch 144's `TSunMgr::load`). loadAfter's body
//    holds exactly one (`i`), and there is no load-bearing second one to add or
//    group, so the knob cannot be exercised here. Rejected, all inert on the
//    ranking: every batch-110 binding level and position (pure frame levers,
//    +8/+8/+0x10), a two-parameter `void` wrapper around the loop body, a named
//    `TRevivalPolluter&` element in the loop body (rotates by one: [offset r31,
//    i r30, element r29, this r28], still wrong), `i` hoisted above the
//    base-class call, postfix `++`, a `while` loop, a `u32` index,
//    `(unk14 + i)->`, `getPolluterNum()`/`getPolluter(i)`, and hoisting the
//    array base or the count (both spill a fifth register). The frame alone is
//    buyable with accessor depth (getPos().getWidth()/getHeight() is +0x28 =
//    exactly the ROM's 0x60) but costs the ROM's argument order, so it is not
//    worth trading residue 1 for it.
// 2b. Batch cc22: the frame is now retail's 0x60 with the argument order kept:
//    height/width through `layer->getPos().getHeight()/getWidth()` as named
//    locals (+0x30) and the counter layer as the raw `unk70` member instead of
//    getCounterLayer() (-8). Only the register-group trade is left; named
//    scalars added to the inlined callee (index, interval, texture), a named
//    manager or element, reversed compare, while loops: all inert on it.
// 3. Closure batch 211 checked research 210's rule: the counter/offset pair
//    cannot be made one inlined call's `this` and parameter, because the only
//    inlined call in the loop is `registerPolluteTex()`, whose receiver is the
//    element pointer and whose map signature takes no argument, and
//    `getPolluter(i)` was already rejected. With 40 bytes of dead low region
//    and no legal carrier (batch 208), this unit stays open.
void TRevivalPolluter::registerPolluteTex()
{
	// TODO: inlines make me cry
	TPollutionLayer* layer = gpPollution->getLayer(mLayerIndex);
	int height = layer->getPos().getHeight();
	int width = layer->getPos().getWidth();
	unk8 = gpPollution->unk70.registerRevivalTexStamp(
	    mLayerIndex, 0, 0, width, height, getStampInterval(),
	    getRevivalStampTex());
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
