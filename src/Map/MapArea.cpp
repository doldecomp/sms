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

// TODO: 99.9%, zero instruction differences; retail's frame is 0x2c0 and ours
// 0x1a0 (288 bytes of dead low region left). Measured levers, all with the
// instruction stream unchanged or improved: routing pointIsInPolygon's twelve
// point reads through getPoint1/2/3() is worth 296 bytes *and* the last
// instruction differences (raw members: 0x78 / 95.7%); the fabricated
// getNormal() level is worth 8; this body's own point reads are worth 16.
// Saturated / rejected: a second (TU-static) forwarder level above
// getPointN() is +0; raw members in checkLinePolygonCollision -16 and 87.4%;
// named `const TVec3&` locals for the three points regress both helpers
// (95.4% / 86.6%); pointIsInGrid taking the point by reference contradicts the
// map's `pointIsInGrid__Fffffff` and gives 94.0%. The remaining 288 bytes
// need a lever family we have not identified - note that retail references no
// stack slot at all below 0x280.
//
// Batch 131 pinned the three helper bodies and priced the batch-127 lever:
//   * all three UNUSED sizes are **exact** (pointIsInGrid 0x40,
//     pointIsInPolygon 0x9c, checkLinePolygonCollision 0x27c), so no body is
//     wrong. Re-measured alternatives for checkLinePolygonCollision, by size:
//     three separate `if`s 0x28c, `return a || b || c;` 0x1b8, named
//     `const TVec3&` points 0x280 / 0x290 / 0x1c8 -- only the committed
//     short-circuit-into-one-`if` form is 0x27c.
//   * a binding level over `getPointN()` (either `const TVec3*` or
//     `const TVec3&` bound and returned -- the two are codegen-identical) is
//     **+12 per read site**, not +8: all twelve reads in
//     checkLinePolygonCollision are +0x240 = 576 across its four expansions,
//     i.e. exactly twice the 288 wanted, and per-member granularity only
//     offers 192 / 384 / 576. The same level on pointIsInPolygon's twelve
//     reads is +0x360 (+18 per site) and adds 58 differing rows; a level that
//     binds the `f32` component instead costs 14 instructions.
//   So 288 is 24 read sites at +12, and no subset of one helper's reads has
//   that cardinality -- the lever is still the wrong family.
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
