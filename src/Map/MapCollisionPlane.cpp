#include <Map/MapCollisionPlane.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TBGCheckData* TMapCheckGroundPlane::getCheckData(int x, int z, int which) const
{
	return &mGrid[(z * mGridHeight + x) * 2 + which];
}

f32 TMapCheckGroundPlane::checkPlaneGround(f32 x, f32 y, f32 z,
                                           const TBGCheckData** result)
{
	if (x < -mExtent || mExtent < x || z < -mExtent || mExtent < z) {
		*result = &TMapCollisionData::mIllegalCheckData;
		return -32767.0f;
	}

	int tileX = worldToGrid(x);
	int tileZ = worldToGrid(z);

	f32 inTileX = x - gridToWorld(tileX);
	f32 inTileZ = z - gridToWorld(tileZ);

	// Grid consists of tiles, each tile is 2 triangles that make a square.
	// Pick the one that the point is in.
	TBGCheckData* res;
	if (mScale - inTileX > inTileZ)
		res = getCheckData(tileX, tileZ, 0);
	else
		res = getCheckData(tileX, tileZ, 1);

	// solve plane equation for unknown Y
	f32 tmp = x * res->getNormal().x + z * res->getNormal().z
	          + res->getPlaneDistance();
	f32 resultY = -tmp / res->getNormal().y;

	*result = res;
	return resultY;
}

void TMapCheckGroundPlane::init(int width, int height, f32 scale)
{
	mGridWidth      = width;
	mGridHeight     = height;
	mScale          = scale;
	mOneOverScale   = 1.0f / mScale;
	mGridWorldWidth = mScale * mGridWidth;
	mExtent         = mGridWorldWidth / 2;
	mGrid           = new TBGCheckData[mGridHeight * mGridWidth * 2];

	gpMapCollisionData->setGroundPlane(this);
}

TMapCheckGroundPlane::TMapCheckGroundPlane()
    : mGridWidth(0)
    , mGridHeight(0)
    , mScale(0.0f)
    , mOneOverScale(0.0f)
    , mGridWorldWidth(0.0f)
    , mExtent(0.0f)
    , mGrid(0)
{
}
