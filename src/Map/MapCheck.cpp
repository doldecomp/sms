#include <Map/MapCollisionData.hpp>
#include <Map/MapCollisionPlane.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/MathUtil.hpp>

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

	f32 cz = r29->mCenter.z;

	f32 d = r31->getPlaneDistance();

	f32 signedDist
	    = d
	      + (r29->mCenter.x * r31->getNormal().x
	         + r29->mCenter.y * r31->getNormal().y + cz * r31->getNormal().z);

	if (signedDist < -radius || radius < signedDist)
		return false;

	f32 y1 = r31->getPoint1().y;
	f32 y2 = r31->getPoint2().y;
	f32 y3 = r31->getPoint3().y;

	if (r31->checkFlag(BG_CHECK_FLAG_X_FACING)) {
		if (r31->getNormal().x > 0.0f) {
			cz = -cz;

			f32 z1 = -r31->getPoint1().z;
			f32 z2 = -r31->getPoint2().z;
			f32 z3 = -r31->getPoint3().z;

			if (skewProduct(y1 - r29->mCenter.y, z1 - cz, y2 - y1, z2 - z1)
			    > 1.0f)
				return false;

			if (skewProduct(y2 - r29->mCenter.y, z2 - cz, y3 - y2, z3 - z2)
			    > 1.0f)
				return false;

			if (skewProduct(y3 - r29->mCenter.y, z3 - cz, y1 - y3, z1 - z3)
			    > 1.0f)
				return false;
		} else {
			cz = -cz;

			f32 z1 = -r31->getPoint1().z;
			f32 z2 = -r31->getPoint2().z;
			f32 z3 = -r31->getPoint3().z;

			if (skewProduct(y1 - r29->mCenter.y, z1 - cz, y2 - y1, z2 - z1)
			    < -1.0f)
				return false;

			if (skewProduct(y2 - r29->mCenter.y, z2 - cz, y3 - y2, z3 - z2)
			    < -1.0f)
				return false;

			if (skewProduct(y3 - r29->mCenter.y, z3 - cz, y1 - y3, z1 - z3)
			    < -1.0f)
				return false;
		}
	} else {
		if (r31->getNormal().z > 0.0f) {
			f32 x1 = r31->getPoint1().x;
			f32 x2 = r31->getPoint2().x;
			f32 x3 = r31->getPoint3().x;

			if (skewProduct(y1 - r29->mCenter.y, x1 - r29->mCenter.x, y2 - y1,
			                x2 - x1)
			    > 1.0f)
				return false;

			if (skewProduct(y2 - r29->mCenter.y, x2 - r29->mCenter.x, y3 - y2,
			                x3 - x2)
			    > 1.0f)
				return false;

			if (skewProduct(y3 - r29->mCenter.y, x3 - r29->mCenter.x, y1 - y3,
			                x1 - x3)
			    > 1.0f)
				return false;
		} else {
			f32 x1 = r31->getPoint1().x;
			f32 x2 = r31->getPoint2().x;
			f32 x3 = r31->getPoint3().x;

			if (skewProduct(y1 - r29->mCenter.y, x1 - r29->mCenter.x, y2 - y1,
			                x2 - x1)
			    < -1.0f)
				return false;

			if (skewProduct(y2 - r29->mCenter.y, x2 - r29->mCenter.x, y3 - y2,
			                x3 - x2)
			    < -1.0f)
				return false;

			if (skewProduct(y3 - r29->mCenter.y, x3 - r29->mCenter.x, y1 - y3,
			                x1 - x3)
			    < -1.0f)
				return false;
		}
	}

	if (!(flags & TBGWallCheckRecord::DONT_MOVE_XZ)) {
		f32 push = radius - signedDist;
		r29->mCenter.x += push * r31->getNormal().x;
		r29->mCenter.z += push * r31->getNormal().z;
	}

	return true;
}

int TMapCollisionData::checkWallList(const TBGCheckList* param_1,
                                     TBGWallCheckRecord* param_2)
{
	if (!param_1)
		return 0;

	TBGCheckData* data;
	f32 f27 = param_2->mCenter.y;
	int num = 0;
	while (param_1) {
		data    = param_1->mData;
		param_1 = param_1->mNext;

		if (data->mMinY > f27)
			continue;

		if (data->mMaxY < f27)
			return num;

		if (!someUnknownInline(data, param_2))
			continue;

		++num;
		param_2->mResultWalls[param_2->mResultWallsNum] = data;
		++param_2->mResultWallsNum;
		if (param_2->mResultWallsNum >= param_2->mMaxResults)
			return num;
	}

	return num;
}

int TMapCollisionData::checkWalls(TBGWallCheckRecord* record) const
{
	record->mResultWallsNum = 0;
	if (record->getCenter().x < -mGridExtentX
	    || mGridExtentX <= record->getCenter().x
	    || record->getCenter().z < -mGridExtentY
	    || mGridExtentY <= record->getCenter().z)
		return 0;

	int gridX = worldToGridX(record->getCenter().x);
	int gridZ = worldToGridZ(record->getCenter().z);

	int num
	    = checkWallList(getMoveCheckList(gridX, gridZ).getWallList(), record);
	if (num >= record->getMaxResults())
		return num;

	num += checkWallList(getStaticCheckList(gridX, gridZ).getWallList(),
	                     record);
	return num;
}

f32 TMapCollisionData::checkRoofList(f32 x, f32 y, f32 z, u8 param_4,
                                     const TBGCheckList* head,
                                     const TBGCheckData** result)
{
	const TBGCheckData* data;
	while (head) {
		data = head->getData();
		head = head->getNext();

		if ((param_4 & IGNORE_WATER_THROUGH) && data->isWaterThrough())
			continue;

		f32 x1 = data->getPoint1().x;
		f32 z1 = data->getPoint1().z;

		if ((z1 - z) * (data->mPoint2.x - x1)
		        - (x1 - x) * (data->mPoint2.z - z1)
		    > 1.0f)
			continue;

		if ((data->mPoint2.z - z) * (data->mPoint3.x - data->mPoint2.x)
		        - (data->mPoint2.x - x) * (data->mPoint3.z - data->mPoint2.z)
		    > 1.0f)
			continue;

		if ((data->mPoint3.z - z) * (x1 - data->mPoint3.x)
		        - (data->mPoint3.x - x) * (z1 - data->mPoint3.z)
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

	int gridX = worldToGridX(x);
	int gridZ = worldToGridZ(z);

	const TBGCheckData* local_4c;
	f32 dVar5 = checkRoofList(x, y, z, flags,
	                          getMoveCheckList(gridX, gridZ).getRoofList(),
	                          &local_4c);

	const TBGCheckData* local_50;
	f32 dVar6 = checkRoofList(x, y, z, flags,
	                          getStaticCheckList(gridX, gridZ).getRoofList(),
	                          &local_50);

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
		const TBGCheckData* data = head->getData();
		head                     = head->getNext();

		if (data->mMinY > y)
			continue;

		if ((flags & IGNORE_WATER_THROUGH) && data->isWaterThrough())
			continue;

		if ((flags & IGNORE_WATER_SURFACE) && data->isWaterSurface())
			continue;

		f32 x1 = data->getPoint1().x;
		f32 z1 = data->getPoint1().z;

		if ((z1 - z) * (data->mPoint2.x - x1)
		        - (x1 - x) * (data->mPoint2.z - z1)
		    < -1.0f)
			continue;

		if ((data->mPoint2.z - z) * (data->mPoint3.x - data->mPoint2.x)
		        - (data->mPoint2.x - x) * (data->mPoint3.z - data->mPoint2.z)
		    < -1.0f)
			continue;

		if ((data->mPoint3.z - z) * (x1 - data->mPoint3.x)
		        - (data->mPoint3.x - x) * (z1 - data->mPoint3.z)
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
	return -32767.0f;
}

f32 TMapCollisionData::checkGround(f32 x, f32 y, f32 z, u8 flags,
                                   const TBGCheckData** result) const
{
	if (x < -mGridExtentX || mGridExtentX <= x || z < -mGridExtentY
	    || mGridExtentY <= z) {
		*result = &mIllegalCheckData;
		return -32767.0f;
	}

	int gridX = worldToGridX(x);
	int gridZ = worldToGridZ(z);

	const TBGCheckData* local_60;
	f32 dVar5 = checkGroundList(x, y, z, flags,
	                            getMoveCheckList(gridX, gridZ).getGroundList(),
	                            &local_60);

	const TBGCheckData* local_64;
	f32 dVar6 = checkGroundList(
	    x, y, z, flags, getStaticCheckList(gridX, gridZ).getGroundList(),
	    &local_64);

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
	a.set(data->getPoint1());
	JGeometry::TVec3<f32> b;
	b.set(data->getPoint2());
	JGeometry::TVec3<f32> c;
	c.set(data->getPoint3());
	a -= hit;
	b -= hit;
	c -= hit;

	f32 angleSum = 0.0f;

	angleSum += fabsf(MsAngleBetween(a, b));
	angleSum += fabsf(MsAngleBetween(b, c));
	angleSum += fabsf(MsAngleBetween(c, a));

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
		const TBGCheckData* data = head->getData();
		head                     = head->getNext();

		if (bgIntersectLine(data, start, end, front_only, hit_pos))
			return data;
	}

	return nullptr;
}

static inline f32 crossFrom(const JGeometry::TVec2<f32>& origin,
                            const JGeometry::TVec2<f32>& p,
                            const JGeometry::TVec2<f32>& q)
{
	JGeometry::TVec2<f32> dq = q;
	JGeometry::TVec2<f32> dp = p;
	dq -= origin;
	dp -= origin;
	return dp.cross(dq);
}

static bool LineInLineXZ(const JGeometry::TVec2<f32>& a0,
                         const JGeometry::TVec2<f32>& a1,
                         const JGeometry::TVec2<f32>& b0,
                         const JGeometry::TVec2<f32>& b1)
{
	if (crossFrom(a0, b1, a1) * crossFrom(a0, b0, a1) <= 0.0f
	    && crossFrom(b0, a1, b1) * crossFrom(b0, a0, b1) <= 0.0f)
		return true;

	return false;
}

const TBGCheckData* TMapCollisionData::intersectLine(
    const JGeometry::TVec3<f32>& start, const JGeometry::TVec3<f32>& end,
    bool front_only, JGeometry::TVec3<f32>* hit_pos) const
{
	int gridX;
	int gridZ;
	int minXi = start.x;
	int minZi = start.z;
	int maxXi = end.x;
	int maxZi = end.z;
	if (minXi > maxXi) {
		int tmp = minXi;
		minXi   = maxXi;
		maxXi   = tmp;
	}

	if (minZi > maxZi) {
		int tmp = minZi;
		minZi   = maxZi;
		maxZi   = tmp;
	}

	int minGridX = worldToGridX(minXi);
	int minGridZ = worldToGridZ(minZi);
	int maxGridX = worldToGridX(maxXi);
	int maxGridZ = worldToGridZ(maxZi);

	JGeometry::TVec2<f32> startXZ(start.x, start.z);
	JGeometry::TVec2<f32> endXZ(end.x, end.z);

	for (gridZ = minGridZ; gridZ <= maxGridZ; ++gridZ) {
		for (gridX = minGridX; gridX <= maxGridX; ++gridX) {
			if (gridX != minGridX || gridZ != minGridZ) {
				JGeometry::TVec2<f32> corners[4];
				corners[0].set<f32>(gridToWorldX(gridX), gridToWorldZ(gridZ));
				corners[1].set<f32>(gridToWorldX(gridX + 1),
				                    gridToWorldZ(gridZ));
				corners[2].set<f32>(gridToWorldX(gridX),
				                    gridToWorldZ(gridZ + 1));
				corners[3].set<f32>(gridToWorldX(gridX + 1),
				                    gridToWorldZ(gridZ + 1));

				if (!LineInLineXZ(startXZ, endXZ, corners[1], corners[0])
				    && !LineInLineXZ(startXZ, endXZ, corners[2], corners[0])
				    && !LineInLineXZ(startXZ, endXZ, corners[3], corners[2])
				    && !LineInLineXZ(startXZ, endXZ, corners[1], corners[3]))
					continue;
			}

			const TBGCheckData* hit = intersectLineList(
			    getStaticCheckList(gridX, gridZ).getGroundList(), start, end,
			    front_only, hit_pos);
			if (hit)
				return hit;

			hit = intersectLineList(
			    getStaticCheckList(gridX, gridZ).getWallList(), start, end,
			    front_only, hit_pos);
			if (hit)
				return hit;

			hit = intersectLineList(
			    getStaticCheckList(gridX, gridZ).getRoofList(), start, end,
			    front_only, hit_pos);
			if (hit)
				return hit;
		}
	}

	return nullptr;
}
