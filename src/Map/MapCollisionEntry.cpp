
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
	unk4[param_1].setVertex(param_2, param_3, param_4);
	gpMapCollisionData->addCheckDataToGrid(&unk4[param_1], getUnk8());
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
	for (int i = 0; i < unkC; ++i)
		unk4[i].unk0 = param_1;
}

void TMapCollisionBase::setAllData(s16 param_1)
{
	for (int i = 0; i < unkC; ++i)
		unk4[i].unk2 = param_1;
}

void TMapCollisionBase::setAllActor(const TLiveActor* param_1)
{
	for (int i = 0; i < unkC; ++i)
		unk4[i].unk44 = param_1;
}

// fabricated
struct CollisionDataHeader {
	/* 0x0 */ u32 unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8;
	/* 0xC */ u32 unkC;
};

void TMapCollisionBase::init(const char* param_1, u16 param_2,
                             const TLiveActor* param_3)
{
	CollisionDataHeader* hdr = (CollisionDataHeader*)loadCollisionData(param_1);

	unk10 = hdr->unk0;
	unk14 = (Vec*)((u8*)hdr + hdr->unk4);
	unk18 = hdr->unk8;
	unk1C = (FabricatedUnk1CStruct*)((u8*)hdr + hdr->unkC);

	if (param_2 & 0x8000)
		unk5C |= 0x8000;

	if (!(unk1C->unk4 & 0x8000)) {
		for (s16 i = 0; i < unk18; ++i) {
			unk1C[i].unk8  = (s16*)((int)unk1C[i].unk8 + (u8*)hdr);
			unk1C[i].unkC  = (u8*)((int)unk1C[i].unkC + (u8*)hdr);
			unk1C[i].unk10 = (u8*)((int)unk1C[i].unk10 + (u8*)hdr);

			if (unk1C[i].unk14)
				unk1C[i].unk14 = (s16*)((int)unk1C[i].unk14 + (u8*)hdr);

			unk1C[i].unk4 |= 0x8000;
		}
	}
}

void TMapCollisionStatic::setUp()
{
	if (unk4)
		return;

	offFlag(1);

	u16 thing = 0;
	if (unk10 >= 0x15E)
		thing |= 4;
	TMapCollisionBase::initCheckData(-9999, thing, unk60);
}

void TMapCollisionStatic::init(const char* param_1, u16 param_2,
                               const TLiveActor* param_3)
{
	TMapCollisionBase::init(param_1, param_2, param_3);
	unk8 = 0;
	if (!(param_2 & 2)) {
		initCheckData(-9999, param_2, param_3);
	} else {
		unk60 = param_3;
		onFlag(1);
	}
}

TMapCollisionStatic::TMapCollisionStatic()
    : unk60(nullptr)
{
}

void TMapCollisionMove::move()
{
	if (checkFlag(1))
		return;

	if (checkFlag(0x4000)) {
		setList();
	} else {
		if (checkFlag(0x8000)) {
			JGeometry::TVec3<f32> local_18;
			local_18.x = unk20[0][3];
			local_18.y = unk20[1][3];
			local_18.z = unk20[2][3];
			TMapCollisionBase::updateTrans(local_18);
		} else {
			TMapCollisionBase::update();
		}
	}
}

void TMapCollisionMove::moveTrans(const JGeometry::TVec3<f32>& param_1)
{
	if (checkFlag(0x1))
		return;

	if (checkFlag(0x4000))
		setList();
	else
		TMapCollisionBase::updateTrans(param_1);
}

void TMapCollisionMove::init(u32 param_1, u16 param_2, short param_3,
                             const TLiveActor* param_4)
{
	unk8 = 1;
	unkC = param_1;
	unk4 = gpMapCollisionData->allocCheckData(getUnkC());
	for (int i = 0; i < getUnkC(); ++i) {
		unk4[i].unk0  = param_2;
		unk4[i].unk2  = param_3;
		unk4[i].unk44 = param_4;
	}
}

void TMapCollisionMove::init(const char* param_1, u16 param_2,
                             const TLiveActor* param_3)
{
	TMapCollisionBase::init(param_1, param_2, param_3);
	unk8 = 1;
	TMapCollisionBase::initCheckData(-9999, param_2 | 2, param_3);
	onFlag(1);
}

TMapCollisionMove::TMapCollisionMove() { }

void TMapCollisionWarp::setUp()
{
	if (!checkFlag(1))
		return;

	offFlag(1);

	unk60 = gpMapCollisionData->getEntryID();
	if (checkFlag(0x8000)) {
		JGeometry::TVec3<f32> local_18;
		local_18.set(unk20[0][3], unk20[1][3], unk20[2][3]);
		TMapCollisionBase::updateTrans(local_18);
	} else {
		TMapCollisionBase::update();
	}

	unk64 = gpMapCollisionData->unk40 - gpMapCollisionData->unk42[unk60];
}

void TMapCollisionWarp::setUpTrans(const JGeometry::TVec3<f32>& param_1)
{
	if (!checkFlag(1))
		return;

	offFlag(1);

	unk60 = gpMapCollisionData->getEntryID();
	TMapCollisionBase::updateTrans(param_1);
	unk64 = gpMapCollisionData->getEntryRelatedThing(unk60);
}

void TMapCollisionWarp::remove()
{
	if (!(checkFlag(1) ? false : true))
		return;

	onFlag(1);
	gpMapCollisionData->removeCheckListData(unk60, unk64);
}

void TMapCollisionWarp::init(const char* param_1, u16 param_2,
                             const TLiveActor* param_3)
{
	TMapCollisionBase::init(param_1, param_2, param_3);
	unk8 = 2;
	TMapCollisionBase::initCheckData(-9999, param_2 | 2, param_3);
	onFlag(1);
}

TMapCollisionWarp::TMapCollisionWarp()
    : unk60(0)
    , unk64(0)
{
}
