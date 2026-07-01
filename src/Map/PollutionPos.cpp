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
		f32 d = getDepth(x, y) * mTexelSize;
		return mVerticalOffset + d;
	} else {
		return -9999.0f;
	}
}

bool TPollutionPos::isSame(int x, int y, f32 param_3) const
{
	if (!isInArea(x, y))
		return false;

	int d = getDepth(x, y);
	if (d < 0xff) {
		int uVar4 = mOwner->unk48;
		int iVar1 = worldToDepth(param_3);
		if (d - uVar4 <= iVar1 && iVar1 <= d + uVar4)
			return true;
	}
	return false;
}

void TPollutionPos::subtractFromYMap(int x, int y, f32) const { }

bool TPollutionPos::isProhibit(int x, int y) const
{
	if (x < 0 || mWidth <= x || y < 0 || mHeight <= y) {
		return 1;
	} else {
		if (getDepth(x, y) < 0xff) {
			return 0;
		} else {
			return 1;
		}
	}
}

int TPollutionPos::worldToDepth(f32 v) const
{
	return (v - mVerticalOffset) * 0.025f;
}

int TPollutionPos::worldToTexSize(f32 v) const
{
	return v * mInverseTexelScale;
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
