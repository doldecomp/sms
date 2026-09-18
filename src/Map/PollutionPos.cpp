#include <Map/PollutionPos.hpp>
#include <Map/PollutionLayer.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: frame 0x20 vs retail 0x38 (24 bytes of inline temporaries short) plus a
// register rotation that survives the correct frame: retail puts mLog2Width in
// r7, x >> 3 in r8, mHeightMap in r6 and x & 7 in r9, ends the index with
// `add r0, r11, r0` ((z & 3) * 8 added last) and uses mHeightMap as the `lbzx`
// base, while we assign r6/r7/r9/r8 in computation order, fold mHeightMap into
// the offset and make the hoisted (z & 3) * 8 the base. (isSame, which shares
// index(), gets retail's operand order already, so it is the loop hoisting that
// flips it.) Measured as +8 on the frame and zero on the registers: a
// getHeightMap()/getLog2Width() accessor, getWidth()/getHeight() inside
// isInArea, a getVerticalOffset() accessor; TPollutionPos accessors saturate at
// one level (+8 total) and only 0x30 is reachable. Rejected: a nested
// blockIndex() level in index() (+8 but -16% here), a named u32 for the index,
// a local u8* for the height map, `!(dx == 0 && dy == 0)`, `dy != 0` first,
// `*(mHeightMap + index(...))`, getDepth() instead of the raw byte read (that
// turns retail's `cmplwi` into `cmpwi`), and putting (z & 3) * 8 last in
// index() (-7%).
int TPollutionPos::getEdgeDegree(int x, int y) const
{
	if (!isInArea(x, y))
		return 0;

	int count = 0;
	for (int dy = -1; dy <= 1; ++dy) {
		for (int dx = -1; dx <= 1; ++dx) {
			if (dx != 0 || dy != 0) {
				if (mHeightMap[index(x + dx, y + dy)] == 0xFF)
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

// TODO: frame 0x38 vs retail 0x58; all 53 instructions match (32 bytes of
// temporary padding gives 100%), so retail has 32 bytes more of inline
// temporaries in this leaf. Ladder measured here: one accessor level anywhere on
// TPollutionPos +8 and saturating (getHeightMap, getLog2Width, getWidth/
// getHeight in isInArea, getVerticalOffset in worldToDepth all give the same
// +8), a getOwner() level on TPollutionLayer +8 on top of it, a nested
// blockIndex() level inside index() +8 more; that reaches 0x48 and nothing
// reaches 0x58. getDepthWorld and isProhibit, which share isInArea, getDepth
// and index(), already match, so the level structure of those is right and the
// missing 32 bytes belong to isSame itself.
bool TPollutionPos::isSame(int x, int z, f32 y) const
{
	if (!isInArea(x, z))
		return false;

	int d = getDepth(x, z);
	if (d < 0xff) {
		s32 iVar1 = worldToDepth(y);
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
