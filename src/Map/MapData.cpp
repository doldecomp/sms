#include <Map/MapData.hpp>
#include <MoveBG/MapObjTree.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TBGCheckData::TBGCheckData()
    : unk0(0)
    , unk2(0)
    , unk4(0)
    , mMinY(0.0f)
    , mMaxY(0.0f)
    , unk40(0.0f)
    , unk44(nullptr)
{
	mPoint1.zero();
	mPoint2.zero();
	mPoint3.zero();
	mNormal.zero();
}

u32 TBGCheckData::getPlaneType()
{
	if (unk0 == 0x801 ? true : false)
		return 0;

	if (mNormal.y > 0.2f)
		return 0;

	if (mNormal.y < -0.2f)
		return 1;

	if (mNormal.x < -0.707f || 0.707f < mNormal.x)
		unk4 |= 0x8;
	else
		unk4 &= ~0x8;

	return 2;
}

f32 TBGCheckData::getActiveJumpPower() const
{
	// TODO: inlines...
	char trash[0x4];
	if (getUnk44() != nullptr && getUnk44()->isActorType(0x40000039))
		return TMapObjTree::mBananaTreeJumpPower;

	return unk2;
}
