#include <Map/Map.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapModel.hpp>
#include <Map/MapWarp.hpp>
#include <Map/MapXlu.hpp>
#include <Map/MapCollisionEntry.hpp>

// rouge include needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>

static void initStage();

TMap::~TMap() { }

TMap::TMap(const char* name)
    : JDrama::TViewObj(name)
{
	mCollisionData = new TMapCollisionData;
	mModelManager  = new TMapModelManager("地形モデル管理");
	mWarp          = new TMapWarp;
	mXlu           = new TMapXlu;
	unk20          = 0;

	gpMap = this;
}

void TMap::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	mXlu->init(stream);
	mModelManager->init();
	mCollisionData->init(stream);
	mWarp->initModel();
	mWarp->init(stream);
	mModelManager->mCollision->setUp();
}

void TMap::loadAfter()
{
	JDrama::TViewObj::loadAfter();
	initStage();
}

void TMap::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 1) {
		update();
		mCollisionData->initMoveCollision();
		mWarp->watchToWarp();
	}

	if (param_1 & 0x200) {
		if ((param_1 & 0x2000000)) {
			if (!mXlu->changeXluJoint(1))
				return;
		} else if ((param_1 & 0x4000000)) {
			if (!mXlu->changeXluJoint(0))
				return;
		} else {
			mXlu->changeNormalJoint();
		}
	}

	if (param_1 & 8)
		draw(param_1, param_2);

	mModelManager->perform(param_1, param_2);
}

void TMap::changeModel(s16 param_1) const { mWarp->changeModel(param_1); }

f32 TMap::checkGround(f32 x, f32 y, f32 z, const TBGCheckData** param_4) const
{
	return mCollisionData->checkGround(x, y, z, 0, param_4);
}

f32 TMap::checkGround(const JGeometry::TVec3<f32>& pos,
                      const TBGCheckData** param_2) const
{
	return mCollisionData->checkGround(pos.x, pos.y, pos.z, 0, param_2);
}

f32 TMap::checkGroundExactY(const JGeometry::TVec3<f32>& pos,
                            const TBGCheckData** param_2) const
{
	return mCollisionData->checkGround(pos.x, pos.y - -78.0f, pos.z, 0,
	                                   param_2);
}

f32 TMap::checkGroundExactY(f32 x, f32 y, f32 z,
                            const TBGCheckData** param_4) const
{
	return mCollisionData->checkGround(x, y - -78.0f, z, 0, param_4);
}

f32 TMap::checkGroundIgnoreWaterSurface(const JGeometry::TVec3<f32>& pos,
                                        const TBGCheckData** param_2) const
{
	return mCollisionData->checkGround(pos.x, pos.y, pos.z, 1, param_2);
}

f32 TMap::checkGroundIgnoreWaterSurface(f32 x, f32 y, f32 z,
                                        const TBGCheckData** param_4) const
{
	return mCollisionData->checkGround(x, y, z, 1, param_4);
}

f32 TMap::checkGroundIgnoreWaterThrough(f32 x, f32 y, f32 z,
                                        const TBGCheckData** param_4) const
{
	return mCollisionData->checkGround(x, y, z, 4, param_4);
}

f32 TMap::checkRoof(const JGeometry::TVec3<f32>& pos,
                    const TBGCheckData** param_2) const
{
	return mCollisionData->checkRoof(pos.x, pos.y, pos.z, 0, param_2);
}

f32 TMap::checkRoof(f32 x, f32 y, f32 z, const TBGCheckData** param_4) const
{
	return mCollisionData->checkRoof(x, y, z, 0, param_4);
}

f32 TMap::checkRoofIgnoreWaterThrough(f32 x, f32 y, f32 z,
                                      const TBGCheckData** param_4) const
{
	return mCollisionData->checkRoof(x, y, z, 4, param_4);
}

bool TMap::isTouchedWallsAndMoveXZ(TBGWallCheckRecord* param_1) const
{
	return mCollisionData->checkWalls(param_1) != 0 ? true : false;
}

bool TMap::isTouchedOneWallAndMoveXZ(f32* x, f32 y, f32* z, f32 param_4) const
{
	TBGWallCheckRecord record(*x, y, *z, param_4, 1, 0);

	int r = mCollisionData->checkWalls(&record);
	if (r != 0 ? true : false) {
		*x = record.unk0.x;
		*z = record.unk0.z;
		return true;
	} else {
		return false;
	}
}

bool TMap::isTouchedOneWall(f32 x, f32 y, f32 z, f32 param_4) const
{
	return isTouchedOneWallAndMoveXZ(&x, y, &z, param_4);
}

bool TMap::isTouchedOneWall(const JGeometry::TVec3<f32>&, f32) const { }

void TMap::intersectLine(const JGeometry::TVec3<f32>& param_1,
                         const JGeometry::TVec3<f32>& param_2, bool param_3,
                         JGeometry::TVec3<f32>* param_4) const
{
	mCollisionData->intersectLine(param_1, param_2, param_3, param_4);
}

bool TMap::isInArea(f32 param_1, f32 param_2) const
{
	if (-mCollisionData->unk0 < param_1 && param_1 < mCollisionData->unk0
	    && -mCollisionData->unk4 < param_2 && param_2 < mCollisionData->unk4)
		return true;

	return false;
}

TBGCheckData* TMap::getIllegalCheckData()
{
	return &TMapCollisionData::mIllegalCheckData;
}

void TMap::update() { }

void updateRicco() { }

void TMap::updateMonte() { }

void TMap::updateDelfino() { }

static void initStage() { }

static void initStageCommon() { }

static void initDolpic() { }

static void initBianco() { }

static void initPinnaBeach() { }

static void initPinnaParco() { }

static void initMare() { }

static void initMonte() { }

static void initSirena() { }

static void initOption() { }
