#include <Map/PollutionObj.hpp>
#include <Map/PollutionLayer.hpp>
#include <Map/Map.hpp>
#include <Map/MapEventSink.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <types.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static bool is_near(f32 a, f32 b)
{
	if (a - 30.0f <= b && b <= 30.0f + a)
		return true;
	return false;
}

static bool is_near(f32 h00, f32 h01, f32 h10, f32 h11)
{
	if (is_near(h00, h01) && is_near(h01, h10) && is_near(h10, h11))
		return true;
	return false;
}

u8 TPollutionObj::getDepthFromMap(int x, int z)
{
	f32 step   = unk34->unk5C.mVerticalScale;
	f32 dx     = (f32)x * step;
	f32 dz     = (f32)z * step;
	f32 worldX = unk34->unk38 + dx;
	f32 worldZ = unk34->unk40 + dz;
	f32 minX   = worldX - 5.0f;
	f32 maxX   = 5.0f + (worldX + step);
	f32 minZ   = worldZ - 5.0f;
	f32 maxZ   = 5.0f + (worldZ + step);

	const TBGCheckData* tmp;
	f32 h00 = gpMap->checkGround(minX, 9999999.0f, minZ, &tmp);
	f32 h10 = gpMap->checkGround(maxX, 9999999.0f, minZ, &tmp);
	f32 h01 = gpMap->checkGround(minX, 9999999.0f, maxZ, &tmp);
	f32 h11 = gpMap->checkGround(maxX, 9999999.0f, maxZ, &tmp);

	if (is_near(h00, h01, h10, h11)) {
		f32 half = 0.5f * step;
		return unk34->unk5C.worldToDepth(
		    gpMap->checkGround(worldX + half, 9999999.0f, worldZ + half, &tmp));
	}
	return 0xFF;
}

void TPollutionObj::updateDepthMap()
{
	for (int z = unk28; z < unk2C; ++z) {
		for (int x = unk20; x < unk24; ++x) {
			u8 depth = getDepthFromMap(x, z);
			unk34->unk5C.setDepth(x, z, depth);
		}
	}
}

bool TPollutionObj::isCleaned() const
{
	if (unk30 < TMapEventSink::mCleanedDegree)
		return true;
	return false;
}

void TPollutionObj::initAreaInfo(TPollutionLayer* layer)
{
	unk34          = layer;
	const Vec& min = mJoint->getMin();
	const Vec& max = mJoint->getMax();

	unk20 = unk34->getTexPosS(min.x);
	unk24 = unk34->getTexPosS(max.x);
	unk28 = unk34->getTexPosT(min.z);
	unk2C = unk34->getTexPosT(max.z);

	if (unk20 < 0)
		unk24 = 0;
	if (unk28 < 0)
		unk2C = 0;
	if (unk24 > unk34->unk5C.mWidth)
		unk24 = unk34->unk5C.mWidth;
	if (unk2C > unk34->unk5C.mHeight)
		unk2C = unk34->unk5C.mHeight;

	for (int i = 0; i < mChildrenNum; ++i)
		((TPollutionObj*)mChildren[i])->initAreaInfo(layer);
}

TPollutionObj::TPollutionObj()
    : unk20(0)
    , unk24(0)
    , unk28(0)
    , unk2C(0)
    , unk30(0)
    , unk34(nullptr)
{
}
