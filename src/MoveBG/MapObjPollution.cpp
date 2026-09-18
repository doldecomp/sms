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

// TODO: 88.1%, and the only function keeping MapObjPollution.cpp from being
// source-linked is its only caller, TMapObjRevivalPollution::loadAfter(), which
// inlines this body. Two residues are left there, and this batch pinned the
// first one down (all measured with single-file compiles; the out-of-line copy
// stays at the map's 0x68 = 26 instructions in every variant below, so that
// size is no discriminator):
//
// 1. Argument evaluation order, SOLVED but not retained. The ROM evaluates the
//    argument list right to left -- texture (arg 7) before interval (arg 6),
//    height (arg 5) before width (arg 4) -- and keeps the layer pointer in r8.
//    Two changes reproduce that block instruction for instruction:
//      * accessors for the polluter's own members
//        (`getStampInterval()`, `getRevivalStampTex()` on TRevivalPolluter)
//        swap the interval/texture pair into the ROM's order (a plain
//        `mStampInterval, mRevivalStampTex` pair always loads left to right);
//      * `int height = layer->mPos.mHeight;` declared *before*
//        `int width = layer->mPos.mWidth;` swaps the dimension pair while
//        keeping the layer in r8. `layer->getTexWidth()/getTexHeight()` or
//        `layer->getPos().getWidth()/getHeight()` as expressions also give
//        height-first, but move the layer into r7 and swap the two load
//        destinations, so they are *not* what the ROM did.
// 2. Frame size and a four-register rotation, UNSOLVED. loadAfter's frame is
//    0x38 against the ROM's 0x60, and the ROM's callee-saved assignment is a
//    rotation of ours: ROM has this=r30, i=r31, byte-offset=r29, element=r28,
//    while every spelling tried gives the two compiler-generated loop values
//    the *high* pair and this/i the low pair. The frame can be bought with
//    accessor depth (raw 0x38; `getTexWidth/getTexHeight` +0x10; the same two
//    named +0x18; `getPos().getWidth()/getHeight()` +0x28 = the ROM's 0x60;
//    `getLayerIndex()` or a named counter-layer reference +8 each), and one
//    asymmetric mix (`getPos().getHeight()` for height, `getTexWidth()` for
//    width) lands on 0x60 *with* the ROM's exact inner block -- but nothing
//    moves the register rotation. Loop shapes tried, all leaving the rotation:
//    `i` declared at the top of the function, postfix `++`, a `while` loop, a
//    `u32` index, `(unk14 + i)->`, a named `TRevivalPolluter*`/`&` inside the
//    loop (rotates by one instead), `getPolluterNum()` as the bound,
//    `getPolluter(i)` as the element (both no-ops), and hoisting the array base
//    or the count into a local (both spill a fifth register).
//    Closure batch 103 reproduced the 0x60 + exact-inner-block state and
//    pinned its spelling: `getLayer(getLayerIndex())` with a raw `mLayerIndex`
//    as argument 1, `layer->getTexWidth()` for width,
//    `layer->getPos().getHeight()` for height, `getStampInterval()`/
//    `getRevivalStampTex()` for the last pair, and a named
//    `TPollutionCounterLayer&`. That is frame 0x60, 45 instructions and
//    retail's whole argument-evaluation order; the only residue left is the
//    four-register rotation plus the layer pointer landing in r7 where retail
//    uses r8. It reads 88.0% against the current 88.1% because fuzzy_match
//    weights the register operands above the frame, so it is not committed.
//    The rotation has the same signature as
//    AudioDecoderForOnMemory's (retail interleaves a source local above the
//    compiler-generated loop temporaries; every spelling we have puts both
//    temporaries on top) -- treat the two together in a research batch.
//    Best next hypothesis: the rotation says MWCC built loadAfter's two loop
//    temporaries *after* this/i in the ROM and before them for us, so look for
//    a source shape that creates a source-level variable in the loop body
//    without folding it away, and re-measure the frame only after that.
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
