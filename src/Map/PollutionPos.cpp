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
		int yy = y + dy;
		for (int dx = -1; dx <= 1; ++dx) {
			if (dx != 0 || dy != 0) {
				if (mMap[index(x + dx, yy)] == 0xFF)
					count += 1;
			}
		}
	}
	return count;
}

f32 TPollutionPos::getDepthWorld(int x, int y) const
{
	if (getDepth(x, y) < 0xff) {
		f32 d = getDepth(x, y) * mVerticalScale;
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
	return v * mInverseVerticalScale;
}

void TPollutionPos::init(TPollutionLayer* param_1, f32 param_2, f32 param_3,
                         u8* param_4, int param_5, int param_6)
{
	mOwner                = param_1;
	mMap                  = param_4;
	mVerticalOffset       = param_2;
	mVerticalScale        = param_3;
	mInverseVerticalScale = 1.0f / mVerticalScale;
	unk8                  = param_5;
	unkC                  = param_6;
	mWidth                = 1 << unk8;
	mHeight               = 1 << unkC;
}

TPollutionPos::TPollutionPos()
{
	mWidth                = 0;
	mHeight               = 0;
	unk8                  = 0;
	unkC                  = 0;
	mVerticalOffset       = 0.0f;
	mVerticalScale        = 0.0f;
	mInverseVerticalScale = 0.0f;
	mMap                  = nullptr;
	mOwner                = nullptr;
}
