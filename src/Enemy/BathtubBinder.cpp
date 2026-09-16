#include <Enemy/BathtubBinder.hpp>
#include <Strategic/LiveActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

#include <M3DUtil/InfectiousStrings.hpp> // rogue include needed for matching rodata

TBathtubBinder::TBathtubBinder()
    : TBinder()
{
	mBathtub = nullptr;
	mWater   = nullptr;
}

TBathtubBinder::~TBathtubBinder() { }

bool TBathtubBinder::init(f32 front_dist, f32 front_margin, f32 back_dist,
                          f32 back_margin, f32 height_offset)
{
	mBathtub = JDrama::TNameRefGen::search<TBathtub>("バスタブ");
	mWater   = JDrama::TNameRefGen::search<TBathWaterManager>("バスタブの水");

	mHeightOffset = height_offset;
	mFrontDist    = front_dist;
	mFrontMargin  = front_margin;
	mBackDist     = back_dist;
	mBackMargin   = back_margin;
	mBackRatio    = mBackMargin / (mFrontMargin + mBackMargin);

	if (mBathtub == nullptr)
		mWater = nullptr;

	return mBathtub != nullptr;
}

void TBathtubBinder::bind(TLiveActor* actor)
{
	if (mBathtub == nullptr || !mBathtub->unk29A)
		float_(actor);
}

void TBathtubBinder::float_(TLiveActor* actor)
{
	if (mWater == nullptr)
		return;

	Mtx rot;
	MsMtxSetRotRPH(rot, actor->mRotation.x, actor->mRotation.y,
	               actor->mRotation.z);

	f32 frontX = rot[0][2] * mFrontDist + actor->mPosition.x;
	f32 frontZ = rot[2][2] * mFrontDist + actor->mPosition.z;
	if (mBathtub != nullptr)
		clampToTub(&frontX, &frontZ, mFrontMargin);
	f32 frontY = mHeightOffset + mWater->getWaterHeight(frontX, frontZ);

	f32 backX = rot[0][2] * -mBackDist + actor->mPosition.x;
	f32 backZ = rot[2][2] * -mBackDist + actor->mPosition.z;
	if (mBathtub != nullptr)
		clampToTub(&backX, &backZ, mBackMargin);
	f32 backY = mHeightOffset + mWater->getWaterHeight(backX, backZ);

	f32 dx = frontX - backX;
	f32 dz = frontZ - backZ;
	f32 dy = frontY - backY;

	f32 y        = actor->mPosition.y;
	actor->mPosition.y = 0.2f * ((mBackRatio * dy + backY) - y) + y;

	if (dx * dx + dy * dy + dz * dz > 0.0000038146973f) {
		f32 flat  = JGeometry::TUtil<f32>::sqrt(dx * dx + dz * dz);
		f32 pitch = matan(flat, dy) * (360.0f / 65536.0f);
		if (pitch < -15.0f)
			pitch = -15.0f;
		else if (pitch > 15.0f)
			pitch = 15.0f;

		f32 rx = actor->mRotation.x;
		actor->mRotation.x = 0.1f * (pitch - rx) + rx;
		actor->mRotation.z = 0.0f;

		if (mBathtub != nullptr) {
			f32 margin = 0.5f * (mFrontMargin + mBackMargin);
			clampToTub(&actor->mPosition.x, &actor->mPosition.z, margin);
			f32 floorY = mBathtub->getBathtubData().getThing().y;
			if (actor->mPosition.y < floorY)
				actor->mPosition.y = floorY;
		}
	}
}
