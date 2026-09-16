# Source linking

An object can match in code and data and still break the DOL when linked from source.
Promotion needs the full DOL SHA-1 check, not just `changes_all`.

## Weak emission order

`validate-symbol-order.py` downgrades weak disorder to a warning, so a unit with only warnings can still be unlinkable.
When a matching object perturbs the link, compare where its weak symbols are emitted against the map before suspecting code.

- **`Map/PollutionEvent`**: four event classes had no virtual members, so the compiler flushed its weak queue early and emitted `TPollutionTest::__dt` and `perform` before `loadAfter`. The map gives each a 0xa0-0xa4 vtable and virtual destructor; declaring the destructors moved the weak pair after `loadAfter` and it linked.
- **`GC2D/MessageUtil`**: `isNotDrained()` emitted `JSUMemoryInputStream::getPosition`/`getLength` between the two functions. The retail copies come from `MarioPositionObj.cpp`; picking ours shifted `SMSMakeTextBuffer` by 16 bytes. Calling inherited `getAvailable() != 0` gives the same instructions without emitting them.
- Placement follows source order reversed: a symbol that must come first goes last in the file.

## Duplicate globals: archive linking

The map lists unreferenced duplicate `cSunVolumeName`/`cSunsetVolumeName` in `MarNameRefGen_Map`.
Loose objects reject them; one archived object lands too late.
Archiving the complete ordered input list with MWLD reproduces retail handling. GMSE01 enables that build path.

## US `.rodata` boundaries

Application's two disc-error strings are 121/106 bytes in the US build, not the imported 124/109.
Seventeen later Application strings shift 4 bytes and section alignment moves the next `.rodata` boundary 8 bytes earlier, so every downstream symbol follows (1,273 shifted, 6,502 unchanged).
Corrected in batch 35; `MarDirectorCreateObjects` and `TargetArrow` then linked.
Correct only documented regional ranges. Do not repeat the boundary-only relink trial from batch 6, which left the linker producing no executable.

## `Camera/CameraInbetween` (unlinkable, open)

Matches 100% but shifts the DOL.
- DOL `.sdata2` (section 14, base 0x8040eba0) is 0x8c40 in ours vs 0x8c60: exactly 0x20 short, the TU's whole literal block.
- Ours emits 7 local literals; the original has 5 plus a dtk gap, and the map lists UNUSED `@1758`/`@1759`.
- The two extras are `0.5f`/`-0.5f` from `CLBRoundf<s16>` (`Camera/cameralib.hpp`), inlined through `CLBChaseConstantSpecifyFrame<s16>`. `CameraBGCheck.o` is the only extracted object defining `CLBRoundf<s>__Ff`, so retail dedup orphaned this TU's copies.
- `CameraBGCheck.cpp` is already before `CameraInbetween.cpp` in `configure.py`.
- `validate-symbol-order` passes, apart from two UNUSED stub size warnings (`setInbetModePosAngleY` 0xbc, `execInbetweenAndCalcPosAndAt` 0xec). The stubs are not the cause.
- `.sdata2` `d` vs `r` (writable vs readonly) is a red herring; `ansi_fp.c` has it and links.
- Unexplained: the DOL loses 0x20, not the 8 bytes the orphaned floats account for. Next step: inspect where our `.sdata2` block lands in the map/ELF.

## Other notes

- Identical caller instructions do not prove identical whole-file emission; inspect extra weak definitions when a promoted object shifts later code.
- A full executable match does not validate bodies in objects still linked from the original.
- Count already-matching unlinked library objects by absence from `config/GMSE01/objects.json`, not `complete_code_percent` (a zero-code unit reports 100 vacuously).
