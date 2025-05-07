#include <NPC/NpcBalloon.hpp>
#include <System/MarDirector.hpp>
#include <GC2D/GCConsole2.hpp>

void TNpcBalloon::setNextMessage(u32 a, s32 timer)
{
	_000                = a;
	mBalloonAppearTimer = timer;
}

bool TNpcBalloon::updateBalloon()
{
	int pad[5];
	bool ret = false;

	if (mBalloonAppearTimer > 0
	    && (--mBalloonAppearTimer, !mBalloonAppearTimer)) {
		gpMarDirector->console->startAppearBalloon(_000, true);
		mBalloonAppearTimer = -1;
		ret                 = true;
	}

	return ret;
}
