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

// Pragma residue (sweep 360): protects TMapCollisionData::addCheckDataToGrid
// (91.4 -> 11.2), the only caller of these list helpers. allocCheckList is 10
// statements against the depth-1 budget of 14.
#pragma dont_inline on
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
#pragma dont_inline off

TBGCheckList* TMapCollisionData::getListRoot(int i, int j, int kind,
                                             int param_4) const
{
	TBGCheckList* result;
	switch (kind) {
	case TMapCollisionBase::KIND_WARP:
	case TMapCollisionBase::KIND_STATIC:
		result = &unk14[j + i * unk8].unk0[param_4];
		break;
	case TMapCollisionBase::KIND_MOVE:
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
	                    std::min(param_1->mPoint2.z, param_1->mPoint3.z));

	f32 maxX
	    = std::max(param_1->mPoint1.x, std::max(param_1->mPoint2.x, param_1->mPoint3.x));
	f32 maxZ
	    = std::max(param_1->mPoint1.z, std::max(param_1->mPoint2.z, param_1->mPoint3.z));

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

// TODO: 91.4%. Two independent residues.
//  - Frame 0xd8 vs retail's 0x108. The 48 bytes are priced: a dead
//    *non-trivial* 12-byte local (a TVec3, say) inside `getListRoot` lands
//    0x108 exactly with no instruction change (8 bytes gives 0x100, 16 gives
//    0x118), and `getListRoot` is UNUSED in the map, so it is a legal carrier
//    per the batch-69 rule. Nothing in a "fetch the list root" function wants
//    a 12-byte object, so it is measured and not committed.
//  - In each of the three copies of the addCheckDataToList block retail's
//    inlined getListRoot accumulates the address straight into r3 (`lwz r4,
//    0x14(r31)` for the base, `add r3, r0, r24`, `add r3, r4, r3`) where ours
//    keeps the base in r3, accumulates in r0 and adds `mr r3, r0`. Measured:
//    splitting `TBGCheckList* list;` from its assignment is +0; spelling
//    `addGroundNode(getListRoot(...), param_1)` in each switch arm expands
//    getListRoot three times per block (46.7%, 511 instructions); reusing one
//    `list2` variable for the root and the node is 86.8% and +10 instructions.
//    The addGroundNode/addRoofNode/addWallNode trio really are file-static
//    free functions (the map mangles them `__FP12TBGCheckList...`), which is
//    why the root has to reach r3 and not r4.
//  - The UNUSED out-of-line addCheckDataToList is 0x27c against our 0x124,
//    but that is the same depth artifact as enemyAttachment's `generate`: at
//    depth 1 the out-of-line copy expands helpers that the three inlined
//    copies here reach at depth 2 and call. It is not evidence of missing
//    statements.
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
					TBGCheckList* list = getListRoot(i, j, kind, iVar7);
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

					TBGCheckList* list3 = allocCheckList(kind, 1);
					list3->unk8         = param_1;
					addAfterPreNode(j, i, list2, list3, kind);
				} else if (iVar7 != 2) {
					int iVar1 = (i + 1) * 1024.0f - mGridExtentY;
					int iVar2 = (i * 1024.0f) - mGridExtentY;
					int iVar3 = (j + 1) * 1024.0f - mGridExtentX;
					int iVar4 = j * 1024.0f - mGridExtentX;
					if (polygonIsInGrid(iVar4, iVar2, iVar3, iVar1, param_1)) {
						TBGCheckList* list = getListRoot(i, j, kind, iVar7);
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
						TBGCheckList* list3 = allocCheckList(kind, 1);
						list3->unk8         = param_1;
						addAfterPreNode(j, i, list2, list3, kind);
					}
				} else {
					int iVar1 = (i + 1) * 1024.0f - mGridExtentY;
					int iVar2 = (i * 1024.0f) - mGridExtentY;
					int iVar3 = (j + 1) * 1024.0f - mGridExtentX;
					int iVar4 = j * 1024.0f - mGridExtentX;
					if (polygonIsInGrid(iVar4 - 80.0f, iVar2 - 80.0f,
					                    iVar3 + 80.0f, iVar1 + 80.0f,
					                    param_1)) {
						TBGCheckList* list = getListRoot(i, j, kind, iVar7);
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

						TBGCheckList* list3 = allocCheckList(kind, 1);
						list3->unk8         = param_1;
						addAfterPreNode(j, i, list2, list3, kind);
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
