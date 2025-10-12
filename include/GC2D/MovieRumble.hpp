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

private:
	inline void updateRumbleState(Koga::ToolData* bcsvData, s32 entryOffset)
	{
		const char* rumbleTypeString;

		int nextEntryIndex     = entryIndex + entryOffset;
		bool dataAndIndexValid = (bcsvData != nullptr) && (nextEntryIndex >= 0);

		// this if statement feels wrong
		if (dataAndIndexValid
		    && (nextEntryIndex < bcsvData->mData->mNumEntries)) {
			// get this rumble entry's data, and update this instance
			bcsvData->GetValue(nextEntryIndex, "start_frame", startFrame);
			bcsvData->GetValue(nextEntryIndex, "end_frame", endFrame);
			bcsvData->GetValue(nextEntryIndex, "type", rumbleTypeString);
			rumbleTypeIndex = RumbleType::getIndex((char*)rumbleTypeString);
		} else {
			rumbleTypeIndex = -1;
		}

		isRumbleActive = false;
	}

public:
	const TTHPRender* thpRenderer;
	Koga::ToolData* toolData; // this is supposedly the only usage of tooldata
	                          // in the entire game lmao
	int entryIndex;
	int startFrame;
	int endFrame;
	int rumbleTypeIndex;
	bool isRumbleActive;
};

#endif // GC2D_MOVIE_RUMBLE_HPP
