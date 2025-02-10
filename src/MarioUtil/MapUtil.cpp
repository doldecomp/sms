#include "dolphin.h"
#include "gpMarDirector.hpp"
#include "unsorted_types.h"
#include "unsorted_variables.h"
#include <Map/Map.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <Strategic/HitActor.hpp>

float SMS_GetSandRiseUpRatio(const TLiveActor* actor)
{
	float retvar = actor->getModel()->mNodeMatrices[0][1][1];

	if (retvar > 1.0f) {
		retvar = 1.0f;
	} else if (retvar < 0.0f) {
		retvar = 0.0f;
	}

	return retvar;
}

void* SMS_GetGroundActor(const TBGCheckData* bgData, u32 value)
{
	void* actor = nullptr;
	if (bgData) {
		actor = bgData->unk44;
		// TODO: this does not match and I have no idea what the original
		// author was thinking, the entire THitActor hierarchy needs to be
		// figured out
		if (bgData->unk44 && value != bgData->unk44->actorType) {
			actor = nullptr;
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
