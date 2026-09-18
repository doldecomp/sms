#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>

// `TBGCheckData::getNormal()` already exists in MapData.hpp and returns exactly
// this reference, so there is nothing to promote: this helper is a *second*
// level over that accessor, and its +8 is what the callers below need. Reading
// `d->getNormal()` directly drops the level.
static inline const JGeometry::TVec3<f32>&
MapArea_getNormal(const TBGCheckData* d)
{
	return d->getNormal();
}

// The two cross products are one reused two-element array: that is the only
// spelling that gives retail's 0x38 frame (two separate arrays give 0x40 and
// four scalars 0x30) with the instruction stream unchanged.
static bool checkLinesCollision(f32 x0, f32 z0, f32 x1, f32 z1, f32 x2, f32 z2,
                                f32 x3, f32 z3)
{
	f32 cross[2];

	cross[0] = (z1 - z0) * (x2 - x1) - (x1 - x0) * (z2 - z1);
	cross[1] = (z1 - z0) * (x3 - x1) - (x1 - x0) * (z3 - z1);

	if ((cross[0] >= 0.0f && cross[1] >= 0.0f)
	    || (cross[0] < 0.0f && cross[1] < 0.0f))
		return false;

	cross[0] = (z3 - z2) * (x0 - x3) - (x3 - x2) * (z0 - z3);
	cross[1] = (z3 - z2) * (x1 - x3) - (x3 - x2) * (z1 - z3);

	if ((cross[0] >= 0.0f && cross[1] >= 0.0f)
	    || (cross[0] < 0.0f && cross[1] < 0.0f))
		return false;

	return true;
}

static bool checkLinePolygonCollision(f32 x0, f32 z0, f32 x1, f32 z1,
                                      TBGCheckData* data)
{
	// The polygon's three vertices: retail declared them here and read the
	// components through the accessors, so the slots are reserved and dead.
	// They are 36 of the 288 bytes polygonIsInGrid's pool needs per
	// expansion; see the note above it.
	JGeometry::TVec3<f32> p1;
	JGeometry::TVec3<f32> p2;
	JGeometry::TVec3<f32> p3;

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
	// Reserved and dead, exactly as in checkLinePolygonCollision below.
	JGeometry::TVec3<f32> p1;
	JGeometry::TVec3<f32> p2;
	JGeometry::TVec3<f32> p3;

	if ((data->getPoint1().z - z) * (data->getPoint2().x - data->getPoint1().x)
	        - (data->getPoint1().x - x) * (data->getPoint2().z - data->getPoint1().z)
	    < 0.0f) {
		return false;
	}

	if ((data->getPoint2().z - z) * (data->getPoint3().x - data->getPoint2().x)
	        - (data->getPoint2().x - x) * (data->getPoint3().z - data->getPoint2().z)
	    < 0.0f) {
		return false;
	}

	if ((data->getPoint3().z - z) * (data->getPoint1().x - data->getPoint3().x)
	        - (data->getPoint3().x - x) * (data->getPoint1().z - data->getPoint3().z)
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

// Closed in batch 151. The 288 bytes of dead low region that polygonIsInGrid
// was short are the reserved slots of three `JGeometry::TVec3<f32>` locals in
// each of `pointIsInPolygon` and `checkLinePolygonCollision` -- the polygon's
// three vertices -- which MWCC reserves per expansion (12 bytes each, so 4
// expansions x 36 twice = 288) even though every value stays in a register.
// Both helpers are UNUSED in the map, so they are legal carriers, and both
// stay byte-size-exact (0x9c and 0x27c) with the locals declared: retail's
// bodies read the vertices through the accessors, because spelling the same
// three locals as value copies (`TVec3<f32> p1 = data->getPoint1();`) emits
// the copies and takes the two helpers to 236 and 708 bytes and the caller to
// 44.9%. `checkLinesCollision` cannot hold any of it -- it has a matching
// out-of-line copy (0xd8), and a dead local there breaks it while leaving the
// caller's frame untouched, which also confirms retail `bl`s it.
// Earlier measurements that stand: routing pointIsInPolygon's twelve point
// reads through getPoint1/2/3() is worth 296 bytes and the last instruction
// differences (raw members: 0x78 / 95.7%); the MapArea_getNormal() level is
// worth 8; this body's own point reads are worth 16; a second forwarder level
// above getPointN() is +0; a binding level over getPointN() is +12 per read
// site (the wrong granularity: 192 / 384 / 576); named `const TVec3&` locals
// for the three points regress both helpers; and pointIsInGrid taking the
// point by reference contradicts the map's `pointIsInGrid__Fffffff`.
bool TMapCollisionData::polygonIsInGrid(f32 minX, f32 minZ, f32 maxX, f32 maxZ,
                                        TBGCheckData* data)
{
	if (MapArea_getNormal(data).y < 0.0f) {
		return true;
	}

	if (pointIsInGrid(data->getPoint1().x, data->getPoint1().z, minX, minZ, maxX, maxZ)
	    || pointIsInGrid(data->getPoint2().x, data->getPoint2().z, minX, minZ, maxX,
	                     maxZ)
	    || pointIsInGrid(data->getPoint3().x, data->getPoint3().z, minX, minZ, maxX,
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
