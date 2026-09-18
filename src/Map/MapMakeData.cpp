#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

template <class T> static inline T max(const T& a, const T& b)
{
	return a > b ? a : b;
}
template <class T> static inline T min(const T& a, const T& b)
{
	return b > a ? a : b;
}

void TBGCheckData::setVertex(const JGeometry::TVec3<f32>& point1,
                             const JGeometry::TVec3<f32>& point2,
                             const JGeometry::TVec3<f32>& point3)
{
	mPoint1 = point1;
	mPoint2 = point2;
	mPoint3 = point3;

	mNormal.x = (mPoint2.y - mPoint1.y) * (mPoint3.z - mPoint2.z)
	            - (mPoint2.z - mPoint1.z) * (mPoint3.y - mPoint2.y);
	mNormal.y = (mPoint2.z - mPoint1.z) * (mPoint3.x - mPoint2.x)
	            - (mPoint2.x - mPoint1.x) * (mPoint3.z - mPoint2.z);
	mNormal.z = (mPoint2.x - mPoint1.x) * (mPoint3.y - mPoint2.y)
	            - (mPoint2.y - mPoint1.y) * (mPoint3.x - mPoint2.x);

	if (mNormal.x != 0.0f || mNormal.y != 0.0f || mNormal.z != 0.0f) {
		MsVECNormalize(&mNormal, &mNormal);
		mPlaneDistance = -(mNormal.x * mPoint1.x + mNormal.y * mPoint1.y
		                   + mNormal.z * mPoint1.z);

		mMinY = min(min(mPoint3.y, mPoint2.y), mPoint1.y);
		mMaxY = max(mPoint1.y, max(mPoint2.y, mPoint3.y));
	}
}

void TMapCollisionMove::setList()
{
	TBGCheckData* checkDataIt = mCheckDatas;
	for (u32 i = mCheckDataNum; i != 0; --i) {
		gpMapCollisionData->addCheckDataToGrid(checkDataIt, getUnk8());
		++checkDataIt;
	}
}

// TODO: 92.1%. Instruction-exact except the nine vertex-component copies and
// the frame (0x98 vs 0x70, 40 bytes short, all of it dead low region below the
// three vectors). Retail's *out-of-line* copy interleaves lfs/stfs per
// component while the two expansions inlined into initAllCheckData batch the
// three loads, which is what the three-argument constructor below produces.
// Measured (closure batch 83): the per-component spelling
// (`p1.x = p1raw[0]; ...`, or the same three stores behind a TU-static inline
// helper so the statement count stays under the depth-1 budget) makes this
// function instruction-exact at 99.8% but drops initAllCheckData 99.8 -> 78.8
// and, when spelled out at 15 statements, pushes setCheckData out of line and
// breaks updateCheckData's UNUSED size (0xf0 -> 0x6c) and update(). `p1.set(x,
// y, z)` and the constructor are codegen-identical. So one source cannot give
// both shapes, and duplicated source in initAllCheckData is implausible; the
// remaining lead is the 40 bytes themselves, which appear only where
// TBGCheckData::setVertex is inlined at depth 1 (in initAllCheckData setVertex
// is a real bl and the frame is only 8 short), i.e. some callee of setVertex
// holds locals whose slots we do not reserve. `volatile char trash[40]` does
// not change the schedule, so the frame alone is not the cause.
//
// Closure batch 90 added two facts.
//  * The min/max helpers above cannot be the carrier: a dead non-trivial
//    4-byte local in each is +32 here (8-byte: +48, so 4 bytes would be the
//    right size) but it also takes the *emitted* TBGCheckData::setVertex from
//    its exact 0x40 to 0x60.  Per the batch-69 rule a carrier must have no
//    matching out-of-line copy, and setVertex has one, so the 40 bytes belong
//    to setCheckData's own body or to setVertex's parameter bindings.
//  * The p1/p2/p3 slots are *ascending* in initAllCheckData's expansion
//    (0x64/0x70/0x7c, i.e. the inline-temp pool, which grows up in expansion
//    order) and *descending* in the emitted copy (0x78/0x6c/0x60, i.e. named
//    body locals, which grow down in declaration order).  That is the same
//    source in two regions, so the batched-vs-interleaved load order is a
//    context property of those two regions rather than two spellings.
//
// Closure batch (this one) settled which spelling is right and took the frame
// off the table as the lever.
//  * The map decides it: updateCheckData is UNUSED at 0xf0 and *inlines*
//    setCheckData (our 0xf0 copy has the vertex construction plus `bl
//    setVertex` and `bl addCheckDataToGrid` in the loop).  Any spelling at 15
//    statements -- which is what nine per-component assignments cost -- makes
//    it a `bl` and updateCheckData 0x6c.  So setCheckData must stay at or
//    under the depth-1 budget, i.e. the three-argument constructor, and the
//    interleaved schedule of the *emitted* copy is a scheduling artefact of
//    that copy alone (retail mixes setVertex's first `lwz` into the p2
//    construction; in initAllCheckData and updateCheckData setVertex is a
//    `bl` and there is nothing to interleave with).
//  * The missing bytes are not the cause of the schedule.  A dead
//    non-trivial 24-byte local, whether block-scoped in this body or in
//    setVertex's body, lands the frame at retail's 0x98 exactly and leaves
//    all nine copies batched (92.3%, 24 operand-only marks).  Neither
//    carrier is legal: a local here is paid twice by initAllCheckData's two
//    mutually exclusive expansions (0xd0 -> 0x100) and one in setVertex
//    grows its byte-exact out-of-line copy (0x40 -> 0x58).  Sizes 8/12/16/32/40
//    all miss.
//  * Also rejected, measured: `p1 = verts[indices[0]]` through the implicit
//    Vec -> TVec3 conversion (frame lands 0x98 exactly, but the conversion
//    temporaries cost instructions, 71%); `TVec3 p1(*(const Vec*)p1raw)` or
//    `p1.set(*(const Vec*)p1raw)` (one statement each, so still inlined, and
//    they *do* produce retail's interleaved schedule -- setCheckData 99.8%,
//    frame only -- but then initAllCheckData's two expansions interleave too
//    and it drops to 78.9%, and updateCheckData's size rules the spelling
//    out); three unnamed `TVec3` temporaries in the setVertex call (87.4%,
//    updateCheckData 252); `TVec3<f32> p[3]` (87.7%).
//
// Closure re-pass (batch 161) narrowed where the 24 remaining bytes live,
// under the new UNUSED-callee carrier rule, and found no legal carrier.
//  * They are not in setCheckData's own body: initAllCheckData is frame-exact
//    at 0xd0 with *two* expansions of it, and any dead local here is paid
//    twice there.  Collapsing those two sites into one with a ternary
//    (`setCheckData(..., (param_3 & 2) ? 3 : 0)`) so a single payment could
//    hide in 0xd0 costs instructions: initAllCheckData 100% -> 70.8%.  So
//    retail really has the two mutually exclusive call sites.
//  * They are not in setVertex's body either: its out-of-line copy is
//    byte-exact at frame 0x40.
//  * The only structure left is the expansion boundary itself -- the 24 bytes
//    appear exactly where setVertex is inlined at depth 1 and nowhere else
//    (in initAllCheckData and updateCheckData setVertex is a real `bl`).
//    `MsVECNormalize` is a `bl` on both sides, so it is not a hidden
//    expansion difference.  No UNUSED function is inlined into this body, so
//    the new "dead uninitialised TVec3 in an UNUSED callee" carrier rule has
//    nothing to attach to here.
void TMapCollisionBase::setCheckData(const f32* vertices, const s16* indices,
                                     TBGCheckData* param_3, int kind)
{
	const f32* p1raw = vertices + indices[0] * 3;
	const f32* p2raw = vertices + indices[1] * 3;
	const f32* p3raw = vertices + indices[2] * 3;

	JGeometry::TVec3<f32> p1(p1raw[0], p1raw[1], p1raw[2]);
	JGeometry::TVec3<f32> p2(p2raw[0], p2raw[1], p2raw[2]);
	JGeometry::TVec3<f32> p3(p3raw[0], p3raw[1], p3raw[2]);

	param_3->setVertex(p1, p2, p3);

	if (kind != 3)
		gpMapCollisionData->addCheckDataToGrid(param_3, getUnk8());
}

void TBGCheckData::updateTrans(const JGeometry::TVec3<f32>& translate_by)
{
	mPoint1 += translate_by;
	mPoint2 += translate_by;
	mPoint3 += translate_by;

	mMinY += translate_by.y;
	mMaxY += translate_by.y;

	mPlaneDistance = -(mNormal.x * mPoint1.x + mNormal.y * mPoint1.y
	                   + mNormal.z * mPoint1.z);
}

// TODO: retail hoists `delta.y` and `delta.z` into f31/f30 in the loop
// preheader and reloads only `delta.x` per use. Measured in a scratch TU with
// the game flags (research batch 86): MWCC gives a callee-saved FPR only to a
// *named* f32 local of the function's own body. An aggregate member read is
// reloaded at every use, and a local of an inlined callee is reloaded too --
// naming `y`/`z` inside `TBGCheckData::updateTrans` and calling it from here
// changes nothing, and passing `delta` to it by reference suppresses the
// promotion outright. So the translation loop has to be spelled here, which is
// why the body duplicates the UNUSED `TBGCheckData::updateTrans` above: that
// helper is never called in retail either (`UNUSED 0xdc` in the map). `y`
// before `z` is what puts y in f31 and z in f30.
// Residue (closure batch 90): 99.7%, every instruction exact, 0x58 vs 0x70.
// Declaring the iterator before the two named scalars was the last instruction
// difference (retail loads mCheckDatas before hoisting delta.y/z).
// The 24 bytes are two separate 12-byte objects, confirmed by probe:
//   * one named 12-byte local declared *after* delta.  A dead
//     `TVec3<f32>` there lands exactly between the operator- temporary and
//     delta (frame 0x58 -> 0x68, +16 because the named block then needs 4
//     bytes of alignment), which is retail's hole at 0x38-0x43.
//   * 12 bytes of outgoing-parameter/pool area below the operator-
//     temporary: retail's locals start at 0x2c, i.e. it reserves the full
//     32-byte outgoing area above 0xc, and ours start at 0x20 (20 bytes).
// Both together give 0x70 exactly (0xc + 32 pool + 36 named + 32 saved), so
// the two holes are the whole residue; neither has a source-level name yet.
// Re-measured this batch: unchanged, and the 12-byte hole between the
// operator- temporary (retail 0x2c, ours 0x20) and delta (retail 0x44, ours
// 0x2c) is the batch-119 dead-pool sizing law from both sides at once -- a
// dead named `TVec3` declared after delta pays the upper 12 and a dead
// non-trivial 12-byte local in an inlined callee the lower 12.  Nothing in
// the body names either, so it stays open rather than padded.
// Closure re-pass (batch 161): re-measured, still 99.7% frame-only, 0x58 vs
// 0x70.  Read as a pure low region the two holes are a dead TVec3 at 0x20 and
// another at 0x38 straddling the operator- temporary at 0x2c, i.e. 24 bytes =
// floor(24/8)*8, the price of two dead vectors in one expansion.  The new
// UNUSED-callee carrier rule cannot supply them: the only callees expanded
// here are `TVec3::operator-`/`operator=` (shared header, off limits) and
// `getUnk8()`, and `getUnk8()` is also expanded in the frame-exact
// `TMapCollisionMove::setList`, so any local there breaks that function.  The
// TU's three UNUSED functions (updateCheckData, updateVertexPos,
// TBGCheckData::updateTrans) are none of them called from this body.
void TMapCollisionBase::updateTrans(const JGeometry::TVec3<f32>& param_1)
{
	JGeometry::TVec3<f32> delta = param_1 - mPrevTranslation;

	TBGCheckData* checkDataIt = mCheckDatas;
	f32 f31                   = delta.y;
	f32 f30                   = delta.z;
	for (int i = 0; i < mCheckDataNum; ++checkDataIt, ++i) {
		checkDataIt->mPoint1.x += delta.x;
		checkDataIt->mPoint1.y += f31;
		checkDataIt->mPoint1.z += f30;
		checkDataIt->mPoint2.x += delta.x;
		checkDataIt->mPoint2.y += f31;
		checkDataIt->mPoint2.z += f30;
		checkDataIt->mPoint3.x += delta.x;
		checkDataIt->mPoint3.y += f31;
		checkDataIt->mPoint3.z += f30;

		checkDataIt->mMinY += f31;
		checkDataIt->mMaxY += f31;

		checkDataIt->mPlaneDistance
		    = -(checkDataIt->mNormal.x * checkDataIt->mPoint1.x
		        + checkDataIt->mNormal.y * checkDataIt->mPoint1.y
		        + checkDataIt->mNormal.z * checkDataIt->mPoint1.z);

		gpMapCollisionData->addCheckDataToGrid(checkDataIt, getUnk8());
	}

	mPrevTranslation = param_1;
}

void TMapCollisionBase::updateVertexPos(f32* vecs)
{
	MTXMultVecArray(unk20, mVertices, (Vec*)vecs, mVertexNum);
}

void TMapCollisionBase::updateCheckData(const f32* data)
{
	TBGCheckData* checkDataIt = mCheckDatas;
	// BUG: one group of triangles assumed
	s16* indexIt = mCollisionGroups[0].mIndices;
	for (u32 i = mCheckDataNum; i != 0; --i) {
		setCheckData(data, indexIt, checkDataIt, mKind);
		indexIt += 3;
		checkDataIt += 1;
	}
}

void TMapCollisionBase::update()
{
	f32 vecs[350 * 3];
	updateVertexPos(vecs);
	updateCheckData(vecs);
}

// Binding level worth +8 of low region, landing
// TMapCollisionBase::initAllCheckData's frame at 0xd0 (batch 124).
static inline TBGCheckData* MapMakeDataGetCheckDataPoolTop(TMapCollisionData* p)
{
	TBGCheckData* checkDataPoolTop = p->getCheckDataPoolTop();
	return checkDataPoolTop;
}

void TMapCollisionBase::initAllCheckData(s16 default_additional_data,
                                         const f32* vertices, u16 param_3,
                                         const TLiveActor* actor)
{
	mCheckDatas   = MapMakeDataGetCheckDataPoolTop(gpMapCollisionData);
	mCheckDataNum = 0;

	for (s16 i = 0; i < mCollisionGroupNum; ++i) {
		TMapCollisionGroup* thing = &mCollisionGroups[i];

		int numTris            = thing->mTriangleNum;
		int bgType             = thing->mBGType;
		BOOL useAdditionalData = thing->mFlags & HAS_ADDITIONAL_DATA;

		u8* unkCit            = thing->unkC;
		u8* unk10it           = thing->unk10;
		s16* indexIt          = thing->mIndices;
		s16* additionalDataIt = thing->mAdditionalDatas;

		TBGCheckData* checkData = gpMapCollisionData->allocCheckData(numTris);
		for (int j = 0; j < numTris; ++j) {
			checkData->mBGType = bgType;
			checkData->mActor  = actor;

			if (param_3 & 2)
				setCheckData(vertices, indexIt, checkData, 3);
			else
				setCheckData(vertices, indexIt, checkData, 0);

			if (useAdditionalData)
				checkData->mData = additionalDataIt[j];
			else
				checkData->mData = default_additional_data;

			checkData->unk6 = unkCit[j];
			checkData->unk7 = unk10it[j];

			indexIt += 3;

			++checkData;
			++mCheckDataNum;
		}
	}
}

void TMapCollisionBase::initCheckData(s16 param_1, u16 param_2,
                                      const TLiveActor* owner_actor)
{
	mPrevTranslation.x = unk20[0][3];
	mPrevTranslation.y = unk20[1][3];
	mPrevTranslation.z = unk20[2][3];

	if (param_2 & 4) {
		initAllCheckData(param_1, &mVertices->x, param_2, owner_actor);
	} else {
		f32 vecs[350 * 3];
		updateVertexPos(vecs);
		initAllCheckData(param_1, vecs, param_2, owner_actor);
	}
}
