#include <Map/PollutionObj.hpp>
#include <Map/PollutionLayer.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <types.h>

// rouge include needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>

TPollutionObj::TPollutionObj()
    : unk20(0)
    , unk24(0)
    , unk28(0)
    , unk2C(0)
    , unk30(0)
    , unk34(nullptr)
{
}

void TPollutionObj::initAreaInfo(TPollutionLayer* layer)
{
	unk34          = layer;
	const Vec& min = mJoint->getMin();
	const Vec& max = mJoint->getMax();

	unk20 = unk34->getTexPosS(min.x);
	unk24 = unk34->getTexPosS(max.x);
	unk28 = unk34->getTexPosT(min.z);
	unk2C = unk34->getTexPosT(max.z);

	if (unk20 < 0)
		unk24 = 0;
	if (unk28 < 0)
		unk2C = 0;
	if (unk24 > unk34->unk5C.mWidth)
		unk24 = unk34->unk5C.mWidth;
	if (unk2C > unk34->unk5C.mHeight)
		unk2C = unk34->unk5C.mHeight;

	for (int i = 0; i < mChildrenNum; ++i)
		((TPollutionObj*)mChildren[i])->initAreaInfo(layer);
}

void TPollutionObj::isCleaned() const { }

void TPollutionObj::updateDepthMap() { }

void TPollutionObj::getDepthFromMap(int, int) { }

void is_near(float, float, float, float) { }

void is_near(float, float) { }
