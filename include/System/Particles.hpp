#ifndef SYSTEM_PARTICLES_HPP
#define SYSTEM_PARTICLES_HPP

#include <JSystem/JParticle/JPAResourceManager.hpp>

// Header completely fabricated, all contents fabricated too

// TODO: enum with particle IDs

extern bool gParticleFlagLoaded[0x201];
extern JPAResourceManager* gpResourceManager;

// fabricated
inline static void SMS_LoadParticle(const char* path, u32 id)
{
	if (!gParticleFlagLoaded[id]) {
		gpResourceManager->load(path, id);
		gParticleFlagLoaded[id] = true;
	}
}

#endif
