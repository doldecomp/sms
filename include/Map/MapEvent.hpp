#ifndef MAP_MAP_EVENT_HPP
#define MAP_MAP_EVENT_HPP

#include <System/EventWatcher.hpp>

class TMapEvent : public TEventWatcher {
public:
	TMapEvent(const char*);
	TMapEvent();

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(unsigned long, JDrama::TGraphics*);
	virtual void isFinishedAll() const;

	void startControl();
	void getBuilding(int) const;
	void watchMapEvent();
	void controlMapEvent();
	void finishControl();
};

#endif
