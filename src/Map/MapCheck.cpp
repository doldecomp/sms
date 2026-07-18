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

	if (r31->checkFlag(0x4)) {
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

int TMapCollisionData::checkWallList(const TBGCheckList* param_1,
                                     TBGWallCheckRecord* param_2)
{
	if (!param_1)
		return 0;

	f32 f27 = param_2->mCenter.y;
	// param_1: r28
	// param_2: r29

	int r30 = 0;
	while (param_1) {
		TBGCheckData* r31 = param_1->unk8;
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

f32 TMapCollisionData::checkRoofList(f32 x, f32 y, f32 z, u8 param_4,
                                     const TBGCheckList* head,
                                     const TBGCheckData** result)
{
	while (head) {
		const TBGCheckData* data = head->unk8;
		head                     = head->mNext;

		if (param_4 & 0x4 && data->isWaterThrough())
			continue;

		if ((data->mPoint1.z - z) * (data->mPoint2.x - data->mPoint1.x)
		        - (data->mPoint1.x - x) * (data->mPoint2.z - data->mPoint1.z)
		    > 1.0f)
			continue;

		if ((data->mPoint2.z - z) * (data->mPoint3.x - data->mPoint2.x)
		        - (data->mPoint2.x - x) * (data->mPoint3.z - data->mPoint2.z)
		    > 1.0f)
			continue;

		if ((data->mPoint3.z - z) * (data->mPoint1.x - data->mPoint3.x)
		        - (data->mPoint3.x - x) * (data->mPoint1.z - data->mPoint3.z)
		    > 1.0f)
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

f32 TMapCollisionData::checkRoof(f32 x, f32 y, f32 z, u8 flags,
                                 const TBGCheckData** result) const
{
	if (x < -mGridExtentX || mGridExtentX <= x || z < -mGridExtentY
	    || mGridExtentY <= z) {
		*result = &mIllegalCheckData;
		return 9999999.0f;
	}

	int gridX = (x + mGridExtentX) * (1.0f / 1024);
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

		if ((data->mPoint1.z - z) * (data->mPoint2.x - data->mPoint1.x)
		        - (data->mPoint1.x - x) * (data->mPoint2.z - data->mPoint1.z)
		    < -1.0f)
			continue;

		if ((data->mPoint2.z - z) * (data->mPoint3.x - data->mPoint2.x)
		        - (data->mPoint2.x - x) * (data->mPoint3.z - data->mPoint2.z)
		    < -1.0f)
			continue;

		if ((data->mPoint3.z - z) * (data->mPoint1.x - data->mPoint3.x)
		        - (data->mPoint3.x - x) * (data->mPoint1.z - data->mPoint3.z)
		    < -1.0f)
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
	return 9999999.0f;
}

f32 TMapCollisionData::checkGround(f32 x, f32 y, f32 z, u8 flags,
                                   const TBGCheckData** result) const
{
	if (x < -mGridExtentX || mGridExtentX <= x || z < -mGridExtentY
	    || mGridExtentY <= z) {
		*result = &mIllegalCheckData;
		return -32767.0f;
	}

	int gridX = (x + mGridExtentX) * (1.0f / 1024);
	int gridZ = (z + mGridExtentY) * (1.0f / 1024);

	const TBGCheckData* local_60;
	f32 dVar5 = checkGroundList(
	    x, y, z, flags, getGridRoot18(gridX, gridZ).getRoofList(), &local_60);

	const TBGCheckData* local_64;
	f32 dVar6 = checkGroundList(
	    x, y, z, flags, getGridRoot14(gridX, gridZ).getRoofList(), &local_64);

	if (mGroundPlane != nullptr) {
		const TBGCheckData* local_68;
		f32 dVar7 = mGroundPlane->checkPlaneGround(x, y, z, &local_68);
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
	f32 dot      = a.x * b.x + a.y * b.y + a.z * b.z;
	f32 angle    = atan2f(crossMag, dot);
	return fabsf(angle);
}

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

inline const TBGCheckData* intersectLineList(const TBGCheckList* head,
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

static bool LineInLineXZ(const JGeometry::TVec2<f32>& a0,
                         const JGeometry::TVec2<f32>& a1,
                         const JGeometry::TVec2<f32>& b0,
                         const JGeometry::TVec2<f32>& b1)
{
	if ((b0 - a0).cross(a1 - a0) * (b1 - a0).cross(a1 - a0) <= 0.0f
	    && (a0 - b0).cross(b1 - b0) * (a1 - b0).cross(b1 - b0) <= 0.0f)
		return true;

	return false;
}

const TBGCheckData* TMapCollisionData::intersectLine(
    const JGeometry::TVec3<f32>& start, const JGeometry::TVec3<f32>& end,
    bool front_only, JGeometry::TVec3<f32>* hit_pos) const
{
	JGeometry::TVec2<int> start2d(start.x, start.z);
	JGeometry::TVec2<int> end2d(end.x, end.z);

	int minXi = start2d.x;
	int maxXi = end2d.x;
	if (start2d.x > end2d.x) {
		minXi = end2d.x;
		maxXi = start2d.x;
	}

	int minZi = start2d.y;
	int maxZi = end2d.y;
	if (start2d.y > end2d.y) {
		minZi = end2d.y;
		maxZi = start2d.y;
	}

	int minGridZ = (int)((minZi + mGridExtentY) * (1.0f / 1024));
	int minGridX = (int)((minXi + mGridExtentX) * (1.0f / 1024));
	int maxGridX = (int)((maxXi + mGridExtentX) * (1.0f / 1024));
	int maxGridZ = (int)((maxZi + mGridExtentY) * (1.0f / 1024));

	for (int gridZ = minGridZ; gridZ <= maxGridZ; ++gridZ) {
		for (int gridX = minGridX; gridX <= maxGridX; ++gridX) {
			if (gridX == minGridX && gridZ == minGridZ) {
			} else {
				f32 x0i = gridX * 1024.0f;
				f32 z0i = gridZ * 1024.0f;
				f32 x1i = (gridX + 1) * 1024.0f;
				f32 z1i = (gridZ + 1) * 1024.0f;

				JGeometry::TVec2<f32> p00((s32)(x0i - mGridExtentX),
				                          (s32)(z0i - mGridExtentY));
				JGeometry::TVec2<f32> p10((s32)(x1i - mGridExtentX),
				                          (s32)(z0i - mGridExtentY));
				JGeometry::TVec2<f32> p01((s32)(x0i - mGridExtentX),
				                          (s32)(z1i - mGridExtentY));
				JGeometry::TVec2<f32> p11((s32)(x1i - mGridExtentX),
				                          (s32)(z1i - mGridExtentY));

				JGeometry::TVec2<f32> lineA(start.x, start.z);
				JGeometry::TVec2<f32> lineB(end.x, end.z);

				if (!LineInLineXZ(lineA, lineB, p10, p00)
				    && !LineInLineXZ(lineA, lineB, p01, p00)
				    && !LineInLineXZ(lineA, lineB, p11, p01)
				    && !LineInLineXZ(lineA, lineB, p10, p11))
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
