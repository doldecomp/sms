# Priority 1 closure audit — batch 38

Baseline: `d4900785`, GMSE01 revision 0.
The preceding checkpoint completed and source-linked MarNameRefGen_Map; it was verified progress.
This checkpoint completes MapCollisionPlane, adding 1,348 source-linked code bytes and 220 data bytes.
Its last nonmatching function, checkPlaneGround, contributes 308 newly exact code bytes.

## MapCollisionPlane — completed

Reviewed the complete 308-byte instruction diff and the batch 26 conversion warning.
The existing worldToGrid returns a fractional coordinate.
MapObjPlane::depress requires that fractional result, while checkPlaneGround converts it into a discrete cell index.
Changing the shared return type would conflate these two uses.

Add a separate worldToGridIndex helper for the collision lookup and mark its name fabricated.
Its body computes mOneOverScale * (v + mExtent) into a named int and returns that value.
Using it for both X and Z reproduces the original integer conversion reuse, removing an extra spill/reload pair and matching the original registers.
The direct arithmetic and named integer result also recover the original 0x88 frame and every conversion slot.
Keep the existing fractional helper and all MapObjPlane callers unchanged.

Rejected intermediate forms:

- Changing the caller's tile locals from int to s32 has no effect.
- An integer helper returning worldToGrid(v) directly fixes instructions/registers but leaves frame 0x80 instead of 0x88.
- Naming a float coordinate inside that helper restores the unwanted spill/reload pair and expands the frame to 0x98.
- Naming an int result of worldToGrid(v) retains the correct instructions but gives frame 0x90.
  Separating its declaration from assignment has no effect.
- Computing the expression directly into the named int gives the exact result.

All five map-listed functions pass presence, order and linkage; there are no UNUSED functions.
All code and data sections match.
The promoted source object passes full executable byte comparison and SHA-1 through the ordered archive build.

## NpcInbetween — helper trials reverted

A position-blend ratio accessor changes the interpolation factor from f2 to the original f1 and preserves all 55 instructions.
However, it expands frame 0x20 to 0x28 and shifts the conversion slots eight bytes upward.
Splitting the caller's local declaration/assignment, making that local const, and removing const from the accessor do not resolve the frame.
Restore the original source and header rather than retain another partial helper.
This is new inline-context evidence; do not repeat the previously rejected reciprocal arithmetic trials.

## Validation and queue

The required baseline equals the batch 37 snapshot.
Full build, changes_all, all 12,904 function comparisons, map validation, DOL byte comparison and expected SHA-1 pass.
Zero missing functions, zero similarity regressions and zero matched-data regressions.
Only checkPlaneGround changes function similarity; all MapObjPlane functions remain unchanged.
No gameplay test was performed.

Source linking: 86 game files / 92,672 code bytes.
Game: 25.033045% matched / 3.254321% source-linked.
Aggregate: 38.612686% matched / 2.571545% source-linked.
Regenerated queue: 96 authorized game files with 1..2,000 unmatched bytes, totaling 90,016 bytes.
CameraInbetween and PollutionEvent retain their documented completion blockers.
Continue the plan's whole-file priorities, preserving separate integer/fractional conversion uses in any related work.
Measurements: GMSE01-batch38.json; verification: build/GMSE01-verification-batch38.json.
