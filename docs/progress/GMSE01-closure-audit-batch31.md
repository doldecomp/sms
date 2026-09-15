# Priority 1 closure audit — batch 31

Baseline: `915877d3`, local GMSE01 revision 0 configuration.
The previous checkpoint supplied retained parser work and closure evidence; continue the plan's file-completion queue.
No source-linked file is added in this checkpoint.

## Map/MapCollisionEntry — three functions exact, one remaining

Recovered the missing 116-byte UNUSED TMapCollisionBase constructor definition by moving its existing header body into the source file.
Place it after base init and before static setUp, following the original map's reversed emission order.
The three derived constructors remain exactly matching.
The whole-map and source scans found no direct construction of the base outside this file; the full build checks all header consumers.
All 22 map symbols now pass presence, order and linkage, and both UNUSED bodies have the recorded sizes.

The translation-only flag uses the same normalized boolean instruction group in move and warp setUp.
Naming that predicate restores move's 0x20 frame and fixes the inlined vector slot in moveSRT at 0x24.
Both runtime functions now match exactly: 164 and 236 bytes.
The same predicate in warp setUp restores its 0x38 frame; declaring the vector before the predicate and initializing it with set restores its original 0x20 slot.
All 208 bytes of warp setUp now match.
These changes preserve the original instruction sequences without padding or invented helpers.

The only remaining nonmatching runtime function is the 168-byte move initializer taking a count, background type, data and actor.
All 42 instructions match except frame/save offsets: current 0x30 frame versus original 0x38.
The file has 2,656 runtime code bytes, with 2,488 exact; it remains unlinked.
Do not promote solely because its data and other functions match.

Rejected trials:

- Using the existing collision-data instance accessor in the initializer does not affect code generation.
- A named manager pointer reverses the count and manager load order without fixing the frame.
- Naming the allocation count or using a u32 loop counter has no effect.
- Naming warp's initial needs-setup predicate has no effect on its vector slot.
- Changing only vector construction to set has no effect; the declaration must also precede the translation-only predicate.

## MSound/MSModBgm — unchanged

Full original/current diffs read for modBgm, xFadeBgm and xFadeBgmForce.
modBgm has two identical extra zero loads when clearing a sound handle and the activity byte.
Changing false to integer zero, changing the byte field from bool to u8, combining those changes, or swapping clear order does not remove them.
Returning nullptr directly adds control-flow differences and is worse.
All source/header trials reverted.

xFadeBgm retains a swapped pair of floating-point registers and a 0x30 frame versus 0x38.
xFadeBgmForce retains a 0x28 frame versus 0x30 with an otherwise exact instruction sequence.
All eight map symbols pass presence/order/linkage, but UNUSED getTiming is 96 bytes versus the original 148.
Its optional u32 output parameter is currently ignored; the only located runtime call supplies nullptr.
The missing non-null behavior cannot be established from that call alone; do not invent output values merely to reach the map size.

## Map/PollutionManager — inspected, unchanged

Read the file and full cleanedAll diff.
cleanedAll has the original unrolled instruction sequence but different registers and a 0x38 frame versus 0x30.
It also has two unfinished UNUSED bodies: subtractFromYMap is four bytes versus 164, and isProhibit is four bytes versus 168.
All 31 map symbols are present with correct linkage; the validator warns about weak-symbol order as well as those body sizes.
No implementation trials or promotion.

## Verification and continuation

Full build, required baseline/changes_all, all 12,904 function-presence/score comparisons, DOL byte comparison and expected SHA-1 pass.
Zero regressions; three newly exact functions / 608 runtime code bytes.
MapCollisionEntry passes the map check without warnings, including the newly emitted base constructor.
No middleware changes or gameplay test.

Source linking remains 76 game files / 80,800 code bytes.
Game code: 24.86575% matched / 2.8374174% source-linked.
Aggregate: 38.48049% matched / 2.24211% source-linked.
The regenerated queue contains 104 authorized small game files / 94,780 unmatched bytes.
MapCollisionEntry moves into the single-function queue with 168 bytes remaining; its initializer requires new evidence before further frame trials.
Continue the ordered queue, consulting the prior audits for deferred files.
Further small multi-function candidates include CameraSecureView and sunmgr.
Measurements: `GMSE01-batch31.json`; local logs: `build/GMSE01-*-batch31.*`.
