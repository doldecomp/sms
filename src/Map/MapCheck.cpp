#include <Map/MapCollisionData.hpp>
#include <Map/MapCollisionPlane.hpp>
#include <Map/MapData.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

inline static bool checkThing(u16 thing)
{
	if (thing == 0x401 || thing == 0x801 || thing == 0x10A || thing == 0x8400)
		return true;

	return false;
}

inline static bool checkThing2(u16 thing)
{
	if (thing == 0x100 || thing == 0x101 || thing == 0x102 || thing == 0x103
	    || thing == 0x104 || thing == 0x105 || thing == 0x4104)
		return true;

	return false;
}

// either this or have to use gotos
inline static bool someUnknownInline(TBGCheckData* r31, TBGWallCheckRecord* r29)
{
	if (r29->unk18 & 0x4 && checkThing(r31->unk0))
		return false;

	if (r31->isWaterSurface())
		return false;

	// TODO: tons of float checks ewww

	return true;
}

#pragma dont_inline on
int TMapCollisionData::checkWallList(const TBGCheckList* param_1,
                                     TBGWallCheckRecord* param_2)
{
	if (!param_1)
		return 0;

	f32 f27 = param_2->unk0.y;
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
		param_2->unk1C[param_2->unk14] = r31;
		++param_2->unk14;
		if (param_2->unk14 >= param_2->unk10)
			return r30;
	}

	return r30;
}
#pragma dont_inline off

int TMapCollisionData::checkWalls(TBGWallCheckRecord* param_1) const
{
	param_1->unk14 = 0;
	if (param_1->unk0.x < -unk0 || unk0 <= param_1->unk0.x
	    || param_1->unk0.z < -unk4 || unk4 <= param_1->unk0.z)
		return 0;

	int gridX = (param_1->unk0.x + unk0) * 0.0009765625f;
	int gridZ = (param_1->unk0.z + unk4) * 0.0009765625f;

	int iVar6
	    = checkWallList(getGridRoot18(gridX, gridZ).getWallList(), param_1);

	if (iVar6 >= param_1->unk10)
		return iVar6;

	iVar6 += checkWallList(getGridRoot14(gridX, gridZ).getWallList(), param_1);
	return iVar6;
}

f32 TMapCollisionData::checkRoofList(f32 x, f32 y, f32 z, u8 param_4,
                                     const TBGCheckList* param_5,
                                     const TBGCheckData** param_6)
{
	while (param_5) {
		TBGCheckData* data = param_5->unk8;
		param_5            = param_5->mNext;

		if (param_4 & 0x4 && checkThing(data->unk0))
			continue;

		// TODO: inlines
		if ((data->mPoint1.z - z) * (data->mPoint2.x - data->mPoint1.x)
		            - (data->mPoint1.x - x)
		                  * (data->mPoint2.z - data->mPoint1.z)
		        > 1.0f
		    || (data->mPoint2.z - z) * (data->mPoint3.x - data->mPoint2.x)
		               - (data->mPoint2.x - x)
		                     * (data->mPoint3.z - data->mPoint2.z)
		           > 1.0f
		    || (data->mPoint3.z - z) * (data->mPoint1.x - data->mPoint3.x)
		               - (data->mPoint3.x - x)
		                     * (data->mPoint1.z - data->mPoint3.z)
		           > 1.0f)
			continue;

		f32 tmp = (x * data->mNormal.x) + (z * data->mNormal.z) + data->unk40;
		f32 dVar10 = -tmp / data->mNormal.y;

		if (!(y - (dVar10 - -78.0f) > 0.0f)) {
			*param_6 = data;
			return dVar10;
		}
	}

	*param_6 = &mIllegalCheckData;
	return 9999999.0f;
}

f32 TMapCollisionData::checkRoof(f32 x, f32 y, f32 z, u8 param_4,
                                 const TBGCheckData** param_5) const
{
	if (x < -unk0 || unk0 <= x || z < -unk4 || unk4 <= z) {
		*param_5 = &mIllegalCheckData;
		return 9999999.0f;
	}

	int gridX = (x + unk0) * 0.0009765625f;
	int gridZ = (z + unk4) * 0.0009765625f;

	const TBGCheckData* local_4c;
	f32 dVar5 = checkRoofList(
	    x, y, z, param_4, getGridRoot18(gridX, gridZ).getRoofList(), &local_4c);

	const TBGCheckData* local_50;
	f32 dVar6 = checkRoofList(
	    x, y, z, param_4, getGridRoot14(gridX, gridZ).getRoofList(), &local_50);

	if (dVar5 < dVar6) {
		*param_5 = local_4c;
		return dVar5;
	} else {
		*param_5 = local_50;
		return dVar6;
	}
}

f32 TMapCollisionData::checkGroundList(f32 x, f32 y, f32 z, u8 param_4,
                                       const TBGCheckList* param_5,
                                       const TBGCheckData** param_6)
{
	while (param_5) {
		const TBGCheckData* data = param_5->unk8;
		param_5                  = param_5->getNext();

		if (data->mMinY > y)
			continue;

		if (param_4 & 0x4 && checkThing(data->unk0))
			continue;

		if (param_4 & 0x1 && checkThing2(data->unk0))
			continue;

		// TODO: inlines
		if ((data->mPoint1.z - z) * (data->mPoint2.x - data->mPoint1.x)
		            - (data->mPoint1.x - x)
		                  * (data->mPoint2.z - data->mPoint1.z)
		        < -1.0f
		    || (data->mPoint2.z - z) * (data->mPoint3.x - data->mPoint2.x)
		               - (data->mPoint2.x - x)
		                     * (data->mPoint3.z - data->mPoint2.z)
		           < -1.0f
		    || (data->mPoint3.z - z) * (data->mPoint1.x - data->mPoint3.x)
		               - (data->mPoint3.x - x)
		                     * (data->mPoint1.z - data->mPoint3.z)
		           < -1.0f)
			continue;

		f32 tmp    = x * data->mNormal.x + z * data->mNormal.z + data->unk40;
		f32 dVar10 = -tmp / data->mNormal.y;

		if (!(y - (dVar10 + -78.0f) < 0.0f)) {
			*param_6 = data;
			return dVar10;
		}
	}

	*param_6 = &mIllegalCheckData;
	return 9999999.0f;
}

f32 TMapCollisionData::checkGround(f32 x, f32 y, f32 z, u8 param_4,
                                   const TBGCheckData** param_5) const
{
	if (x < -unk0 || unk0 <= x || z < -unk4 || unk4 <= z) {
		*param_5 = &mIllegalCheckData;
		return -32767.0f;
	}

	int gridX = (x + unk0) * 0.0009765625f;
	int gridZ = (z + unk4) * 0.0009765625f;

	const TBGCheckData* local_60;
	f32 dVar5 = checkGroundList(
	    x, y, z, param_4, getGridRoot18(gridX, gridZ).getRoofList(), &local_60);

	const TBGCheckData* local_64;
	f32 dVar6 = checkGroundList(
	    x, y, z, param_4, getGridRoot14(gridX, gridZ).getRoofList(), &local_64);

	if (unk244 != nullptr) {
		const TBGCheckData* local_68;
		f32 dVar7 = unk244->checkPlaneGround(x, y, z, &local_68);
		if (dVar7 > dVar6) {
			local_64 = local_68;
			dVar6    = dVar7;
		}
	}

	if (dVar5 > dVar6) {
		*param_5 = local_60;
		return dVar5;
	} else {
		*param_5 = local_64;
		return dVar6;
	}
}

void bgIntersectLine(const TBGCheckData*, const JGeometry::TVec3<f32>&,
                     const JGeometry::TVec3<f32>&, bool, JGeometry::TVec3<f32>*)
{
}

void intersectLineList(const TBGCheckList*, const JGeometry::TVec3<f32>&,
                       const JGeometry::TVec3<f32>&, bool,
                       JGeometry::TVec3<f32>*)
{
}

void LineInLineXZ(const JGeometry::TVec2<f32>&, const JGeometry::TVec2<f32>&,
                  const JGeometry::TVec2<f32>&, const JGeometry::TVec2<f32>&)
{
}

void TMapCollisionData::intersectLine(const JGeometry::TVec3<f32>&,
                                      const JGeometry::TVec3<f32>&, bool,
                                      JGeometry::TVec3<f32>*) const
{
}
