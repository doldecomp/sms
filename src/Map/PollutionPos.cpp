#include <Map/PollutionPos.hpp>
#include <Map/PollutionLayer.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

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
