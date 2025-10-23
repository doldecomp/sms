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
	char* szSubtitlePathExtension;
	char szSubtitlePath[128];
	sprintf(szSubtitlePath, "/subtitle/rnbl/%s", subtitleName);
	szSubtitlePathExtension = (char*)strrchr(szSubtitlePath, 46);
	char* result            = strcpy(szSubtitlePathExtension, ".bcr");

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
			if (rumbleTypeIndex != -1
			    && startFrame <= thpRenderer->frameNumber) {
				SMSRumbleMgr->start(rumbleTypeIndex, -1, (float*)0);
				isRumbleActive = true;
			}
		}
	}
}

#pragma dont_inline on
void TMovieRumble::checkRumbleOff()
{
	if (rumbleTypeIndex != -1) {
		if (endFrame <= thpRenderer->frameNumber) {
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
