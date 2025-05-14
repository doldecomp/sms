#ifndef CAMERA_CAMERA_HPP
#define CAMERA_CAMERA_HPP

#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRCamera.hpp>
#include <dolphin/mtx.h>

class TBaseNPC;
class TCameraMapTool;
class TMarioGamePad;

class CPolarSubCamera : public JDrama::TLookAtCamera {
public:
	enum EnumNoticeOnOffMode {};

	void calcSecureViewTarget_(s16, f32*, f32*);
	void execSecureView_(s16, Vec*);
	bool isLButtonCameraSpecifyMode(int) const;
	void isLButtonCameraInbetween() const;
	void isJetCoaster1stCamera() const;
	void isTalkCameraSpecifyMode(int) const;
	void isTalkCameraInbetween() const;
	void isNormalCameraSpecifyMode(int) const;
	void isNormalCameraCompletely() const;
	void isTowerCameraSpecifyMode(int) const;
	void isFollowCameraSpecifyMode(int) const;
	void isRailCameraSpecifyMode(int) const;
	void isFixCameraSpecifyMode(int) const;
	void isDefiniteCameraSpecifyMode(int) const;
	void isOverHipAttackSpecifyMode(int) const;
	void isSlopeCameraMode() const;
	void warpPosAndAt(const Vec&, const Vec&);
	void warpPosAndAt(f32, s16);
	void addMoveCameraAndMario(const Vec&);
	void startReproduceDemoCamera_(const char*, const JGeometry::TVec3<f32>*);
	void restartReproduceDemoCamera_();
	void endReproduceDemoCamera_();
	void endSimpleDemoCamera_();
	void updateDemoCamera_(bool);
	void updateGateDemoCamera_();
	void startGateDemoCamera(const JDrama::TActor*);
	void startDemoCamera(const char*, const JGeometry::TVec3<f32>*, s32, f32,
	                     bool);
	void endDemoCamera();
	bool isSimpleDemoCamera() const;
	void getTotalDemoFrames() const;
	void getRestDemoFrames() const;
	void ctrlNormalDeadDemo_();
	void execDeadDemoProc_();
	void isHellDeadDemo() const;
	void isNormalDeadDemo() const;
	void chaseOptionCamera_(f32);
	void ctrlOptionCamera_();
	void ctrlJetCoasterCamera_();
	void createMultiPlayer(u8);
	void addMultiPlayer(const JGeometry::TVec3<f32>*, f32, f32);
	void removeMultiPlayer(const JGeometry::TVec3<f32>*);
	void ctrlMultiPlayerCamera_();
	void makeMtxForTalk(const TBaseNPC*);
	void makeMtxForPrevTalk();
	void ctrlTalkCamera_();
	void calcTowerCenterPos_(Vec*);
	void ctrlNormalOrTowerCamera_();
	void setNoticeInfo();
	void getNoticeActor_();
	void execNoticeOnOffProc_(CPolarSubCamera::EnumNoticeOnOffMode);
	void calcNoticeTargetYrot_(const Vec&);
	void getNozzleTopPos_(JGeometry::TVec3<f32>*) const;
	void ctrlLButtonCamera_();
	void killHeightPanWhenChangeCamMode_();
	void isNotHeightPanCamMode_() const;
	void execHeightPan_();
	void killHeightPan_();

	CPolarSubCamera(const char*);
	void startJetCoasterCam1();
	void loadAfter();
	void isNowInbetween() const;
	void getToroccoMtx_() const;
	void setMarioLookat_();
	void getUsualLookat() const;
	void calcAngleXFromXRotRatio_() const;
	void calcDistFromXRotRatio_() const;
	void calcNowTargetFromPosAndAt_(const Vec&, const Vec&);
	void rotateX_ByStickY_(f32);
	void rotateY_ByStickX_(f32);
	void offMoveApproach_();
	void onMoveApproach_();
	void isMarioReadyGun_() const;
	void isMarioAimWithGun_() const;
	void isMarioCrabWalk_() const;
	void execInvalidAutoChase_();
	void isMomentDefinite_() const;
	void calcSlopeAngleX_(s16*);
	void calcPosAndAt_();
	void calcFinalPosAndAt_();
	void calcExternalData_();
	void ctrlGameCamera_();
	void perform(u32, JDrama::TGraphics*);
	void getOffsetAngleX() const;
	void getOffsetAngleY() const;
	void getFinalAngleZ() const;
	~CPolarSubCamera();
	void controlByCameraCode_(int*);
	void getLButtonCameraModeByNozzle_();
	void getCameraInbetweenFrame_(int);
	void setUpToLButtonCamera_(int);
	void setUpFromLButtonCamera_();
	void changeCamModeSub_(int, int, bool);
	void changeCamModeSpecifyFrame_(int, int);
	void changeCamModeSpecifyCamMapTool_(const TCameraMapTool*);
	void changeCamModeSpecifyCamMapToolAndFrame_(const TCameraMapTool*, int);
	void execFrontRotate_();
	void doLButtonCameraOn_();
	void doLButtonCameraOff_(bool);
	void isChangeToBossGesoCamera_() const;
	void isChangeToCancanCamera_() const;
	void isChangeToParallelCameraByMoveBG_() const;
	void isChangeToParallelCameraCByMoveBG_() const;
	void execCameraModeChangeProc_(int);
	void calcInHouseNoSub_();
	void calcInHouseNo_(bool);
	void isNeedGroundCheck_();
	void isNeedRoofCheck_() const;
	void isNeedWallCheck_() const;
	void execWallCheck_(Vec*);
	void execRoofCheck_(Vec);
	void execGroundCheck_(Vec);

	s16 getUnk2C8() const { return unk2C8; }

public:
	/* 0x50 */ int mMode;
	/* 0x54 */ char unk54[0x120 - 0x54];
	/* 0x120 */ TMarioGamePad* unk120;
	/* 0x124 */ JGeometry::TVec3<f32> unk124;
	/* 0x130 */ char unk130[0xC];
	/* 0x13C */ JGeometry::TVec3<f32> unk13C;
	/* 0x148 */ char unk148[0x1EC - 0x148];
	/* 0x1EC */ Mtx unk1EC;
	/* 0x21C */ char unk21C[0x2C8 - 0x21C];
	/* 0x2C8 */ s16 unk2C8;
};

extern CPolarSubCamera* gpCamera;

#endif
