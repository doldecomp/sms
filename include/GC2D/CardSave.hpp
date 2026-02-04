#ifndef GC2D_CARD_SAVE_HPP
#define GC2D_CARD_SAVE_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <GC2D/Progress.hpp>

class TMarioGamePad;
class J2DTextBox;
class J2DPicture;

class TCardSave : public JDrama::TViewObj {
public:
	void changeMode(long);

	TCardSave(const char* name = "<TCardSave>", bool = false);

	void load(JSUMemoryInputStream& stream);
	void initData(TMarioGamePad*);
	void init(int);
	void perform(unsigned long, JDrama::TGraphics*);
	void makeBuffer(J2DTextBox*, int);
	void setMessage(J2DTextBox*, long, unsigned long);
	void waitForStop(TEProgress);
	void endWaitForChoice();
	void waitForChoice(TEProgress, TEProgress, signed char);
	void waitForChoiceBM(TEProgress, TEProgress, signed char);
	void endDrawMessage();
	void drawMessage(TEProgress);
	void drawMessageBM(TEProgress);
	void waitForAnyKey(TEProgress);
	void waitForSelectOver();
	void waitForSelect2(TEProgress, TEProgress);
	void waitForSelect3(TEProgress, TEProgress, TEProgress);
	void waitForAnyKeyBM(TEProgress);
	void selectBookmarks(TEProgress, TEProgress, TEProgress, TEProgress);
	void changePattern(J2DPicture*, short, unsigned long);
	void execMovement_();
	u8 getNextState();
	void execIssueGX_(JDrama::TGraphics*);

	static int cMessageID; // TODO: wrong type
};

#endif
