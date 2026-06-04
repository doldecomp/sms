#ifndef CAMERA_CAMERA_HPP
#define CAMERA_CAMERA_HPP

#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRCamera.hpp>
#include <dolphin/mtx.h>

class TBaseNPC;
class TCameraMapTool;
class TMarioGamePad;
class TCameraInbetween;
class TCameraKindParam;
class TBossGesso;
class TCameraBck;
class TCamSaveKindParam;
class TCameraJetCoaster;
class TCamSaveNotice;
class TCamSaveEx;
class TLiveActor;

class TTargetCamera {
public:
	/* 0x0 */ JGeometry::TVec3<f32> unk0;
	/* 0xC */ JGeometry::TVec3<f32> unkC;
	/* 0x18 */ Vec unk18;
	/* 0x24 */ s16 unk24;
	/* 0x26 */ s16 unk26;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ s16 unk2C;
	/* 0x30 */ u32 unk30;
};

class CPolarSubCamera : public JDrama::TLookAtCamera {
public:
	enum EnumNoticeOnOffMode { };

	CPolarSubCamera(const char* = "<CPolarCamera>");

	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);

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
	void getTotalDemoFrames() const;
	int getRestDemoFrames() const;

	void isHellDeadDemo() const;
	void isNormalDeadDemo() const;
	void setNoticeInfo();

	void createMultiPlayer(u8);
	void addMultiPlayer(const JGeometry::TVec3<f32>*, f32, f32);
	void removeMultiPlayer(const JGeometry::TVec3<f32>*);
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
	bool isThing4(int mode) const
	{
		return isFixCameraSpecifyMode(mode)
		       || isDefiniteCameraSpecifyMode(mode);
	}

private:
	void calcSecureViewTarget_(s16, f32*, f32*);
	void execSecureView_(s16, Vec*);

	void getToroccoMtx_() const;
	void setMarioLookat_();

	void startReproduceDemoCamera_(const char*, const JGeometry::TVec3<f32>*);
	void restartReproduceDemoCamera_();
	void endReproduceDemoCamera_();
	void endSimpleDemoCamera_();
	void updateDemoCamera_(bool);
	void updateGateDemoCamera_();

	void calcAngleXFromXRotRatio_() const;
	void calcDistFromXRotRatio_() const;
	void calcNowTargetFromPosAndAt_(const Vec&, const Vec&);
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
	void isNotHeightPanCamMode_() const;
	void execHeightPan_();
	void killHeightPan_();

	bool controlByCameraCode_(int*);
	void getLButtonCameraModeByNozzle_();
	s16 getCameraInbetweenFrame_(int);
	void setUpToLButtonCamera_(int);
	void setUpFromLButtonCamera_();
	void changeCamModeSub_(int, int, bool);
	void changeCamModeSpecifyFrame_(int, int);
	void changeCamModeSpecifyCamMapTool_(const TCameraMapTool*);
	void changeCamModeSpecifyCamMapToolAndFrame_(const TCameraMapTool*, int);
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
	/* 0x54 */ int unk54;
	/* 0x58 */ u32 unk58;
	/* 0x5C */ u32 unk5C;
	/* 0x60 */ u32 unk60;
	/* 0x64 */ u16 unk64;
	/* 0x68 */ TCameraKindParam* unk68;
	/* 0x6C */ TCameraInbetween* unk6C;
	/* 0x70 */ TCameraMapTool* unk70;
	/* 0x74 */ u32 unk74;
	/* 0x78 */ u32 unk78;
	/* 0x7C */ u32 unk7C;
	/* 0x80 */ TTargetCamera unk80;
	/* 0xB4 */ TTargetCamera unkB4;
	/* 0xE8 */ TTargetCamera unkE8;
	/* 0x11C */ u32 unk11C;
	/* 0x120 */ TMarioGamePad* unk120;
	/* 0x124 */ JGeometry::TVec3<f32> unk124;
	/* 0x130 */ Vec unk130;
	/* 0x13C */ JGeometry::TVec3<f32> unk13C;
	/* 0x148 */ JGeometry::TVec3<f32> unk148;
	/* 0x154 */ Vec unk154;
	/* 0x160 */ JGeometry::TVec3<f32> unk160;
	/* 0x16C */ Mtx44 unk16C;
	/* 0x1AC */ Mtx44 unk1AC;
	/* 0x1EC */ Mtx unk1EC;
	/* 0x21C */ Mtx unk21C;
	/* 0x24C */ f32 unk24C;
	/* 0x250 */ f32 unk250;
	/* 0x254 */ s16 unk254;
	/* 0x256 */ s16 unk256;
	/* 0x258 */ s16 unk258;
	/* 0x25C */ Vec unk25C;
	/* 0x268 */ f32 unk268;
	/* 0x26C */ f32 unk26C;
	/* 0x270 */ f32 unk270;
	/* 0x274 */ s16 unk274;
	/* 0x276 */ s16 unk276;
	/* 0x278 */ u16 unk278;
	/* 0x27A */ u16 unk27A;
	/* 0x27C */ s16 unk27C;
	/* 0x27E */ s16 unk27E;
	/* 0x280 */ s16 unk280;
	/* 0x282 */ s16 unk282;
	/* 0x284 */ s32 unk284;
	/* 0x288 */ f32 unk288;
	/* 0x28C */ s16 unk28C;
	/* 0x28E */ s16 unk28E;
	/* 0x290 */ f32 unk290;
	/* 0x294 */ f32 unk294;
	/* 0x298 */ f32 unk298;
	/* 0x29C */ int unk29C;
	/* 0x2A0 */ TLiveActor** unk2A0;
	/* 0x2A4 */ TLiveActor* unk2A4;
	/* 0x2A8 */ TLiveActor* unk2A8;

	struct CameraUnk2ACStruct {
		/* 0x0 */ s16 unk0;
		/* 0x4 */ f32 unk4;
		/* 0x8 */ f32 unk8;
		/* 0xC */ f32 unkC;
	};

	/* 0x2AC */ CameraUnk2ACStruct* unk2AC;
	/* 0x2B0 */ TCameraBck* unk2B0;

	struct CameraUnk2B4Struct {
		/* 0x0 */ u32 unk0;
		/* 0x4 */ f32 unk4;
		/* 0x8 */ u32 unk8;
		/* 0xC */ u8 unkC;
		/* 0x10 */ u32 unk10;
		/* 0x14 */ u32 unk14;
	};

	/* 0x2B4 */ CameraUnk2B4Struct* unk2B4;
	/* 0x2B8 */ TCameraJetCoaster* unk2B8;
	/* 0x2BC */ int unk2BC;
	/* 0x2C0 */ f32 unk2C0;
	/* 0x2C4 */ f32 unk2C4;
	/* 0x2C8 */ s16 unk2C8;
	/* 0x2CA */ s16 unk2CA;
	/* 0x2CC */ u8 unk2CC;
	/* 0x2D0 */ TCamSaveNotice* unk2D0;
	/* 0x2D4 */ TCamSaveEx* unk2D4;
	/* 0x2D8 */ TCamSaveKindParam* unk2D8[73];

	static const char* mCamKindNameSaveFile[73];
};

extern CPolarSubCamera* gpCamera;

#endif
