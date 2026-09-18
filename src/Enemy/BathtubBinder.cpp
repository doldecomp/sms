#include <Enemy/BathtubBinder.hpp>
#include <Strategic/LiveActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

#include <M3DUtil/InfectiousStrings.hpp> // rogue include needed for matching rodata

// Radius of the tub's water circle. Split over two levels because retail
// reaches JGeometry::TUtil<f32>::sqrt at depth 4 from float_ and therefore
// calls it out of line, where one level still expands its frsqrte sequence.
// TODO: these two probably belong on TBathtubData and TBathtub
// (MapObjCorona.hpp); parked as TU-local helpers because shared headers are
// off limits in this batch.
static inline f32 BathtubInnerRadius(const TBathtubData& data)
{
	return JGeometry::TUtil<f32>::sqrt(data.unk3C * data.unk3C
	                                   - data.unk44 * data.unk44);
}

static inline f32 BathtubRadius(const TBathtubData& data)
{
	return BathtubInnerRadius(data);
}

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

// TODO: 96.2% of 1,148 bytes. The frame total (0x178) and every structural
// element now match; what is left is a six-instruction scheduling difference
// in the height blend and a few local slots that sit 4-12 bytes high (rot at
// 0xd8 against 0xdc, the three constrain_ getThing() temporaries at
// 0xcc/0xac/0x90 against 0xc4/0xa8/0x84).
//
// Closed in closure batch 71:
//   * The helper is the map's UNUSED `constrain_(JGeometry::TVec3<f>&, f32)`
//     (0x164), not a pointer-pair clamp. Taking the point by reference is what
//     lets the third site share one tub centre between the circle clamp and
//     the floor clamp on y (retail reads centre.y out of the slot the clamp
//     built); at the first two sites the y clamp is dead-code-eliminated
//     because nothing reads the sampled point's y. Our body is byte-size-exact
//     at 0x164, and it must be a *plain* method so the standalone (dead-
//     stripped) copy is emitted: marking it `inline` inlines it just as well
//     but drops the symbol the map lists.
//   * It is 14 statements exactly, which is the depth-1 inlining budget; the
//     15-statement spelling with `pos.x = ...; pos.z = ...;` is *called*
//     instead of inlined (float_ 37%). `pos.set(x, pos.y, z)` pays that one
//     statement with no codegen change and keeps the 0x164 size, while
//     dropping the named `data` reference instead costs a re-fetch and 0x13c.
//   * TUtil<f32>::sqrt is *called* by retail and was inlined by us. Not a
//     whole-tree problem: sqrt is a three-statement body, refused only at
//     depth 4 (allowance 2), and retail reaches it two levels under
//     constrain_. BathtubRadius/BathtubInnerRadius add exactly those levels
//     and turn all three sites into `bl`. inv_sqrt stays inlined at depth 2 in
//     the same function, which is what rules out a declaration-level
//     explanation (and retires the old note here that claimed JGUtil.hpp was
//     the blocker).
//   * The null check lives *inside* constrain_, not around its call sites:
//     retail loads the margin argument before the `beq` that skips the clamp.
//   * The epsilon guard is an early `return` on `<=` (retail's `cror eq,lt,eq`
//     + `beq`); `if (... > eps) { ... }` gives a bare `ble`.
//   * The pitch clamp assigns into a *separate* variable, so each arm writes
//     the limit register and only the fallthrough moves the raw value.
//   * `f32 cz = center.z` is load-bearing (+2 points): retail keeps centre.z
//     in an FPR across the radius test and reloads only centre.x.
//   * Frame: `front`/`back`/`dir` are TVec3 locals declared *before* `rot`
//     (retail's named region has 44 bytes above the matrix: three 12-byte
//     vectors plus the int-to-float conversion pair), and getFrontMargin() is
//     the last 8 bytes. Measured alternatives: getBackMargin() alone is the
//     same 8; both together also 8 (saturating); getBathtub() +24;
//     getHeightOffset() +8 but costs 7 instructions; getWater() 0 and costs
//     one; getBackRatio() and getFrontDist() 0.
//
// Still open: retail puts dirZ in f31 and dirX in f30, we do the opposite
// whichever order they are declared in; and retail interleaves the
// `mPosition.y` update with the squared-length sum, loading `mPosition.y`
// after `dy`. Ruled out for the latter: naming the blend target, moving
// `f32 y` above `dy`, the `+=` spelling, reordering the epsilon sum,
// reordering dx/dz and the two clamp stores.
void TBathtubBinder::float_(TLiveActor* actor)
{
	if (mWater == nullptr)
		return;

	JGeometry::TVec3<f32> front;
	JGeometry::TVec3<f32> back;
	JGeometry::TVec3<f32> dir;
	Mtx rot;
	MsMtxSetRotRPH(rot, actor->mRotation.x, actor->mRotation.y,
	               actor->mRotation.z);

	dir.x = rot[0][2];
	dir.z = rot[2][2];

	front.x = dir.x * mFrontDist + actor->getPosition().x;
	front.z = dir.z * mFrontDist + actor->mPosition.z;
	constrain_(front, getFrontMargin());
	f32 frontY = mHeightOffset + mWater->getWaterHeight(front.x, front.z);

	back.x = dir.x * -mBackDist + actor->mPosition.x;
	back.z = dir.z * -mBackDist + actor->mPosition.z;
	constrain_(back, mBackMargin);
	f32 backY = mHeightOffset + mWater->getWaterHeight(back.x, back.z);

	f32 dx = front.x - back.x;
	f32 dz = front.z - back.z;
	f32 dy = frontY - backY;

	f32 y        = actor->mPosition.y;
	actor->mPosition.y = 0.2f * ((mBackRatio * dy + backY) - y) + y;

	if (dx * dx + dy * dy + dz * dz <= 0.0000038146973f)
		return;
	{
		f32 flat     = JGeometry::TUtil<f32>::sqrt(dx * dx + dz * dz);
		f32 rawPitch = matan(flat, dy) * (360.0f / 65536.0f);
		f32 pitch;
		if (rawPitch < -15.0f)
			pitch = -15.0f;
		else if (rawPitch > 15.0f)
			pitch = 15.0f;
		else
			pitch = rawPitch;

		f32 rx = actor->mRotation.x;
		actor->mRotation.x = 0.1f * (pitch - rx) + rx;
		actor->mRotation.z = 0.0f;

		f32 margin = 0.5f * (mFrontMargin + mBackMargin);
		constrain_(actor->mPosition, margin);
	}
}

void TBathtubBinder::constrain_(JGeometry::TVec3<f32>& pos, f32 margin)
{
	if (mBathtub == nullptr)
		return;

	const TBathtubData& data     = mBathtub->getBathtubData();
	JGeometry::TVec3<f32> center = data.getThing();
	f32 radius                   = BathtubRadius(data) - margin;
	f32 cz                       = center.z;
	f32 dz                       = pos.z - cz;
	f32 dx                       = pos.x - center.x;
	f32 lsq                      = dx * dx + dz * dz;
	if (lsq > radius * radius) {
		f32 scale = radius * JGeometry::TUtil<f32>::inv_sqrt(lsq);
		pos.set(scale * dx + center.x, pos.y, scale * dz + cz);
	}
	if (pos.y < center.y)
		pos.y = center.y;
}
