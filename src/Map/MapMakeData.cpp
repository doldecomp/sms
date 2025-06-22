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

void TBGCheckData::setVertex(const JGeometry::TVec3<f32>& param_1,
                             const JGeometry::TVec3<f32>& param_2,
                             const JGeometry::TVec3<f32>& param_3)
{
	mPoint1 = param_1;
	mPoint2 = param_2;
	mPoint3 = param_3;

	mNormal.x = (mPoint2.y - mPoint1.y) * (mPoint3.z - mPoint2.z)
	            - (mPoint2.z - mPoint1.z) * (mPoint3.y - mPoint2.y);
	mNormal.y = (mPoint2.z - mPoint1.z) * (mPoint3.x - mPoint2.x)
	            - (mPoint2.x - mPoint1.x) * (mPoint3.z - mPoint2.z);
	mNormal.z = (mPoint2.x - mPoint1.x) * (mPoint3.y - mPoint2.y)
	            - (mPoint2.y - mPoint1.y) * (mPoint3.x - mPoint2.x);

	if (mNormal.x != 0.0f || mNormal.y != 0.0f || mNormal.z != 0.0f) {
		MsVECNormalize(&mNormal, &mNormal);
		unk40 = -(mNormal.x * mPoint1.x + mNormal.y * mPoint1.y
		          + mNormal.z * mPoint1.z);

		mMinY = min(min(mPoint3.y, mPoint2.y), mPoint1.y);
		mMaxY = max(mPoint1.y, max(mPoint2.y, mPoint3.y));
	}
}

void TMapCollisionMove::setList()
{
	TBGCheckData* data = unk4;
	for (u32 i = unkC; i != 0; --i) {
		gpMapCollisionData->addCheckDataToGrid(data, getUnk8());
		++data;
	}
}

void TMapCollisionBase::setCheckData(const f32* param_1, const s16* param_2,
                                     TBGCheckData* param_3, int param_4)
{
	const f32* p1raw = param_1 + param_2[0] * 3;
	const f32* p2raw = param_1 + param_2[1] * 3;
	const f32* p3raw = param_1 + param_2[2] * 3;

	JGeometry::TVec3<f32> p1(p1raw[0], p1raw[1], p1raw[2]);
	JGeometry::TVec3<f32> p2(p2raw[0], p2raw[1], p2raw[2]);
	JGeometry::TVec3<f32> p3(p3raw[0], p3raw[1], p3raw[2]);

	param_3->setVertex(p1, p2, p3);

	if (param_4 != 3)
		gpMapCollisionData->addCheckDataToGrid(param_3, getUnk8());
}

void TBGCheckData::updateTrans(const JGeometry::TVec3<f32>& param_1)
{
	mPoint1.add(param_1);
	mPoint2.add(param_1);
	mPoint3.add(param_1);

	mMinY += param_1.y;
	mMaxY += param_1.y;

	unk40 = -(mNormal.x * mPoint1.x + mNormal.y * mPoint1.y
	          + mNormal.z * mPoint1.z);
}

void TMapCollisionBase::updateTrans(const JGeometry::TVec3<f32>& param_1)
{
	JGeometry::TVec3<f32> copy = calcOffset(param_1);

	TBGCheckData* unk4it = unk4;
	for (int i = 0; i < unkC; ++i) {
		unk4it->updateTrans(copy);
		gpMapCollisionData->addCheckDataToGrid(unk4it, getUnk8());
		unk4it += 1;
	}

	unk50 = param_1;
}

void TMapCollisionBase::updateVertexPos(f32*) { }

void TMapCollisionBase::updateCheckData(const f32* data)
{
	TBGCheckData* unk4it = unk4;
	s16* it              = unk1C->unk8;
	for (u32 i = unkC; i != 0; --i) {
		setCheckData(data, it, unk4it, unk8);
		it += 3;
		unk4it += 1;
	}
}

void TMapCollisionBase::update()
{
	Vec vecs[350]; // TODO: size?
	MTXMultVecArray(unk20, unk14, vecs, unk10);
	updateCheckData((f32*)vecs);
}

void TMapCollisionBase::initAllCheckData(s16 param_1, const f32* param_2,
                                         u16 param_3, const TLiveActor* param_4)
{
	unk4 = gpMapCollisionData->unk28 + gpMapCollisionData->unk34;
	unkC = 0;

	for (s16 i = 0; i < unk18; ++i) {
		FabricatedUnk1CStruct* thing = &unk1C[i];

		int sVar2 = thing->unk2;
		int sVar4 = thing->unk0;
		int uVar3 = thing->unk4 & 1;

		u8* unkCit    = thing->unkC;
		u8* unk10it   = thing->unk10;
		s16* unk8it   = thing->unk8;
		s16* unk14ptr = thing->unk14;

		TBGCheckData* checkData = gpMapCollisionData->allocCheckData(sVar2);
		for (int j = 0; j < sVar2; ++j) {
			checkData->unk0  = sVar4;
			checkData->unk44 = param_4;

			if (param_3 & 2)
				setCheckData(param_2, unk8it, checkData, 3);
			else
				setCheckData(param_2, unk8it, checkData, 0);

			if (uVar3)
				checkData->unk2 = unk14ptr[j];
			else
				checkData->unk2 = param_1;

			checkData->unk6 = unkCit[j];
			checkData->unk7 = unk10it[j];

			unk8it += 3;

			++checkData;
			++unkC;
		}
	}
}

void TMapCollisionBase::initCheckData(s16 param_1, u16 param_2,
                                      const TLiveActor* param_3)
{
	unk50.x = unk20[0][3];
	unk50.y = unk20[1][3];
	unk50.z = unk20[2][3];

	if (param_2 & 4) {
		initAllCheckData(param_1, &unk14->x, param_2, param_3);
	} else {
		Vec vecs[350]; // TODO: is size correct?
		MTXMultVecArray(unk20, unk14, vecs, unk10);
		initAllCheckData(param_1, &vecs[0].x, param_2, param_3);
	}
}
