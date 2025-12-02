#include <Player/Watergun.hpp>
#include <Player/NozzleTrigger.hpp>
#include <Player/NozzleBase.hpp>
#include <Player/NozzleDeform.hpp>

#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>

#include <gpMarDirector.hpp>

#include <System/StageUtil.hpp>

#include <MarioUtil/MathUtil.hpp>
#include <JSystem/JMath.hpp>

// Define the global variable in .data section
TNozzleBmdData nozzleBmdData
    = { { { 0,                                          // _00
	        0,                                          // _04
	        nullptr,                                    // mHelmetPath
	        "/mario/watergun2/normal_wg",               // mPath
	        "/mario/watergun2/normal_wg/normal_wg.bmd", // mBmdPath
	        1,                                          // mNumEmitters
	        2,                                          // _15
	        { { 1, 0 }, { 4, 0 }, { 4, 0 } } },
	      { 0,                                          // _00
	        0,                                          // _04
	        nullptr,                                    // mHelmetPath
	        "/mario/watergun2/rocket_wg",               // mPath
	        "/mario/watergun2/rocket_wg/rocket_wg.bmd", // mBmdPath
	        1,                                          // mNumEmitters
	        2,                                          // _15
	        { { 2, 1 }, { 4, 0 }, { 4, 0 } } },
	      { 0,                                        // _00
	        0,                                        // _04
	        "/mario/bmd/wg_hel_diver.bmd",            // mHelmetPath
	        "/mario/watergun2/hover_wg",              // mPath
	        "/mario/watergun2/hover_wg/hover_wg.bmd", // mBmdPath
	        2,                                        // mNumEmitters
	        0xc,                                      // _15
	        { { 1, 0 }, { 1, 0 }, { 4, 0 } } },
	      { 0,                                        // _00
	        0,                                        // _04
	        nullptr,                                  // mHelmetPath
	        "/mario/watergun2/dummy_wg",              // mPath
	        "/mario/watergun2/dummy_wg/dummy_wg.bmd", // mBmdPath
	        1,                                        // mNumEmitters
	        2,                                        // _15
	        { { 3, 0 }, { 4, 0 }, { 4, 0 } } },
	      { 0,                                        // _00
	        0,                                        // _04
	        nullptr,                                  // mHelmetPath
	        "/mario/watergun2/hover_wg",              // mPath
	        "/mario/watergun2/hover_wg/hover_wg.bmd", // mBmdPath
	        2,                                        // mNumEmitters
	        0xc,                                      // _15
	        { { 1, 0 }, { 1, 0 }, { 4, 0 } } },
	      { 0,                                      // _00
	        0,                                      // _04
	        nullptr,                                // mHelmetPath
	        "/mario/watergun2/back_wg",             // mPath
	        "/mario/watergun2/back_wg/back_wg.bmd", // mBmdPath
	        1,                                      // mNumEmitters
	        2,                                      // _15
	        { { 1, 0 }, { 4, 0 }, { 4, 0 } } } } };

// TODO: Pretty certain these are defined in header file?
// s32 TNozzleTrigger::getNozzleKind() const { return 1; }
s32 TNozzleDeform::getNozzleKind() const { return 3; }

TWaterGun::TWaterGun(TMario* mario)
    : mNozzleDeform("normal_wg", "/Mario/WaterGun/NozzleDeform.prm", this)
    , mNozzleRocket(nullptr, "/Mario/WaterGun/NozzleTrgRocket.prm", this)
    , mNozzleUnderWater("hover_wg", "/Mario/WaterGun/NozzleDiving.prm", this)
    , mNozzleYoshiDeform("dummy_wg", "/Mario/WaterGun/NozzleYoshiMouth.prm",
                         this)
    , mNozzleHover("hover_wg", "/Mario/WaterGun/NozzleTrgHover.prm", this)
    , mNozzleTurbo("back_wg", "/Mario/WaterGun/NozzleTrgTurbo.prm", this)
    , mWatergunParams("/Mario/WaterGun.prm")
    , mMario(mario)
{
	mWatergunParams.load(mWatergunParams.mPrmPath);
	mMario = mario;
}

static bool NozzleCtrl(J3DNode* node, BOOL param_2)
{
	// TODO: Inlined stack space
	if (!param_2) {
		if (gpMarioForCallBack != nullptr) {
			s16 gunAngle = gpMarioForCallBack->mWaterGun
			                   ->mNozzleList[gpMarioForCallBack->mWaterGun
			                                     ->mCurrentNozzle]
			                   ->getGunAngle();
			if (gunAngle < 0) {
				Mtx mtx;
				// Unused stack space
				// volatile u32 unused2[6];
				MsMtxSetRotRPH(mtx, 0.0f, 0.0f, 0.005493164f * gunAngle);
				PSMTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
			}
		}
	}
	return 1;
}

static bool RotateCtrl(J3DNode* node, BOOL param_2)
{
	if (!param_2 && gpMarioForCallBack != nullptr) {
		s16 local1cd0 = gpMarioForCallBack->mWaterGun->unk1CD0;
		Mtx mtx;
		// Unused stack space
		// volatile u32 unused2[7];
		MsMtxSetRotRPH(mtx, 0.005493164f * local1cd0, 0.0f, 0.0f);
		PSMTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
	}
	return 1;
}

static bool WaterGunDivingCtrlL(J3DNode* node, BOOL param_2)
{
	if (!param_2) {
		// This looks very weird to me, probably because of some inline?
		// I could imagine some s32 getNozzleSpeedY() and
		// s16 localXXX = -getNozzleSpeedY();
		s32 nozzleSpeedY = gpMarioForCallBack->mWaterGun->mNozzleSpeedY;
		s16 neg          = -nozzleSpeedY;
		Mtx mtx;
		// Unused stack space
		// volatile u32 unused2[7];
		MsMtxSetRotRPH(mtx, 0.0f, 0.0f, 0.005493164f * neg);
		PSMTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
	}
	return 1;
}

static bool WaterGunDivingCtrlR(J3DNode* node, BOOL param_2)
{
	if (!param_2) {
		// This looks very weird to me, probably because of some inline?
		// I could imagine some s32 getNozzleSpeedY() and
		// s16 localXXX = -getNozzleSpeedY();
		s32 nozzleSpeedY = gpMarioForCallBack->mWaterGun->mNozzleSpeedZ;
		s16 neg          = -nozzleSpeedY;
		Mtx mtx;
		// Unused stack space
		// volatile u32 unused2[7];
		MsMtxSetRotRPH(mtx, 0.0f, 0.0f, 0.005493164f * neg);
		PSMTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
	}
	return 1;
}

// TODO: Nozzle deform

void TWaterGun::init()
{
	mFlags                       = 0;
	mNozzleList[Spray]           = &mNozzleDeform;
	mNozzleList[Rocket]          = &mNozzleRocket;
	mNozzleList[Underwater]      = &mNozzleUnderWater;
	mNozzleList[Yoshi]           = &mNozzleYoshiDeform;
	mNozzleList[Hover]           = &mNozzleHover;
	mNozzleList[Turbo]           = &mNozzleTurbo;
	mCurrentNozzle               = Spray;
	mSecondNozzle                = Hover;
	mNozzleRocket.mSoundID       = 0x81b;
	mNozzleTurbo.mSoundID        = 0x814;
	mNozzleDeform.mBomb.mSoundID = 0x813;
	mCurrentWater = mNozzleList[mCurrentNozzle]->mEmitParams.mAmountMax.get();
	mIsEmitWater  = false;
	unk1C88       = 0.0f;
	mCurrentPressure  = 0;
	mPreviousPressure = 0;
	unk1CEC           = 1.0f;
	unk1CF0           = 0.1f;
	unk1CF4           = 0.0049999999f;
	unk1CF8           = 0x168;
	unk1CFA           = 0;
	mFluddSwitchTween = 0.0f;
	unk1D00           = 0.0f;
	unk1D04           = 0;
	unk1D06           = -0x1800;
	unk1D08           = 0;

	// mEmitInfo = new TWaterEmitInfo; TODO

	unk1D08                                  = 0;
	mNozzleDeform.mBomb.mRumbleOnCharge      = true;
	mNozzleYoshiDeform.mBomb.mRumbleOnCharge = true;

	mEmitPos[3].x = mMario->mPosition.x;
	mEmitPos[3].y = mMario->mPosition.y;
	mEmitPos[3].z = mMario->mPosition.z;

	unk1CC0 = 0;
	unk1CC2 = 0;
	unk1CC4 = 0;

	mNozzleSpeedY = 0.0f;
	unk1CD0       = 0;
	unk1CD2       = 0;

	// This is definitely an inlined funciton. Creating a model seems quite
	// useful
	// TODO: Check if already exists
	MActorAnmData* watergunAnmData = new MActorAnmData();
	watergunAnmData->init("/mario/watergun2/body", nullptr);
	mFluddModel = new MActor(watergunAnmData);

	void* fluddModelData
	    = JKRFileLoader::getGlbResource("/mario/watergun2/body/wg_mdl1.bmd");
	J3DModel* fluddModel = new J3DModel(
	    J3DModelLoaderDataBase::load(fluddModelData, 0x10040000), 0, 1);
	mFluddModel->setModel(fluddModel, 0);

	// Requires M3UModelMario at 0x3a8 in TMario
	// Missing decompilation
	// PSMTXCopy(
	//     mMario->mModelMario->unk8->mJointArray[mMario->mBindBoneIDArray[0]],
	//     mFluddModel->unk4->mBaseMtx);

	mFluddModel->unk4->initialize();

	s32 handleIdx
	    = mFluddModel->unk4->mModelData->unkB0->getIndex("jnt_G_handle");

	// unk1CDC is TMultiMtxEffect, but confused from implementation atm

	mCurFluddTransformIdx
	    = mFluddModel->unk4->mModelData->unkB0->getIndex("nozzle_center");

	for (int i = 0; i < 6; ++i) {
		if (!nozzleBmdData.getPath(i)) {
			mNozzleList[i]->mActor = nullptr;
			continue;
		}

		MActorAnmData* nozzleData = new MActorAnmData();
		nozzleData->init(nozzleBmdData.getPath(i), nullptr);
		mNozzleList[i]->mActor = new MActor(nozzleData);

		void* nozzleModelData
		    = JKRFileLoader::getGlbResource(nozzleBmdData.getBmdPath(i));
		J3DModel* nozzleModel = new J3DModel(
		    J3DModelLoaderDataBase::load(nozzleModelData, 0x10040000), 0, 1);
		mNozzleList[i]->mActor->setModel(nozzleModel, 0);

		// TODO: Figure out
		// ResTIMG* img = mFluddModel->unk4->mModelData->unkAC->getResTIMG(0);
		// SMS_ChangeTextureAll(mNozzleList[i]->mActor->unk4->mModelData,
		//                      "H_watergun_main_dummy", img);

		mNozzleList[i]->mActor->initDL();
		// Definitely inline potential
		if (nozzleBmdData.getFlags(i, 0) != 4) {
			s32 jointIdx
			    = mNozzleList[i]->mActor->unk4->mModelData->unkB0->getIndex(
			        "null_G_muzzle");
			nozzleBmdData.setJointIndex(i, 0, jointIdx);
		}
		if (nozzleBmdData.getFlags(i, 1) != 4) {
			s32 jointIdx
			    = mNozzleList[i]->mActor->unk4->mModelData->unkB0->getIndex(
			        "null_G_muzzle2");
			nozzleBmdData.setJointIndex(i, 1, jointIdx);
		}
		if (nozzleBmdData.getFlags(i, 2) != 4) {
			s32 jointIdx
			    = mNozzleList[i]->mActor->unk4->mModelData->unkB0->getIndex(
			        "null_G_muzzle3");
			nozzleBmdData.setJointIndex(i, 2, jointIdx);
		}
	}

	s32 jointIdx
	    = mNozzleList[Spray]->mActor->unk4->mModelData->unkB0->getIndex(
	        "chn_muzzle_l");
	// mNozzleList[Spray]
	//     ->mActor->unk4->mModelData->mJointNodePointer[jointIdx]
	//     ->mCallBack = functionPtr;
	// 5 more of these

	// PSMTXCopy that requires M3UModelMario at 0x3a8 in TMario

	unk1D10 = new TMirrorActor("水鉄砲in鏡");
	unk1D10->init(mFluddModel->unk4, 4);

	// TODO: Definitely an inlined function
	// Another function does the exact same thing
	for (int i = 0; i < nozzleBmdData.getEmitterCount(mCurrentNozzle); ++i) {
		MtxPtr emitMtx = getEmitMtx(i);
		if (emitMtx != nullptr) {
			mEmitPos[i].x = emitMtx[0][3];
			mEmitPos[i].y = emitMtx[1][3];
			mEmitPos[i].z = emitMtx[2][3];
		}
	}

	// TODO: Continue
}

void TWaterGun::calcAnimation(JDrama::TGraphics* graphics)
{
	// TODO: Inlined stack space
	volatile u32 unused2[12];

	gpMarioForCallBack      = mMario;
	J3DFrameCtrl* frameCtrl = mFluddModel->getFrameCtrl(MActor::BCK);
	if (mMario == nullptr) {
		return;
	}

	s32 var380 = mMario->unk380;
	if ((var380 & 0x8000) != 0) {
		var380 = 0;
	}

	// Definitely wrong, possibly a swtich statement, but couldn't make that
	// work either
	if (var380 == 5) {
		// Unused
	} else if (var380 >= 5) {
		// Unused
	} else if (2 <= var380) {
	} else {
		if (var380 >= 0) {
		} else {
			if (unk1CEC == 0.0f) {
				if (mMario->fabricatedActionInline()) {
					mFluddModel->setBck("wg_fepmp");
				} else if (mMario->hasAttribute(ATTR_IS_SHALLOW_WATER
				                                | ATTR_IS_WATER)) {
					mFluddModel->setBck("wg_swpmp");
				} else {
					// TODO: Cast would be weird here, probably an inlined
					// getter that converts to s32
					if ((s32)mMario->unk0FA != 0x33) {
						mFluddModel->setBck("wg_hgpmp");
					} else {
						mFluddModel->setBck("wg_pump");
					}
				}
				frameCtrl->setSpeed(0.0f);
				frameCtrl->setFrame(mMario->getPumpFrame());
				unk1CFA = unk1CF8;
			} else {
				mFluddModel->setBck("wg_house");
				if ((0.0f < unk1CEC)
				    && (unk1CEC = unk1CEC - 0.1f, unk1CEC <= 0.0f)) {
					unk1CEC = 0.0f;
				}
				frameCtrl->setSpeed(0.0f);
				frameCtrl->setFrame(unk1CEC * frameCtrl->getEndFrame());
			}
			return;
		}
	}

	if (unk1CFA == 0) {
		if (unk1CEC < 1.0f) {
			unk1CEC = unk1CEC + unk1CF4;
			mFluddModel->setBck("wg_house");
			frameCtrl->setSpeed(0.0f);
			frameCtrl->setFrame(unk1CEC * frameCtrl->getEndFrame());
		} else {
			unk1CEC = 1.0f;
			mFluddModel->setBck("wg_house");
			frameCtrl->setSpeed(0.0f);
			frameCtrl->setFrame(unk1CEC * frameCtrl->getEndFrame());
		}
	} else {
		unk1CFA = unk1CFA - 1;
	}
}

// TODO: Do i really need to explcitly say this?
#pragma dont_inline on
MtxPtr TWaterGun::getEmitMtx(int jointIndex)
{
	volatile u32 unused2[24]; // TODO: A lot of stack space, possibly a lot of
	                          // inlined functions.
	MtxPtr result;
	if (!this->mMario->onYoshi()) {
		result = this->getYoshiMtx();
	} else {
		// This entire block is likely an inlined function.
		u8 currentNozzle = this->mCurrentNozzle;
		s8 flag          = nozzleBmdData.getFlags(currentNozzle, jointIndex);
		if (flag < 3) {
			return this->getCurrentNozzle()->mActor->unk4->getAnmMtx(
			    nozzleBmdData.getJointIndex(this->mCurrentNozzle, jointIndex));
		} else if (flag == 3) {
			return getYoshiMtx();
		}
	}
	return result;
}
#pragma dont_inline off

MtxPtr TWaterGun::getNozzleMtx()
{
	return this->mFluddModel->unk4->getAnmMtx(this->mCurFluddTransformIdx);
}

void TWaterGun::initInLoadAfter() { }

bool TWaterGun::isEmitting() { return false; }

void TWaterGun::changeNozzle(TNozzleType nozzleType, bool animate)
{
	f32 usedWater = mCurrentWater
	                / mNozzleList[mCurrentNozzle]->mEmitParams.mAmountMax.get();
	if (nozzleType == Spray) {
		if (animate) {
			mFluddSwitchTween = 0.0f;
		}
	} else {
		mSecondNozzle = nozzleType;
		if (animate) {
			mFluddSwitchTween = 1.0f;
		}
	}
	mCurrentNozzle = nozzleType;
	mNozzleList[mCurrentNozzle]->init();
	if (nozzleType == Yoshi) {
		mCurrentWater = mMario->mYoshi->_11[0];
	} else {
		mCurrentWater
		    = usedWater
		      * mNozzleList[mCurrentNozzle]->mEmitParams.mAmountMax.get();
	}
}

void TWaterGun::movement()
{
	volatile u32 unused2[69]; // TODO: A lot of stack space, possibly a lot of

	bool canSpray; // Not sure if this is correct variable name
	if (mCurrentWater == 0) {
		canSpray = false;
	} else {
		s32 kind = getCurrentNozzle()->getNozzleKind();
		if (kind == 1) {
			TNozzleTrigger* triggerNozzle
			    = (TNozzleTrigger*)mNozzleList[mCurrentNozzle];
			if (triggerNozzle->mSprayState == TNozzleTrigger::ACTIVE) {
				canSpray = true;
			} else {
				canSpray = false;
			}
		} else if (getCurrentNozzle()->unk378 > 0.0f) {
			canSpray = true;
		} else {
			canSpray = false;
		}
	}

	if (!canSpray) {
		unk1CC2 = 0;
		unk1CC4 = 0;
	}

	mNozzleSpeedY
	    += (unk1CC2 - mNozzleSpeedY) * mWatergunParams.mChangeSpeed.get();
	mNozzleSpeedZ
	    += (unk1CC4 - mNozzleSpeedZ) * mWatergunParams.mChangeSpeed.get();

	TNozzleBase* currentNozzle = this->getCurrentNozzle();

	rotateProp(currentNozzle->unk378);

	// They do the same thing again?... This is the exact same code as
	// rotateProp
	if (mCurrentNozzle == 5) {
		unk1CD2 += mNozzleList[mCurrentNozzle]->unk378
		           * mWatergunParams.mNozzleAngleYSpeed.get();
		unk1CD2 *= mWatergunParams.mNozzleAngleYBrake.get();
		if (mWatergunParams.mHoverRotMax.get() < unk1CD2) {
			unk1CD2 = mWatergunParams.mHoverRotMax.get();
		}
		unk1CD0 = unk1CD0 + unk1CD2;
	} else {
		unk1CD2 = 0;
		unk1CD0 = 0;
	}

	// Yoshi nozzle
	if (mCurrentNozzle == 3) {
		mCurrentWater = getCurrentNozzle()->mEmitParams.mAmountMax.get();
	}

	if (SMS_isDivingMap()) {
		mCurrentWater = getCurrentNozzle()->mEmitParams.mAmountMax.get();
	}

	if (mCurrentNozzle == 3) {
		unk1CEC = 0.0f;
	}

	// Nozzle swapping
	if (unk1D00 != 0.0f) {
		f32 unk           = mFluddSwitchTween;
		f32 sum           = unk + unk1D00;
		mFluddSwitchTween = sum;
		if ((unk < 0.5f) && (0.5f <= sum)) {
			u8 curNozzle        = mCurrentNozzle;
			s32 currentWater    = mCurrentWater;
			u8 secondNozzle     = mSecondNozzle;
			f32 maxWater        = currentNozzle->mEmitParams.mAmountMax.get();
			f32 waterPercentage = currentWater / maxWater;

			if (secondNozzle != 0) {
				mSecondNozzle = secondNozzle;
			}
			mCurrentNozzle = secondNozzle;

			currentNozzle = this->getCurrentNozzle();
			currentNozzle->init(); // TODO: 2 vtable entry

			if (secondNozzle == 3) {
				mCurrentWater
				    = mMario->mYoshi->_11[0]; // TODO: Proper yoshi stuff
			} else {
				mCurrentWater = waterPercentage
				                * currentNozzle->mEmitParams.mAmountMax.get();
			}
		}
		if ((sum < 0.5f) && (0.5f <= unk)) {
			f32 currentWater    = (f32)mCurrentWater;
			f32 maxWater        = currentNozzle->mEmitParams.mAmountMax.get();
			f32 waterPercentage = currentWater / maxWater;

			mCurrentNozzle = 0;

			currentNozzle = this->getCurrentNozzle();
			currentNozzle->init(); // TODO: 2 vtable entry

			mCurrentWater
			    = waterPercentage * currentNozzle->mEmitParams.mAmountMax.get();
		}

		if (mFluddSwitchTween < 0.0) {
			mFluddSwitchTween = 0.0f;
			unk1D00           = 0.0f;
		}
		if (1.0f < mFluddSwitchTween) {
			mFluddSwitchTween = 1.0f;
			unk1D00           = 0.0f;
		}
	}
	currentNozzle->animation(mCurrentNozzle);
}

void TNozzleBase::emitCommon(int param_1, TWaterEmitInfo* param_2)
{
	param_2->mAlive.set(
	    gpModelWaterManager->mWaterParticleTypes[mEmitParams.mType.get()]
	        ->mAlive.get());

	JGeometry::TVec3<f32> pos;
	JGeometry::TVec3<f32> dir;
	JGeometry::TVec3<f32> speed;
	mFludd->getEmitPosDirSpeed(param_1, &pos, &dir, &speed);

	// TODO: This feels wrong
	// TODO: Fix asm
	param_2->mPos.value = pos;
	param_2->mV.value   = speed;
	param_2->mDir.value = dir;

	param_2->mDirTremble  = mEmitParams.mDirTremble;
	param_2->mPowTremble  = mEmitParams.mPowTremble;
	param_2->mSize        = mEmitParams.mSize;
	param_2->mSizeTremble = mEmitParams.mSizeTremble;
	param_2->mType        = mEmitParams.mType;
	param_2->mHitRadius   = mEmitParams.mHitRadius;
	param_2->mHitHeight   = mEmitParams.mHitHeight;
}

void TNozzleBase::emit(int param_1)
{
	if (mFludd->mCurrentWater > 0 && unk378 != 0.0f) {
		TWaterEmitInfo* emitInfo = mFludd->mEmitInfo;
		this->emitCommon(param_1, emitInfo);

		unk37C          = unk37C + mEmitParams.mNum.get();
		s32 local37cInt = (s32)unk37C;
		if (local37cInt != 0) {
			unk37C = unk37C - (f32)local37cInt;
			emitInfo->mNum.set(local37cInt);
			emitInfo->mAttack = mEmitParams.mAttack;
			f32 emitCtrl      = mEmitParams.mEmitCtrl.get();
			f32 emitPow       = mEmitParams.mEmitPow.get();
			emitInfo->mPow.set(emitCtrl * emitPow * unk378
			                   + emitPow * (1.0f - emitCtrl));
			emitInfo->mFlag.set(0x40);
			u16 flags = mFludd->mFlags;
			u32 flagResult;
			if ((flags & 2) != 0) {
				flagResult = 1;
			} else {
				flagResult = 0;
			}
			if (flagResult != 0) {
				emitInfo->mFlag.set(emitInfo->mFlag.get() | 0x80);
			}

			int emittedWater     = gpModelWaterManager->emitRequest(*emitInfo);
			u32 emittedWaterU32  = (u32)emittedWater & 0xFF;
			mFludd->mIsEmitWater = (u8)emittedWater;
			f32* unk1C88Ptr      = (f32*)&mFludd->unk1C88;
			f32 unk1C88          = *unk1C88Ptr;
			u8 currentNozzle     = mFludd->mCurrentNozzle;
			TNozzleBase** nozzleList      = mFludd->mNozzleList;
			TNozzleBase* currentNozzlePtr = nozzleList[currentNozzle];
			u32 unk1C88U32                = (u32)mFludd->unk1C88;
			s16 decRate       = currentNozzlePtr->mEmitParams.mDecRate.get();
			f32 emittedWaterF = (f32)emittedWaterU32;
			f32 decRateF      = (f32)decRate;
			f32 unk1C88OldF   = (f32)unk1C88U32;
			f32 temp          = emittedWaterF * decRateF;
			f32 temp2         = temp / unk1C88OldF;
			*unk1C88Ptr       = 10.0f * temp2 + unk1C88;
			if (emittedWaterU32 == 0) {
				goto skip_velocity;
			}
			mFludd->mCurrentWater
			    -= emittedWaterU32 * mEmitParams.mDecRate.get();
			if (mFludd->mCurrentWater < 0) {
				mFludd->mCurrentWater = 0;
			}

			f32* powPtr                   = &emitInfo->mPow.value;
			JGeometry::TVec3<f32>* dirPtr = &emitInfo->mDir.value;
			f32 powVal                    = *powPtr;
			s16 faceAngleY                = mFludd->mMario->mFaceAngle.y;
			f32 cosAngle                  = JMASCos(faceAngleY);
			f32 sinAngle                  = JMASSin(faceAngleY);
			f32 dirX                      = -dirPtr->x;
			f32 dirZ                      = dirPtr->z;
			f32 dirY                      = dirPtr->y;
			f32 reactionPow = powVal * mEmitParams.mReactionPow.get();
			f32 reactionY   = mEmitParams.mReactionY.get();
			f32 unkE0       = mEmitParams.mReactionPow.value;
			f32 unkF4       = mEmitParams.mReactionY.value;
			f32 f31         = powVal * unkE0;

			mFludd->mMario->addVelocity((dirX * sinAngle - dirZ * cosAngle)
			                            * reactionPow);

			f32* velX = &mFludd->mMario->mVel.x;
			*velX     = -dirPtr->x * reactionPow - *velX;
			f32* velZ = &mFludd->mMario->mVel.z;
			*velZ     = -dirPtr->z * reactionPow - *velZ;
			f32* velY = &mFludd->mMario->mVel.y;
			*velY     = *velY - dirY * powVal * unkF4 * reactionY;
		skip_velocity:;
		}
	}
}

void TWaterGun::setBaseTRMtx(Mtx mtx)
{
	volatile u32 unused1[10];
	f32 initialAngle = mtx[1][0];
	if (initialAngle < 0.0f) {
		initialAngle = -initialAngle;
	}

	// Seemingly some adjustment of fluddpack angle
	f32 baseAngle = unk1D06;
	f32 angleDiff = unk1D04 - unk1D06;

	s16 angle = initialAngle * angleDiff + baseAngle;

	Mtx result;
	Vec unused2;
	MsMtxSetRotRPH(result, 0.0f, 0.0f, 0.005493164f * angle);

	PSMTXConcat(mtx, result, result);
	PSMTXCopy(result, mFluddModel->unk4->mBaseMtx);
}

// Not sure why this get's inlined aggressively
#pragma dont_inline on
TNozzleBase::TNozzleBase(const char* name, const char* prm, TWaterGun* fludd)
    : mEmitParams(prm)
    , mFludd(fludd)
{
	mEmitParams.load(mEmitParams.mPrmPath);
	this->mAnimState = 2;
	this->mGunAngle  = 0;
	this->unk372     = 0;
	this->unk378     = 0.0f;
	this->unk37C     = 0.0f;
}
#pragma dont_inline off

void TNozzleBase::init()
{
	this->mAnimState = 2;
	this->mGunAngle  = 0;
	this->unk372     = 0;
	this->unk378     = 0.0f;
	this->unk37C     = 0.0f;
}

void TNozzleBase::calcGunAngle(const TMarioControllerWork& work)
{
	// volatile u32 unused1[17];
	if (mFludd->mMario == gpMarioAddress
	    && (gpCamera->isLButtonCameraSpecifyMode(gpCamera->mMode)
	        || gpCamera->isJetCoaster1stCamera())) {
		mGunAngle = gpCamera->unkA4;
		return;
	}

	s16 angle;
	if (mFludd->mMario->mAction == 0xC008220) {
		// TODO: Wrong reguster used, using r3 instead of r4
		angle = mGunAngle
		        + (s16)(mFludd->mMario->mGamePad->mCompSPos[0].y
		                * mEmitParams.mRButtonMult.get());
	} else {
		angle = -mEmitParams.mLAngleBase.get();
	}

	if (angle < mEmitParams.mLAngleMin.get()) {
		angle = mEmitParams.mLAngleMin.get();
	}

	if (angle > mEmitParams.mLAngleMax.get()) {
		angle = mEmitParams.mLAngleMax.get();
	}

	f32 diff = angle - mGunAngle;
	mGunAngle += diff * mEmitParams.mLAngleChase.get();
}

// TODO: This has a lot of inline functions, find them and update them
// properly
void TNozzleBase::animation(int param_1)
{
	if (param_1 != 2) {
		return;
	}

	if (0.0f < this->mFludd->unk1D00) {
		this->mAnimState = 4;
	}

	if (this->mFludd->unk1D00 < 0.0f) {
		this->mAnimState = 3;
	}

	switch (this->mAnimState) {
	case 0: {

		if (!this->mActor->checkCurBckFromIndex(4)) {
			this->mActor->setBckFromIndex(4);
		}
		J3DFrameCtrl* ctrl = this->mActor->getFrameCtrl(MActor::BCK);
		if (ctrl->mCurrentFrame <= (ctrl->mEndFrame - 0.1)
		    && (ctrl->mFlags & 3) == 0) {
			return;
		}
		this->mAnimState = 1;
		break;
	}
	case 1: {

		if (!this->mActor->checkCurBckFromIndex(2)) {
			this->mActor->setBckFromIndex(2);
		}

		TWaterGun* fludd     = this->mFludd;
		bool updateAnimation = false;
		if (fludd->mCurrentWater == 0) {
			updateAnimation = false;
		} else {
			u8 gameState = gpMarDirector->mGameState;
			if (gameState != 3 && gameState != 4) {
				updateAnimation = true;
				if (gameState != 1 && gameState != 2) {
					updateAnimation = true;
				}

				if (!updateAnimation) {
					updateAnimation = true;
					u32 nozzleKind = fludd->getCurrentNozzle()->getNozzleKind();
					if (nozzleKind == 1) {
						TNozzleTrigger* trigger
						    = (TNozzleTrigger*)fludd->getCurrentNozzle();
						if (trigger->mSprayState == 1) {
							updateAnimation = true;
						} else {
							updateAnimation = false;
						}

					} else if (fludd->getCurrentNozzle()->unk378 <= 0.0f) {
						updateAnimation = false;
					} else {
						updateAnimation = true;
					}
				}
			}
		}

		if (updateAnimation) {
			return;
		}

		this->mAnimState = 2;
		break;
	}
	case 2: {

		if (!this->mActor->checkCurBckFromIndex(3)) {
			this->mActor->setBckFromIndex(3);
		}
		TWaterGun* fludd     = this->mFludd;
		bool updateAnimation = false;
		if (fludd->mCurrentWater == 0) {
			updateAnimation = false;
		} else {
			u8 gameState = gpMarDirector->mGameState;
			if (gameState != 3 && gameState != 4) {
				updateAnimation = true;
				if (gameState != 1 && gameState != 2) {
					updateAnimation = true;
				}

				if (!updateAnimation) {
					updateAnimation = true;
					u32 nozzleKind = fludd->getCurrentNozzle()->getNozzleKind();
					if (nozzleKind == 1) {
						TNozzleTrigger* trigger
						    = (TNozzleTrigger*)fludd->getCurrentNozzle();
						if (trigger->mSprayState == 1) {
							updateAnimation = true;
						} else {
							updateAnimation = false;
						}

					} else if (fludd->getCurrentNozzle()->unk378 <= 0.0f) {
						updateAnimation = false;
					} else {
						updateAnimation = true;
					}
				}
			}
		}

		if (updateAnimation) {
			this->mAnimState = 0;
		}
		break;
	}

	case 3: {

		J3DFrameCtrl* ctrl = this->mActor->getFrameCtrl(MActor::BCK);
		if (!this->mActor->checkCurBckFromIndex(1)) {
			this->mActor->setBckFromIndex(1);
		}

		// Use external tween value
		ctrl->mCurrentFrame = this->mFludd->mFluddSwitchTween * ctrl->mEndFrame;
		ctrl->mSpeed        = 0.0f;
		break;
	}
	case 4: {

		J3DFrameCtrl* ctrl = this->mActor->getFrameCtrl(MActor::BCK);
		if (!this->mActor->checkCurBckFromIndex(0)) {
			this->mActor->setBckFromIndex(0);
		}

		// Use external tween value
		ctrl->mCurrentFrame = this->mFludd->mFluddSwitchTween * ctrl->mEndFrame;
		ctrl->mSpeed        = 0.0f;

		if (this->mFludd->mFluddSwitchTween < 1.0f) {
			return;
		}
		this->mAnimState = 0;
		break;
	}
	}
}

void TNozzleTrigger::init()
{
	this->mRumbleOnCharge   = false;
	this->mSprayState       = 0;
	mAnimState              = 0;
	mSprayQuarterFramesLeft = 0;
	mTriggerFill            = 0.0f;
}

void TNozzleBase::movement(const TMarioControllerWork& controllerWork)
{
	// TODO: Missing stack space
	volatile u32 unused2[2];

	if (this->mFludd->mCurrentWater <= 0) {
		return;
	}
	s32 var1 = 256.0f * controllerWork.mAnalogL * 150.0f;

	if (var1 > unk372) {
		unk378 = (var1 - unk372) * 0.000015258789f;
		unk374 = unk378;
		unk372 = unk372 + mEmitParams.mTriggerRate.get();
		if (var1 < unk372) {
			unk372 = var1;
		}
	} else {
		unk378 = 0.0f;
		unk372 = var1;
	}
	calcGunAngle(controllerWork);
}

void TWaterGun::perform(u32 flags, JDrama::TGraphics* graphics)
{
	// TODO: Missing stack space
	// volatile u32 unused2[24];

	if ((flags & 0x1) != 0) {
		if ((this->mFlags & 0x10) != 0) {
			this->mCurrentWater = 0;
		}
		this->movement();
	}

	if ((flags & 0x2) != 0) {
		this->calcAnimation(graphics);
	}

	this->mFluddModel->perform(flags, graphics);

	if ((flags & 0x2) != 0) {
		MActor* p2 = this->getCurrentNozzle()->mActor;
		if (p2 != nullptr) {
			PSMTXCopy(this->getModel()->getAnmMtx(this->mCurFluddTransformIdx),
			          p2->unk4->mBaseMtx);
		}

		for (s32 index = 0;
		     index < nozzleBmdData.getEmitterCount(this->mCurrentNozzle);
		     index++) {
			MtxPtr p1 = this->getEmitMtx(index);
			if (p1 != nullptr) {
				mEmitPos[index].x = p1[0][3];
				mEmitPos[index].y = p1[1][3];
				mEmitPos[index].z = p1[2][3];
			}
		}
	}

	if (this->getCurrentNozzle()->mActor) {
		this->getCurrentNozzle()->mActor->perform(flags, graphics);
	}
}

TNozzleBase* TWaterGun::getCurrentNozzle() const
{
	return mNozzleList[mCurrentNozzle];
}

void TWaterGun::setAmountToRate(f32 rate)
{
	// volatile u32 unused2[7]; // TODO: possibly inlined function
	if (this->mCurrentNozzle == 3) {
		TNozzleBase* currentNozzle = this->getCurrentNozzle();
		s32 amountMax       = currentNozzle->mEmitParams.mAmountMax.get();
		this->mCurrentWater = amountMax;
	} else {
		TNozzleBase* currentNozzle = this->getCurrentNozzle();
		this->mCurrentWater
		    = rate * currentNozzle->mEmitParams.mAmountMax.get();
	}
}

bool TWaterGun::isPressureOn()
{
	// volatile u32 unused2[6];
	if (this->getCurrentNozzle()->getNozzleKind() == 1) {
		TNozzleTrigger* triggerNozzle
		    = (TNozzleTrigger*)this->getCurrentNozzle();
		if (triggerNozzle->mTriggerFill > 0.0f) {
			return true;
		}
	}
	return false;
}

f32 TWaterGun::getPressure()
{
	// TODO: Missing stack space
	// volatile u32 unused2[5];
	if (this->getCurrentNozzle()->getNozzleKind() == 1) {
		TNozzleTrigger* triggerNozzle
		    = (TNozzleTrigger*)this->getCurrentNozzle();
		return triggerNozzle->mTriggerFill;
	}
	return 0.0f;
}

f32 TWaterGun::getPressureMax()
{
	// TODO: Missing stack space
	// volatile u32 unused2[6];

	if (this->getCurrentNozzle()->getNozzleKind() == 1) {
		return this->getCurrentNozzle()->mEmitParams.mInsidePressureMax.get();
	}

	return 0.0f;
}

// TODO: Figure out why inline happens
#pragma dont_inline on
void TWaterGun::getEmitPosDirSpeed(int index, JGeometry::TVec3<f32>* pos,
                                   JGeometry::TVec3<f32>* dir,
                                   JGeometry::TVec3<f32>* speed)
{
	// TODO: Fix unused stack space
	// volatile u32 unused2[6];

	MtxPtr nozzleEmitMtx = getEmitMtx(index);
	pos->set(mEmitPos[index]);

	if (nozzleEmitMtx != nullptr) {
		dir->x = nozzleEmitMtx[0][0];
		dir->y = nozzleEmitMtx[1][0];
		dir->z = nozzleEmitMtx[2][0];
	} else {
		dir->set(0.0f, 0.0f, 1.0f);
	}

	speed->x = mMario->mVel.x * 0.125f;
	speed->y = 0.0f;
	speed->z = mMario->mVel.z * 0.125f;
}
#pragma dont_inline off

void TWaterGun::rotateProp(f32 rotation)
{
	if (mCurrentNozzle == 5) {
		unk1CD2 += rotation * mWatergunParams.mNozzleAngleYSpeed.get();
		unk1CD2 *= mWatergunParams.mNozzleAngleYBrake.get();
		if (mWatergunParams.mHoverRotMax.get() < unk1CD2) {
			unk1CD2 = mWatergunParams.mHoverRotMax.get();
		}
		unk1CD0 = unk1CD0 + unk1CD2;
	} else {
		unk1CD2 = 0;
		unk1CD0 = 0;
	}
}

void TWaterGun::triggerPressureMovement(
    const TMarioControllerWork& controllerWork)
{
	mCurrentPressure = controllerWork.mAnalogL * 150.0f;

	TNozzleBase* currentNozzle = getCurrentNozzle();
	currentNozzle->movement(controllerWork);

	if (mCurrentPressure > mPreviousPressure) {
		mPreviousPressure = mCurrentPressure;
	} else if (mPreviousPressure != 0) {
		mPreviousPressure -= 1;
	} else {
		mPreviousPressure = 0;
	}
}
void TWaterGun::emit()
{
	// TODO: Missing stack space
	// volatile u32 unused1[25];

	// TODO: Another possible inline to check if emit is possible
	if (!mMario->hasAttribute(ATTR_HAS_HELMET_FLW_CAMERA)
	    && mMario->hasAttribute(ATTR_IS_SHALLOW_WATER | ATTR_IS_WATER)) {
		// I can imagine this also being an inline function that checks
		// if the emit point is below the water height, but i will leave
		// it for now. TODO.
		MtxPtr nozzleEmitMtx;
		if ((nozzleEmitMtx = getEmitMtx(0)) != nullptr) {
			if (nozzleEmitMtx[1][3] < mMario->mFloorPosition.z + 20.0f) {
				return;
			}
		}
	}

	if (!mMario->onYoshi()) {
		if (this->unk1CEC > 0.0f) {
			return;
		}
	}

	// TODO: Probably an enum
	// TODO: Probably inline function?
	if (this->hasFlag(0x4)) {
		this->mFlags &= ~0x4;
		return;
	}

	u8 currentNozzleType       = mCurrentNozzle;
	TNozzleBase* currentNozzle = getNozzle(currentNozzleType);
	for (int i = 0; i < nozzleBmdData.getEmitterCount(currentNozzleType); ++i) {
		currentNozzle->emit(i);
	}
	if (mCurrentWater > 0) {
		switch (currentNozzleType) {
		case Spray: {
			playSprayNozzleSound(0x24, getCurrentNozzle()->unk374);
		}
		case Yoshi:
		case Turbo: {
			playSprayNozzleSound(0x0, getCurrentNozzle()->unk378);
			break;
		}
		case Underwater: {
			playSound2(0x18);
		} break;
		case Rocket:
			break;
		case Hover:
			if (mIsEmitWater) {
				playSound2(0x18);
			}
			break;
		}
	}
}
bool TWaterGun::suck()
{
	// TODO: Missing stack space
	// volatile u32 unused1[7];
	if (this->isYoshiNozzle()) {
		return false;
	} else {
		s32 suckRate = this->getSuckRate();
		if (suckRate > 0) {
			this->incWater(suckRate);
			return true;
		}
	}
	return false;
}

bool TWaterGun::damage()
{
	if (this->hasWater()) {
		TNozzleBase* nozzle = this->getCurrentNozzle();

		this->mCurrentWater -= nozzle->mEmitParams.mDamageLoss.value;

		if (this->mCurrentWater < 0) {
			this->mCurrentWater = 0;
		}
		return true;
	}
	return false;
}

void TWaterGun::changeBackup()
{
	// volatile u32 unused2[5]; // TODO: Missing stack space
	if (this->mFluddSwitchTween == 0.0f) {
		if (gpMSound->gateCheck(0x812)) {
			MSoundSESystem::MSoundSE::startSoundSystemSE(0x812, 0, nullptr, 0);
		}
		this->unk1D00 = mWatergunParams.mChangeSpeed.get();
	}

	if (this->mFluddSwitchTween == 1.0f) {
		if (gpMSound->gateCheck(0x811)) {
			MSoundSESystem::MSoundSE::startSoundSystemSE(0x811, 0, nullptr, 0);
		}
		this->unk1D00 = -mWatergunParams.mChangeSpeed.get();
	}
}
