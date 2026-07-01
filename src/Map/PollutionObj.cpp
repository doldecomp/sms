#include <Map/PollutionObj.hpp>
#include <Map/PollutionLayer.hpp>
#include <Map/Map.hpp>
#include <Map/MapEventSink.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <types.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static bool is_near(f32 a, f32 b)
{
	if (a - 30.0f <= b && b <= 30.0f + a)
		return true;
	return false;
}

static bool is_near(f32 h00, f32 h01, f32 h10, f32 h11)
{
	if (is_near(h00, h01) && is_near(h01, h10) && is_near(h10, h11))
		return true;
	return false;
}

u8 TPollutionObj::getDepthFromMap(int x, int z)
{
	// TODO: inlines are wrong here!
	(void)0;
	f32 worldX = mLayer->getWorldPosX(x);
	f32 worldZ = mLayer->getWorldPosZ(z);

	f32 minX = worldX - 5.0f;
	f32 maxX = (worldX + mLayer->getTexelSize()) + 5.0f;
	f32 minZ = worldZ - 5.0f;
	f32 maxZ = (worldZ + mLayer->getTexelSize()) + 5.0f;

	f32 texelSz = mLayer->getTexelSize();

	const TBGCheckData* tmp;
	f32 h00 = gpMap->checkGround(minX, 9999999.0f, minZ, &tmp);
	f32 h10 = gpMap->checkGround(maxX, 9999999.0f, minZ, &tmp);
	f32 h01 = gpMap->checkGround(minX, 9999999.0f, maxZ, &tmp);
	f32 h11 = gpMap->checkGround(maxX, 9999999.0f, maxZ, &tmp);

	if (is_near(h00, h01, h10, h11)) {
		f32 half = 0.5f * texelSz;
		return mLayer->worldToDepth(
		    gpMap->checkGround(worldX + half, 9999999.0f, worldZ + half, &tmp));
	}
	return 0xFF;
}

void TPollutionObj::updateDepthMap()
{
	for (int z = mMinT; z < mMaxT; ++z) {
		for (int x = mMinS; x < mMaxS; ++x) {
			u8 depth = getDepthFromMap(x, z);
			mLayer->getPos().setDepth(x, z, depth);
		}
	}
}

bool TPollutionObj::isCleaned() const
{
	if (mCounter < TMapEventSink::mCleanedDegree)
		return true;
	return false;
}

void TPollutionObj::initAreaInfo(TPollutionLayer* layer)
{
	mLayer         = layer;
	const Vec& min = getJoint()->getMin();
	const Vec& max = getJoint()->getMax();

	mMinS = mLayer->getTexPosS(min.x);
	mMaxS = mLayer->getTexPosS(max.x);
	mMinT = mLayer->getTexPosT(min.z);
	mMaxT = mLayer->getTexPosT(max.z);

	if (mMinS < 0)
		mMaxS = 0;
	if (mMinT < 0)
		mMaxT = 0;
	if (mMaxS > mLayer->getTexWidth())
		mMaxS = mLayer->getTexWidth();
	if (mMaxT > mLayer->getTexHeight())
		mMaxT = mLayer->getTexHeight();

	for (int i = 0; i < getChildrenNum(); ++i)
		getChild(i)->initAreaInfo(layer);
}

TPollutionObj::TPollutionObj()
    : mMinS(0)
    , mMaxS(0)
    , mMinT(0)
    , mMaxT(0)
    , mCounter(0)
    , mLayer(nullptr)
{
}
