#ifndef MAP_POLLUTION_EVENT_HPP
#define MAP_POLLUTION_EVENT_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TPollutionTest : public JDrama::TViewObj {
public:
	TPollutionTest(const char* name = "落書きテスト")
	    : JDrama::TViewObj(name)
	{
	}

	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics) { }

	void registerEvent(JDrama::TViewObj*);
};

class TPollutionEventModelStampMario : public JDrama::TViewObj {
public:
	void perform(u32 cue, JDrama::TGraphics* graphics);
};

class TPollutionEventModelStamp : public JDrama::TViewObj {
public:
	TPollutionEventModelStamp(const char*);

	void perform(u32 cue, JDrama::TGraphics* graphics);
	void init(const char*);
};

class TPollutionEventMaze : public JDrama::TViewObj {
public:
	TPollutionEventMaze(const char*);

	void perform(u32 cue, JDrama::TGraphics* graphics);
	void init();
};

class TPollutionEventAreaObj : public JDrama::TViewObj {
public:
	TPollutionEventAreaObj(const char*);

	void perform(u32 cue, JDrama::TGraphics* graphics);
	void init(const char*);
};

#endif
