#ifndef SYSTEM_PARTICLES_HPP
#define SYSTEM_PARTICLES_HPP

#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <System/EmitterViewObj.hpp>

// Header fabricated

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

// Actual, real funcs!!!!!!!!

// TODO: fill in the enums with particle IDs
enum E_SMS_EFFECT_ONETIME_NORMAL /* 0 */ {
	PARTICLE_MS_DMG_B = 0xB,

	PARTICLE_MS_JUMP_ED_A = 0x10,
	PARTICLE_MS_JUMP_ED_B = 0x11,

	PARTICLE_MS_HIPDROP_B    = 0x13,
	PARTICLE_MS_HIPDROP_C    = 0x14,
	PARTICLE_MS_M_AMIATTACK  = 0x39,
	PARTICLE_MS_POI_SAND     = 0x53,
	PARTICLE_MS_SMB_AP_ROCK  = 0xB6,
	PARTICLE_MS_SMB_AP_SMOKE = 0xB7,
	PARTICLE_MS_POI_DEAD     = 0xC0,
	PARTICLE_MS_TAMA_FLOWER  = 0xCC,
	PARTICLE_MS_ENM_WATHIT   = 0xE7,
};
enum E_SMS_EFFECT_LOOP_NORMAL /* 1 */ {
	PARTICLE_MS_M_BLUR2     = 0x105,
	PARTICLE_MS_POI_ZZZ     = 0x124,
	PARTICLE_MS_POI_KIZETSU = 0x12F,
	PARTICLE_MS_TAMA_HIT    = 0x185,
	PARTICLE_MS_TAMA_BLUR   = 0x186,
};
enum E_SMS_EFFECT_LOOP_INDIRECT /* 3 */ { };

// TODO: the fabricated setScale inline is most likely incorrect.

template <class T>
JPABaseEmitter* SMS_EasyEmitParticle(T, const JGeometry::TVec3<f32>*,
                                     const void*, const JGeometry::TVec3<f32>&);

template <>
JPABaseEmitter* SMS_EasyEmitParticle(E_SMS_EFFECT_ONETIME_NORMAL param_1,
                                     const JGeometry::TVec3<f32>* param_2,
                                     const void* param_3,
                                     const JGeometry::TVec3<f32>& param_4)
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
	    param_1, param_2, 0, param_3);
	if (emitter)
		emitter->setScale(param_4);
	return emitter;
}

template <>
JPABaseEmitter* SMS_EasyEmitParticle(E_SMS_EFFECT_LOOP_NORMAL param_1,
                                     const JGeometry::TVec3<f32>* param_2,
                                     const void* param_3,
                                     const JGeometry::TVec3<f32>& param_4)
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
	    param_1, param_2, 1, param_3);
	if (emitter)
		emitter->setScale(param_4);
	return emitter;
}

template <>
JPABaseEmitter* SMS_EasyEmitParticle(E_SMS_EFFECT_LOOP_INDIRECT param_1,
                                     const JGeometry::TVec3<f32>* param_2,
                                     const void* param_3,
                                     const JGeometry::TVec3<f32>& param_4)
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
	    param_1, param_2, 3, param_3);
	if (emitter)
		emitter->setScale(param_4);
	return emitter;
}

template <class T>
JPABaseEmitter* SMS_EasyEmitParticle(T, MtxPtr, const void*,
                                     const JGeometry::TVec3<f32>&);

template <>
JPABaseEmitter* SMS_EasyEmitParticle(E_SMS_EFFECT_ONETIME_NORMAL param_1,
                                     MtxPtr param_2, const void* param_3,
                                     const JGeometry::TVec3<f32>& param_4)
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    param_1, param_2, 0, param_3);
	if (emitter)
		emitter->setScale(param_4);
	return emitter;
}

template <>
JPABaseEmitter* SMS_EasyEmitParticle(E_SMS_EFFECT_LOOP_NORMAL param_1,
                                     MtxPtr param_2, const void* param_3,
                                     const JGeometry::TVec3<f32>& param_4)
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    param_1, param_2, 1, param_3);
	if (emitter)
		emitter->setScale(param_4);
	return emitter;
}

template <>
JPABaseEmitter* SMS_EasyEmitParticle(E_SMS_EFFECT_LOOP_INDIRECT param_1,
                                     MtxPtr param_2, const void* param_3,
                                     const JGeometry::TVec3<f32>& param_4)
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    param_1, param_2, 3, param_3);
	if (emitter)
		emitter->setScale(param_4);
	return emitter;
}

#endif
