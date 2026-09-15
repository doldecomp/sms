# Priority 1 closure audit — batch 39

Baseline: `562ed4e0`, GMSE01 revision 0.
Reread DECOMPILATION_PLAN.md and preserve its whole-file priorities.
No file is completed or promoted in this checkpoint.
The two retained corrections improve instruction similarity but add no exact or source-linked bytes.

## NpcCallback — retained corrections, still incomplete

Reviewed the full 1,064-byte callback diff.
Use checkLiveFlag with the explicit conditional bool expression to remove extra flag-normalization branches.
Keep Mario's adjusted Y coordinate in a float local across the squared-range call, reproducing the original register-held value rather than reloading it from the vector.
Similarity improves from 92.890976% to 96.04511%.
The remaining differences include the rotation-vector copy, neck initialization/register allocation, and stack layout.

Rejected trials, all reverted:

- Naming both rotation results and an explicit subtraction vector introduced extra copies and scalar subtraction.
- Binding references to both returned rotation vectors recovered the integer copy and subtraction call but kept an extra address register live across the second call; stack locations still differed.
- Making MsGetRotFromZaxis return a const vector did not change this callback's generated instructions.
  Both MathUtil declaration and definition were restored.
  Its caller inventory includes NpcWalkTurn, bossgesso, bgtentacle, bgpoldrop, graph, lensflare, AnimalBase and this callback; the separate yaw-only helper is not the same API.

The protected vector header's existing by-value subtraction operator returns a reference to its parameter.
Do not change that library autonomously or assume it proves the missing-copy cause.
The callback's four map functions remain present; the existing weak ordering warning remains.

## AnimalNerve — correct random bounds, still incomplete

Reviewed all 334 instructions of TNerveAnimalGraphWander::execute.
Two timer initialization sites passed MsRandI(hi, lo), while the original computes the positive range hi - lo and adds lo.
Change both to MsRandI(lo, hi), matching the two already-correct sites in this function.
Similarity improves from 99.532936% to 99.62276%.
The original frame is 0xE8; the retained source still emits 0x118 and has different early register allocation.
All six map functions pass presence, order and linkage, with no UNUSED functions.

Rejected trials, all reverted:

- Replacing calcDist with local vectors and length() inlines square root when the original calls it; frame becomes 0x108 and similarity falls to about 91.3%.
- Passing the distance helper's first vector by value retains the original instructions but leaves frame 0x110 and vector slots 0x24 too high.
- A shared timer-reset helper restores the other-actor register but leaves frame 0x110 and the count/index register exchange.
- Removing the point-reference and save-parameter locals does not resolve those differences.

## MapObjPollution — new accessor trials reverted

The previous dimension-getter trial is documented in batch 25.
This pass additionally tests accessors for stamp texture and interval; together they recover the original argument load order but leave registers and frame different (0x48 versus 0x60).
Using getPos().getWidth()/getHeight() plus a named polluter pointer reaches frame 0x60 but retains the wrong saved-register ordering, loop registers and layer-load registers.
Restore the source and header completely; no accessor is retained.

## Other inspected candidates and next work

CameraInbetween and PollutionEvent retain their previously documented missing/dead-body and emission/data-layout blockers.
SampleCtrlMaterial was inspected without a new closure hypothesis; no changes retained.
BathtubBinder's empty source and map were inspected, but no reconstruction was started.
It has five class runtime methods, one local vector helper and an UNUSED constrain_ method; a future reconstruction must finish the scaffolding/map workflow and use m2c.
Keep such empty-file work behind the near-complete queue.

Continue with the plan's single-function queue after checking these deferrals; spider and beam remain candidates with substantive instruction differences.
Do not repeat the frame-only trials above without new evidence.

## Validation

The saved baseline equals the batch 38 snapshot.
Full build, changes_all, all 12,904 function comparisons, changed-unit map checks, DOL byte comparison and expected SHA-1 pass.
Zero missing functions, zero similarity regressions and zero matched-data regressions.
Only the two functions above change similarity.
Both changed objects remain nonmatching and are still linked from original binary objects; the DOL match does not establish their source correctness.
No gameplay test was performed.

Source linking remains 86 game files / 92,672 code bytes.
Game: 25.033045% matched / 3.254321% source-linked.
Aggregate: 38.612686% matched / 2.571545% source-linked.
Regenerated queue: 96 authorized game files with 1..2,000 unmatched bytes, totaling 90,016 bytes.
Measurements: GMSE01-batch39.json; verification: build/GMSE01-verification-batch39.json.
