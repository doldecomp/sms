#pragma once

#include <SMS/Player/Mario.hxx>
#include <types.h>

namespace CapeMod {

// --- State ID ---
// Custom state IDs must satisfy (state & 0x1C0) == 0x1C0.
// Eclipse-style format: 0xF000__C0.
constexpr u32 STATE_CAPE_FLY = 0xF00001C0;

// --- Fall state (from decomp) ---
constexpr u32 STATE_FALL = 0x0000088C;

// --- Controller button masks (verified from decomp) ---
constexpr u32 BTN_R = 0x0020;
constexpr u32 BTN_A = 0x0100;
constexpr u32 BTN_B = 0x0200;
constexpr u32 BTN_L = 0x4000;

// --- Physics tuning constants ---
constexpr f32 CAPE_GRAVITY       = -0.5f;   // Reduced gravity while gliding
constexpr f32 CAPE_FORWARD_SPEED = 16.0f;   // Forward speed during glide
constexpr f32 CAPE_LIFT_ACCEL    = 0.8f;    // Upward acceleration on activation
constexpr f32 CAPE_MAX_Y_SPEED   = 10.0f;   // Terminal upward velocity
constexpr f32 CAPE_MIN_Y_SPEED   = -8.0f;   // Terminal downward velocity
constexpr f32 CAPE_PITCH_RATE    = 2.0f;    // Pitch control sensitivity
constexpr f32 CAPE_YAW_RATE      = 4.0f;    // Yaw/turning sensitivity

// --- Activation requirements ---
constexpr f32 MIN_ACTIVATION_HEIGHT = 100.0f;  // Min height above ground to activate
constexpr s32 CAPE_COOLDOWN_FRAMES  = 30;      // Frames before cape can reactivate

// --- Player field offsets (decomp-verified) ---
// These document the raw offsets for reference. Code should use BSE accessors.
//   mFaceAngle  @ 0x94  (TVec3<s16>)  -> BSE: mAngle
//   mVel        @ 0xA4  (TVec3<f32>)  -> BSE: mSpeed
//   mForwardVel @ 0xB0  (f32)         -> BSE: mForwardSpeed
//   unk108      @ 0x108 (TMarioControllerWork*) -> BSE: mController
//   mState      @ 0x118 (u32)         -> BSE: mState
//   mWaterGun   @ 0x3E4 (TWaterGun*)  -> BSE: mFludd

/// Per-player cape runtime data.
struct CapeData {
    bool isActive;          // Currently gliding
    bool isUnlocked;        // Cape ability available
    s32  cooldownTimer;     // Frames remaining before reactivation
    f32  glideTimer;        // Seconds spent in current glide
    f32  pitchAngle;        // Current pitch in degrees

    void reset() {
        isActive       = false;
        isUnlocked     = false;
        cooldownTimer  = 0;
        glideTimer     = 0.0f;
        pitchAngle     = 0.0f;
    }
};

}  // namespace CapeMod
