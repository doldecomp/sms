#include <JSystem/JDrama/JDRFrmGXSet.hpp>

using namespace JDrama;

// TODO: 96.8%. All 137 instructions match; the residue is frame 0xb8 vs
// retail's 0x130 and a uniform one-step shift of the scratch registers (retail
// r7/r8 for the display reads and r6 for `&graphics->unkFC`, ours r6/r7 and
// r4, i.e. retail still has r4 -- `cue` -- allocated where we release it).
//
// The 0x78 gap is exactly **two** GXRenderModeObj objects: sizeof is 0x3c, and
// two of them pack to 0x78 while one alone costs 0x40 (trailing alignment).
// Measured in header round 15, all with 137 instructions unchanged:
//   `GXRenderModeObj rmo = getRenderMode(); mRenderMode = rmo;`      0xf8
//   a by-value `TGraphics::setRenderMode(GXRenderModeObj)` setter    0xf8
//   the same setter taking `const GXRenderModeObj&`                  0xc0
//   the named local *and* the by-value setter together              0x130 (exact)
//   two chained named copies                                        0x130 (exact)
// So retail copied the render mode twice on the way in. The natural single
// source for that is a by-value *return* from TDisplay::getRenderMode() plus
// one binding, but a by-value return cannot be what System/RenderModeObj uses
// (`&param_1->getRenderMode()`), and a `TGraphics::setRenderMode` by-value
// setter has no symbol in the map. Left as is rather than guessed; the
// register shift is a separate, unexplained residue (a `getDisplay()` accessor
// takes the frame to 0x170 and does not move it).
//
// Re-measured in closure batch 133: the two residues really are independent.
// `GXRenderModeObj rmo = unk10->getRenderMode(); graphics->mRenderMode = rmo;`
// is 0xf8 at 137 instructions (MWCC reserves the slot and folds the copy), and
// a second chained named copy lands 0x130 at 137 instructions -- but the score
// stays 96.8% because all ~40 register operands are still one step low. So the
// frame is a solved two-slot problem and the blocker is the allocator: this is
// a leaf with no saved registers, retail allocates its scratch values in
// r6/r7/r8 while we start at r4, i.e. retail's `cue` (r4) is still reserved
// where ours is free. Nothing in the body reads `cue` after the `CUE_DRAW`
// test, so the missing use is either another statement that consumes it or a
// guard spelling that keeps it live; no spelling tried so far does.
// Also ruled out: a parked by-value `TGraphics::setRenderMode(GXRenderModeObj)`
// and a parked by-value `getRenderMode()` accessor -- both land 0x130 but stop
// MWCC folding the copy and add 30 instructions (167 against 137).
// Closure re-pass 2026-09-18: the early-return guard spelling
// (`if (!(cue & CUE_DRAW)) return;`) is codegen-identical -- same 137
// instructions, same 0xb8 frame, same 84 low operands -- so it is not the
// missing use of `cue` either.
// Library re-pass 2026-09-18: header round 26's binding inside
// `TDisplay::getRenderMode()` has since moved this frame 0xb8 -> 0xc0, so the
// shortfall is now 0x70, not 0x78, and the two-copy reading above has to be
// re-derived. The register residue is re-measured and unchanged: this is a leaf
// with no saved registers, ours allocates the scratch pool {r4, r6, r7} and
// retail {r6, r7, r8}, i.e. the only difference is that r4 -- `cue` -- is still
// reserved in retail while we hand it to `&graphics->unkFC`. Both builds give
// the long-lived `unkFC` pointer the lowest free register and the two display
// temps the next two, so the shape of the allocation is right and only the
// start of the free list is wrong. Since all 137 instructions match, retail's
// extra live value cannot be materialised anywhere: `cue` itself has to stay
// live past the guard, and no guard spelling, accessor or named copy tried so
// far does that.
void TFrmGXSet::perform(u32 cue, TGraphics* graphics)
{
	if (cue & CUE_DRAW) {
		graphics->mFrameBuffer = unk10->getCurrentFrameBuffer();
		graphics->mRenderMode  = unk10->getRenderMode();

		graphics->getUnkFC().setBit(0x1, unk10->unk64.check(0x1));
		graphics->getUnkFC().setBit(0x2, unk10->unk64.check(0x2));
		graphics->getUnkFC().setBit(0x4, unk10->unk64.check(0x4));
		graphics->getUnkFC().setBit(0x8, unk10->unk64.check(0x8));
		graphics->getUnkFC().setBit(0x10, unk10->unk64.check(0x10));
		graphics->getUnkFC().setBit(0x20, unk10->unk64.check(0x20));
		graphics->getUnkFC().setBit(0x40, unk10->unk64.check(0x40));

		graphics->mFBClamp    = unk10->getFBClamp();
		graphics->mClearColor = unk10->getClearColor();
		graphics->mClearZ     = unk10->getClearZ();
	}
}
