#include <Strategic/LiveActor.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/question.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Binder.hpp>
#include <System/MarDirector.hpp>
#include <MarioUtil/MtxUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/LodAnm.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <Map/MapData.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Enemy/Conductor.hpp>
#include <MSound/MAnmSound.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

f32 TLiveActor::mVelocityMinY = -40.0f;

TLiveActor::TLiveActor(const char* name)
    : TTakeActor(name)
{
	mManager       = nullptr;
	mMActor        = nullptr;
	mMActorKeeper  = nullptr;
	mInstanceIndex = 0;
	mAnmSound      = nullptr;
	mAnmSoundPath  = nullptr;
	mBinder        = nullptr;
	mSpine         = nullptr;
	unk90          = nullptr;

	mLinearVelocity.zero();
	mAngularVelocity.zero();

	mVelocity.set(0.0f, 0.0f, 0.0f);

	mScaledBodyRadius    = 10.0f;
	mBodyRadius          = 25.0f;
	mHeadHeight          = 50.0f;
	mGroundPlane         = nullptr;
	mGroundHeight        = 0.0f;
	mGravity             = 0.15f;
	unkD0                = nullptr;
	mGroundActor         = nullptr;
	mGroundActorYaw      = 0.0f;
	unkE8                = 1;
	mMapCollisionManager = nullptr;
	mLiveFlag            = 0x100;

	mRidePos.zero();

	mGroundPlane = TMap::getIllegalCheckData();
	if (gpMarDirector->mMap != 8)
		mLiveFlag |= 0x2000;
}

TLiveActor::~TLiveActor() { }

void TLiveActor::calcRidePos()
{
	if (!mGroundActor)
		return;

	Mtx mtx;
	if (!mGroundActor->getRootJointMtx())
		SMS_GetActorMtx(*mGroundActor, mtx);
	else
		MTXCopy(*mGroundActor->getRootJointMtx(), mtx);
	MTXInverse(mtx, mtx);
	MTXMultVec(mtx, &mPosition, &mRidePos);
}

BOOL TLiveActor::belongToGround() const
{
	if (mGroundPlane && mGroundPlane->isLegal()
	    && mGroundPlane->getActor() != nullptr && !isAirborne())
		return true;

	return false;
}

void TLiveActor::calcRideMomentum()
{
	if (unkE8 == 0)
		return;

	if (belongToGround()) {
		if (mGroundActor == nullptr
		    || mGroundActor != mGroundPlane->getActor()) {
			mGroundActor = mGroundPlane->getActor();
			calcRidePos();

			if (unkE8 >= 2 && mGroundActor != nullptr)
				mGroundActorYaw = mGroundActor->mRotation.y;
		} else {
			Mtx mtx;
			if (!mGroundActor->getRootJointMtx())
				SMS_GetActorMtx(*mGroundActor, mtx);
			else
				MTXCopy(*mGroundActor->getRootJointMtx(), mtx);

			// Did they call this function calcRideMomentum because they
			// didn't know the difference between momentum and velocity?
			JGeometry::TVec3<f32> rideVelocity;
			// mRidePos is from last frame here
			MTXMultVec(mtx, &mRidePos, &rideVelocity);
			rideVelocity -= mPosition;
			mLinearVelocity += rideVelocity;

			if (unkE8 >= 2) {
				mAngularVelocity.y
				    += MsAngleDiff(mGroundActor->mRotation.y, mGroundActorYaw);
				mGroundActorYaw = mGroundActor->mRotation.y;
			}
		}
	} else {
		mGroundActor = nullptr;
	}
}

J3DModel* TLiveActor::getModel() const { return mMActor->unk4; }

Mtx* TLiveActor::getRootJointMtx() const { return nullptr; }

void TLiveActor::initLodAnm(const TLodAnmIndex* param_1, int param_2,
                            f32 param_3)
{
	if (!unkD0)
		unkD0 = new TLodAnm(this, param_1, param_2, param_3);
}

void TLiveActor::init(TLiveManager* manager)
{
	if (!manager) {
		if (TObjChara* chara = (TObjChara*)unk3C) {
			mMActorKeeper = new TMActorKeeper(nullptr, 1);
			// TODO: could be TSMSSmplChara instead
			mMActor = mMActorKeeper->createMActorFromDefaultBmd(
			    chara->getFolder(), 0);
		}
		gpConductor->registerAloneActor(this);
	} else {
		mManager      = manager;
		mMActorKeeper = new TMActorKeeper(mManager, 1);
		mManager->manageActor(this);
		mMActor = mMActorKeeper->createMActorFromNthData(0, 0);
	}

	initHitActor(0, 1, 0, mBodyRadius, mHeadHeight, mBodyRadius, mHeadHeight);

	onHitFlag(HIT_FLAG_NO_COLLISION);
	offLiveFlag(LIVE_FLAG_UNK400);

	if (!mAnmSound)
		initAnmSound();
}

void TLiveActor::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);

	char buffer[256];
	stream.readString(buffer, 256);
	TLiveManager* mgr = JDrama::TNameRefGen::search<TLiveManager>(buffer);

	mGroundPlane = TMap::getIllegalCheckData();

	init(mgr);
}

void TLiveActor::bind()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	if (mBinder != nullptr) {
		mBinder->bind(this);
		return;
	}

	JGeometry::TVec3<f32> nextPos = mPosition;
	nextPos += mLinearVelocity;
	nextPos += mVelocity;

	// Apply gravity & air resistance
	mVelocity.y -= getGravityY();
	if (mVelocity.y < -40.0f)
		mVelocity.y = -40.0f;

	// Handle vertical collisions
	{
		if (checkLiveFlag(LIVE_FLAG_UNK1000)) {
			mGroundHeight = gpMap->checkGroundIgnoreWaterSurface(
			    nextPos.x, nextPos.y + mHeadHeight, nextPos.z, &mGroundPlane);
		} else {
			mGroundHeight = gpMap->checkGround(
			    nextPos.x, nextPos.y + mHeadHeight, nextPos.z, &mGroundPlane);
		}
		mGroundHeight += 1.0f;

		// Will we hit the ground next frame?
		if (nextPos.y <= mGroundHeight + 0.05f) {
			if (mGroundPlane->checkFlag(BG_CHECK_FLAG_ILLEGAL))
				kill();
			offLiveFlag(LIVE_FLAG_AIRBORNE);
			mVelocity.set(0.0f, 0.0f, 0.0f);
			nextPos.y = mGroundHeight;
		} else {
			onLiveFlag(LIVE_FLAG_AIRBORNE);
		}
	}

	// Handle horizontal collisions
	gpMap->isTouchedOneWallAndMoveXZ(&nextPos.x, nextPos.y + mHeadHeight,
	                                 &nextPos.z, mBodyRadius);

	// We're done, this is the displacement for this frame
	mLinearVelocity = nextPos - mPosition;
}

void TLiveActor::control()
{
	// TODO: what is unk90???
	if (unk90 == nullptr || *(int*)((char*)unk90 + 4) == 0) {
		if (mSpine)
			mSpine->update();
	} else {
		if (!mSpine) {
			if (unk90 && *(int*)((char*)unk90 + 4) != 0) {
				// call on unk90
			}
		} else if (mSpine->isIdle()) {
			// call on unk90
		} else {
			mSpine->update();
		}
	}
}

void TLiveActor::calcRootMatrix()
{
	J3DModel* model = mMActor->getModel();
	MsMtxSetXYZRPH(model->getBaseTRMtx(), mPosition.x, mPosition.y, mPosition.z,
	               mRotation.x, mRotation.y, mRotation.z);
	model->setBaseScale(mScaling);
}

void TLiveActor::kill()
{
	mLiveFlag |= 0x1;
	mLiveFlag |= 0x40;
}

BOOL TLiveActor::receiveMessage(THitActor*, u32) { return FALSE; }

u32 TLiveActor::getShadowType() { return 0; }

void TLiveActor::setGroundCollision()
{
	if (!mMapCollisionManager)
		return;
	if (!mMapCollisionManager->unk8)
		return;

	mMapCollisionManager->unk8->moveSRT(mPosition, mRotation, mScaling);
}

void TLiveActor::moveObject()
{
	ensureTakeSituation();

	mLinearVelocity.zero();
	mAngularVelocity.zero();

	control();
	calcRideMomentum();
	bind();

	mPosition += mLinearVelocity;
	mRotation += mAngularVelocity;

	setGroundCollision();
	calcRidePos();
}

void TLiveActor::requestShadow()
{
	if (mLiveFlag & 0xB)
		return;

	if (!(mLiveFlag & 0x204) || (mLiveFlag & 0x400)) {
		TCircleShadowRequest local_2c;

		local_2c.unk0 = mPosition;

		if (!(mLiveFlag & 0x80 ? 0 : 1)) {
			local_2c.unk0.y = mGroundHeight;
			local_2c.unk1D  = 0;
		}

		local_2c.unk10 = local_2c.unkC = mScaledBodyRadius;

		local_2c.unk1C = getShadowType();
		local_2c.unk14 = mRotation.y;

		if (mLiveFlag & 0x400) {
			gpBindShadowManager->forceRequest(local_2c, getActorType());
		} else {
			gpBindShadowManager->request(local_2c, getActorType());
		}
	}

	if (!(mLiveFlag & 0x204) && !checkActorType(0x40000000)) {
		gpQuestionManager->request(mPosition, mScaledBodyRadius);
	}
}

void TLiveActor::drawObject(JDrama::TGraphics*)
{
	if (mLiveFlag & 3 || !mMActor)
		return;

	mMActor->setLightData(mGroundPlane, mPosition);
	mMActor->entry();
}

void TLiveActor::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (mLiveFlag & 0x201)
		return;

	if (param_1 & 1)
		moveObject();

	if (param_1 & 2)
		updateAnmSound();

	if (mMActor) {
		if (param_1 & 2)
			mMActor->frameUpdate();

		if (param_1 & 4)
			requestShadow();

		if (!(mLiveFlag & 6)) {
			if (param_1 & 2) {
				calcRootMatrix();
				mMActor->calc();
			}

			if (param_1 & 4)
				mMActor->viewCalc();

			if (param_1 & 0x200)
				drawObject(param_2);
		}
	}
}

void TLiveActor::performOnlyDraw(u32 param_1, JDrama::TGraphics* param_2)
{
	if (mLiveFlag & 0x201)
		return;
	if (!mMActor)
		return;

	if (param_1 & 4)
		requestShadow();

	if (!(mLiveFlag & 6)) {
		if (param_1 & 2) {
			calcRootMatrix();
			mMActor->calc();
		}

		if (param_1 & 4)
			mMActor->viewCalc();

		if (param_1 & 0x200)
			drawObject(param_2);
	}
}

JGeometry::TVec3<f32>
TLiveActor::calcVelocityToJumpToY(const JGeometry::TVec3<f32>& param_1,
                                  f32 speed, f32 gravity) const
{
	JGeometry::TVec3<f32> vec;
	SMSCalcJumpVelocityY(param_1, mPosition, speed, gravity, -3.0625f, &vec);
	return vec;
}

f32 TLiveActor::getGravityY() const { return mGravity; }

BOOL TLiveActor::hasMapCollision() const
{
	return mMapCollisionManager ? 1 : 0;
}

int TLiveActor::getJointTransByIndex(int param_1,
                                     JGeometry::TVec3<f32>* param_2) const
{
	if (mMActor == nullptr) {
		*param_2 = mPosition;
		return -1;
	}

	if (mLiveFlag & 4) {
		*param_2 = mPosition;
		return param_1;
	}

	MtxPtr mtx = mMActor->unk4->getAnmMtx(param_1);
	param_2->set(mtx[0][3], mtx[1][3], mtx[2][3]);
	return param_1;
}

Vec TLiveActor::getFocalPoint() const { return mPosition; }

MtxPtr TLiveActor::getTakingMtx()
{
	if (!mMActor)
		return nullptr;

	return mMActor->unk4->unk20;
}

void TLiveActor::initAnmSound()
{
	if (mAnmSound)
		return;

	if (checkActorType(0x4000000))
		mAnmSound = new MAnmSoundNPC(gpMSound);
	else
		mAnmSound = new MAnmSound(gpMSound);

	mAnmSound->initAnmSound(nullptr, 1, 0.0f);
}

void TLiveActor::updateAnmSound()
{
	if (!mAnmSound)
		return;
	if (!mAnmSoundPath)
		return;

	J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
	mAnmSound->animeLoop(&mPosition, ctrl->getCurrentFrame(), ctrl->getRate(),
	                     0, 4);
}

void TLiveActor::setAnmSound(const char* path)
{
	if (!mAnmSound)
		OSPanic(__FILE__, 0x385, "TLiveActor[%s] : mAnmSound == NULL\n", mName);

	mAnmSoundPath = path;

	if (mAnmSoundPath != nullptr) {
		void* res = JKRFileLoader::getGlbResource(mAnmSoundPath);
		mAnmSound->initAnmSound(res, 1, 0.0f);
	} else {
		mAnmSound->initAnmSound(nullptr, 1, 0.0f);
	}
}

void TLiveActor::setCurAnmSound()
{
	const char* name = nullptr;

	if (mMActor) {
		int idx = mMActor->getCurAnmIdx(0);
		if (idx >= 0) {
			const char** table = getBasNameTable();

			name = !table ? nullptr : table[idx];
		}
	}

	setAnmSound(name);
}

const char** TLiveActor::getBasNameTable() const { return nullptr; }

void TLiveActor::stopAnmSound()
{
	if (mAnmSound && mAnmSoundPath)
		mAnmSound->stop();
}
