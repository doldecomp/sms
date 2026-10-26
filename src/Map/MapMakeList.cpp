#include <Map/MapCollisionData.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <dolphin/types.h>

TBGCheckData* TMapCollisionData::allocCheckData(u32 count)
{
	TBGCheckData* result = &unk28[unk34];
	unk34 += count;
	return result;
}

u32 TMapCollisionData::getEntryID()
{
	unk42[unk242] = unk40;
	u16 result    = unk242;
	do {
		++unk242;
	} while (unk42[unk242] != 9999);
	return result;
}

TBGCheckList* TMapCollisionData::allocCheckList(int kind, int count)
{
	TBGCheckList* result;
	switch (kind) {
	case TMapCollisionBase::KIND_MOVE:
		result = &mCheckListPool[mMoveListTop];
		mMoveListTop -= count;
		break;
	case TMapCollisionBase::KIND_WARP:
		result = &mWarpCheckListPool[unk40];
		unk40 += count;
		break;
	default:
		result = &mCheckListPool[mStaticListUsed];
		mStaticListUsed += count;
		break;
	}
	return result;
}

TBGCheckList* TMapCollisionData::getListRoot(int i, int j, int kind,
                                             int planeType) const
{
	switch (kind) {
	case TMapCollisionBase::KIND_WARP:
	case TMapCollisionBase::KIND_STATIC:
		return &mStaticGrid[getCellIndex(j, i)].unk0[planeType];
	case TMapCollisionBase::KIND_MOVE:
		return &mMoveGrid[getCellIndex(j, i)].unk0[planeType];
	}
	return nullptr;
}

static void addAfterPreNode(int param_1, int param_2, TBGCheckList* param_3,
                            TBGCheckList* param_4, int kind)
{
	param_4->setNext(param_3->getNext());
	if (kind == TMapCollisionBase::KIND_WARP) {
		TBGCheckListWarp* casted = static_cast<TBGCheckListWarp*>(param_4);
		casted->unk10            = param_1;
		casted->unk12            = param_2;
		casted->setPreNode(param_3);
	}

	if (param_3->getNext() != nullptr) {
		param_3->getNext()->setPreNode(param_4);
	}

	param_3->setNext(param_4);
}

static TBGCheckList* addWallNode(TBGCheckList* param_1, TBGCheckData* param_2)
{
	TBGCheckList* next;
	while (param_1->getNext()) {
		next = param_1->getNext();
		if (param_2->mMaxY > next->mData->mMaxY
		    || (param_2->mMaxY == next->mData->mMaxY
		        && param_2->mMinY > next->mData->mMinY))
			return param_1;
		param_1 = next;
	}
	return param_1;
}

static TBGCheckList* addRoofNode(TBGCheckList* param_1, TBGCheckData* param_2)
{
	TBGCheckList* next;
	while (param_1->getNext()) {
		next = param_1->getNext();
		if (param_2->mMaxY < next->mData->mMaxY
		    || (param_2->mMaxY == next->mData->mMaxY
		        && param_2->mMinY < next->mData->mMinY))
			return param_1;
		param_1 = next;
	}
	return param_1;
}

static TBGCheckList* addGroundNode(TBGCheckList* param_1, TBGCheckData* param_2)
{
	TBGCheckList* next;
	while (param_1->getNext()) {
		next = param_1->getNext();
		if (param_2->mMinY > next->mData->mMinY
		    || (param_2->mMinY == next->mData->mMinY
		        && param_2->mMaxY > next->mData->mMaxY))
			return param_1;
		param_1 = next;
	}
	return param_1;
}

void TMapCollisionData::addCheckDataToList(int i, int j, int param_3,
                                           int planeType, TBGCheckData* param_5)
{
	TBGCheckList* list = getListRoot(i, j, param_3, planeType);
	TBGCheckList* list2;
	switch (planeType) {
	case BG_PLANE_TYPE_GROUND:
		list2 = addGroundNode(list, param_5);
		break;
	case BG_PLANE_TYPE_ROOF:
		list2 = addRoofNode(list, param_5);
		break;
	case BG_PLANE_TYPE_WALL:
		list2 = addWallNode(list, param_5);
		break;
	}
	TBGCheckList* list3 = allocCheckList(param_3, 1);
	list3->mData        = param_5;
	addAfterPreNode(j, i, list2, list3, param_3);
}

bool TMapCollisionData::getGridArea(const TBGCheckData* data, int plane_type,
                                    int* grid_min_x, int* grid_min_z,
                                    int* grid_max_x, int* grid_max_z)
{
	f32 minX = MsMin(data->getPoint1().x,
	                 MsMin(data->getPoint2().x, data->getPoint3().x));
	f32 minZ = MsMin(data->getPoint1().z,
	                 MsMin(data->getPoint2().z, data->getPoint3().z));

	f32 maxX = MsMax(data->getPoint1().x,
	                 MsMax(data->getPoint2().x, data->getPoint3().x));
	f32 maxZ = MsMax(data->getPoint1().z,
	                 MsMax(data->getPoint2().z, data->getPoint3().z));

	if (maxX < -mGridExtentX || maxZ < -mGridExtentY || minX > mGridExtentX
	    || minZ > mGridExtentY)
		return false;

	if (plane_type == BG_PLANE_TYPE_WALL) {
		minX -= 80.0f;
		minZ -= 80.0f;
		maxX += 80.0f;
		maxZ += 80.0f;
	}

	*grid_min_x = worldToGridX(minX);
	if (*grid_min_x < 0)
		*grid_min_x = 0;

	*grid_max_x = worldToGridX(maxX);
	if (*grid_max_x >= mGridSizeX)
		*grid_max_x = mGridSizeX - 1;

	*grid_min_z = worldToGridZ(minZ);
	if (*grid_min_z < 0)
		*grid_min_z = 0;

	*grid_max_z = worldToGridZ(maxZ);
	if (*grid_max_z >= mGridSizeZ)
		*grid_max_z = mGridSizeZ - 1;

	return true;
}

void TMapCollisionData::addCheckDataToGrid(TBGCheckData* data, int kind)
{
	int planeType = data->getPlaneType();
	int gridMinX;
	int gridMinZ;
	int gridMaxX;
	int gridMaxZ;
	if (getGridArea(data, planeType, &gridMinX, &gridMinZ, &gridMaxX,
	                &gridMaxZ)) {
		for (int z = gridMinZ; z <= gridMaxZ; ++z) {
			for (int x = gridMinX; x <= gridMaxX; ++x) {
				if (kind == TMapCollisionBase::KIND_MOVE) {
					addCheckDataToList(z, x, kind, planeType, data);
				} else if (planeType != BG_PLANE_TYPE_WALL) {
					f32 maxZ = gridToWorldZ(z + 1);
					f32 maxX = gridToWorldX(x + 1);
					f32 minZ = gridToWorldZ(z);
					f32 minX = gridToWorldX(x);
					if (polygonIsInGrid(minX, minZ, maxX, maxZ, data))
						addCheckDataToList(z, x, kind, planeType, data);
				} else {
					f32 maxZ = gridToWorldZ(z + 1);
					f32 maxX = gridToWorldX(x + 1);
					f32 minZ = gridToWorldZ(z);
					f32 minX = gridToWorldX(x);
					if (polygonIsInGrid(minX - 80.0f, minZ - 80.0f,
					                    maxX + 80.0f, maxZ + 80.0f, data))
						addCheckDataToList(z, x, kind, planeType, data);
				}
			}
		}
	}
}

void TMapCollisionData::removeCheckListNode(s32 start, s32 end)
{
	for (s32 i = start; i < end; ++i) {
		TBGCheckListWarp* curr = &mWarpCheckListPool[i];
		curr->getPreNode()->setNext(curr->getNext());
		if (curr->getNext() != nullptr)
			curr->getNext()->setPreNode(curr->getPreNode());
		curr->mData = nullptr;
	}
}

void TMapCollisionData::updateCheckListNode(s32 param_1, s32 param_2,
                                            s32 param_3)
{
	TBGCheckListWarp* l   = &mWarpCheckListPool[param_3];
	TBGCheckListWarp* r   = &mWarpCheckListPool[param_1];
	TBGCheckListWarp* end = &mWarpCheckListPool[param_3 + param_2];

	// TODO: I think this is a fakematch.
	u32 offset = ((u32)l - (u32)r);

	for (int i = param_1; i < param_1 + param_2; ++i) {
		TBGCheckListWarp* warp2 = &mWarpCheckListPool[i];
		TBGCheckListWarp* warp3 = (TBGCheckListWarp*)warp2->getNext();
		if (l <= warp3 && warp3 < end)
			warp2->setNext((TBGCheckListWarp*)((char*)warp3 - offset));

		TBGCheckListWarp* warp4 = warp2->getPreNode();
		if (l <= warp4 && warp4 < end)
			warp2->setPreNode((TBGCheckListWarp*)((char*)warp4 - offset));
	}

	for (int i = param_1; i < param_1 + param_2; ++i) {
		TBGCheckListWarp* warp4 = &mWarpCheckListPool[i];
		warp4->getPreNode()->setNext(warp4);
		if (warp4->getNext())
			warp4->getNext()->setPreNode(warp4);
	}
}

void printData(const TBGCheckListWarp*, int) { }

void printList(const TBGCheckList*) { }

void TMapCollisionData::removeCheckListData(u16 start, s32 count)
{
	int rangeEnd;
	u32 rangeStart;
	int i;

	if (!count)
		return;

	rangeStart = unk42[start];
	rangeEnd   = unk42[start] + count;

	unk42[start] = 9999;
	unk242       = start;

	removeCheckListNode(rangeStart, rangeEnd);

	unk40 -= count;

	memmove(&mWarpCheckListPool[rangeStart], &mWarpCheckListPool[rangeEnd],
	        (unk40 + count - rangeStart) * sizeof(TBGCheckListWarp));

	updateCheckListNode(rangeStart, unk40 - rangeStart, rangeEnd);

	for (i = 0; i < 256; ++i)
		if (unk42[i] != 9999 && unk42[i] > rangeStart)
			unk42[i] -= count;
}
