#include <Map/MapCollisionPlane.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TBGCheckData* TMapCheckGroundPlane::getCheckData(int param_1, int param_2,
                                                 int param_3) const
{
	return &unk18[(param_2 * unk4 + param_1) * 2 + param_3];
}

f32 TMapCheckGroundPlane::checkPlaneGround(f32 x, f32 y, f32 z,
                                           const TBGCheckData** result)
{
	if (x < -mExtent || mExtent < x || z < -mExtent || mExtent < z) {
		*result = &TMapCollisionData::mIllegalCheckData;
		return -32767.0f;
	}

	int xInt = (int)(mScale * (x + mExtent));
	int zInt = (int)(mScale * (z + mExtent));

	f32 xThing = x - (xInt * unk8 - mExtent);
	f32 zThing = z - (zInt * unk8 - mExtent);

	TBGCheckData* res;
	if (unk8 - xThing > zThing)
		res = getCheckData(xInt, zInt, 0);
	else
		res = getCheckData(xInt, zInt, 1);

	f32 tmp  = x * res->mNormal.x + z * res->mNormal.z + res->mPlaneDistance;
	f32 dist = -tmp / res->mNormal.y;
	*result  = res;
	return dist;
}

void TMapCheckGroundPlane::init(int param_1, int param_2, f32 param_3)
{
	unk0    = param_1;
	unk4    = param_2;
	unk8    = param_3;
	mScale  = 1.0f / unk8;
	unk10   = unk8 * unk0;
	mExtent = unk10 * 0.5f;
	unk18   = new TBGCheckData[unk4 * unk0 * 2];

	gpMapCollisionData->unk244 = this;
}

TMapCheckGroundPlane::TMapCheckGroundPlane()
    : unk0(0)
    , unk4(0)
    , unk8(0.0f)
    , mScale(0.0f)
    , unk10(0.0f)
    , mExtent(0.0f)
    , unk18(0)
{
}
