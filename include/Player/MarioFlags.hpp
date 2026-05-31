#ifndef MARIO_FLAGS_HPP
#define MARIO_FLAGS_HPP

// TODO: check these flag names!
enum E_MARIO_FLAG {
	// True when mario is occluded by geometry and so
	// isn't visible to the camera
	MARIO_FLAG_OCCLUDED            = 0x1,
	MARIO_FLAG_VISIBLE             = 0x2, // TODO: wrong-ish name
	MARIO_FLAG_UNK4                = 0x4,
	MARIO_FLAG_NPC_TALKING         = 0x8,
	MARIO_FLAG_RECENTLY_LEFT_WATER = 0x10,
	MARIO_FLAG_UNK_20              = 0x20, // in shadow?
	MARIO_FLAG_DIRTY               = 0x40,
	MARIO_FLAG_UNK_80              = 0x80,  // sucking in water?
	MARIO_FLAG_UNK100              = 0x100, // can jump on wire?
	MARIO_FLAG_UNK200              = 0x200,
	MARIO_FLAG_GAME_OVER           = 0x400,
	MARIO_FLAG_GROUND_POUND_SIT_UP = 0x800,
	MARIO_FLAG_HELMET_FLW_CAMERA   = 0x1000,
	MARIO_FLAG_HELMET              = 0x2000,
	MARIO_FLAG_FLUDD_EMITTING      = 0x4000, // TODO: incorrect?
	MARIO_FLAG_HAS_FLUDD           = 0x8000,
	MARIO_FLAG_IN_SHALLOW_WATER    = 0x10000,
	MARIO_FLAG_IN_WATER            = 0x20000,
	MARIO_FLAG_ON_SAND             = 0x40000,
	MARIO_FLAG_HAS_SHIRT           = 0x100000,
	MARIO_FLAG_IS_PERFORMING       = 0x200000,

	MARIO_FLAG_IN_ANY_WATER
	= MARIO_FLAG_IN_SHALLOW_WATER | MARIO_FLAG_IN_WATER, // 0x30000
};

#endif
