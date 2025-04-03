#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>
#include <Strategic/LiveActor.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <string.h>
#include <stdio.h>

TMapCollisionWarp::TMapCollisionWarp()
    : unk60(0)
    , unk64(0)
{
}

void TMapCollisionWarp::init(const char* param_1, u16 param_2,
                             const TLiveActor* param_3)
{
	TMapCollisionBase::init(param_1, param_2, param_3);
	unk8 = 2;
	TMapCollisionBase::initCheckData(-9999, param_2 | 2, param_3);
	unk5C |= 1;
}

void TMapCollisionWarp::remove()
{
	if (!(checkFlag(1) ? false : true))
		return;

	unk5C |= 1;
	gpMapCollisionData->removeCheckListData(unk60, unk64);
}

void TMapCollisionWarp::setUpTrans(const JGeometry::TVec3<f32>& param_1)
{
	if (!checkFlag(1))
		return;

	unk5C &= ~1;

	unk60 = gpMapCollisionData->getEntryID();
	TMapCollisionBase::updateTrans(param_1);
	unk64 = gpMapCollisionData->unk40 - gpMapCollisionData->unk42[unk60];
}

void TMapCollisionWarp::setUp()
{
	if (!checkFlag(1))
		return;

	unk5C &= ~1;

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

TMapCollisionMove::TMapCollisionMove() { }

void TMapCollisionMove::init(const char* param_1, u16 param_2,
                             const TLiveActor* param_3)
{
	TMapCollisionBase::init(param_1, param_2, param_3);
	unk8 = 1;
	TMapCollisionBase::initCheckData(-9999, param_2 | 2, param_3);
	unk5C |= 1;
}

void TMapCollisionMove::init(u32 param_1, u16 param_2, short param_3,
                             const TLiveActor* param_4)
{
	unk8 = 1;
	unkC = param_1;
	unk4 = gpMapCollisionData->allocCheckData(unkC);
	for (int i = 0; i < unkC; ++i) {
		unk4[i].unk0  = param_2;
		unk4[i].unk2  = param_3;
		unk4[i].unk44 = param_4;
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

TMapCollisionStatic::TMapCollisionStatic()
    : unk60(nullptr)
{
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
		unk5C |= 1;
	}
}

void TMapCollisionStatic::setUp()
{
	if (unk4)
		return;

	unk5C &= ~1;

	u16 thing = 0;
	if (unk10 >= 0x15E)
		thing |= 4;
	TMapCollisionBase::initCheckData(-9999, thing, unk60);
}

void TMapCollisionBase::init(const char* param_1, u16 param_2,
                             const TLiveActor* param_3)
{
	char buffer[256];

	const char* extensionIdx = strstr(param_1, ".col");
	if (extensionIdx == nullptr) {
		if (*param_1 == '/')
			sprintf(buffer, "/scene%s.col", param_1);
		else
			sprintf(buffer, "/scene/%s.col", param_1);
		param_1 = buffer;
	} else {
		if (*param_1 != '/')
			sprintf(buffer, "/scene/%s", param_1);
	}

	u8* res = (u8*)JKRGetResource(param_1);
	unk10   = *(u32*)res;
	unk14   = (Vec*)(res + *(u32*)(res + 0x4));
	unk18   = *(u32*)(res + 0x8);
	unk1C   = (void*)(res + *(u32*)(res + 0xC));
	if (param_2 & 0x8000)
		unk5C |= 0x8000;

	// TODO: what is unk1C?
}

void TMapCollisionBase::setAllActor(const TLiveActor* param_1)
{
	for (int i = 0; i < unkC; ++i)
		unk4[i].unk44 = param_1;
}

void TMapCollisionBase::setAllData(s16 param_1)
{
	for (int i = 0; i < unkC; ++i)
		unk4[i].unk2 = param_1;
}

void TMapCollisionBase::setAllBGType(u16 param_1)
{
	for (int i = 0; i < unkC; ++i)
		unk4[i].unk0 = param_1;
}

void loadCollisionData(const char*) { }

void TMapCollisionBase::setVertexData(u32 param_1,
                                      const JGeometry::TVec3<f32>& param_2,
                                      const JGeometry::TVec3<f32>& param_3,
                                      const JGeometry::TVec3<f32>& param_4)
{
	unk4[param_1].setVertex(param_2, param_3, param_4);
	gpMapCollisionData->addCheckDataToGrid(&unk4[param_1], unk8);
}
