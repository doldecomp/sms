#ifndef CAMERA_CAMERA_SHAKE_HPP
#define CAMERA_CAMERA_SHAKE_HPP

#include <JSystem/JGeometry.hpp>

class TCamSaveShake;

enum EnumCamShakeMode {
	CAM_SHAKE_MODE_UNK2  = 0x2,
	CAM_SHAKE_MODE_UNK3  = 0x3,
	CAM_SHAKE_MODE_UNK4  = 0x4,
	CAM_SHAKE_MODE_UNK5  = 0x5,
	CAM_SHAKE_MODE_UNK6  = 0x6,
	CAM_SHAKE_MODE_UNK7  = 0x7,
	CAM_SHAKE_MODE_UNK12 = 0x12,
	CAM_SHAKE_MODE_UNK13 = 0x13,
	CAM_SHAKE_MODE_UNK14 = 0x14,
	CAM_SHAKE_MODE_UNK15 = 0x15,
};

class TCameraShake;

extern TCameraShake* gpCameraShake;

class TCameraShake {
public:
	struct TCamShakeAngle {
		/* 0x0 */ f32 mAmp;
		/* 0x4 */ f32 mDec;
		/* 0x8 */ s16 mVel;
	};

	struct TCamShakeInfo {

		bool isActive() const { return mDuration != 0 ? true : false; }

		void reset()
		{
			mMode         = 1;
			mIsKeep       = 0;
			mIsDecreasing = 0;
			mFrame        = 0;
			mDuration     = 0;
			mAngleX.mDec  = 0.0f;
			mAngleX.mAmp  = 0.0f;
			mAngleX.mVel  = 0;
			mAngleY.mDec  = 0.0f;
			mAngleY.mAmp  = 0.0f;
			mAngleY.mVel  = 0;
			mAngleZ.mDec  = 0.0f;
			mAngleZ.mAmp  = 0.0f;
			mAngleZ.mVel  = 0;
		}

		/* 0x00 */ int mMode;
		/* 0x04 */ u8 mIsKeep;
		/* 0x05 */ u8 mIsDecreasing;
		/* 0x06 */ u16 mDuration;
		/* 0x08 */ u16 mFrame;
		/* 0x0C */ TCamShakeAngle mAngleX;
		/* 0x18 */ TCamShakeAngle mAngleY;
		/* 0x24 */ TCamShakeAngle mAngleZ;
	};

	TCameraShake();
	TCamShakeInfo* getUseShakeData_();
	void setShakeAngleOne_(TCameraShake::TCamShakeAngle*, f32, s16, u16, f32);
	void setShakeAngleAll_(TCameraShake::TCamShakeInfo*, const TCamSaveShake*,
	                       u16, f32);
	void startShake(EnumCamShakeMode, f32);
	void keepShake(EnumCamShakeMode, f32);
	void execShake(const JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>*,
	               JGeometry::TVec3<f32>*);

	static const char* mCamShakeNameSave[];

public:
	/* 0x000 */ s16 mRollAccum;
	/* 0x004 */ TCamShakeInfo mShakeInfo[32];
	/* 0x604 */ TCamSaveShake* mShakeData[41];
};

#endif
