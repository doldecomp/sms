#include <Enemy/BossHanachan.hpp>
#include <Enemy/BossHanachanSub.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Conductor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Binder.hpp>
#include <Strategic/ObjModel.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/CameraShake.hpp>
#include <Camera/cameralib.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAttach.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <math.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

const char* cSandPillarModelName = "sunabashira.bmd";
const char* cHitPoint1_RailName = "bosshanachan2";
const char* cHitPoint2_RailName = "bosshanachan1";
const char* cSandTextureName = "suna";
const char* cDummyTextureName = "M_dummy";

TBossHanachanManager::TBossHanachanManager(const char* name)
    : TEnemyManager(name)
{
	static const char* sCommonSaveFileName = "/enemy/bosshanachanCommon.prm";
	static const char* sChangeSaveFileName[] = {
		"/enemy/bosshanachan0.prm",
		"/enemy/bosshanachan1.prm",
		"/enemy/bosshanachan2.prm",
	};
	mCommonParams = new TBossHanachanCommonSaveParams(sCommonSaveFileName);
	for (int i = 0; i < 3; ++i)
		mChangeParams[i] = new TBossHanachanChangeSaveParams(sChangeSaveFileName[i]);
}

void TBossHanachanManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "hanabody_model.bmd", 0x10300000, 0 },
		{ "hanahead_model.bmd", 0x10100000, 0 },
		{ cSandPillarModelName, 0x10010000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TBossHanachanManager::loadAfter()
{
	J3DMaterialTable* materials = gpMapObjManager->unkC0;
	ResTIMG* sand = materials->getTexture()->getResTIMG(
	    materials->getTextureName()->getIndex(cSandTextureName));
	for (int i = 0; i < 2; ++i)
		SMS_ChangeTextureAll(getModelDataKeeper()->getNthData(i)->getModelData(),
		                     cDummyTextureName, *sand);
}

void TBossHanachanManager::clipEnemies(JDrama::TGraphics* graphics)
{
	clipActorsAux(graphics, mCommonParams->mSLViewClipFar.get(),
	              mCommonParams->mSLViewClipRadius.get());
}

BOOL TBossHanachanManager::hasMapCollision() const { return TRUE; }

TBossHanachan::TBossHanachan(const char* name)
    : TSpineEnemy(name)
    , mWeakBodyIndex(0)
    , unk178(nullptr)
    , mCollisionPosition(0.0f, 0.0f, 0.0f)
    , mPreviousLinearVelocity(0.0f, 0.0f, 0.0f)
    , unk194(0.0f)
    , unk198(0.0f)
    , mSandPillarActor(nullptr)
    , mSandPillarPosition(0.0f, 0.0f, 0.0f)
    , mDeathSoundPosition(0.0f, 0.0f, 0.0f)
    , unk1B8(-1)
    , mCommonParams(nullptr)
    , mChangeParams(nullptr)
{
	setRandomWeakBodyIndex();
}

void TBossHanachan::setRandomWeakBodyIndex()
{
	mWeakBodyIndex = 8.0f * MsRandF();
}

// TODO: reconstruct init, throwMario_, CalcRevisionPosByRotateZ,
// execHeadCalcAnim_, and execBodyCalcAnim_ from the original unit.

void TBossHanachan::kill() { }

void TBossHanachan::bind()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;
	if (mBinder) {
		mBinder->bind(this);
		return;
	}
	JGeometry::TVec3<f32> nextPosition = mPosition;
	nextPosition += mLinearVelocity;
	nextPosition += mVelocity;
	mVelocity.y -= getGravityY();
	if (mVelocity.y < mVelocityMinY)
		mVelocity.y = mVelocityMinY;
	mCollisionPosition.set(nextPosition);
	f32 offsetX, offsetZ;
	BHSCalcRevisionDistXZByRotateZ(mRotation.y, mRotation.z,
	    unk178->mRotationMoveScale, &offsetX, &offsetZ);
	mCollisionPosition.x += offsetX;
	mCollisionPosition.z += offsetZ;
	JGeometry::TVec3<f32> beforeCollision = mCollisionPosition;
	mGroundHeight = gpMap->checkGroundIgnoreWaterSurface(mCollisionPosition.x,
	    mCollisionPosition.y + mHeadHeight, mCollisionPosition.z, &mGroundPlane);
	mGroundHeight += 1.0f;
	if (mCollisionPosition.y <= 0.05f + mGroundHeight) {
		if (mGroundPlane && (mGroundPlane->isIllegalData() == true ? false : true)) {
			offLiveFlag(LIVE_FLAG_AIRBORNE);
			mVelocity.set(0.0f, 0.0f, 0.0f);
			mCollisionPosition.y = mGroundHeight;
		}
	} else {
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	}
	gpMap->isTouchedOneWallAndMoveXZ(&mCollisionPosition.x,
	    mCollisionPosition.y + mHeadHeight, &mCollisionPosition.z, mBodyRadius);
	JGeometry::TVec3<f32> correction = mCollisionPosition - beforeCollision;
	JGeometry::TVec3<f32> displacement = nextPosition - mPosition;
	mLinearVelocity = displacement + correction;
}

void TBossHanachan::moveObject()
{
	updateSquareToMario();
	mPreviousLinearVelocity.x = mLinearVelocity.x;
	mPreviousLinearVelocity.y = mLinearVelocity.y;
	mPreviousLinearVelocity.z = mLinearVelocity.z;
	TLiveActor::moveObject();
	if (mSpine->getLatestNerve() != &TNerveBossHanachanGetUp::theNerve())
		CLBChaseDecrease(&mRotation.z, mBodies[0]->mRotation.z, 0.04f, 0.0f);
	mHead->mPosition = mPosition;
	mHead->mRotation = mRotation;
	mHead->mGroundPlane = mGroundPlane;
}

// TODO: reconstruct perform and its inlined animation helpers.

bool TBossHanachan::isTumbleCompletelyAllBody() const
{
	bool result = true;
	f32 angle = mBodies[0]->mRotation.z;
	bool tumbled = (-179.0f == angle || 179.0f == angle) ? true : false;
	if (!tumbled) {
		result = false;
	} else {
		for (int i = 1; i < 8; ++i) {
			if (mBodies[i]->mRotation.z != angle) {
				result = false;
				break;
			}
		}
	}
	return result;
}

bool TBossHanachan::checkFallDecideAndSetup()
{
	bool result = false;
	for (int i = 0; i < 8; ++i) {
		TBossHanachanPartsBody* body = mBodies[i];
		if (CLBAbs(body->mRotation.z) > mChangeParams->mSLFallDecideRotateZ.get()) {
			emitOneTimeSandPillar_(body);
			if (body->mRotation.z > 0.0f)
				unk194 = 179.0f;
			else
				unk194 = -179.0f;
			f32 change = CLBAbs(body->unk138.y - body->mRotation.z);
			unk198 = change * mChangeParams->mSLWaveFallDownSpeed.get();
			f32 minimum = mChangeParams->mSLFallDecideMinSpeed.get();
			if (unk198 < minimum)
				unk198 = minimum;
			result = true;
			break;
		}
	}
	return result;
}

f32 TBossHanachan::getBodyMaxRotateZ() const
{
	f32 result = 0.0f;
	for (int i = 0; i < 8; ++i) {
		if (fabs(mBodies[i]->mRotation.z) > fabs(result))
			result = mBodies[i]->mRotation.z;
	}
	return result;
}

bool TBossHanachan::isCanWalk() const
{
	bool result = true;
	JGeometry::TVec3<f32> target = unkF4.getPoint();
	JGeometry::TVec3<f32> delta;
	delta.set<f32>(target.x - mPosition.x, 0.0f, target.z - mPosition.z);
	if (delta.squared() < CLBSquared(10.0f))
		result = false;
	return result;
}

void TBossHanachan::execWalk(bool accelerate)
{
	if (accelerate)
		CLBChaseGeneralConstantSpecifySpeed(&mMarchSpeed,
		    mChangeParams->mSLMaxMarchSpeed.get(), mChangeParams->mSLMarchAccel.get());
	else
		CLBChaseGeneralConstantSpecifySpeed(&mMarchSpeed, 0.0f,
		    mChangeParams->mSLMarchDecrease.get());
	mTurnSpeed = mChangeParams->mSLWalkTurnSpeed.get();
	if (isCanWalk())
		walkToCurPathNode(mMarchSpeed, mTurnSpeed, 0.0f);
	JGeometry::TVec3<f32> target = unkF4.getPoint();
	JGeometry::TVec3<f32> delta(target.x - mPosition.x, 0.0f,
	                           target.z - mPosition.z);
	if (delta.squared() < CLBSquared(100.0f)) {
		if (!unk114.empty())
			switchNextGoalPath();
		else
			goToDirLimitedNextGraphNode(90.0f);
	}
}

void TBossHanachan::execSlip()
{
	CLBChaseGeneralConstantSpecifySpeed(&mMarchSpeed, 0.0f,
	    mChangeParams->mSLMarchDecrease.get());
	mTurnSpeed = 0.1f;
	if (mMarchSpeed > 0.001f) {
		JGeometry::TVec3<f32> direction = mPreviousLinearVelocity;
		if (mMarchSpeed > 4.0f) {
			f32 roll = getBodyMaxRotateZ();
			JGeometry::TVec3<f32> side(1.0f, 0.0f, 0.0f);
			if (roll > 0.0f)
				side.negate();
			s16 angle = CLBDegToShortAngle(mRotation.y);
			f32 sine = JMASSin(angle);
			f32 cosine = JMASCos(angle);
			f32 x = side.x * cosine + side.z * sine;
			side.z = -side.x * sine + side.z * cosine;
			side.x = x;
			side.scale(0.005f * mMarchSpeed);
			direction += side;
		}
		bool stopped = direction.isZero();
		if (!stopped) {
			MsVECNormalize(&direction, &direction);
			direction.scale(500.0f);
			JGeometry::TVec3<f32> goal = mPosition;
			goal += direction;
			setGoalPath(goal);
			walkToCurPathNode(mMarchSpeed, mTurnSpeed, 0.0f);
		}
		gpCameraShake->keepShake((EnumCamShakeMode)9, 1.0f);
		if (SMS_IsMarioTouchGround4cm() && mSpine->getTime() < 120)
			SMSRumbleMgr->start(0x16, (f32*)nullptr);
	}
}

void TBossHanachan::goToInitialRecoverGraphNode()
{
	unk124->reset();
	unk124->reset2();
	int node = unk124->getGraph()->findNearestVisibleIndex(mPosition, mRotation.y,
	    mCommonParams->mSLRecoverSearchDist.get(),
	    mCommonParams->mSLRecoverSearchDegree.get(), 0xffffffff);
	if (node < 0) {
		goToShortestNextGraphNode();
		return;
	}
	unk124->setTo(node);
	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
}

void TBossHanachan::execDamage()
{
	mSpine->reset();
	if (mHitPoints != 0)
		--mHitPoints;
	if (mHitPoints == 0) {
		mHead->unk100->onHitFlag(HIT_FLAG_NO_COLLISION);
		for (int i = 0; i < 8; ++i) {
			mBodies[i]->unk100->onHitFlag(HIT_FLAG_NO_COLLISION);
			mBodies[i]->mFeet[0]->onHitFlag(HIT_FLAG_NO_COLLISION);
			mBodies[i]->mFeet[1]->onHitFlag(HIT_FLAG_NO_COLLISION);
		}
		mSpine->setNext(&TNerveBossHanachanDead::theNerve());
		setAnmTimerWhenDead();
		mDeathSoundPosition = *gpMarioPos;
		if (gpMSound->gateCheck(0x28E6))
			MSoundSESystem::MSoundSE::startSoundActor(0x28E6,
			    &mDeathSoundPosition, 0, nullptr, 0, 4);
	} else {
		mSpine->setNext(&TNerveBossHanachanDamage::theNerve());
		setAnmTimerWhenDamage();
		mChangeParams = ((TBossHanachanManager*)mManager)->mChangeParams[3 - mHitPoints];
		const char* railName = cHitPoint1_RailName;
		switch (mHitPoints) {
		case 1:
			railName = cHitPoint1_RailName;
			break;
		case 2:
			railName = cHitPoint2_RailName;
			break;
		}
		unk124->setGraph(gpConductor->getGraphByName(railName));
		onLiveFlag(LIVE_FLAG_UNK20000);
		const JGeometry::TVec3<f32>* soundPosition = &mBodies[mWeakBodyIndex]->unk154;
		if (gpMSound->gateCheck(0x280F))
			MSoundSESystem::MSoundSE::startSoundActor(0x280F,
			    soundPosition, 0, nullptr, 0, 4);
	}
}

void TBossHanachan::removeAllMapCollision()
{
	mHead->unk104->remove();
	for (int i = 0; i < 8; ++i)
		mBodies[i]->unk104->remove();
}

BOOL TBossHanachan::hasMapCollision() const { return TRUE; }
