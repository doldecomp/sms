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
- **Header round c-pathnode, `TPathNode(THitActor*) : unk0(actor), unk4(0.0f, 0.0f, 0.0f)`, not landed.**
  Tree-wide: TStayPakkun::load 80.1 -> 100, BeeHive Break/Attack/MarioWaterIn ~94-95 -> ~99, but ~20 functions that reach the ctor through `setGoalPathMario()` then `bl` the zero set at depth 4 where retail expands it (TPakkun::load 100 -> 78, TNerveBombHeiAttack 100 -> 81, TNervePopoWait 100 -> 81); total fuzzy 99.20 -> 99.18.
  Retail gives TPakkun::load and TStayPakkun::load identical slots (node 0x18, frame 0x30) with only the zero set's depth differing.
  Every honest depth-1 spelling at TPakkun::load (direct conversion, named `mario`, temporary, named node, copy-init) expands the zeros but leaves the node 4 low; a `const TPathNode&` binding costs 8 bytes of frame.
  Delegating TStayPakkun::load to TPakkun::load (auto-inlined) puts the whole ctor out of line (45%).
  Details in the PathNode.hpp comment.
- **Header round c-qmul, no-locals `TQuat4::mul(a, b)` (BeeHive's `mulQuat`), landed in c-qmul2.**
  Tree-wide +14 functions, none worse (doAttackPose +5.23, moveCoaster +3.74, flyAroundMario +3.62, decideTargetAtDir +3.48, makeInitialVelocity -> 99.88, SMS_Eular2Quat 91.88 -> 96.97); BeeHive stays 100 without its fork, and `TYumbo::shotSeeds` needs `roll.mul(pitch); roll.rotate(dir, dir);` (95.74).
  The former blocker was the in-place `SMS_Eular2Quat` (`qx.mul(qx, qz); qy.mul(qy, qx); return qy;`, 97.0) being auto-inlined into `TAnimalBase::execWalk` (89.89 -> 50.1).
  Naming the angles as `f32` locals, each just before its quaternion (`f32 z = K * rot.z; TQuat4 qz; qz.setEulerZ(z);` and so on for y and x), keeps the same 97.0 body and stays a call; two of the three already suffice, one does not.
  Other spellings that raise the cost enough to stay a call change the body: a `TVec3` of angles (82.5-82.6, or 76.5 via `scale`), an extra copied quaternion (62.4); a third named product `qxz` is still inlined.
  Moving the definition below `execWalk` changes nothing (deferred inlining).
  The one-argument `mul` in the same form adds makeQuat +4.03 and updatePosture +1.0, but shotSeeds then stays at 93.5 under every spelling tried.
- **Hand-written GX FIFO writers want the `GXVert.h` inline level.** `ShapePacketCallBackFunc` (PacketUtil, linked) closed 91.7 -> 100 when its FIFO helpers wrote through `GXCmd1u8`/`GXParam1u16`/`GXParam1u32` instead of assigning `GXWGFifo` directly: each missing inline level was retail's 8 bytes per helper expansion, and the r0/r3 swap went with it. Read an 8-per-expansion frame gap in GX-writing code as this first.
