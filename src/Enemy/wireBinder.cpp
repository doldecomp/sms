#include <Enemy/WireBinder.hpp>
#include <Map/MapWire.hpp>
#include <Map/MapWireManager.hpp>

bool TWireBinder::init(const JGeometry::TVec3<f32>& param_1)
{
	return reset(param_1);
}

bool TWireBinder::reset(const JGeometry::TVec3<f32>& param_1)
{
	JGeometry::TVec3<f32> local24;
	JGeometry::TVec3<f32> local30;

	mWireNumber = gpMapWireManager->getWireNo(param_1);
	if (mWireNumber == -1)
		return false;

	TMapWire* wire = gpMapWireManager->getWire(mWireNumber);

	local24 = wire->getStartPoint();
	local30 = wire->getEndPoint();
	local30 -= local24;

	mDir.normalize(local30);
	return true;
}

void TWireBinder::bind(TLiveActor* actor)
{
	JGeometry::TVec3<f32> unk_14;
	actor->getNextFramePosition(unk_14);

	JGeometry::TVec3<f32> unk_20;
	getPoint(&unk_20, unk_14);

	if (isnan(unk_20.x) || isnan(unk_20.y) || isnan(unk_20.z))
		unk_20.set(actor->getPosition());

	f32 fVar = 0.05f + unk_20.y;

	if (unk_14.y <= fVar) {
		actor->offLiveFlag(LIVE_FLAG_AIRBORNE);
	} else {
		actor->onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	actor->setLinearVelocity(unk_20 - actor->getPosition());
}

JGeometry::TVec3<f32>
TWireBinder::getDirAtPos(const JGeometry::TVec3<f32>& param_1,
                         f32 param_2) const
{
	f32 posInWire = getRangePos(param_1);

	f32 fVar1;
	f32 fVar2;

	if (posInWire <= 0.01f && param_2 < 0.0f
	    || 0.99f <= posInWire && 0.0f < param_2) {
		fVar1 = posInWire - 0.01f * param_2;
		fVar2 = posInWire;
	} else {
		fVar1 = posInWire;
		fVar2 = posInWire + 0.01f * param_2;
	}

	JGeometry::TVec3<f32> vec1;
	JGeometry::TVec3<f32> vec2;
	getPoint(&vec1, fVar1);
	getPoint(&vec2, fVar2);

	vec2 -= vec1;

	return vec2;
}

void TWireBinder::getPoint(JGeometry::TVec3<f32>* param_1, f32 param_2) const
{
	getWire()->getPointPosOnWire(param_2, param_1);
}

void TWireBinder::getPoint(JGeometry::TVec3<f32>* param_1,
                           const JGeometry::TVec3<f32>& param_2) const
{
	getPoint(param_1, getRangePos(param_2));
}

bool TWireBinder::isOnWire(const JGeometry::TVec3<f32>& param_1)
{
	return gpMapWireManager->getWireNo(param_1) != -1;
}

f32 TWireBinder::getRangePos(const JGeometry::TVec3<f32>& param_1) const
{
	return getWire()->getPosInWire(param_1);
}

TMapWire* TWireBinder::getWire() const
{
	return gpMapWireManager->getWire(mWireNumber);
}

void TWireBinder::isStartWire(const JGeometry::TVec3<f32>&, f32) const { }

bool TWireBinder::isEndWire(const JGeometry::TVec3<f32>& param_1,
                            f32 param_2) const
{
	f32 posInWire = getRangePos(param_1);
	f32 targetPos = 0.0f < param_2 ? 1.0f : 0.0f;

	return fabsf(posInWire - targetPos) < 0.015f;
}

void TWireBinder::getStartRangePos(f32) { }

void TWireBinder::getEndRangePos(f32) { }
