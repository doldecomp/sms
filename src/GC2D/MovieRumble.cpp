#include <GC2D/MovieRumble.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <stdio.h>
#include <string.h>

TMovieRumble::TMovieRumble(const TTHPRender* thpRender)
{
	thpRenderer = thpRender;
}

void TMovieRumble::init(const char* subtitleName)
{
	char szSubtitlePath[128];
	sprintf(szSubtitlePath, "/subtitle/rnbl/%s", subtitleName);
	char* szSubtitlePathExtension = (char*)strrchr(szSubtitlePath, '.');
	char* result                  = strcpy(szSubtitlePathExtension, ".bcr");

	Koga::ToolData* data = new Koga::ToolData;
	toolData             = data;
	void* glbResource    = JKRFileLoader::getGlbResource(szSubtitlePath);
	toolData->Attach(glbResource);

	if (toolData->dataExists() == FALSE)
		entryIndex = -1;
	else
		entryIndex = 0;

	Koga::ToolData* pData = toolData;
	s32 nextEntryIndex    = entryIndex;

	updateRumbleState(pData, nextEntryIndex);

	isRumbleActive = false;
}

void TMovieRumble::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if ((flags & 1) != 0) {
		if (isRumbleActive) {
			checkRumbleOff();
		} else {
			bool rumbleTypeValid = rumbleTypeIndex != -1;
			if (rumbleTypeValid
			    && startFrame <= thpRenderer->getFrameNumber()) {
				SMSRumbleMgr->start(rumbleTypeIndex, -1, (float*)0);
				isRumbleActive = true;
			}
		}
	}
}

#pragma dont_inline on
void TMovieRumble::checkRumbleOff()
{
	bool rumbleTypeValid = rumbleTypeIndex != -1;
	if (rumbleTypeValid) {
		if (endFrame <= thpRenderer->getFrameNumber()) {
			SMSRumbleMgr->stop();
			entryIndex++;

			Koga::ToolData* pData = toolData;
			s32 nextEntryIndex    = entryIndex;
			updateRumbleState(pData, nextEntryIndex);
			isRumbleActive = false;
		}
	}
}
#pragma dont_inline off

TMovieRumble::~TMovieRumble() { }
