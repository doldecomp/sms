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
	JGeometry::TVec3<f32> unk_14 = actor->mPosition;
	JGeometry::TVec3<f32> unk_20;

	unk_14.x += actor->mLinearVelocity.x;
	unk_14.y += actor->mLinearVelocity.y;
	unk_14.z += actor->mLinearVelocity.z;

	JGeometry::TVec3<f32> velocity = actor->mVelocity;

	unk_14.x += velocity.x;
	unk_14.y += velocity.y;
	unk_14.z += velocity.z;

	TMapWire* wire = gpMapWireManager->getWire(mWireNumber);
	f32 posInWire  = wire->getPosInWire(unk_14);
	gpMapWireManager->unk18[mWireNumber]->getPointPosOnWire(posInWire, &unk_20);

	// TODO: Investigate this further
	// The stack addresses used for the isnan checks are off by 0x10,
	// regardless of whether this is before or after the isnan checks.
	// However, this padding does align everything else above here.
	u8 padding[0x10]; // to match allocated stack size

	if (isnan(unk_20.x) || isnan(unk_20.y) || isnan(unk_20.z)) {
		unk_20.set(actor->mPosition);
	}

	f32 fVar = 0.05f + unk_20.y;

	if (unk_14.y <= fVar) {
		actor->mLiveFlag &= ~LIVE_FLAG_AIRBORNE;
	} else {
		actor->mLiveFlag |= LIVE_FLAG_AIRBORNE;
	}

	actor->mLinearVelocity = unk_20 - actor->mPosition;
}

JGeometry::TVec3<f32>
TWireBinder::getDirAtPos(const JGeometry::TVec3<f32>& param_1,
                         f32 param_2) const
{
	JGeometry::TVec3<f32> vec1;
	JGeometry::TVec3<f32> vec2;

	TMapWire* wire = gpMapWireManager->unk18[mWireNumber];
	f32 posInWire  = wire->getPosInWire(param_1);

	u8 padding[0x30]; // to match allocated stack size

	f32 dVar1;
	f32 dVar2;

	if (posInWire <= 0.01f && param_2 < 0.0f
	    || 0.99f <= posInWire && 0.0f < param_2) {
		dVar1 = posInWire - 0.01f * param_2;
		dVar2 = posInWire;
	} else {
		dVar1 = posInWire;
		dVar2 = posInWire + 0.01f * param_2;
	}

	gpMapWireManager->unk18[mWireNumber]->getPointPosOnWire(dVar1, &vec1);
	gpMapWireManager->unk18[mWireNumber]->getPointPosOnWire(dVar2, &vec2);

	vec2.sub(vec1);
	return vec2;
}

void TWireBinder::getPoint(JGeometry::TVec3<f32>* param_1, f32 param_2) const
{
	gpMapWireManager->unk18[mWireNumber]->getPointPosOnWire(param_2, param_1);
}

void TWireBinder::getPoint(JGeometry::TVec3<float>* param_1,
                           const JGeometry::TVec3<float>& param_2) const
{
	u8 padding[0x20]; // to match allocated stack size

	f32 posInWire = gpMapWireManager->unk18[mWireNumber]->getPosInWire(param_2);
	gpMapWireManager->unk18[mWireNumber]->getPointPosOnWire(posInWire, param_1);
}

bool TWireBinder::isEndWire(const JGeometry::TVec3<float>& param_1,
                            f32 param_2) const
{
	u8 padding[0x18]; // to match allocated stack size

	f32 posInWire = gpMapWireManager->unk18[mWireNumber]->getPosInWire(param_1);
	f32 targetPos = 0 < param_2 ? 1.0f : 0.0f;

	return fabsf(posInWire - targetPos) < 0.015f;
}

bool TWireBinder::isOnWire(const JGeometry::TVec3<f32>& param_1)
{
	return gpMapWireManager->getWireNo(param_1) != -1;
}

TWireBinder::~TWireBinder() { }
