#include <Map/MapCollisionData.hpp>
#include <Map/MapCollisionPlane.hpp>
#include <Map/MapData.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// fabricated
static inline f32 skewProduct(f32 x1, f32 y1, f32 x2, f32 y2)
{
	return x1 * y2 - y1 * x2;
}

// either this or have to use gotos
inline static bool someUnknownInline(TBGCheckData* r31, TBGWallCheckRecord* r29)
{
	f32 radius = r29->mRadius;

	u32 flags = r29->mFlags;

	if ((flags & TBGWallCheckRecord::IGNORE_WATER_THROUGH)
	    && r31->isWaterThrough())
		return false;

	if ((flags & TBGWallCheckRecord::IGNORE_WATER_SURFACE)
	    && r31->isWaterSurface())
		return false;

	f32 cx = r29->mCenter.x;
	f32 cy = r29->mCenter.y;
	f32 cz = r29->mCenter.z;

	f32 nx = r31->getNormal().x;
	f32 ny = r31->getNormal().y;
	f32 nz = r31->getNormal().z;
	f32 d  = r31->getPlaneDistance();

	f32 signedDist = d + (cx * nx + cy * ny + cz * nz);

	if (signedDist < -radius || radius < signedDist)
		return false;

	f32 y1 = r31->getPoint1().y;
	f32 y2 = r31->getPoint2().y;
	f32 y3 = r31->getPoint3().y;

	if (r31->checkFlag(0x8)) {
		if (nx > 0.0f) {
			cz = -cz;

			f32 z1 = -r31->getPoint1().z;
			f32 z2 = -r31->getPoint2().z;
			f32 z3 = -r31->getPoint3().z;

			if (skewProduct(y1 - cy, z1 - cz, y2 - y1, z2 - z1) > 1.0f)
				return false;

			if (skewProduct(y2 - cy, z2 - cz, y3 - y2, z3 - z2) > 1.0f)
				return false;

			if (skewProduct(y3 - cy, z3 - cz, y1 - y3, z1 - z3) > 1.0f)
				return false;
		} else {
			cz = -cz;

			f32 z1 = -r31->getPoint1().z;
			f32 z2 = -r31->getPoint2().z;
			f32 z3 = -r31->getPoint3().z;

			if (skewProduct(y1 - cy, z1 - cz, y2 - y1, z2 - z1) < -1.0f)
				return false;

			if (skewProduct(y2 - cy, z2 - cz, y3 - y2, z3 - z2) < -1.0f)
				return false;

			if (skewProduct(y3 - cy, z3 - cz, y1 - y3, z1 - z3) < -1.0f)
				return false;
		}
	} else {
		if (nz > 0.0f) {
			f32 x1 = r31->getPoint1().x;
			f32 x2 = r31->getPoint2().x;
			f32 x3 = r31->getPoint3().x;

			if (skewProduct(y1 - cy, x1 - cx, y2 - y1, x2 - x1) > 1.0f)
				return false;

			if (skewProduct(y2 - cy, x2 - cx, y3 - y2, x3 - x2) > 1.0f)
				return false;

			if (skewProduct(y3 - cy, x3 - cx, y1 - y3, x1 - x3) > 1.0f)
				return false;
		} else {
			f32 x1 = r31->getPoint1().x;
			f32 x2 = r31->getPoint2().x;
			f32 x3 = r31->getPoint3().x;

			if (skewProduct(y1 - cy, x1 - cx, y2 - y1, x2 - x1) < -1.0f)
				return false;

			if (skewProduct(y2 - cy, x2 - cx, y3 - y2, x3 - x2) < -1.0f)
				return false;

			if (skewProduct(y3 - cy, x3 - cx, y1 - y3, x1 - x3) < -1.0f)
				return false;
		}
	}

	if (!(flags & TBGWallCheckRecord::DONT_MOVE_XZ)) {
		f32 d = radius - signedDist;
		r29->mCenter.x += d * r31->getNormal().x;
		r29->mCenter.z += d * r31->getNormal().z;
	}

	return true;
}

// TODO: 97.4%. Retail's frame is 0xe0 larger (0x310 against 0x230) with no
// extra stack traffic, so the fabricated someUnknownInline/skewProduct shape
// is missing inline levels; the volatile FPR numbering of cx/cy/cz and the
// early `mr r3, r31` before the first mMinY compare follow from that.
// A named result in skewProduct is +0x30 and inert otherwise.
int TMapCollisionData::checkWallList(const TBGCheckList* param_1,
                                     TBGWallCheckRecord* param_2)
{
	if (!param_1)
		return 0;

	f32 f27 = param_2->mCenter.y;
	// param_1: r28
	// param_2: r29

	TBGCheckData* r31;
	int r30 = 0;
	while (param_1) {
		r31 = param_1->unk8;
		param_1           = param_1->mNext;

		if (r31->mMinY > f27)
			continue;

		if (r31->mMaxY < f27)
			return r30;

		if (!someUnknownInline(r31, param_2))
			continue;

		++r30;
		param_2->mResultWalls[param_2->mResultWallsNum] = r31;
		++param_2->mResultWallsNum;
		if (param_2->mResultWallsNum >= param_2->mMaxResults)
			return r30;
	}

	return r30;
}

int TMapCollisionData::checkWalls(TBGWallCheckRecord* param_1) const
{
	param_1->mResultWallsNum = 0;
	if (param_1->mCenter.x < -mGridExtentX || mGridExtentX <= param_1->mCenter.x
	    || param_1->mCenter.z < -mGridExtentY
	    || mGridExtentY <= param_1->mCenter.z)
		return 0;

	int gridX = (param_1->mCenter.x + mGridExtentX) * (1.0f / 1024);
	int gridZ = (param_1->mCenter.z + mGridExtentY) * (1.0f / 1024);

	int iVar6
	    = checkWallList(getGridRoot18(gridX, gridZ).getWallList(), param_1);

	if (iVar6 >= param_1->mMaxResults)
		return iVar6;

	iVar6 += checkWallList(getGridRoot14(gridX, gridZ).getWallList(), param_1);
	return iVar6;
}

// Edge test shared by the roof and ground lists. The four named corner
// components give retail's 0x70 roof frame; declaring the z components first
// gives its FPR pairing.
// TODO: retail still loads a.x into f31 and evaluates (ax - x) first, and
// keeps the list entry in r3 (flags in r6/r7); ground's frame is 8 short.
static inline f32 MapCheckEdge(const JGeometry::TVec3<f32>& a,
                               const JGeometry::TVec3<f32>& b, f32 x, f32 z)
{
	f32 az = a.z;
	f32 ax = a.x;
	f32 bz = b.z;
	f32 bx = b.x;
	return (az - z) * (bx - ax) - (ax - x) * (bz - az);
}

f32 TMapCollisionData::checkRoofList(f32 x, f32 y, f32 z, u8 param_4,
                                     const TBGCheckList* head,
                                     const TBGCheckData** result)
{
	while (head) {
		const TBGCheckData* data = head->unk8;
		head                     = head->mNext;

		if (param_4 & 0x4 && data->isWaterThrough())
			continue;

		if (MapCheckEdge(data->mPoint1, data->mPoint2, x, z) > 1.0f)
			continue;

		if (MapCheckEdge(data->mPoint2, data->mPoint3, x, z) > 1.0f)
			continue;

		if (MapCheckEdge(data->mPoint3, data->mPoint1, x, z) > 1.0f)
			continue;

		f32 tmp = (x * data->mNormal.x) + (z * data->mNormal.z)
		          + data->mPlaneDistance;
		f32 dVar10 = -tmp / data->mNormal.y;

		if (!(y - (dVar10 - -78.0f) > 0.0f)) {
			*result = data;
			return dVar10;
		}
	}

	*result = &mIllegalCheckData;
	return 9999999.0f;
}

// TODO: local_4c/local_50 sit at 0x48/0x44 vs retail 0x44/0x40. Inert: raw
// mGridExtentX at every subset of sites, local_4c declared at the top.
f32 TMapCollisionData::checkRoof(f32 x, f32 y, f32 z, u8 flags,
                                 const TBGCheckData** result) const
{
	if (x < -getGridExtentX() || getGridExtentX() <= x || z < -mGridExtentY
	    || mGridExtentY <= z) {
		*result = &mIllegalCheckData;
		return 9999999.0f;
	}

	int gridX = (x + getGridExtentX()) * (1.0f / 1024);
	int gridZ = (z + mGridExtentY) * (1.0f / 1024);

	const TBGCheckData* local_4c;
	f32 dVar5 = checkRoofList(
	    x, y, z, flags, getGridRoot18(gridX, gridZ).getRoofList(), &local_4c);

	const TBGCheckData* local_50;
	f32 dVar6 = checkRoofList(
	    x, y, z, flags, getGridRoot14(gridX, gridZ).getRoofList(), &local_50);

	if (dVar5 < dVar6) {
		*result = local_4c;
		return dVar5;
	} else {
		*result = local_50;
		return dVar6;
	}
}

f32 TMapCollisionData::checkGroundList(f32 x, f32 y, f32 z, u8 flags,
                                       const TBGCheckList* head,
                                       const TBGCheckData** result)
{
	while (head) {
		const TBGCheckData* data = head->unk8;
		head                     = head->getNext();

		if (data->mMinY > y)
			continue;

		if ((flags & IGNORE_WATER_THROUGH) && data->isWaterThrough())
			continue;

		if ((flags & IGNORE_WATER_SURFACE) && data->isWaterSurface())
			continue;

		if (MapCheckEdge(data->mPoint1, data->mPoint2, x, z) < -1.0f)
			continue;

		if (MapCheckEdge(data->mPoint2, data->mPoint3, x, z) < -1.0f)
			continue;

		if (MapCheckEdge(data->mPoint3, data->mPoint1, x, z) < -1.0f)
			continue;

		f32 tmp
		    = x * data->mNormal.x + z * data->mNormal.z + data->mPlaneDistance;
		f32 dVar10 = -tmp / data->mNormal.y;

		if (!(y - (dVar10 + -78.0f) < 0.0f)) {
			*result = data;
			return dVar10;
		}
	}

	*result = &mIllegalCheckData;
	return -32767.0f;
}

// HEADER NEED: TBGCheckListRoot has getRoofList()/getWallList() but no
// getGroundList(); parked here as a TU-local until the shared header can take
// it.
static inline const TBGCheckList* MapCheckGroundList(const TBGCheckListRoot& r)
{
	return r.unk0[0].getNext();
}

// Binding level over a raw member read, worth +16 of low region in
// TMapCollisionData::checkGround (batch 127).
static inline TMapCheckGroundPlane* MapCheckGroundPlane(const TMapCollisionData* p)
{
	TMapCheckGroundPlane* groundPlane = p->mGroundPlane;
	return groundPlane;
}

f32 TMapCollisionData::checkGround(f32 x, f32 y, f32 z, u8 flags,
                                   const TBGCheckData** result) const
{
	TMapCheckGroundPlane* groundPlane;
	if (x < -mGridExtentX || mGridExtentX <= x || z < -mGridExtentY
	    || mGridExtentY <= z) {
		*result = &mIllegalCheckData;
		return -32767.0f;
	}

	int gridX = (x + mGridExtentX) * (1.0f / 1024);
	int gridZ = (z + mGridExtentY) * (1.0f / 1024);

	const TBGCheckListRoot& gridRoot = getGridRoot18(gridX, gridZ);
	const TBGCheckData* local_60;
	f32 dVar5 = checkGroundList(
	    x, y, z, flags, MapCheckGroundList(gridRoot),
	    &local_60);

	const TBGCheckData* local_64;
	f32 dVar6 = checkGroundList(
	    x, y, z, flags, MapCheckGroundList(getGridRoot14(gridX, gridZ)),
	    &local_64);

	groundPlane = MapCheckGroundPlane(this);
	if (groundPlane != nullptr) {
		const TBGCheckData* local_68;
		f32 dVar7 = MapCheckGroundPlane(this)->checkPlaneGround(x, y, z, &local_68);
		if (dVar7 > dVar6) {
			local_64 = local_68;
			dVar6    = dVar7;
		}
	}

	if (dVar5 > dVar6) {
		*result = local_60;
		return dVar5;
	} else {
		*result = local_64;
		return dVar6;
	}
}

static f32 angle_between(const JGeometry::TVec3<f32>& a,
                         const JGeometry::TVec3<f32>& b)
{
	JGeometry::TVec3<f32> cross;
	cross.cross(a, b);
	f32 crossMag = cross.length();
	f32 dot      = a.dot(b);
	f32 angle    = atan2f(crossMag, dot);
	return fabsf(angle);
}

// TODO: frame 0x188 vs retail 0x1d8 (0x50 of missing inline temporaries;
// a.dot(b) in angle_between gave 0x10). Retail's named block is 0x24 taller
// (a/b/c reserve slots) -- early declarations, by-value angle_between
// parameters and `a = p - hit` are all worse. The rest is FPR scheduling.
// Also inert: a/b/c declared after dir or before hit; angle_between with
// fewer named results or marked inline.
static bool bgIntersectLine(const TBGCheckData* data,
                            const JGeometry::TVec3<f32>& start,
                            const JGeometry::TVec3<f32>& end, bool front_only,
                            JGeometry::TVec3<f32>* hit_pos)
{
	if (!data)
		return false;

	if (data->isMarioThrough())
		return false;

	JGeometry::TVec3<f32> normal = data->getNormal();
	JGeometry::TVec3<f32> dir    = end;
	dir -= start;

	f32 nDotDir = normal.dot(dir);
	if (front_only && nDotDir >= 0.0f)
		return false;

	if (fabsf(nDotDir) < 0.00001f)
		return false;

	f32 planeDist = -(data->getPlaneDistance() + normal.dot(start)) / nDotDir;

	if (planeDist < 0.0f)
		return false;

	if (1.0f < planeDist)
		return false;

	dir *= planeDist;

	JGeometry::TVec3<f32> hit = start + dir;

	JGeometry::TVec3<f32> a;
	a.sub(data->getPoint1(), hit);
	JGeometry::TVec3<f32> b;
	b.sub(data->getPoint2(), hit);
	JGeometry::TVec3<f32> c;
	c.sub(data->getPoint3(), hit);

	f32 angleSum = 0.0f;

	angleSum += fabsf(angle_between(a, b));
	angleSum += fabsf(angle_between(b, c));
	angleSum += fabsf(angle_between(c, a));

	if (fabsf(6.2831855f - angleSum) > 0.001f)
		return false;

	if (hit_pos)
		*hit_pos = hit;

	return true;
}

static const TBGCheckData* intersectLineList(const TBGCheckList* head,
                                             const JGeometry::TVec3<f32>& start,
                                             const JGeometry::TVec3<f32>& end,
                                             bool front_only,
                                             JGeometry::TVec3<f32>* hit_pos)
{
	while (head) {
		TBGCheckData* data = head->unk8;
		head               = head->mNext;

		if (bgIntersectLine(data, start, end, front_only, hit_pos))
			return data;
	}

	return nullptr;
}

// Retail copies both endpoints before subtracting the origin from either.
static inline f32 LineSide(const JGeometry::TVec2<f32>& o,
                           const JGeometry::TVec2<f32>& a,
                           const JGeometry::TVec2<f32>& b)
{
	JGeometry::TVec2<f32> p = a;
	JGeometry::TVec2<f32> q = b;
	p -= o;
	q -= o;
	return p.cross(q);
}

static bool LineInLineXZ(const JGeometry::TVec2<f32>& a0,
                         const JGeometry::TVec2<f32>& a1,
                         const JGeometry::TVec2<f32>& b0,
                         const JGeometry::TVec2<f32>& b1)
{
	if (LineSide(a0, a1, b0) * LineSide(a0, a1, b1) <= 0.0f
	    && LineSide(b0, b1, a0) * LineSide(b0, b1, a1) <= 0.0f)
		return true;

	return false;
}

// TODO: 96.6%. Frame 0x18 too small and a register permutation: retail
// keeps the parameters in r24-r28 above the grid bounds (r20-r23); ours puts
// the bounds above the parameters (declaring the loop counters first, or the
// bounds as named ints, changes nothing). Retail converts start.x/end.x twice
// (once for the swap test) but start.z/end.z once, which the int swap below
// reproduces. Grid bounds built by a TVec2<int>-returning helper give the
// exact 0x510 frame but 96.3% (slot pairs and the same register swap).
const TBGCheckData* TMapCollisionData::intersectLine(
    const JGeometry::TVec3<f32>& start, const JGeometry::TVec3<f32>& end,
    bool front_only, JGeometry::TVec3<f32>* hit_pos) const
{
	int minXi = start.x, maxXi = end.x, minZi = start.z, maxZi = end.z;
	if ((int)start.x > (int)end.x) {
		int t = minXi;
		minXi = maxXi;
		maxXi = t;
	}
	if (minZi > maxZi) {
		int t = minZi;
		minZi = maxZi;
		maxZi = t;
	}

	JGeometry::TVec2<f32> lineA(start.x, start.z);
	JGeometry::TVec2<f32> lineB(end.x, end.z);

	JGeometry::TVec2<int> minGrid((int)((minXi + mGridExtentX) * (1.0f / 1024)),
	                              (int)((minZi + mGridExtentY) * (1.0f / 1024)));
	JGeometry::TVec2<int> maxGrid((int)((maxXi + mGridExtentX) * (1.0f / 1024)),
	                              (int)((maxZi + mGridExtentY) * (1.0f / 1024)));

	for (int gridZ = minGrid.y; gridZ <= maxGrid.y; ++gridZ) {
		for (int gridX = minGrid.x; gridX <= maxGrid.x; ++gridX) {
			if (gridX == minGrid.x && gridZ == minGrid.y) {
			} else {
				f32 x0i = gridX * 1024.0f;
				f32 z0i = gridZ * 1024.0f;
				f32 x1i = (gridX + 1) * 1024.0f;
				f32 z1i = (gridZ + 1) * 1024.0f;

				JGeometry::TVec2<f32> corner[4];
				corner[0].set((s32)(x0i - mGridExtentX), (s32)(z0i - mGridExtentY));
				corner[1].set((s32)(x1i - mGridExtentX), (s32)(z0i - mGridExtentY));
				corner[2].set((s32)(x0i - mGridExtentX), (s32)(z1i - mGridExtentY));
				corner[3].set((s32)(x1i - mGridExtentX), (s32)(z1i - mGridExtentY));


				if (!LineInLineXZ(lineA, lineB, corner[0], corner[1])
				    && !LineInLineXZ(lineA, lineB, corner[0], corner[2])
				    && !LineInLineXZ(lineA, lineB, corner[2], corner[3])
				    && !LineInLineXZ(lineA, lineB, corner[3], corner[1]))
					continue;
			}

			const TBGCheckData* hit = intersectLineList(
			    getGridRoot14(gridX, gridZ).unk0[0].getNext(), start, end,
			    front_only, hit_pos);
			if (hit)
				return hit;

			hit = intersectLineList(
			    getGridRoot14(gridX, gridZ).unk0[2].getNext(), start, end,
			    front_only, hit_pos);
			if (hit)
				return hit;

			hit = intersectLineList(
			    getGridRoot14(gridX, gridZ).unk0[1].getNext(), start, end,
			    front_only, hit_pos);
			if (hit)
				return hit;
		}
	}

	return nullptr;
}
