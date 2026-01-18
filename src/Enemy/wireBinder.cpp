#include <Enemy/WireBinder.hpp>
#include <Map/MapWire.hpp>
#include <Map/MapWireManager.hpp>

bool TWireBinder::init(const JGeometry::TVec3<f32>& param_1)
{
	JGeometry::TVec3<f32> local24;
	JGeometry::TVec3<f32> local30;

	mWireNumber = gpMapWireManager->getWireNo(param_1);
	if ((s32)mWireNumber == -1) {
		return false;
	}

	TMapWire* wire = gpMapWireManager->unk18[mWireNumber];

	local24 = wire->unk00;
	local30 = wire->unk0C;
	local30.sub(local24);

	// TODO: Why does the compiler inline stuff here but not in the target?
	// See also: TMapObjPlane::calcNrm, TBGAttackHit::perform, etc.
	mDir.setLength(local30, 1.0f);
	return true;
}

void TWireBinder::bind(TLiveActor* actor)
{
	JGeometry::TVec3<f32> unk_14;
	actor->getNextFramePosition(unk_14);
	
	JGeometry::TVec3<f32> unk_20;
	f32 posInWire = gpMapWireManager->getPosInWire(mWireNumber, unk_14);
	gpMapWireManager->getPointPosOnWire(mWireNumber, posInWire, unk_20);

	if (isnan(unk_20.x) || isnan(unk_20.y) || isnan(unk_20.z)) {
		unk_20.set(actor->getPosition());
	}

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
	JGeometry::TVec3<f32> vec1;
	JGeometry::TVec3<f32> vec2;

	f32 posInWire = gpMapWireManager->getPosInWire(mWireNumber, param_1);

	// This would make stack addresses align, but it's too much of a hack
	// u8 padding[0x28];

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

	gpMapWireManager->getPointPosOnWire(mWireNumber, fVar1, vec1);
	gpMapWireManager->getPointPosOnWire(mWireNumber, fVar2, vec2);

	vec2.sub(vec1);
	return vec2;
}

void TWireBinder::getPoint(JGeometry::TVec3<f32>* param_1, f32 param_2) const
{
	gpMapWireManager->getPointPosOnWire(mWireNumber, param_2, *param_1);
}

void TWireBinder::getPoint(JGeometry::TVec3<float>* param_1,
                           const JGeometry::TVec3<float>& param_2) const
{
	f32 posInWire = gpMapWireManager->getPosInWire(mWireNumber, param_2);
	getPoint(param_1, posInWire);
}

bool TWireBinder::isEndWire(const JGeometry::TVec3<float>& param_1,
                            f32 param_2) const
{
	// This would make stack addresses align, but it's too much of a hack
	// u8 padding[0x18];

	f32 posInWire = gpMapWireManager->getPosInWire(mWireNumber, param_1);
	f32 targetPos = 0.0f < param_2 ? 1.0f : 0.0f;

	return fabsf(posInWire - targetPos) < 0.015f;
}

bool TWireBinder::isOnWire(const JGeometry::TVec3<f32>& param_1)
{
	return gpMapWireManager->getWireNo(param_1) != -1;
}

TWireBinder::~TWireBinder() { }
