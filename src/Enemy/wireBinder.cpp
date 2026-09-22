#include <Enemy/WireBinder.hpp>
#include <Map/MapWire.hpp>
#include <Map/MapWireManager.hpp>

// reset()'s two endpoint reads must be the raw members, not TMapWire's
// getStartPoint()/getEndPoint(): each accessor reserves a dead 12-byte slot at
// the bottom of init()'s inline-temporary pool, which was its whole 24-byte
// over-frame (0x68 against retail's 0x50).  Still rejected: spelling reset()'s
// body out here (84 instructions instead of 67, so init really does inline
// reset), routing the wire fetch through getWire() (70 instructions),
// setLength(v, 1.0f) for normalize(), the two-argument sub(), and initialised
// vector declarations.
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

	local24 = wire->mStartPoint;
	local30 = wire->mEndPoint;
	local30 -= local24;

	mDir.normalize(local30);
	return true;
}

// All 174 instructions and the 0x78 frame are exact; the one residue is the
// offset of the `unk_20 - actor->mPosition` temporary, 0x3c here against
// retail's 0x24.  That is the known `a = b - c` pool-ordering residue (see
// docs/catalog/frame-gaps.md, research batches 113/116/119): retail allocates
// the operator- by-value temporary after the three isnan() scratch words, our
// build hoists it above them, and every reachable spelling either moves
// nothing or costs instructions.  Measured here and rejected: a by-value
// destination helper (a reference-argument forwarder keeps 174 instructions
// but shifts the three isnan() words and the sub temporary together +0xc), a
// block-scoped or named copy plus sub()/-= (the copy is scalar-replaced,
// 91.8%), TVec3(unk_20) at the site, mLinearVelocity written
// directly, actor->getPosition() at either or both reads (+8/+16 frame),
// getVelocity(), the getPoint chain spelled out or its range position named
// (97.8%), and a dead non-trivial local in getRangePos/getPoint/getWire (the
// only UNUSED carrier, getRangePos, also feeds the byte-exact out-of-line
// isEndWire and getPoint(TVec3*, const TVec3&), so it breaks them).
//
// The rest of the frame was two anti-levers: actor->getPosition() at the two
// position reads (+16) and, once those were raw, TLiveActor's fabricated
// getNextFramePosition() wrapper.  Its body belongs to bind() -- `velocity` is
// bind()'s own third local, which is why it sits below unk_20 at 0x4c instead
// of in the temporary pool -- and no other TU calls it.
void TWireBinder::bind(TLiveActor* actor)
{
	JGeometry::TVec3<f32> unk_14;
	JGeometry::TVec3<f32> unk_20;
	JGeometry::TVec3<f32> velocity;

	unk_14 = actor->mPosition;
	unk_14.add(actor->mLinearVelocity);
	velocity = actor->mVelocity;
	unk_14.add(velocity);

	getPoint(&unk_20, unk_14);

	if (isnan(unk_20.x) || isnan(unk_20.y) || isnan(unk_20.z))
		unk_20.set(actor->mPosition);

	f32 fVar = 0.05f + unk_20.y;

	if (unk_14.y <= fVar) {
		actor->offLiveFlag(LIVE_FLAG_AIRBORNE);
	} else {
		actor->onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	actor->setLinearVelocity(unk_20 - actor->mPosition);
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
