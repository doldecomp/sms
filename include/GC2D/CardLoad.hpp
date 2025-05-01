#ifndef GC2D_CARD_LOAD_HPP
#define GC2D_CARD_LOAD_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/J2D/J2DPicture.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>

// TODO: probably shouldn't be here
enum TEProgress {

};

class TCardLoad;

extern TCardLoad* gpCardLoad;

class TCardLoad : public JDrama::TViewObj {
public:
	TCardLoad(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

	void changePattern(J2DPicture*, s16, u32);
	void setupTitleScreen();
	void setupScoreScreen();
	void loadAfter();
	void titleDraw();
	void makeBuffer(J2DTextBox*, int);
	void changeMode(s32);
	void setMessage(J2DTextBox*, s32, int);
	void waitForChoice(TEProgress, TEProgress, int);
	void waitForChoiceBM(TEProgress, TEProgress, int);
	void waitForAnyKey(TEProgress);
	void waitForAnyKeyBM(TEProgress);
	void waitForStart(TEProgress);
	void drawMessage(TEProgress);
	void drawMessageBM(TEProgress);
	void selectBookmark(TEProgress, TEProgress, bool);
	void selectFunction();
	void setSelected(u8);
	void changeScene();

	u32 cMessageID;
};

#endif
