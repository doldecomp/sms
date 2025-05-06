#ifndef ENEMY_SPLINE_HPP
#define ENEMY_SPLINE_HPP

#include <JSystem/JGeometry/JGVec3.hpp>
#include <dolphin/types.h>
#include <dolphin/mtx.h>

class TSplinePath {
public:
	TSplinePath(int);
	~TSplinePath();
	void makeTable(f32*, f32*, f32*);
	void calcTable();
	f32 calcSpline(f32, f32*, f32*, f32*);
	JGeometry::TVec3<f32> getPoint(f32 t)
	{
		if (mInitialized == 0)
			calcTable();

		if (t < 0.0f)
			t = 0.0f;
		if (1.0f < t)
			t = 1.0f;

		return JGeometry::TVec3<f32>(
		    calcSpline(t, mParametrization, mXs, unk10),
		    calcSpline(t, mParametrization, mYs, unk14),
		    calcSpline(t, mParametrization, mZs, unk18));
	}

	// fabricated
	void setPoint(int i, Vec v)
	{
		if (i < 0 || mPointNum <= i)
			return;

		mXs[i] = v.x;
		mYs[i] = v.y;
		mZs[i] = v.z;

		mInitialized = false;
	}

public:
	/* 0x0 */ int mPointNum;
	/* 0x4 */ f32* mXs;
	/* 0x8 */ f32* mYs;
	/* 0xC */ f32* mZs;
	/* 0x10 */ f32* unk10;
	/* 0x14 */ f32* unk14;
	/* 0x18 */ f32* unk18;
	/* 0x1C */ f32* mParametrization;
	/* 0x20 */ f32* mTmpParamDeltas;
	/* 0x24 */ f32* mTmp;
	/* 0x28 */ BOOL mInitialized;
};

#endif
