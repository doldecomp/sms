#ifndef CAMERA_CAMERA_SHAKE_HPP
#define CAMERA_CAMERA_SHAKE_HPP

#include <JSystem/JGeometry.hpp>

class TCamSaveShake;

// TCameraShake indexes mShakeData directly with the mode, so
// TCameraShake::mCamShakeNameSave in src/Camera/CamShakeDefine.cpp is the
// authority on what every mode is: entry N is mode N's .prm file, and these
// names are that table with the "shake" prefix and the ".prm" dropped. The
// boss abbreviations are the game's own: Boha is the sand bird, Bopa the
// Petey, Boge the Gooper Blooper, Bowa the boss chain chomp, BEel the eel and
// BTelesa the Phantamanta.
enum EnumCamShakeMode {
	CAM_SHAKE_MODE_HIP_DROP               = 0x0,
	CAM_SHAKE_MODE_WALL_JUMP              = 0x1,
	CAM_SHAKE_MODE_DAMAGE                 = 0x2,
	CAM_SHAKE_MODE_ENEMY                  = 0x3,
	CAM_SHAKE_MODE_ENEMY2                 = 0x4,
	CAM_SHAKE_MODE_BUILDING_APPEAR        = 0x5,
	CAM_SHAKE_MODE_KILLER                 = 0x6,
	CAM_SHAKE_MODE_TAMANOKO               = 0x7,
	CAM_SHAKE_MODE_BOHA_SAND_CRASH        = 0x8,
	CAM_SHAKE_MODE_BOHA_SLIP              = 0x9,
	CAM_SHAKE_MODE_BOHA_STEP              = 0xA,
	CAM_SHAKE_MODE_BOHA_RUN               = 0xB,
	CAM_SHAKE_MODE_BOHA_GETUP             = 0xC,
	CAM_SHAKE_MODE_SAND_BOMB_APPEAR       = 0xD,
	CAM_SHAKE_MODE_BOPA_DOWN              = 0xE,
	CAM_SHAKE_MODE_BOPA_POPO              = 0xF,
	CAM_SHAKE_MODE_BOPA_GETUP             = 0x10,
	CAM_SHAKE_MODE_BOPA_DOWN_LOOP         = 0x11,
	CAM_SHAKE_MODE_BOGE_BEAK_HIT          = 0x12,
	CAM_SHAKE_MODE_BOGE_TUMBLE            = 0x13,
	CAM_SHAKE_MODE_BOGE_LAND              = 0x14,
	CAM_SHAKE_MODE_BOGE_TE_HIT            = 0x15,
	CAM_SHAKE_MODE_BOWA_LAND1             = 0x16,
	CAM_SHAKE_MODE_BOWA_LAND2             = 0x17,
	CAM_SHAKE_MODE_BEEL_ROLL              = 0x18,
	CAM_SHAKE_MODE_BEEL_OUT               = 0x19,
	CAM_SHAKE_MODE_BEEL_HANUKE_IN         = 0x1A,
	CAM_SHAKE_MODE_BEEL_SUCK_OPEN         = 0x1B,
	CAM_SHAKE_MODE_BEEL_SUCK_LOOP         = 0x1C,
	CAM_SHAKE_MODE_BEEL_SUCK_CLOSE        = 0x1D,
	CAM_SHAKE_MODE_BEEL_DIE               = 0x1E,
	CAM_SHAKE_MODE_BTELESA_DAMAGE         = 0x1F,
	CAM_SHAKE_MODE_BTELESA_HIT            = 0x20,
	CAM_SHAKE_MODE_BTELESA_DOWN           = 0x21,
	CAM_SHAKE_MODE_BTELESA_APPEAR         = 0x22,
	CAM_SHAKE_MODE_BTELESA_ROLL           = 0x23,
	CAM_SHAKE_MODE_CANNON_DEMO            = 0x24,
	CAM_SHAKE_MODE_BATHTUB_GRIP_BREAKING  = 0x25,
	CAM_SHAKE_MODE_BATHTUB_GRIP_BREAKING2 = 0x26,
	CAM_SHAKE_MODE_KOOPA_HIPDROP          = 0x27,
	CAM_SHAKE_MODE_BATHTUB_KILLER         = 0x28,
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
