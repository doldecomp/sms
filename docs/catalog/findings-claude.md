# Claude findings

Record binary-backed findings from Claude unit work here before promoting reusable rules into the topic catalog.

## 2026-09-23 (session 5eae6404)

- **Pairs of opposite frame levers close slot residues that singles never do.** Each change alone moves the frame the wrong way (+8 or -8); together they land retail. Closed: MSL `powf` (direct return from `__log2f` + a named `frac2` between two locals), chuuhana `TNerveChuuHanaForceJumped::execute` (named `int index = range.rand();` + a direct-return `ChuuHanaSafeNode`), bosstelesa `TTelesaSlot::moveObject` (raw `mOwner` + the sibling's `getSaveParam2()` binder), seal `init` to 99.99. Pairs do not reach pure register tie-breaks.
- **A `TMsRange` local 4 bytes off is usually an unnamed `rand()` result.** Naming one (`f32 speed = range.rand();`) moved both ranges to retail's slots (`TBossTelesa::rouletteStart`; chuuhana above).
- **FPR mirrors come from component store order, not from the expression.** Retail stores cross-product and copied vectors z, y, x in JPADrawVisitor's directional draws; the four functions went 99.2 -> 99.7-99.97.
- **Sibling spelling transfers.** JPADrawVisitor: the unit's own direct-read angle helper, `Mtx` declared first and x0,y0,x1,y1 corner order closed five functions once copied from the exact `JPADrawExecRotationCross`.
- **A large codeless frame gap can be inline parameter slots.** `drawShadowGD` gained its full 0x270 by drawing its cubes through a helper taking named `TVec3 min/max`, as the sibling `drawShadow` does with `SMS_DrawCube`.
- Refuted (docs/catalog/frame-gaps.md): other compiler builds and return-value flags (rvo1), shared header inlines as the dead-low-region cause (fd1), unit-level compile flags (uf1).
