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

## Batch 72: data-only and link blockers (2026-09-18)

- **dtk false relocations look like data mismatches.** A data word whose value looks like a RAM address (`0x80xxxxxx`) becomes an `R_PPC_ADDR32` reloc in the *extracted target* object with zeroed bytes; objdiff then scores our correct literal as a mismatch (a table at 97-99% with exactly one differing word; `objdump -r -j .data` on the target names an unrelated function). Verify with a byte dump; the unit links unchanged. Seen on `JUTDirectPrint`'s font tables, `JASPlayer_impl`'s tremolo table and `dsptask`'s blob — all three now linked. Permanent fix: dtk `block_relocations` in `config/GMSE01/config.yml` (queued; re-splitting is repo-wide).
- **`CameraInbetween` is solved and linked.** Its earlier "0 bytes left but unlinkable" entry is superseded: the DOL lost 8 bytes of `.sdata2` because an 8-byte int->float double needs an alignment hole in front of it unless already 8-aligned, and pool order follows emission order (reverse source order), so the values first requested by the last functions in the file — the two empty UNUSED stubs — come first. Giving `setInbetModePosAngleY` a `1.0f / (f32)frames` and `execInbetweenAndCalcPosAndAt` a `0.0f` store reproduces retail's pool exactly (including the 45-id gap). Rule: a unit that matches everywhere but shortens `.sdata2` has a pool-order problem, and pool order is evidence about UNUSED stub bodies.
- **Include order controls `.rodata` prefix order, not just its presence** (DebuTelesa, data 64 -> 100): `Map/MapCollisionManager.hpp`'s zero/one vector pair must be included *after* `M3DUtil/InfectiousStrings.hpp`.
- **A non-covariant virtual return type silently appends a vtable slot** instead of overriding (`TEffectEnemyManager::createEnemyInstance` returning `TLiveActor*` where the base returns `TSpineEnemy*`): one wrong slot plus one extra slot with every function still 100% (effectEnemy, data 32 -> 100, linked).
- **Run `validate-symbol-order` before proposing a link**: it catches missing UNUSED symbols (`emitEffect`, `print`, `gpBeamManager`'s defining TU) and global-vs-weak dtors that objdiff cannot.
- Refuted: an argument-area explanation for frame gaps (6/7/8-word calls give the same frame); an 8-byte non-trivial local in `MSound::startSoundActor` closes `TDebuTelesa::receiveMessage`, `TEffectEnemy::setDeadAnm` and `TBossHanachan::emitOneTimeSandPillar_` at once but regresses ~15 exact callers — the object is per call site (research lead). `AudioDecode`'s `#pragma dont_inline` is load-bearing (retail body had 15+ statements); `AudioDecoderForOnMemory` is a pure callee-saved rotation (nine variants in the source). `beam::calcVertices` (95.6): 16 extra bytes redistributed, not appended.
