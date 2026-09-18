#include <Enemy/WireBinder.hpp>
#include <Map/MapWire.hpp>
#include <Map/MapWireManager.hpp>

// TODO: instruction-exact, frame 0x68 vs retail's 0x50; reset()'s two vectors
// land at 0x3c/0x48 where retail has 0x24/0x30, i.e. +24 bytes of low region,
// exactly as in bind().  Rejected: spelling reset()'s body out here (84
// instructions instead of 67, so init really does inline reset), routing the
// wire fetch through getWire() (70 instructions), and initialised vector
// declarations (no change).
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

// TODO: instruction-exact, frame 0x88 vs retail's 0x78.  The named locals and
// every inline temporary sit at the right *relative* offsets; the whole block is
// 24 bytes too high because our low (inline-temporary) region is 48 bytes where
// retail's is 24 -- the same +24 low region as init() below, and the two
// functions' only shared trait is that they inline a callee which owns
// JGeometry::TVec3<f32> locals of its own (getNextFramePosition's `velocity`
// here, reset()'s two vectors there).  Rejected: dropping the named `f32 fVar`
// (+0), spelling the last line as a named vector plus `-=` (186 instructions),
// initialised instead of assigned vector declarations (no change).
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

	f32 toPos;

	// Sample two points 0.01 of the wire's range apart, stepping backwards
	// instead of forwards when the requested direction would run off the end.
	if (posInWire <= 0.01f && param_2 < 0.0f
	    || 0.99f <= posInWire && 0.0f < param_2) {
		toPos     = posInWire;
		posInWire = posInWire - 0.01f * param_2;
	} else {
		toPos = posInWire + 0.01f * param_2;
	}

	JGeometry::TVec3<f32> vec1;
	JGeometry::TVec3<f32> vec2;
	getPoint(&vec1, posInWire);
	getPoint(&vec2, toPos);

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

// UNUSED, 0x6c in the map -- four bytes more than isEndWire, and it is called
// out of line from TWireTrap::isStartWire, so it really returns an answer.
// TODO: the body is a guess mirroring isEndWire's; only the target position
// flips.
bool TWireBinder::isStartWire(const JGeometry::TVec3<f32>& param_1,
                              f32 param_2) const
{
	f32 posInWire = getRangePos(param_1);
	f32 targetPos = getStartRangePos(param_2);
	f32 diff      = posInWire - targetPos;

	return fabsf(diff) < 0.015f;
}

bool TWireBinder::isEndWire(const JGeometry::TVec3<f32>& param_1,
                            f32 param_2) const
{
	f32 posInWire = getRangePos(param_1);
	f32 targetPos = getEndRangePos(param_2);
	f32 diff      = posInWire - targetPos;

	return fabsf(diff) < 0.015f;
}

f32 TWireBinder::getStartRangePos(f32 param_1)
{
	return 0.0f < param_1 ? 0.0f : 1.0f;
}

f32 TWireBinder::getEndRangePos(f32 param_1)
{
	return 0.0f < param_1 ? 1.0f : 0.0f;
}
