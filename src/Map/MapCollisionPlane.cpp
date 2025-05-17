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
                                           const TBGCheckData** param_4)
{
	if (x < -unk14 || unk14 < x || z < -unk14 || unk14 < z) {
		*param_4 = &TMapCollisionData::mIllegalCheckData;
		return -32767.0f;
	}

	int xInt = (int)(unkC * (x + unk14));
	int zInt = (int)(unkC * (z + unk14));

	f32 xThing = x - (xInt * unk8 - unk14);
	f32 zThing = z - (zInt * unk8 - unk14);

	TBGCheckData* pTVar7;
	if (unk8 - xThing > zThing)
		pTVar7 = getCheckData(xInt, zInt, 0);
	else
		pTVar7 = getCheckData(xInt, zInt, 1);

	f32 tmp    = x * pTVar7->mNormal.x + z * pTVar7->mNormal.z + pTVar7->unk40;
	f32 result = -tmp / pTVar7->mNormal.y;
	*param_4   = pTVar7;
	return result;
}

void TMapCheckGroundPlane::init(int param_1, int param_2, f32 param_3)
{
	unk0  = param_1;
	unk4  = param_2;
	unk8  = param_3;
	unkC  = 1.0f / unk8;
	unk10 = unk8 * unk0;
	unk14 = unk10 * 0.5f;
	unk18 = new TBGCheckData[unk4 * unk0 * 2];

	gpMapCollisionData->unk244 = this;
}

TMapCheckGroundPlane::TMapCheckGroundPlane()
    : unk0(0)
    , unk4(0)
    , unk8(0.0f)
    , unkC(0.0f)
    , unk10(0.0f)
    , unk14(0.0f)
    , unk18(0)
{
}
