#include <NPC/NpcBalloon.hpp>
#include <System/MarDirector.hpp>
#include <GC2D/GCConsole2.hpp>

void TNpcBalloon::setNextMessage(u32 a, s32 timer)
{
	unk0                = a;
	mBalloonAppearTimer = timer;
}

bool TNpcBalloon::updateBalloon()
{
	bool result = false;

	if (mBalloonAppearTimer > 0) {
		mBalloonAppearTimer -= 1;

		if (mBalloonAppearTimer == 0) {
			gpMarDirector->getConsole()->startAppearBalloon(unk0, true);
			mBalloonAppearTimer = -1;

			result = true;
		}
	}

	return result;
}
