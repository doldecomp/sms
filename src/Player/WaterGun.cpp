#include <Player/WaterGun.hpp>
#include <Player/NozzleTrigger.hpp>
#include <Player/NozzleBase.hpp>
#include <Player/NozzleDeform.hpp>

#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>

#include <System/MarDirector.hpp>

#include <System/StageUtil.hpp>

#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <JSystem/JMath.hpp>

extern size_t gpMarioAddress;

TNozzleBmdData nozzleBmdData = {
	{
	    {
	        0,                                          // _00
	        0,                                          // _04
	        nullptr,                                    // mHelmetPath
	        "/mario/watergun2/normal_wg",               // mPath
	        "/mario/watergun2/normal_wg/normal_wg.bmd", // mBmdPath
	        1,                                          // mNumEmitters
	        2,                                          // _15
	        {
	            { 1, 0 },
	            { 4, 0 },
	            { 4, 0 },
	        },
	    },
	    {
	        0,                                          // _00
	        0,                                          // _04
	        nullptr,                                    // mHelmetPath
	        "/mario/watergun2/rocket_wg",               // mPath
	        "/mario/watergun2/rocket_wg/rocket_wg.bmd", // mBmdPath
	        1,                                          // mNumEmitters
	        2,                                          // _15
	        {
	            { 2, 1 },
	            { 4, 0 },
	            { 4, 0 },
	        },
	    },
	    {
	        0,                                        // _00
	        0,                                        // _04
	        "/mario/bmd/wg_hel_diver.bmd",            // mHelmetPath
	        "/mario/watergun2/hover_wg",              // mPath
	        "/mario/watergun2/hover_wg/hover_wg.bmd", // mBmdPath
	        2,                                        // mNumEmitters
	        0xc,                                      // _15
	        {
	            { 1, 0 },
	            { 1, 0 },
	            { 4, 0 },
	        },
	    },
	    {
	        0,                                        // _00
	        0,                                        // _04
	        nullptr,                                  // mHelmetPath
	        "/mario/watergun2/dummy_wg",              // mPath
	        "/mario/watergun2/dummy_wg/dummy_wg.bmd", // mBmdPath
	        1,                                        // mNumEmitters
	        2,                                        // _15
	        {
	            { 3, 0 },
	            { 4, 0 },
	            { 4, 0 },
	        },
	    },
	    {
	        0,                                        // _00
	        0,                                        // _04
	        nullptr,                                  // mHelmetPath
	        "/mario/watergun2/hover_wg",              // mPath
	        "/mario/watergun2/hover_wg/hover_wg.bmd", // mBmdPath
	        2,                                        // mNumEmitters
	        0xc,                                      // _15
	        {
	            { 1, 0 },
	            { 1, 0 },
	            { 4, 0 },
	        },
	    },
	    {
	        0,                                      // _00
	        0,                                      // _04
	        nullptr,                                // mHelmetPath
	        "/mario/watergun2/back_wg",             // mPath
	        "/mario/watergun2/back_wg/back_wg.bmd", // mBmdPath
	        1,                                      // mNumEmitters
	        2,                                      // _15
	        {
	            { 1, 0 },
	            { 4, 0 },
	            { 4, 0 },
	        },
	    },
	},
};

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
				MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
			}
		}
	}
	return true;
}

static bool RotateCtrl(J3DNode* node, BOOL param_2)
{
	if (!param_2 && gpMarioForCallBack != nullptr) {
		s16 local1cd0 = gpMarioForCallBack->mWaterGun->unk1CD0;
		Mtx mtx;
		// Unused stack space
		// volatile u32 unused2[7];
		MsMtxSetRotRPH(mtx, 0.005493164f * local1cd0, 0.0f, 0.0f);
		MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
	}
	return true;
}

static bool WaterGunDivingCtrlL(J3DNode* node, BOOL param_2)
{
	if (!param_2) {
		// This looks very weird to me, probably because of some inline?
		// I could imagine some s32 getNozzleSpeedY() and
		// s16 localXXX = -getNozzleSpeedY();
		s32 nozzleSpeedY = gpMarioForCallBack->mWaterGun->unk1CC8;
		s16 neg          = -nozzleSpeedY;
		Mtx mtx;
		// Unused stack space
		// volatile u32 unused2[7];
		MsMtxSetRotRPH(mtx, 0.0f, 0.0f, 0.005493164f * neg);
		MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
	}
	return true;
}

static bool WaterGunDivingCtrlR(J3DNode* node, BOOL param_2)
{
	if (!param_2) {
		// This looks very weird to me, probably because of some inline?
		// I could imagine some s32 getNozzleSpeedY() and
		// s16 localXXX = -getNozzleSpeedY();
		s32 nozzleSpeedY = gpMarioForCallBack->mWaterGun->unk1CCC;
		s16 neg          = -nozzleSpeedY;
		Mtx mtx;
		// Unused stack space
		// volatile u32 unused2[7];
		MsMtxSetRotRPH(mtx, 0.0f, 0.0f, 0.005493164f * neg);
		MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
	}
	return true;
}

// TODO: Nozzle deform

void TWaterGun::init()
{
	mFlags                     = 0;
	mNozzleList[Spray]         = &mNozzleDeform;
	mNozzleList[Rocket]        = &mNozzleRocket;
	mNozzleList[Underwater]    = &mNozzleUnderWater;
	mNozzleList[Yoshi]         = &mNozzleYoshiDeform;
	mNozzleList[Hover]         = &mNozzleHover;
	mNozzleList[Turbo]         = &mNozzleTurbo;
	mCurrentNozzle             = Spray;
	mSecondNozzle              = Hover;
	mNozzleRocket.unk38C       = 0x81b;
	mNozzleTurbo.unk38C        = 0x814;
	mNozzleDeform.mBomb.unk38C = 0x813;
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
	unk1CFC           = 0.0f;
	unk1D00           = 0.0f;
	unk1D04           = 0;
	unk1D06           = -0x1800;
	unk1D08           = 0;

	// mEmitInfo = new TWaterEmitInfo; TODO

	unk1D08                         = 0;
	mNozzleDeform.mBomb.unk384      = true;
	mNozzleYoshiDeform.mBomb.unk384 = true;

	mEmitPos[3].x = mMario->mPosition.x;
	mEmitPos[3].y = mMario->mPosition.y;
	mEmitPos[3].z = mMario->mPosition.z;

	unk1CC0 = 0;
	unk1CC2 = 0;
	unk1CC4 = 0;

	unk1CC8 = 0.0f;
	unk1CD0 = 0;
	unk1CD2 = 0;

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
	// MTXCopy(
	//     mMario->mModelMario->unk8->mJointArray[mMario->mBindBoneIDArray[0]],
	//     mFluddModel->unk4->unk20);

	mFluddModel->unk4->initialize();

	s32 handleIdx
	    = mFluddModel->unk4->mModelData->unkB0->getIndex("jnt_G_handle");

	// unk1CDC is TMultiMtxEffect, but confused from implementation atm

	unk1CD8 = mFluddModel->unk4->mModelData->unkB0->getIndex("nozzle_center");

	for (int i = 0; i < 6; ++i) {
		if (!nozzleBmdData.getPath(i)) {
			mNozzleList[i]->unk380 = nullptr;
			continue;
		}

		MActorAnmData* nozzleData = new MActorAnmData();
		nozzleData->init(nozzleBmdData.getPath(i), nullptr);
		mNozzleList[i]->unk380 = new MActor(nozzleData);

		void* nozzleModelData
		    = JKRFileLoader::getGlbResource(nozzleBmdData.getBmdPath(i));
		J3DModel* nozzleModel = new J3DModel(
		    J3DModelLoaderDataBase::load(nozzleModelData, 0x10040000), 0, 1);
		mNozzleList[i]->unk380->setModel(nozzleModel, 0);

		// TODO: Figure out
		// ResTIMG* img = mFluddModel->unk4->mModelData->unkAC->getResTIMG(0);
		// SMS_ChangeTextureAll(mNozzleList[i]->unk380->unk4->mModelData,
		//                      "H_watergun_main_dummy", img);

		mNozzleList[i]->unk380->initDL();
		// Definitely inline potential
		if (nozzleBmdData.getFlags(i, 0) != 4) {
			s32 jointIdx
			    = mNozzleList[i]->unk380->unk4->mModelData->unkB0->getIndex(
			        "null_G_muzzle");
			nozzleBmdData.setJointIndex(i, 0, jointIdx);
		}
		if (nozzleBmdData.getFlags(i, 1) != 4) {
			s32 jointIdx
			    = mNozzleList[i]->unk380->unk4->mModelData->unkB0->getIndex(
			        "null_G_muzzle2");
			nozzleBmdData.setJointIndex(i, 1, jointIdx);
		}
		if (nozzleBmdData.getFlags(i, 2) != 4) {
			s32 jointIdx
			    = mNozzleList[i]->unk380->unk4->mModelData->unkB0->getIndex(
			        "null_G_muzzle3");
			nozzleBmdData.setJointIndex(i, 2, jointIdx);
		}
	}

	s32 jointIdx
	    = mNozzleList[Spray]->unk380->unk4->mModelData->unkB0->getIndex(
	        "chn_muzzle_l");
	// mNozzleList[Spray]
	//     ->unk380->unk4->mModelData->mJointNodePointer[jointIdx]
	//     ->mCallBack = functionPtr;
	// 5 more of these

	// MTXCopy that requires M3UModelMario at 0x3a8 in TMario

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
	J3DFrameCtrl* frameCtrl = mFluddModel->getFrameCtrl(MActor::ANM_TYPE_BCK);
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
				} else if (mMario->checkFlag(MARIO_FLAG_IN_SHALLOW_WATER
				                             | MARIO_FLAG_IN_WATER)) {
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
	if (!mMario->onYoshi()) {
		result = getYoshiMtx();
	} else {
		// This entire block is likely an inlined function.
		u8 currentNozzle = mCurrentNozzle;
		s8 flag          = nozzleBmdData.getFlags(currentNozzle, jointIndex);
		if (flag < 3) {
			return getCurrentNozzle()->unk380->unk4->getAnmMtx(
			    nozzleBmdData.getJointIndex(mCurrentNozzle, jointIndex));
		} else if (flag == 3) {
			return getYoshiMtx();
		}
	}
	return result;
}
#pragma dont_inline off

MtxPtr TWaterGun::getNozzleMtx()
{
	return mFluddModel->unk4->getAnmMtx(unk1CD8);
}

void TWaterGun::initInLoadAfter() { }

bool TWaterGun::isEmitting() { return false; }

void TWaterGun::changeNozzle(TNozzleType nozzleType, bool animate)
{
	f32 usedWater = mCurrentWater
	                / mNozzleList[mCurrentNozzle]->mEmitParams.mAmountMax.get();
	if (nozzleType == Spray) {
		if (animate) {
			unk1CFC = 0.0f;
		}
	} else {
		mSecondNozzle = nozzleType;
		if (animate) {
			unk1CFC = 1.0f;
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
			if (triggerNozzle->unk385 == TNozzleTrigger::ACTIVE) {
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

	unk1CC8 += (unk1CC2 - unk1CC8) * mWatergunParams.mChangeSpeed.get();
	unk1CCC += (unk1CC4 - unk1CCC) * mWatergunParams.mChangeSpeed.get();

	TNozzleBase* currentNozzle = getCurrentNozzle();

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
		f32 unk = unk1CFC;
		f32 sum = unk + unk1D00;
		unk1CFC = sum;
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

			currentNozzle = getCurrentNozzle();
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

			currentNozzle = getCurrentNozzle();
			currentNozzle->init(); // TODO: 2 vtable entry

			mCurrentWater
			    = waterPercentage * currentNozzle->mEmitParams.mAmountMax.get();
		}

		if (unk1CFC < 0.0) {
			unk1CFC = 0.0f;
			unk1D00 = 0.0f;
		}
		if (1.0f < unk1CFC) {
			unk1CFC = 1.0f;
			unk1D00 = 0.0f;
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
		emitCommon(param_1, emitInfo);

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

	MTXConcat(mtx, result, result);
	MTXCopy(result, mFluddModel->unk4->unk20);
}

// Not sure why this get's inlined aggressively
#pragma dont_inline on
TNozzleBase::TNozzleBase(const char* name, const char* prm, TWaterGun* fludd)
    : mEmitParams(prm)
    , mFludd(fludd)
{
	mEmitParams.load(mEmitParams.mPrmPath);
	unk36C = 2;
	unk36E = 0;
	unk372 = 0;
	unk378 = 0.0f;
	unk37C = 0.0f;
}
#pragma dont_inline off

void TNozzleBase::init()
{
	unk36C = 2;
	unk36E = 0;
	unk372 = 0;
	unk378 = 0.0f;
	unk37C = 0.0f;
}

void TNozzleBase::calcGunAngle(const TMarioControllerWork& work)
{
	// volatile u32 unused1[17];
	if ((size_t)mFludd->mMario == gpMarioAddress
	    && (gpCamera->isLButtonCameraSpecifyMode(gpCamera->mMode)
	        || gpCamera->isJetCoaster1stCamera())) {
		unk36E = gpCamera->unkA4;
		return;
	}

	s16 angle;
	if (mFludd->mMario->mAction == 0xC008220) {
		// TODO: Wrong reguster used, using r3 instead of r4
		angle = unk36E
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

	f32 diff = angle - unk36E;
	unk36E += diff * mEmitParams.mLAngleChase.get();
}

// TODO: This has a lot of inline functions, find them and update them
// properly
void TNozzleBase::animation(int param_1)
{
	if (param_1 != 2) {
		return;
	}

	if (0.0f < mFludd->unk1D00) {
		unk36C = 4;
	}

	if (mFludd->unk1D00 < 0.0f) {
		unk36C = 3;
	}

	switch (unk36C) {
	case 0: {

		if (!unk380->checkCurBckFromIndex(4)) {
			unk380->setBckFromIndex(4);
		}
		J3DFrameCtrl* ctrl = unk380->getFrameCtrl(MActor::ANM_TYPE_BCK);
		if (ctrl->mCurrentFrame <= (ctrl->mEndFrame - 0.1)
		    && (ctrl->mFlags & 3) == 0) {
			return;
		}
		unk36C = 1;
		break;
	}
	case 1: {

		if (!unk380->checkCurBckFromIndex(2)) {
			unk380->setBckFromIndex(2);
		}

		TWaterGun* fludd     = mFludd;
		bool updateAnimation = false;
		if (fludd->mCurrentWater == 0) {
			updateAnimation = false;
		} else {
			u8 gameState = gpMarDirector->unk124;
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
						if (trigger->unk385 == TNozzleTrigger::ACTIVE) {
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

		unk36C = 2;
		break;
	}
	case 2: {

		if (!unk380->checkCurBckFromIndex(3)) {
			unk380->setBckFromIndex(3);
		}
		TWaterGun* fludd     = mFludd;
		bool updateAnimation = false;
		if (fludd->mCurrentWater == 0) {
			updateAnimation = false;
		} else {
			u8 gameState = gpMarDirector->unk124;
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
						if (trigger->unk385 == TNozzleTrigger::ACTIVE) {
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
			unk36C = 0;
		}
		break;
	}

	case 3: {

		J3DFrameCtrl* ctrl = unk380->getFrameCtrl(MActor::ANM_TYPE_BCK);
		if (!unk380->checkCurBckFromIndex(1)) {
			unk380->setBckFromIndex(1);
		}

		// Use external tween value
		ctrl->mCurrentFrame = mFludd->unk1CFC * ctrl->mEndFrame;
		ctrl->mSpeed        = 0.0f;
		break;
	}
	case 4: {

		J3DFrameCtrl* ctrl = unk380->getFrameCtrl(MActor::ANM_TYPE_BCK);
		if (!unk380->checkCurBckFromIndex(0)) {
			unk380->setBckFromIndex(0);
		}

		// Use external tween value
		ctrl->mCurrentFrame = mFludd->unk1CFC * ctrl->mEndFrame;
		ctrl->mSpeed        = 0.0f;

		if (mFludd->unk1CFC < 1.0f) {
			return;
		}
		unk36C = 0;
		break;
	}
	}
}

void TNozzleTrigger::init()
{
	unk384 = false;
	unk385 = 0;
	unk36C = 0;
	unk386 = 0;
	unk388 = 0.0f;
}

void TNozzleTrigger::movement(const TMarioControllerWork& controllerWork)
{
	// TODO: Missing stack space
	volatile u32 unused[54];

	if (mFludd->mCurrentWater <= 0) {
		unk385 = TNozzleTrigger::INACTIVE;
		unk386 = 0;
		unk388 = 0.0f;
		return;
	}

	if (unk385 == TNozzleTrigger::ACTIVE) {
		unk386 -= 1;

		// Very likely an inline
		bool check;
		if (mFludd->mMario->unk380 == 0) {
			check = true;
		} else {
			check = false;
		}
		if (!check || unk386 <= 0) {
			unk385 = TNozzleTrigger::DEAD;
			unk388 = 0.0f;
			unk386 = 0;
		}
	}
	// Spam spray sound?
	if ((unk384 == true
	     && (controllerWork.mFrameInput & TMarioControllerWork::A) != 0
	     && (controllerWork.mInput & TMarioControllerWork::R) != 0)
	    && unk385 == TNozzleTrigger::INACTIVE) {
		unk385 = TNozzleTrigger::ACTIVE;
		if (unk38C != 0xffffffff) {
			u32 soundId;
			if (unk378 < 1.0f) {
				soundId = 0x806;
			} else {
				soundId = 0x805;
			}
			bool canPlay = gpMSound->gateCheck(soundId);
			if (canPlay) {
				MSoundSESystem::MSoundSE::startSoundActor(
				    soundId, mFludd->mEmitPos[0], 0, nullptr, 0, 4);
			}
		}
		unk386 = mEmitParams.mTriggerTime.get();
	}

	TMario* mario = mFludd->mMario;

	// Most likely some inlined stuff, not matching
	bool canSpray;
	bool other = true;
	if (mario->unk380 == 0) {
		canSpray = true;
	} else {
		canSpray = false;
	}
	if ((mario->unk118 & 0x30000) == 0
	    && mFludd->mCurrentWater < mEmitParams.mAmountMax.get()) {
		canSpray = false;
	}

	if (other && canSpray == true) {
		unk388 += 150.0f * controllerWork.mAnalogR;
		if (!unk384 && unk385 == TNozzleTrigger::INACTIVE) {
			// Pretty certain there is some inline function shenanigans here
			// Also what?
			// if(gpMarDirector->unk58 == (gpMarDirector->unk58 / mario->unk568)
			// * mario->unk568) {
			// SMSRumbleMgr->start((int)0x14, (int)mario->unk564,
			// (f32*)nullptr);
			//}
		}
		if (unk384 && unk385 == TNozzleTrigger::INACTIVE
		    && controllerWork.mAnalogR > 0.0f) {
			bool canPlay = gpMSound->gateCheck(0x4022);
			if (canPlay) {
				MSoundSESystem::MSoundSE::startSoundActor(
				    0x4022, mFludd->mEmitPos[0], 0, nullptr, 0, 4);
			}
		}
	}
	unk388 -= mEmitParams.mInsidePressureDec.get();
	if (unk388 < 0.0f) {
		unk388 = 0.0f;
	}

	if (unk388 > mEmitParams.mInsidePressureMax.get()) {
		unk388 = mEmitParams.mInsidePressureMax.get();
		if (!unk384 && unk385 == TNozzleTrigger::INACTIVE) {
			unk385      = TNozzleTrigger::ACTIVE;
			unk386      = mEmitParams.mTriggerTime.get();
			u32 soundId = unk38C;
			if (soundId != 0xffffffff) {
				// Non matching: Produces mr r4, r29 instead of addi r4, r29, 0
				const Vec* soundPos = (Vec*)(&mFludd->mEmitPos[0]);
				bool canPlay        = gpMSound->gateCheck(soundId);
				if (canPlay) {
					MSoundSESystem::MSoundSE::startSoundActor(soundId, soundPos,
					                                          0, nullptr, 0, 4);
				}
			}
			if (mFludd->mCurrentNozzle == (s8)TWaterGun::Hover) {
				SMSRumbleMgr->start((int)0x15, 0x8, (f32*)nullptr);
			}
			if (mFludd->mCurrentNozzle == (s8)TWaterGun::Rocket
			    || mFludd->mCurrentNozzle == (s8)TWaterGun::Turbo) {
				SMSRumbleMgr->start((int)0x15, 0x14, (f32*)nullptr);
			}
		}
	}

	if (unk385 == TNozzleTrigger::DEAD) {
		unk388 = 0.0f;
		if (controllerWork.mAnalogR == 0.0f) {
			unk385 = TNozzleTrigger::INACTIVE;
		}
	}

	calcGunAngle(controllerWork);
}

void TNozzleBase::movement(const TMarioControllerWork& controllerWork)
{
	// TODO: Missing stack space
	volatile u32 unused2[2];

	if (mFludd->mCurrentWater <= 0) {
		return;
	}
	s32 var1 = 256.0f * controllerWork.mAnalogR * 150.0f;

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
		if ((mFlags & 0x10) != 0) {
			mCurrentWater = 0;
		}
		movement();
	}

	if ((flags & 0x2) != 0) {
		calcAnimation(graphics);
	}

	mFluddModel->perform(flags, graphics);

	if ((flags & 0x2) != 0) {
		MActor* p2 = getCurrentNozzle()->unk380;
		if (p2 != nullptr) {
			MTXCopy(getModel()->getAnmMtx(unk1CD8), p2->unk4->unk20);
		}

		for (s32 index = 0;
		     index < nozzleBmdData.getEmitterCount(mCurrentNozzle); index++) {
			MtxPtr p1 = getEmitMtx(index);
			if (p1 != nullptr) {
				mEmitPos[index].x = p1[0][3];
				mEmitPos[index].y = p1[1][3];
				mEmitPos[index].z = p1[2][3];
			}
		}
	}

	if (getCurrentNozzle()->unk380) {
		getCurrentNozzle()->unk380->perform(flags, graphics);
	}
}

TNozzleBase* TWaterGun::getCurrentNozzle() const
{
	return mNozzleList[mCurrentNozzle];
}

void TWaterGun::setAmountToRate(f32 rate)
{
	// volatile u32 unused2[7]; // TODO: possibly inlined function
	if (mCurrentNozzle == 3) {
		TNozzleBase* currentNozzle = getCurrentNozzle();
		s32 amountMax = currentNozzle->mEmitParams.mAmountMax.get();
		mCurrentWater = amountMax;
	} else {
		TNozzleBase* currentNozzle = getCurrentNozzle();
		mCurrentWater = rate * currentNozzle->mEmitParams.mAmountMax.get();
	}
}

bool TWaterGun::isPressureOn()
{
	// volatile u32 unused2[6];
	if (getCurrentNozzle()->getNozzleKind() == 1) {
		TNozzleTrigger* triggerNozzle = (TNozzleTrigger*)getCurrentNozzle();
		if (triggerNozzle->unk388 > 0.0f) {
			return true;
		}
	}
	return false;
}

f32 TWaterGun::getPressure()
{
	// TODO: Missing stack space
	// volatile u32 unused2[5];
	if (getCurrentNozzle()->getNozzleKind() == 1) {
		TNozzleTrigger* triggerNozzle = (TNozzleTrigger*)getCurrentNozzle();
		return triggerNozzle->unk388;
	}
	return 0.0f;
}

f32 TWaterGun::getPressureMax()
{
	// TODO: Missing stack space
	// volatile u32 unused2[6];

	if (getCurrentNozzle()->getNozzleKind() == 1) {
		return getCurrentNozzle()->mEmitParams.mInsidePressureMax.get();
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
	mCurrentPressure = controllerWork.mAnalogR * 150.0f;

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
	if (!mMario->checkFlag(MARIO_FLAG_HELMET_FLW_CAMERA)
	    && mMario->checkFlag(MARIO_FLAG_IN_SHALLOW_WATER
	                         | MARIO_FLAG_IN_WATER)) {
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
		if (unk1CEC > 0.0f) {
			return;
		}
	}

	// TODO: Probably an enum
	// TODO: Probably inline function?
	if (hasFlag(0x4)) {
		mFlags &= ~0x4;
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
			JGeometry::TVec3<f32>* emitPos = &mEmitPos[0];
			playSoundWithInfo(0x24, emitPos, 0, getCurrentNozzle()->unk374);
		}
		case Yoshi:
		case Turbo: {
			JGeometry::TVec3<f32>* emitPos = &mEmitPos[0];
			playSoundWithInfo(0x0, emitPos, 0, getCurrentNozzle()->unk378);
			break;
		}
		case Underwater: {
			JGeometry::TVec3<f32>* emitPos = &mEmitPos[0];
			if (gpMSound->gateCheck(0x18)) {
				MSoundSESystem::MSoundSE::startSoundActor(0x18, emitPos, 0,
				                                          nullptr, 0, 4);
			}
		} break;
		case Rocket:
			break;
		case Hover:
			if (mIsEmitWater) {
				JGeometry::TVec3<f32>* emitPos = &mEmitPos[0];
				if (gpMSound->gateCheck(0x18)) {
					MSoundSESystem::MSoundSE::startSoundActor(0x18, emitPos, 0,
					                                          nullptr, 0, 4);
				}
			}
			break;
		}
	}
}
bool TWaterGun::suck()
{
	// TODO: Missing stack space
	// volatile u32 unused1[7];
	if (mCurrentNozzle == (s8)Yoshi) {
		return false;
	} else {
		s32 suckRate = getSuckRate();
		if (suckRate > 0) {
			mCurrentWater += suckRate;

			s32 currentWater = mCurrentWater;
			s32 maxWater     = getCurrentNozzle()->mEmitParams.mAmountMax.get();
			if (currentWater > maxWater) {
				mCurrentWater = maxWater;
			}

			if (!(mCurrentWater
			      >= getCurrentNozzle()->mEmitParams.mAmountMax.get())) {
				JGeometry::TVec3<f32>* emitPos = &mEmitPos[0];
				MSound* sound                  = gpMSound;
				if (sound->gateCheck(0xf)) {
					MSoundSESystem::MSoundSE::startSoundActor(0xf, emitPos, 0,
					                                          nullptr, 0, 4);
				}
			}
			return true;
		}
	}
	return false;
}

bool TWaterGun::damage()
{
	if (hasWater()) {
		TNozzleBase* nozzle = getCurrentNozzle();

		mCurrentWater -= nozzle->mEmitParams.mDamageLoss.value;

		if (mCurrentWater < 0) {
			mCurrentWater = 0;
		}
		return true;
	}
	return false;
}

void TWaterGun::changeBackup()
{
	// TODO: Missing stack space
	// volatile u32 unused2[5];
	if (unk1CFC == 0.0f) {
		if (gpMSound->gateCheck(0x812)) {
			MSoundSESystem::MSoundSE::startSoundSystemSE(0x812, 0, nullptr, 0);
		}
		unk1D00 = mWatergunParams.mChangeSpeed.get();
	}

	if (unk1CFC == 1.0f) {
		if (gpMSound->gateCheck(0x811)) {
			MSoundSESystem::MSoundSE::startSoundSystemSE(0x811, 0, nullptr, 0);
		}
		unk1D00 = -mWatergunParams.mChangeSpeed.get();
	}
}
