#include <Enemy/WireBinder.hpp>
#include <Map/MapWire.hpp>
#include <Map/MapWireManager.hpp>

bool TWireBinder::init(const JGeometry::TVec3<f32>& wireNumber)
{
	return reset(wireNumber);
}

bool TWireBinder::reset(const JGeometry::TVec3<f32>& wireNumber)
{
	mWireNumber = gpMapWireManager->getWireNo(wireNumber);
	if (mWireNumber == -1)
		return false;

	TMapWire* wire = gpMapWireManager->getWire(mWireNumber);

	JGeometry::TVec3<f32> wireStart = wire->getStartPoint();
	JGeometry::TVec3<f32> wireEnd   = wire->getEndPoint();
	wireEnd -= wireStart;

	mDir.normalize(wireEnd);
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

	actor->setPositionDelta(unk_20 - actor->getPosition());
}

JGeometry::TVec3<f32> TWireBinder::getDirAtPos(const JGeometry::TVec3<f32>& pos,
                                               f32 param_2) const
{
	f32 posInWire = getRangePos(pos);

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

void TWireBinder::getPoint(JGeometry::TVec3<f32>* out, f32 pos) const
{
	getWire()->getPointPosOnWire(pos, out);
}

void TWireBinder::getPoint(JGeometry::TVec3<f32>* out,
                           const JGeometry::TVec3<f32>& point) const
{
	getPoint(out, getRangePos(point));
}

bool TWireBinder::isOnWire(const JGeometry::TVec3<f32>& wireNumber)
{
	return gpMapWireManager->getWireNo(wireNumber) != -1;
}

f32 TWireBinder::getRangePos(const JGeometry::TVec3<f32>& point) const
{
	return getWire()->getPosInWire(point);
}

TMapWire* TWireBinder::getWire() const
{
	return gpMapWireManager->getWire(mWireNumber);
}

bool TWireBinder::isStartWire(const JGeometry::TVec3<f32>&, f32) const { }

bool TWireBinder::isEndWire(const JGeometry::TVec3<f32>& point,
                            f32 direction) const
{
	f32 posInWire = getRangePos(point);
	return fabsf(posInWire - getEndRangePos(direction)) < 0.015f;
}

void TWireBinder::getStartRangePos(f32 direction) { }

f32 TWireBinder::getEndRangePos(f32 direction)
{
	return 0.0f < direction ? 1.0f : 0.0f;
}
