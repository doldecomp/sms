#include "dolphin.h"

#include "gpMarDirector.hpp"
#include "NPC/NpcBalloon.hpp"

bool TNpcBalloon::updateBalloon()
{
	int pad[5];
	bool ret = false;

	if (this->mBalloonAppearTimer > 0
	    && (--this->mBalloonAppearTimer, !this->mBalloonAppearTimer)) {
		gpMarDirector->console->startAppearBalloon(this->_000, true);
		this->mBalloonAppearTimer = -1;
		ret                       = true;
	}

	return ret;
}

void TNpcBalloon::setNextMessage(u32 a, s32 timer)
{
	this->_000                = a;
	this->mBalloonAppearTimer = timer;
}
