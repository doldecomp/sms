#include <Player/Yoshi.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/ModelUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/M3UModelMario.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Strategic/question.hpp>
#include <Strategic/MirrorActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MAnmSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MoveBG/Item.hpp>
#include <Player/Tongue.hpp>
#include <Player/MarioMain.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/WaterGun.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static const char cDirtyFileName[] = "/scene/map/pollution/H_ma_rak.bti";
static const char cDirtyTexName[]  = "H_ma_rak_dummy";

static const GXColor bodyColor[4] = {
	{ 0x40, 0xA1, 0x24, 0xFF },
	{ 0xFF, 0x8C, 0x1C, 0xFF },
	{ 0xAA, 0x4C, 0xFF, 0xFF },
	{ 0xFF, 0xA0, 0xBE, 0xFF },
};

void TYoshi::startVoice(u32 param_1) { }

BOOL YoshiHeadCtrl(J3DNode* param_1, int param_2)
{
	if (param_2 == 0) {
		const TWaterGun* waterGun = SMS_GetMarioWaterGun();
		s16 angle                 = waterGun->getCurrentNozzle()->getGunAngle();
		Mtx mtx;
		MsMtxSetRotRPH(mtx, 0.0f, 0.0f, SHORTANGLE2DEG(angle));
		MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
	}
	return true;
}

J3DFrameCtrl* TYoshi::getFrameCtrl() const { return mActor->getFrameCtrl(0); }

MtxPtr TYoshi::getMtxPtrFootL() const
{
	return mActor->getModel()->getAnmMtx(mJointIdxFootL);
}

MtxPtr TYoshi::getMtxPtrFootR() const
{
	return mActor->getModel()->getAnmMtx(mJointIdxFootR);
}

void TYoshi::init(TMario* param_1)
{
	mMario = param_1;
	mState = STATE_EGG;
	unk1   = 0;
	unk2   = unk6;
	unk4   = 7200;
	unk6   = 7200;
	unk8   = 21300;
	unkC   = unk8;
	mTranslation.set(0.0f, 0.0f, 0.0f);
	unk2C        = 0.0f;
	mEggRotSpeed = 0;
	unk72        = 0x180;
	unk30        = new MActorAnmData;
	unk30->init("yoshi", nullptr);
	mActor               = new MActor(unk30);
	void* yoshiModelRaw  = JKRGetResource("/yoshi/yoshi_model.bmd");
	J3DModel* yoshiModel = new J3DModel(
	    J3DModelLoaderDataBase::load(yoshiModelRaw, 0x10040000), 0, 1);
	mActor->setModel(yoshiModel, 0);
	mActor->initNormalMotionBlend();
	mActor->offMakeDL();

	mJointIdxTongue
	    = (u8)mActor->getModel()->getModelData()->getJointName()->getIndex(
	        "null_tongue");
	mJointIdxFootL
	    = (u8)mActor->getModel()->getModelData()->getJointName()->getIndex(
	        "jnt_foot_L");
	mJointIdxFootR
	    = (u8)mActor->getModel()->getModelData()->getJointName()->getIndex(
	        "jnt_foot_R");
	mJointIdxCenter
	    = (u8)mActor->getModel()->getModelData()->getJointName()->getIndex(
	        "center");

	mMirrorModels[0]
	    = SMS_CreatePartsModel("/yoshi/yoshi_hand2_l.bmd", 0x10040000);
	mMirrorModels[1]
	    = SMS_CreatePartsModel("/yoshi/yoshi_hand2_r.bmd", 0x10040000);

	{
		J3DModelData* modelData = mMirrorModels[0]->getModelData();
		modelData->getTexture()->setResTIMG(
		    0,
		    *mActor->getModel()->getModelData()->getTexture()->getResTIMG(0));
		DCFlushRange(modelData->getTexture()->getResTIMG(0), sizeof(ResTIMG));
	}

	{
		J3DModelData* modelData = mMirrorModels[1]->getModelData();
		modelData->getTexture()->setResTIMG(
		    0,
		    *mActor->getModel()->getModelData()->getTexture()->getResTIMG(0));
		DCFlushRange(modelData->getTexture()->getResTIMG(0), sizeof(ResTIMG));
	}

	unk4C = J3DAnmLoaderDataBase::load(JKRGetResource("/yoshi/yoshi_eat.bck"));
	unk54 = J3DNewMtxCalcAnm(mActor->getModel()->getModelData()->getUnkC(),
	                         (J3DAnmTransform*)unk4C);
	unk50 = J3DAnmLoaderDataBase::load(
	    JKRGetResource("/yoshi/yoshi_eat_end.bck"));
	unk58 = J3DNewMtxCalcAnm(mActor->getModel()->getModelData()->getUnkC(),
	                         (J3DAnmTransform*)unk50);

	unk5C.setFrame(unk5C.getStart());
	unk5C.setRate(1.0f);
	unk5C.setAttribute(0);
	unk5C.setRate(0.5f);

	mTongue = new TYoshiTongue;
	mTongue->init(this);
	unk80   = 0.01f;
	unk84.x = (f32)bodyColor[0].r;
	unk84.y = (f32)bodyColor[0].g;
	unk84.z = (f32)bodyColor[0].b;
	unk90   = 160.0f;
	unk94   = new TRidingInfo;
	unk98   = 20.0f;
	unk9C   = 30.0f;
	unkA0   = 0.1f;
	unkA4   = 0.045f;
	unkA8   = new J3DDrawBuffer(0x20);
	unkAC   = new J3DDrawBuffer(0x20);
	unkB0   = 0;
	unkB4   = 0;
	unkA8->frameInit();
	unkAC->frameInit();
	mFlutterState        = 2;
	mFlutterTimer        = 0;
	mMaxFlutterTimer     = 120;
	mMaxVSpdStartFlutter = -5.0f;
	mFlutterAcceleration = 1.2f;
	mType                = 0;
	unkD8                = 2000;
	unkD4                = unkD8;
	unkDC                = 0;
	unkDE                = 0;
	unkE0                = 0;
	unkE4                = 0.1f;
	unkE8                = 120;
	unkEA                = 240;
	unkEC                = 4.0f;
	mBtpIndex            = 4;
	mEgg                 = nullptr;

	unkF6 = mActor->getModel()->getModelData()->getJointName()->getIndex(
	    "jnt_mouth");
	unkF8 = mActor->getModel()->getModelData()->getJointName()->getIndex(
	    "jnt_neck_2");
	unkFC.zero();
	unk108.zero();
	unk114 = 80.0f;
	mActor->getFrameCtrl(0)->setRate(0.5f);
	mActor->getFrameCtrl(3)->setRate(0.5f);

	mActor->getModel()->setBaseTRMtx(
	    mMario->mModel->getModel()->getBaseTRMtx());
	mActor->getModel()->calc();

	mActor->getModel()->getModelData()->onFlag1OnAllShapes();

	mMirrorModels[0]->setBaseTRMtx(mActor->getModel()->getAnmMtx(37));
	mMirrorModels[1]->setBaseTRMtx(mActor->getModel()->getAnmMtx(32));
	mMirrorModels[0]->calc();
	mMirrorModels[1]->calc();

	for (int i = 0; i < 2; ++i)
		mMirrorModels[i]->getModelData()->onFlag1OnAllShapes();

	mBodyAnmSound = new MAnmSound(SMSGetMSound());
	mBodyAnmSound->initAnmSound(nullptr, 1, 0.0f);
	mTongueAnmSound = new MAnmSound(SMSGetMSound());
	mTongueAnmSound->initAnmSound(nullptr, 1, 0.0f);

	{
		u32 tableSize      = 26;
		mBodyAnmSoundTable = new void*[tableSize];
		for (int i = 0; i < 26; ++i)
			mBodyAnmSoundTable[i] = nullptr;
	}

	// clang-format off
	mBodyAnmSoundTable[0]  = JKRGetResource("/yoshi/bas/yoshi_born.bas");
	mBodyAnmSoundTable[1]  = JKRGetResource("/yoshi/bas/yoshi_damage.bas");
	mBodyAnmSoundTable[2]  = JKRGetResource("/yoshi/bas/yoshi_demo_shine_get.bas");
	mBodyAnmSoundTable[3]  = JKRGetResource("/yoshi/bas/yoshi_eat.bas");
	mBodyAnmSoundTable[4]  = JKRGetResource("/yoshi/bas/yoshi_eat_end.bas");
	mBodyAnmSoundTable[5]  = JKRGetResource("/yoshi/bas/yoshi_goff.bas");
	mBodyAnmSoundTable[6]  = JKRGetResource("/yoshi/bas/yoshi_hip_end.bas");
	mBodyAnmSoundTable[8]  = JKRGetResource("/yoshi/bas/yoshi_hip_start.bas");
	mBodyAnmSoundTable[9]  = JKRGetResource("/yoshi/bas/yoshi_hold_jump.bas");
	mBodyAnmSoundTable[10] = JKRGetResource("/yoshi/bas/yoshi_jump.bas");
	mBodyAnmSoundTable[11] = JKRGetResource("/yoshi/bas/yoshi_jump_end.bas");
	mBodyAnmSoundTable[13] = JKRGetResource("/yoshi/bas/yoshi_pivot.bas");
	mBodyAnmSoundTable[14] = JKRGetResource("/yoshi/bas/yoshi_ride.bas");
	mBodyAnmSoundTable[15] = JKRGetResource("/yoshi/bas/yoshi_run.bas");
	mBodyAnmSoundTable[16] = JKRGetResource("/yoshi/bas/yoshi_sidewalk_l.bas");
	mBodyAnmSoundTable[17] = JKRGetResource("/yoshi/bas/yoshi_sidewalk_r.bas");
	mBodyAnmSoundTable[18] = JKRGetResource("/yoshi/bas/yoshi_slide_end.bas");
	mBodyAnmSoundTable[20] = JKRGetResource("/yoshi/bas/yoshi_wait.bas");
	mBodyAnmSoundTable[21] = JKRGetResource("/yoshi/bas/yoshi_wait_alone.bas");
	mBodyAnmSoundTable[22] = JKRGetResource("/yoshi/bas/yoshi_walk.bas");
	mBodyAnmSoundTable[23] = JKRGetResource("/yoshi/bas/yoshi_water_die.bas");
	// clang-format on

	changeAnimation(0x17);
}

void TYoshi::initInLoadAfter()
{
	mTongue->initInLoadAfter();

	TMirrorActor* mirrorMain = new TMirrorActor("ヨッシーin鏡");
	mirrorMain->init(mActor->getModel(), 4);

	for (int i = 0; i < 2; ++i) {
		TMirrorActor* mirror = new TMirrorActor("ヨッシー手in鏡");
		mirror->init(mMirrorModels[i], 4);
	}

	mActor->getModel()->getModelData()->getJointNodePointer(21)->setCallBack(
	    &YoshiHeadCtrl);
}

void TYoshi::thinkBtp(int id)
{
	u16 btpId = 4;

	switch (id) {
	case 0:
		btpId = 0;
		break;
	case 1:
		btpId = 1;
		break;
	case 9:
		btpId = 2;
		break;
	case 25:
		btpId = 3;
		break;
	}

	if (mBtpIndex != btpId) {
		mActor->setBtpFromIndex(btpId);
		J3DFrameCtrl* frameCtrl = mActor->getFrameCtrl(3);
		frameCtrl->setRate(0.5f);
		mBtpIndex = btpId;
	}
}

void TYoshi::changeAnimation(int id)
{
	if (id != mActor->getCurAnmIdx(0)) {
		mActor->setBck(id);
		thinkBtp(id);
		mBodyAnmSound->initAnmSound(mBodyAnmSoundTable[id], 1, 0.0f);
	}
}

u16 TYoshi::changeHand()
{
	u16 curIdx = mActor->getCurAnmIdx(0);
	u32 action = mMario->mAction;

	if (action & 0x400) {
		if (curIdx == 12)
			return 11;
		if (action == 0x800456 || action == 0x84045D || action == 0x4045E
		    || ((action & 0x40000) ? true : false))
			return 19;
		return 15;
	}
	if (action & 0x800) {
		if (mFlutterState == 1)
			return 9;
		if (action == 0x8008A9) {
			u16 actionState = mMario->mActionState;
			if ((s16)actionState < 2) {
				if ((s16)actionState >= 0)
					return 8;
			} else if ((s16)actionState < 4) {
				return 7;
			}
		}
		if (mMario->mVel.y > 0.0f)
			return 10;
		return 12;
	}
	if ((action & 0x200)
	    && (action == 0x386 || (u32)(action - 0x0C00023D) <= 1)) {
		return 18;
	}
	if ((action & 0x8000) ? true : false) {
		if (mMario->mGamePad->checkMeaning(0x2000)) {
			E_SIDEWALK_TYPE type;
			f32 a, b;
			mMario->getSideWalkValues(&type, &a, &b);
			switch ((s32)type) {
			case 0:
				return 22;
			case 1:
				return 16;
			case 2:
				return 17;
			}
		}
		if (mMario->mGamePad->checkMeaning(0x400))
			return 13;
	}

	u32 a2 = mMario->mAction;
	if (a2 == 0x8023C)
		return 6;
	if (a2 == 0x1302)
		return 2;
	return 22;
}

#pragma dont_inline on
void TYoshi::getEmitPosDir(JGeometry::TVec3<f32>* dir,
                           JGeometry::TVec3<f32>* pos) const
{
	MtxPtr mtx = mActor->getModel()->getAnmMtx(mJointIdxTongue);

	pos->x = mtx[0][0];
	pos->y = mtx[1][0];
	pos->z = mtx[2][0];

	dir->x = mtx[0][3];
	dir->y = mtx[1][3];
	dir->z = mtx[2][3];
}
#pragma dont_inline off

void TYoshi::setEggYoshiPtr(TEggYoshi* egg) { mEgg = egg; }

void TYoshi::appear() { }

bool TYoshi::appearFromEgg(const JGeometry::TVec3<f32>& pos, f32 yrot,
                           TEggYoshi* egg)
{
	mLastTranslation = pos;
	mTranslation     = pos;
	mTranslation.y += 1.0f;
	mEggRotSpeed = DEG2SHORTANGLE(yrot);
	mState       = STATE_UNK2;

	changeAnimation(0);

	TTakeActor* fruit = (TTakeActor*)egg->getFruit();
	if (mMario->getHeldObject() == fruit) {
		fruit->receiveMessage(mMario->getFloorHitActor(), HIT_MESSAGE_UNK8);
		fruit->mHolder      = nullptr;
		mMario->mHeldObject = nullptr;
	}

	fruit->receiveMessage(mMario->getFloorHitActor(), HIT_MESSAGE_UNKB);

	doEat(fruit->getActorType());
	unkC = unk8;
	mEgg = egg;
	unk2 = unk4;
	return true;
}

bool TYoshi::disappear()
{
	if (isHatched()) {
		if (mState == STATE_MOUNTED)
			mMario->getOffYoshi(true);

		if (mMario->checkFlag(0x30000)) {
			mState = STATE_DROWNING;
			changeAnimation(0x19);
		} else {
			mState = STATE_DYING;
		}

		mType = 0;
		unk2  = 30;
		return true;
	}

	return false;
}

void TYoshi::kill()
{

	disappear();
	mBodyAnmSound->stop();
	mTongueAnmSound->stop();
}

void TYoshi::ride()
{
	mState = STATE_MOUNTED;

	changeAnimation(0x16);

	gpModelWaterManager->unk5D5F = mType;

	SMSGetMSound()->startSoundActor(0x7921, &mTranslation, 0, nullptr, 0, 4);

	SMSGetMSound()->unk88 = 1;
	MSBgm::setStageBgmYoshiPercussion(true);
	gpMarDirector->fireRideYoshi(this);
}

void TYoshi::getOff(bool param_1)
{
	if (mState != STATE_MOUNTED)
		return;

	unk2C  = 0.0f;
	mState = STATE_UNMOUNTED;
	unk2   = unk4;

	if (param_1 == true) {
		changeAnimation(1);

		SMSGetMSound()->startSoundActor(0x7918, &mTranslation, 0, nullptr, 0,
		                                4);

		SMSRumbleMgr->start(0x15, 0x14, (f32*)nullptr);
	} else {
		changeAnimation(0x17);

		SMSGetMSound()->startSoundActor(0x7924, &mTranslation, 0, nullptr, 0,
		                                4);
	}

	SMS_RideMoveCalcLocalPos(unk94, mTranslation);
	SMSGetMSound()->unk88 = 1;
	MSBgm::setStageBgmYoshiPercussion(false);
}

void TYoshi::thinkJumpEnd(u16, u16*) { }

// TODO: tons of missing inlines
void TYoshi::thinkAnimation()
{
	f32 nextFrame = mMario->getMotionFrameCtrl().getRate();
	u16 curIdx    = mActor->getCurAnmIdx(0);
	u16 newIdx    = curIdx;
	u32 action    = mMario->mAction;

	if (action & 0x400) {
		BOOL tmp;
		if (curIdx == 12) {
			newIdx = 11;
			tmp    = true;
		} else {
			tmp = false;
		}

		if (!tmp) {
			newIdx = 15;
			if (action == 0x800456 || action == 0x84045D || action == 0x4045E
			    || (action & 0x40000 ? true : false)) {
				newIdx = 19;
			}
		}
	} else if (action & 0x800) {
		if (mFlutterState == 1) {
			newIdx = 9;
		} else if (action == 0x8008A9) {
			switch (mMario->mActionState) {
			case 0:
			case 1:
				newIdx = 8;
				break;
			case 2:
			case 3:
				newIdx = 7;
				break;
			default:
				goto fallback;
			}
		} else {
		fallback:
			if (mMario->mVel.y > 0.0f)
				newIdx = 10;
			else
				newIdx = 12;
		}
	} else if ((action & 0x200)
	           && (action == 0x386 || action == 0xC000023D
	               || action == 0xC000023E)) {
		newIdx = 18;
	} else {
		bool sliding = (action & 0x8000) ? true : false;
		if (sliding) {
			if (mMario->mGamePad->checkMeaning(0x2000)) {
				E_SIDEWALK_TYPE type;
				f32 dummy;
				mMario->getSideWalkValues(&type, &nextFrame, &dummy);
				switch (type) {
				case 0:
					newIdx = 22;
					break;
				case 1:
					newIdx = 16;
					break;
				case 2:
					newIdx = 17;
					break;
				}
			} else if (mMario->mGamePad->checkMeaning(0x400)) {
				newIdx = 13;
			} else {
				goto walking;
			}
		} else {
		walking:
			u32 act = mMario->getAction();
			if (act == 0x8023C)
				newIdx = 6;
			else if (act == 0x1302)
				newIdx = 2;
			else
				newIdx = 22;
		}
	}

	if (curIdx == 24)
		curIdx = 15;
	if (newIdx == 24)
		newIdx = 15;

	if (newIdx != curIdx) {
		if (newIdx == 15) {
			mActor->setBckFromIndex(24);
			mActor->setBckFromIndex(15);
		} else {
			mActor->setBck(newIdx);
		}
		thinkBtp(newIdx);
		mBodyAnmSound->initAnmSound(mBodyAnmSoundTable[newIdx], 1, 0.0f);
	}

	if (newIdx == 15) {
		f32 base  = unk98;
		f32 ratio = (mMario->mForwardVel - base) / (unk9C - base);
		if (ratio < 0.0f)
			ratio = 0.0f;
		if (ratio > 1.0f)
			ratio = 1.0f;

		f32 tmp = 1.0f - ratio;
		mActor->setMotionBlendRatioForBck(tmp);

		J3DAnmTransform* oldAnm = mActor->getBckOldMotionBlendAnmPtr();
		oldAnm->mFrame          = mActor->getFrameCtrl(0)->getFrame();

		if (mMario->mAction == 0x4045C)
			nextFrame = mMario->getMotionFrameCtrl().getRate();
		else
			nextFrame = unkA0 + unkA4 * mMario->mForwardVel;
	} else {
		mActor->setMotionBlendRatioForBck(0.0f);
	}

	mActor->getFrameCtrl(0)->setRate(nextFrame);
}

void TYoshi::thinkUpper()
{
	if (mState != STATE_MOUNTED)
		return;

	unk5C.update();

	J3DJoint* joint
	    = mActor->getModel()->getModelData()->getJointNodePointer(18);
	const TWaterGun* waterGun = mMario->mWaterGun;

	bool shouldUseEatMtx = false;

	if (mTongue->mState != TYoshiTongue::STATE_IDLE
	    && waterGun->mCurrentWater != 0) {
		if (waterGun->getCurrentNozzle()->getNozzleKind() == 1) {
			if (((TNozzleTrigger*)waterGun->getCurrentNozzle())->unk385 == 1)
				shouldUseEatMtx = true;
			else
				shouldUseEatMtx = false;
		} else {
			if (waterGun->getCurrentNozzle()->unk378 > 0.0f)
				shouldUseEatMtx = true;
			else
				shouldUseEatMtx = false;
		}
	}

	if (shouldUseEatMtx) {
		if (joint->getMtxCalc() != unk54) {
			unk5C.setFrame(unk5C.getStart());
			unk5C.setRate(1.0f);
			unk5C.setEnd(unk4C->getFrameMax());
			unk5C.setFrame(0.0f);
			joint->setMtxCalc(unk54);
			mBodyAnmSound->initAnmSound(mBodyAnmSoundTable[3], 1, 0.0f);
		}

		unk4C->setFrame(unk5C.getFrame());
	} else {
		if (joint->getMtxCalc() == unk58) {
			unk5C.setFrame(unk5C.getStart());
			unk5C.setRate(1.0f);
			unk5C.setEnd(unk50->getFrameMax());
			unk5C.setFrame(0.0f);
			joint->setMtxCalc(unk58);
			mBodyAnmSound->initAnmSound(mBodyAnmSoundTable[4], 1, 0.0f);
		} else if (joint->getMtxCalc() != unk58) {
			if (unk5C.checkState(J3DFrameCtrl::STATE_COMPLETED_ONCE
			                     | J3DFrameCtrl::STATE_LOOPED_ONCE))
				joint->setMtxCalc(nullptr);
		}

		unk50->setFrame(unk5C.getFrame());
	}
}

void TYoshi::emitTongue()
{
	JGeometry::TVec3<f32> pos;
	JGeometry::TVec3<f32> dir;
	getEmitPosDir(&pos, &dir);
	JGeometry::TVec3<f32> vel;
	vel.set(mMario->mVel);
	mTongue->emit(pos, dir, vel);

	int tries = 0;
	do {
		mTongue->movement();
		// TODO: dumb but matches?
		if (JGeometry::TVec3<f32>(mTongue->mTipPos - pos).length() > unk90)
			break;
		++tries;
	} while (tries < 10);

	unkDC = 3;
}

void TYoshi::doSearch()
{
	switch (unkDC) {
	case 0:
		unkDE -= 1;
		if ((s16)unkDE <= 0) {
			THitActor* target = mTongue->findTarget(false, false);
			if (target != nullptr) {
				JGeometry::TVec3<f32> diff;
				diff.sub(target->mPosition, mTranslation);
				unkE0 = matan(diff.z, diff.x);
				unkDC = 1;
				return;
			}
			unkDE = (s16)((f32)(unkEA - unkE8) * MsRandF() + (f32)unkE8);
		}
		return;

	case 1: {
		s16 prev     = mEggRotSpeed;
		s16 delta    = (s16)(unkE4 * (f32)(unkE0 - prev));
		mEggRotSpeed = prev + delta;
		if (delta > -256 && delta < 256) {
			emitTongue();
			changeAnimation(3);
		}
		break;
	}

	case 2:
		if (mTongue->findTarget(false, true) != nullptr) {
			emitTongue();
		} else {
			unkDE = (s16)((f32)(unkEA - unkE8) * MsRandF() + (f32)unkE8);
			unkDC = 0;
		}
		break;

	case 3: {
		thinkEat();
		if (mTongue->mState == TYoshiTongue::STATE_IDLE) {
			unkDE = (s16)((f32)(unkEA - unkE8) * MsRandF() + (f32)unkE8);
			unkDC = 0;
			changeAnimation(0x17);
		}
		break;
	}
	}
}

void TYoshi::doEat(u32 param_1)
{
	int r31;
	BOOL bVar1 = true;

	switch (param_1) {
	case 0x40000391:
	case 0x40000392:
		r31 = 1;
		break;
	case 0x40000393:
	case 0x40000395:
		r31 = 2;
		break;
	case 0x40000394:
	case 0x40000390:
		r31 = 3;
		break;
	default:
		bVar1 = FALSE;
		break;
	}

	gpMarioParticleManager->emitAndBindToMtxPtr(
	    0x3D, mActor->getModel()->getAnmMtx(unkF6), 0, this);

	if (bVar1 == TRUE) {
		mType = r31;
		unkC  = unk8;
		gpMarioParticleManager->emitAndBindToPosPtr(0x3E, &unk108, 0, this);
		SMSGetMSound()->startSoundActor(0x1947, &mTongue->mTipPos, 0, nullptr,
		                                0, 4);
	}
}

void TYoshi::thinkEat()
{
	if (mTongue->mActorTypeInMouth != 0) {
		doEat(mTongue->mActorTypeInMouth);
		if (mMario->onYoshi())
			SMSRumbleMgr->start(0x15, 0x14, (f32*)nullptr);
		mTongue->mActorTypeInMouth = 0;
	}
}

void TYoshi::thinkHoldOut()
{
	switch (mFlutterState) {
	case 0:
		if (mMario->mVel.y < mMaxVSpdStartFlutter
		    && mMario->mGamePad->checkMeaning(0x80))
			mFlutterState = 1;
		break;
	case 1:
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    0x119, mActor->getModel()->getAnmMtx(unkF6), 1, this);
		if (mMario->mVel.y < 0.0f
		    && 0.0f <= mFlutterAcceleration + mMario->mVel.y)
			SMSGetMSound()->startSoundActor(0x7926, &mTranslation, 0, nullptr,
			                                0, 4);
		if (mFlutterTimer != 0) {
			mFlutterTimer -= 1;
			mMario->mVel.y += mFlutterAcceleration;
		} else {
			mFlutterState = 2;
		}

		if (!mMario->mGamePad->checkMeaning(0x80))
			mFlutterState = 2;
		break;
	case 2:
		mFlutterTimer = 0;
		break;
	}
}

void TYoshi::movement()
{
	if (!gpMarDirector->isDemoMode3() && !gpMarDirector->isDemoMode4()
	    && !gpMarDirector->isTalkModeNow()) {
		if (!mMario->checkActionFlag(0x1000) && unkC > 0) {
			unkC -= 1;
		}
	}

	thinkEat();

	switch (mState) {
	case STATE_UNK2:
		if (mActor->curAnmEndsNext(0, nullptr)) {
			mState = STATE_UNMOUNTED;
			changeAnimation(0x17);
			gpMSound->startMarioVoice(0x7919, 1, 1);
		}
		break;

	case STATE_UNMOUNTED: {
		if (mActor->curAnmEndsNext(0, nullptr))
			mActor->setBckFromIndex(0x17);

		f32 rot = SHORTANGLE2DEG(mEggRotSpeed);
		SMS_RideMoveByGroundActor(unk94, &mTranslation, &rot);
		mEggRotSpeed = DEG2SHORTANGLE(rot);

		const TBGCheckData* ground;
		JGeometry::TVec3<f32> trans = mTranslation;
		f32 groundY
		    = gpMap->checkGround(trans.x, 200.0f + trans.y, trans.z, &ground);
		mActor->setLightData(ground, mTranslation);
		unk2C -= mMario->mJumpParams.mGravity.get();
		mTranslation.y += unk2C;

		if (groundY > mTranslation.y) {
			if (ground->isIllegalData() || ground->isWaterSurface()
			    || ground->isDeathPlane()) {
				disappear();
				break;
			} else {
				mTranslation.y = groundY;
				unk2C          = 0.0f;
			}
		}
		doSearch();
		if (unkC <= 0)
			disappear();
		break;
	}

	case STATE_UNK7: {
		s16 prev = mEggRotSpeed;
		mEggRotSpeed
		    = (s16)(unkE4 * (f32)(mMario->mFaceAngle.y - prev) + (f32)prev);
		break;
	}

	case STATE_MOUNTED: {
		mTranslation = mMario->mPosition;
		mEggRotSpeed = mMario->mFaceAngle.y;

		if (mMario->mGamePad->checkMeaning(0x100)) {
			emitTongue();
		}
		if (unkC <= 0)
			disappear();
		break;
	}

	case STATE_UNK1:
		unk2 -= 1;
		if (unk2 <= 0) {
			mState = STATE_UNMOUNTED;
			changeAnimation(0x17);
			unk2 = unk4;
		}
		break;

	case STATE_DROWNING:
		if (mActor->getFrameCtrl(0)->checkPass(60.0f)) {
			gpMarioParticleManager->emitAndBindToPosPtr(0x3F, &unk74, 0, this);
		}
		if (mActor->curAnmEndsNext(0, nullptr)) {
			mState = STATE_UNK5;
			unk2   = 30;
		}
		break;

	case STATE_DYING:
		gpMarioParticleManager->emitAndBindToPosPtr(0x3F, &unk74, 0, this);
		mState = STATE_UNK5;
		unk2   = 30;
		break;

	case STATE_UNK5:
		mState = STATE_EGG;
		if (mEgg != nullptr)
			mEgg->startFruit();
		break;
	}

	if (isHatched()) {
		f32 dr = unk80 * ((f32)bodyColor[mType].r - unk84.x);
		unk84.x += dr;
		f32 dg = unk80 * ((f32)bodyColor[mType].g - unk84.y);
		unk84.y += dg;
		f32 db = unk80 * ((f32)bodyColor[mType].b - unk84.z);
		unk84.z += db;

		mTongue->movement();

		const TBGCheckData* ground;
		f32 groundY = gpMap->checkGround(
		    mTranslation.x, 200.0f + mTranslation.y, mTranslation.z, &ground);
		mActor->setLightData(ground, mTranslation);

		if (mState == STATE_UNMOUNTED && groundY > mTranslation.y) {
			if (ground->isWaterSurface()) {
				disappear();
			} else {
				mTranslation.y = groundY;
				unk2C          = 0.0f;
			}
		}

		mActor->getModel()->getModelData()->offFlag1OnAllShapes();
		mMirrorModels[0]->getModelData()->offFlag1OnAllShapes();
		mMirrorModels[1]->getModelData()->offFlag1OnAllShapes();
	} else {
		mActor->getModel()->getModelData()->onFlag1OnAllShapes();
		mMirrorModels[0]->getModelData()->onFlag1OnAllShapes();
		mMirrorModels[1]->getModelData()->onFlag1OnAllShapes();
	}

	if (mMario->onYoshi())
		gpModelWaterManager->unk5D5F = mType;

	{
		MtxPtr m = mActor->getModel()->getAnmMtx(mJointIdxCenter);
		unk74.x  = m[0][3];
		unk74.y  = m[1][3];
		unk74.z  = m[2][3];
	}
}

void TYoshi::calcAnim()
{
	Mtx mtx;

	switch (mState) {
	case STATE_UNK2:
	case STATE_DROWNING:
	case STATE_UNMOUNTED:
	case STATE_UNK7:
		J3DGetTranslateRotateMtx(0, mEggRotSpeed, 0, mTranslation.x,
		                         mTranslation.y, mTranslation.z, mtx);
		break;
	case STATE_MOUNTED:
		thinkAnimation();
		PSMTXCopy(mMario->getTakenMtx(), mtx);
		break;
	case STATE_UNK1:
		J3DGetTranslateRotateMtx(0, mEggRotSpeed, 0, mTranslation.x,
		                         mTranslation.y + 100.0f * (f32)unk2,
		                         mTranslation.z, mtx);
		break;
	case STATE_UNK5:
		J3DGetTranslateRotateMtx(0, mEggRotSpeed, 0, mTranslation.x,
		                         mTranslation.y, mTranslation.z, mtx);
		break;
	case STATE_DYING:
	case STATE_EGG:
		break;
	}

	if (isHatched()) {
		thinkUpper();
		switch (mActor->getCurAnmIdx(0)) {
		case 10:
		case 12:
		case 15:
			mActor->getModel()->getModelData()->getShapeNodePointer(0)->onFlag(
			    1);
			mActor->getModel()->getModelData()->getShapeNodePointer(1)->onFlag(
			    1);
			mMirrorModels[0]->getModelData()->offFlag1OnAllShapes();
			mMirrorModels[1]->getModelData()->offFlag1OnAllShapes();
			break;
		default:
			mActor->getModel()->getModelData()->getShapeNodePointer(0)->offFlag(
			    1);
			mActor->getModel()->getModelData()->getShapeNodePointer(1)->offFlag(
			    1);
			mMirrorModels[0]->getModelData()->onFlag1OnAllShapes();
			mMirrorModels[1]->getModelData()->onFlag1OnAllShapes();
			break;
		}

		PSMTXCopy(mtx, mActor->getModel()->getBaseTRMtx());
		mActor->calcAnm();
		PSMTXCopy(mActor->getModel()->getAnmMtx(37),
		          mMirrorModels[0]->getBaseTRMtx());
		PSMTXCopy(mActor->getModel()->getAnmMtx(32),
		          mMirrorModels[1]->getBaseTRMtx());
		mMirrorModels[0]->calc();
		mMirrorModels[1]->calc();
		Mtx tongueMtx;
		PSMTXCopy(mActor->getModel()->getAnmMtx(mJointIdxTongue), tongueMtx);
		mTongue->calcAnim(tongueMtx);
	}

	{
		MtxPtr m = mActor->getModel()->getAnmMtx(unkF6);
		unkFC.x  = m[0][3];
		unkFC.y  = m[1][3];
		unkFC.z  = m[2][3];
	}
	{
		MtxPtr m = mActor->getModel()->getAnmMtx(unkF8);
		unk108.x = m[0][3];
		unk108.y = m[1][3];
		unk108.z = m[2][3];
	}

	u32 soundFlags = mMario->mSoundFlags;

	mBodyAnmSound->animeLoop(&mTranslation, mActor->getFrameCtrl(0)->getFrame(),
	                         mActor->getFrameCtrl(0)->getRate(),
	                         soundFlags + 0x10000000, 4);
	mTongueAnmSound->animeLoop(&unkFC, unk5C.getFrame(), unk5C.getRate(),
	                           soundFlags + 0x10000000, 4);
}

void TYoshi::viewCalc()
{
	if (!isHatched())
		return;
	mActor->viewCalc();
	mMirrorModels[0]->viewCalc();
	mMirrorModels[1]->viewCalc();
	mTongue->viewCalc();
}

void TYoshi::entry()
{
	if (!isHatched())
		return;

	bool bVar1 = true;
	if (mState == STATE_UNMOUNTED || mState == STATE_MOUNTED) {
		if (unkC >= 360 && unkC < 600 && !(unkC & 0x10))
			bVar1 = false;

		if (unkC < 360 && !(unkC & 0x8))
			bVar1 = false;
	}

	if (mState == STATE_EGG)
		bVar1 = false;

	if (unkC < 600)
		mType = 0;

	if (!isHatched())
		return;

	if (bVar1 != true)
		return;

	s16 r = (s16)unk84.x;
	s16 g = (s16)unk84.y;
	s16 b = (s16)unk84.z;

	J3DModelData* modelData = mActor->getModel()->getModelData();
	for (u16 i = 0; i < modelData->getMaterialNum(); ++i) {
		J3DGXColorS10 tevColor;
		tevColor.color.r = r;
		tevColor.color.g = g;
		tevColor.color.b = b;
		tevColor.color.a = 0xFF;
		modelData->getMaterialNodePointer(i)->setTevColor(2, &tevColor);
	}

	{
		J3DGXColorS10 tevColor;
		tevColor.color.r = r;
		tevColor.color.g = g;
		tevColor.color.b = b;
		tevColor.color.a = 0xFF;
		mMirrorModels[0]
		    ->getModelData()
		    ->getMaterialNodePointer(0)
		    ->setTevColor(2, &tevColor);
	}

	{
		J3DGXColorS10 tevColor;
		tevColor.color.r = r;
		tevColor.color.g = g;
		tevColor.color.b = b;
		tevColor.color.a = 0xFF;
		mMirrorModels[1]
		    ->getModelData()
		    ->getMaterialNodePointer(0)
		    ->setTevColor(2, &tevColor);
	}

	mActor->entry();
	mMirrorModels[0]->entry();
	mMirrorModels[1]->entry();
	mTongue->entry();

	TCircleShadowRequest shadowRequest;
	shadowRequest.unk0 = mTranslation;
	shadowRequest.unkC = shadowRequest.unk10 = unk114;

	gpBindShadowManager->request(shadowRequest, 0);
	gpQuestionManager->request(mTranslation, unk114);
}
