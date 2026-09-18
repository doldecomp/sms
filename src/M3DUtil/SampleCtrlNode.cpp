#include <M3DUtil/SampleCtrlNode.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/Blocks/J3DColorBlocks.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTevs.hpp>

SampleCtrlJoint::SampleCtrlJoint(J3DJoint* joint)
    : unk4(nullptr)
    , unk8(nullptr)
    , mChild(nullptr)
    , mYounger(nullptr)
    , unk14(nullptr)
{
	unk4    = joint;
	mRadius = joint->getRadius();
	mMin.set(joint->getMin());
	mMax.set(joint->getMax());
}

// TODO (batch 131): 311/311 instructions are identical and the frame is now
// retail's 0xc8; the whole residue is 4 bytes, the getAttnFn() lookup table at
// 0x94 against retail's 0x90.
// Half of the original 8 came from spelling the slice explicitly:
// `unk38 = *(J3DTevOrderInfo*)material->getTevOrder(0)` instead of relying on
// the implicit base-class slice. That is a plain upcast (J3DTevOrder derives
// from J3DTevOrderInfo), not a reinterpret, and it is exactly what a developer
// writes when the copy is meant to take only the info part -- so it is kept.
// `*(J3DTevOrderInfo*)material->getTevBlock()->getTevOrder(0)` is the same -4
// (the two knobs are not additive), which says the 4 bytes are one inline
// *level*: `material->getTevOrder(i)` is the J3DMaterial forwarder plus
// J3DTevBlock's own getter, and skipping the forwarder removes one.
// The last 4 bytes therefore need exactly one of the 24 `getColorChan(i)` /
// `getTevStage(i)` sites to skip the forwarder as well. Three spellings are
// byte-exact and interchangeable -- `material->getColorBlock()->getColorChan(i)`
// at the mAttnFn site, or `material->getTevBlock()->getTevStage(i)` at either
// the first or the last field of the tev loop -- and all three are a mixed
// accessor path at one line out of eighteen identical ones, so none is
// committed: see the report for batch 131.
//
// Header round 22 decided this and rejects all of them, because the knob is
// the *size* of the inline-temporary pool and not any one site. The table is
// the top temporary, so its displacement is `0xc + pool`; ours is 4 bytes of
// pool more than retail's and every one of the 24 `getColorChan(i)` /
// `getTevStage(i)` forwarder expansions contributes exactly 4 bytes of it.
// Any single one of the 24 routed through its block therefore lands the match,
// which is precisely why none of them is evidence: retail's source cannot be
// recovered from a total. Committing one would be picking an arbitrary line to
// spell differently from its seventeen (or five) identical neighbours, i.e. a
// fakematch, so the unit stays out of objects.json over these 4 bytes. Also
// measured and rejected in round 22: `unk38` in an initialiser list instead of
// the first assignment (98.0%, and the pool does not move).
//
// The way out is a real -4 somewhere in the chain that is *not* one of the 24
// -- a J3DMaterial.hpp forwarder shape that expands without a dead pointer
// temporary, or one fewer expansion in the `getMatColor(0)` / `getTevStageNum`
// prologue -- not another permutation of this body.
//
// Every plausible alternative is +0:
// `j3dDefaultTevOrderInfoNull` through the same cast, `getMatColor(0)` or
// `getTevStageNum()` through their blocks, `i++` for `++i`, `4U` for the first
// bound, and `u16` for either loop counter (`u32`/`int`/`s32` cost four
// instructions; `int stageNum` does remove 8 bytes but from *above* the table
// and it turns retail's unsigned `cmplw` bound test into `cmpw`).
// Historical note, for the record (ours was 0xd0 / inline-temp region ending at
// 0x9c against retail 0xc8 / 0x94). The named
// local region is already right (the getAttnFn() lookup table sits 0x38 below
// the frame top in both). Priced in the temp region: each
// J3DMaterial::getColorChan/getTevStage expansion is +4 (6 and 18 sites),
// getTevOrder(0) +4, getMatColor(0) and getTevStageNum() +0, and the base is 8
// bytes more than retail's. Three spellings reach an exact match but all need a
// mixed accessor path nobody would write: two of the eighteen sites as
// material->getTevBlock()->getTevStage(i); or one such site plus either
// *(J3DTevOrderInfo*)material->getTevOrder(0) or
// *material->getTevBlock()->getTevOrder(0). Ruled out (no effect): explicit
// (u8)/(u32) casts on any getter or index, a u32 index local, getTevStageNum()
// or getMatColor(0) through their blocks, naming the J3DTevOrder*, a
// J3DTevOrderInfo& local, getTevOrderInfo() (+8 and moves the table 0xc),
// declaring stageNum at the top, getTevStage(u8)/const in J3DMaterial.hpp.
// Ruled out (breaks instructions): an initialiser list for unk4..unk10, a
// J3DColorChanInfo&/J3DTevStageInfo& row reference, a u32 loop index, dropping
// stageNum, and getColorBlock()->getColorChan(i) at two or more sites (a
// callee-saved rotation).
// TODO: 100.0% fuzzy with exactly two markers left, and they are one 4-byte
// pool slot: the lookup table that a J3D `getColorChan` accessor inline copies
// to the stack (`lwz r0, @NNNN; stw r0, N(r1)`, then `lbzx r0, r26, r0`) sits
// at 0x90 in retail and 0x94 here.  Everything else is pinned -- frame 0xc8,
// `stmw` at 0xb0 -- and that table is the *only* referenced slot in the whole
// 164-byte pool, so the residue is 4 bytes too many charged to the statements
// after it (the pool is allocated in reverse statement order), not a shortage.
// That is re-pass 172's pinned-pool allocation-order class, where the lever
// ladder has no +4 rung; the table itself comes from a shared J3D header, so
// there is nothing to steer from this .cpp.
SampleCtrlMaterial::SampleCtrlMaterial(J3DMaterial* material)
{
	unk38 = j3dDefaultTevOrderInfoNull;
	unk4  = nullptr;
	unk8  = nullptr;
	unkC  = 0;
	unk10 = nullptr;
	unk4  = material;
	unk14 = material->getMatColor(0)->color;

	for (u8 i = 0; i < 4; ++i) {
		unk18[i].mEnable    = material->getColorChan(i)->getEnable();
		unk18[i].mMatSrc    = material->getColorChan(i)->getMatSrc();
		unk18[i].mAmbSrc    = material->getColorChan(i)->getAmbSrc();
		unk18[i].mLightMask = material->getColorChan(i)->getLightMask();
		unk18[i].mDiffuseFn = material->getColorChan(i)->getDiffuseFn();
		unk18[i].mAttnFn    = material->getColorChan(i)->getAttnFn();
	}

	unk38 = *(J3DTevOrderInfo*)material->getTevOrder(0);

	u8 stageNum = material->getTevStageNum();
	for (u8 i = 0; i < stageNum; ++i) {
		unk3C[i].field_0x1  = material->getTevStage(i)->getTevColorA();
		unk3C[i].field_0x2  = material->getTevStage(i)->getTevColorB();
		unk3C[i].field_0x3  = material->getTevStage(i)->getTevColorC();
		unk3C[i].field_0x4  = material->getTevStage(i)->getTevColorD();
		unk3C[i].field_0x5  = material->getTevStage(i)->getTevColorOp();
		unk3C[i].field_0x6  = material->getTevStage(i)->getTevColorBias();
		unk3C[i].field_0x7  = material->getTevStage(i)->getTevColorScale();
		unk3C[i].field_0x8  = material->getTevStage(i)->getTevColorClamp();
		unk3C[i].field_0x9  = material->getTevStage(i)->getTevColorOutReg();
		unk3C[i].field_0xa  = material->getTevStage(i)->getAlphaA();
		unk3C[i].field_0xb  = material->getTevStage(i)->getAlphaB();
		unk3C[i].field_0xc  = material->getTevStage(i)->getAlphaC();
		unk3C[i].field_0xd  = material->getTevStage(i)->getAlphaD();
		unk3C[i].field_0xe  = material->getTevStage(i)->getTevAlphaOp();
		unk3C[i].field_0xf  = material->getTevStage(i)->getTevAlphaBias();
		unk3C[i].field_0x10 = material->getTevStage(i)->getTevAlphaScale();
		unk3C[i].field_0x11 = material->getTevStage(i)->getTevAlphaClamp();
		unk3C[i].field_0x12 = material->getTevStage(i)->getTevAlphaOutReg();
	}
}

SampleCtrlShape::SampleCtrlShape(J3DShape* param_1)
    : unk4(nullptr)
{
	unk4 = param_1;
	if (unk4->checkFlag(J3DShpFlag_Visible))
		unk8 = false;
	else
		unk8 = true;
}
