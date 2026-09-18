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
// Residue: 0x58 vs 0x70. The remaining 24 bytes are two 12-byte holes -- one
// extra inline-expansion temporary below the `operator-` temporary, and one
// named 12-byte local declared *after* `delta` (retail: temp 0x2c, delta 0x44;
// ours: temp 0x20, delta 0x2c).
void TMapCollisionBase::updateTrans(const JGeometry::TVec3<f32>& param_1)
{
	JGeometry::TVec3<f32> delta = param_1 - mPrevTranslation;
	f32 f31                     = delta.y;
	f32 f30                     = delta.z;

	TBGCheckData* checkDataIt = mCheckDatas;
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

void TMapCollisionBase::initAllCheckData(s16 default_additional_data,
                                         const f32* vertices, u16 param_3,
                                         const TLiveActor* actor)
{
	mCheckDatas   = gpMapCollisionData->getCheckDataPoolTop();
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
