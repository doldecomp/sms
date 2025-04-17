#ifndef MAP_MAP_EVENT_HPP
#define MAP_MAP_EVENT_HPP

#include <System/EventWatcher.hpp>
#include <Map/JointModel.hpp>

class TMapEvent : public TEventWatcher {
public:
	TMapEvent(const char*);
	TMapEvent();

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(unsigned long, JDrama::TGraphics*);
	virtual bool isFinishedAll() const;
	virtual bool watch() = 0;
	virtual void startControl() { unk18 = 2; }
	virtual bool control() = 0;
	virtual void finishControl();
	virtual TJointObj* getBuilding(int i) const
	{
		return unk1C->mChildren[0]->mChildren[i];
	}

	void watchMapEvent();
	void controlMapEvent();

	bool stateIs(u32 state) const { return unk18 == state ? true : false; }

public:
	/* 0x18 */ u32 unk18;
	/* 0x1C */ TJointModel* unk1C;
};

#endif
