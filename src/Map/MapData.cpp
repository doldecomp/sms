#include <Map/MapData.hpp>
#include <MoveBG/MapObjTree.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

f32 TBGCheckData::getActiveJumpPower() const
{
	// TODO: inlines...
	char trash[0x4];
	if (getActor() != nullptr && getActor()->isActorType(0x40000039))
		return TMapObjTree::mBananaTreeJumpPower;

	return mData;
}

u32 TBGCheckData::getPlaneType()
{
	if (mBGType == 0x801 ? true : false)
		return 0;

	if (mNormal.y > 0.2f)
		return 0;

	if (mNormal.y < -0.2f)
		return 1;

	if (mNormal.x < -0.707f || 0.707f < mNormal.x)
		mFlags |= 0x8;
	else
		mFlags &= ~0x8;

	return 2;
}

TBGCheckData::TBGCheckData()
    : mBGType(0)
    , mData(0)
    , mFlags(0)
    , mMinY(0.0f)
    , mMaxY(0.0f)
    , mPlaneDistance(0.0f)
    , mActor(nullptr)
{
	mPoint1.zero();
	mPoint2.zero();
	mPoint3.zero();
	mNormal.zero();
}
