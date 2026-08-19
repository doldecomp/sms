
#include <Map/MapEvent.hpp>
#include <Map/MapModel.hpp>
#include <Map/Map.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

bool TMapEvent::isFinishedAll() const { return stateIs(0) ? true : false; }

void TMapEvent::finishControl()
{
	if (isFinishedAll())
		unk18 = 0;
	else
		unk18 = 1;
}

void TMapEvent::controlMapEvent()
{
	if (control())
		finishControl();
}

void TMapEvent::watchMapEvent()
{
	if (watch())
		startControl();
}

void TMapEvent::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!stateIs(0) && (cue & CUE_MOVE)) {
		if (stateIs(1))
			watchMapEvent();

		if (stateIs(2))
			controlMapEvent();
	}
}

void TMapEvent::load(JSUMemoryInputStream& stream)
{
	TEventWatcher::load(stream);
	unk1C = gpMap->getModelManager()->getJointModel(0);
}

TMapEvent::TMapEvent(const char* name)
    : TEventWatcher(name)
    , unk18(1)
    , unk1C(nullptr)
{
}
