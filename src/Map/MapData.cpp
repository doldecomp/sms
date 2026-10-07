#include <Map/MapData.hpp>
#include <MoveBG/MapObjTree.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

f32 TBGCheckData::getActiveJumpPower() const
{
	// TODO: inlines...
	char trash[0x4];
	if (getActor() != nullptr
	    && getActor()->isActorType(ACTOR_TYPE_BANANA_TREE))
		return TMapObjTree::mBananaTreeJumpPower;

	return mData;
}

u32 TBGCheckData::getPlaneType()
{
	if (isEverythingButMapObjectsThrough())
		return 0;

	if (mNormal.y > 0.2f)
		return 0;

	if (mNormal.y < -0.2f)
		return 1;

	if (mNormal.x < -0.707f || 0.707f < mNormal.x)
		mFlags |= BG_CHECK_FLAG_X_FACING;
	else
		mFlags &= ~BG_CHECK_FLAG_X_FACING;

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
