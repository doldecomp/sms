# US (GMSE01) layout differences

Guard these with `#if defined(VERSION_GMSE01)`; the other regions cannot be rebuilt here.
Keep Japanese member names and annotate the offset shift rather than renaming.

## `MSound`

Inventory before changing the header: `MSound.cpp`, `MSoundSE.cpp`, `MAnmSound.cpp`, `MSoundMainSide.cpp`, `MarDirectorDirect.cpp`, `MarioSound.cpp`, `BossHanachanNerve.cpp`, plus `MSGMSound`, `gpMSound`, `SMSGetMSound()` and unqualified member accesses.

- 0x98: `u8 mWaterFilterOverride`, compared with 1 in `MSSeCallBack::setWaterCameraFir`. Keep the original enabled/disabled filter store, then apply the override to 0x78; a logical OR changes the sequence.
- 0x9A: `u16 mTimerParameter`, written by every branch of `MSound::playTimer`, read by sequence callback case 15. Non-US code keeps base `unk94`.
- Consequent offsets: tempo controller 0x9C, crossfade 0xA0, flags 0xAC, camera array 0xB0, camera sound handle 0xC8, demo flags 0xCC, scene bytes 0xD1/0xD2. Do not compensate with per-caller casts or by swapping controller types.
- Constructor: `setWaterCameraFir(false)` goes after zeroing handles 0x7C/0x80; the early filter store before `initDriver` is non-US only.
- **Open:** the constructor stores -1 as a word at 0x94 and `startMarioVoice` uses it as the previous voice ID, but `JAIBasic.hpp` declares a halfword there. Base/derived ownership of the tail fields is unresolved. (Libraries are now in scope, so `JAIBasic.hpp` may be corrected once its own evidence supports it.)
- `MSound::getDistPowFromCamera(const Vec&)` is US-only, 136 bytes at 0x800151C8, between `getDistFromCamera` and the constructor in source order.
- Sequence callback: US handles commands 0, 1, 12, 13, 15, 20, 30, 110, 120, 121 and explicit 127. Commands 1 and 127 fall through to `JAIBasic::setParameterSeqSync`; 40 and 123-126 use the base callback. Keep explicit case 127 so the `bge` for values >= 122 emerges. Case 110 returns 0xFFFF for scene 8 with episode 6 or 1, else the loaded scene.
- `MSMarioPosVolume::getDistFromMario` (UNUSED, `MAnmSound.cpp`) differs: its caller sums after all three `powf` calls and calls `std::sqrtf` out of line. Do not globally replace `std::powf`.

## `GCConsole2`

The US constructor writes a `u16` at 0x3AE and a byte at 0x3B0; later pointers start at 0x3B4.
Their US timer/flag behaviour is not reconstructed yet, and existing `unk3AC[1]` users are a follow-up.

## Strings and IDs

- `ProgSelect`: US strings are five named mutable arrays, not Japanese literals. Width 360 and X 145 are US draw arguments.
- `HelpActor`: help ID base is `0x33`; `0xE0030` is non-US only.
- `Application`: shorter disc-error strings shift downstream `.rodata`; see `linking.md`.
