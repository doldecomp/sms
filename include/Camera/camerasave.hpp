#ifndef CAMERA_CAMERA_SAVE_HPP
#define CAMERA_CAMERA_SAVE_HPP

#include <System/Params.hpp>
#include <System/ParamInst.hpp>

class TCamSaveKindParam : public TParams {
public:
	TCamSaveKindParam(const char*);

	/* 0x8 */ TParamRT<f32> mSLFovy;
	/* 0x1C */ TParamRT<f32> mSLNearClip;
	/* 0x30 */ TParamRT<f32> mSLDistMin;
	/* 0x44 */ TParamRT<f32> mSLDistMax;
	/* 0x58 */ TParamRT<f32> mSLCushionMin;
	/* 0x6C */ TParamRT<f32> mSLCushionMax;
	/* 0x80 */ TParamRT<s16> mSLXAngleMin;
	/* 0x94 */ TParamRT<s16> mSLXAngleMax;
	/* 0xA8 */ TParamRT<f32> mSLXRotRatioManualSpeed;
	/* 0xBC */ TParamRT<s16> mSLYAngleManualSpeedXMin;
	/* 0xD0 */ TParamRT<s16> mSLYAngleManualSpeedXMax;
	/* 0xE4 */ TParamRT<f32> mSLAtOffsetY;
	/* 0xF8 */ TParamRT<f32> mSLXRotRatioAtOffsetY;
	/* 0x10C */ TParamRT<f32> mSLTargetAtJumpOffsetY;
	/* 0x120 */ TParamRT<f32> mSLAtJumpOffsetSpeed;
	/* 0x134 */ TParamRT<f32> mSLHeightPanChaseRateY;
	/* 0x148 */ TParamRT<f32> mSLSecureViewChase;
	/* 0x15C */ TParamRT<f32> mSLSecureViewDistXMin;
	/* 0x170 */ TParamRT<f32> mSLSecureViewDistZMin;
	/* 0x184 */ TParamRT<f32> mSLSecureViewDistXMax;
	/* 0x198 */ TParamRT<f32> mSLSecureViewDistZMax;
	/* 0x1AC */ TParamRT<f32> mSLHoldAddDistXZMin;
	/* 0x1C0 */ TParamRT<f32> mSLHoldAddDistXZMax;
	/* 0x1D4 */ TParamRT<s16> mSLHoldOffsetAngleXMin;
	/* 0x1E8 */ TParamRT<s16> mSLHoldOffsetAngleXMax;
	/* 0x1FC */ TParamRT<s16> mSLOffsetAngleX;
	/* 0x210 */ TParamRT<s16> mSLOffsetAngleY;
	/* 0x224 */ TParamRT<f32> mSLOffsetLookatXZ;
	/* 0x238 */ TParamRT<s16> mSLMaxAddAngleY;
	/* 0x24C */ TParamRT<s32> mSLAutoChaseStartFrame;
	/* 0x260 */ TParamRT<s32> mSLAutoChaseCompleteFrame;
	/* 0x274 */ TParamRT<f32> mSLFollowSpeedXmin;
	/* 0x288 */ TParamRT<f32> mSLFollowSpeedXmax;
	/* 0x29C */ TParamRT<f32> mSLJumpFollowSpeedXmin;
	/* 0x2B0 */ TParamRT<f32> mSLJumpFollowSpeedXmax;
	/* 0x2C4 */ TParamRT<f32> mSLObstructMaginfXmin;
	/* 0x2D8 */ TParamRT<f32> mSLObstructMaginfXmax;
	/* 0x2EC */ TParamRT<f32> mSLInHouseMaginfXmin;
	/* 0x300 */ TParamRT<f32> mSLInHouseMaginfXmax;
	/* 0x314 */ TParamRT<f32> mSLLFollowMaginfXmin;
	/* 0x328 */ TParamRT<f32> mSLLFollowMaginfXmax;
	/* 0x33C */ TParamRT<f32> mSLPosChaseRateXZ;
	/* 0x350 */ TParamRT<f32> mSLPosChaseRateXZ_C;
	/* 0x364 */ TParamRT<f32> mSLPosChaseRateY;
	/* 0x378 */ TParamRT<f32> mSLPosChaseRateY_C;
	/* 0x38C */ TParamRT<f32> mSLAtChaseRateXZ;
	/* 0x3A0 */ TParamRT<f32> mSLAtChaseRateY;
	/* 0x3B4 */ TParamRT<s16> mSLInbetFollow;
	/* 0x3C8 */ TParamRT<s16> mSLInbetParallel;
	/* 0x3DC */ TParamRT<s16> mSLInbetMultiPlayer;
	/* 0x3F0 */ TParamRT<s16> mSLInbetWallJump;
	/* 0x404 */ TParamRT<s16> mSLInbetHipAttack;
	/* 0x418 */ TParamRT<s16> mSLInbetRocketJump;
	/* 0x42C */ TParamRT<s16> mSLInbetWire;
	/* 0x440 */ TParamRT<s16> mSLInbetLNormal;
	/* 0x454 */ TParamRT<s16> mSLInbetMareUnderGround;
	/* 0x468 */ TParamRT<s16> mSLInbetDefiniteD2;
	/* 0x47C */ TParamRT<s16> mSLInbetTalkE;
	/* 0x490 */ TParamRT<s16> mSLInbetLeanMirror;
	/* 0x4A4 */ TParamRT<s16> mSLInbetTalkA;
	/* 0x4B8 */ TParamRT<s16> mSLInbetUnderGround;
	/* 0x4CC */ TParamRT<s16> mSLInbetIndoor;
	/* 0x4E0 */ TParamRT<s16> mSLInbetHang;
	/* 0x4F4 */ TParamRT<s16> mSLInbetWireHang;
	/* 0x508 */ TParamRT<s16> mSLInbetSandBird;
	/* 0x51C */ TParamRT<s16> mSLInbetHovering;
	/* 0x530 */ TParamRT<s16> mSLInbetJumpCode;
	/* 0x544 */ TParamRT<s16> mSLInbetDelfino;
	/* 0x558 */ TParamRT<s16> mSLInbetClimb;
	/* 0x56C */ TParamRT<s16> mSLInbetFixA;
	/* 0x580 */ TParamRT<s16> mSLInbetFixB;
	/* 0x594 */ TParamRT<s16> mSLInbetFixC;
	/* 0x5A8 */ TParamRT<s16> mSLInbetFixD;
	/* 0x5BC */ TParamRT<s16> mSLInbetFixE;
	/* 0x5D0 */ TParamRT<s16> mSLInbetFixF;
	/* 0x5E4 */ TParamRT<s16> mSLInbetFixG;
	/* 0x5F8 */ TParamRT<s16> mSLInbetFixH;
	/* 0x60C */ TParamRT<s16> mSLInbetDefiniteA;
	/* 0x620 */ TParamRT<s16> mSLInbetDefiniteB;
	/* 0x634 */ TParamRT<s16> mSLInbetDefiniteC;
	/* 0x648 */ TParamRT<s16> mSLInbetDefiniteD;
	/* 0x65C */ TParamRT<s16> mSLInbetDefiniteE;
	/* 0x670 */ TParamRT<s16> mSLInbetDefiniteF;
	/* 0x684 */ TParamRT<s16> mSLInbetDefiniteG;
	/* 0x698 */ TParamRT<s16> mSLInbetDefiniteH;
	/* 0x6AC */ TParamRT<s16> mSLInbetExMap0;
	/* 0x6C0 */ TParamRT<s16> mSLInbetTowerA;
	/* 0x6D4 */ TParamRT<s16> mSLInbetTowerB;
	/* 0x6E8 */ TParamRT<s16> mSLInbetTowerC;
	/* 0x6FC */ TParamRT<s16> mSLInbetSlider;
	/* 0x710 */ TParamRT<s16> mSLInbetDiving;
	/* 0x724 */ TParamRT<s16> mSLInbetTurbo;
	/* 0x738 */ TParamRT<s16> mSLInbetTalkB;
	/* 0x74C */ TParamRT<s16> mSLInbetJetCoaster;
	/* 0x760 */ TParamRT<s16> mSLInbetParallelB;
	/* 0x774 */ TParamRT<s16> mSLInbetSurfing;
	/* 0x788 */ TParamRT<s16> mSLInbetSwimming;
	/* 0x79C */ TParamRT<s16> mSLInbetClimbJump;
	/* 0x7B0 */ TParamRT<s16> mSLInbetLookDown;
	/* 0x7C4 */ TParamRT<s16> mSLInbetRailFence;
	/* 0x7D8 */ TParamRT<s16> mSLInbetFollowB;
	/* 0x7EC */ TParamRT<s16> mSLInbetFollowC;
	/* 0x800 */ TParamRT<s16> mSLInbetTowerD;
	/* 0x814 */ TParamRT<s16> mSLInbetDelfinoAttic;
	/* 0x828 */ TParamRT<s16> mSLInbetBossGeso;
	/* 0x83C */ TParamRT<s16> mSLInbetFixI;
	/* 0x850 */ TParamRT<s16> mSLInbetDefiniteI;
	/* 0x864 */ TParamRT<s16> mSLInbetFence;
	/* 0x878 */ TParamRT<s16> mSLInbetMonteFence;
	/* 0x88C */ TParamRT<s16> mSLInbetMonteHang;
	/* 0x8A0 */ TParamRT<s16> mSLInbetTalkC;
	/* 0x8B4 */ TParamRT<s16> mSLInbetTalkD;
	/* 0x8C8 */ TParamRT<s16> mSLInbetTowerE;
	/* 0x8DC */ TParamRT<s16> mSLInbetDelfinoB;
	/* 0x8F0 */ TParamRT<s16> mSLInbetCancan;
	/* 0x904 */ TParamRT<s16> mSLInbetAquaticTurbo;
	/* 0x918 */ TParamRT<s16> mSLInbetFollowD;
	/* 0x92C */ TParamRT<s16> mSLInbetFollowE;
	/* 0x940 */ TParamRT<s16> mSLInbetParallelC;
	/* 0x954 */ TParamRT<s16> mSLInbetParallelD;
};

class TCamSaveEx : public TParams {
public:
	TCamSaveEx();

	/* 0x8 */ TParamRT<f32> mXRotStart;
	/* 0x1C */ TParamRT<f32> mPanAfterMagnif;
	/* 0x30 */ TParamRT<f32> mPanAfterMinHeight;
	/* 0x44 */ TParamRT<s16> mPanWarpAngleX;
	/* 0x58 */ TParamRT<f32> mSLMinCushionXZ;
	/* 0x6C */ TParamRT<f32> mSLWallCheckRadius;
	/* 0x80 */ TParamRT<f32> mSLWallRevisionRatio;
	/* 0x94 */ TParamRT<f32> mGroundChangeY;
	/* 0xA8 */ TParamRT<f32> mSLGroundHeightNormal;
	/* 0xBC */ TParamRT<f32> mSLGroundHeightReadyGun;
	/* 0xD0 */ TParamRT<f32> mSLRoofChangeY;
	/* 0xE4 */ TParamRT<f32> mSLRoofHeight;
	/* 0xF8 */ TParamRT<u8> mInHouseMinFrame;
	/* 0x10C */ TParamRT<s16> mYButtonRotateChase;
	/* 0x120 */ TParamRT<s16> mLButtonRotateChase;
	/* 0x134 */ TParamRT<s16> mSLAddAngleYSpeed;
	/* 0x148 */ TParamRT<f32> mSLReproduceDemoNearClip;
	/* 0x15C */ TParamRT<s16> mSLHoldAngleXChase;
	/* 0x170 */ TParamRT<f32> mSLHoldDistChase;
	/* 0x184 */ TParamRT<s16> mSLAimAngleYChaseMin;
	/* 0x198 */ TParamRT<s16> mSLAimAngleYChaseMax;
	/* 0x1AC */ TParamRT<s16> mSLLimitMinAngleX;
	/* 0x1C0 */ TParamRT<s16> mSLLimitMaxAngleX;
	/* 0x1D4 */ TParamRT<s16> mSLSlopeMaxAngleX;
	/* 0x1E8 */ TParamRT<s16> mSLSlopeSpeedAngleX;
	/* 0x1FC */ TParamRT<f32> mSLSlopeForwardDistXZ;
};

class TCamSaveJetCoaster : public TParams {
public:
	TCamSaveJetCoaster();

	/* 0x8 */ TParamRT<s16> mSLOffsetAngleXLimit;
	/* 0x1C */ TParamRT<s16> mSLOffsetAngleYLimit;
	/* 0x30 */ TParamRT<s16> mSLOffsetAngleXManualSpeed;
	/* 0x44 */ TParamRT<s16> mSLOffsetAngleYManualSpeed;
	/* 0x58 */ TParamRT<s16> mSLOffsetAngleXChase;
	/* 0x6C */ TParamRT<s16> mSLOffsetAngleYChase;
};

class TCamSaveShake : public TParams {
public:
	TCamSaveShake(const char*);

	/* 0x8 */ TParamRT<s16> mShakeTime;
	/* 0x1C */ TParamRT<f32> mShakeAmpX;
	/* 0x30 */ TParamRT<s16> mShakeVelX;
	/* 0x44 */ TParamRT<f32> mShakeAmpY;
	/* 0x58 */ TParamRT<s16> mShakeVelY;
	/* 0x6C */ TParamRT<f32> mShakeAmpZ;
	/* 0x80 */ TParamRT<s16> mShakeVelZ;
};

class TCamSaveNotice : public TParams {
public:
	TCamSaveNotice();

	/* 0x8 */ TParamRT<f32> mOnDist;
	/* 0x1C */ TParamRT<f32> mOffDist;
	/* 0x30 */ TParamRT<f32> mOnClipRatio;
	/* 0x44 */ TParamRT<f32> mOffClipRatio;
	/* 0x58 */ TParamRT<f32> mOnDegree;
	/* 0x6C */ TParamRT<s16> mRotateYSpeed;
	/* 0x80 */ TParamRT<f32> mRotateMinDistXZ;
	/* 0x94 */ TParamRT<f32> mRotateFastMinDistXZ;
	/* 0xA8 */ TParamRT<f32> mRotateMagnifXmax;
};

#endif
