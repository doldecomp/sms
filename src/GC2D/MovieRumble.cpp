#include <GC2D/MovieRumble.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/RumbleType.hpp>
#include <MarioUtil/ToolData.hpp>
#include <System/THPRender.hpp>
#include <stdio.h>

// TODO: removeme
static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

TMovieRumble::TMovieRumble(const TTHPRender* param_1)
    : unk10(param_1)
{
}

void TMovieRumble::init(const char* param_1)
{
	char acStack_90[128];
	makeBcrName(acStack_90, 128, param_1);

	unk14     = new Koga::ToolData;
	void* bcr = JKRGetResource(acStack_90);
	unk14->Attach(bcr);
	if (!unk14->dataExists())
		unk18 = -1;
	else
		unk18 = 0;

	readCurInfo();
	unk28 = false;
}

// TODO: 99.8%, frame 0x28 vs 0x30, no stack slot referenced on either
// side. The 8 bytes belong to the inlined `movement`/`checkRumbleOn` pair,
// and `movement`'s UNUSED size says where: map 0xb4, ours 0x78, i.e.
// fifteen instructions of `movement` are still missing.
void TMovieRumble::perform(u32 cue, JDrama::TGraphics*)
{
	if (cue & CUE_MOVE)
		movement();
}

void TMovieRumble::movement()
{
	if (unk28) {
		checkRumbleOff();
	} else {
		checkRumbleOn();
	}
}

void TMovieRumble::checkRumbleOn()
{
	if (unk24 != -1 && unk1C <= unk10->getFrameNumber()) {
		SMSRumbleMgr->start(unk24, -1, (f32*)nullptr);
		unk28 = true;
	}
}

// TODO: the pragma is a placeholder. Retail emits checkRumbleOff and calls
// it from the inlined `movement`, i.e. at depth 2, where the plain-callee
// budget is 9 statements; this body has 5, so it expands without the
// pragma. The missing statements are most likely `movement`'s (see its
// UNUSED size above), not this function's.
#pragma dont_inline on
void TMovieRumble::checkRumbleOff()
{
	if (unk24 != -1 && unk20 <= unk10->getFrameNumber()) {
		SMSRumbleMgr->stop();
		unk18 += 1;
		readCurInfo();
		unk28 = false;
	}
}
#pragma dont_inline off

// TODO: one construct short, and it is the same one in both inlining
// callers (init 98.5% / frame 0xb8 vs 0xc0, checkRumbleOff 98.1% / 0x40 vs
// 0x48): retail copies the ToolData pointer into a second, callee-saved
// register (`addi r30, r3, 0` between the `cmplwi` and the `beq` of the
// inlined isValid) and then lets the guard's `isIndexValid` consume the
// first copy (`lwz r3, 4(r3)`), so the pointer is live in two registers.
// Declaring the local above the `if` (done below) recovers the rest of the
// register assignment: +0.7 on checkRumbleOff and +0.5 on init. Measured
// with no further effect: `const Koga::ToolData*`, `Koga::ToolData&`,
// `const Koga::ToolData&`, the local inside the `if`, and factoring the
// three GetValue calls into an inlined one- or two-parameter `readInfo`
// member (parameter binding is free here; it does land readCurInfo's own
// UNUSED size on the map's 0xd0, but costs both callers 0.7/1.8).
// `getToolData()` is +8 of low region per call site, saturating at two.
void TMovieRumble::readCurInfo()
{
	int group                = unk18;
	Koga::ToolData* toolData = getToolData();

	if (isValid() && getToolData()->isIndexValid(group)) {
		toolData->GetValue(group, "start_frame", unk1C);
		toolData->GetValue(group, "end_frame", unk20);
		const char* type;
		toolData->GetValue(group, "type", type);
		unk24 = RumbleType::getIndex((char*)type);
	} else {
		unk24 = -1;
	}
}

void TMovieRumble::makeBcrName(char* acStack_90, int, const char* param_1)
{
	sprintf(acStack_90, "/subtitle/rnbl/%s", param_1);
	char* it = strrchr(acStack_90, '.');
	strcpy(it, ".bcr");
}
