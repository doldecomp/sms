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

	char path[256];
	snprintf(path, 256, "/scene/map/map/gate/%s.bmd",
	         gateMActorNames[unk71]);
	unk78 = SMS_MakeMActor("/scene/map/map/gate", path, 0, 0x11100000);
	unk72
	    = unk78->getModel()->getModelData()->unkB0->getIndex("center");

	// The gate's window shows the THP movie, so its three YUV planes have to
	// be resized to whatever is playing.
	if (ActivePlayer.open) {
		THPVideoInfo videoInfo;
		THPPlayerGetVideoInfo(&videoInfo);
		u16 width       = videoInfo.xSize;
		u16 height      = videoInfo.ySize;
		// TODO: retail's halves are `extrwi rD, rS, 15, 16`, i.e.
		// `(x >> 1) & 0x7fff` -- fifteen bits, so it knew the dividend
		// was already 16-bit -- while `videoInfo.xSize / 2` on the u32
		// field gives `extrwi rD, rS, 16, 15`. Deriving them from the
		// `width`/`height` u16 locals is not it either (556 instructions
		// against retail's 548: MWCC re-materialises the truncation).
		u16 halfX       = videoInfo.xSize / 2;
		u16 halfY       = videoInfo.ySize / 2;
		J3DTexture* tex = unk78->getModel()->getModelData()->unkAC;
		tex->mResources[0].format = 1;
		tex->mResources[0].width  = width;
		tex->mResources[0].height = height;
		tex->mResources[1].format = 1;
		tex->mResources[1].width  = halfX;
		tex->mResources[1].height = halfY;
		tex->mResources[2].format = 1;
		tex->mResources[2].width  = halfX;
		tex->mResources[2].height = halfY;
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
	MTXMultVec(mtx, unkAC, unkAC);

	MTXInverse(unk78->getModel()->getAnmMtx(unk72), mInvCenterMtx);
	MtxPtr center = unk78->getModel()->getAnmMtx(unk72);
	mCenterYaw    = matan(center[2][0], center[0][0]);

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

void TModelGate::screenBlur(JDrama::TGraphics* graphics)
{
	// Direction from the gate to Mario, in view space, so the radial blur
	// streaks towards the gate on screen.
	JGeometry::TVec3<f32> viewDir;
	JGeometry::TVec3<f32> toMario;
	toMario.x = gpMarioPos->x - mPosition.x;
	toMario.y = 0.0f;
	toMario.z = gpMarioPos->z - mPosition.z;
	VECNormalize(toMario, toMario);
	MTXMultVecSR(graphics->mViewMtx, toMario, viewDir);

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

	// TODO (shared header, ScreenUtil.hpp): read off retail's tail, the last
	// three fields are one `JGeometry::TVec3<f32>` member at 0x5C, not three
	// f32s. Retail computes the alpha and the radius first, copies `viewDir`
	// into a fifth 12-byte stack vector with interleaved lfs/stfs (i.e.
	// `dir.set(viewDir)`), loads gpAfterEffect only then, and writes the
	// member with three integer `lwz`/`stw` (i.e. a `TVec3` assignment).
	// That accounts for 12 of the function's 48-byte frame gap; another 8 are
	// a second `stfd` slot for the `(s16)(182.04445f * mRotation.y - ...)`
	// conversion, which retail materialises twice from one `fctiwz`, and the
	// remaining 28 are dead low region. A by-value TU-local setter was tried
	// and is worse (157 -> 161 instructions); the honest fix is the TVec3
	// member plus a setter on TAfterEffect.
	gpAfterEffect->unk15 = 2;
	gpAfterEffect->unk1C = (u8)(mBlurAlpha * (1.0f - gpCamera->unk270));
	gpAfterEffect->unk50 = mBlurRadius;
	gpAfterEffect->unk5C = viewDir;
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
// Two thin levels, a stand-in for unreconstructed inline structure inside
// perform (its frame is still 232 bytes short of retail's 0x220, so real
// levels are missing): retail reaches JGeometry::TUtil<f32>::sqrt as a `bl`
// here, which needs depth 4, and through length() alone the site is at depth
// 2. Same idiom as the WrapDirection/WrapRadian pair in wireTrap and
// MapObjCorona. 699 -> 689 instructions against retail's 682.
static inline f32 ModelGateLengthInner(const JGeometry::TVec3<f32>& v)
{
	return v.length();
}

static inline f32 ModelGateLength(const JGeometry::TVec3<f32>& v)
{
	return ModelGateLengthInner(v);
}

// TODO: 232 bytes of frame short (0x138 against retail's 0x220) and retail
// saves r23-r31 where we save r26-r31, so several inlined helpers are still
// missing. @3054 is an 8-entry jump table whose grouping already matches and
// whose addends are gated on this function's size (ours 0x698/0x650/... against
// retail's 0x694/0x64c/...), so the unit's data score cannot reach 100% until
// perform does.
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
			JGeometry::TVec3<f32> toMario(*gpMarioPos);
			toMario.x -= mPosition.x;
			toMario.y -= mPosition.y;
			toMario.z -= mPosition.z;
			JGeometry::TVec3<f32> dist(toMario);
			if (ModelGateLength(dist) < 1000.0f) {
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

		default:
			mOpenRate -= mIdleCloseSpeed;
			break;
		}

		if (mOpenRate > 1.0f)
			mOpenRate = 1.0f;
		if (mOpenRate < 0.0f)
			mOpenRate = 0.0f;

		J3DMaterial* material
		    = unk78->getModel()->getModelData()->mMaterials[0];
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
			ctrl->mTevStageInfo[0].field_0x5  = 0;
			ctrl->mTevStageInfo[2].field_0x5  = 0;
			ctrl->mTevStageInfo[3].field_0x5  = 0;
			ctrl->mTevStageInfo[3].field_0x6  = 0;
			ctrl->mTevStageInfo[3].field_0x7  = 0;
			ctrl->mTevStageInfo[3].field_0x8  = 1;
			ctrl->mTevStageInfo[5].field_0x11 = 1;

			switch (mSwirlStep) {
			case 0:
				break;
			case 1:
				ctrl->mTevStageInfo[3].field_0x5 = 1;
				ctrl->mTevStageInfo[3].field_0x8 = 0;
				break;
			case 2:
				ctrl->mTevStageInfo[0].field_0x5 = 8;
				break;
			case 3:
				ctrl->mTevStageInfo[3].field_0x5 = 8;
				break;
			case 4:
				ctrl->mTevStageInfo[3].field_0x7 = 1;
				break;
			case 5:
				ctrl->mTevStageInfo[5].field_0x11 = 0;
				break;
			case 6:
				ctrl->mTevStageInfo[2].field_0x5 = 1;
				break;
			case 7:
				ctrl->mTevStageInfo[3].field_0x5 = 0;
				ctrl->mTevStageInfo[3].field_0x6 = 1;
				ctrl->mTevStageInfo[3].field_0x8 = 0;
				break;
			}

			material->getTevStage(0)->setTevStageInfo(
			    ctrl->mTevStageInfo[0]);
			material->getTevStage(2)->setTevStageInfo(
			    ctrl->mTevStageInfo[2]);
			material->getTevStage(3)->setTevStageInfo(
			    ctrl->mTevStageInfo[3]);
			material->getTevStage(5)->setTevStageInfo(
			    ctrl->mTevStageInfo[5]);
		}

		f32 frame = mOpenRate * (f32)unk78->getFrameCtrl(5)->getEnd();
		unk78->getFrameCtrl(5)->setFrame(frame);
	}

	if (cue & CUE_CALC_VIEW)
		screenBlur(graphics);

	THitActor::perform(cue, graphics);
}
