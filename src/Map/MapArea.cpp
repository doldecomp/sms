#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>

static bool checkLinesCollision(f32 x0, f32 z0, f32 x1, f32 z1, f32 x2, f32 z2,
                                f32 x3, f32 z3)
{
	f32 c0 = (z1 - z0) * (x2 - x1) - (x1 - x0) * (z2 - z1);
	f32 c1 = (z1 - z0) * (x3 - x1) - (x1 - x0) * (z3 - z1);

	if ((c0 >= 0.0f && c1 >= 0.0f) || (c0 < 0.0f && c1 < 0.0f))
		return false;

	f32 d0 = (z3 - z2) * (x0 - x3) - (x3 - x2) * (z0 - z3);
	f32 d1 = (z3 - z2) * (x1 - x3) - (x3 - x2) * (z1 - z3);
	if ((d0 >= 0.0f && d1 >= 0.0f) || (d0 < 0.0f && d1 < 0.0f))
		return false;

	return true;
}

static bool checkLinePolygonCollision(f32 x0, f32 z0, f32 x1, f32 z1,
                                      TBGCheckData* data)
{
	if (checkLinesCollision(x0, z0, x1, z1, data->getPoint1().x,
	                        data->getPoint1().z, data->getPoint2().x,
	                        data->getPoint2().z)
	    || checkLinesCollision(x0, z0, x1, z1, data->getPoint2().x,
	                           data->getPoint2().z, data->getPoint3().x,
	                           data->getPoint3().z)
	    || checkLinesCollision(x0, z0, x1, z1, data->getPoint3().x,
	                           data->getPoint3().z, data->getPoint1().x,
	                           data->getPoint1().z)) {
		return true;
	}

	return false;
}

static bool pointIsInPolygon(f32 x, f32 z, TBGCheckData* data)
{
	if ((data->mPoint1.z - z) * (data->mPoint2.x - data->mPoint1.x)
	        - (data->mPoint1.x - x) * (data->mPoint2.z - data->mPoint1.z)
	    < 0.0f) {
		return false;
	}

	if ((data->mPoint2.z - z) * (data->mPoint3.x - data->mPoint2.x)
	        - (data->mPoint2.x - x) * (data->mPoint3.z - data->mPoint2.z)
	    < 0.0f) {
		return false;
	}

	if ((data->mPoint3.z - z) * (data->mPoint1.x - data->mPoint3.x)
	        - (data->mPoint3.x - x) * (data->mPoint1.z - data->mPoint3.z)
	    < 0.0f) {
		return false;
	}

	return true;
}

static bool pointIsInGrid(f32 x, f32 z, f32 minX, f32 minZ, f32 maxX, f32 maxZ)
{
	if (minX <= x && x <= maxX && minZ <= z && z <= maxZ) {
		return true;
	}

	return false;
}

bool TMapCollisionData::polygonIsInGrid(f32 minX, f32 minZ, f32 maxX, f32 maxZ,
                                        TBGCheckData* data)
{
	if (data->mNormal.y < 0.0f) {
		return true;
	}

	if (pointIsInGrid(data->mPoint1.x, data->mPoint1.z, minX, minZ, maxX, maxZ)
	    || pointIsInGrid(data->mPoint2.x, data->mPoint2.z, minX, minZ, maxX,
	                     maxZ)
	    || pointIsInGrid(data->mPoint3.x, data->mPoint3.z, minX, minZ, maxX,
	                     maxZ)) {
		return true;
	}

	if (pointIsInPolygon(minX, minZ, data) || pointIsInPolygon(maxX, minZ, data)
	    || pointIsInPolygon(minX, maxZ, data)
	    || pointIsInPolygon(maxX, maxZ, data)) {
		return true;
	}

	if (checkLinePolygonCollision(minX, minZ, maxX, minZ, data)) {
		return true;
	}

	if (checkLinePolygonCollision(minX, maxZ, maxX, maxZ, data)) {
		return true;
	}

	if (checkLinePolygonCollision(minX, minZ, minX, maxZ, data)) {
		return true;
	}

	if (checkLinePolygonCollision(maxX, minZ, maxX, maxZ, data)) {
		return true;
	}

	return false;
}
