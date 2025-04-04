#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <macros.h>

// rouge include needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>

TBGCheckData TMapCollisionData::mIllegalCheckData;
TMapCollisionData* gpMapCollisionData;

TBGCheckList::TBGCheckList()
    : unk4(nullptr)
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

void TMapCollisionData::initGrid(TBGCheckListRoot*) { }

void TMapCollisionData::initMoveCollision()
{
	unk3C = unk20 - 1;
	memset(unk18, 0, unk10 * sizeof(*unk18));
}

void TMapCollisionData::initAllCheckDataAndList() { }

void TMapCollisionData::init(JSUMemoryInputStream& stream)
{
	unk8  = stream.readU32();
	unkC  = stream.readU32();
	unk1C = stream.readU32();
	unk20 = stream.readU32();
	unk24 = stream.readU32();

	unk0 = (unk8 / 2) * 1024.0f;
	unk4 = (unkC / 2) * 1024.0f;

	unk10 = unk8 * unkC;

	unk14 = new TBGCheckListRoot[unk10];
	unk18 = new TBGCheckListRoot[unk10];
	unk28 = new TBGCheckData[unk1C];
	unk2C = new TBGCheckList[unk20];
	unk30 = new TBGCheckListWarp[unk24];

	TBGCheckListRoot* roots = unk14;
	for (int i = 0; i < unk10; ++i) {
		roots[i].unk0[0].unk4 = 0;
		roots[i].unk0[1].unk4 = 0;
		roots[i].unk0[2].unk4 = 0;
	}

	unk34 = 0;
	unk38 = 0;
	unk40 = 0;
	unk3C = unk20 - 1;
	memset(unk18, 0, unk10 * sizeof(*unk18));

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
