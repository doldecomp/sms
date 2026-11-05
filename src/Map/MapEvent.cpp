
#include <Map/MapEvent.hpp>
#include <Map/MapModel.hpp>
#include <Map/Map.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/MActorAnm.hpp>

bool TMapEvent::isFinishedAll() const
{
	return stateIs(STATE_FINISHED) ? true : false;
}

void TMapEvent::finishControl()
{
	if (isFinishedAll())
		mState = STATE_FINISHED;
	else
		mState = STATE_WATCHING;
}

void TMapEvent::controlMapEvent() { }

void TMapEvent::watchMapEvent() { }

void TMapEvent::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!stateIs(STATE_FINISHED) && (cue & CUE_MOVE)) {
		if (stateIs(STATE_WATCHING) && watch())
			startControl();

		if (stateIs(STATE_CONTROLLING) && control())
			finishControl();
	}
}

void TMapEvent::load(JSUMemoryInputStream& stream)
{
	TEventWatcher::load(stream);
	mRootMapJointModel = gpMap->getRootJointModel();
}

TMapEvent::TMapEvent(const char* name)
    : TEventWatcher(name)
    , mState(STATE_WATCHING)
    , mRootMapJointModel(nullptr)
{
}
