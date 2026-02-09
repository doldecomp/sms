#ifndef GC2D_TALK_2D_2_HPP
#define GC2D_TALK_2D_2_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class JUTPoint;
class J2DTextBox;
class JMSMesgEntry;
class TBaseNPC;

class TTalk2D2 : public JDrama::TViewObj {
public:
	TTalk2D2(const char* name = "<TTalk2D2>");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);

	void setMessageID(u32, u32);
	void forceCloseTalk();
	void closeTalkWindow();
	void openTalkWindow(TBaseNPC*);
	void makeBoxLine(s8, char*);
	void openBoardWindow();
	void openNormalWindow();
	void moveBoardWindow();
	void checkBoardControler();
	void moveTalkWindow();
	void checkControler();
	void closeNormalWindow();
	void closeBoardWindow();
	void eraseNormalWindow();
	void eraseBoardWindow();
	void appearBoardBoxWindow();
	void makeLine(f32*, f32*, f32, JUTPoint&, JUTPoint&, JUTPoint&);
	void setupBoardTextBox(const void*, JMSMesgEntry*);
	void setupTextBox(const void*, JMSMesgEntry*);
	void setTagParam(JSUMemoryInputStream&, J2DTextBox&, int*, int*);
	void openWindow(s8, f32);

	static void* cColorTable;

public:
	/* 0x10 */ char unk10[0x248 - 0x10];
	/* 0x248 */ u32 unk248;
	/* 0x24C */ char unk24C[0x344 - 0x24C];
};

#endif
