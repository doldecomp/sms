#include <Enemy/Spline.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <math.h>

TSplinePath::TSplinePath(int param_1)
    : mPointNum(param_1)
{
	mXs              = new f32[mPointNum];
	mYs              = new f32[mPointNum];
	mZs              = new f32[mPointNum];
	unk10            = new f32[mPointNum];
	unk14            = new f32[mPointNum];
	unk18            = new f32[mPointNum];
	mParametrization = new f32[mPointNum];
	mTmpParamDeltas  = new f32[mPointNum];
	mTmp             = new f32[mPointNum];
	mInitialized     = FALSE;

	for (int i = 0; i < mPointNum; ++i)
		mXs[i] = mYs[i] = mZs[i] = i;
}

void TSplinePath::makeTable(f32* parametrization, f32* coords, f32* param_3)
{
	param_3[0]             = 0.0f;
	param_3[mPointNum - 1] = 0.0f;
	for (int i = 0; i < mPointNum - 1; ++i) {
		mTmpParamDeltas[i] = parametrization[i + 1] - parametrization[i];
		if (std::fabsf(mTmpParamDeltas[i]) < 0.0001f) {
			mTmp[i + 1] = 0.0f;
		} else {
			mTmp[i + 1] = (coords[i + 1] - coords[i]) / mTmpParamDeltas[i];
		}
	}

	param_3[1] = -(mTmpParamDeltas[0] * param_3[0] - (mTmp[2] - mTmp[1]));
	mTmp[1]    = (parametrization[2] - parametrization[0]) * 2.0f;
	for (int i = 1; i < mPointNum - 2; ++i) {
		if (std::fabsf(mTmp[i]) < 0.0001f)
			mTmp[i] = 0.0001f;

		f32 fVar1 = mTmpParamDeltas[i] / mTmp[i];

		param_3[i + 1] = -(fVar1 * param_3[i] - (mTmp[i + 2] - mTmp[i + 1]));
		mTmp[i + 1]    = (parametrization[i + 2] - parametrization[i]) * 2.0f
		              - fVar1 * mTmpParamDeltas[i];
	}

	param_3[mPointNum - 2]
	    -= mTmpParamDeltas[mPointNum - 2] * param_3[mPointNum - 1];

	for (int i = mPointNum - 2; i > 0; --i) {
		param_3[i]
		    = (param_3[i] - mTmpParamDeltas[i] * param_3[i + 1]) / mTmp[i];
	}
}

void TSplinePath::calcTable()
{
	if (mPointNum <= 2) {
		mInitialized = TRUE;
		return;
	}

	mParametrization[0] = 0.0f;
	for (int i = 1; i < mPointNum; ++i) {
		JGeometry::TVec3<f32> v;
		v.set(mXs[i] - mXs[i - 1], mYs[i] - mYs[i - 1], mZs[i] - mZs[i - 1]);

		f32 next = VECMag(&v) + mParametrization[i - 1];

		mParametrization[i] = next;
	}

	for (int i = 1; i < mPointNum; ++i) {
		mParametrization[i] /= mParametrization[mPointNum - 1];
	}

	makeTable(mParametrization, mXs, unk10);
	makeTable(mParametrization, mYs, unk14);
	makeTable(mParametrization, mZs, unk18);
	mInitialized = TRUE;
}

f32 TSplinePath::calcSpline(f32 t, f32* parametrization, f32* coords,
                            f32* param_4)
{
	if (mPointNum == 1)
		return coords[0];

	if (mPointNum == 2)
		return coords[0] * (1.0f - t) + coords[1] * t;

	// Binary search to find the segment where t belongs :O
	int l = 0;
	int r = mPointNum - 1;
	while (l < r) {
		u32 mid = (l + r) / 2;
		if (parametrization[mid] < t)
			l = mid + 1;
		else
			r = mid;
	}

	if (l > 0)
		--l;

	f32 dt = t - parametrization[l];
	f32 dp = parametrization[l + 1] - parametrization[l];

	// TODO: these are probably inlines
	f32 a = param_4[l] * 3.0f + (dt * (param_4[l + 1] - param_4[l])) / dp;
	f32 d = (coords[l + 1] - coords[l]) / dp
	        - dp * (param_4[l] * 2.0f + param_4[l + 1]);

	f32 c = dt * a + d;
	return dt * c + coords[l];
}

// NOTE: order of floats in sdata2 is wrong because of lack of inlines
static void dummy(f32* f)
{
	*f = 1.0f;
	*f = 3.0f;
}
