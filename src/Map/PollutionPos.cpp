#include <Map/PollutionPos.hpp>
#include <Map/PollutionLayer.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// `index()` is two inline levels short here: retail's loop ends the address
// with `add r0, r11, r0` ((z & 3) * 8, hoisted out of the loop, added last) and
// uses `mHeightMap` as the `lbzx` base, while a direct `index()` call folds
// `mHeightMap` into the offset and makes the hoisted term the base. One level
// that binds its result restores retail's whole register assignment (85/85
// instructions) and is +8 of frame; a second such level is the remaining +16,
// giving retail's 0x38 exactly. Two bindings inside one level are only +16, so
// it is the expansion count that pays, not the bindings.
// These two stand in for named intermediates inside `TPollutionPos::index()`
// itself (the block index and the final index), but `Map/PollutionPos.hpp` is
// included by the source-linked Pollution TUs -- reordering `index()`'s terms
// there fails the DOL SHA-1 check -- so they are parked here and reported.
static inline u32 PollutionPosIndexInner(const TPollutionPos* pos, int x, int z)
{
	u32 inner = pos->index(x, z);
	return inner;
}

static inline u32 PollutionPosIndex(const TPollutionPos* pos, int x, int z)
{
	u32 idx = PollutionPosIndexInner(pos, x, z);
	return idx;
}

static inline bool PollutionPosIsInArea(const TPollutionPos* pos, int x, int z)
{
	bool inArea = pos->isInArea(x, z);
	return inArea;
}

static inline int PollutionPosGetDepth(const TPollutionPos* pos, int x, int z)
{
	int depth = pos->getDepth(x, z);
	return depth;
}

static inline int PollutionPosWorldToDepth(const TPollutionPos* pos, f32 y)
{
	int depth = pos->worldToDepth(y);
	return depth;
}

int TPollutionPos::getEdgeDegree(int x, int y) const
{
	if (!isInArea(x, y))
		return 0;

	int count = 0;
	for (int dy = -1; dy <= 1; ++dy) {
		for (int dx = -1; dx <= 1; ++dx) {
			if (dx != 0 || dy != 0) {
				if (mHeightMap[PollutionPosIndex(this, x + dx, y + dy)] == 0xFF)
					count += 1;
			}
		}
	}
	return count;
}

f32 TPollutionPos::getDepthWorld(int x, int y) const
{
	if (getDepth(x, y) < 0xff) {
		return depthToWorld(getDepth(x, y));
	} else {
		return -9999.0f;
	}
}

// The 32 bytes that batch 69 could not place are three inline levels, one per
// member this function reads through: `isInArea`, `getDepth` and
// `worldToDepth` each sit one expansion deeper than a direct call, and each
// binds its result. Individually `isInArea` and `getDepth` are worth zero and
// `worldToDepth` +8; `isInArea` + `getDepth` together are +16 (lever pair) and
// all three are +32, which is 0x58 exactly at 53 instructions.
// The levels belong on the members themselves -- a body of the
// `int depth = mHeightMap[index(x, z)]; return depth;` shape in
// `Map/PollutionPos.hpp` says the same thing -- but that header is included by
// the source-linked Pollution TUs and any change to `index()`/`isInArea()`
// there breaks the DOL (measured: reordering `index()`'s terms fails the
// SHA-1 check), so they are parked here under TU-prefixed names and reported.
// Superseded trials, all at frame 0x38: one accessor level anywhere on
// TPollutionPos +8 and saturating, a `getOwner()` level on TPollutionLayer +8
// on top of it, a nested `blockIndex()` level inside `index()` +8 more (0x48
// ceiling); a dead 28-32 byte non-trivial local in `worldToDepth` lands 0x58
// but grows `worldToDepth`'s own emitted 100% copy, so it is not a legal
// carrier.
bool TPollutionPos::isSame(int x, int z, f32 y) const
{
	if (!PollutionPosIsInArea(this, x, z))
		return false;

	int d = PollutionPosGetDepth(this, x, z);
	if (d < 0xff) {
		s32 iVar1 = PollutionPosWorldToDepth(this, y);
		int uVar4 = mOwner->getUnk48();
		if (d - uVar4 <= iVar1 && iVar1 <= d + uVar4)
			return true;
	}
	return false;
}

void TPollutionPos::subtractFromYMap(int x, int z, f32 y) const { }

bool TPollutionPos::isProhibit(int x, int z) const
{
	if (x < 0 || mWidth <= x || z < 0 || mHeight <= z) {
		return 1;
	} else {
		if (getDepth(x, z) < 0xff) {
			return 0;
		} else {
			return 1;
		}
	}
}

int TPollutionPos::worldToDepth(f32 v) const
{
	v -= mVerticalOffset;
	v *= 0.025f;
	return v;
}

int TPollutionPos::worldToTexSize(f32 v) const
{
	v *= mInverseTexelScale;
	return v;
}

void TPollutionPos::init(TPollutionLayer* owner, f32 vertical_offset,
                         f32 texel_size, u8* height_map, int log2_width,
                         int log2_height)
{
	mOwner             = owner;
	mHeightMap         = height_map;
	mVerticalOffset    = vertical_offset;
	mTexelSize         = texel_size;
	mInverseTexelScale = 1.0f / mTexelSize;
	mLog2Width         = log2_width;
	mLog2Height        = log2_height;
	mWidth             = 1 << mLog2Width;
	mHeight            = 1 << mLog2Height;
}

TPollutionPos::TPollutionPos()
{
	mWidth             = 0;
	mHeight            = 0;
	mLog2Width         = 0;
	mLog2Height        = 0;
	mVerticalOffset    = 0.0f;
	mTexelSize         = 0.0f;
	mInverseTexelScale = 0.0f;
	mHeightMap         = nullptr;
	mOwner             = nullptr;
}
