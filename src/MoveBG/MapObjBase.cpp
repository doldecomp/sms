#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjGeneral.hpp>
#include <System/MarDirector.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TMapObjBase::changeObjMtx(MtxPtr mtx)
{
	mPosition.x = mtx[0][3];
	mPosition.y = mtx[1][3] + mYOffset;
	mPosition.z = mtx[2][3];
	if (mMActor) {
		if (checkMapObjFlag(MAP_OBJ_FLAG_NO_ANIMATIONS)) {
			setModelMtx(mtx);
		} else {
			calcRootMatrix();
			getModel()->calc();
		}
	}
	removeMapCollision();
	setUpCurrentMapCollision();
}

void TMapObjBase::changeObjSRT(const JGeometry::TVec3<f32>& param_1,
                               const JGeometry::TVec3<f32>& param_2,
                               const JGeometry::TVec3<f32>& param_3)
{
	Mtx mtx;
	MsMtxSetTRS(mtx, param_1.x, param_1.y, param_1.z, param_2.x, param_2.y,
	            param_2.z, param_3.x, param_3.y, param_3.z);
	changeObjMtx(mtx);
}

u32 TMapObjBase::getSDLModelFlag() const { return 3; }

void TMapObjBase::awake()
{
	offLiveFlag(LIVE_FLAG_UNK4000);
	offHitFilter(HIT_FILTER_NO_COLLISION);
	setUpCurrentMapCollision();
}

void TMapObjBase::sleep()
{
	onLiveFlag(LIVE_FLAG_UNK4000);
	onHitFilter(HIT_FILTER_NO_COLLISION);
	removeMapCollision();
}

void TMapObjBase::setObjHitData(u16 param_1)
{
	if (!mMapObjData->mHit)
		return;

	if (mMapObjData->mHit->mEntryNum <= param_1)
		return;

	const TMapObjHitDataTable* hitData = &mMapObjData->mHit->mEntries[param_1];

	if (hitData->mAttackRadius >= 0.0f) {
		f32 radiusScale = mScaling.x > mScaling.z ? mScaling.x : mScaling.z;
		f32 heightScale = mScaling.y;
		setHitParams(hitData->mAttackRadius * radiusScale,
		             hitData->mAttackHeight * heightScale,
		             hitData->mDamageRadius * radiusScale,
		             hitData->mDamageHeight * heightScale);
	}
}

void TMapObjBase::removeMapCollision()
{
	if (!mMapCollisionManager)
		return;

	if (mMapCollisionManager->getActiveCollision()
	    && mMapCollisionManager->getActiveCollision()->getKind()
	           != TMapCollisionBase::KIND_STATIC)
		mMapCollisionManager->removeActiveCollision();
}

void TMapObjBase::setUpCurrentMapCollision()
{
	TMapCollisionManager* colman = mMapCollisionManager;
	if (!colman)
		return;

	if (checkMapObjFlag(MAP_OBJ_FLAG_UNK8)) {
		mMapCollisionManager->getActiveCollision()->setUpMtx(
		    getModel()->getAnmMtx(0));
	} else {
		JGeometry::TVec3<f32> pos(mPosition.x, mPosition.y - mYOffset,
		                          mPosition.z);
		colman->setUpActiveCollisionTRS(pos, mRotation, mScaling);
	}
}

void TMapObjBase::setUpMapCollision(u16 param_1)
{
	if (!mMapObjData->mCollision
	    || !mMapObjData->mCollision->mEntries[param_1].mColFileNoExt)
		return;

	JGeometry::TVec3<f32> pos(mPosition.x, mPosition.y - mYOffset, mPosition.z);

	mMapCollisionManager->changeCollision(param_1);

	if (checkMapObjFlag(MAP_OBJ_FLAG_UNK8)) {
		mMapCollisionManager->getActiveCollision()->setUpMtx(
		    getModel()->getAnmMtx(0));
	} else {
		mMapCollisionManager->setUpActiveCollisionTRS(pos, mRotation, mScaling);
	}
}

void TMapObjBase::soundBas(u32 param_1, f32 param_2, f32 param_3)
{
	f32 currFrame = mMActor->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
	if (currFrame <= param_2 && param_2 < currFrame + param_3) {
		SMSGetMSound()->startSoundActor(param_1, &mPosition, 0, nullptr, 0, 4);
	}
}

void TMapObjBase::startSound(u16 index)
{
	if (mActiveSoundIndex != index)
		mActiveSoundIndex = index;

	if (!mMapObjData->mSound) {
		u32 soundId
		    = TMapObjGeneral::mDefaultSound.mSoundIdTable[mActiveSoundIndex];
		if (soundId != 0xffffffff)
			SMSGetMSound()->startSoundActor(soundId, &mPosition, 0, nullptr, 0,
			                                4);
	} else {
		u32 soundId
		    = mMapObjData->mSound->mData->mSoundIdTable[mActiveSoundIndex];
		if (soundId != 0xffffffff)
			SMSGetMSound()->startSoundActor(soundId, &mPosition, 0, nullptr, 0,
			                                4);
	}
}

bool TMapObjBase::hasModelOrAnimData(u16 param_1) const
{
	if (!mMapObjData->mAnim || mMapObjData->mAnim->mEntryNum <= param_1
	    || (!mMapObjData->mAnim->mEntries[param_1].mAnmName
	        && !mMapObjData->mAnim->mEntries[param_1].mBmdFileName)) {
		return false;
	}

	return true;
}

bool TMapObjBase::hasAnim(u16 param_1) const
{
	if (!mMapObjData->mAnim || mMapObjData->mAnim->mEntryNum <= param_1
	    || !mMapObjData->mAnim->mEntries[param_1].mAnmName) {
		return false;
	}

	return true;
}

bool TMapObjBase::animIsFinished() const
{
	if (!mMapObjData->mAnim || mMapObjData->mAnim->mEntryNum == 0
	    || !mMapObjData->mAnim->mEntries[mActiveAnimIndex].mAnmName)
		return true;

	if (mMActor->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
		return true;
	else
		return false;
}

void TMapObjBase::stopAnim() { }

void TMapObjBase::startControlAnim(u16 param_1)
{
	startAnim(param_1);
	if (mMapObjData->mAnim && param_1 < mMapObjData->mAnim->mEntryNum)
		mMActor->getFrameCtrl(mMapObjData->mAnim->mEntries[param_1].mAnmType)
		    ->setRate(0);
}

void TMapObjBase::startBck(const char* param_1)
{
	offMapObjFlag(MAP_OBJ_FLAG_NO_ANIMATIONS);
	mMActor->setBck(param_1);
}

void TMapObjBase::startAnim(u16 index)
{
	if (mAnmSound)
		setAnmSound(nullptr);

	if (!mMActor) {
		const TMapObjAnimDataInfo* anim = mMapObjData->mAnim;
		if (anim->mEntryNum != 0)
			mMActor = mMActorKeeper->getMActor(anim->mEntries[0].mBmdFileName);
	}

	const TMapObjAnimDataInfo* anim = mMapObjData->mAnim;
	if (!anim || anim->mEntryNum <= index)
		return;

	const TMapObjAnimData* data = &anim->mEntries[index];
	if (data->mAnmType == 0) {
		if (mActiveAnimIndex != 0xffff && anim && anim->mEntryNum != 0) {
			const TMapObjAnimData* d2 = &anim->mEntries[mActiveAnimIndex];
			if (d2->mAnmName != nullptr) {
				u8 anmType = d2->mAnmType;
				mMActor->getFrameCtrl(anmType)->setRate(0.0f);
				mMActor->getFrameCtrl(anmType)->setFrame(0.0f);
				mMActor->getUnk28(anmType)->unk0 = 0xffffffff;
				mActiveAnimIndex                 = 0xffff;
			}
		}
		stopAnmSound();
		if (mActiveAnimIndex != index) {
			mActiveAnimIndex = index;
			if (data->mBmdFileName)
				mMActor = mMActorKeeper->getMActor(data->mBmdFileName);
		}
	}

	if (data->mAnmName) {
		offMapObjFlag(MAP_OBJ_FLAG_NO_ANIMATIONS);
		mMActor->setAnimation(data->mAnmName, data->mAnmType);
		if (checkMapObjFlag(MAP_OBJ_FLAG_TURN_OFF_ANIMATIONS_AT_END))
			offMapObjFlag(MAP_OBJ_FLAG_NO_ANIMATIONS);
		if (data->mBasFilePath)
			setAnmSound(data->mBasFilePath);
	} else {
		MActor* actor = mMActor;
		actor->getModel()->getModelData()->getJointNodePointer(0)->setMtxCalc(
		    actor->getMtxCalc());
	}
}

void TMapObjBase::makeObjDefault()
{
	mPosition.set(mInitialPosition.x, mInitialPosition.y + mYOffset,
	              mInitialPosition.z);

	mRotation = mInitialRotation;
	mScaling  = mInitialScaling;

	mVelocity.zero();
	onLiveFlag(LIVE_FLAG_UNK10);
	if (mMActor) {
		calcRootMatrix();
		getModel()->calc();
	}
	mGroundHeight = gpMap->checkGround(mPosition, &mGroundPlane);
}

void TMapObjBase::makeObjDead()
{
	mVelocity.zero();
	onLiveFlag(LIVE_FLAG_UNK10);

	if (mActiveAnimIndex != 0xffff && mMapObjData->mAnim
	    && mMapObjData->mAnim->mEntryNum > 0
	    && mMapObjData->mAnim->mEntries[mActiveAnimIndex].mAnmName) {
		u32 anmType = mMapObjData->mAnim->mEntries[mActiveAnimIndex].mAnmType;
		mMActor->getFrameCtrl(anmType)->setRate(0.0f);
		mMActor->getFrameCtrl(anmType)->setFrame(0.0f);
		mMActor->getUnk28(anmType)->unk0 = 0xffffffff;
		mActiveAnimIndex                 = 0xffff;
	}

	mActiveSoundIndex = 0xffff;
	onHitFilter(HIT_FILTER_NO_COLLISION);
	removeMapCollision();
	mStateTimer = 0;
	if (mHeldObject) {
		mHeldObject->receiveMessage(this, HIT_MESSAGE_DETACH);
		mHeldObject = nullptr;
	}

	if (mHolder) {
		mHolder->receiveMessage(this, HIT_MESSAGE_DETACH);
		mHolder = nullptr;
	}

	onLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_UNK8 | LIVE_FLAG_UNK10
	           | LIVE_FLAG_UNK40 | LIVE_FLAG_AIRBORNE);
	mState = STATE_DEAD;
	if (mMActor)
		SMS_HideAllShapePacket(getModel());
}

void TMapObjBase::makeObjAppeared()
{
	offLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_UNK8);
	mVelocity.zero();
	onLiveFlag(LIVE_FLAG_UNK10);
	mStateTimer = 0;
	offHitFilter(HIT_FILTER_NO_COLLISION);
	setObjHitData(0);
	startSound(0);

	mGroundHeight = gpMap->checkGround(mPosition, &mGroundPlane);
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		onLiveFlag(LIVE_FLAG_AIRBORNE);

	if (mMapObjData->mMove) {
		J3DFrameCtrl* ctrl = mMapObjData->mMove->mFrameCtrl;
		ctrl->setFrame((f32)ctrl->getStart());
		ctrl->setRate(1.0f);

		mMapObjData->mMove->mFrameCtrl->setRate(SMSGetAnmFrameRate());
	}

	startAnim(0);
	if (mMActor)
		SMS_ShowAllShapePacket(getModel());

	mPosition.y -= mYOffset;
	setUpMapCollision(0);
	mPosition.y += mYOffset;
	mState = STATE_NORMAL;
}

void TMapObjBase::moveByBck() { }

void TMapObjBase::touchBoss(THitActor* boss)
{
	boss->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

void TMapObjBase::touchEnemy(THitActor* enemy)
{
	enemy->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

void TMapObjBase::touchPlayer(THitActor* player)
{
	player->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

void TMapObjBase::touchActor(THitActor* actor)
{
	if (actor->isHitCategory(HIT_CATEGORY_PLAYER))
		touchPlayer(actor);
	else if (actor->isHitCategory(HIT_CATEGORY_ENEMY))
		touchEnemy(actor);
	else if (actor->isHitCategory(HIT_CATEGORY_BOSS))
		touchBoss(actor);
}

void TMapObjBase::ensureTakeSituation()
{
	if (mHeldObject && mHeldObject->getHolder() != this)
		mHeldObject = nullptr;

	if (mHolder && mHolder->getHeldObject() != this) {
		if (mPosition.y != mGroundHeight)
			offLiveFlag(LIVE_FLAG_UNK10);

		mHolder = nullptr;
	}
}

void TMapObjBase::control()
{
	for (int i = 0; i < mColCount; ++i)
		touchActor(getCollision(i));

	if (mMapObjData->mMove) {
		TMapObjMoveData* move = mMapObjData->mMove;

		move->mBckAnm->setFrame(move->mFrameCtrl->getFrame());
		move->mFrameCtrl->update();
		J3DTransformInfo info;
		move->mBckAnm->getTransform(1, &info);
		mPositionDelta.x = info.mTranslate.x + mInitialPosition.x - mPosition.x;
		mPositionDelta.y = info.mTranslate.y + mInitialPosition.y - mPosition.y;
		mPositionDelta.z = info.mTranslate.z + mInitialPosition.z - mPosition.z;
		mRotation.x
		    = info.mRotation.x * (360.0f / 65536.0f) + mInitialRotation.x;
		mRotation.y
		    = info.mRotation.y * (360.0f / 65536.0f) + mInitialRotation.y;
		mRotation.z
		    = info.mRotation.z * (360.0f / 65536.0f) + mInitialRotation.z;
	}
}

void TMapObjBase::setGroundCollision()
{
	if (!mMapCollisionManager)
		return;
	switch (mMapCollisionManager->getActiveCollision()->getKind()) {
	case TMapCollisionBase::KIND_MOVE:
		if (checkMapObjFlag(MAP_OBJ_FLAG_MOVE_COLLISION_ON_CONTACT)) {
			if (mColCount == 0 && mMoveCollisionOnContactGraceTimer == 0)
				return;
			--mMoveCollisionOnContactGraceTimer;
			if (mColCount != 0)
				mMoveCollisionOnContactGraceTimer
				    = MOVE_COLLISION_ON_CONTACT_GRACE_TIMER;
		}

		if (checkMapObjFlag(MAP_OBJ_FLAG_UNK8)) {
			MtxPtr mtx = getModel()->getAnmMtx(0);
			mMapCollisionManager->moveActiveCollisionMtx(mtx);
		} else {
			JGeometry::TVec3<f32> pos(mPosition.x, mPosition.y - mYOffset,
			                          mPosition.z);
			if (checkMapObjFlag(MAP_OBJ_FLAG_SCALE_AND_ROTATE_COLLISION)) {
				mMapCollisionManager->getActiveCollision()->offFlag(
				    TMapCollisionBase::FLAG_UNK8000);
				mMapCollisionManager->getActiveCollision()->offFlag(
				    TMapCollisionBase::FLAG_UNK4000);
				mMapCollisionManager->moveActiveCollisionSRT(pos, mRotation,
				                                             mScaling);
			} else {
				mMapCollisionManager->getActiveCollision()->offFlag(
				    TMapCollisionBase::FLAG_UNK4000);
				mMapCollisionManager->moveActiveCollisionTrans(pos);
			}
		}
		break;
	}
}

void TMapObjBase::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (SMSGetMarDirector()->isTalkModeNow()
	    && !SMSGetMarDirector()->isDemoModeNow()) {
		if (checkLiveFlag(LIVE_FLAG_DEAD)
		    || isActorType(ACTOR_TYPE_MAP_OBJ_TREE_SCALE))
			return;

		if (cue & CUE_MOVE) {
			setGroundCollision();
			cue &= ~CUE_MOVE;
		}

		if ((cue & CUE_CALC_ANIM) && mMActor) {
			if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT | LIVE_FLAG_UNK200)) {
				if (getModel()->getShapePacket(0)->isVisible())
					SMS_HideAllShapePacket(getModel());
			} else {
				if (!getModel()->getShapePacket(0)->isVisible())
					SMS_ShowAllShapePacket(getModel());
			}
		}

		if (checkLiveFlag(LIVE_FLAG_UNK200))
			return;

		if (hasMapCollision())
			cue &= ~CUE_CALC_ANIM;
	}

	if (cue & CUE_MOVE) {
		if (isStateTimerEngaged())
			--mStateTimer;

		if (mActiveSoundIndex == 0)
			startSound(mActiveSoundIndex);
		if (checkLiveFlag(LIVE_FLAG_DEAD))
			dead();
	}

	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	if (cue & CUE_DRAW)
		draw();

	if (checkLiveFlag(LIVE_FLAG_UNK4000)) {
		if (cue & CUE_CALC_ANIM)
			cue &= ~CUE_CALC_ANIM;
		if (cue & CUE_ENTRY)
			cue &= ~CUE_ENTRY;
		if (cue & CUE_CALC_VIEW)
			cue &= ~CUE_CALC_VIEW;
	}

	if (cue & CUE_CALC_ANIM) {
		calc();
		if (mMActor) {
			if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT | LIVE_FLAG_UNK200)) {
				if (getModel()->getShapePacket(0)->isVisible())
					SMS_HideAllShapePacket(getModel());
			} else {
				if (!getModel()->getShapePacket(0)->isVisible())
					SMS_ShowAllShapePacket(getModel());
			}
		}
		if (checkMapObjFlag(MAP_OBJ_FLAG_NO_ANIMATIONS)) {
			cue &= ~CUE_CALC_ANIM;
		} else if (checkMapObjFlag(MAP_OBJ_FLAG_TURN_OFF_ANIMATIONS_AT_END)) {
			if (mMActor && mMActor->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
				onMapObjFlag(MAP_OBJ_FLAG_NO_ANIMATIONS);
		}
	}

	if (cue & CUE_ENTRY) {
		if (checkMapObjFlag(MAP_OBJ_FLAG_UNK1000))
			cue &= ~CUE_ENTRY;
		if (checkMapObjFlag(MAP_OBJ_FLAG_STATIC_DRAW))
			cue &= ~CUE_ENTRY;
	}

	if ((cue & CUE_CALC_VIEW) && mMActor
	    && checkMapObjFlag(MAP_OBJ_FLAG_USE_SIMPLE_VIEW_CALC)) {
		static_cast<SDLModel*>(getModel())->viewCalcSimple();
		cue &= ~CUE_CALC_VIEW;
		requestShadow();
	}

	TLiveActor::perform(cue, graphics);
}

u32 TMapObjBase::getShadowType()
{
	if (isActorType(ACTOR_TYPE_PALM_NORMAL) || isActorType(ACTOR_TYPE_PALM_OUGI)
	    || isActorType(ACTOR_TYPE_PALM_SAGO)
	    || isActorType(ACTOR_TYPE_PALM_NATUME)
	    || isActorType(ACTOR_TYPE_BANANA_TREE)) {
		return SHADOW_TYPE_TREE;
	} else if (checkMapObjFlag(MAP_OBJ_FLAG_SQUARE_SHADOW)) {
		return SHADOW_TYPE_SQUARE;
	} else {
		return SHADOW_TYPE_CIRCLE;
	}
}

Mtx* TMapObjBase::getRootJointMtx() const
{
	if (checkMapObjFlag(MAP_OBJ_FLAG_UNK8) || mYOffset != 0.0f)
		return (Mtx*)mMActor->getModel()->getAnmMtx(0);
	return nullptr;
}

void TMapObjBase::calcRootMatrix()
{
	J3DModel* model = getModel();
	MsMtxSetXYZRPH(model->getBaseTRMtx(), mPosition.x, mPosition.y - mYOffset,
	               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
	model->setBaseScale(mScaling);
}

BOOL TMapObjBase::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_ATTACH
	    && checkMapObjFlag(MAP_OBJ_FLAG_CLIMBABLE)) {
		mHeldObject = (TTakeActor*)sender;
		return true;
	}

	if (message == HIT_MESSAGE_SPRAYED_BY_WATER)
		return touchWater(sender);

	return false;
}

void TMapObjBase::initAndRegister(const char* instance_name)
{
	mIndividualName = instance_name;
	initMapObj();
	if (mMapObjData->mIdxGroupName) {
		TIdxGroupObj* group = static_cast<TIdxGroupObj*>(
		    JDrama::TNameRefGen::search(mMapObjData->mIdxGroupName));
		group->getChildren().push_back(this);
	}
}

void TMapObjBase::loadAfter()
{
	TLiveActor::loadAfter();
	mGroundHeight = gpMap->checkGround(mPosition, &mGroundPlane);
}

void TMapObjBase::load(JSUMemoryInputStream& stream)
{
	TActor::load(stream);

	mIndividualName = stream.readString();
	loadBeforeInit(stream);
	initMapObj();
	offMapObjFlag(MAP_OBJ_FLAG_DISAPPEARING);
	makeObjAppeared();
	if (checkMapObjFlag(MAP_OBJ_FLAG_LOAD_HIT_HEIGHT)) {
		f32 value;
		stream >> value;
		setDamageHeight(value);
		setAttackHeight(value);
		offHitFilter(HIT_FILTER_NO_DAMAGE);
		offHitFilter(HIT_FILTER_NO_ATTACK);
	}
}

TMapObjBase::TMapObjBase(const char* name)
    : TLiveActor(name)
    , mIndividualName(nullptr)
    , mMapObjFlags(0)
    , mState(STATE_NORMAL)
    , mActiveAnimIndex(0xffff)
    , mActiveSoundIndex(0xffff)
    , mMoveCollisionOnContactGraceTimer(0)
    , mStateTimer(0)
    , mYOffset(0.0f)
    , mMapObjData(nullptr)
    , mEventId(0)
{
	mInitialPosition.zero();
	mInitialRotation.zero();
	mInitialScaling.set(1.0f, 1.0f, 1.0f);
}
