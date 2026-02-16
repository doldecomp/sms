#include <GC2D/MovieRumble.hpp>
#include <stdio.h>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <System/THPRender.hpp>
#include <MarioUtil/RumbleType.hpp>
#include <MarioUtil/ToolData.hpp>
#include <MarioUtil/RumbleMgr.hpp>

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
	if (!unk14->GetData())
		unk18 = -1;
	else
		unk18 = 0;

	readCurInfo();
	unk28 = false;
}

void TMovieRumble::perform(u32 param_1, JDrama::TGraphics*)
{
	if (param_1 & 0x1)
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

void TMovieRumble::readCurInfo()
{
	int group = unk18;

	if (isValid() && group < unk14->GetGroupCount()) {
		Koga::ToolData* toolData = unk14;
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
