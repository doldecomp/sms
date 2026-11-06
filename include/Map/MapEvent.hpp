#ifndef MAP_MAP_EVENT_HPP
#define MAP_MAP_EVENT_HPP

#include <System/EventWatcher.hpp>
#include <Map/JointModel.hpp>

class TMapEvent : public TEventWatcher {
public:
	TMapEvent(const char*);
	TMapEvent();

	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual bool isFinishedAll() const;
	virtual bool watch() = 0;
	virtual void startControl() { mState = STATE_CONTROLLING; }
	virtual bool control() = 0;
	virtual void finishControl();
	virtual TJointObj* getBuilding(int i) const
	{
		return mRootMapJointModel->getChild(0)->getChild(i);
	}

	void watchMapEvent();
	void controlMapEvent();

	bool stateIs(u32 state) const { return mState == state ? true : false; }

protected:
	enum {
		STATE_FINISHED,
		STATE_WATCHING,
		STATE_CONTROLLING,
	};

	/* 0x18 */ u32 mState;
	/* 0x1C */ TJointModel* mRootMapJointModel;
};

#endif
