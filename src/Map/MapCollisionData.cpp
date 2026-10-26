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
    , mData(nullptr)
{
}

TBGCheckListWarp::TBGCheckListWarp()
    : unkC(nullptr)
    , unk10(0)
    , unk12(0)
{
}

void TMapCollisionData::initGrid(TBGCheckListRoot* roots)
{
	int x = mGridCellNum;
	while (x--) {
		roots->unk0[BG_PLANE_TYPE_GROUND].setNext(nullptr);
		roots->unk0[BG_PLANE_TYPE_ROOF].setNext(nullptr);
		roots->unk0[BG_PLANE_TYPE_WALL].setNext(nullptr);
		roots++;
	}
}

void TMapCollisionData::initMoveCollision()
{
	mMoveListTop = unk20 - 1;
	memset(mMoveGrid, 0, mGridCellNum * sizeof(*mMoveGrid));
}

void TMapCollisionData::initAllCheckDataAndList()
{
	initGrid(mStaticGrid);

	unk34           = 0;
	mStaticListUsed = 0;
	unk40           = 0;

	initMoveCollision();

	for (int i = 0; i < 256; ++i)
		unk42[i] = 9999;
}

void TMapCollisionData::init(JSUMemoryInputStream& stream)
{
	s32 value;
	stream >> value;
	mGridSizeX = value;
	stream >> value;
	mGridSizeZ = value;
	stream >> value;
	unk1C = value;
	stream >> value;
	unk20 = value;
	stream >> value;
	unk24 = value;

	mGridExtentX = (mGridSizeX / 2) * 1024.0f;
	mGridExtentY = (mGridSizeZ / 2) * 1024.0f;

	mGridCellNum = mGridSizeX * mGridSizeZ;

	mStaticGrid        = new TBGCheckListRoot[mGridCellNum];
	mMoveGrid          = new TBGCheckListRoot[mGridCellNum];
	unk28              = new TBGCheckData[unk1C];
	mCheckListPool     = new TBGCheckList[unk20];
	mWarpCheckListPool = new TBGCheckListWarp[unk24];

	initAllCheckDataAndList();
}

TMapCollisionData::TMapCollisionData()
    : mGridExtentX(0.0f)
    , mGridExtentY(0.0f)
    , mGridSizeX(0)
    , mGridSizeZ(0)
    , mGridCellNum(0)
    , mStaticGrid(nullptr)
    , mMoveGrid(nullptr)
    , unk1C(0)
    , unk20(0)
    , unk24(0)
    , unk28(nullptr)
    , mCheckListPool(nullptr)
    , mWarpCheckListPool(nullptr)
    , unk34(0)
    , mStaticListUsed(0)
    , mMoveListTop(0)
    , unk40(0)
    , unk242(0)
    , mGroundPlane(nullptr)
{
	JGeometry::TVec3<f32> v1(-32767.0f, -32767.0f, -32767.0f);
	JGeometry::TVec3<f32> v2(-32767.0f, -32767.0f, 32767.0f);
	JGeometry::TVec3<f32> v3(32767.0f, -32767.0f, 32767.0f);
	mIllegalCheckData.setVertex(v1, v2, v3);
	mIllegalCheckData.mBGType = 0;
	mIllegalCheckData.mFlags |= BG_CHECK_FLAG_ILLEGAL;
	gpMapCollisionData = this;
}
