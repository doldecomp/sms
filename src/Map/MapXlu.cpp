#include <Map/MapXlu.hpp>
#include <Map/MapModel.hpp>
#include <Map/Map.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// The root joint's child count is read raw at both loop headers below: routing
// it through TJointObj::getChildrenNum() costs one inline temporary per use
// (16 bytes here, 8 in changeXluJoint) that retail's frames do not have, while
// getRootJointModel() and getChild() must stay accessors (spelling either of
// them raw takes both frames 0x18/0x20 under). A non-const getChildrenNum()
// overload does not help, so retail read the field.
void TMapXlu::changeNormalJoint()
{
	for (int i = 0; i < gpMap->getRootJointModel()->mChildrenNum; ++i)
		gpMap->getRootJointModel()->getChild(i)->stand();

	for (int i = 0; i < mPrioGroupNum; ++i)
		for (int j = 0; j < mPrioGroups[i].mObjectNum; ++j)
			gpMap->getRootJointModel()
			    ->getChild(mPrioGroups[i].mChildIdx[j])
			    ->getChild(mPrioGroups[i].mGrandchildIdx[j])
			    ->sit();
}

bool TMapXlu::changeXluJoint(int prio)
{
	if (prio >= mPrioGroupNum)
		return false;

	for (int i = 0; i < gpMap->getRootJointModel()->mChildrenNum; ++i)
		gpMap->getRootJointModel()->getChild(i)->sit();

	for (int i = 0; i < mPrioGroups[prio].mObjectNum; ++i)
		gpMap->getRootJointModel()
		    ->getChild(mPrioGroups[prio].mChildIdx[i])
		    ->getChild(mPrioGroups[prio].mGrandchildIdx[i])
		    ->stand();

	return true;
}

void TMapXlu::init(JSUMemoryInputStream& stream)
{
	s32 tmp;
	stream >> tmp;
	mPrioGroupNum = tmp;
	if (mPrioGroupNum != 0) {
		mPrioGroups = new TXluPrioGroup[mPrioGroupNum];
		for (int i = 0; i < mPrioGroupNum; ++i) {
			TXluPrioGroup& entry = mPrioGroups[i];
			stream >> tmp;
			entry.mObjectNum     = tmp;
			entry.mChildIdx      = new u32[entry.mObjectNum];
			entry.mGrandchildIdx = new u32[entry.mObjectNum];
			for (int j = 0; j < entry.mObjectNum; ++j) {
				stream >> tmp;
				entry.mChildIdx[j] = tmp;
				stream >> tmp;
				entry.mGrandchildIdx[j] = tmp;
			}
		}
	}
}

TMapXlu::TMapXlu()
    : mPrioGroupNum(0)
    , mPrioGroups(nullptr)
{
}
