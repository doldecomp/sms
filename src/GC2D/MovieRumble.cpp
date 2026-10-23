#include <GC2D/MovieRumble.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/RumbleType.hpp>
#include <MarioUtil/ToolData.hpp>
#include <System/THPRender.hpp>
#include <stdio.h>

#define MOVIE_RUMBLE_DATA_INVALID (-1)

// TODO: removeme
#include <System/Application.hpp>

TMovieRumble::TMovieRumble(const TTHPRender* thpRenderer)
    : mThpRenderer(thpRenderer)
{
}

void TMovieRumble::init(const char* movieRumbleFile)
{
	char bcrName[128];
	makeBcrName(bcrName, sizeof(bcrName), movieRumbleFile);

	mToolData = new Koga::ToolData;
	void* bcr = JKRGetResource(bcrName);
	mToolData->Attach(bcr);
	if (!mToolData->dataExists())
		mTooldataIndex = MOVIE_RUMBLE_DATA_INVALID;
	else
		mTooldataIndex = 0;

	readCurInfo();
	mIsDoingRumble = false;
}

void TMovieRumble::perform(u32 cue, JDrama::TGraphics* graphics)
{
	bool isPerform = cue & CUE_MOVE;
	if (isPerform)
		movement();
}

void TMovieRumble::movement()
{
	if (mIsDoingRumble) {
		checkRumbleOff();
	} else {
		checkRumbleOn();
	}
}

void TMovieRumble::checkRumbleOn()
{
	if (mRumbleType != -1 && mStartFrame <= mThpRenderer->getFrameNumber()) {
		SMSRumbleMgr->start(mRumbleType, -1, (f32*)nullptr);
		mIsDoingRumble = true;
	}
}

#pragma dont_inline on
void TMovieRumble::checkRumbleOff()
{
	if (mRumbleType != -1 && mEndFrame <= mThpRenderer->getFrameNumber()) {
		SMSRumbleMgr->stop();
		mTooldataIndex += 1;
		readCurInfo();
		mIsDoingRumble = false;
	}
}
#pragma dont_inline off

void TMovieRumble::readCurInfo()
{
	int group = mTooldataIndex;

	if (isValid() && mToolData->isIndexValid(group)) {
		Koga::ToolData* toolData = mToolData;
		toolData->GetValue(group, "start_frame", mStartFrame);
		toolData->GetValue(group, "end_frame", mEndFrame);
		const char* type;
		toolData->GetValue(group, "type", type);
		mRumbleType = RumbleType::getIndex((char*)type);
	} else {
		mRumbleType = -1;
	}
}

void TMovieRumble::makeBcrName(char* out_bcrName, int,
                               const char* movieRumbleFile)
{
	sprintf(out_bcrName, "/subtitle/rnbl/%s", movieRumbleFile);
	char* it = strrchr(out_bcrName, '.');
	strcpy(it, ".bcr");
}
