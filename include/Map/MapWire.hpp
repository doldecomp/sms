#ifndef MAP_MAP_WIRE_HPP
#define MAP_MAP_WIRE_HPP

#include <JSystem/JGeometry/JGVec2.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>

class J3DModel;
class TCubeGeneralInfo;

class TMapWirePoint {
public:
	TMapWirePoint();

	// fabricated
	void reset()
	{
		mPosition  = mDefaultPosition;
		mPosOnWire = mDefaultPosOnWire;
	}

public:
	/* 0x00 */ JGeometry::TVec3<f32> mPosition;
	/* 0x0C */ JGeometry::TVec3<f32> mDefaultPosition;
	/* 0x18 */ f32 mPosOnWire;
	/* 0x1C */ f32 mDefaultPosOnWire;
	/* 0x20 */ f32 mPosReturnRate;
};

class TMapWire {
public:
	enum { IDLE = 0, HANGING = 1, RELEASED = 2 };

	void drawLower() const;
	void drawUpper() const;
	f32 getPointPowerAtReleased(f32) const;
	void getPointPosAtReleased(f32, JGeometry::TVec3<f32>*) const;
	void updatePointAtReleased(int);
	void updateMovePointAtReleased();
	void initPointAtJustReleased(f32, TMapWirePoint*);
	void release();
	void getPointPosAtHanged(f32, JGeometry::TVec3<f32>*) const;
	void getPointInfoAtHanged(f32, TMapWirePoint*);
	void setFootPointsAtHanged(MtxPtr);
	void calcViewAndDBEntry();
	void move();
	f32 getPosInWire(const JGeometry::TVec3<f32>&) const;
	void getPointPosOnLine(f32, JGeometry::TVec3<f32>*) const;
	void getPointPosOnWire(f32, JGeometry::TVec3<f32>*) const;
	void getPointPosDefault(f32, JGeometry::TVec3<f32>*) const;
	void initTipPoints(const TCubeGeneralInfo*);
	void init(const TCubeGeneralInfo*);
	TMapWire();

	static f32 mMoveTimerSpeed;
	static f32 mDownRateMax;
	static f32 mEndRate;
	static f32 mStretchRate;
	static f32 mHeightRate;
	static f32 mReleaseHeight;
	static f32 mFootLength;
	static f32 mDrawWidth;
	static f32 mDrawHeight;

	// fabricated
	const JGeometry::TVec3<f32>& getStartPoint() const { return mStartPoint; }
	const JGeometry::TVec3<f32>& getEndPoint() const { return mEndPoint; }

	TMapWirePoint* getPoint(int i) { return &mMapWirePoints[i]; }

public:
	/* 0x00 */ JGeometry::TVec3<f32> mStartPoint;
	/* 0x0C */ JGeometry::TVec3<f32> mEndPoint;
	/* 0x18 */ JGeometry::TVec3<f32> mWireSpan;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ f32 mWireLength;
	/* 0x34 */ f32 mWireHAngle; // in degrees
	/* 0x38 */ f32 mWireSag;
	/* 0x3C */ J3DModel* mStartFittingModel;
	/* 0x40 */ J3DModel* mEndFittingModel;
	/* 0x44 */ u16 mNumActiveMapWirePoints;
	/* 0x46 */ u16 mNumMapWirePoints;
	/* 0x48 */ TMapWirePoint* mMapWirePoints;
	/* 0x4C */ f32 mHangPos;
	/* 0x50 */ JGeometry::TVec3<f32>
	    mHangOrBouncePoint; // Mario's location when hanging on the wire, or the
	                        // amount of "bounce" when released
	/* 0x5C */ f32 mMoveTimer; // Position in "bounce" cycle when released
	/* 0x60 */ f32 mBounceRemainingPower;
	/* 0x64 */ f32 mBounceAmplitude;
	/* 0x68 */ f32 mBounceDecayRate;
	/* 0x6C */ JGeometry::TVec2<f32> mDrawAxes;
	/* 0x74 */ f32 mHangReferencePos1;
	/* 0x78 */ f32 mHangReferencePos2;
	/* 0x7C */ u16 mState;
};

#endif
