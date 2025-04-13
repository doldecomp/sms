#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>
#include <types.h>

void TMapCollisionData::removeCheckListData(u16 param_1, s32 count)
{
	TBGCheckListWarp* curr;
	int rangeEnd;
	u32 rangeStart;
	int i;

	if (!count)
		return;

	rangeStart = unk42[param_1];
	rangeEnd   = rangeStart + count;

	unk42[param_1] = 9999;
	unk242         = param_1;

	for (i = rangeStart; i < rangeEnd; ++i) {
		curr             = &unk30[i];
		curr->unkC->unk4 = curr->unk4;
		if (curr->unk4 != nullptr) {
			curr->unk4->setPreNode(curr->unkC);
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

void printList(const TBGCheckList*) { }

void printData(const TBGCheckListWarp*, int) { }

void TMapCollisionData::updateCheckListNode(s32 param_1, s32 param_2,
                                            s32 param_3)
{
}

void TMapCollisionData::removeCheckListNode(s32, s32) { }

void TMapCollisionData::addCheckDataToGrid(TBGCheckData*, int) { }

void TMapCollisionData::getGridArea(const TBGCheckData*, int, int*, int*, int*,
                                    int*)
{
}

void TMapCollisionData::addCheckDataToList(int, int, int, int, TBGCheckData*) {
}

static TBGCheckList* addGroundNode(TBGCheckList* param_1, TBGCheckData* param_2)
{
	TBGCheckList* next;
	while (param_1->unk4) {
		next = param_1->unk4;
		if (param_2->unk8 > next->unk8->unk8
		    || (param_2->unk8 == next->unk8->unk8
		        && param_2->unkC > next->unk8->unkC))
			return param_1;
		param_1 = next;
	}
	return param_1;
}

static TBGCheckList* addRoofNode(TBGCheckList* param_1, TBGCheckData* param_2)
{
	TBGCheckList* next;
	while (param_1->unk4) {
		next = param_1->unk4;
		if (param_2->unkC < next->unk8->unkC
		    || (param_2->unkC == next->unk8->unkC
		        && param_2->unk8 < next->unk8->unk8))
			return param_1;
		param_1 = next;
	}
	return param_1;
}

static TBGCheckList* addWallNode(TBGCheckList* param_1, TBGCheckData* param_2)
{
	TBGCheckList* next;
	while (param_1->unk4) {
		next = param_1->unk4;
		if (param_2->unkC > next->unk8->unkC
		    || (param_2->unkC == next->unk8->unkC
		        && param_2->unk8 > next->unk8->unk8))
			return param_1;
		param_1 = next;
	}
	return param_1;
}

static void addAfterPreNode(int param_1, int param_2, TBGCheckList* param_3,
                            TBGCheckList* param_4, int param_5)
{
	param_4->unk4 = param_3->unk4;
	if (param_5 == 2) {
		static_cast<TBGCheckListWarp*>(param_4)->unk10 = param_1;
		static_cast<TBGCheckListWarp*>(param_4)->unk12 = param_2;
		param_4->setPreNode(param_3);
	}

	if (param_3->unk4 != nullptr) {
		param_3->unk4->setPreNode(param_4);
	}

	param_3->unk4 = param_4;
}

void TMapCollisionData::getListRoot(int, int, int, int) const { }

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

u32 TMapCollisionData::getEntryID()
{
	unk42[unk242] = unk40;
	u16 result    = unk242;
	do {
		++unk242;
	} while (unk42[unk242] != 9999);
	return result;
}

TBGCheckData* TMapCollisionData::allocCheckData(u32 count)
{
	TBGCheckData* result = &unk28[unk34];
	unk34 += count;
	return result;
}
