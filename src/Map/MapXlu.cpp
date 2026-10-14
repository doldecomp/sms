#include <Map/MapXlu.hpp>
#include <Map/MapModel.hpp>
#include <Map/Map.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TMapXlu::changeNormalJoint()
{
	for (int i = 0; i < gpMap->getRootJointModel()->getChildrenNum(); ++i)
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

	for (int i = 0; i < gpMap->getRootJointModel()->getChildrenNum(); ++i)
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
