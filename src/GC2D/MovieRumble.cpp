#include <GC2D/MovieRumble.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/RumbleType.hpp>
#include <MarioUtil/ToolData.hpp>
#include <System/THPRender.hpp>
#include <System/DummyStrings.hpp>
#include <stdio.h>

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
	if (unk24 != -1) {
		int frame = unk10->getFrameNumber();
		if (unk1C <= frame) {
			SMSRumbleMgr->start(unk24, -1, (f32*)nullptr);
			unk28 = true;
		}
	}
}

// TODO: fakematch; perform must call checkRumbleOff out of line.
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
#pragma dont_inline off // TODO: scope the checkRumbleOff workaround.

void TMovieRumble::readCurInfo()
{
	Koga::ToolData* toolData = unk14;
	int group                = unk18;

	if (isValid() && toolData->isIndexValid(group)) {
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
