#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <macros.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TBGCheckData TMapCollisionData::mIllegalCheckData;
TMapCollisionData* gpMapCollisionData;

TBGCheckList::TBGCheckList()
    : mNext(nullptr)
    , unk8(0)
{
}

// TODO: inline deferred or nah?
TBGCheckListWarp::TBGCheckListWarp()
    : unkC(0)
    , unk10(0)
    , unk12(0)
{
}

void TMapCollisionData::initGrid(TBGCheckListRoot* roots)
{
	int x = unk10;
	while (x--) {
		roots->unk0[0].setNext(nullptr);
		roots->unk0[1].setNext(nullptr);
		roots->unk0[2].setNext(nullptr);
		roots++;
	}
}

void TMapCollisionData::initMoveCollision()
{
	unk3C = unk20 - 1;
	memset(unk18, 0, unk10 * sizeof(*unk18));
}

void TMapCollisionData::initAllCheckDataAndList() { }

void TMapCollisionData::init(JSUMemoryInputStream& stream)
{
	int value;
	stream.read(&value, 4);
	unk8 = value;
	stream.read(&value, 4);
	unkC = value;
	stream.read(&value, 4);
	unk1C = value;
	stream.read(&value, 4);
	unk20 = value;
	stream.read(&value, 4);
	unk24 = value;

	unk0 = (unk8 / 2) * 1024.0f;
	unk4 = (unkC / 2) * 1024.0f;

	unk10 = unk8 * unkC;

	unk14 = new TBGCheckListRoot[unk10];
	unk18 = new TBGCheckListRoot[unk10];
	unk28 = new TBGCheckData[unk1C];
	unk2C = new TBGCheckList[unk20];
	unk30 = new TBGCheckListWarp[unk24];

	initGrid(unk14);

	unk34 = 0;
	unk38 = 0;
	unk40 = 0;

	initMoveCollision();

	for (int i = 0; i < 256; ++i)
		unk42[i] = 9999;
}

TMapCollisionData::TMapCollisionData()
    : unk0(0.0f)
    , unk4(0.0f)
    , unk8(0)
    , unkC(0)
    , unk10(0)
    , unk14(nullptr)
    , unk18(nullptr)
    , unk1C(0)
    , unk20(0)
    , unk24(0)
    , unk28(nullptr)
    , unk2C(nullptr)
    , unk30(nullptr)
    , unk34(0)
    , unk38(0)
    , unk3C(0)
    , unk40(0)
    , unk242(0)
    , unk244(nullptr)
{
	JGeometry::TVec3<float> v1(-32767.0f, -32767.0f, -32767.0f);
	JGeometry::TVec3<float> v2(-32767.0f, -32767.0f, 32767.0f);
	JGeometry::TVec3<float> v3(32767.0f, -32767.0f, 32767.0f);
	mIllegalCheckData.setVertex(v1, v2, v3);
	mIllegalCheckData.unk0 = 0;
	mIllegalCheckData.unk4 |= 0x10;
	gpMapCollisionData = this;
}
