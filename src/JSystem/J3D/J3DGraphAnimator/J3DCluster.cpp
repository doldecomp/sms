#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/os/OSCache.h>
#include <math.h>

void J3DDeformData::clear() { }

J3DDeformData::J3DDeformData() { }

void J3DDeformData::onAllFlag(u32 flag)
{
	for (u16 i = 0; i < mClusterNum; i++)
		mClusters[i].getDeformer()->onFlag(flag);
}

void J3DDeformData::offAllFlag(u32 flag)
{
	for (u16 i = 0; i < mClusterNum; i++)
		mClusters[i].getDeformer()->offFlag(flag);
}

void J3DDeformData::base(J3DModel*) { }

void J3DDeformData::deform(J3DModel* model)
{
	model->getVertexBuffer()->swapVtxPosArrayPointer();
	model->getVertexBuffer()->swapVtxNrmArrayPointer();

	for (u16 i = 0; i < mClusterNum; i++)
		mClusters[i].mDeformer->deform(model, i);

	DCStoreRange(model->getVertexBuffer()->getVtxPosArrayPointer(0),
	             model->getModelData()->getVtxNum() * sizeof(Vec));
	DCStoreRange(model->getVertexBuffer()->getVtxNrmArrayPointer(0),
	             model->getModelData()->getNrmNum() * sizeof(Vec));

	model->getVertexBuffer()->setCurrentVtxPos(
	    model->getVertexBuffer()->getVtxPosArrayPointer(0));
	model->getVertexBuffer()->setCurrentVtxNrm(
	    model->getVertexBuffer()->getVtxNrmArrayPointer(0));
}

void J3DDeformData::setAnm(J3DAnmCluster* anm)
{
	for (u16 i = 0; i < mClusterNum; i++)
		mClusters[i].getDeformer()->setAnmCluster(anm);
}

void J3DDeformer::clear() { }

void J3DDeformer::deform(J3DModel* model, u16 idx)
{
	u16 keyIdx = 0;
	if (mAnmCluster) {
		for (u16 i = 0; i < idx; i++)
			keyIdx += mDeformData->getClusterPointer(i)->mKeyNum;

		for (u16 i = 0; i < mDeformData->getClusterPointer(idx)->mKeyNum; i++)
			mWeightList[i] = mAnmCluster->getWeight(keyIdx++);

		deform(model, idx, mWeightList);
	}
}

// TODO: 99.7%. Two residues, no instruction is missing or extra.
// (1) frame 0x118 vs 0x110. Research 215 (2026-09-19) corrects the earlier
//     "two independent 4-byte gaps" reading: it is ONE missing word, and it
//     sits in the low inline-temp pool, below `pos[3]`. Slot map, retail
//     against ours: `pos[3]` 0x70/0x74/0x78 vs 0x6c/0x70/0x74, `sign[2]`
//     0x98/0x9c vs 0x94/0x98, the u16-to-f32 conversion pair 0xb0/0xb4 vs
//     0xa8/0xac, `stmw r20` 0xb8 vs 0xb0. Every named slot is exactly 4 low,
//     and the conversion pair is 8 low only because it is `lfd`-aligned: one
//     extra word under `pos[3]` shifts the whole named block by 4 and rounds
//     the conversion pair, the saved registers and the frame to retail's.
//     Measured: `volatile char trash[4]` declared first (above `cluster`) is
//     +8 and lands frame 0x118 with the conversion slot and `stmw` exact,
//     leaving `pos`/`sign` 4 low -- it fills the gap from the wrong end. The
//     missing word is an inline-expansion temp; a +4 rung (a direct-return
//     fork over a plain member read, nested in a binder) on one of the raw
//     `cluster->`/`vertex->` member reads is the open candidate.
// (2) the position loop's sign multiplies schedule the two `rlwinm` bit
//     extracts in the opposite order (retail computes the y index at 0x5f0 and
//     the z index at 0x604, we do the reverse) with r6/r7/r8/r9 rotated
//     accordingly in the normal loop as well.
// Closure round 2026-09-18, three more spellings, frame unmoved at 0x110 in
// all three: `f32 weight = 1.0f; weight /= vertex->mNum;` (469 instructions),
// `f32 sign[2];` with two stores instead of the aggregate initialiser (four
// operands worse, same frame), and hoisting `sign` above the zeroing loop
// (473 instructions).
// TODO: 99.8%, 18 operand markers, all of them register allocation; the frame
// is now exact. Closure 217: the 8-byte frame gap research 215 measured was
// one 4-byte inline-accessor temporary at the bottom of the dead low pool
// (every named slot sat 4 low and the `lfd`-aligned save area rounded that to
// 8). Routing *any single* one of this function's raw `cluster->` reads
// through an in-class accessor lands frame 0x118 and drops 60 markers to 18:
// `mNrmNum`, `mPosNum`, `mClusterVertexNum` and `mFlags` are byte-identical
// choices, and each is read exactly once in the whole tree, so which member
// retail's accessor covered is not decidable from this unit -- `getNrmNum()`
// is the one used here. Refuted: `getKeyNum()` (four sites, +2 instructions,
// frame 0x120), `getPosDstIdx()` (frame right, 59 markers),
// `J3DClusterVertex::getNum()` at both its sites (40) or
// `getSrcIdx()` (40), `getClusterVertex()` (37), and the angle accessors
// (frame 0x120/0x128, over by one and two rungs).
// The remaining 18 markers are three independent volatile-register rotations
// in the normal loop (r7/r9 and r6/r7 around `lwz r7, 4(r7)`, r6/r8 in the
// sign-flip block, r7/r8 at the `li` before it) plus the swapped emission
// order of the two `rlwinm ..., 20/21, 29, 29` sign-index shifts; no frame
// lever moves them.
// Research 219 (2026-09-19) measured the source knobs on those 18 and every
// one is worse, so the stock body is a local optimum:
// reusing `flag` for the `src` index (20 markers, 2 opcode diffs), a named
// `u16* posFlag = clusterKey->mPosFlag;` (20/2), dropping the named
// `clusterKey` in the position loop (17 but 2 opcode diffs: retail's
// `add r7, r27, r7; lwz r7, 4(r7)` becomes `addi r7, r7, 4; lwzx r9, ...`,
// so the named local is right) or in the normal loop (18/2), swapping the
// declaration order of `flag` and `src` (21 -- the order below is the better
// one), `sign[(flag >> 15) & 1]` (27/4/5/5), naming the three shift results
// or the three `sign[]` values as locals (41 each), three `f32` locals in
// place of the `Vec` (41), `posDstIdx[i]` in the `dst` expression (52) and
// `cluster->mPosDstIdx[i]` in the zeroing loop (91).
// The two surviving shapes are the same one phenomenon, and it is the one
// that also blocks `J3DSkinDeform::initMtxIndexArray`: retail builds an
// address chain in a single volatile register (`add r7, r27, r7` then
// `lwz r7, 4(r7)` then `clrlslwi r7` reusing it again) where we spend a
// fresh one per step; the `rlwinm 20`/`rlwinm 21` pair is a pure schedule
// swap -- both sides put the y index in r7 and the z index in r6, only the
// two slots are exchanged.
// Closure c1 (2026-09-22), all inert (18) or worse: `u16`/`s32` `flag` in
// either loop, `keys + j`, `*(weightList + j)`, `Vec` hoisted, `src` before
// `flag` (21), a named `f32 w` (43), `int` loop counters (51+), `u16` counts.
void J3DDeformer::deform(J3DModel* model, u16 idx, f32* weightList)
{
	if (checkFlag(2) && model->getModelData()->isDeformableVertexFormat()) {
		J3DCluster* cluster = mDeformData->getClusterPointer(idx);
		s32 posNum          = cluster->mPosNum;
		s32 keyNum          = cluster->mKeyNum;

		u16 keyStart = 0;
		for (u16 i = 0; i < idx; i++)
			keyStart += mDeformData->getClusterPointer(i)->mKeyNum + 1;

		J3DClusterKey* keys = mDeformData->getClusterKeyPointer(keyStart);
		normalizeWeight(keyNum, weightList);

		f32* vtxPosDst
		    = (f32*)model->getVertexBuffer()->getVtxPosArrayPointer(0);
		f32* vtxPosSrc = mDeformData->getVtxPos();
		u16* posDstIdx = cluster->mPosDstIdx;

		for (s32 i = 0; i < posNum; i++) {
			s32 idx            = posDstIdx[i] * 3;
			vtxPosDst[idx]     = 0.0f;
			vtxPosDst[idx + 1] = 0.0f;
			vtxPosDst[idx + 2] = 0.0f;
		}

		f32 sign[2] = { 1.0f, -1.0f };
		for (u16 i = 0; i < posNum; i++) {
			for (u16 j = 0; j < keyNum; j++) {
				J3DClusterKey* clusterKey = &keys[j];

				int flag = clusterKey->mPosFlag[i];
				f32* src = &vtxPosSrc[(clusterKey->mPosFlag[i] & ~0xE000) * 3];

				Vec deform;
				deform.x = src[0];
				deform.y = src[1];
				deform.z = src[2];

				deform.x *= sign[(flag & 0x8000) >> 15];
				deform.y *= sign[(flag & 0x4000) >> 14];
				deform.z *= sign[(flag & 0x2000) >> 13];

				f32* dst = &vtxPosDst[cluster->mPosDstIdx[i] * 3];
				dst[0] += deform.x * weightList[j];
				dst[1] += deform.y * weightList[j];
				dst[2] += deform.z * weightList[j];
			}
		}

		if (checkFlag(1) && cluster->mFlags != 0) {
			f32* vtxNrmDst
			    = (f32*)model->getVertexBuffer()->getVtxNrmArrayPointer(0);
			f32* vtxNrmSrc = mDeformData->getVtxNrm();
			f32* nrmBuf    = field_0x0c;

			for (u16 i = 0; i < cluster->getNrmNum(); i++) {
				f32* dst = &nrmBuf[i * 3];
				dst[0]   = 0.0f;
				dst[1]   = 0.0f;
				dst[2]   = 0.0f;

				for (u16 j = 0; j < cluster->mKeyNum; j++) {
					J3DClusterKey* clusterKey = &keys[j];

					int flag = clusterKey->mNrmFlag[i];
					f32* src
					    = &vtxNrmSrc[(clusterKey->mNrmFlag[i] & ~0xE000) * 3];

					Vec deform;
					deform.x = src[0];
					deform.y = src[1];
					deform.z = src[2];

					if (flag & 0x8000)
						deform.x = -deform.x;
					if (flag & 0x4000)
						deform.y = -deform.y;
					if (flag & 0x2000)
						deform.z = -deform.z;

					dst[0] += deform.x * weightList[j];
					dst[1] += deform.y * weightList[j];
					dst[2] += deform.z * weightList[j];
				}

				normalize(dst);
			}

			for (u16 i = 0; i < cluster->mClusterVertexNum; i++) {
				J3DClusterVertex* vertex = &cluster->mClusterVertex[i];
				f32 pos[3];
				pos[0] = 0.0f;
				pos[1] = 0.0f;
				pos[2] = 0.0f;

				f32 weight = 1.0f / (f32)vertex->mNum;
				for (u16 j = 0; j < vertex->mNum; j++) {
					f32* src = &nrmBuf[vertex->mSrcIdx[j] * 3];
					pos[0] += weight * src[0];
					pos[1] += weight * src[1];
					pos[2] += weight * src[2];
				}
				normalize(pos);

				for (u16 j = 0; j < vertex->mNum; j++) {
					u16 srcIdx = vertex->mSrcIdx[j];
					u16 dstIdx = vertex->mDstIdx[j];

					if (dstIdx == 0xFFFF)
						continue;

					f32 dot = pos[0] * nrmBuf[srcIdx * 3]
					          + pos[1] * nrmBuf[srcIdx * 3 + 1]
					          + pos[2] * nrmBuf[srcIdx * 3 + 2];

					f32 angle;
					if (dot >= 1.0f) {
						angle = 0.0f;
					} else if (dot > -1.0f) {
						angle = acosf(dot);
						angle = (angle * 180.0f) / 3.1415f;
					} else {
						angle = 180.0f;
					}

					if (angle <= cluster->mMinAngle) {
						vtxNrmDst[dstIdx * 3 + 0] = pos[0];
						vtxNrmDst[dstIdx * 3 + 1] = pos[1];
						vtxNrmDst[dstIdx * 3 + 2] = pos[2];
					} else if (angle > cluster->mMaxAngle) {
						vtxNrmDst[dstIdx * 3 + 0] = nrmBuf[srcIdx * 3];
						vtxNrmDst[dstIdx * 3 + 1] = nrmBuf[srcIdx * 3 + 1];
						vtxNrmDst[dstIdx * 3 + 2] = nrmBuf[srcIdx * 3 + 2];
					} else {
						f32 weights[2];
						weights[0]
						    = (angle - cluster->mMinAngle)
						      / (cluster->mMaxAngle - cluster->mMinAngle);
						weights[1] = 1.0f - weights[0];

						vtxNrmDst[dstIdx * 3 + 0]
						    = weights[1] * pos[0]
						      + weights[0] * nrmBuf[srcIdx * 3];
						vtxNrmDst[dstIdx * 3 + 1]
						    = weights[1] * pos[1]
						      + weights[0] * nrmBuf[srcIdx * 3 + 1];
						vtxNrmDst[dstIdx * 3 + 2]
						    = weights[1] * pos[2]
						      + weights[0] * nrmBuf[srcIdx * 3 + 2];
					}
				}
			}
		}
	}
}

void J3DDeformer::base(J3DModel*) { }

void J3DDeformer::normalize(float* vec)
{
	f32 inv = 1.0f
	          / std::sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
	vec[0] *= inv;
	vec[1] *= inv;
	vec[2] *= inv;
}

void J3DDeformer::normalizeWeight(int count, float* weight)
{
	f32 sum = 0.0f;
	for (u16 i = 0; i < count; i++)
		sum += weight[i];
	sum = 1.0f / sum;
	for (u16 i = 0; i < count; i++)
		weight[i] *= sum;
}

J3DSkinDeform::J3DSkinDeform()
{
	mPosUseMtx = nullptr;
	mNrmUseMtx = nullptr;
	mNrmMtx    = nullptr;
	mFlags     = 3;
}

void J3DSkinDeform::initMtxIndexArray(J3DModelData* modelData)
{

	if (mPosUseMtx != nullptr && mNrmUseMtx != nullptr)
		return;

	mPosUseMtx = new u16[modelData->getVtxNum()];

	for (u32 i = 0; i < modelData->getVtxNum(); i++)
		mPosUseMtx[i] = 0;

	if (modelData->getNrmNum() != 0) {
		mNrmUseMtx = new u16[modelData->getNrmNum()];

		for (u32 i = 0; i < modelData->getNrmNum(); i++)
			mNrmUseMtx[i] = 0;
	} else {
		mNrmUseMtx = nullptr;
	}

	mNrmMtx = new (0x20) Mtx33[modelData->getDrawMtxNum()];

	for (u16 i = 0; i < modelData->getShapeNum(); i++) {
		int size[4] = { 0, 1, 1, 2 };

		int pnmtxIdxOffs = -1;
		int posOffs      = -1;
		int nrmOffs      = -1;
		int vtxSize      = 0;
		for (GXVtxDescList* desc
		     = modelData->getShapeNodePointer(i)->getVtxDesc();
		     desc->attr != GX_VA_NULL; desc++) {
			switch (desc->attr) {
			case GX_VA_PNMTXIDX:
				pnmtxIdxOffs = vtxSize;
				break;
			case GX_VA_POS:
				posOffs = vtxSize;
				if (desc->type != GX_INDEX16) {
					return;
				}
				break;
			case GX_VA_NRM:
				nrmOffs = vtxSize;
				if (desc->type != GX_INDEX16) {
					return;
				}
				break;
			case GX_VA_TEX0:
				break;
			default:
				break;
			}

			vtxSize += size[desc->type];
		}

		for (u16 j = 0; j < modelData->getShapeNodePointer(i)->getMtxGroupNum();
		     j++) {
			J3DShapeMtx* shapeMtx
			    = modelData->getShapeNodePointer(i)->getShapeMtx(j);

			u8* displayListStart = modelData->getShapeNodePointer(i)
			                           ->getShapeDraw(j)
			                           ->getDisplayList();
			for (u8* dl = displayListStart;
			     (dl - displayListStart) < modelData->getShapeNodePointer(i)
			                                   ->getShapeDraw(j)
			                                   ->getDisplayListSize();) {
				u8 cmd = dl[0];
				if (cmd != GX_TRIANGLEFAN && cmd != GX_TRIANGLESTRIP)
					break;

				u16 vtxCount = *(u16*)&dl[1];

				u16 useMtxIdxBuf[10];
				for (s32 k = 0; k < vtxCount; k++) {
					// TODO: the only residue in this function is the operand
					// order of this one `add`: retail emits
					// `add r4, dl, vtxSize*k`, we emit the reverse, and the
					// following `addi r4, r4, 3` and the three indexed loads
					// all match. Every other spelling costs an extra
					// instruction (`dl + 3 + vtxSize * k`,
					// `&dl[vtxSize * k] + 3`, `(dl + vtxSize * k) + 3`) or
					// perturbs the frame (a named `int ofs`); `vtx += ...`,
					// `dl + (3 + ...)` and `&dl[vtxSize * k + 3]` are all
					// identical to this one.
					// Closure round 2026-09-18: `dl + vtxSize * k + 3`,
					// `(dl + vtxSize * k) + 3`, `dl + 3 + vtxSize * k` and
					// `&(dl + vtxSize * k)[3]` are all 248 instructions;
					// `&dl[k * vtxSize + 3]` flips the `mullw` operands
					// instead; the `(u32)` cast forms, dropping the `vtx`
					// local for three spelled-out indices (248) and
					// splitting off a `vtxBase` local (frame 0x118) are all
					// no better.
					// Research 215 (2026-09-19), ~60 spellings measured in a
					// scratch TU carrying this loop nest: the operand order
					// is not an index spelling at all, it is the statement
					// form. A single address expression whose constant sits
					// inside the index (`&dl[3 + p]`, `dl + (p + 3)`) gives
					// `add rD, rIndex, rPtr` then `addi 3`; a constant left
					// outside (`dl + p + 3`, `&(dl + p)[3]`, a struct member
					// offset such as `((Hdr*)(dl + p))->data`) gives `addi 3`
					// then `add rD, rPtr, rIndex`, which costs one extra `mr`
					// here because the product lives in r0; and a compound
					// assignment `p += index` gives `add rD, rPtr, rIndex` --
					// retail's order. So
					// `u8* base = dl; base += vtxSize * k; u8* vtx = base + 3;`
					// reproduces retail's two instructions exactly (247
					// instructions, frame 0x110, operand order right) and
					// leaves only the destination register: retail keeps one
					// register (`add r4, r25, r0; addi r4, r4, 3`), the
					// two-object chain gets r3 then r4. One object with a
					// separate `vtx += 3` instead sinks the 3 into the three
					// load displacements (249 instructions), and every
					// one-object two-step form tried sinks the same way, so
					// the single-expression spelling below is kept as the
					// closer one (1 differing operand, not 2). Refuted for
					// the register: declaration order and scope of the two
					// locals, a TU-local `p += n; return p;` helper,
					// `(base += n) + 3`, `(vtx += n) += 3` (spills vtx to the
					// stack), the comma operator.
					// Closure 217: the sink is a *use* property, not a
					// statement-form one.  This loop's own outer step
					// `dl += vtxSize * vtxCount; dl += 3;` compiles to
					// exactly retail's `add rD, rPtr, rIndex; addi rD, rD, 3`
					// because `dl` also has non-indexed uses; the inner
					// pointer's only three uses are indexed, so any form that
					// materialises `vtx` before the `+ 3` lets MWCC fold the
					// 3 into three separate `add`s plus `lbz/lhz 3(rX)`
					// (+2 instructions, 249).  `vtx += vtxSize * k + 3`,
					// `vtx += 3 + vtxSize * k` and the int-first
					// `(3 + vtxSize * k) + dl` all canonicalise back to this
					// line's index-first `add`; `dl + 3` before the compound
					// (either order) is 248 with the `addi` and `add` merely
					// swapped.  The two-object form
					// `u8* base = dl; base += vtxSize * k; u8* vtx = base + 3;`
					// is 247 with retail's operand order and its *only*
					// residue is the intermediate register (r3 for retail's
					// r4, 2 markers against this line's 1), so the last step
					// is a volatile-register knob, not another spelling.
					// Research 219 (2026-09-19) names the mechanism and
					// closes the question as unreachable from the source.
					// The `+ 3` sink is MWCC's *address flattening*: inside
					// one basic block every `p[reg]` use of a pointer whose
					// definition is a chain of adds is rewritten to
					// `add rX, base, reg` plus a displacement load, so a
					// separate `addi 3` only survives when the flattener
					// cannot see the definition.  Measured in a scratch TU
					// carrying this loop nest (~40 further spellings): the
					// sink is insensitive to the number of uses (1, 2 and 3
					// indexed uses all sink), to an extra non-address use
					// (`f(vtx)` sinks *and* pays its own `addi`), to a
					// constant-0 index, to `++vtx` x3, `vtx = vtx + 3`,
					// `vtx += sizeof(T)`, a `u8**`/`u8*&` helper,
					// `do { } while (0)` and a later `vtx - dl` use.
					// The chain also coalesces into ONE register the moment
					// there is no second SSA value, which is exactly retail's
					// `add r4, r25, r0; addi r4, r4, 3`:
					// `u8* vtx = dl; vtx += vtxSize * k;` alone compiles to
					// `add r4, r25, r0` -- retail's operand order *and*
					// retail's register.  Everything that keeps the `+ 3`
					// alive introduces a second value, and a second value
					// that dies immediately always takes the lowest free
					// volatile (r3) while the long-lived one takes r4: the
					// two-object form, a `(J3DDLHdr*)vtx + 1` retype (which
					// does block the sink), `u32` arithmetic, an inlined
					// `p += off; return p + 3;` helper, `register`, and both
					// declaration scopes all give r3/r4 = 2 markers.
					// The ONE construct that reproduces retail exactly is a
					// basic-block boundary between the two steps
					// (`vtx += vtxSize * k; if (c) vtx += 3;`): the flattener
					// stops at the block edge and the phi forces both defs
					// into r4.  There is no branch there in retail's stream,
					// so no honest source reaches it; the single-expression
					// spelling below (1 marker, the add's operand order) is
					// the closest form and is kept.
					// Closure c1 (2026-09-22): `dl - -(...)`, `(u32)`/`(s32)` index casts
					// are identical; `3 + k * vtxSize` also flips the `mullw`; every
					// `(dl + 3) + ...`/`3 + dl + ...` reorder is 248 instructions.
					u8* vtx     = &dl[3 + vtxSize * k];
					u8 pnmtxIdx = ((u32)(*(u8*)&vtx[pnmtxIdxOffs])) / 3;
					u16 posIdx  = *(u16*)&vtx[posOffs];
					u16 nrmIdx  = *(u16*)&vtx[nrmOffs];

					u16 useMtxIdx = shapeMtx->getUseMtxIndex(pnmtxIdx);
					if (useMtxIdx == 0xFFFF) {
						useMtxIdx = useMtxIdxBuf[pnmtxIdx];
					} else if (pnmtxIdxOffs != -1) {
						useMtxIdxBuf[pnmtxIdx] = useMtxIdx;
					}

					mPosUseMtx[posIdx] = useMtxIdx;
					if (nrmOffs != -1)
						mNrmUseMtx[nrmIdx] = useMtxIdx;
				}

				dl += vtxSize * vtxCount;
				dl += 3;
			}
		}

		if (nrmOffs == -1) {
			modelData->getShapeNodePointer(i)->onFlag(J3DShpFlag_EnableLod);
			modelData->getShapeNodePointer(i)->offFlag(J3DShpFlag_SkinNrmCpu);
		}
	}

	for (u32 i = 0; i < modelData->getVtxNum(); i++) {
		if (mPosUseMtx[i] >= modelData->getDrawMtxNum()) {
			mPosUseMtx[i] = 0;
		}
	}

	for (u32 i = 0; i < modelData->getNrmNum(); i++) {
		if (mNrmUseMtx[i] >= modelData->getDrawMtxNum()) {
			mNrmUseMtx[i] = 0;
		}
	}
}

void J3DSkinDeform::calcNrmMtx(J3DModel* model)
{
	J3DModelData* modelData = model->getModelData();
	for (u16 i = 0; i < modelData->getDrawMtxNum(); i++) {
		if (modelData->getDrawMtxFlag(i) == 0) {
			if (model->getScaleFlag(modelData->getDrawMtxIndex(i)) == 1) {
				setNrmMtx(i, model->getAnmMtx(modelData->getDrawMtxIndex(i)));
			} else {
				J3DPSCalcInverseTranspose(
				    model->getAnmMtx(modelData->getDrawMtxIndex(i)),
				    getNrmMtx(i));
			}
		} else {
			if (model->getEnvScaleFlag(modelData->getDrawMtxIndex(i)) == 1) {
				setNrmMtx(
				    i, model->getWeightAnmMtx(modelData->getDrawMtxIndex(i)));
			} else {
				J3DPSCalcInverseTranspose(
				    model->getWeightAnmMtx(modelData->getDrawMtxIndex(i)),
				    getNrmMtx(i));
			}
		}
	}
}

void J3DSkinDeform::deform(J3DModel* model)
{
	Mtx* anmMtx = nullptr;
	Mtx* mtxArr[2];
	mtxArr[0] = (Mtx*)model->getAnmMtx(0);
	mtxArr[1] = (Mtx*)model->getWeightAnmMtx(0);

	if (model->checkFlag(0x4)) {
		model->getVertexBuffer()->swapTransformedVtxPos();

		Vec* curVtxPos = (Vec*)model->getVertexBuffer()->getCurrentVtxPos();
		s32 vtxNum     = model->getModelData()->getVtxNum();
		Vec* transformedVtxPos
		    = (Vec*)model->getVertexBuffer()->getTransformedVtxPos(0);

		for (s32 i = 0; i < vtxNum; i++) {
			anmMtx
			    = mtxArr[model->getModelData()->getDrawMtxFlag(mPosUseMtx[i])];
			J3DPSMulMtxVec(
			    anmMtx[model->getModelData()->getDrawMtxIndex(mPosUseMtx[i])],
			    &curVtxPos[i], &transformedVtxPos[i]);
		}

		DCStoreRange(model->getVertexBuffer()->getTransformedVtxPos(0),
		             model->getModelData()->getVtxNum() * sizeof(Vec));
		model->getVertexBuffer()->setCurrentVtxPos(transformedVtxPos);
	}

	if (model->checkFlag(0x8)) {
		model->getVertexBuffer()->swapTransformedVtxNrm();

		J3DVertexBuffer* vtxBuffer = model->getVertexBuffer();
		s32 vtxNum                 = model->getModelData()->getNrmNum();
		Vec* curVtxNrm             = (Vec*)vtxBuffer->getCurrentVtxNrm();
		Vec* transformedVtxNrm     = (Vec*)vtxBuffer->getTransformedVtxNrm(0);

		calcNrmMtx(model);

		for (s32 i = 0; i < vtxNum; i++) {
			J3DPSMulMtxVec(mNrmMtx[mNrmUseMtx[i]], &curVtxNrm[i],
			               &transformedVtxNrm[i]);
		}

		DCStoreRange(model->getVertexBuffer()->getTransformedVtxNrm(0),
		             model->getModelData()->getNrmNum() * sizeof(Vec));
		model->getVertexBuffer()->setCurrentVtxNrm(transformedVtxNrm);
	}
}

void J3DVtxColorCalc::calc(J3DModel* model)
{
	// UNUSED, impl from TP, size doesn't match.
	if (checkFlag(1) && mpVtxColor) {
		model->getVertexBuffer()->swapVtxColArrayPointer();
		u16 anmTableNum = mpVtxColor->getAnmTableNum(0);
		GXColor* colorArray
		    = model->getVertexBuffer()->getVtxColArrayPointer(0);
		for (u16 i = 0; i < anmTableNum; i++) {
			GXColor color;
			mpVtxColor->getColor(0, i, &color);
			J3DAnmVtxColorIndexData* r28
			    = mpVtxColor->getAnmVtxColorIndexData(0, i);
			for (u32 j = 0; j < r28->mNum; j++) {
				colorArray[((u16*)(void*)r28->mpData)[j]] = color;
			}
		}
		DCStoreRange(colorArray,
		             model->getVertexBuffer()->getVertexData()->getColNum()
		                 * 4);
		model->getVertexBuffer()->setCurrentVtxCol(colorArray);
	}
}
