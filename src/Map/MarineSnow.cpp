#include <Map/MarineSnow.hpp>
#include <Camera/Camera.hpp>
#include <System/EmitterViewObj.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>

TMarineSnow::TMarineSnow(const char* name)
    : JDrama::TActor(name)
{
}

// TODO: move this
extern u8 gParticleFlagLoaded[];
extern JPAResourceManager* gpResourceManager;
inline static void loadParticle(const char* path, u32 id)
{
	if (gParticleFlagLoaded[id] == 0) {
		gpResourceManager->load(path, id);
		gParticleFlagLoaded[id] = 1;
	}
}

void TMarineSnow::loadAfter()
{
	loadParticle("/scene/others/marinesnow/ms_mare_marinsnow.jpa", 0x14B);
}

void TMarineSnow::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 2) {
		mPosition.set(gpCamera->unk124);
		gpMarioParticleManager->emitAndBindToPosPtr(0x14B, &mPosition, 1, this);
	}
}
