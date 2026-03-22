#pragma once

#include <Dolphin/types.h>

// Forward declare — BSE header provides full definition
class TMario;

// Custom state ID — must satisfy (state & 0x1C0) == 0x1C0
// BSE's registerStateMachine validates this mask.
// Eclipse-style format: 0xF000__C0
constexpr u32 STATE_CAPE_GLIDE = 0xF00031C0;

// Fall state (verified from decomp MarioMove.cpp:1500)
constexpr u32 STATE_FALL = 0x88C;

// Per-player data key for BSE's registerData system
constexpr const char *CAPE_DATA_KEY = "__cape_powerup";

// Controller button masks (verified from decomp MarioGamePad.hpp)
constexpr u32 BTN_R = 0x20;
constexpr u32 BTN_A = 0x100;
constexpr u32 BTN_B = 0x200;
constexpr u32 BTN_L = 0x4000;

// Physics constants — all tunable, start conservative
constexpr f32 CAPE_TIMER_DURATION   = 60.0f;   // seconds
constexpr f32 CAPE_FADE_START       = 50.0f;   // seconds elapsed when fade begins
constexpr f32 CAPE_BASE_GLIDE_SPEED = 20.0f;
constexpr f32 CAPE_MAX_DIVE_SPEED   = 60.0f;
constexpr f32 CAPE_STALL_SPEED      = 8.0f;
constexpr f32 CAPE_DRAG_DECEL       = 0.2f;
constexpr f32 CAPE_DIVE_ACCEL       = 1.5f;
constexpr f32 CAPE_CLIMB_DECEL      = 1.2f;
constexpr f32 CAPE_PITCH_RATE       = 2.0f;    // degrees per frame
constexpr f32 CAPE_TURN_RATE        = 4.0f;    // degrees per frame
constexpr f32 CAPE_PITCH_MIN        = -60.0f;  // max dive angle (degrees)
constexpr f32 CAPE_PITCH_MAX        = 60.0f;   // max climb angle (degrees)

// Decomp field reference (for documentation — code uses BSE accessors):
//   mFaceAngle  @ 0x94  TVec3<s16>              -> BSE: mAngle
//   mVel        @ 0xA4  TVec3<f32>              -> BSE: mSpeed
//   mForwardVel @ 0xB0  f32                     -> BSE: mForwardSpeed
//   unk108      @ 0x108 (TMarioControllerWork*) -> BSE: mController
//   mState      @ 0x118 u32                     -> BSE: mState
//   mWaterGun   @ 0x3E4 TWaterGun*              -> BSE: mFludd

struct CapeData {
    bool hasCape;               // cape ability is active
    f32 timer;                  // counts down from 60.0 (seconds)
    bool isGliding;             // currently in glide state
    f32 glideSpeed;             // scalar speed along flight vector
    f32 glidePitch;             // pitch angle in degrees (-60 to +60)
    f32 glideYaw;               // facing direction in degrees
    u8 storedNozzle;            // saved TWaterGun::mCurrentNozzle
    u8 storedSecondNozzle;      // saved TWaterGun::mSecondNozzle
    s32 storedWater;            // saved TWaterGun::mCurrentWater
    bool persistAcrossLoad;     // true = preserve cape across loading zone
};

CapeData *getCapeData(TMario *player);
void initCapeData(CapeData *data);
