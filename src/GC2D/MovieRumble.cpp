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

// Bare-return fork over unk14.  At the dataExists site it is +4 of pool
// and puts `type` at retail's 0x28(r1).  Combined with assigning `unk14`
// directly (no `data` binding) the 128-byte name buffer stays at 0x30 and
// the frame stays 0xc0.  The same fork through getToolData() is +16 and
// adds a reload; Attach must keep reading unk14 raw so retail's reload
// stays.  Not in the map, so the level stays TU-local.
static inline Koga::ToolData* MovieRumbleUnk14(TMovieRumble* self)
{
	return self->unk14;
}

void TMovieRumble::init(const char* param_1)
{
	char acStack_90[128];
	makeBcrName(acStack_90, 128, param_1);

	unk14     = new Koga::ToolData;
	void* bcr = JKRGetResource(acStack_90);
	unk14->Attach(bcr);
	if (!MovieRumbleUnk14(this)->dataExists())
		unk18 = -1;
	else
		unk18 = 0;

	readCurInfo();
	unk28 = false;
}

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

// The binding over TTHPRender::getFrameNumber is what closes perform (frame
// 0x28 -> 0x30) and checkRumbleOff (0x40 -> 0x48): both were exactly 8 low-
// region bytes short and every instruction already matched. Retail therefore
// read the frame number through one extra level here; the natural candidate
// is a getFrameNumber() in THPRender.hpp that binds its member before
// returning it, but that header is shared with linked units, so the level is
// parked TU-locally.
// Header round 23 tried the promotion and it loses: a binding written into
// `TTHPRender::getFrameNumber()` itself is only +4 (batch 143's own-accessor
// price), so `checkRumbleOff` falls to 99.97%, and it also breaks
// `TMovieSubTitle::perform`, which expands the same accessor and is exact
// without the binding. The level therefore stays TU-local: its +8 is a property
// of the extra level, not of the accessor.
static inline s32 MovieFrameNumber(const TTHPRender* render)
{
	s32 frame = render->getFrameNumber();
	return frame;
}

void TMovieRumble::checkRumbleOn()
{
	if (unk24 != -1 && unk1C <= MovieFrameNumber(unk10)) {
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
	if (unk24 != -1 && unk20 <= MovieFrameNumber(unk10)) {
		SMSRumbleMgr->stop();
		unk18 += 1;
		readCurInfo();
		unk28 = false;
	}
}
#pragma dont_inline off

// The `addi r30, r3, 0` that used to be missing in both callers is the
// binding inside getToolData(): the pointer is loaded into a volatile
// register for isValid()'s `cmplwi` and for the guard's `isIndexValid`, and
// copied into a callee-saved one for the three GetValue calls.
// Measured and rejected: the local inside the `if`; the guard reading
// `toolData` instead of `unk14` (98.1/97.4); the guard reading
// `getToolData()` a second time (that expansion is +12 of low region, not
// +4); `const Koga::ToolData*` / `Koga::ToolData&`; a `readInfo` helper
// taking the pointer as a parameter.
void TMovieRumble::readCurInfo()
{
	int group                = unk18;
	Koga::ToolData* toolData = getToolData();

	if (isValid() && unk14->isIndexValid(group)) {
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
