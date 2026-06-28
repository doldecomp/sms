#ifndef MAP_POLLUTION_EVENT_HPP
#define MAP_POLLUTION_EVENT_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TPollutionTest : public JDrama::TViewObj {
public:
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*) { }

	void registerEvent(JDrama::TViewObj*);
};

class TPollutionEventModelStampMario {
public:
	void perform(u32, JDrama::TGraphics*);
};

class TPollutionEventModelStamp {
public:
	TPollutionEventModelStamp(const char*);

	void perform(u32, JDrama::TGraphics*);
	void init(const char*);
};

class TPollutionEventMaze {
public:
	TPollutionEventMaze(const char*);

	void perform(u32, JDrama::TGraphics*);
	void init();
};

class TPollutionEventAreaObj {
public:
	TPollutionEventAreaObj(const char*);

	void perform(u32, JDrama::TGraphics*);
	void init(const char*);
};

#endif
