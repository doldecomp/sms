#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>
#include <types.h>

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

#pragma dont_inline on
TBGCheckList* TMapCollisionData::allocCheckList(int type, int count)
{
	TBGCheckList* result;
	switch (type) {
	case 1:
		result = &unk2C[unk3C];
		unk3C -= count;
		break;
	case 2:
		result = &unk30[unk40];
		unk40 += count;
		break;
	default:
		result = &unk2C[unk38];
		unk38 += count;
		break;
	}
	return result;
}
#pragma dont_inline off

TBGCheckList* TMapCollisionData::getListRoot(int i, int j, int param_3,
                                             int param_4) const
{
	TBGCheckList* result;
	switch (param_3) {
	case 2:
	case 0:
		result = &unk14[j + i * unk8].unk0[param_4];
		break;
	case 1:
		result = &unk18[j + i * unk8].unk0[param_4];
		break;
	default:
		result = nullptr;
		break;
	}
	return result;
}

#pragma dont_inline on
static void addAfterPreNode(int param_1, int param_2, TBGCheckList* param_3,
                            TBGCheckList* param_4, int param_5)
{
	param_4->setNext(param_3->getNext());
	if (param_5 == 2) {
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
		if (param_2->mMaxY > next->unk8->mMaxY
		    || (param_2->mMaxY == next->unk8->mMaxY
		        && param_2->mMinY > next->unk8->mMinY))
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
		if (param_2->mMaxY < next->unk8->mMaxY
		    || (param_2->mMaxY == next->unk8->mMaxY
		        && param_2->mMinY < next->unk8->mMinY))
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
		if (param_2->mMinY > next->unk8->mMinY
		    || (param_2->mMinY == next->unk8->mMinY
		        && param_2->mMaxY > next->unk8->mMaxY))
			return param_1;
		param_1 = next;
	}
	return param_1;
}
#pragma dont_inline off

void TMapCollisionData::addCheckDataToList(int i, int j, int param_3,
                                           int param_4, TBGCheckData* param_5)
{
	TBGCheckList* list = getListRoot(i, j, param_3, param_4);
	TBGCheckList* list2;
	switch (param_4) {
	case 0:
		list2 = addGroundNode(list, param_5);
		break;
	case 1:
		list2 = addRoofNode(list, param_5);
		break;
	case 2:
		list2 = addWallNode(list, param_5);
		break;
	}
	TBGCheckList* list3 = allocCheckList(param_3, 1);
	list3->unk8         = param_5;
	addAfterPreNode(j, i, list2, list3, param_3);
}

template <class T> static inline T max(const T& a, const T& b)
{
	return a > b ? a : b;
}
template <class T> static inline T min(const T& a, const T& b)
{
	return b > a ? a : b;
}

bool TMapCollisionData::getGridArea(const TBGCheckData* param_1, int param_2,
                                    int* param_3, int* param_4, int* param_5,
                                    int* param_6)
{
	f32 minX
	    = min(min(param_1->mPoint2.x, param_1->mPoint3.x), param_1->mPoint1.x);
	f32 minZ
	    = min(min(param_1->mPoint2.z, param_1->mPoint3.z), param_1->mPoint1.z);

	f32 maxX
	    = max(param_1->mPoint1.x, max(param_1->mPoint2.x, param_1->mPoint3.x));
	f32 maxZ
	    = max(param_1->mPoint1.z, max(param_1->mPoint2.z, param_1->mPoint3.z));

	if (maxX < -unk0 || maxZ < -unk4 || minX > unk0 || minZ > unk4)
		return false;

	if (param_2 == 2) {
		minX -= 80.0f;
		minZ -= 80.0f;
		maxX += 80.0f;
		maxZ += 80.0f;
	}

	*param_3 = (minX + unk0) * 0.0009765625f;
	if (*param_3 < 0)
		*param_3 = 0;

	*param_5 = (maxX + unk0) * 0.0009765625f;
	if (*param_5 >= unk8)
		*param_5 = unk8 - 1;

	*param_4 = (minZ + unk0) * 0.0009765625f;
	if (*param_4 < 0)
		*param_4 = 0;

	*param_6 = (maxZ + unk0) * 0.0009765625f;
	if (*param_6 >= unkC)
		*param_6 = unkC - 1;

	return true;
}

void TMapCollisionData::addCheckDataToGrid(TBGCheckData* param_1, int param_2)
{
	int iVar7 = param_1->getPlaneType();
	int local_ac;
	int local_b4;
	int local_b0;
	int local_b8;
	if (getGridArea(param_1, iVar7, &local_ac, &local_b4, &local_b0,
	                &local_b8)) {
		for (int i = local_b4; i <= local_b8; ++i) {
			for (int j = local_ac; j <= local_b0; ++j) {
				if (param_2 == 1) {
					TBGCheckList* list = getListRoot(i, j, param_2, iVar7);
					TBGCheckList* list2;
					switch (iVar7) {
					case 0:
						list2 = addGroundNode(list, param_1);
						break;
					case 1:
						list2 = addRoofNode(list, param_1);
						break;
					case 2:
						list2 = addWallNode(list, param_1);
						break;
					}

					TBGCheckList* list3 = allocCheckList(param_2, 1);
					list3->unk8         = param_1;
					addAfterPreNode(j, i, list2, list3, param_2);
				} else if (iVar7 != 2) {
					int iVar1 = (i + 1) * 1024.0f - unk4;
					int iVar2 = (i * 1024.0f) - unk4;
					int iVar3 = (j + 1) * 1024.0f - unk0;
					int iVar4 = j * 1024.0f - unk0;
					if (polygonIsInGrid(iVar4, iVar2, iVar3, iVar1, param_1)) {
						TBGCheckList* list = getListRoot(i, j, param_2, iVar7);
						TBGCheckList* list2;
						switch (iVar7) {
						case 0:
							list2 = addGroundNode(list, param_1);
							break;
						case 1:
							list2 = addRoofNode(list, param_1);
							break;
						case 2:
							list2 = addWallNode(list, param_1);
							break;
						}
						TBGCheckList* list3 = allocCheckList(param_2, 1);
						list3->unk8         = param_1;
						addAfterPreNode(j, i, list2, list3, param_2);
					}
				} else {
					int iVar1 = (i + 1) * 1024.0f - unk4;
					int iVar2 = (i * 1024.0f) - unk4;
					int iVar3 = (j + 1) * 1024.0f - unk0;
					int iVar4 = j * 1024.0f - unk0;
					if (polygonIsInGrid(iVar4 - 80.0f, iVar2 - 80.0f,
					                    iVar3 + 80.0f, iVar1 + 80.0f,
					                    param_1)) {
						TBGCheckList* list = getListRoot(i, j, param_2, iVar7);
						TBGCheckList* list2;
						switch (iVar7) {
						case 0:
							list2 = addGroundNode(list, param_1);
							break;
						case 1:
							list2 = addRoofNode(list, param_1);
							break;
						case 2:
							list2 = addWallNode(list, param_1);
							break;
						}

						TBGCheckList* list3 = allocCheckList(param_2, 1);
						list3->unk8         = param_1;
						addAfterPreNode(j, i, list2, list3, param_2);
					}
				}
			}
		}
	}
}

void TMapCollisionData::removeCheckListNode(s32, s32) { }

void TMapCollisionData::updateCheckListNode(s32 param_1, s32 param_2,
                                            s32 param_3)
{
	TBGCheckListWarp* l   = &unk30[param_3];
	TBGCheckListWarp* r   = &unk30[param_1];
	TBGCheckListWarp* end = &unk30[param_3 + param_2];

	// TODO: I think this is a fakematch.
	u32 offset = ((u32)l - (u32)r);

	for (int i = param_1; i < param_1 + param_2; ++i) {
		TBGCheckListWarp* warp2 = &unk30[i];
		TBGCheckListWarp* warp3 = (TBGCheckListWarp*)warp2->getNext();
		if (l <= warp3 && warp3 < end)
			warp2->setNext((TBGCheckListWarp*)((char*)warp3 - offset));

		TBGCheckListWarp* warp4 = warp2->getPreNode();
		if (l <= warp4 && warp4 < end)
			warp2->setPreNode((TBGCheckListWarp*)((char*)warp4 - offset));
	}

	for (int i = param_1; i < param_1 + param_2; ++i) {
		TBGCheckListWarp* warp4 = &unk30[i];
		warp4->getPreNode()->setNext(warp4);
		if (warp4->getNext())
			warp4->getNext()->setPreNode(warp4);
	}
}

void printData(const TBGCheckListWarp*, int) { }

void printList(const TBGCheckList*) { }

void TMapCollisionData::removeCheckListData(u16 param_1, s32 count)
{
	TBGCheckListWarp* curr;
	int rangeEnd;
	u32 rangeStart;
	int i;

	if (!count)
		return;

	rangeStart = unk42[param_1];
	rangeEnd   = unk42[param_1] + count;

	unk42[param_1] = 9999;
	unk242         = param_1;

	for (i = rangeStart; i < rangeEnd; ++i) {
		curr = &unk30[i];
		curr->getPreNode()->setNext(curr->getNext());
		if (curr->getNext() != nullptr) {
			curr->getNext()->setPreNode(curr->getPreNode());
		}
		curr->unk8 = nullptr;
	}

	unk40 -= count;

	memmove(&unk30[rangeStart], &unk30[rangeEnd],
	        (unk40 + count - rangeStart) * sizeof(TBGCheckListWarp));

	updateCheckListNode(rangeStart, unk40 - rangeStart, rangeEnd);

	for (i = 0; i < 256; ++i)
		if (unk42[i] != 9999 && unk42[i] > rangeStart)
			unk42[i] -= count;
}
