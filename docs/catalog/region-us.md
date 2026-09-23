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
0x3B0 is `mAppearFromDemo` (JP 0x3AD): written by `startAppearMario(bool)`, read by `perform`'s camera-demo guard; the US byte at 0x3AD is still unidentified.
US-only in this shape: `checkDolpic8()` and the 47 `scDolpicNewsDolpic8_<A-D><a-c><1-4>` tables (upper letter = story suffix, lower = prefix, digit = nozzle middle); the ROM's outer `case 0:` has no `break`, so the whole A group is dead in the shipped game (reproduced deliberately).
Dolpic 5: both flags -> `5_4`, one each -> `5_2`/`5_3`, neither -> `5_1`.
Balloon colour markers are single ASCII bytes (`@` green, `#` orange, `%` yellow, `+`/`<`/`>`/`0xA5` grey, `$` light blue); the escape is `\033FX[30]\033FY[26]`; `case 0` falls through into `case 0x0A`.
Telop font size is `gpSystemFont->getWidth() << 10` with one shared `JUTRect` (that is why `JUTResFont::getWidth` is the map's weak symbol).
Life sounds: `0x480C` under water, `0x4823` on land, `0x4801` on gain; segment colours are `setWhite` only.
`memset(p, 0, 0x400)` is inlined by retail as an 8-byte `stb` loop where we `bl memset` (`string.h` declares it `__declspec(section ".init")`; open).

## `Option`, `ConsoleStr`, `StageUtil`

- `TOptionSubtitleUnit` (a third option row: panes `txp2`/`me_2`/`sel6`/`sel5`) exists only in the US build; `TBalloonControl` does not exist in it at all (`new TOptionRumbleUnit` asks 0x24, not 0x28).
- The US message bank has no ids 8/9, so `scScenarioNameTable` skips them and every later scenario group shifts down by two (now under `VERSION_GMSE01` in `StageUtil.hpp`; `ConsoleStr` data 49 -> 100, `PauseMenu2` 35 -> 76).

## Strings and IDs

- `ProgSelect`: US strings are five named mutable arrays, not Japanese literals. Width 360 and X 145 are US draw arguments.
- `HelpActor`: help ID base is `0x33`; `0xE0030` is non-US only.
- `Application`: shorter disc-error strings shift downstream `.rodata`; see `linking.md`.

- **US debug strings are English where the JP source is Japanese** (batch 100): all 21 `TMenuDirector::setFixedStageValue` stage names (`Beach %d`, `Hotel %d`, `Casino 0/1`, `Park %d`, `Noki %d`, `sea bottom`, `%02d scene %d`, ...), the six Application disc-error messages (raw `\x99` inside "NINTENDO GAMECUBE") and the banner path `/card/mariobnr.bti` (not `_jpn`). A unit whose only nonmatching data rows are Shift-JIS strings in a debug menu should be checked against the target `.rodata` first.

## Linker-computed stack symbols (library pass 135, 2026-09-18)

- **A linker-computed stack symbol is a region constant.** The US map's linker-generated list (lines ~102828-102872 of `marioUS.MAP`) gives `_stack_end 0x804177e4`, `_stack_addr 0x804277e8`, `_db_stack_addr 0x804297e8`, and `__ArenaLo = (_db_stack_addr + 0x1f) & ~0x1f = 0x80429800`. `__init_registers` (`__start`), `__OSThreadInit` (OSThread), `InitMetroTRK` (dolphin_trk) and `OSInit` (OS.c, still at the Japanese 0x80424008; `OSInit`'s three remaining operands are exactly `_stack_addr` and `__ArenaLo`) each materialise one as a literal, and three units had the Japanese values. Guard with `#if defined(VERSION_GMSE01)`; keep `AT_ADDRESS` rather than letting the lcf define the symbol, because the extracted target object has no relocation there. `__start`, OSThread and dolphin_trk linked from this.

## Immediate-constant scan (rg1, 2026-09-23)

- `TCardSave::execMovement_`: the two `waitForChoiceBM(PROGRESS_UNK16, PROGRESS_UNK1, x)` calls (cases UNK15 and UNK33) pass 0 on US, 1 in the JP source.
- `TMarDirector::setupObjects`: debug `TSnapTimeObj` colours differ on US: Pollution Draw and MapObj Draw are `0x0000FFFF` (JP source `0xFF00FFFF`), Shadow Draw is `0xFF0000FF` (JP source `0xFFFF00FF`).
