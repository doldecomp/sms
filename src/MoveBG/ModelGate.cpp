#include <MoveBG/ModelGate.hpp>
#include <M3DUtil/MActorUtil.hpp>
#include <M3DUtil/SampleCtrlModel.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/ScreenUtil.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/Camera.hpp>
#include <System/FlagManager.hpp>
#include <System/Particles.hpp>
#include <Strategic/Strategy.hpp>
#include <THPPlayer/THPPlayer.h>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/ResTIMG.hpp>
#include <math.h>
#include <stdio.h>
#include <string.h>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

/// The gate model and its BTK/BRK share one name per destination.
static const char* gateMActorNames[] = {
	"05_gate01", "05_gate02rico", "05_gate03manma", "05_gate04monte",
	"05_gate05mare",
};

// The three YUV planes are resized through a binder on the ResTIMG: retail
// reloads `mResources` once per plane and folds the plane offset into the
// first store (`stbu`), which only happens when the three fields are written
// through one bound pointer.
static inline void setGateTexRes(ResTIMG* res, u16 width, u16 height)
{
	res->format = 1;
	res->width  = width;
	res->height = height;
}

// TODO: frame is 0x48 short (0x268 vs retail 0x2b0); every instruction
// matches.
void TModelGate::loadAfter()
{
	initHitActor(0x080000C0, 5, 0x80000000, 300.0f, 400.0f, 300.0f,
	             400.0f);

	static const char* gateNames[] = {
		"Gate", "GateToRicco", "GateToMamma", "GateToMonte", "GateToMare",
		nullptr,
	};

	onHitFlag(HIT_FLAG_NO_COLLISION);
	mFlags     = 0;
	unk71 = 0;
	for (u8 i = 0; i < 5; i++) {
		if (strcmp(gateNames[i], mName) == 0) {
			unk71 = i;
			break;
		}
	}

	u32 modelFlags = 0x11100000;
	char path[256];
	snprintf(path, 256, "/scene/map/map/gate/%s.bmd",
	         gateMActorNames[unk71]);
	unk78 = SMS_MakeMActor("/scene/map/map/gate", path, 0, modelFlags);
	unk72
	    = unk78->getModel()->getModelData()->unkB0->getIndex("center");

	// The gate's window shows the THP movie, so its three YUV planes have to
	// be resized to whatever is playing.
	if (ActivePlayer.open) {
		THPVideoInfo videoInfo;
		THPPlayerGetVideoInfo(&videoInfo);
		u16 width       = videoInfo.xSize;
		u16 height      = videoInfo.ySize;
		J3DTexture* tex = unk78->getModel()->getModelData()->unkAC;
		setGateTexRes(&tex->mResources[0], width, height);
		setGateTexRes(&tex->mResources[1], width >> 1, height >> 1);
		setGateTexRes(&tex->mResources[2], width >> 1, height >> 1);
	}

	unkB8          = 0;
	mSwirlStep     = 0;
	unkBA          = 0;
	mSwirlInterval = 240;
	mSwirlTimer    = mSwirlInterval;

	unk78->setBtk(gateMActorNames[unk71]);
	unk78->setBrk(gateMActorNames[unk71]);
	unk78->getFrameCtrl(5)->setRate(0.0f);

	mCtrlModelData
	    = new SampleCtrlModelData(unk78->getModel()->getModelData());

	unkC5       = 0x20;
	unkC6       = 0xFF;
	mState      = STATE_SWIRL;
	mWindTime   = 360;
	mWindTimeLeft = 0;
	mWindFrame  = 0;
	unkCE       = 60;
	mOpenRate   = 0.0f;
	mOpenSpeed  = 0.1f;
	mCloseSpeed = 0.02f;
	mIdleCloseSpeed = 0.025f;

	mScaling.x = 1.0f;
	mScaling.y = 1.0f;
	mScaling.z = 1.0f;

	JDrama::TNameRefGen::search<TIdxGroupObj>("マップグループ")
	    ->getChildren()
	    .push_back(this);

	Mtx mtx;
	SMS_GetActorMtx(*this, mtx);
	MTXCopy(mtx, unk78->getModel()->getBaseTRMtx());
	unk78->getModel()->calc();

	MTXTrans(mtx, 0.0f, 0.0f, 250.0f);
	MTXConcat(unk78->getModel()->getAnmMtx(unk72), mtx, mtx);
	unkAC.x = 0.0f;
	unkAC.y = 0.0f;
	unkAC.z = 0.0f;
	MTXMultVec(mtx, &unkAC, &unkAC);

	MTXInverse(unk78->getModel()->getAnmMtx(unk72), mInvCenterMtx);
	mCenterYaw = matan(unk78->getModel()->getAnmMtx(unk72)[2][2],
	                   unk78->getModel()->getAnmMtx(unk72)[0][2]);

	mBlurStrength    = 0;
	mBlurAlpha       = 0.0f;
	mBlurAlphaRate   = 0.01f;
	mBlurRadius      = 0.02f;
	mBlurNearDist    = 500.0f;
	mBlurFarDist     = 1000.0f;
	mHitParticleRate = 0.7f;
	unkFC            = 0.0f;
	unk100           = 200.0f;
	unk104           = 150.0f;
	unk108           = -1000.0f;
	unk10C           = 150.0f;
	unk110           = 0.0f;
	unk114           = 300.0f;

	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_body.jpa", 0x1A);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_head.jpa", 0x1B);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_cap.jpa", 0x1C);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_rhand.jpa", 0x1D);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_lhand.jpa", 0x1E);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_rleg.jpa", 0x1F);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_rfoot.jpa", 0x20);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_lleg.jpa", 0x21);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_lfoot.jpa", 0x22);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_watgun.jpa", 0x23);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_dust.jpa", 0x3C);
	SMS_LoadParticle("/scene/map/map/gate/ms_mariowp_senko.jpa", 0x51);
	SMS_LoadParticle("/scene/map/map/gate/ms_gatewind_a.jpa", 0x131);
	SMS_LoadParticle("/scene/map/map/gate/ms_gatewind_a2.jpa", 0x132);
	SMS_LoadParticle("/scene/map/map/gate/ms_gatewind_a3.jpa", 0x133);
	SMS_LoadParticle("/scene/map/map/gate/ms_gatewind_b.jpa", 0x134);
	SMS_LoadParticle("/scene/map/map/gate/ms_gatehit_a.jpa", 0x1DD);
	SMS_LoadParticle("/scene/map/map/gate/ms_gatehit_b.jpa", 0x1DE);

	bool opened = false;
	switch (unk71) {
	case 0:
		opened = TFlagManager::getInstance()->getBool(0x10385);
		break;
	case 1:
		opened = TFlagManager::getInstance()->getBool(0x10386);
		break;
	case 2:
		opened = TFlagManager::getInstance()->getBool(0x10387);
		break;
	case 3:
		opened = TFlagManager::getInstance()->getBool(0x10387);
		break;
	case 4:
		opened = TFlagManager::getInstance()->getBool(0x10387);
		break;
	}

	if (opened == true) {
		mFlags |= GATE_FLAG_ACTIVE;
		offHitFlag(HIT_FLAG_NO_COLLISION);
	} else {
		mFlags &= ~GATE_FLAG_ACTIVE;
		mFlags |= GATE_FLAG_OPENING;
		onHitFlag(HIT_FLAG_NO_COLLISION);
	}
}

void TModelGate::startOpen()
{
	mFlags |= GATE_FLAG_ACTIVE;
	mState = STATE_WAIT_ANIM;
	unk78->setBpk(gateMActorNames[unk71]);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mFlags |= GATE_FLAG_OPENING;
}

static inline void ModelGateStartRadialBlur(u8 alpha, f32 radius,
                                            JGeometry::TVec3<f32> dir)
{
	TAfterEffect* effect = gpAfterEffect;
	effect->unk15        = 2;
	effect->unk1C        = alpha;
	effect->unk50        = radius;
	effect->unk5C        = dir;
}

void TModelGate::screenBlur(JDrama::TGraphics* graphics)
{
	// Direction from the gate to Mario, in view space, so the radial blur
	// streaks towards the gate on screen.
	Vec viewDir;
	JGeometry::TVec3<f32> toMario;
	toMario.x = gpMarioPos->x - mPosition.x;
	toMario.y = 0.0f;
	toMario.z = gpMarioPos->z - mPosition.z;
	VECNormalize(toMario, toMario);
	MTXMultVecSR(graphics->mViewMtx, toMario, &viewDir);

	JGeometry::TVec3<f32> localPos;
	JGeometry::TVec3<f32> marioPos(*gpMarioPos);
	MTXMultVec(mInvCenterMtx, marioPos, localPos);

	f32 strength = 0.0f;
	if (-250.0f <= localPos.x && localPos.x <= 250.0f
	    && -250.0f <= localPos.y && localPos.y <= 250.0f
	    && 0.0f <= localPos.z && localPos.z <= mBlurFarDist) {
		if (localPos.z < mBlurNearDist)
			strength = 1.0f;
		if (mBlurNearDist <= localPos.z && localPos.z <= mBlurFarDist)
			strength = 1.0f
			    - (localPos.z - mBlurNearDist)
			        / (mBlurFarDist - mBlurNearDist);
		if (mBlurFarDist < localPos.z)
			strength = 0.0f;
	}

	// Only blur while the camera is roughly facing the gate.
	f32 target = (f32)mBlurStrength * strength;
	if ((s16)(182.04445f * mRotation.y - gpCamera->unk258) < -0x2AAA
	    || (s16)(182.04445f * mRotation.y - gpCamera->unk258) > 0x2AAA)
		target = 0.0f;

	mBlurAlpha += mBlurAlphaRate * (target - mBlurAlpha);

	// Retail hands the blur to one inline: gpAfterEffect is loaded once for
	// all four stores, `viewDir` is a plain Vec copied by floats into the
	// by-value TVec3 parameter, and the alpha is computed first.
	// TODO: frame 0x78 against retail's 0x98. Retail converts the facing
	// angle twice into two stack slots, both before the first compare
	// (ours converts the second one after the `blt`).
	u8 alpha = mBlurAlpha * (1.0f - gpCamera->unk270);
	ModelGateStartRadialBlur(alpha, mBlurRadius, viewDir);
}

BOOL TModelGate::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->mActorType == 0x80000001 && message == HIT_MESSAGE_ATTACK) {
		mWindTime = 0;
		mState    = STATE_HIT;
		return TRUE;
	}

	if (sender->mActorType == 0x01000001) {
		JGeometry::TVec3<f32> localPos;
		MtxPtr invCenter = mInvCenterMtx;
		MTXMultVec(invCenter, sender->getPosition(), localPos);
		Mtx center;
		MTXCopy(unk78->getModel()->getAnmMtx(unk72), center);

		if (localPos.x * localPos.x + localPos.y * localPos.y < 40000.0f
		    && -100.0f < localPos.z && localPos.z < unkFC) {
			if (mFlags & GATE_FLAG_OPENING) {
				mOpenRate += mOpenSpeed;
				if (mOpenRate > 1.0f) {
					mWindTimeLeft = mWindTime;
					mOpenRate     = 1.0f;
					mFlags &= ~GATE_FLAG_OPENING;
				}
			}
			if (MsRandF() < mHitParticleRate) {
				gpMarioParticleManager->emitWithRotate(
				    0x1DD, &sender->mPosition, 0, mCenterYaw, 0, 2, nullptr);
				gpMarioParticleManager->emitWithRotate(
				    0x1DE, &sender->mPosition, 0, mCenterYaw, 0, 2, nullptr);
			}
			return TRUE;
		}
	}

	return FALSE;
}

// TODO: literal-pool order. Ours emits a 4-byte 0.5f and 3.0f that the
// target's .sdata2 does not have at all (32 objects against the map's 30):
// retail keeps only the 8-byte 0.5/3.0 double pair (@3049/@3050, MWCC's
// inline sqrt), so one of this function's square roots is spelled on floats
// here and on doubles in retail.
// The open-range test copies Mario's position into a by-value parameter,
// subtracts in place, then copies the difference into a by-value length
// level that squares it and calls TUtil<f32>::sqrt out of line (the same
// shape as TYoshiTongue::movement's grab-range test).
static inline f32 ModelGateLength(JGeometry::TVec3<f32> v)
{
	return JGeometry::TUtil<f32>::sqrt(v.squared());
}

static inline f32 ModelGateDist(JGeometry::TVec3<f32> a,
                                const JGeometry::TVec3<f32>& b)
{
	a.sub(b);
	return ModelGateLength(a);
}

// TODO: frame 0x160 against retail's 0x220 (0xc0 short) and the callee-saved
// and volatile register choices around the swirl stage references differ;
// the instruction stream and the @3054 jump table match. The swirl fields
// are written through `u8&` locals: retail hoists every field address the
// switch stores through (`addi rN, base, off` before the table) and folds
// the unconditional resets into displacements, which only named references
// reproduce.
void TModelGate::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!(mFlags & GATE_FLAG_ACTIVE))
		return;

	if (cue & CUE_DRAW) {
		// Point the gate's three YUV textures at the current movie frame.
		if (ActivePlayer.open && ActivePlayer.dispTextureSet) {
			THPTextureSet* set = ActivePlayer.dispTextureSet;
			J3DTexture* tex    = unk78->getModel()->getModelData()->unkAC;
			tex->mResources[0].imageDataOffset
			    = (u32)set->ytexture - (u32)&tex->mResources[0];
			tex->mResources[1].imageDataOffset
			    = (u32)set->utexture - (u32)&tex->mResources[1];
			tex->mResources[2].imageDataOffset
			    = (u32)set->vtexture - (u32)&tex->mResources[2];
		}
	}

	unk78->perform(cue, graphics);

	if (cue & CUE_MOVE) {
		if (!(mFlags & GATE_FLAG_OPENING)) {
			if (ModelGateDist(*gpMarioPos, mPosition) < 1000.0f) {
				mOpenRate += 0.01f;
				if (mOpenRate > 1.0f) {
					mOpenRate     = 1.0f;
					mWindTimeLeft = mWindTime;
				}
			} else {
				mWindTimeLeft--;
				if (mWindTimeLeft <= 0)
					mWindTimeLeft = 0;
				mOpenRate = (f32)mWindTimeLeft / 1000.0f;
			}
		}

		JGeometry::TVec3<f32> localPos;
		MTXMultVec(mInvCenterMtx, *gpMarioPos, localPos);
		if (-unk104 < localPos.x && localPos.x < unk104
		    && unk108 < localPos.y && localPos.y < unk10C
		    && unk110 < localPos.z && localPos.z < unk114) {
			if (mWindTimeLeft > 0) {
				// Mario can only be caught mid-jump.
				bool jumping = false;
				if (SMS_IsMarioStatusTypeJumping())
					jumping = true;
				if (jumping == true) {
					if (SMS_GetMarioHitActor()->receiveMessage(
					        this, HIT_MESSAGE_TAKE)
					    == TRUE)
						mHeldObject = (TTakeActor*)SMS_GetMarioHitActor();
				}
			} else {
				// Blow Mario away from the gate.
				f32 dx  = gpMarioPos->x - mPosition.x;
				f32 dz  = gpMarioPos->z - mPosition.z;
				f32 len = std::sqrtf(dx * dx + dz * dz);
				if (len < unk100) {
					JGeometry::TVec3<f32> target(*gpMarioPos);
					target.x += 10.0f * (dx / len);
					target.z += 10.0f * (dz / len);
					SMS_MarioMoveRequest(target);
				}
			}
		}
	}

	if (cue & CUE_CALC_ANIM) {
		switch (mState) {
		case STATE_WAIT_ANIM:
			if (unk78->getFrameCtrl(2)->checkState(
			        J3DFrameCtrl::STATE_COMPLETED_ONCE
			        | J3DFrameCtrl::STATE_LOOPED_ONCE))
				mState = STATE_SWIRL;
			break;

		case STATE_SWIRL:
			if (mWindTimeLeft > 0) {
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    0x131, unk78->getModel()->getAnmMtx(unk72), 1,
				    this);
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    0x132, unk78->getModel()->getAnmMtx(unk72), 1,
				    this);
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    0x133, unk78->getModel()->getAnmMtx(unk72), 1,
				    this);
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    0x134, unk78->getModel()->getAnmMtx(unk72), 1,
				    this);
				mWindTimeLeft--;
				mWindFrame++;
				if (gpMSound->gateCheck(MSD_SE_OBJ_GATE_LIGHT))
					MSoundSESystem::MSoundSE::startSoundActor(
					    MSD_SE_OBJ_GATE_LIGHT, &mPosition, 0, nullptr, 0, 4);
				if (gpMSound->gateCheck(MSD_SE_OBJ_GATE_WAVE))
					MSoundSESystem::MSoundSE::startSoundActor(
					    MSD_SE_OBJ_GATE_WAVE, &mPosition, 0, nullptr, 0, 4);
			} else {
				mWindTimeLeft = 0;
				mWindFrame    = 0;
				mOpenRate -= mCloseSpeed;
				if (mOpenRate < 0.0f)
					mOpenRate = 0.0f;
			}
			break;

		// The STATE_HIT label is retail's dead `b <default>` arm.
		case STATE_HIT:
		default:
			mOpenRate -= mIdleCloseSpeed;
			break;
		}

		if (mOpenRate > 1.0f)
			mOpenRate = 1.0f;
		if (mOpenRate < 0.0f)
			mOpenRate = 0.0f;

		J3DTevBlock* tevBlock
		    = unk78->getModel()->getModelData()->mMaterials[0]->getTevBlock();
		if (unkB8 == 1) {
			unkBA = mSwirlStep;
			mSwirlTimer--;
			if (mSwirlTimer == 0) {
				mSwirlStep++;
				mSwirlTimer = mSwirlInterval;
				if (mSwirlStep >= 8)
					mSwirlStep = 0;
			}

			SampleCtrlMaterial* ctrl = mCtrlModelData->mMaterials[0];
			u8& swirl0              = ctrl->mTevStageInfo[0].field_0x5;
			J3DTevStageInfo& info2 = ctrl->mTevStageInfo[2];
			u8& swirl2              = info2.field_0x5;
			J3DTevStageInfo& info3 = ctrl->mTevStageInfo[3];
			u8& swirl3a             = info3.field_0x5;
			u8& swirl3b             = info3.field_0x6;
			u8& swirl3c             = info3.field_0x7;
			u8& swirl3d             = info3.field_0x8;
			J3DTevStageInfo& info5 = ctrl->mTevStageInfo[5];
			u8& swirl5              = info5.field_0x11;
			swirl0  = 0;
			swirl2  = 0;
			swirl3a = 0;
			swirl3b = 0;
			swirl3c = 0;
			swirl3d = 1;
			swirl5  = 1;

			switch (mSwirlStep) {
			case 0:
				break;
			case 1:
				swirl3a = 1;
				swirl3d = 0;
				break;
			case 2:
				swirl0 = 8;
				break;
			case 3:
				swirl3a = 8;
				break;
			case 4:
				swirl3c = 1;
				break;
			case 5:
				swirl5 = 0;
				break;
			case 6:
				swirl2 = 1;
				break;
			case 7:
				swirl3a = 0;
				swirl3b = 1;
				swirl3d = 0;
				break;
			}

			J3DTevStageInfo& info0 = ctrl->mTevStageInfo[0];
			tevBlock->getTevStage(0)->setTevStageInfo(info0);
			tevBlock->getTevStage(2)->setTevStageInfo(
			    info2);
			tevBlock->getTevStage(3)->setTevStageInfo(
			    info3);
			tevBlock->getTevStage(5)->setTevStageInfo(
			    info5);
		}

		f32 frame = mOpenRate * (f32)unk78->getFrameCtrl(5)->getEnd();
		unk78->getFrameCtrl(5)->setFrame(frame);
	}

	if (cue & CUE_CALC_VIEW)
		screenBlur(graphics);

	THitActor::perform(cue, graphics);
}
