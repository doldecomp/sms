#include <Map/MapData.hpp>
#include <MoveBG/MapObjTree.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TBGCheckData::TBGCheckData()
    : unk0(0)
    , unk2(0)
    , unk4(0)
    , unk8(0.0f)
    , unkC(0.0f)
    , unk40(0.0f)
    , unk44(nullptr)
{
	unk10.x = unk10.y = unk10.z = 0.0f;
	unk1C.x = unk1C.y = unk1C.z = 0.0f;
	unk28.x = unk28.y = unk28.z = 0.0f;
	unk34.x = unk34.y = unk34.z = 0.0f;
}

u32 TBGCheckData::getPlaneType()
{
	if (unk0 == 0x801 ? true : false)
		return 0;

	if (unk34.y > 0.2f)
		return 0;

	if (unk34.y < -0.2f)
		return 1;

	if (unk34.x < -0.707f || 0.707f < unk34.x)
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
