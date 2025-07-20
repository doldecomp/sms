#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjGeneral.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TMapObjBase::changeObjMtx(MtxPtr mtx)
{
	mPosition.x = mtx[3][0];
	mPosition.y = mtx[3][1] + unk108;
	mPosition.z = mtx[3][2];
	if (mMActor) {
		if (checkMapObjFlag(0x100)) {
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
	offLiveFlag(0x4000);
	unk64 &= ~1;
	setUpCurrentMapCollision();
}

void TMapObjBase::sleep()
{
	onLiveFlag(0x4000);
	unk64 |= 1;
	removeMapCollision();
}

void TMapObjBase::setObjHitData(u16 param_1)
{
	if (!unk130->mHit)
		return;

	if (unk130->mHit->unk0 <= param_1)
		return;

	const TMapObjHitDataTable* table = &unk130->mHit->unkC[param_1];

	if (table->unk0 >= 0.0f) {
		f32 fVar2     = mScaling.x > mScaling.z ? mScaling.x : mScaling.z;
		f32 fVar3     = mScaling.y;
		mAttackRadius = table->unk0 * fVar2;
		mAttackHeight = table->unk4 * fVar3;
		mDamageRadius = table->unk8 * fVar2;
		mDamageHeight = table->unkC * fVar3;
		calcEntryRadius();
	}
}

void TMapObjBase::removeMapCollision()
{
	if (!unkEC)
		return;

	if (!unkEC->unk8)
		return;

	if (unkEC->unk8 && unkEC)
		unkEC->unk8->remove();
}

void TMapObjBase::setUpCurrentMapCollision()
{
	TMapCollisionManager* colman = unkEC;
	if (!colman)
		return;

	if (checkMapObjFlag(8)) {
		J3DModel* model        = getModel();
		TMapCollisionBase* col = unkEC->getUnk8();
		MTXCopy(model->getAnmMtx(0), col->unk20);
		col->setUp();
	} else {
		Mtx mtx;
		JGeometry::TVec3<f32> pos(mPosition.x, mPosition.y - unk108,
		                          mPosition.z);
		MsMtxSetTRS(mtx, pos.x, pos.y, pos.z, mRotation.x, mRotation.y,
		            mRotation.z, mScaling.x, mScaling.y, mScaling.z);
		TMapCollisionBase* col = colman->getUnk8();
		MTXCopy(mtx, col->unk20);
		col->setUp();
	}
}

void TMapObjBase::setUpMapCollision(u16 param_1)
{
	if (!unk130->mCollision || !unk130->mCollision->unk4[param_1].unk0)
		return;

	JGeometry::TVec3<f32> pos(mPosition.x, mPosition.y - unk108, mPosition.z);

	unkEC->changeCollision(param_1);

	if (checkMapObjFlag(8)) {
		J3DModel* model        = getModel();
		TMapCollisionBase* col = unkEC->getUnk8();
		MTXCopy(model->getAnmMtx(0), col->unk20);
		col->setUp();
	} else {
		Mtx mtx;
		TMapCollisionManager* colman = unkEC;
		MsMtxSetTRS(mtx, pos.x, pos.y, pos.z, mRotation.x, mRotation.y,
		            mRotation.z, mScaling.x, mScaling.y, mScaling.z);
		TMapCollisionBase* col = colman->getUnk8();
		MTXCopy(mtx, col->unk20);
		col->setUp();
	}
}

void TMapObjBase::soundBas(u32 param_1, f32 param_2, f32 param_3)
{
	f32 currFrame = mMActor->getFrameCtrl(0)->getCurrentFrame();
	if (currFrame <= param_2 && param_2 < currFrame + param_3) {
		if (gpMSound->gateCheck(param_1))
			MSoundSESystem::MSoundSE::startSoundActor(param_1, getPosition(), 0,
			                                          nullptr, 0, 4);
	}
}

void TMapObjBase::startSound(u16 param_1)
{
	if (unk100 != param_1)
		unk100 = param_1;

	if (!unk130->mSound) {
		u32 uVar3 = TMapObjGeneral::mDefaultSound.unk0[unk100];
		if (uVar3 != 0xffffffff && gpMSound->gateCheck(uVar3))
			MSoundSESystem::MSoundSE::startSoundActor(uVar3, mPosition, 0,
			                                          nullptr, 0, 4);
	} else {
		u32 uVar3 = unk130->mSound->unk4->unk0[unk100];
		if (uVar3 != 0xffffffff && gpMSound->gateCheck(uVar3))
			MSoundSESystem::MSoundSE::startSoundActor(uVar3, mPosition, 0,
			                                          nullptr, 0, 4);
	}
}

bool TMapObjBase::hasModelOrAnimData(u16 param_1) const
{
	if (!unk130->mAnim || unk130->mAnim->unk0 <= param_1
	    || (!unk130->mAnim->unk4[param_1].unk4
	        && !unk130->mAnim->unk4[param_1].unk0)) {
		return false;
	}

	return true;
}

bool TMapObjBase::hasAnim(u16 param_1) const
{
	if (!unk130->mAnim || unk130->mAnim->unk0 <= param_1
	    || !unk130->mAnim->unk4[param_1].unk4) {
		return false;
	}

	return true;
}

bool TMapObjBase::animIsFinished() const
{
	if (!unk130->mAnim || unk130->mAnim->unk0 == 0
	    || !unk130->mAnim->unk4[unkFE].unk4)
		return true;

	if (mMActor->curAnmEndsNext(0, nullptr))
		return true;
	else
		return false;
}

void TMapObjBase::stopAnim() { }

void TMapObjBase::startControlAnim(u16 param_1)
{
	startAnim(param_1);
	if (unk130->mAnim && param_1 < unk130->mAnim->unk0)
		mMActor->getFrameCtrl(unk130->mAnim->unk4[param_1].unk8)->setSpeed(0);
}

void TMapObjBase::startBck(const char* param_1)
{
	unkF8 &= ~0x100;
	mMActor->setBck(param_1);
}

void TMapObjBase::startAnim(u16 param_1) { }

void TMapObjBase::makeObjDefault()
{
	mPosition.x = unk10C.x;
	mPosition.y = unk10C.y + unk108;
	mPosition.z = unk10C.z;

	mRotation = unk118;
	mScaling  = unk124;

	unkAC.x = unkAC.y = unkAC.z = 0.0f;
	onLiveFlag(0x10);
	if (mMActor) {
		calcRootMatrix();
		getModel()->calc();
	}
	unkC8 = gpMap->checkGround(mPosition, &unkC4);
}

void TMapObjBase::makeObjDead()
{
	unkAC.x = unkAC.y = unkAC.z = 0.0f;
	mLiveFlag |= 0x10;

	if (unkFE != 0xffff && unk130->mAnim && unk130->mAnim->unk0 > 0
	    && unk130->mAnim->unk4[unkFE].unk4) {
		u32 uVar6 = unk130->mAnim->unk4[unkFE].unk8;
		mMActor->getFrameCtrl(uVar6)->setSpeed(0.0f);
		mMActor->getFrameCtrl(uVar6)->setFrame(0.0f);
		mMActor->getUnk28(uVar6)->unk0 = 0xffffffff;
		unkFE                          = 0xffff;
	}

	unk100 = 0xffff;
	unk64 |= 1;
	removeMapCollision();
	unk104 = 0;
	if (mHeldObject) {
		mHeldObject->receiveMessage(this, 0x8);
		mHeldObject = nullptr;
	}

	if (mHolder) {
		mHolder->receiveMessage(this, 0x8);
		mHolder = nullptr;
	}

	onLiveFlag(0xD9);
	mState = 0;
	if (mMActor)
		SMS_HideAllShapePacket(getModel());
}

void TMapObjBase::makeObjAppeared() { }

void TMapObjBase::moveByBck() { }

void TMapObjBase::touchBoss(THitActor* boss)
{
	boss->receiveMessage(this, 0xE);
}

void TMapObjBase::touchEnemy(THitActor* enemy)
{
	enemy->receiveMessage(this, 0xE);
}

void TMapObjBase::touchPlayer(THitActor* player)
{
	player->receiveMessage(this, 0xE);
}

void TMapObjBase::touchActor(THitActor* actor)
{
	if (actor->checkActorType(ACTOR_TYPE_PLAYER))
		touchPlayer(actor);
	else if (actor->checkActorType(ACTOR_TYPE_ENEMY))
		touchEnemy(actor);
	else if (actor->checkActorType(ACTOR_TYPE_BOSS))
		touchBoss(actor);
}

void TMapObjBase::ensureTakeSituation()
{
	if (mHeldObject && mHeldObject->mHolder != this)
		mHeldObject = nullptr;

	if (mHolder && mHolder->mHeldObject != this) {
		if (mPosition.y != unkC8)
			mLiveFlag &= ~0x10;

		mHolder = nullptr;
	}
}

void TMapObjBase::control()
{
	for (int i = 0; i < mColCount; ++i)
		touchActor(getCollision(i));

	if (unk130->mMove) {
		TMapObjMoveData* move = unk130->mMove;

		move->unk4->setFrame(move->unk8->getCurrentFrame());
		move->unk8->update();
		J3DTransformInfo info;
		move->unk4->getTransform(1, &info);
		unk94.x     = info.mTranslate.x + unk10C.x - mPosition.x;
		unk94.y     = info.mTranslate.y + unk10C.y - mPosition.y;
		unk94.z     = info.mTranslate.z + unk10C.z - mPosition.z;
		mRotation.x = info.mRotation.x * (360.0f / 65536.0f) + unk118.x;
		mRotation.y = info.mRotation.y * (360.0f / 65536.0f) + unk118.y;
		mRotation.z = info.mRotation.z * (360.0f / 65536.0f) + unk118.z;
	}
}

void TMapObjBase::setGroundCollision() { }

void TMapObjBase::perform(u32, JDrama::TGraphics*) { }

u32 TMapObjBase::getShadowType()
{
	if (isActorType(0x40000034) || isActorType(0x40000035)
	    || isActorType(0x40000036) || isActorType(0x40000037)
	    || isActorType(0x40000039)) {
		return 2;
	} else if (checkMapObjFlag(0x2000)) {
		return 1;
	} else {
		return 0;
	}
}

Mtx* TMapObjBase::getRootJointMtx() const
{
	if (checkMapObjFlag(0x8) || unk108 != 0.0f)
		return (Mtx*)mMActor->getUnk4()->getAnmMtx(0);
	return nullptr;
}

void TMapObjBase::calcRootMatrix()
{
	J3DModel* model                  = getModel();
	const JGeometry::TVec3<f32>& pos = getPosition();
	const JGeometry::TVec3<f32>& rot = getRotation();
	MsMtxSetXYZRPH(model->unk20, pos.x, pos.y - unk108, pos.z,
	               rot.x * (65536.0f / 360.0f), rot.y * (65536.0f / 360.0f),
	               rot.z * (65536.0f / 360.0f));
	model->unk14 = mScaling;
}

BOOL TMapObjBase::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == 5 && checkMapObjFlag(0x40)) {
		mHeldObject = (TTakeActor*)param_1;
		return 1;
	} else if (param_2 == 0xF) {
		return touchWater(param_1);
	} else {
		return false;
	}
}

void TMapObjBase::initAndRegister(const char* param_1)
{
	unkF4 = param_1;
	initMapObj();
	if (unk130->unkC) {
		JDrama::TNameRefGen::search<TIdxGroupObj>(unk130->unkC)
		    ->push_back(this);
	}
}

void TMapObjBase::loadAfter()
{
	TLiveActor::loadAfter();
	unkC8 = gpMap->checkGround(mPosition, &unkC4);
}

void TMapObjBase::load(JSUMemoryInputStream& stream)
{
	TActor::load(stream);

	unkF4 = stream.readString();
	loadBeforeInit(stream);
	initMapObj();
	unkF8 &= ~0x40000;
	makeObjAppeared();
	if (checkMapObjFlag(0x80)) {
		f32 value;
		stream.read(&value, 4);
		mDamageHeight = value;
		calcEntryRadius();
		mAttackHeight = value;
		calcEntryRadius();
		unk64 &= ~4;
		unk64 &= ~2;
	}
}

TMapObjBase::TMapObjBase(const char* name)
    : TLiveActor(name)
    , unkF4(nullptr)
    , unkF8(0)
    , mState(1)
    , unkFE(0xffff)
    , unk100(0xffff)
    , unk102(0)
    , unk104(0)
    , unk108(0.0f)
    , unk130(nullptr)
    , unk134(0)
{
	unk10C.x = unk10C.y = unk10C.z = 0.0f;
	unk118.x = unk118.y = unk118.z = 0.0f;

	unk124.x = 1.0f;
	unk124.y = 1.0f;
	unk124.z = 1.0f;
}
