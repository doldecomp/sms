#ifndef GC2D_MOVIE_RUMBLE_HPP
#define GC2D_MOVIE_RUMBLE_HPP

#include <dolphin/types.h>
#include <System/THPRender.hpp>
#include <MarioUtil/RumbleType.hpp>
#include <MarioUtil/ToolData.hpp>

class TMovieRumble : public JDrama::TViewObj {
public:
	TMovieRumble(const TTHPRender* thpRender);
	virtual ~TMovieRumble();
	void init(const char* subtitleName);
	virtual void perform(u32 flags, JDrama::TGraphics* graphics);
	void checkRumbleOff();

public:
	const TTHPRender* thpRenderer;
	Koga::ToolData* toolData; // this is supposedly the only usage of tooldata
	                          // in the entire game lmao
	s32 entryIndex;
	s32 startFrame;
	s32 endFrame;
	s32 rumbleTypeIndex;
	bool isRumbleActive;

private:
	inline void updateRumbleState(Koga::ToolData* toolData, s32 entryIndex)
	{
		const char* rumbleTypeString;
		s32 theEntryIndex = entryIndex;
		bool isDataValid  = (toolData != nullptr) && (theEntryIndex >= 0);

		if (isDataValid && toolData->isIndexValid(theEntryIndex)) {
			// get this rumble entry's data, and update this instance
			toolData->GetValue(theEntryIndex, "start_frame", startFrame);
			toolData->GetValue(theEntryIndex, "end_frame", endFrame);
			toolData->GetValue(theEntryIndex, "type", rumbleTypeString);
			rumbleTypeIndex = RumbleType::getIndex((char*)rumbleTypeString);
		} else {
			rumbleTypeIndex = -1;
		}
	}
};

#endif // GC2D_MOVIE_RUMBLE_HPP
