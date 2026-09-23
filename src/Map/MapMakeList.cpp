#include <Map/MapCollisionData.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapData.hpp>
#include <algorithm>
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

TBGCheckList* TMapCollisionData::allocCheckList(int kind, int count)
{
	TBGCheckList* result;
	switch (kind) {
	case TMapCollisionBase::KIND_MOVE:
		result = &unk2C[unk3C];
		unk3C -= count;
		break;
	case TMapCollisionBase::KIND_WARP:
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

TBGCheckList* TMapCollisionData::getListRoot(int i, int j, int kind,
                                             int param_4) const
{
	switch (kind) {
	case TMapCollisionBase::KIND_WARP:
	case TMapCollisionBase::KIND_STATIC:
		return &unk14[j + i * unk8].unk0[param_4];
	case TMapCollisionBase::KIND_MOVE:
		return &unk18[j + i * unk8].unk0[param_4];
	default:
		return nullptr;
	}
}

// addCheckDataToGrid reaches these walkers, allocCheckList and addAfterPreNode
// at depth 2 through the inlined UNUSED addCheckDataToList, so they stay calls
// there; the out-of-line addCheckDataToList expands them at depth 1.
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

// TODO: 99.5%. Two residues left.
//  - Frame 0x70 against retail's 0xe0: 112 bytes of dead low region below the
//    single int-to-float conversion pair, which is the only referenced slot on
//    either side. std::min/max' `const T&` parameters bind nothing here; the
//    same templates taking their arguments by value are +0x28 and cost three
//    register pairs, so the carrier is not the parameter form.
//  - The Z inner min/max load p2.z and p3.z into the opposite registers
//    (retail f6 = p3.z, f5 = p2.z); the X pair, with the identical spelling,
//    comes out right, so this is an FPR permutation and not an argument-order
//    difference (both compare a = p2).
bool TMapCollisionData::getGridArea(const TBGCheckData* param_1, int param_2,
                                    int* param_3, int* param_4, int* param_5,
                                    int* param_6)
{
	f32 minX = std::min(param_1->mPoint1.x,
	                    std::min(param_1->mPoint2.x, param_1->mPoint3.x));
	f32 minZ = std::min(param_1->mPoint1.z,
	                    std::min(param_1->mPoint3.z, param_1->mPoint2.z));

	f32 maxX
	    = std::max(param_1->mPoint1.x, std::max(param_1->mPoint2.x, param_1->mPoint3.x));
	f32 maxZ
	    = std::max(param_1->mPoint1.z, std::max(param_1->mPoint3.z, param_1->mPoint2.z));

	if (maxX < -mGridExtentX || maxZ < -mGridExtentY || minX > mGridExtentX
	    || minZ > mGridExtentY)
		return false;

	if (param_2 == 2) {
		minX -= 80.0f;
		minZ -= 80.0f;
		maxX += 80.0f;
		maxZ += 80.0f;
	}

	*param_3 = (minX + mGridExtentX) * 0.0009765625f;
	if (*param_3 < 0)
		*param_3 = 0;

	*param_5 = (maxX + mGridExtentX) * 0.0009765625f;
	if (*param_5 >= unk8)
		*param_5 = unk8 - 1;

	*param_4 = (minZ + mGridExtentY) * 0.0009765625f;
	if (*param_4 < 0)
		*param_4 = 0;

	*param_6 = (maxZ + mGridExtentY) * 0.0009765625f;
	if (*param_6 >= unkC)
		*param_6 = unkC - 1;

	return true;
}

// TODO: 93.1%. Retail calls the UNUSED addCheckDataToList at each of the
// three sites, which expands the UNUSED getListRoot at depth 2 and keeps the
// list helpers as calls.
//  - Frame 0xd0 against retail's 0x108: 0x38 bytes of dead low region below
//    the four getGridArea outputs (retail 0x50..0x5c).
//  - The hoisted constants take f31 = 1024.0f, f30 = the int-conversion
//    magic in retail; ours are swapped.
//  - The int<->float conversion slots and their scheduling around the two
//    polygonIsInGrid calls differ (retail's temporaries are allocated in
//    z1, x1, z0, x0 order, the call's right-to-left argument order).
//    Declaring the four ints in other orders, spelling the products
//    constant-first or passing the conversions inline were inert or worse.
//  - addCheckDataToList out of line is 0x270 against the map's 0x27c.
void TMapCollisionData::addCheckDataToGrid(TBGCheckData* param_1, int kind)
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
				if (kind == TMapCollisionBase::KIND_MOVE) {
					addCheckDataToList(i, j, kind, iVar7, param_1);
				} else if (iVar7 != 2) {
					int iVar1 = (i + 1) * 1024.0f - mGridExtentY;
					int iVar2 = (i * 1024.0f) - mGridExtentY;
					int iVar3 = (j + 1) * 1024.0f - mGridExtentX;
					int iVar4 = j * 1024.0f - mGridExtentX;
					if (polygonIsInGrid(iVar4, iVar2, iVar3, iVar1, param_1)) {
						addCheckDataToList(i, j, kind, iVar7, param_1);
					}
				} else {
					int iVar1 = (i + 1) * 1024.0f - mGridExtentY;
					int iVar2 = (i * 1024.0f) - mGridExtentY;
					int iVar3 = (j + 1) * 1024.0f - mGridExtentX;
					int iVar4 = j * 1024.0f - mGridExtentX;
					if (polygonIsInGrid(iVar4 - 80.0f, iVar2 - 80.0f,
					                    iVar3 + 80.0f, iVar1 + 80.0f,
					                    param_1)) {
						addCheckDataToList(i, j, kind, iVar7, param_1);
					}
				}
			}
		}
	}
}

void TMapCollisionData::removeCheckListNode(s32, s32) { }

// TODO: instruction-exact, frame 0x48 vs 0x60 (24 low bytes). A getWarpNode
// fork saturates at +8 here and costs every callee-saved register beyond that;
// the trial table is at MapCollisionData.hpp's rejected declaration.
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

// TODO: instruction-exact, frame 0x48 vs 0x70 (40 low bytes). The register
// residue is fixed (the sentinel store goes through setEntryStart); the
// remaining low region has no candidate -- see the trial table at
// MapCollisionData.hpp's rejected getWarpNode.
void TMapCollisionData::removeCheckListData(u16 start, s32 count)
{
	TBGCheckListWarp* curr;
	int rangeEnd;
	u32 rangeStart;
	int i;

	if (!count)
		return;

	rangeStart = unk42[start];
	rangeEnd   = unk42[start] + count;

	setEntryStart(start, 9999);
	unk242       = start;

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
