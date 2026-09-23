#include <Enemy/BossHanachan.hpp>
#include <Enemy/BossHanachanSub.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Conductor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Binder.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <M3DUtil/MActor.hpp>
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
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <System/MarDirector.hpp>
#include <System/TargetArrow.hpp>
#include <GC2D/GCConsole2.hpp>
#include <MoveBG/ItemManager.hpp>
#include <NPC/NpcInbetween.hpp>
#include <MarioUtil/MapUtil.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <math.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MSound/MSModBgm.hpp>
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

// Binding level worth +16 of low region, landing
// TBossHanachanManager::loadAfter's frame at 0x40 (batch 124).
static inline J3DTexture* BossHanachanMainGetTexture(const J3DMaterialTable* p)
{
	J3DTexture* texture = p->getTexture();
	return texture;
}

void TBossHanachanManager::loadAfter()
{
	J3DMaterialTable* materials = gpMapObjManager->unkC0;
	ResTIMG* sand = BossHanachanMainGetTexture(materials)->getResTIMG(
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
	f32 rand       = MsRandF();
	mWeakBodyIndex = 8.0f * rand;
}

// TODO: every instruction matches; the frame is 0x130 against the ROM's
// 0x188, with every stack slot lower (likely a missing inline level).
void TBossHanachan::init(TLiveManager* manager)
{
	mManager = manager;
	manager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(manager, 10);
	mSandPillarActor = mMActorKeeper->createMActor(cSandPillarModelName, 0);
	mCommonParams = ((TBossHanachanManager*)manager)->mCommonParams;
	mChangeParams = ((TBossHanachanManager*)manager)->mChangeParams[0];
	mBodyScale = 1.0f;
	mBodyRadius = 350.0f;
	mWallRadius = mBodyRadius;
	mHeadHeight = 500.0f;
	mMarchSpeed = 0.0f;
	mGravity = 2.0f;
	mHitPoints = 3;
	mScaledBodyRadius = 0.0f;
	onLiveFlag(LIVE_FLAG_UNK1000 | LIVE_FLAG_UNK8);
	mSpine->initWith(&TNerveBossHanachanGraphWander::theNerve());
	unk124->reset();
	goToShortestNextGraphNode();
	initHitActor(0x08000014, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	for (int i = 0; i < 8; ++i) {
		mBodies[i] = new TBossHanachanPartsBody(this, "ボスハナチャンの体");
		mBodies[i]->unk114 = i;
	}
	mHead = new TBossHanachanPartsHead(this, "ボスハナチャンの頭");
	mMActor = mHead->mMActor;
	mCollisionPosition = mPosition;
	JGeometry::TVec3<f32> bodyPosition;
	bodyPosition = mCollisionPosition;
	s16 angle = DEG2SHORTANGLE(mRotation.y);
	const f32& headLength = mCommonParams->mSLHeadLength.get();
	f32 sine = JMASSin(angle);
	bodyPosition.x -= sine * headLength;
	f32 cosine = JMASCos(angle);
	bodyPosition.z -= cosine * headLength;
	unk178 = new TSphereLink(8,
	    JGeometry::TVec3<f32>(bodyPosition.x, bodyPosition.y, bodyPosition.z),
	    mCommonParams->mSLBodyLength.get(), mCommonParams->mSLBodyAttackRadius.get(),
	    0.2f, -2.0f, -3.5f, mRotation.y);
	mHead->mPosition = mPosition;
	mHead->mRotation = mRotation;
	mHead->mGroundPlane = mGroundPlane;
	for (int i = 0; i < 8; ++i) {
		TBossHanachanPartsBody* body = mBodies[i];
		body->mPosition = unk178->mPoints[i].mPosition;
		body->mPreviousPosition = body->mPosition;
		body->mOlderPosition = body->mPreviousPosition;
		body->mRotation = mRotation;
	}
	setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK0, BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
	execHeadCalcAnim_();
	execBodyCalcAnim_();
	TIdxGroupObj* group = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");
	mHead->initMapCollisionAndHitActor_(group);
	for (int i = 0; i < 8; ++i) {
		mBodies[i]->initMapCollisionAndHitActor_(group);
		mBodies[i]->initFootHitActor_(group);
	}
}

void TBossHanachan::throwMario_(THitActor* actor)
{
	JGeometry::TVec3<f32> direction = *gpMarioPos - actor->mPosition;
	f32 speed;
	if (direction.isZero()) {
		direction.set(0.0f, 1.0f, 0.0f);
		speed = mMarchSpeed * mChangeParams->mSLThrowTotalPower.get();
	} else {
		s16 moveAngle = CLBDegToShortAngle(MsGetRotFromZaxisY(mPreviousLinearVelocity));
		s16 marioAngle = CLBDegToShortAngle(MsGetRotFromZaxisY(direction));
		s16 angleDifference = moveAngle - marioAngle;
		int absoluteDifference = CLBAbs<int>(angleDifference);
		f32 ratio = (1.0f / 32768.0f) * absoluteDifference;
		ratio = 1.0f - ratio;
		speed = ratio * (mMarchSpeed * mChangeParams->mSLThrowTotalPower.get());
		JGeometry::TVec3<f32> movement = mPreviousLinearVelocity;
		movement.scale(ratio * mChangeParams->mSLThrowMoveDirPower.get());
		direction += movement;
		direction.y = mChangeParams->mSLThrowVecY.get();
	}
	speed = MsClamp(speed, mChangeParams->mSLThrowSpeedMin.get(),
	                 mChangeParams->mSLThrowSpeedMax.get());
	SMS_SendMessageToMario(mHead, 0xE);
	SMS_SendMessageToMario(mHead, 7);
	SMS_ThrowMario(direction, speed);
	mHead->unk100->onWaterHitCounter();
}

static void CalcRevisionPosByRotateZ(const JGeometry::TVec3<f32>& rotation,
                                    f32 heightScale, Vec* position)
{
	f32 roll = fabsf(rotation.z);
	position->y += heightScale * roll;
	if (roll > 90.0f) {
		JGeometry::TVec3<f32> offset(7.0f * (roll - 90.0f), 0.0f, 0.0f);
		if (rotation.z > 0.0f)
			offset.x = -offset.x;
		s16 angle = CLBDegToShortAngle(rotation.y);
		f32 cosine = JMASCos(angle);
		f32 sine = JMASSin(angle);
		// The rotated offset is never written back: retail stores only
		// into position, so both components are plain locals.
		// TODO: 4 marks left, all volatile-FPR numbering (retail puts the
		// rotated x in f3 and position->x in f2; declaration order and the
		// `+=` spelling are both inert; so are swapped product order,
		// inline JMASCos/JMASSin calls and a rotated TVec3 local).
		f32 x = offset.x * cosine + offset.z * sine;
		f32 z = -offset.x * sine + offset.z * cosine;
		position->x += x;
		position->z += z;
	}
}

void TBossHanachan::execHeadCalcAnim_()
{
	JGeometry::TVec3<f32> position = mPosition;
	CalcRevisionPosByRotateZ(mRotation, mCommonParams->mSLHeadPlusYByRotateZ.get(),
	                        &position);
	CLBCalcRotateZXYTranslateMatrix(mHead->mMActor->getModel()->getBaseTRMtx(),
	                               mRotation, position);
	mHead->mMActor->calc();
}

void TBossHanachan::execBodyCalcAnim_()
{
	for (int i = 0; i < 8; ++i) {
		TBossHanachanPartsBody* body = mBodies[i];
		JGeometry::TVec3<f32> position = body->mPosition;
		CalcRevisionPosByRotateZ(body->mRotation,
		    mCommonParams->mSLBodyPlusYByRotateZ.get(), &position);
		Mtx transform;
		CLBCalcRotateZXYTranslateMatrix(transform, body->mRotation, position);
		body->mMActor->getModel()->setBaseTRMtx(transform);
		body->mMActor->calc();
	}
}

void TBossHanachan::kill() { }

// Binding level over a raw member read, worth +16 of low region in
// TBossHanachan::bind (batch 127).
static inline TMap* BossHanachanMainGetMap()
{
	TMap* map = gpMap;
	return map;
}

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
	mGroundHeight = BossHanachanMainGetMap()->checkGroundIgnoreWaterSurface(mCollisionPosition.x,
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
	BossHanachanMainGetMap()->isTouchedOneWallAndMoveXZ(&mCollisionPosition.x,
	    mCollisionPosition.y + mHeadHeight, &mCollisionPosition.z, mBodyRadius);
	JGeometry::TVec3<f32> correction = mCollisionPosition - beforeCollision;
	JGeometry::TVec3<f32> displacement;
	displacement = nextPosition - mPosition;
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

// Wraps a yaw into [-180, 180). Retail calls MsWrap<f> out of line at all three
// sand-slope sites while inlining MsGetRotFromZaxisY beside them, so the wrap is
// one inline level deeper than the vector yaw. The angle is taken by reference
// because retail loads mRotation.y straight into f1 for the third call.
static inline f32 BossHanachanWrapDegree(const f32& angle)
{
	return MsWrap(angle, -180.0f, 180.0f);
}

static inline void BossHanachanUpdateCentrifugalForce(TBossHanachan* self)
{
	bool tumbling = false;
	f32 force = 0.0f;
	if (self->mSpine->getLatestNerve() == &TNerveBossHanachanTumble::theNerve()) {
		tumbling = true;
		force = self->getBodyMaxRotateZ();
	}
	for (int i = 0; i < 8; ++i) {
		TBossHanachanPartsBody* body = self->mBodies[i];
		if (!tumbling)
			force = BHSCalcCentrifugalForce(body->mPosition, body->mPreviousPosition,
			                               body->mOlderPosition, body->mRotation.y)
			        * self->mChangeParams->mSLCentrifugalForce.get();
		CLBChaseGeneralConstantSpecifySpeed(&body->unk144, force,
		                                    self->mChangeParams->mSLCentrifugalSpeed.get());
		body->unk144 = MsClamp(body->unk144, -179.0f, 179.0f);
	}
}

static inline void BossHanachanUpdateBodyRotateZ(TBossHanachan* self)
{
	f32 maxRoll = self->getBodyMaxRotateZ();
	const TNerveBase<TLiveActor>* current = self->mSpine->getLatestNerve();
	f32 length = self->mCommonParams->mSLBodyLength.get();
	f32 inverseLengthSquared = 1.0f / (length * length);
	f32 damping = self->mChangeParams->mSLWaveDecrease.get() * (1.0f / 120.0f);
	f32 inverseDamping = 1.0f / (1.0f + damping);
	f32 previousDamping = 1.0f - damping;
	f32 velocity = self->mChangeParams->mSLWaveVelocity.get();
	f32 waveScale = (1.0f / 120.0f) * ((1.0f / 120.0f)
	                   * (velocity * velocity));
	for (int i = 0; i < 8; ++i) {
		TBossHanachanPartsBody* body = self->mBodies[i];
		bool overturned = (-179.0f == body->mRotation.z
		                   || 179.0f == body->mRotation.z)
		                      ? true : false;
		if (!overturned || body->mRotation.z != maxRoll) {
			f32 previous = i == 0 ? self->mBodies[i + 1]->mPreviousRoll
			                      : self->mBodies[i - 1]->mPreviousRoll;
			f32 next = i == 7 ? self->mBodies[i - 1]->mPreviousRoll
			                  : self->mBodies[i + 1]->mPreviousRoll;
			f32 twiceRoll = 2.0f * body->mPreviousRoll;
			f32 roll = twiceRoll * inverseDamping
			           + inverseDamping
			                 * (waveScale
			                    * (inverseLengthSquared * (next + previous - twiceRoll)
			                       + body->unk148))
			           - inverseDamping * (body->mOlderRoll * previousDamping);
			roll = MsClamp(roll, -179.0f, 179.0f);
			CLBChaseGeneralConstantSpecifySpeed(&body->mRotation.z, roll,
			                                    self->mChangeParams->mSLRotateZLeanSpeed.get());
			bool onSand = false;
			if (current == &TNerveBossHanachanGraphWander::theNerve()) {
				if (body->getSandActor_()) {
					onSand = true;
					if (body->unk120 != 0.0f) {
						f32 speed = body->unk120 * self->mMarchSpeed
						            * self->mChangeParams->mSLSandSlopeForce.get();
						f32 target = 179.0f;
						if (body->unk120 < 0.0f)
							target = -179.0f;
						CLBChaseGeneralConstantSpecifySpeed(&body->mRotation.z, target, speed);
					}
				} else {
					CLBChaseGeneralConstantSpecifySpeed(&body->mRotation.z, body->unk120,
					                                    self->mChangeParams->mSLRotateZRestorationSpeed.get());
				}
			} else if (current == &TNerveBossHanachanTumble::theNerve()) {
				CLBChaseGeneralConstantSpecifySpeed(&body->mRotation.z, self->unk194, self->unk198);
			}
			body->mRotation.z = MsClamp(body->mRotation.z, -179.0f, 179.0f);
			if (current == &TNerveBossHanachanGraphWander::theNerve()
			    && (self->mSpine->getTime() < self->mChangeParams->mSLNotFallDownFrames.get()
			        || !onSand)) {
				f32 limit = self->mChangeParams->mSLMaxRotateZNotSand.get();
				if (body->mRotation.z < -limit)
					CLBChaseGeneralConstantSpecifySpeed(&body->mRotation.z, -limit, 15.0f);
				else if (body->mRotation.z > limit)
					CLBChaseGeneralConstantSpecifySpeed(&body->mRotation.z, limit, 15.0f);
			}
		}
	}
	f32 limit = self->mChangeParams->mSLDiffMaxRotateZ.get();
	for (int i = 1; i < 8; ++i) {
		const f32& previous = self->mBodies[i - 1]->mRotation.z;
		f32& roll = self->mBodies[i]->mRotation.z;
		if (fabs(previous - roll) > limit) {
			if (roll < previous)
				roll = previous - limit;
			else
				roll = previous + limit;
		}
	}
}

static inline void BossHanachanSaveHistory(TBossHanachan* self)
{
	for (int i = 0; i < 8; ++i) {
		TBossHanachanPartsBody* body = self->mBodies[i];
		body->mOlderPosition = body->mPreviousPosition;
		body->mPreviousPosition = body->mPosition;
		body->mOlderRoll = body->mPreviousRoll;
		body->mPreviousRoll = body->mRotation.z;
		body->unk148 = body->unk144;
	}
}

void TBossHanachan::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (checkLiveFlag(0x201))
		return;
	if (checkLiveFlag(0x40000)) {
		if ((cue & 1) && (graphics->unk0 & 2)) {
			if (gpMSound->gateCheck(0x6010))
				MSoundSESystem::MSoundSE::startSoundActor(0x6010, &mPosition,
				                                        0, nullptr, 0, 4);
			if (!gpMarDirector->isThing()
			    && (checkLiveFlag(0x100000)
			        || !gpMSound->unk98->modBgm(1, 1))) {
				mLiveFlag |= 0x41;
				gpItemManager->makeShineAppearWithDemo(
				    "シャイン（ボス用）", "ボスシャインカメラ", mCollisionPosition.x,
				    mCollisionPosition.y + mCommonParams->mSLShineAppearOffsetY.get(),
				    mCollisionPosition.z);
			}
		}
		return;
	}

	if (cue & 1) {
		if (gpMarDirector->isThing()) {
			mLinearVelocity.zero();
			mAngularVelocity.zero();
			if ((graphics->unk0 & 2)
			    && mSpine->getLatestNerve() == &TNerveBossHanachanDead::theNerve()
			    && !checkLiveFlag(0x100000)) {
				mLiveFlag |= 0x100000;
				MSBgm::stopTrackBGM(1, 30);
			}
		} else {
			if (!checkLiveFlag(0x80000) && mHitPoints == 3
			    && mMarchSpeed != 0.0f
			    && mSpine->getLatestNerve() != &TNerveBossHanachanTumble::theNerve()) {
				if (unk1B8 == -1 && gpMarDirector->mState == 4) {
					unk1B8 = 7200;
				} else if (unk1B8 > 0) {
					--unk1B8;
					if (unk1B8 == 0) {
						unk1B8 = 7200;
						gpMarDirector->mConsole->startAppearBalloon(6, true);
					}
				}
			}
			moveObject();
			const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
			BossHanachanSaveHistory(this);
			s16 angle = CLBDegToShortAngle(mBodies[0]->mRotation.y);
			CLBChaseAngleDecrease(&angle, CLBDegToShortAngle(mRotation.y),
			                      20);
			mBodies[0]->mRotation.y = (360.0f / 65536.0f) * angle;
			for (int i = 1; i < 8; ++i) {
				TBossHanachanPartsBody* body = mBodies[i];
				JGeometry::TVec3<f32> axis = unk178->mPoints[i - 1].mPosition
				                            - unk178->mPoints[i].mPosition;
				body->mRotation.y
				    = MsAngleWrap(MsGetRotFromZaxisY(axis));
			}
			unk178->mHeadDegreeY = mBodies[0]->mRotation.y;
			for (int i = 0; i < 8; ++i)
				unk178->setDegreeZAndRevisionPosXZ(i, mBodies[i]->mRotation.z);
			JGeometry::TVec3<f32> headPosition = mPosition;
			headPosition.x
			    -= JMASin(mRotation.y) * mCommonParams->mSLHeadLength.get();
			headPosition.z -= JMACos(mRotation.y) * mCommonParams->mSLHeadLength.get();
			f32 offsetX, offsetZ;
			BHSCalcRevisionDistXZByRotateZ(mRotation.y, mRotation.z,
			                              unk178->mRotationMoveScale, &offsetX, &offsetZ);
			headPosition.x += offsetX;
			headPosition.z += offsetZ;
			unk178->moveHead(headPosition);
			for (int i = 0; i < 8; ++i) {
				TBossHanachanPartsBody* body = mBodies[i];
				BHSCalcRevisionDistXZByRotateZ(body->mRotation.y, body->mRotation.z,
				                              unk178->mRotationMoveScale, &offsetX, &offsetZ);
				body->mPosition = unk178->mPoints[i].mPosition;
				body->mPosition.x -= offsetX;
				body->mPosition.z -= offsetZ;
			}
			BossHanachanUpdateCentrifugalForce(this);
			if (nerve != &TNerveBossHanachanDown::theNerve()) {
				for (int i = 0; i < 8; ++i) {
					TBossHanachanPartsBody* body = mBodies[i];
					f32 groundHeight = gpMap->checkGroundIgnoreWaterSurface(
					    body->mPosition.x, 500.0f + body->mPosition.y,
					    body->mPosition.z, &body->mGroundPlane);
					const TLiveActor* sand = body->getSandActor_();
					if (sand) {
						JGeometry::TVec3<f32> delta = sand->mPosition - mPosition;
						JGeometry::TVec3<f32> direction;
						direction = delta;
						if (direction.x * direction.x + direction.z * direction.z
						    <= CLBSquared(50.0f)) {
							body->unk120 = 0.0f;
						} else {
							f32 yaw = BossHanachanWrapDegree(MsGetRotFromZaxisY(direction));
							f32 difference = BossHanachanWrapDegree(
							    yaw - BossHanachanWrapDegree(mRotation.y));
							f32 absolute = CLBAbs(difference);
							if (absolute <= 15.0f || absolute >= 165.0f) {
								body->unk120 = 0.0f;
							} else {
								f32 rise = SMS_GetSandRiseUpRatio(sand);
								if (difference > 15.0f)
									body->unk120 = 70.0f * rise;
								else
									body->unk120 = -70.0f * rise;
							}
						}
					} else {
						JGeometry::TVec3<f32> side(200.0f, 0.0f, 0.0f);
						s16 yaw = CLBDegToShortAngle(body->mRotation.y);
						f32 cosine = JMASCos(yaw);
						f32 sine = JMASSin(yaw);
						f32 x = side.x * cosine + side.z * sine;
						side.z = -side.x * sine + side.z * cosine;
						side.x = x;
						JGeometry::TVec3<f32> opposite(-side.x, -side.y, -side.z);
						side += body->mPosition;
						opposite += body->mPosition;
						const TBGCheckData* ground;
						f32 left = gpMap->checkGroundIgnoreWaterSurface(
						    side.x, 500.0f + side.y, side.z, &ground);
						f32 right = gpMap->checkGroundIgnoreWaterSurface(
						    opposite.x, 500.0f + opposite.y, opposite.z, &ground);
						left -= groundHeight;
						right -= groundHeight;
						f64 absoluteLeft = fabs(left);
						f64 absoluteRight = fabs(right);
						if (absoluteLeft < 0.001f && absoluteRight < 0.001f)
							body->unk120 = 0.0f;
						else if (absoluteLeft > absoluteRight)
							body->unk120 = (360.0f / 65536.0f) * matan(200.0f, left);
						else
							body->unk120 = -((360.0f / 65536.0f) * matan(200.0f, right));
					}
				}
				if (nerve == &TNerveBossHanachanGetUp::theNerve()) {
					mHead->calcRotateZWhenGetUp_();
					mRotation.z = mHead->mRotation.z;
					for (int i = 0; i < 8; ++i)
						mBodies[i]->calcRotateZWhenGetUp_();
				} else {
					BossHanachanUpdateBodyRotateZ(this);
				}
			}
			bool thrown = false;
			TWaterHitActor* hit = mHead->unk100;
			if (hit->mWaterHitCounter >= 1)
				--hit->mWaterHitCounter;
			hit = mHead->unk100;
			for (int j = 0; j < hit->getColNum(); ++j) {
				if (hit->getCollision(j)->getActorType() == 0x80000001) {
					throwMario_(hit);
					thrown = true;
					break;
				}
			}
			for (int i = 0; i < 8; ++i) {
				TBossHanachanPartsBody* body = mBodies[i];
				TWaterHitActor* hit = body->unk100;
				if (hit->mWaterHitCounter >= 1)
					--hit->mWaterHitCounter;
				if (!thrown) {
					hit = body->unk100;
					for (int j = 0; j < hit->getColNum(); ++j) {
						if (hit->getCollision(j)->getActorType() == 0x80000001) {
							throwMario_(hit);
							thrown = true;
							break;
						}
					}
				}
				for (int j = 0; j < 2; ++j) {
					TFootHitActor* foot = body->mFeet[j];
					if (foot->mWaterHitCounter >= 1)
						--foot->mWaterHitCounter;
					if (!thrown) {
						for (int k = 0; k < foot->getColNum(); ++k) {
							if (foot->getCollision(k)->getActorType() == 0x80000001) {
								throwMario_(foot);
								thrown = true;
								break;
							}
						}
					}
				}
			}
			if ((graphics->unk0 & 2) && nerve == &TNerveBossHanachanDead::theNerve()) {
				if (gpMSound->gateCheck(0x6010))
					MSoundSESystem::MSoundSE::startSoundActor(0x6010, &mPosition, 0, nullptr, 0, 4);
				if (!checkLiveFlag(0x100000))
					gpMSound->unk98->modBgm(1, 1);
			}
		}
		mHead->moveMapCollision_();
		for (int i = 0; i < 8; ++i)
			mBodies[i]->moveMapCollision_();
	}
	if (cue & 2) {
		gpTargetArrow->unk14 = 0;
		if (!gpMarDirector->isThing()) {
			bool walking = false;
			if (mSpine->getLatestNerve() == &TNerveBossHanachanGraphWander::theNerve()
			    && mMarchSpeed > 0.001f)
				walking = true;
			JGeometry::TVec3<f32> position(mHead->unk108[0][3],
			                               mHead->unk108[1][3], mHead->unk108[2][3]);
			mHead->unk100->mPosition.set(
			    position.x, position.y - mCommonParams->mSLHeadHitOffsetY.get(),
			    position.z);
			if (walking) {
				if (!((mHead->unk100->mHitFlags & 0x80000000) ? true : false)) {
					mHead->unk100->onHitFlag(0x80000000);
					mHead->unk104->remove();
				}
			} else {
				if ((mHead->unk100->mHitFlags & 0x80000000) ? true : false) {
					mHead->unk100->offHitFlag(0x80000000);
					mHead->unk104->setUpTrans(position);
				}
			}
			f32 bodyOffset = mCommonParams->mSLBodyHitOffsetY.get();
			f32 footOffset = mCommonParams->mSLFootHitOffsetY.get();
			for (int i = 0; i < 8; ++i) {
				TBossHanachanPartsBody* body = mBodies[i];
				TWaterHitActor* hit = body->unk100;
				body->unk154.set(body->unk108[0][3], body->unk108[1][3],
				                 body->unk108[2][3]);
				hit->mPosition.set(body->unk154.x, body->unk154.y - bodyOffset,
				                   body->unk154.z);
				if (walking) {
					if (!(hit->checkHitFlag(0x80000000) ? true : false)) {
						hit->onHitFlag(0x80000000);
						body->unk104->remove();
					}
				} else {
					if (hit->checkHitFlag(0x80000000) ? true : false) {
						hit->offHitFlag(0x80000000);
						body->unk104->setUpTrans(body->unk154);
					}
				}
				if (walking) {
					for (int j = 0; j < 2; ++j) {
						TFootHitActor* foot = body->mFeet[j];
						foot->mPosition.set(foot->mJointMtx[0][3],
						                    foot->mJointMtx[1][3] - footOffset,
						                    foot->mJointMtx[2][3]);
						foot->onHitFlag(0x80000000);
					}
				} else {
					for (int j = 0; j < 2; ++j) {
						TFootHitActor* foot = body->mFeet[j];
						foot->mPosition.set(foot->mJointMtx[0][3],
						                    foot->mJointMtx[1][3] - footOffset,
						                    foot->mJointMtx[2][3]);
						foot->offHitFlag(0x80000000);
					}
				}
			}
			emitParticle_();
			emitCamShake_();
		}
		if (checkLiveFlag(0x10000)
		    && mSandPillarActor->curAnmEndsNext(0, nullptr))
			mLiveFlag &= ~0x10000;
		if (!gpMarDirector->isThing())
			changeAnmRateAndFrameUpdate_();
		mHead->mInbetween->execMotionBlend(mHead->getMActor());
		for (int i = 0; i < 8; ++i) {
			TBossHanachanPartsBody* body = mBodies[i];
			body->mInbetween->execMotionBlend(body->getMActor());
		}
	}
	if (cue & 2) {
		execHeadCalcAnim_();
		execBodyCalcAnim_();
		if (!gpMarDirector->isThing()) {
			if (mSpine->getLatestNerve() == &TNerveBossHanachanTumble::theNerve()
			    || mSpine->getLatestNerve() == &TNerveBossHanachanDown::theNerve()) {
				MtxPtr matrix = mBodies[mWeakBodyIndex]->unk108;
				JGeometry::TVec3<f32> position(matrix[0][3], 400.0f + matrix[1][3],
				                               matrix[2][3]);
				gpTargetArrow->setPos(position);
				gpTargetArrow->unk14 = 1;
			}
		}
	}
	if (cue & 0x200) {
		mHead->entryCircleShadow_();
		mHead->setDamageFog_(graphics);
		mHead->drawObject(graphics);
		for (int i = 0; i < 8; ++i) {
			TBossHanachanPartsBody* body = mBodies[i];
			body->entryCircleShadow_();
			body->setDamageFog_(graphics);
			body->drawObject(graphics);
		}
	}
	if (cue & 4) {
		mHead->getMActor()->viewCalc();
		for (int i = 0; i < 8; ++i)
			mBodies[i]->getMActor()->viewCalc();
	}
	if (checkLiveFlag(0x10000))
		mSandPillarActor->perform(cue, graphics);
}

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
			unk198 = CLBAbs(body->mPreviousRoll - body->mRotation.z) * mChangeParams->mSLWaveFallDownSpeed.value;
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

// The map emits JGeometry::TVec3<f32>::set<f32>(f32, f32, f32) as a local
// instantiation for this TU and execWalk's inlined copy of isCanWalk reaches
// it with a `bl`, so retail has one inline level between isCanWalk and the
// unnamed vector's constructor: that puts `set` (three statements) at depth 4,
// where the allowance is two. Same shape as NpcWalkTurn's
// NpcWalkTurnSquaredXZ; parked here because the map has no symbol for it.
static inline f32 BossHanachanSquaredXZ(const JGeometry::TVec3<f32>& a,
                                        const JGeometry::TVec3<f32>& b)
{
	return JGeometry::TVec3<f32>(a.x - b.x, 0.0f, a.z - b.z).squared();
}

bool TBossHanachan::isCanWalk() const
{
	bool result = true;
	JGeometry::TVec3<f32> target = unkF4.getPoint();
	if (BossHanachanSquaredXZ(target, mPosition) < CLBSquared(10.0f))
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
	if (JGeometry::TVec3<f32>(target.x - mPosition.x, 0.0f,
	                          target.z - mPosition.z).squared()
	    < CLBSquared(100.0f)) {
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
			f32 cosine = JMASCos(angle);
			f32 sine = JMASSin(angle);
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
