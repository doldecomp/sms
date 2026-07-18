
static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>
#include <Strategic/LiveActor.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <string.h>
#include <stdio.h>

void TMapCollisionBase::setVertexData(u32 param_1,
                                      const JGeometry::TVec3<f32>& param_2,
                                      const JGeometry::TVec3<f32>& param_3,
                                      const JGeometry::TVec3<f32>& param_4)
{
	mCheckDatas[param_1].setVertex(param_2, param_3, param_4);
	gpMapCollisionData->addCheckDataToGrid(&mCheckDatas[param_1], getUnk8());
}

static void* loadCollisionData(const char* param_1)
{
	char buffer[256];

	const char* extensionIdx = strstr(param_1, ".col");
	if (extensionIdx == nullptr) {
		if (*param_1 != '/')
			sprintf(buffer, "/scene/%s.col", param_1);
		else
			sprintf(buffer, "/scene%s.col", param_1);
		param_1 = buffer;
	} else {
		if (*param_1 != '/')
			sprintf(buffer, "/scene/%s", param_1);
	}

	return JKRGetResource(param_1);
}

void TMapCollisionBase::setAllBGType(u16 param_1)
{
	for (int i = 0; i < mCheckDataNum; ++i)
		mCheckDatas[i].mBGType = param_1;
}

void TMapCollisionBase::setAllData(s16 param_1)
{
	for (int i = 0; i < mCheckDataNum; ++i)
		mCheckDatas[i].mData = param_1;
}

void TMapCollisionBase::setAllActor(const TLiveActor* param_1)
{
	for (int i = 0; i < mCheckDataNum; ++i)
		mCheckDatas[i].mActor = param_1;
}

// fabricated
struct CollisionDataHeader {
	/* 0x0 */ u32 unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8;
	/* 0xC */ u32 unkC;
};

void TMapCollisionBase::init(const char* path, u16 param_2,
                             const TLiveActor* param_3)
{
	CollisionDataHeader* hdr = (CollisionDataHeader*)loadCollisionData(path);

	mVertexNum         = hdr->unk0;
	mVertices          = (Vec*)((u8*)hdr + hdr->unk4);
	mCollisionGroupNum = hdr->unk8;
	mCollisionGroups   = (TMapCollisionGroup*)((u8*)hdr + hdr->unkC);

	if (param_2 & 0x8000)
		onFlag(FLAG_UNK8000);

	if (!(mCollisionGroups->mFlags & WAS_PATCHED)) {
		for (s16 i = 0; i < mCollisionGroupNum; ++i) {
			mCollisionGroups[i].mIndices
			    = (s16*)((int)mCollisionGroups[i].mIndices + (u8*)hdr);
			mCollisionGroups[i].unkC
			    = (u8*)((int)mCollisionGroups[i].unkC + (u8*)hdr);
			mCollisionGroups[i].unk10
			    = (u8*)((int)mCollisionGroups[i].unk10 + (u8*)hdr);

			if (mCollisionGroups[i].mAdditionalDatas)
				mCollisionGroups[i].mAdditionalDatas
				    = (s16*)((int)mCollisionGroups[i].mAdditionalDatas
				             + (u8*)hdr);

			mCollisionGroups[i].mFlags |= WAS_PATCHED;
		}
	}
}

void TMapCollisionStatic::setUp()
{
	if (mCheckDatas)
		return;

	offFlag(FLAG_NEEDS_SETUP);

	u16 thing = 0;
	if (mVertexNum >= 350)
		thing |= 4;
	TMapCollisionBase::initCheckData(-9999, thing, mOwnerActor);
}

void TMapCollisionStatic::init(const char* param_1, u16 param_2,
                               const TLiveActor* param_3)
{
	TMapCollisionBase::init(param_1, param_2, param_3);
	mKind = KIND_STATIC;
	if (!(param_2 & 2)) {
		initCheckData(-9999, param_2, param_3);
	} else {
		mOwnerActor = param_3;
		onFlag(FLAG_NEEDS_SETUP);
	}
}

TMapCollisionStatic::TMapCollisionStatic()
    : mOwnerActor(nullptr)
{
}

void TMapCollisionMove::move()
{
	if (checkFlag(FLAG_NEEDS_SETUP))
		return;

	if (checkFlag(FLAG_UNK4000)) {
		setList();
		return;
	}

	if (checkFlag(FLAG_UNK8000)) {
		JGeometry::TVec3<f32> local_18;
		local_18.x = unk20[0][3];
		local_18.y = unk20[1][3];
		local_18.z = unk20[2][3];
		TMapCollisionBase::updateTrans(local_18);
	} else {
		TMapCollisionBase::update();
	}
}

void TMapCollisionMove::moveTrans(const JGeometry::TVec3<f32>& param_1)
{
	if (checkFlag(FLAG_NEEDS_SETUP))
		return;

	if (checkFlag(FLAG_UNK4000)) {
		setList();
		return;
	}

	TMapCollisionBase::updateTrans(param_1);
}

void TMapCollisionMove::init(u32 param_1, u16 bg_type, s16 data,
                             const TLiveActor* actor)
{
	mKind         = 1;
	mCheckDataNum = param_1;
	mCheckDatas   = gpMapCollisionData->allocCheckData(getUnkC());
	for (int i = 0; i < getUnkC(); ++i) {
		mCheckDatas[i].mBGType = bg_type;
		mCheckDatas[i].mData   = data;
		mCheckDatas[i].mActor  = actor;
	}
}

void TMapCollisionMove::init(const char* param_1, u16 param_2,
                             const TLiveActor* param_3)
{
	TMapCollisionBase::init(param_1, param_2, param_3);
	mKind = 1;
	TMapCollisionBase::initCheckData(-9999, param_2 | 2, param_3);
	onFlag(FLAG_NEEDS_SETUP);
}

TMapCollisionMove::TMapCollisionMove() { }

void TMapCollisionWarp::setUp()
{
	if (!checkFlag(FLAG_NEEDS_SETUP))
		return;

	offFlag(FLAG_NEEDS_SETUP);

	mEntryId = gpMapCollisionData->getEntryID();

	if (checkFlag(FLAG_UNK8000)) {
		JGeometry::TVec3<f32> local_18(unk20[0][3], unk20[1][3], unk20[2][3]);
		TMapCollisionBase::updateTrans(local_18);
	} else {
		TMapCollisionBase::update();
	}

	mEntrySize = gpMapCollisionData->getEntrySize(mEntryId);
}

void TMapCollisionWarp::setUpTrans(const JGeometry::TVec3<f32>& param_1)
{
	if (!checkFlag(FLAG_NEEDS_SETUP))
		return;

	offFlag(FLAG_NEEDS_SETUP);

	mEntryId = gpMapCollisionData->getEntryID();
	TMapCollisionBase::updateTrans(param_1);
	mEntrySize = gpMapCollisionData->getEntrySize(mEntryId);
}

void TMapCollisionWarp::remove()
{
	if (!isSetUp())
		return;

	onFlag(FLAG_NEEDS_SETUP);
	gpMapCollisionData->removeCheckListData(mEntryId, mEntrySize);
}

void TMapCollisionWarp::init(const char* param_1, u16 param_2,
                             const TLiveActor* param_3)
{
	TMapCollisionBase::init(param_1, param_2, param_3);
	mKind = 2;
	TMapCollisionBase::initCheckData(-9999, param_2 | 2, param_3);
	onFlag(FLAG_NEEDS_SETUP);
}

TMapCollisionWarp::TMapCollisionWarp()
    : mEntryId(0)
    , mEntrySize(0)
{
}
