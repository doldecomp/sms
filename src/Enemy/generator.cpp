#include <Enemy/Generator.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Graph.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Strategic/Strategy.hpp>
#include <dolphin/mtx.h>

TGenerator::TGenerator(const char* name)
    : JDrama::TViewObj(name)
{
	mManagerName = nullptr;
	mManager     = nullptr;
	mGraphName   = nullptr;
	mGraph       = nullptr;
	mInterval    = 1;
	mTimer       = 0;
}

void TGenerator::load(JSUMemoryInputStream& stream)
{
	// TODO: 89.9% - logic exact. Residual = the 3 discarded reads land in 3
	// stack slots vs the target's 1 (stack-coalescing/frame-padding family).
	JDrama::TNameRef::load(stream);

	stream >> mPos.x >> mPos.y >> mPos.z;
	stream >> mRot.x >> mRot.y >> mRot.z;

	f32 unused;
	stream >> unused;
	stream >> unused;
	stream >> unused;
	stream.readString();

	s32 count;
	stream >> count;
	for (int i = 0; i < count; ++i) {
		s32 dummy;
		stream >> dummy;
		stream.readString();
	}

	mGraphName   = stream.readString();
	mManagerName = stream.readString();

	stream >> mInterval;
	mTimer = (s32)(mInterval * MsRandF());

	gpConductor->registerGenerator(this);
}

void TGenerator::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if (flags & 1) {
		mTimer -= 1;
		if (mTimer < 0)
			mTimer = mInterval;

		if (mTimer == 0) {
			if (mManager == nullptr)
				mManager = (TEnemyManager*)gpConductor->getManagerByName(
				    mManagerName);

			TSpineEnemy* enemy = mManager->getFarOutEnemy();
			if (enemy != nullptr) {
				if (mGraph == nullptr)
					mGraph = gpConductor->getGraphByName(mGraphName);

				enemy->getTracer()->setGraph(mGraph);

				JGeometry::TVec3<f32> rot(0.0f, 0.0f, 0.0f);
				JGeometry::TVec3<f32> vel(0.0f, 4.0f, 0.0f);

				Mtx m;
				MsMtxSetRotRPH(m, mRot.x, mRot.y, mRot.z);
				PSMTXMultVec(m, &vel, &vel);

				enemy->resetSRTV(mPos, rot, enemy->mScaling, vel);
			}
		}
	}
}

TOneShotGenerator::TOneShotGenerator(const char* name)
    : THitActor(name)
{
	mManagerName = nullptr;
	mManager     = nullptr;
	mGraphName   = nullptr;
	mGraph       = nullptr;
	mCount       = 1;
}

void TOneShotGenerator::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);
	mGraphName   = stream.readString();
	mManagerName = stream.readString();
}

void TOneShotGenerator::loadAfter()
{
	if (mCollisions == nullptr) {
		mManager = (TEnemyManager*)gpConductor->getManagerByName(mManagerName);
		if (mGraph == nullptr)
			mGraph = gpConductor->getGraphByName(mGraphName);

		initHitActor(0x02000001, 1, 0x80000000, 80.0f, 120.0f, 80.0f, 120.0f);
		mHitFlags &= ~1;

		JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
		    ->getChildren()
		    .push_back(this);
		gpConductor->registerOtherObj(this);
	}
}

BOOL TOneShotGenerator::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->isActorType(0x01000001)) {
		if (mCount != 0) {
			TSpineEnemy* enemy = mManager->getFarOutEnemy();
			if (enemy != nullptr) {
				enemy->getTracer()->setGraph(mGraph);

				JGeometry::TVec3<f32> rot(0.0f, 0.0f, 0.0f);
				JGeometry::TVec3<f32> vel(0.0f, 4.0f, 0.0f);

				Mtx m;
				MsMtxSetRotRPH(m, mRotation.x, mRotation.y, mRotation.z);
				PSMTXMultVec(m, &vel, &vel);

				enemy->resetSRTV(mPosition, rot, enemy->mScaling, vel);
			}
			mCount = 0;
		}
		return TRUE;
	}
	return FALSE;
}
