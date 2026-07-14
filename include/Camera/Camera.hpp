#ifndef CAMERA_CAMERA_HPP
#define CAMERA_CAMERA_HPP

#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRCamera.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/CameraMarioData.hpp>
#include <Camera/CameraKindParam.hpp>
#include <Camera/CameraInbetween.hpp>
#include <Camera/CameraMode.hpp>
#include <Camera/cameralib.hpp>

class TBaseNPC;
class TCameraMapTool;
class TMarioGamePad;
class TCameraInbetween;
class TCameraKindParam;
class TCameraBck;
class TCamSaveKindParam;
class TCameraJetCoaster;
class TCameraMultiPlayer;
class TCamSaveNotice;
class TCamSaveEx;
class TLiveActor;

class TTargetCamera {
public:
	TTargetCamera& operator=(const TTargetCamera& other)
	{
		mPosition = other.mPosition;
		mTarget   = other.mTarget;
		unk18     = other.unk18;
		mPitch    = other.mPitch;
		mYaw      = other.mYaw;
		unk28     = other.unk28;
		unk2C     = other.unk2C;
		unk30     = other.unk30;

		return *this;
	}

	/* 0x0 */ JGeometry::TVec3<f32> mPosition;
	/* 0xC */ JGeometry::TVec3<f32> mTarget;
	/* 0x18 */ JGeometry::TVec3<f32> unk18;
	/* 0x24 */ s16 mPitch;
	/* 0x26 */ s16 mYaw;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ s16 unk2C;
	/* 0x30 */ f32 unk30;
};

class CPolarSubCamera : public JDrama::TLookAtCamera {
public:
	enum EnumNoticeOnOffMode {
		NOTICE_MODE_UNK0 = 0,
		NOTICE_MODE_UNK1 = 1,
		NOTICE_MODE_UNK2 = 2,
	};

	CPolarSubCamera(const char* = "<CPolarCamera>");

	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	bool isLButtonCameraSpecifyMode(int) const;
	bool isLButtonCameraInbetween() const;
	bool isJetCoaster1stCamera() const;
	bool isTalkCameraSpecifyMode(int) const;
	bool isTalkCameraInbetween() const;
	bool isNormalCameraSpecifyMode(int) const;
	bool isNormalCameraCompletely() const;
	bool isTowerCameraSpecifyMode(int) const;
	bool isFollowCameraSpecifyMode(int) const;
	bool isRailCameraSpecifyMode(int) const;
	bool isFixCameraSpecifyMode(int) const;
	bool isDefiniteCameraSpecifyMode(int) const;
	bool isOverHipAttackSpecifyMode(int) const;
	bool isSlopeCameraMode() const;
	void warpPosAndAt(const Vec&, const Vec&);
	void warpPosAndAt(f32, s16);
	void addMoveCameraAndMario(const Vec&);
	void startGateDemoCamera(const JDrama::TActor*);
	void startDemoCamera(const char*, const JGeometry::TVec3<f32>*, s32, f32,
	                     bool);
	void endDemoCamera();
	bool isSimpleDemoCamera() const;
	int getTotalDemoFrames() const;
	// "Rest" as in "nokori" as in REMAINING frames
	int getRestDemoFrames() const;

	bool isHellDeadDemo() const;
	bool isNormalDeadDemo() const;
	void setNoticeInfo();

	void createMultiPlayer(u8);
	bool addMultiPlayer(const JGeometry::TVec3<f32>*, f32, f32);
	bool removeMultiPlayer(const JGeometry::TVec3<f32>*);
	void makeMtxForTalk(const TBaseNPC*);
	void makeMtxForPrevTalk();

	void startJetCoasterCam1();
	bool isNowInbetween() const;
	JGeometry::TVec3<f32> getUsualLookat() const;
	s16 getOffsetAngleX() const;
	s16 getOffsetAngleY() const;
	s16 getFinalAngleZ() const;

	// Fabricated
	s16 getUnk2C8() const { return unk2C8; }
	MtxPtr getUnk1EC() { return unk1EC; }
	const JGeometry::TVec3<f32>& getUnk124() const { return unk124; }
	s16 getUnk258() const { return unk258; }
	bool isThing() const
	{
		return isTalkCameraSpecifyMode(mMode) && !isNowInbetween() ? true
		                                                           : false;
	}
	bool isThing2() const
	{
		return isLButtonCameraSpecifyMode(mMode) && !isNowInbetween() ? true
		                                                              : false;
	}
	bool isThing3() const
	{
		bool result = false;
		if (unk2AC->unk8 > 0.3f || isThing() || isThing2())
			result = true;
		return result;
	}
	MtxPtr getUnk16C() { return unk16C; }
	MtxPtr getUnk1AC() { return unk1AC; }

	// Fabricated name but real inline
	bool fabricatedInline3()
	{
		bool result = true;
		if (mMode == CAMERA_MODE_UNDER_GROUND
		    || (mPrevMode == CAMERA_MODE_UNDER_GROUND
		        && (isNowInbetween() || mMode == CAMERA_MODE_JUMP_CODE)))
			result = false;

		return result;
	}

	bool isLButtonCamera() const { return isLButtonCameraSpecifyMode(mMode); }
	bool isBckDemoCamera() const
	{
		return mMode == CAMERA_MODE_REPRODUCE_DEMO ? true : false;
	}
	bool isDemoCamera() const
	{
		return isSimpleDemoCamera() || isBckDemoCamera() ? true : false;
	}

	bool isFixOrDefiniteCameraSpecifyMode(int mode) const
	{
		return isFixCameraSpecifyMode(mode)
		       || isDefiniteCameraSpecifyMode(mode);
	}

private:
	// fabricated
	void fabricatedInline2()
	{
		CLBCrossToPolar(mTarget, mPosition, &unk256, &unk258);

		unk25C.set(unk148.x - unk124.x, unk148.y - unk124.y,
		           unk148.z - unk124.z);
		unk25C.normalize();
		unk270 = MsClamp(CLBCalcRatio(mCurrentParams->mXAngleMin,
		                              mCurrentParams->mXAngleMax, unk256),
		                 0.0f, 1.0f);
	}

	void calcSecureViewTarget_(s16, f32*, f32*);
	void execSecureView_(s16, Vec*);

	MtxPtr getToroccoMtx_() const;
	void setMarioLookat_();

	bool startReproduceDemoCamera_(const char*, const JGeometry::TVec3<f32>*);
	void restartReproduceDemoCamera_();
	void endReproduceDemoCamera_();
	void endSimpleDemoCamera_();
	void updateDemoCamera_(bool);
	void updateGateDemoCamera_();

	s16 calcAngleXFromXRotRatio_() const;
	f32 calcDistFromXRotRatio_() const;
	void calcNowTargetFromPosAndAt_(const Vec& pos, const Vec& at);
	void rotateX_ByStickY_(f32);
	void rotateY_ByStickX_(f32);
	void offMoveApproach_();
	void onMoveApproach_();
	bool isMarioReadyGun_() const;
	bool isMarioAimWithGun_() const;
	bool isMarioCrabWalk_() const;
	void execInvalidAutoChase_();
	bool isMomentDefinite_() const;
	void calcSlopeAngleX_(s16*);
	void calcPosAndAt_();
	void calcFinalPosAndAt_();
	void calcExternalData_();
	void ctrlGameCamera_();

	void drawJetCoasterBalloonMessage_();

	void ctrlNormalDeadDemo_();
	void execDeadDemoProc_();
	void chaseOptionCamera_(f32);
	void ctrlOptionCamera_();
	void ctrlJetCoasterCamera_();
	void ctrlMultiPlayerCamera_();
	void ctrlTalkCamera_();
	void calcTowerCenterPos_(Vec* result);
	void ctrlNormalOrTowerCamera_();
	TLiveActor* getNoticeActor_();
	void execNoticeOnOffProc_(CPolarSubCamera::EnumNoticeOnOffMode);
	void calcNoticeTargetYrot_(const Vec&);
	void getNozzleTopPos_(JGeometry::TVec3<f32>*) const;
	void ctrlLButtonCamera_();
	void killHeightPanWhenChangeCamMode_();
	void execHeightPan_();
	void killHeightPan_();

	bool isNotHeightPanCamMode_() const
	{
		(void)0;
		(void)0;
		(void)0;
		(void)0;
		(void)0;
		(void)0;
		(void)0;
		bool bVar1 = false;
		if (isLButtonCameraSpecifyMode(mMode)
		    || isRailCameraSpecifyMode(mMode)) {
			bVar1 = true;
		} else {
			switch (mMode) {
			case CAMERA_MODE_MARE_UNDER_GROUND:
			case CAMERA_MODE_UNDER_GROUND:
			case CAMERA_MODE_HANG:
			case CAMERA_MODE_HOVERING:
			case CAMERA_MODE_JUMP_CODE:
			case CAMERA_MODE_DIVING:
			case CAMERA_MODE_SWIMMING:
			case CAMERA_MODE_LOOK_DOWN:
			case CAMERA_MODE_MONTE_HANG:
			case CAMERA_MODE_TOWER_E:
				bVar1 = true;
			}
		}
		return bVar1;
	}
	bool fabricatedInline()
	{
		bool result = false;
		if (!isNotHeightPanCamMode_() && !SMS_IsMarioTouchGround4cm()
		    && !gpCameraMario->isMarioGoDown() && !SMS_IsMarioOnWire()
		    && SMS_GetMarioStatus() != MARIO_STATUS_KICK_ROOF_ROLL_UP)
			result = true;
		return result;
	}

	bool controlByCameraCode_(int*);
	void getLButtonCameraModeByNozzle_();
	s16 getCameraInbetweenFrame_(int);
	void setUpToLButtonCamera_(int);
	void setUpFromLButtonCamera_();
	void changeCamMode_(int mode)
	{
		changeCamModeSpecifyFrame_(mode, getCameraInbetweenFrame_(mode));
	}
	void changeCamModeSub_(int mode, int tween_frames, bool force);
	void changeCamModeSpecifyFrame_(int mode, int tween_frames);
	void changeCamModeSpecifyCamMapTool_(const TCameraMapTool*);
	void changeCamModeSpecifyCamMapToolAndFrame_(const TCameraMapTool* tool,
	                                             int tween_frames);
	void execFrontRotate_();
	void doLButtonCameraOn_();
	void doLButtonCameraOff_(bool);
	bool isChangeToBossGesoCamera_() const;
	bool isChangeToCancanCamera_() const;
	bool isChangeToParallelCameraByMoveBG_() const;
	bool isChangeToParallelCameraCByMoveBG_() const;
	void execCameraModeChangeProc_(int);
	void calcInHouseNoSub_();
	void calcInHouseNo_(bool);
	bool isNeedGroundCheck_();
	bool isNeedRoofCheck_() const;
	bool isNeedWallCheck_() const;
	bool execWallCheck_(Vec*);
	bool execRoofCheck_(Vec);
	bool execGroundCheck_(Vec);

public:
	/* 0x50 */ int mMode;
	/* 0x54 */ int mPrevMode;
	/* 0x58 */ int mSavedModeBeforeTalk;
	/* 0x5C */ int mInitialMode;

	struct CameraUnk60Struct {

		CameraUnk60Struct(int count)
		    : unk0(count)
		    , unk4(0)
		    , unk8(nullptr)
		{
			unk8 = new int[unk0];
		}

		int getThing() const
		{
			if (unk4 <= 0)
				return (int)unk8; // what

			return unk8[unk4 - 1];
		}

		void popThing()
		{
			if (unk4 > 0)
				--unk4;
		}

		void doStuff(const int& param_1)
		{
			if (unk4 >= unk0) {
				for (int i = 0; i < unk0 - 1; ++i)
					unk8[i] = unk8[i + 1];
				unk8[unk0 - 1] = param_1;
			} else {
				unk8[unk4] = param_1;
				++unk4;
			}
		}

		/* 0x0 */ int unk0;
		/* 0x4 */ int unk4;
		/* 0x8 */ int* unk8;
	};

	/* 0x60 */ CameraUnk60Struct* unk60;

	enum {
		CAMERA_FLAG_UNK1              = 0x1,
		CAMERA_FLAG_UNK2              = 0x2,
		CAMERA_FLAG_UNK4              = 0x4,
		CAMERA_FLAG_UNK8              = 0x8,
		CAMERA_FLAG_UNK10             = 0x10,
		CAMERA_FLAG_NOTICE_ACTIVE     = 0x20,
		CAMERA_FLAG_UNK40             = 0x40,
		CAMERA_FLAG_UNK80             = 0x80,
		CAMERA_FLAG_UNK100            = 0x100,
		CAMERA_FLAG_GATE_DEMO         = 0x200,
		CAMERA_FLAG_DEAD_DEMO         = 0x400,
		CAMERA_FLAG_HELL_DEAD_DEMO    = 0x800,
		CAMERA_FLAG_JET_COASTER_SCENE = 0x1000,
	};

	/* 0x64 */ u16 unk64;
	/* 0x68 */ TCameraKindParam* mCurrentParams;
	/* 0x6C */ TCameraInbetween* mInbetween;
	/* 0x70 */ const TCameraMapTool* unk70;
	/* 0x74 */ const TCameraMapTool* unk74;
	/* 0x78 */ u32 mPosFreezeFrames;
	/* 0x7C */ u32 mTargetFreezeFrames;
	/* 0x80 */ TTargetCamera mCurrentTarget;
	/* 0xB4 */ TTargetCamera mPreviousTarget;
	/* 0xE8 */ TTargetCamera mTargetBeforeFixedMode;
	/* 0x11C */ u32 unk11C;
	/* 0x120 */ TMarioGamePad* unk120;
	/* 0x124 */ JGeometry::TVec3<f32> unk124;
	/* 0x130 */ JGeometry::TVec3<f32> unk130;
	/* 0x13C */ JGeometry::TVec3<f32> unk13C;
	/* 0x148 */ JGeometry::TVec3<f32> unk148;
	/* 0x154 */ JGeometry::TVec3<f32> unk154;
	/* 0x160 */ JGeometry::TVec3<f32> unk160;
	/* 0x16C */ Mtx44 unk16C;
	/* 0x1AC */ Mtx44 unk1AC;
	/* 0x1EC */ TPosition3f unk1EC;
	/* 0x21C */ Mtx unk21C;
	/* 0x24C */ f32 mHeightPanOffset;
	/* 0x250 */ f32 unk250;
	/* 0x254 */ s16 unk254;
	/* 0x256 */ s16 unk256;
	/* 0x258 */ s16 unk258;
	/* 0x25C */ JGeometry::TVec3<f32> unk25C;
	/* 0x268 */ f32 unk268;
	/* 0x26C */ f32 unk26C;
	/* 0x270 */ f32 unk270;
	/* 0x274 */ s16 unk274;
	/* 0x276 */ s16 unk276;
	/* 0x278 */ u16 unk278;
	/* 0x27A */ u16 unk27A;
	/* 0x27C */ u16 mDeadDemoCountdown;
	/* 0x27E */ u16 mDeadDemoCountdownToFovZoom;
	/* 0x280 */ u16 mDeadDemoFovZoomTimer;
	/* 0x282 */ u16 unk282;
	/* 0x284 */ s32 unk284;
	/* 0x288 */ f32 unk288;
	/* 0x28C */ s16 unk28C;
	/* 0x28E */ s16 unk28E;
	/* 0x290 */ f32 unk290;
	/* 0x294 */ f32 unk294;
	/* 0x298 */ f32 unk298;
	/* 0x29C */ int unk29C;
	/* 0x2A0 */ TLiveActor** unk2A0;
	/* 0x2A4 */ TLiveActor* mNoticeActor;
	/* 0x2A8 */ TLiveActor* unk2A8;

	struct CameraUnk2ACStruct {
		CameraUnk2ACStruct() { reset(); }

		void reset()
		{
			unk0 = 0;
			unk4 = 0.0f;
			unk8 = 0.0f;
			unkC = 0.0f;
		}

		/* 0x0 */ s16 unk0;
		/* 0x4 */ f32 unk4;
		/* 0x8 */ f32 unk8;
		/* 0xC */ f32 unkC;
	};

	/* 0x2AC */ CameraUnk2ACStruct* unk2AC;
	/* 0x2B0 */ TCameraBck* unk2B0;

	// Fabricated, it all got inlined D:
	struct TCameraDemo {
		TCameraDemo()
		    : unk0(nullptr)
		    , unk4(0.0f)
		    , unk8(nullptr)
		    , unkC(0)
		    , mTotalFrames(0)
		    , mRemainingFrames(0)
		{
		}

		void setLengthFrames(int frames)
		{
			mTotalFrames     = frames;
			mRemainingFrames = frames;
		}

		/* 0x0 */ const JGeometry::TVec3<f32>* unk0;
		/* 0x4 */ f32 unk4;
		/* 0x8 */ TCameraMapTool* unk8;
		/* 0xC */ u8 unkC;
		/* 0x10 */ int mTotalFrames;
		/* 0x14 */ int mRemainingFrames;
	};

	/* 0x2B4 */ TCameraDemo* mCameraDemo;
	/* 0x2B8 */ TCameraJetCoaster* unk2B8;
	/* 0x2BC */ TCameraMultiPlayer* unk2BC;
	/* 0x2C0 */ f32 unk2C0;
	/* 0x2C4 */ f32 unk2C4;
	/* 0x2C8 */ s16 unk2C8;
	/* 0x2CA */ s16 unk2CA;
	/* 0x2CC */ u8 unk2CC;
	/* 0x2D0 */ TCamSaveNotice* mSaveNotice;
	/* 0x2D4 */ TCamSaveEx* mSaveEx;
	/* 0x2D8 */ TCamSaveKindParam* mSaveKindParam[73];

	static const char* mCamKindNameSaveFile[73];
};

extern CPolarSubCamera* gpCamera;

#endif
