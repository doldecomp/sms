#include <Enemy/BossHanachan.hpp>
#include <System/MarDirector.hpp>
#include <Player/ModelWaterManager.hpp>

// TODO: why TF are these defined here and aren't even weak? Mysterious.
void TWaterHitActor::onWaterHitCounter() { mWaterHitCounter = 0x3C; }

BOOL TWaterHitActor::receiveMessage(THitActor* sender, u32 message)
{
	BOOL result = false;

	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (SMSGetMarDirector()->isThing()) {
			mWaterHitCounter = 0;
		} else {
			onWaterHitCounter();
		}
		result = true;
	}

	return result;
}
