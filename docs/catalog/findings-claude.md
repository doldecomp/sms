# Claude findings

Record binary-backed findings from Claude unit work here before promoting reusable rules into the topic catalog.

## 2026-09-23 (session 5eae6404)

- **Pairs of opposite frame levers close slot residues that singles never do.** Each change alone moves the frame the wrong way (+8 or -8); together they land retail. Closed: MSL `powf` (direct return from `__log2f` + a named `frac2` between two locals), chuuhana `TNerveChuuHanaForceJumped::execute` (named `int index = range.rand();` + a direct-return `ChuuHanaSafeNode`), bosstelesa `TTelesaSlot::moveObject` (raw `mOwner` + the sibling's `getSaveParam2()` binder), seal `init` to 99.99. Pairs do not reach pure register tie-breaks.
- **A `TMsRange` local 4 bytes off is usually an unnamed `rand()` result.** Naming one (`f32 speed = range.rand();`) moved both ranges to retail's slots (`TBossTelesa::rouletteStart`; chuuhana above).
- **FPR mirrors come from component store order, not from the expression.** Retail stores cross-product and copied vectors z, y, x in JPADrawVisitor's directional draws; the four functions went 99.2 -> 99.7-99.97.
- **Sibling spelling transfers.** JPADrawVisitor: the unit's own direct-read angle helper, `Mtx` declared first and x0,y0,x1,y1 corner order closed five functions once copied from the exact `JPADrawExecRotationCross`.
- **A large codeless frame gap can be inline parameter slots.** `drawShadowGD` gained its full 0x270 by drawing its cubes through a helper taking named `TVec3 min/max`, as the sibling `drawShadow` does with `SMS_DrawCube`.
- Refuted (docs/catalog/frame-gaps.md): other compiler builds and return-value flags (rvo1), shared header inlines as the dead-low-region cause (fd1), unit-level compile flags (uf1).
- **Header round c-hdr1 (MsGetRotFromZaxisY, TQuat4 rotate), measured tree-wide with `ninja changes_all`.**
  Landed: `TQuat4::rotateQ` reads the quaternion's members with no `* 0` terms (fireWanwan's fork); execWalk 86.78 -> 89.89, fireWanwan drops its rotate fork, nothing regresses.
  Not landed, MsGetRotFromZaxisY with `return axis.x >= 0.0f ? 90.0f : -90.0f;`: no gain outside fireWanwan; MsIsInSight 100 -> 99.95, TBGKMtxCalc::calc 100 -> 99.60, walkToCurPathNode -0.25. Nested-if variants do not inline in fireWanwan; dropping `theta` costs ~40 functions.
  Not landed, two-argument rotate as a one-level member-read body: Kumokun weak rotate 90.39 -> 100, rotateGoalDirToLocal -> 100, +0.02..+2.32 on nine more sites; costs doAttackPose -3.25, makeQuat -2.21, fireWanwan bindBody -1.14. As a two-level forwarder it inlines at Kumokun's Wait nerve and the weak copy vanishes.
  Removing Bird's rotate fork for the now-identical `rotateInPlace` moves doLanding 93.40 -> 92.38 with doLanding's own text unchanged.
- **Header round c-hdr2, one-level member-read two-argument rotate, not landed.**
  Re-measured tree-wide: gains as in c-hdr1 plus Bird doLanding +0.22; losses doAttackPose -2.73, makeQuat -2.21, bindBody -1.14.
  Body variants with a q2 temporary or local copies of v or the quaternion all drop Kumokun's weak rotate below 100, so the member-read body stands; the old locals are what give bindBody retail's 0x1e8 frame and f5/f6 colouring.
  No honest call-site spelling recovers the three losers (details in the JGQuat4.hpp TODO); the blocker is regalloc and frame, not the call form.
