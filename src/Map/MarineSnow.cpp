#include <Map/MarineSnow.hpp>
#include <Camera/Camera.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>

TMarineSnow::TMarineSnow(const char* name)
    : JDrama::TActor(name)
{
}

void TMarineSnow::loadAfter()
{
	SMS_LoadParticle("/scene/others/marinesnow/ms_mare_marinsnow.jpa",
	                 OTHERS_MARINESNOW_MS_MARE_MARINSNOW);
}

void TMarineSnow::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		mPosition.set(gpCamera->getUnk124());
		gpMarioParticleManager->emitAndBindToPosPtr(
		    OTHERS_MARINESNOW_MS_MARE_MARINSNOW, &mPosition, 1, this);
	}
}
