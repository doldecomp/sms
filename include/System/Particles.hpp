#ifndef SYSTEM_PARTICLES_HPP
#define SYSTEM_PARTICLES_HPP

#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <System/EmitterViewObj.hpp>

// Header fabricated

extern bool gParticleFlagLoaded[0x201];
extern JPAResourceManager* gpResourceManager;

// fabricated
inline static void SMS_LoadParticle(const char* path, u16 id)
{
	if (!gParticleFlagLoaded[id]) {
		gpResourceManager->load(path, id);
		gParticleFlagLoaded[id] = true;
	}
}

// Actual, real funcs!!!!!!!!

// TODO: fill in the enums with particle IDs
enum E_SMS_EFFECT_ONETIME_NORMAL /* 0 */ {
	PARTICLE_MS_M_ROCKET_A  = 0x1,
	PARTICLE_MS_M_ROCKET_B  = 0x2,
	PARTICLE_MS_M_ROCKET_B2 = 0x3,
	PARTICLE_MS_M_ROCKET_C  = 0x4,
	PARTICLE_MS_M_ROCKET_D  = 0x5,

	PARTICLE_MS_FUMI_A = 0x7,
	PARTICLE_MS_FUMI_B = 0x8,
	PARTICLE_MS_FUMI_C = 0x9,

	PARTICLE_MS_DMG_A      = 0xA,
	PARTICLE_MS_DMG_B      = 0xB,
	PARTICLE_MS_DMG_C      = 0xC,
	PARTICLE_MS_ASE        = 0xD,
	PARTICLE_MS_ITEMGET1_A = 0xE,
	PARTICLE_MS_ITEMGET1_B = 0xF,

	PARTICLE_MS_JUMP_ED_A = 0x10,
	PARTICLE_MS_JUMP_ED_B = 0x11,

	PARTICLE_MS_HIPDROP_A = 0x12,
	PARTICLE_MS_HIPDROP_B = 0x13,
	PARTICLE_MS_HIPDROP_C = 0x14,

	PARTICLE_MS_MARIWALK1_A = 0x15,
	PARTICLE_MS_MARIWALK1_B = 0x16,
	PARTICLE_MS_MARIWALK1_C = 0x17,

	PARTICLE_MS_WALLKICK_A = 0x18,
	PARTICLE_MS_WALLKICK_B = 0x19,

	MAP_MAP_GATE_MS_MARIOWP_BODY   = 0x1A,
	MAP_MAP_GATE_MS_MARIOWP_HEAD   = 0x1B,
	MAP_MAP_GATE_MS_MARIOWP_CAP    = 0x1C,
	MAP_MAP_GATE_MS_MARIOWP_RHAND  = 0x1D,
	MAP_MAP_GATE_MS_MARIOWP_LHAND  = 0x1E,
	MAP_MAP_GATE_MS_MARIOWP_RLEG   = 0x1F,
	MAP_MAP_GATE_MS_MARIOWP_RFOOT  = 0x20,
	MAP_MAP_GATE_MS_MARIOWP_LLEG   = 0x21,
	MAP_MAP_GATE_MS_MARIOWP_LFOOT  = 0x22,
	MAP_MAP_GATE_MS_MARIOWP_WATGUN = 0x23,

	PARTICLE_MS_MARIOAP_BODY   = 0x24,
	PARTICLE_MS_MARIOAP_HEAD   = 0x25,
	PARTICLE_MS_MARIOAP_CAP    = 0x26,
	PARTICLE_MS_MARIOAP_RHAND  = 0x27,
	PARTICLE_MS_MARIOAP_LHAND  = 0x28,
	PARTICLE_MS_MARIOAP_RLEG   = 0x29,
	PARTICLE_MS_MARIOAP_RFOOT  = 0x2A,
	PARTICLE_MS_MARIOAP_LLEG   = 0x2B,
	PARTICLE_MS_MARIOAP_LFOOT  = 0x2C,
	PARTICLE_MS_MARIOAP_WATGUN = 0x2D,

	PARTICLE_MS_M_TOBIKOMI_A   = 0x2F,
	PARTICLE_MS_M_TOBIKOMI_B   = 0x30,
	PARTICLE_MS_M_TOBIKOMI_S_A = 0x31,
	PARTICLE_MS_M_TOBIKOMI_S_B = 0x32,
	PARTICLE_MS_M_AWAHAMON     = 0x33,

	PARTICLE_MS_M_WATRUN_A       = 0x34,
	PARTICLE_MS_COINGET_A        = 0x37,
	PARTICLE_MS_COINGET_B        = 0x38,
	PARTICLE_MS_M_AMIATTACK      = 0x39,
	PARTICLE_MS_M_WALKSAND_B     = 0x3B,
	MAP_MAP_GATE_MS_MARIOWP_DUST = 0x3C,
	PARTICLE_MS_M_GOUT_GATE      = 0x40,
	PARTICLE_MS_M_GOUT_DOKAN     = 0x41,

	PARTICLE_MS_M_SPHIPD_HIT_A = 0x43,
	PARTICLE_MS_M_SPHIPD_HIT_B = 0x44,
	PARTICLE_MS_M_SPHIPD_HIT_C = 0x45,
	PARTICLE_MS_M_SPHIPD_HIT_D = 0x46,

	MAP_POLLUTION_MS_M_ASHIOS     = 0x50,
	MAP_MAP_GATE_MS_MARIOWP_SENKO = 0x51,
	PARTICLE_MS_POI_SAND          = 0x53,
	PARTICLE_MS_MNT_KIRA          = 0x70,
	PARTICLE_MS_MARE_KIRA         = 0x71,
	PARTICLE_MS_MNT_YOGORE_B      = 0x72,
	PARTICLE_MS_MNT_YOGORE_F      = 0x73,
	PARTICLE_MS_MARE_YOGORE       = 0x74,
	PARTICLE_MS_KINO_YOGORE       = 0x75,
	PARTICLE_MS_SMB_AP_ROCK       = 0xB6,
	PARTICLE_MS_SMB_AP_SMOKE      = 0xB7,
	PARTICLE_MS_POI_DEAD          = 0xC0,
	PARTICLE_MS_TAMA_FLOWER       = 0xCC,
	PARTICLE_MS_ENM_WATHIT        = 0xE7,
	PARTICLE_MS_MOE_FIRE_OFF      = 0x8B,
	PARTICLE_MS_CHO_ASE           = 0xC9,
};

enum E_SMS_EFFECT_LOOP_NORMAL /* 1 */ {
	PARTICLE_MS_HIKAGE1_A         = 0x100,
	PARTICLE_MS_MARIWALLSL1       = 0x102,
	PARTICLE_MS_M_SLIPSMOKE       = 0x103,
	PARTICLE_MS_M_BLUR3           = 0x104,
	PARTICLE_MS_M_BLUR2           = 0x105,
	PARTICLE_MS_M_BLUR2SP         = 0x106,
	PARTICLE_MS_M_HAMON_D         = 0x109,
	PARTICLE_MS_M_AWA             = 0x10C,
	PARTICLE_MS_M_BLUR1           = 0x10E,
	PARTICLE_MS_M_SLIDESAND_A     = 0x10F,
	PARTICLE_MS_M_SLIDESAND_B     = 0x110,
	PARTICLE_MS_M_AWA_S           = 0x111,
	PARTICLE_MS_M_WATSLIDE_B      = 0x112,
	PARTICLE_MS_M_WATSLIDE_C      = 0x113,
	PARTICLE_MS_M_SPINSHOT_A      = 0x114,
	PARTICLE_MS_M_SPINSHOT_B      = 0x115,
	PARTICLE_MS_M_BIRI_A          = 0x116,
	PARTICLE_MS_M_BIRI_B          = 0x117,
	PARTICLE_MS_M_BIRI_C          = 0x118,
	PARTICLE_MS_M_SPHIPD_A        = 0x11A,
	PARTICLE_MS_M_SPHIPD_B        = 0x11B,
	PARTICLE_MS_M_SPHIPD_C        = 0x11C,
	PARTICLE_MS_M_SPHIPD_D        = 0x11D,
	PARTICLE_MS_M_SEASMOKE        = 0x11E,
	PARTICLE_MS_TORO_WIND         = 0x11F,
	PARTICLE_MS_TORO_HIBANA       = 0x120,
	PARTICLE_MS_GESOSURF_B        = 0x121,
	PARTICLE_MS_GESOSURF_C        = 0x122,
	PARTICLE_MS_GESOSURF_D        = 0x123,
	PARTICLE_MS_POI_ZZZ           = 0x124,
	MAP_POLLUTION_MS_M_SPINOS     = 0x126,
	MAP_POLLUTION_MS_M_TOKEOS     = 0x12B,
	PARTICLE_MS_POI_KIZETSU       = 0x12F,
	BWANWAN_JPA_MS_BWAN_KIRA      = 0x168,
	PARTICLE_MS_NPC_HAMON_A       = 0x171,
	PARTICLE_MS_MNT_WASH          = 0x172,
	PARTICLE_MS_MARE_WASH         = 0x173,
	PARTICLE_MS_KIL_SMOKE         = 0x174,
	PARTICLE_MS_TAMA_HIT          = 0x185,
	PARTICLE_MS_TAMA_BLUR         = 0x186,
	PARTICLE_MS_MOE_FIRE_A        = 0x135,
	PARTICLE_MS_MOE_FIRE_B        = 0x136,
	PARTICLE_MS_MOE_FIRE_D        = 0x137,
	PARTICLE_MS_NPC_KOKUEN        = 0x170,
	PARTICLE_MS_YNB_ONPU          = 0x18B,
	AMENBO_JPA_MS_AME_HAMON       = 0x18E,
	FIREWANWAN_JPA_MS_CAN_HITYUGE = 0x18f,
};

// TODO: what is "type 2"?

enum E_SMS_EFFECT_LOOP_INDIRECT /* 3 */ {
	FIREWANWAN_JPA_MS_CAN_YUGAMI = 0x1F4,
	PARTICLE_MS_NPC_HAMON_B      = 0x1F7,
	PARTICLE_MS_GESOSURF_A       = 0x1E7,
	PARTICLE_MS_M_WATSLIDE_A     = 0x1EA,
	PARTICLE_MS_MOE_FIRE_C       = 0x1ED,
};

// NOTE: doing this makes inlining work out, specializations DON'T.
template <class T> struct FabricatedParticleTypeToFlag;
template <> struct FabricatedParticleTypeToFlag<E_SMS_EFFECT_ONETIME_NORMAL> {
	enum { value = 0 };
};
template <> struct FabricatedParticleTypeToFlag<E_SMS_EFFECT_LOOP_NORMAL> {
	enum { value = 1 };
};
template <> struct FabricatedParticleTypeToFlag<E_SMS_EFFECT_LOOP_INDIRECT> {
	enum { value = 3 };
};

// TODO: the fabricated setScale inline is most likely incorrect.

template <class T>
JPABaseEmitter*
SMS_EasyEmitParticle(T param_1, const JGeometry::TVec3<f32>* param_2,
                     const void* param_3, const JGeometry::TVec3<f32>& param_4)
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
	    param_1, param_2, FabricatedParticleTypeToFlag<T>::value, param_3);
	if (emitter)
		emitter->setScale(param_4);
	return emitter;
}

template <class T>
JPABaseEmitter* SMS_EasyEmitParticle(T param_1, MtxPtr param_2,
                                     const void* param_3,
                                     const JGeometry::TVec3<f32>& param_4)
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    param_1, param_2, FabricatedParticleTypeToFlag<T>::value, param_3);
	if (emitter)
		emitter->setScale(param_4);
	return emitter;
}

#endif
