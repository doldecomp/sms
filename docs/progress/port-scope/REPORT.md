# PC-port compile scope: game + JSystem under g++ 13

Measured 2026-09-23 on `wt/c-pcscope` (base `5c5f8a71`), read-only: nothing under `src/` or `include/` was changed.
Question: how much of the decomp's game + JSystem source compiles with a mainstream compiler today, what has to change, and what platform surface does a PC port have to provide.
Machine-readable tables sit next to this file:
`errors.tsv` lists every unique error location in every pass,
and `api-surface.tsv` lists every SDK function the in-scope code calls, with use counts.
The measurement scripts are in `scripts/`.

## Answer in one screen

- **As-is, with the project's own headers (MSL, `-m32`): 32 of 589 units pass `g++ -fsyntax-only` (5.4%).**
  Game code: 6 / 383 (1.6%).
  JSystem: 21 / 200 (10.5%).
  THPPlayer: 4 / 5.
- **As-is, with the host's glibc/libstdc++ in place of MSL (`-m64`): 160 / 589 (27.2%).**
  JSystem: 140 / 200 (70.0%).
  Game code: 14 / 383 (3.7%).
- The failures are few and concentrated, not spread through the code.
  The strict MSL pass (`s32`) has 113 unique error locations.
  52 of them are inside MSL headers or caused by MSL typedefs, and they go away when the port uses host headers.
  That leaves **61 unique locations** in game/JSystem code and the SDK headers they include.
  Four header sites block most units:
  MSL `string.h` `__declspec(section)` blocks 509 units,
  `__cntlzw` in `dolphin/gd/GDLight.h` blocks 300,
  MSL `iterator` without `typename` blocks 220,
  and `MActorData.hpp:82` (`J3DAnmBase**` to `void**`) blocks 182.
- `-DGEKKO` makes no practical difference: the result is 32 / 589 with or without it.
  Only `include/dolphin/os.h:209` checks it, and without it `JASTrack.cpp` loses `OSf32tos8`, so keep it defined.
- A 64-bit build adds a separate class of problems (309 pointer-truncation sites in 59 units, pointer-tag enums, `long` vs `u32` spellings).
  **Target 32-bit x86 first.**
- The platform surface is dominated by GX.
  GX accounts for 149 of the 377 distinct SDK functions and 3,940 of the 4,884 uses.
  1,012 of those uses are the inline vertex writers (`GXPosition3f32` and friends) that store straight to the write-gather pipe at `0xCC008000`.

## Method

- **Units:** every `.c`/`.cpp` under `src/` except `src/dolphin`, `src/PowerPC_EABI_Support` and `src/TRK_MINNOW_DOLPHIN`, 589 in all.
  That is 383 game units (including `main.cpp`), 200 JSystem, 5 THPPlayer and 1 OdemuExi2.
  Every one of them already compiles under MWCC (`docs/progress/nonmatching-link/SUMMARY.txt`), so every error below is a difference between the compilers, not broken source.
- **Flags per unit:** taken from the unit's `build.ninja` edge where one exists (140 units), otherwise from its `configure.py` library.
  The `-i` paths become `-I`, and the `-D`s are passed through (`-DVERSION_GMSE01 -DBUILD_VERSION=2 -DNDEBUG=1 -DGEKKO`).
  `-prefix SMS.mch` becomes `-include include/SMS.pch`, which applies to 299 units.
  `.c` files in JSystem and THPPlayer compile as C++ because those libraries pass `-lang=c++`.
  The two remaining C units (`GC2D/hx_wiper.c`, `OdemuExi2/DebuggerDriver.c`) go through `gcc -std=gnu99`.
- **Base command:** `g++ -x c++ -std=gnu++03 [-fpermissive] -fsyntax-only -w -fmax-errors=0 -m32|-m64 ...`, run at `nice -n 19` with two jobs.
- **Toolchain notes:**
  g++ 13.3 is installed; clang is not.
  **gcc-multilib / g++-multilib are not installed.**
  `-m32 -fsyntax-only` still works with the repo's MSL headers, because nothing reaches glibc.
  With host headers `-m32` dies on the missing `gnu/stubs-32.h` and 32-bit `bits/c++config.h`, so the host-header passes use `-m64`.
  A real 32-bit Linux port build needs `gcc-multilib g++-multilib`.
- **Classification:** each unique `file:line:col:message` is classified once by message text and source line.
  "Error lines" counts repeats across units, so a header error counts once per unit that includes it.
  The fix curves are greedy: at each step they fix whichever category unblocks the most units.

### Passes

| Pass | Configuration | Clean TUs | Game | JSystem | THPPlayer | Error lines | Unique locations |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: |
| `p32` | `-m32 -fpermissive`, MSL headers, `-DGEKKO` (closest to the MWCC configuration) | 32 / 589 (5.4%) | 6 / 383 | 21 / 200 | 4 / 5 | 3295 | 97 |
| `s32` | as p32 without `-fpermissive` | 32 / 589 (5.4%) | 6 / 383 | 21 / 200 | 4 / 5 | 5761 | 113 |
| `p32ng` | as p32 without `-DGEKKO` | 32 / 589 (5.4%) | 6 / 383 | 21 / 200 | 4 / 5 | 3297 | 99 |
| `p64` | `-m64`, strict, MSL headers, `u32`/`s32` forced to `int` | 32 / 589 (5.4%) | 6 / 383 | 21 / 200 | 4 / 5 | 4079 | 409 |
| `host64` | `-m64 -fpermissive`, host glibc/libstdc++ instead of MSL, stock `u32` | 160 / 589 (27.2%) | 14 / 383 | 140 / 200 | 5 / 5 | 1660 | 171 |
| `compat64` | `-m64`, strict, host headers, `u32`=`int`, compat header (`__declspec`, `__cntlzw` only), `-fno-gnu-keywords`, case-fixed includes | 135 / 589 (22.9%) | 27 / 383 | 103 / 200 | 4 / 5 | 3164 | 518 |

`compat64` is a port-like probe, not a finished configuration.
Its compat header only covered what the MSL passes had shown, and it counts 64-bit problems as errors.
If the errors that g++ tags `[-fpermissive]` are ignored, it has 167 / 589 clean.

`-fpermissive` changes no pass/fail result.
It only hides 16 unique locations: 13 `operator new(size_t)` signature mismatches and 3 const-dropping conversions.

### Fix curves (units clean after each category is fixed)

- `s32`, MSL headers:
  declspec 128, size_t-width 210, `__cntlzw` 283, MSL `typename` 333, `MActorData` cast 472, include case 518, `typeof` 541, MSL cascade 558, two-phase lookup 574.
  The last 15 units need about 25 one-off leniency fixes, which reach 589.
- `host64`, host headers:
  intrinsics 329, implicit conversion 456, include case 502, MSL-provided libc names 533, `typeof` 556, two-phase lookup 571, then about 20 one-off sites reach 589.
- `compat64`, host headers, 64-bit:
  intrinsics 256, pointer-tag enums 291, pointer truncation 330, `long`/`u32` spelling 354, libc names 373, `MActorData` cast 555, two-phase lookup 571, then one-off sites reach 589.

## Error categories

Counts are unique locations / units affected.
The `s32` column is the project's MSL configuration; `host64`/`compat64` is the port-like configuration; "-" means the category does not occur there.
Every fix marked **neutral** is a spelling change that MWCC compiles to the same bytes.
Land those in the decomp, but still run the usual `ninja changes_all` check.
Fixes marked **verify** can move MWCC codegen and need that check per site.

| # | Category | `s32` | `host64` / `compat64` | Examples | Suggested fix |
| --- | --- | --- | --- | --- | --- |
| 1 | MWCC `__declspec(section ".init")` | 3 / 509 | - (see 19) | `PowerPC_EABI_Support/.../string.h:10,11,16` | Port uses host headers; otherwise the compat header does `#define __declspec(x)`. |
| 2 | MSL typedef width: `operator new/delete` not taking the compiler's `size_t` | 13 / 351 | 6 / 1 | `JKRHeap.hpp:195-197` (MSL `size_t` is `unsigned long`), `JKRHeap.cpp:255` (defined with `u32`) | Spell the parameter `size_t` in `JKRHeap.cpp` (**neutral**: `size_t` is `unsigned long` = `u32` under MWCC). |
| 3 | PPC intrinsics | 1 / 300 | 18 / 367 | `dolphin/gd/GDLight.h:78` `__cntlzw`; `JGUtil.hpp:70,79`, `MathUtil.hpp:371`, `fake_tgmath.h:11,26`, `JPAMath.cpp:8` `__frsqrte` (13 sites); `JPAField.cpp:91,98`, `JPAMath.cpp:165`, `MathUtil.cpp:130` `__fres` (4 sites) | Port compat header: `__frsqrte(x)` becomes `1.0/sqrt(x)`, `__fres(x)` becomes `1.0f/(x)`, `__cntlzw(x)` becomes `x ? __builtin_clz(x) : 32`. The hardware estimates are about 12-bit, so results drift slightly from GameCube. |
| 4 | Missing `typename` (MWCC leniency) | 5 / 220 | - | `MSL_C++/MSL_Common/iterator:15-19` | Only in MSL. Use host `<iterator>`, or add `typename` (**neutral**). |
| 5 | Implicit pointer conversions MWCC accepts | 4 / 183 | 8 / 209 | `M3DUtil/MActorData.hpp:82` `J3DAnmBase**` to `void**` (one line, 182 units); const-dropping at `JGadget/linklist.hpp:287`, `Enemy/tinkoopa.cpp:1104,1109`, `MoveBG/MapObjHide.cpp:626` | Explicit casts (**neutral**). |
| 6 | Include path case (MWCC runs case-insensitive) | 6 / 46 | 6 / 46 | `J3DMaterial.hpp:7` `J3d/`; `Enemy/beam.cpp:1`, `MarNameRefGen_Enemy.cpp:54` `beam.hpp`; `hamukuri.cpp:26` `MSoundSe.hpp`; `MapEventSink.cpp:21` `Jsystem/`; `MessageUtil.cpp:1` `GC2d/` | Correct the spelling (**neutral**). This is the complete list; a case-insensitive resolver found no other mismatches and no missing files. |
| 7 | `typeof` used as an identifier (GNU keyword) | 13 / 23 | 0 with `-fno-gnu-keywords` | `Strategic/spcinterp.hpp:98` `TSpcSlice::typeof()`, `spcinterp.cpp:100,243,...`, `System/EventWatcher.cpp:100` | Build flag `-fno-gnu-keywords` (GCC) or `-std=c++03` instead of `gnu++03`; MSVC is unaffected. Do not rename it. |
| 8 | Cascade of #1 (`memcpy`/`memset` undeclared) | 31 / 17 | - | `JDRRenderMode.cpp:88`, `CardManager.cpp:41` | Disappears with #1. |
| 9 | Two-phase name lookup | 5 / 17 | 5 / 17 | `JGeometry/JGQuat4.hpp:49,62` `set(...)` from a dependent base; `JSupport/JSUList.hpp:180,182` `append`/`remove`; `Camera/cameralib.hpp:86` function called before it is declared | `this->set(...)`, `this->append(...)`, and a forward declaration (**neutral**). With host headers, bare `remove(child)` binds to libc `remove(const char*)`, which is the `JSUList.hpp:182` "cannot convert" error. |
| 10 | Reference binding leniency | 8 / 4 | 10 / 5 | `Enemy/bombhei.cpp:282`, `Enemy/tobiPuku.cpp:898` (const `TVec3` to non-const ref); `GC2D/GCConsole2.cpp:833-851` (rvalue to `TBoundPane*&`); `System/MarDirectorInitECT.cpp:121` | Const-correct the callee or use a named temporary (**verify**). |
| 11 | `goto`/`case` jumping over an initialisation | 9 / 4 | 9 / 4 | `Enemy/gesso.cpp:675,678`; `Player/MarioAutodemo.cpp:37,41`; `System/MarDirectorDirect.cpp:515-542`; `JASystem/JASTrack.cpp:809` | Brace the case bodies or hoist the declarations (**verify**, since the scopes can move stack slots). |
| 12 | MWCC `asm` functions | 6 / 2 | 6 / 2 | `J3DTransform.cpp:33,391,414,539`; `MarioUtil/MathUtil.cpp:474,491` | See "Inline asm" below. |
| 13 | Overload ambiguity | 2 / 2 | 2 / 2 | `JKernel/JKRDvdArchive.cpp:12`, `JKRMemArchive.cpp:41` `JKRArchive(0, MOUNT_x)` | Cast the literal to the intended parameter type (**neutral**). |
| 14 | Conflicting local redeclaration | 4 / 1 | 4 / 1 | `Map/MapDraw.cpp:15,16` redeclare `SMSGetGameRenderWidth/Height` as `int` | Include the owning header and drop the local declaration (**verify**). |
| 15 | Access control leniency | 2 / 1 | 2 / 1 | `Enemy/elecNokonoko.cpp:82,91` private `TSpineBase<>::Nerve` | Make the typedef public (**neutral**). |
| 16 | `void main()` | 1 / 1 | 1 / 1 | `main.cpp:6` | In the port, `#define main SMS_main` under `TARGET_PC` and call it from the platform `main`. |
| 17 | Names that MSL provides transitively | - | 81 / 38 (host64); 98 / 222 (compat64) | `snprintf` without `<stdio.h>` (`M3DUtil/MActorData.hpp:72-75`, `MapObjLib.cpp:243`); `std::sqrtf` (32), `std::powf`, `std::fmodf`, `std::fabsf` (`Enemy/KoopaNerve.hpp:17`, `Player/MarioPhysics.cpp:55`); `DEG_TO_RAD`/`RAD_TO_DEG`/`TAU` (defined in MSL `math.h`; `J2DPane.cpp:255`, `GC2D/Option.cpp:199`); `va_start` (`JUTDirectPrint.cpp:151`) | Force-included port compat header that pulls in `<stdio.h> <stdarg.h> <math.h> <ctype.h>` and adds `namespace std { using ::sqrtf; using ::powf; ... }`. Move `DEG_TO_RAD`/`TAU` out of MSL `math.h` into a game header (**neutral**). |
| 18 | Clashes with host libc | - | 8 / 2 | `JSupport/JSUStreamEnum.hpp:10` `enum EIoState { GOOD, EOF }` against `#define EOF (-1)`; `include/fake_tgmath.h:22,38,48` float `sqrt/fabs/floor` overloads against libstdc++ | Under `TARGET_PC`, `#undef EOF` before the enum. Guard the `fake_tgmath.h` overloads with `__MWERKS__`. |
| 19 | MSL header included by path | - | 7 / 1 | `MoveBG/MapObjTown.cpp:21` (`string.h`; errors); `JSystem/JMath.cpp:4` (`math.h`; no errors yet) | Include `<math.h>`/`<string.h>` instead (**neutral**, since the MSL dirs are on MWCC's `-i` path). |
| 20 | 64-bit: pointer to 32-bit integer casts | - | 309 / 59 (compat64); 274 / 51 (p64) | `JSupport.hpp:11,21` (`JSUConvertOffsetToPtr`); `JKRExpHeap.hpp:27`; per file: `System/EventWatcher.cpp` 92, `NPC/NpcEvent.cpp` 21, `JKRExpHeap.cpp` 12, `J3DModelLoader.cpp` 12, `PacketUtil.cpp` 11, `liveinterp.cpp` 10, `spcinterp.cpp` 9, `JKRAram.cpp` 9 | Address arithmetic can move to `uintptr_t` (**neutral**: it is `u32` under MWCC). The SPC script VM keeps pointers in 32-bit slots and the resource loaders turn 32-bit file offsets into pointers in place, so 64-bit needs design work. Ship 32-bit first. |
| 21 | 64-bit: pointer-tag flag enums | - | 6 / 237 | `J3DGraphBase/J3DPacket.hpp:184` `DIFF_FLAG = 1 << DIFF_BIT`; `J3DMaterial.hpp:125,126` `1 << (sizeof(uintptr_t) * CHAR_BIT - 1)` | Use a `uintptr_t` constant instead of an `int` enum under `TARGET_PC` (32-bit builds are unaffected). |
| 22 | Code spelling `long`/`unsigned long` where headers say `s32`/`u32` | - | 25 / 33 (only when `u32` is made `int`) | `JKRArchive.hpp:121` / `JKRFileCache.hpp:35` `getResSize` return type; `Camera/sunmgr.cpp:112`, `GC2D/CardSave.cpp` (6), `GC2D/MovieRumble.cpp` (4), `Map/MapDraw.cpp:100`, `JKRFileFinder.cpp:5`, `JKRExpHeap.cpp:1030` | Respell as `u32`/`s32` (**neutral**: same mangling, because MWCC's `u32` is `unsigned long`). This is required before `u32` can become `int`. |

Pass-by-pass tables with full example lists are in the appendix below and in `errors.tsv`.

### `errors.tsv` columns

`pass`, `category`, `file`, `line`, `col`, `kind`, `message`, `tus_affected`, `sample_tu`, `source_line`: one row per unique error location per pass (1,407 rows).
Filter on `pass == "s32"` for the MSL configuration and on `pass == "compat64"` for the port-like one.

## Things that compile but are wrong on a non-MWCC compiler

A clean syntax check does not mean correct code.
These items pass g++ silently.

- **Compiled-out bodies.**
  The game/JSystem tree has 25 `#ifdef __MWERKS__` blocks, and 24 of them have no `#else`.
  - 22 of those hold Gekko assembly, so on g++ these functions come out empty or half-done:
    - `J3DTransform.cpp`: 8 blocks. They are the bodies of the four `asm` functions below plus the ones in `J3DScaleNrmMtx33`, `J3DMtxProjConcat`, `J3DPSMtx33Copy` and `J3DMTXConcatArrayIndexedSrc`.
    - `J3DModel.cpp`: 5 blocks, in `calcWeightEnvelopeMtx` and its neighbours.
    - `J3DTransform.hpp`: 4 blocks, in the `J3DPSMulMtxVec` overloads.
    - `MathUtil.cpp`: 2 blocks (`MsVECMag2`, `MsVECNormalize`).
    - One block each in `J3DAnimation.cpp` (`J3DHermiteInterpolationS`), `JUTException.cpp` (`getFpscr`) and `JGMatrix34.hpp` (`gekko_ps_copy12`).
    - The SDK header `dolphin/os.h` adds `OSInitFastCast` and `OSf32tos8`.
  - `JASTrack.hpp:88,143` compiles the `MoveParam_`/`AInnerParam_` constructors only for MWCC, so g++ leaves those fields uninitialised.
  - `JUTConsole.hpp:154` hard-codes `-sizeof(JKRDisposer)` in place of `offsetof(JUTConsole, mLinkNode)` for non-MWCC compilers, which holds only while that layout does; recheck it on 64-bit.
- **`u32`/`s32` are `unsigned long`/`signed long`** (`include/dolphin/types.h:8-9`).
  That is 64 bits on LP64 hosts (Linux and macOS, x86-64 and arm64), which silently changes every struct that mirrors a file or hardware layout.
  On LP64 they must be `int` under `TARGET_PC`, and that needs category 22 fixed first.
  Windows (LLP64) is unaffected.
- **`AT_ADDRESS` expands to nothing on GCC.**
  The write-gather pipe (`GXWGFifo`, `0xCC008000`) is a plain pointer dereference.
  The 1,012 vertex-writer uses and the direct use in `MarioUtil/PacketUtil.cpp` will fault at run time unless the platform layer redefines `GXWGFifo`/`GXPosition*`.
- **Other direct hardware access:**
  - `__DSPRegs` in `JSystem/osdsp_task.c`.
  - `OSPhysicalToCached` in `dsptask.c`, `JKRHeap.cpp` and `JRenderer.cpp`.
  - Arena functions (`OSGet/SetArenaLo/Hi`) in `JKRHeap.cpp`/`JUTException.cpp`.
  - Low-memory `*(OSModuleInfo**)0x800030C8` at `JUTException.cpp:191`.
  - `LCEnable` in `THPPlayer.c`.
  - `OdemuExi2` (the debugger link: `__PIRegs`, `__EXIRegs`, `0xE0000000`) should be dropped from the port.
- **`char` signedness:** a probe compile shows MWCC GC/1.2.5 treats plain `char` as signed by default (`extsb`), the same as x86 GCC/Clang/MSVC.
  ARM64 Linux and Android default to unsigned, so pass `-fsigned-char` there.
- **No `#pragma` produced an error**, because g++ ignores unknown pragmas.
- **Big-endian data** (J3D/J2D/JPA/BMG/audio/scene archives read into structs in place) is outside a syntax check, but it is the largest run-time work item after GX.

## Inline asm inventory (game, JSystem, THPPlayer, and the SDK headers they include)

| File | Whole-function `asm` | `asm {}` blocks | Functions |
| --- | ---: | ---: | --- |
| `src/JSystem/J3D/J3DGraphBase/J3DTransform.cpp` | 4 | 4 | `J3DPSCalcInverseTranspose`, `J3DPSMtx33CopyFrom34`, `J3DPSMtxArrayCopy`, `J3DPSMtxArrayConcat`; blocks in `J3DScaleNrmMtx33`, `J3DMtxProjConcat`, `J3DPSMtx33Copy`, `J3DMTXConcatArrayIndexedSrc` |
| `src/JSystem/J3D/J3DGraphAnimator/J3DModel.cpp` | 0 | 5 | `J3DModel::calcWeightEnvelopeMtx` and neighbours |
| `src/MarioUtil/MathUtil.cpp` | 2 | 0 | `MsVECMag2`, `MsVECNormalize` |
| `src/JSystem/J3D/J3DGraphAnimator/J3DAnimation.cpp` | 0 | 1 | `J3DHermiteInterpolationS` |
| `src/JSystem/JUtility/JUTException.cpp` | 0 | 1 | `JUTException::getFpscr` |
| `include/JSystem/J3D/J3DGraphBase/J3DTransform.hpp` | 0 | 4 | `J3DPSMulMtxVec` (4 overloads) |
| `include/JSystem/JGeometry/JGMatrix34.hpp` | 0 | 1 | `gekko_ps_copy12` |
| `include/dolphin/os.h` (SDK) | 0 | 2 | `OSInitFastCast`, `OSf32tos8` |

That is 6 whole-function `asm` definitions and 18 `asm {}` blocks, about 24 bodies.
They are 3x4-matrix, vector and quantisation routines with well-known C equivalents (the SDK's own `C_MTX*` versions show the semantics).
A **neutral** pattern keeps MWCC bytes identical:
`#ifdef __MWERKS__` / `#define ASM asm` / `#else` / `#define ASM` / `#endif` on the signature, with each asm body followed by an `#else` C++ body.

## Platform API surface

Calls from game, JSystem and THPPlayer sources plus non-SDK headers.
Names come from the declarations in `include/dolphin/**` and the macro-generated `GXVert.h` writers; comments are stripped first.
Per-function rows are in `api-surface.tsv`.

| Subsystem | Distinct functions | Uses | Game src | JSystem src | Game/JSystem headers | THPPlayer + OdemuExi2 | Most used |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| GX | 149 | 3940 | 3049 | 792 | 8 | 91 | GXPosition3f32 388, GXTexCoord2f32 227, GXSetVtxDesc 171, GXSetVtxAttrFmt 168, GXSetTevOrder 148, GXSetChanCtrl 127 |
| OS | 72 | 414 | 100 | 216 | 35 | 63 | OSPanic 32, OSRestoreInterrupts 30, OSSendMessage 28, OSReceiveMessage 25, OSInitMessageQueue 23, OSUnlockMutex 22 |
| GD | 32 | 117 | 46 | 65 | 6 | 0 | GDOverflowCheck 36, GDSetCurrent 9, GDSetZMode 8, GDSetBlendMode 5, GDBegin 4, GDEnd 4 |
| OS cache | 7 | 78 | 28 | 44 | 0 | 6 | DCStoreRange 41, DCFlushRange 24, DCInvalidateRange 9, DCFlushRangeNoSync 1, DCZeroRange 1, LCDisable 1 |
| DVD | 20 | 78 | 14 | 48 | 0 | 16 | DVDReadPrio 20, DVDClose 15, DVDOpen 7, DVDConvertPathToEntrynum 6, DVDChangeDir 4, DVDGetDriveStatus 4 |
| VI | 12 | 61 | 20 | 34 | 0 | 7 | VIWaitForRetrace 15, VIGetTvFormat 13, VIGetRetraceCount 7, VIGetNextField 5, VISetBlack 5, VIFlush 4 |
| MTX (paired-single) | 13 | 47 | 45 | 1 | 0 | 1 | PSMTXCopy 9, PSVECNormalize 9, PSMTXConcat 8, PSMTXIdentity 7, PSVECCrossProduct 4, PSMTXMultVec 2 |
| MTX (C) | 6 | 35 | 26 | 8 | 0 | 1 | C_MTXOrtho 11, C_MTXLookAt 9, C_MTXPerspective 8, C_MTXLightPerspective 5, C_MTXLightFrustum 1, C_MTXLightOrtho 1 |
| CARD | 21 | 30 | 30 | 0 | 0 | 0 | CARDClose 6, CARDGetFileNo 2, CARDGetStatus 2, CARDSetIconFormat 2, CARDSetIconSpeed 2, CARDCheck 1 |
| DSP | 7 | 23 | 0 | 23 | 0 | 0 | DSPCheckMailToDSP 7, DSPSendMailToDSP 6, DSPCheckMailFromDSP 4, DSPReadMailFromDSP 3, DSPAddTask 1, DSPAssertInt 1 |
| AI | 11 | 17 | 0 | 16 | 0 | 1 | AISetStreamVolLeft 3, AISetStreamVolRight 3, AIInitDMA 2, AISetStreamPlayState 2, AIGetDSPSampleRate 1, AIInit 1 |
| PAD | 8 | 15 | 5 | 9 | 1 | 0 | PADControlMotor 8, PADClamp 1, PADInit 1, PADRead 1, PADRecalibrate 1, PADReset 1 |
| DB | 7 | 7 | 0 | 0 | 0 | 7 | DBClose 1, DBInitComm 1, DBInitInterrupts 1, DBOpen 1, DBQueryData 1, DBRead 1 |
| MTX | 1 | 6 | 3 | 3 | 0 | 0 | MTXMultVecSR 6 |
| AR | 4 | 6 | 0 | 6 | 0 | 0 | ARAlloc 3, ARGetBaseAddress 1, ARGetSize 1, ARInit 1 |
| ARQ | 2 | 5 | 0 | 5 | 0 | 0 | ARQPostRequest 4, ARQInit 1 |
| THP | 3 | 3 | 0 | 0 | 0 | 3 | THPAudioDecode 1, THPInit 1, THPVideoDecode 1 |
| OS/PPC | 2 | 2 | 0 | 2 | 0 | 0 | PPCMfmsr 1, PPCSync 1 |
| **Total** | 377 | 4884 | 3366 | 1272 | 50 | 196 | |

Reading the table as work for the port's platform layer:

- **GX + GD (181 functions, 4,057 uses)** is the renderer.
  The port needs a GX-to-GPU layer that also consumes the GX command lists J3D builds with GD (`GDOverflowCheck`/`GDSet*`) and the WG-pipe vertex writes.
  Reusing an existing open-source GX emulation layer (for example encounter's `aurora`) is far cheaper than writing one.
- **OS (72 + 7 cache)**: threads, message queues, mutexes, interrupts, alarms, time and arena map onto host threads and synchronisation.
  The cache ops become no-ops.
- **DVD (20), CARD (21), PAD (8), VI (12)**: file I/O on an extracted disc, save files, SDL-style input and rumble, window, present and vsync.
- **Audio: DSP (7), AI (11), AR (4), ARQ (2)**, almost entirely inside JAudio (one AI call is in THPPlayer).
  The DSP microcode mixer and ARAM streaming need a software replacement.
  That is a contained but non-trivial subsystem.
- **MTX (20)**: map the `PSMTX*`/`PSVEC*` calls to the C implementations.
- **THP (3)**: needs a software THP (MJPEG-style) video decoder, because the SDK decoder in `src/dolphin/thp` is excluded here and is PPC-specific.

## Rough effort

1. **Syntax-clean on GCC/Clang, 32-bit, C++03**, which is the first milestone.
   - About 61 unique sites in game/JSystem/SDK-header code.
   - The host-header-only categories 17-19 (96 unique locations, nearly all fixed by the compat header rather than by edits).
   - A roughly 40-line port compat header (intrinsics, `std::` libm aliases, stdio/stdarg includes, `main` rename).
   - Build flags: `-fno-gnu-keywords` and `-DGEKKO`, plus the six include-case fixes.
   - C++ fallbacks for about 24 asm bodies.
   - Estimate: **2-4 engineer-days**, and 1-2 of those are the asm fallbacks.
   - Nearly every decomp-side change is neutral.
     About 20 sites (categories 10, 11 and 14) need a per-site `changes_all` check.
2. **64-bit** adds about 340 sites (categories 20-22) plus design work for pointer-in-32-bit-slot data: the SPC VM, resource relocation, and pointer-tag flags.
   That is **weeks**; defer it until the 32-bit build runs.
3. **Platform layer**:
   - GX dominates; it takes months from scratch, or weeks if an existing GX emulation layer is adopted.
   - OS, DVD, CARD, PAD and VI: about 2-4 weeks.
   - Audio DSP replacement: several weeks.
   - Big-endian asset loading: not measured here, likely the second-largest item after GX.

## Appendix: per-pass category tables (generated)

Examples show the three locations that hit the most units.
The "Greedy fix curve" lines are the source of the fix curves above.

### Categories in `s32`

| Category | Unique locations | Error lines (all TUs) | TUs affected | Examples |
| --- | ---: | ---: | ---: | --- |
| declspec | 3 | 1527 | 509 | `include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/string.h:10: expected constructor, destructor, or type conversion before '(' token`<br>`include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/string.h:11: expected constructor, destructor, or type conversion before '(' token`<br>`include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/string.h:16: expected constructor, destructor, or type conversion before '(' token` |
| size_t-width | 13 | 2463 | 351 | `include/JSystem/JKernel/JKRHeap.hpp:195: 'operator new' takes type 'size_t' ('unsigned int') as first parameter [-fpermissive]`<br>`include/JSystem/JKernel/JKRHeap.hpp:196: 'operator new' takes type 'size_t' ('unsigned int') as first parameter [-fpermissive]`<br>`include/JSystem/JKernel/JKRHeap.hpp:197: 'operator new' takes type 'size_t' ('unsigned int') as first parameter [-fpermissive]` |
| ppc-intrinsic | 1 | 300 | 300 | `include/dolphin/gd/GDLight.h:78: '__cntlzw' was not declared in this scope` |
| missing-typename | 5 | 1100 | 220 | `include/PowerPC_EABI_Support/Msl/MSL_C++/MSL_Common/iterator:15: need 'typename' before 'I::difference_type' because 'I' is a dependent scope`<br>`include/PowerPC_EABI_Support/Msl/MSL_C++/MSL_Common/iterator:16: need 'typename' before 'I::value_type' because 'I' is a dependent scope`<br>`include/PowerPC_EABI_Support/Msl/MSL_C++/MSL_Common/iterator:17: need 'typename' before 'I::pointer' because 'I' is a dependent scope` |
| implicit-conv | 4 | 185 | 183 | `include/M3DUtil/MActorData.hpp:82: cannot convert 'J3DAnmBase**' to 'void**'`<br>`include/JSystem/JGadget/linklist.hpp:287: invalid conversion from 'const JUTConsole*' to 'JUTConsole*' [-fpermissive]`<br>`src/Enemy/tinkoopa.cpp:1104: invalid conversion from 'const TNerveBase<TLiveActor>*' to 'TNerveBase<TLiveActor>*' [-fpermissive]` |
| case-include | 6 | 46 | 46 | `include/JSystem/J3D/J3DGraphBase/J3DMaterial.hpp:7: JSystem/J3d/J3DGraphBase/Blocks/J3DTevBlocks.hpp: No such file or directory`<br>`src/Enemy/beam.cpp:1: Enemy/beam.hpp: No such file or directory`<br>`src/Enemy/hamukuri.cpp:26: MSound/MSoundSe.hpp: No such file or directory` |
| gnu-keyword | 13 | 57 | 23 | `include/Strategic/spcinterp.hpp:98: expected primary-expression before ')' token`<br>`include/Strategic/spcinterp.hpp:98: expected unqualified-id before '{' token`<br>`src/Strategic/spcinterp.cpp:100: expected unqualified-id before 'typeof'` |
| msl-cascade | 31 | 31 | 17 | `src/GC2D/MovieSubtitle.cpp:92: 'memset' was not declared in this scope`<br>`src/JSystem/JDrama/JDRRenderMode.cpp:88: 'memcpy' was not declared in this scope`<br>`src/JSystem/JDrama/JDRRenderMode.cpp:93: 'memcpy' was not declared in this scope` |
| two-phase-lookup | 5 | 19 | 17 | `include/JSystem/JGeometry/JGQuat4.hpp:62: 'set' was not declared in this scope, and no declarations were found by argument-dependent lookup at the poi`<br>`include/Camera/cameralib.hpp:86: 'CLBTwoDegreeGeneralInbetween' was not declared in this scope, and no declarations were found by argument-dependent l`<br>`include/JSystem/JGeometry/JGQuat4.hpp:49: 'set' was not declared in this scope, and no declarations were found by argument-dependent lookup at the poi` |
| jump-over-init | 9 | 9 | 4 | `src/Enemy/gesso.cpp:675: jump to case label`<br>`src/Enemy/gesso.cpp:678: jump to case label`<br>`src/JSystem/JAudio/JASystem/JASTrack.cpp:809: jump to label 'bail'` |
| ref-binding | 8 | 8 | 4 | `src/Enemy/bombhei.cpp:282: binding reference of type 'JGeometry::TVec3<float>&' to 'const JGeometry::TVec3<float>' discards qualifiers`<br>`src/Enemy/tobiPuku.cpp:898: binding reference of type 'JGeometry::TVec3<float>&' to 'const JGeometry::TVec3<float>' discards qualifiers`<br>`src/GC2D/GCConsole2.cpp:833: cannot bind non-const lvalue reference of type 'TBoundPane*&' to an rvalue of type 'TBoundPane*'` |
| asm-function | 6 | 6 | 2 | `src/JSystem/J3D/J3DGraphBase/J3DTransform.cpp:33: expected '(' before 'bool'`<br>`src/JSystem/J3D/J3DGraphBase/J3DTransform.cpp:391: expected '(' before 'void'`<br>`src/JSystem/J3D/J3DGraphBase/J3DTransform.cpp:414: expected '(' before 'void'` |
| overload-ambiguity | 2 | 2 | 2 | `src/JSystem/JKernel/JKRDvdArchive.cpp:12: call of overloaded 'JKRArchive(int, JKRArchive::EMountMode)' is ambiguous`<br>`src/JSystem/JKernel/JKRMemArchive.cpp:41: call of overloaded 'JKRArchive(int, JKRArchive::EMountMode)' is ambiguous` |
| conflicting-redecl | 4 | 4 | 1 | `src/Map/MapDraw.cpp:15: ambiguating new declaration of 'int SMSGetGameRenderHeight()'`<br>`src/Map/MapDraw.cpp:16: ambiguating new declaration of 'int SMSGetGameRenderWidth()'`<br>`src/Map/MapDraw.cpp:77: 'SMSGetGameRenderWidth' was not declared in this scope` |
| access-control | 2 | 2 | 1 | `src/Enemy/elecNokonoko.cpp:82: 'typedef const class TNerveBase<TLiveActor>* TSpineBase<TLiveActor>::Nerve' is private within this context`<br>`src/Enemy/elecNokonoko.cpp:91: 'typedef const class TNerveBase<TLiveActor>* TSpineBase<TLiveActor>::Nerve' is private within this context` |
| main-signature | 1 | 1 | 1 | `src/main.cpp:6: '::main' must return 'int'` |

Greedy fix curve (category fixed -> clean TUs): declspec 128 -> size_t-width 210 -> ppc-intrinsic 283 -> missing-typename 333 -> implicit-conv 472 -> case-include 518 -> gnu-keyword 541 -> msl-cascade 558 -> two-phase-lookup 574 -> ref-binding 578 -> jump-over-init 582 -> overload-ambiguity 584 -> asm-function 586 -> main-signature 587 -> access-control 588 -> conflicting-redecl 589

### Categories in `compat64`

| Category | Unique locations | Error lines (all TUs) | TUs affected | Examples |
| --- | ---: | ---: | ---: | --- |
| ppc-intrinsic | 17 | 1010 | 378 | `include/JSystem/JGeometry/JGUtil.hpp:70: '__frsqrte' was not declared in this scope; did you mean '__sqrtl'?`<br>`include/JSystem/JGeometry/JGUtil.hpp:79: '__frsqrte' was not declared in this scope; did you mean '__sqrtl'?`<br>`include/MarioUtil/MathUtil.hpp:371: '__frsqrte' was not declared in this scope; did you mean '__sqrtl'?` |
| ptr-width-flags | 6 | 632 | 237 | `include/JSystem/J3D/J3DGraphBase/J3DPacket.hpp:184: right operand of shift expression '(1 << 63)' is greater than or equal to the precision 32 of the `<br>`include/JSystem/J3D/J3DGraphBase/J3DPacket.hpp:184: enumerator value for 'DIFF_FLAG' is not an integer constant`<br>`include/JSystem/J3D/J3DGraphBase/J3DMaterial.hpp:125: right operand of shift expression '(1 << 63)' is greater than or equal to the precision 32 of th` |
| libc-decl | 98 | 701 | 222 | `include/M3DUtil/MActorData.hpp:72: there are no arguments to 'snprintf' that depend on a template parameter, so a declaration of 'snprintf' must be av`<br>`include/M3DUtil/MActorData.hpp:73: there are no arguments to 'snprintf' that depend on a template parameter, so a declaration of 'snprintf' must be av`<br>`include/M3DUtil/MActorData.hpp:75: there are no arguments to 'snprintf' that depend on a template parameter, so a declaration of 'snprintf' must be av` |
| implicit-conv | 8 | 214 | 209 | `include/M3DUtil/MActorData.hpp:82: cannot convert 'J3DAnmBase**' to 'void**'`<br>`include/JSystem/JGadget/linklist.hpp:287: invalid conversion from 'const JUTConsole*' to 'JUTConsole*' [-fpermissive]`<br>`include/JSystem/JSupport/JSUList.hpp:182: cannot convert 'JSUTree<J2DPane>*' to 'const char*'` |
| ptr-truncation | 309 | 323 | 59 | `include/JSystem/JSupport.hpp:11: cast from 'const void*' to 's32' {aka 'int'} loses precision [-fpermissive]`<br>`include/JSystem/JSupport.hpp:21: cast from 'const void*' to 's32' {aka 'int'} loses precision [-fpermissive]`<br>`include/JSystem/JSupport.hpp:21: cast from 'const void*' to 's32' {aka 'int'} loses precision [-fpermissive]` |
| u32-long-spelling | 25 | 43 | 33 | `include/JSystem/JKernel/JKRArchive.hpp:121: conflicting return type specified for 'virtual s32 JKRArchive::getResSize(const void*) const'`<br>`include/JSystem/JKernel/JKRFileCache.hpp:35: conflicting return type specified for 'virtual s32 JKRFileCache::getResSize(const void*) const'`<br>`src/Camera/sunmgr.cpp:112: no declaration matches 'void TSunMgr::perform(u32, JDrama::TGraphics*)'` |
| two-phase-lookup | 5 | 19 | 17 | `include/JSystem/JGeometry/JGQuat4.hpp:62: 'set' was not declared in this scope, and no declarations were found by argument-dependent lookup at the poi`<br>`include/Camera/cameralib.hpp:86: 'CLBTwoDegreeGeneralInbetween' was not declared in this scope, and no declarations were found by argument-dependent l`<br>`include/JSystem/JGeometry/JGQuat4.hpp:49: 'set' was not declared in this scope, and no declarations were found by argument-dependent lookup at the poi` |
| ref-binding | 10 | 10 | 5 | `src/Enemy/bombhei.cpp:282: binding reference of type 'JGeometry::TVec3<float>&' to 'const JGeometry::TVec3<float>' discards qualifiers`<br>`src/Enemy/tobiPuku.cpp:898: binding reference of type 'JGeometry::TVec3<float>&' to 'const JGeometry::TVec3<float>' discards qualifiers`<br>`src/GC2D/GCConsole2.cpp:833: cannot bind non-const lvalue reference of type 'TBoundPane*&' to an rvalue of type 'TBoundPane*'` |
| jump-over-init | 9 | 9 | 4 | `src/Enemy/gesso.cpp:675: jump to case label`<br>`src/Enemy/gesso.cpp:678: jump to case label`<br>`src/JSystem/JAudio/JASystem/JASTrack.cpp:809: jump to label 'bail'` |
| host-libc-clash | 8 | 8 | 2 | `include/JSystem/JSupport/JSUStreamEnum.hpp:10: expected identifier before '(' token`<br>`include/JSystem/JSupport/JSUStreamEnum.hpp:10: expected unqualified-id before '-' token`<br>`include/JSystem/JSupport/JSUStreamEnum.hpp:10: expected ')' before '-' token` |
| asm-function | 6 | 6 | 2 | `src/JSystem/J3D/J3DGraphBase/J3DTransform.cpp:33: expected '(' before 'bool'`<br>`src/JSystem/J3D/J3DGraphBase/J3DTransform.cpp:391: expected '(' before 'void'`<br>`src/JSystem/J3D/J3DGraphBase/J3DTransform.cpp:414: expected '(' before 'void'` |
| size_t-width | 6 | 6 | 1 | `src/JSystem/JKernel/JKRHeap.cpp:255: 'operator new' takes type 'size_t' ('long unsigned int') as first parameter [-fpermissive]`<br>`src/JSystem/JKernel/JKRHeap.cpp:259: 'operator new' takes type 'size_t' ('long unsigned int') as first parameter [-fpermissive]`<br>`src/JSystem/JKernel/JKRHeap.cpp:263: 'operator new' takes type 'size_t' ('long unsigned int') as first parameter [-fpermissive]` |
| conflicting-redecl | 4 | 4 | 1 | `src/Map/MapDraw.cpp:15: ambiguating new declaration of 'int SMSGetGameRenderHeight()'`<br>`src/Map/MapDraw.cpp:16: ambiguating new declaration of 'int SMSGetGameRenderWidth()'`<br>`src/Map/MapDraw.cpp:77: 'SMSGetGameRenderWidth' was not declared in this scope` |
| explicit-msl-include | 4 | 4 | 1 | `include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/string.h:20: ambiguating new declaration of 'void* memchr(const void*, int, size_t)'`<br>`include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/string.h:22: ambiguating new declaration of 'char* strrchr(const char*, int)'`<br>`include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/string.h:23: ambiguating new declaration of 'char* strchr(const char*, int)'` |
| access-control | 2 | 2 | 1 | `src/Enemy/elecNokonoko.cpp:82: 'typedef const class TNerveBase<TLiveActor>* TSpineBase<TLiveActor>::Nerve' is private within this context`<br>`src/Enemy/elecNokonoko.cpp:91: 'typedef const class TNerveBase<TLiveActor>* TSpineBase<TLiveActor>::Nerve' is private within this context` |
| main-signature | 1 | 1 | 1 | `src/main.cpp:6: '::main' must return 'int'` |

Greedy fix curve (category fixed -> clean TUs): ppc-intrinsic 256 -> ptr-width-flags 291 -> ptr-truncation 330 -> u32-long-spelling 354 -> libc-decl 373 -> implicit-conv 555 -> two-phase-lookup 571 -> ref-binding 576 -> jump-over-init 580 -> asm-function 582 -> host-libc-clash 584 -> size_t-width 585 -> explicit-msl-include 586 -> conflicting-redecl 587 -> access-control 588 -> main-signature 589

### Categories in `p64`

| Category | Unique locations | Error lines (all TUs) | TUs affected | Examples |
| --- | ---: | ---: | ---: | --- |
| declspec | 3 | 1527 | 509 | `include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/string.h:10: expected constructor, destructor, or type conversion before '(' token`<br>`include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/string.h:11: expected constructor, destructor, or type conversion before '(' token`<br>`include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/string.h:16: expected constructor, destructor, or type conversion before '(' token` |
| ppc-intrinsic | 1 | 300 | 300 | `include/dolphin/gd/GDLight.h:78: '__cntlzw' was not declared in this scope` |
| missing-typename | 5 | 1100 | 220 | `include/PowerPC_EABI_Support/Msl/MSL_C++/MSL_Common/iterator:15: need 'typename' before 'I::difference_type' because 'I' is a dependent scope`<br>`include/PowerPC_EABI_Support/Msl/MSL_C++/MSL_Common/iterator:16: need 'typename' before 'I::value_type' because 'I' is a dependent scope`<br>`include/PowerPC_EABI_Support/Msl/MSL_C++/MSL_Common/iterator:17: need 'typename' before 'I::pointer' because 'I' is a dependent scope` |
| ptr-width-flags | 2 | 386 | 193 | `include/JSystem/J3D/J3DGraphBase/J3DPacket.hpp:184: right operand of shift expression '(1 << 63)' is greater than or equal to the precision 32 of the `<br>`include/JSystem/J3D/J3DGraphBase/J3DPacket.hpp:184: enumerator value for 'DIFF_FLAG' is not an integer constant` |
| implicit-conv | 6 | 187 | 184 | `include/M3DUtil/MActorData.hpp:82: cannot convert 'J3DAnmBase**' to 'void**'`<br>`include/JSystem/JGadget/linklist.hpp:287: invalid conversion from 'const JUTConsole*' to 'JUTConsole*' [-fpermissive]`<br>`src/Enemy/tinkoopa.cpp:1104: invalid conversion from 'const TNerveBase<TLiveActor>*' to 'TNerveBase<TLiveActor>*' [-fpermissive]` |
| ptr-truncation | 274 | 281 | 51 | `include/JSystem/JSupport.hpp:11: cast from 'const void*' to 's32' {aka 'int'} loses precision [-fpermissive]`<br>`include/JSystem/JKernel/JKRExpHeap.hpp:27: cast from 'void*' to 'u32' {aka 'unsigned int'} loses precision [-fpermissive]`<br>`include/JSystem/JSupport.hpp:21: cast from 'const void*' to 's32' {aka 'int'} loses precision [-fpermissive]` |
| case-include | 6 | 46 | 46 | `include/JSystem/J3D/J3DGraphBase/J3DMaterial.hpp:7: JSystem/J3d/J3DGraphBase/Blocks/J3DTevBlocks.hpp: No such file or directory`<br>`src/Enemy/beam.cpp:1: Enemy/beam.hpp: No such file or directory`<br>`src/Enemy/hamukuri.cpp:26: MSound/MSoundSe.hpp: No such file or directory` |
| u32-long-spelling | 25 | 43 | 33 | `include/JSystem/JKernel/JKRArchive.hpp:121: conflicting return type specified for 'virtual s32 JKRArchive::getResSize(const void*) const'`<br>`include/JSystem/JKernel/JKRFileCache.hpp:35: conflicting return type specified for 'virtual s32 JKRFileCache::getResSize(const void*) const'`<br>`src/Camera/sunmgr.cpp:112: no declaration matches 'void TSunMgr::perform(u32, JDrama::TGraphics*)'` |
| gnu-keyword | 13 | 57 | 23 | `include/Strategic/spcinterp.hpp:98: expected primary-expression before ')' token`<br>`include/Strategic/spcinterp.hpp:98: expected unqualified-id before '{' token`<br>`src/Strategic/spcinterp.cpp:100: expected unqualified-id before 'typeof'` |
| msl-cascade | 31 | 31 | 17 | `src/GC2D/MovieSubtitle.cpp:92: 'memset' was not declared in this scope`<br>`src/JSystem/JDrama/JDRRenderMode.cpp:88: 'memcpy' was not declared in this scope`<br>`src/JSystem/JDrama/JDRRenderMode.cpp:93: 'memcpy' was not declared in this scope` |
| two-phase-lookup | 5 | 19 | 17 | `include/JSystem/JGeometry/JGQuat4.hpp:62: 'set' was not declared in this scope, and no declarations were found by argument-dependent lookup at the poi`<br>`include/Camera/cameralib.hpp:86: 'CLBTwoDegreeGeneralInbetween' was not declared in this scope, and no declarations were found by argument-dependent l`<br>`include/JSystem/JGeometry/JGQuat4.hpp:49: 'set' was not declared in this scope, and no declarations were found by argument-dependent lookup at the poi` |
| ref-binding | 10 | 10 | 5 | `src/Enemy/bombhei.cpp:282: binding reference of type 'JGeometry::TVec3<float>&' to 'const JGeometry::TVec3<float>' discards qualifiers`<br>`src/Enemy/tobiPuku.cpp:898: binding reference of type 'JGeometry::TVec3<float>&' to 'const JGeometry::TVec3<float>' discards qualifiers`<br>`src/GC2D/GCConsole2.cpp:833: cannot bind non-const lvalue reference of type 'TBoundPane*&' to an rvalue of type 'TBoundPane*'` |
| jump-over-init | 9 | 9 | 4 | `src/Enemy/gesso.cpp:675: jump to case label`<br>`src/Enemy/gesso.cpp:678: jump to case label`<br>`src/JSystem/JAudio/JASystem/JASTrack.cpp:809: jump to label 'bail'` |
| asm-function | 6 | 6 | 2 | `src/JSystem/J3D/J3DGraphBase/J3DTransform.cpp:33: expected '(' before 'bool'`<br>`src/JSystem/J3D/J3DGraphBase/J3DTransform.cpp:391: expected '(' before 'void'`<br>`src/JSystem/J3D/J3DGraphBase/J3DTransform.cpp:414: expected '(' before 'void'` |
| size_t-width | 6 | 6 | 1 | `src/JSystem/JKernel/JKRHeap.cpp:255: 'operator new' takes type 'size_t' ('long unsigned int') as first parameter [-fpermissive]`<br>`src/JSystem/JKernel/JKRHeap.cpp:259: 'operator new' takes type 'size_t' ('long unsigned int') as first parameter [-fpermissive]`<br>`src/JSystem/JKernel/JKRHeap.cpp:263: 'operator new' takes type 'size_t' ('long unsigned int') as first parameter [-fpermissive]` |
| conflicting-redecl | 4 | 4 | 1 | `src/Map/MapDraw.cpp:15: ambiguating new declaration of 'int SMSGetGameRenderHeight()'`<br>`src/Map/MapDraw.cpp:16: ambiguating new declaration of 'int SMSGetGameRenderWidth()'`<br>`src/Map/MapDraw.cpp:77: 'SMSGetGameRenderWidth' was not declared in this scope` |
| access-control | 2 | 2 | 1 | `src/Enemy/elecNokonoko.cpp:82: 'typedef const class TNerveBase<TLiveActor>* TSpineBase<TLiveActor>::Nerve' is private within this context`<br>`src/Enemy/elecNokonoko.cpp:91: 'typedef const class TNerveBase<TLiveActor>* TSpineBase<TLiveActor>::Nerve' is private within this context` |
| main-signature | 1 | 1 | 1 | `src/main.cpp:6: '::main' must return 'int'` |

Greedy fix curve (category fixed -> clean TUs): declspec 174 -> ppc-intrinsic 223 -> case-include 269 -> missing-typename 304 -> ptr-truncation 334 -> ptr-width-flags 357 -> implicit-conv 493 -> u32-long-spelling 519 -> gnu-keyword 542 -> msl-cascade 559 -> two-phase-lookup 574 -> ref-binding 579 -> jump-over-init 583 -> asm-function 585 -> size_t-width 586 -> main-signature 587 -> access-control 588 -> conflicting-redecl 589

### Categories in `host64`

| Category | Unique locations | Error lines (all TUs) | TUs affected | Examples |
| --- | ---: | ---: | ---: | --- |
| ppc-intrinsic | 18 | 1214 | 367 | `include/JSystem/JGeometry/JGUtil.hpp:70: '__frsqrte' was not declared in this scope; did you mean '__sqrtl'?`<br>`include/JSystem/JGeometry/JGUtil.hpp:79: '__frsqrte' was not declared in this scope; did you mean '__sqrtl'?`<br>`include/dolphin/gd/GDLight.h:78: '__cntlzw' was not declared in this scope` |
| implicit-conv | 2 | 183 | 182 | `include/M3DUtil/MActorData.hpp:82: cannot convert 'J3DAnmBase**' to 'void**'`<br>`include/JSystem/JSupport/JSUList.hpp:182: cannot convert 'JSUTree<J2DPane>*' to 'const char*'` |
| case-include | 6 | 46 | 46 | `include/JSystem/J3D/J3DGraphBase/J3DMaterial.hpp:7: JSystem/J3d/J3DGraphBase/Blocks/J3DTevBlocks.hpp: No such file or directory`<br>`src/Enemy/beam.cpp:1: Enemy/beam.hpp: No such file or directory`<br>`src/Enemy/hamukuri.cpp:26: MSound/MSoundSe.hpp: No such file or directory` |
| libc-decl | 81 | 81 | 38 | `include/Enemy/KoopaNerve.hpp:17: 'fmodf' is not a member of 'std'; did you mean 'modf'?`<br>`include/M3DUtil/MActorData.hpp:72: 'snprintf' was not declared in this scope`<br>`include/M3DUtil/MActorData.hpp:73: 'snprintf' was not declared in this scope, and no declarations were found by argument-dependent lookup at the point` |
| gnu-keyword | 13 | 57 | 23 | `include/Strategic/spcinterp.hpp:98: expected primary-expression before ')' token`<br>`include/Strategic/spcinterp.hpp:98: expected unqualified-id before '{' token`<br>`src/Strategic/spcinterp.cpp:100: expected unqualified-id before 'typeof'` |
| two-phase-lookup | 4 | 16 | 15 | `include/JSystem/JGeometry/JGQuat4.hpp:62: 'set' was not declared in this scope, and no declarations were found by argument-dependent lookup at the poi`<br>`include/Camera/cameralib.hpp:86: 'CLBTwoDegreeGeneralInbetween' was not declared in this scope, and no declarations were found by argument-dependent l`<br>`include/JSystem/JGeometry/JGQuat4.hpp:49: 'set' was not declared in this scope, and no declarations were found by argument-dependent lookup at the poi` |
| jump-over-init | 9 | 9 | 4 | `src/Enemy/gesso.cpp:675: jump to case label`<br>`src/Enemy/gesso.cpp:678: jump to case label`<br>`src/JSystem/JAudio/JASystem/JASTrack.cpp:809: jump to label 'bail'` |
| ref-binding | 8 | 8 | 4 | `src/Enemy/bombhei.cpp:282: binding reference of type 'JGeometry::TVec3<float>&' to 'const JGeometry::TVec3<float>' discards qualifiers`<br>`src/Enemy/tobiPuku.cpp:898: binding reference of type 'JGeometry::TVec3<float>&' to 'const JGeometry::TVec3<float>' discards qualifiers`<br>`src/GC2D/GCConsole2.cpp:833: cannot bind non-const lvalue reference of type 'TBoundPane*&' to an rvalue of type 'TBoundPane*'` |
| host-libc-clash | 8 | 8 | 2 | `include/JSystem/JSupport/JSUStreamEnum.hpp:10: expected identifier before '(' token`<br>`include/JSystem/JSupport/JSUStreamEnum.hpp:10: expected unqualified-id before '-' token`<br>`include/JSystem/JSupport/JSUStreamEnum.hpp:10: expected ')' before '-' token` |
| asm-function | 6 | 6 | 2 | `src/JSystem/J3D/J3DGraphBase/J3DTransform.cpp:33: expected '(' before 'bool'`<br>`src/JSystem/J3D/J3DGraphBase/J3DTransform.cpp:391: expected '(' before 'void'`<br>`src/JSystem/J3D/J3DGraphBase/J3DTransform.cpp:414: expected '(' before 'void'` |
| overload-ambiguity | 2 | 2 | 2 | `src/JSystem/JKernel/JKRDvdArchive.cpp:12: call of overloaded 'JKRArchive(int, JKRArchive::EMountMode)' is ambiguous`<br>`src/JSystem/JKernel/JKRMemArchive.cpp:41: call of overloaded 'JKRArchive(int, JKRArchive::EMountMode)' is ambiguous` |
| explicit-msl-include | 7 | 7 | 1 | `include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/string.h:10: expected constructor, destructor, or type conversion before '(' token`<br>`include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/string.h:11: expected constructor, destructor, or type conversion before '(' token`<br>`include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/string.h:16: expected constructor, destructor, or type conversion before '(' token` |
| conflicting-redecl | 4 | 4 | 1 | `src/Map/MapDraw.cpp:15: ambiguating new declaration of 'int SMSGetGameRenderHeight()'`<br>`src/Map/MapDraw.cpp:16: ambiguating new declaration of 'int SMSGetGameRenderWidth()'`<br>`src/Map/MapDraw.cpp:77: 'SMSGetGameRenderWidth' was not declared in this scope` |
| access-control | 2 | 2 | 1 | `src/Enemy/elecNokonoko.cpp:82: 'typedef const class TNerveBase<TLiveActor>* TSpineBase<TLiveActor>::Nerve' is private within this context`<br>`src/Enemy/elecNokonoko.cpp:91: 'typedef const class TNerveBase<TLiveActor>* TSpineBase<TLiveActor>::Nerve' is private within this context` |
| main-signature | 1 | 1 | 1 | `src/main.cpp:6: '::main' must return 'int'` |

Greedy fix curve (category fixed -> clean TUs): ppc-intrinsic 329 -> implicit-conv 456 -> case-include 502 -> libc-decl 533 -> gnu-keyword 556 -> two-phase-lookup 571 -> ref-binding 575 -> jump-over-init 579 -> overload-ambiguity 581 -> asm-function 583 -> host-libc-clash 585 -> explicit-msl-include 586 -> main-signature 587 -> access-control 588 -> conflicting-redecl 589
