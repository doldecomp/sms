#include "dolphin.h"
#include "gpMarDirector.hpp"
#include "types.h"

float SMS_GetSandRiseUpRatio(const TLiveActor* actor)
{
	float retvar = actor->getModel()->unk58->unk14;

	if (retvar > 1.0f) {
		retvar = 1.0f;
	} else if (retvar < 0.0f) {
		retvar = 0.0f;
	}

	return retvar;
}

void* SMS_GetGroundActor(const TBGCheckData* bgData, u32 value)
{
	void* actor = NULL;
	if (bgData) {
		actor = bgData->unk44;
		if (bgData->unk44 && value != bgData->unk44->unk4C) {
			actor = NULL;
		}
	}
	return actor;
}

int SMS_GetMonteVillageAreaInMario()
{
	int retvar = 4;
	if (gpMarDirector->map == 8) {
		if (gpCamera->unk50 == 0x33) {
			return retvar = 1;
		}
		switch (gpCubeFastC->unk1C) {
		case 0:
			retvar = 2;
			break;
		case 1:
			retvar = 0;
			break;
		default:
			retvar = 3;
			break;
		}
	}
	return retvar;
}
