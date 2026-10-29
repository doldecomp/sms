#include <Enemy/BossHanachan.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <System/TargetArrow.hpp>
#include <System/MarDirector.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Binder.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/MarioAccess.hpp>
#include <MSound/SoundEffects.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSModBgm.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/MapUtil.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/Map.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <GC2D/GCConsole2.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/BossHanachanSub.hpp>
#include <Enemy/BossHanachanParts.hpp>
#include <Enemy/BossHanachanChangeSaveParams.hpp>
#include <Camera/CameraShake.hpp>
#include <Camera/cameralib.hpp>
#include <macros.h>
#include <stdlib.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

const char* cSandPillarModelName = "sunabashira.bmd";
const char* cHitPoint1_RailName  = "bosshanachan2";
const char* cHitPoint2_RailName  = "bosshanachan1";
const char* cSandTextureName     = "suna";
const char* cDummyTextureName    = "M_dummy";

static void CalcRevisionPosByRotateZ(const JGeometry::TVec3<f32>&, f32, Vec*);

TBossHanachanManager::TBossHanachanManager(const char* name)
    : TEnemyManager(name)
{
	static const char* sCommonSaveFileName   = "/enemy/bosshanachanCommon.prm";
	static const char* sChangeSaveFileName[] = {
		"/enemy/bosshanachan0.prm",
		"/enemy/bosshanachan1.prm",
		"/enemy/bosshanachan2.prm",
	};
	unk54 = new TBossHanachanCommonSaveParams(sCommonSaveFileName);
	for (int i = 0; i < ARRAY_COUNT(unk58); ++i)
		unk58[i] = new TBossHanachanChangeSaveParams(sChangeSaveFileName[i]);
}

void TBossHanachanManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "hanabody_model.bmd", 0x10300000, 0 },
		{ "hanahead_model.bmd", 0x10100000, 0 },
		{ cSandPillarModelName, 0x10010000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TBossHanachanManager::loadAfter()
{
	J3DMaterialTable* table = gpMapObjManager->getSandBombBaseMatTable();
	const ResTIMG& texture  = *table->getTexture()->getResTIMG(
        (u16)table->getTextureName()->getIndex(cSandTextureName));
	for (int i = 0; i < 2; ++i)
		SMS_ChangeTextureAll(
		    getModelDataKeeper()->getNthData(i)->getModelData(),
		    cDummyTextureName, texture);
}

void TBossHanachanManager::clipEnemies(JDrama::TGraphics* graphics)
{
	clipActorsAux(graphics, unk54->mSLViewClipFar.get(),
	              unk54->mSLViewClipRadius.get());
}

BOOL TBossHanachanManager::hasMapCollision() const { return TRUE; }

// TODO: frame-only mismatch.
TBossHanachan::TBossHanachan(const char* name)
    : TSpineEnemy(name)
    , unk174(0)
    , unk178(nullptr)
    , unk17C(0.0f, 0.0f, 0.0f)
    , unk188(0.0f, 0.0f, 0.0f)
    , unk194(0.0f)
    , unk198(0.0f)
    , unk19C(nullptr)
    , unk1A0(0.0f, 0.0f, 0.0f)
    , unk1AC(0.0f, 0.0f, 0.0f)
    , unk1B8(-1)
    , unk1BC(nullptr)
    , unk1C0(nullptr)
{
	setRandomWeakBodyIndex();
}

void TBossHanachan::setRandomWeakBodyIndex()
{
	unk174 = ARRAY_COUNT(mBodyParts) * MsRandF();
}

// TODO: frame offsets and register allocation differ in the final setup loop.
void TBossHanachan::init(TLiveManager* manager)
{
	mManager = manager;
	manager->manageActor(this);
	mMActorKeeper     = new TMActorKeeper(manager, 10);
	unk19C            = mMActorKeeper->createMActor(cSandPillarModelName, 0);
	unk1BC            = ((TBossHanachanManager*)manager)->unk54;
	unk1C0            = ((TBossHanachanManager*)manager)->unk58[0];
	mBodyScale        = 1.0f;
	mBodyRadius       = 350.0f;
	mWallRadius       = mBodyRadius;
	mHeadHeight       = 500.0f;
	mMarchSpeed       = 0.0f;
	mGravity          = 2.0f;
	mHitPoints        = 3;
	mScaledBodyRadius = 0.0f;
	onLiveFlag(LIVE_FLAG_UNK1000 | LIVE_FLAG_UNK8);
	mSpine->initWith(&TNerveBossHanachanGraphWander::theNerve());
	unk124->reset();
	goToShortestNextGraphNode();
	initHitActor(ACTOR_TYPE_BOSS_UNK14, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f);
	onHitFilter(HIT_FILTER_NO_COLLISION);
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
		mBodyParts[i]         = new TBossHanachanPartsBody(this);
		mBodyParts[i]->unk114 = i;
	}
	mHead   = new TBossHanachanPartsHead(this);
	mMActor = mHead->getMActor();
	unk17C  = mPosition;

	Vec position  = unk17C;
	s16 direction = mRotation.y * (65536.0f / 360.0f);
	position.x -= JMASSin(direction) * unk1BC->mSLHeadLength.get();
	position.z -= JMASCos(direction) * unk1BC->mSLHeadLength.get();

	unk178           = new TSphereLink(8, position, unk1BC->mSLBodyLength.get(),
	                                   unk1BC->mSLBodyAttackRadius.get(), 0.2f, -2.0f,
	                                   -3.5f, mRotation.y);
	mHead->mPosition = mPosition;
	mHead->mRotation = mRotation;
	mHead->mGroundPlane = mGroundPlane;
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
		TBossHanachanPartsBody* body = mBodyParts[i];
		body->mPosition              = unk178->mPoints[i].mPosition;
		body->unk124                 = body->mPosition;
		body->unk130                 = body->unk124;
		body->mRotation              = mRotation;
	}
	setHeadAndBodyAnm(BOSS_HANACHAN_ANM_UNK0,
	                  BOSS_HANACHAN_STOP_MOTION_BLEND_OFF);
	execHeadCalcAnim_();
	execBodyCalcAnim_();
	TIdxGroupObj* group = (TIdxGroupObj*)JDrama::TNameRefGen::getInstance()
	                          ->getRootNameRef()
	                          ->search("敵グループ");
	mHead->initMapCollisionAndHitActor_(group);
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
		mBodyParts[i]->initMapCollisionAndHitActor_(group);
		mBodyParts[i]->initFootHitActor_(group);
	}
}

void TBossHanachan::throwMario_(THitActor* hit_actor)
{
	JGeometry::TVec3<f32> direction = *gpMarioPos - hit_actor->mPosition;
	f32 power;
	if (direction.isZero()) {
		direction.set(0.0f, 1.0f, 0.0f);
		power = mMarchSpeed * unk1C0->mSLThrowTotalPower.get();
	} else {
		s16 moveAngle  = CLBDegToShortAngle(MsGetRotFromZaxisY(unk188));
		s16 angle      = CLBDegToShortAngle(MsGetRotFromZaxisY(direction));
		s16 difference = moveAngle - angle;
		f32 ratio      = (1.0f / 32768.0f) * CLBAbs<int>(difference);
		ratio          = 1.0f - ratio;
		power = ratio * (mMarchSpeed * unk1C0->mSLThrowTotalPower.get());
		JGeometry::TVec3<f32> movement = unk188;
		movement *= ratio * unk1C0->mSLThrowMoveDirPower.get();
		direction += movement;
		direction.y = unk1C0->mSLThrowVecY.get();
	}

	power = MsClamp(power, unk1C0->mSLThrowSpeedMin.get(),
	                unk1C0->mSLThrowSpeedMax.get());
	SMS_SendMessageToMario(mHead, HIT_MESSAGE_ATTACK);
	SMS_SendMessageToMario(mHead, HIT_MESSAGE_THROWN);
	SMS_ThrowMario(direction, power);
	mHead->unk100->onWaterHitCounter();
}

static void CalcRevisionPosByRotateZ(const JGeometry::TVec3<f32>& rotation,
                                     f32 height_scale, Vec* position)
{
	f32 angle = fabsf(rotation.z);
	position->y += height_scale * angle;
	if (angle > 90.0f) {
		JGeometry::TVec2<f32> offset(7.0f * (angle - 90.0f), 0.0f);
		if (rotation.z > 0.0f)
			offset.x = -offset.x;
		s16 direction = CLBDegToShortAngle(rotation.y);
		f32 cosY      = JMASCos(direction);
		f32 sinY      = JMASSin(direction);
		position->x += offset.x * cosY + offset.y * sinY;
		position->z += -offset.x * sinY + offset.y * cosY;
	}
}

void TBossHanachan::execHeadCalcAnim_()
{
	JGeometry::TVec3<f32> position = mPosition;
	CalcRevisionPosByRotateZ(mRotation, unk1BC->mSLHeadPlusYByRotateZ.get(),
	                         &position);
	CLBCalcRotateZXYTranslateMatrix(
	    mHead->getMActor()->getModel()->getBaseTRMtx(), mRotation, position);
	mHead->getMActor()->calc();
}

void TBossHanachan::execBodyCalcAnim_()
{
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
		TBossHanachanPartsBody* body   = mBodyParts[i];
		JGeometry::TVec3<f32> position = body->mPosition;
		CalcRevisionPosByRotateZ(
		    body->mRotation, unk1BC->mSLBodyPlusYByRotateZ.get(), &position);
		Mtx matrix;
		CLBCalcRotateZXYTranslateMatrix(matrix, body->mRotation, position);
		MTXCopy(matrix, body->getMActor()->getModel()->getBaseTRMtx());
		body->getMActor()->calc();
	}
}

void TBossHanachan::kill() { }

// TODO: frame offsets and revision-argument load order differ.
void TBossHanachan::bind()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;
	if (mBinder) {
		mBinder->bind(this);
		return;
	}
	JGeometry::TVec3<f32> next = mPosition;
	next.add(mPositionDelta);
	next.add(mVelocity);
	mVelocity.y -= getGravityY();
	if (mVelocity.y < mVelocityMinY)
		mVelocity.y = mVelocityMinY;
	unk17C.set(next);
	f32 x, z;
	BHSCalcRevisionDistXZByRotateZ(mRotation.y, mRotation.z,
	                               unk178->mRevisionScale, &x, &z);
	unk17C.x += x;
	unk17C.z += z;
	JGeometry::TVec3<f32> old = unk17C;
	mGroundHeight             = gpMap->checkGroundIgnoreWaterSurface(
        unk17C.x, unk17C.y + mHeadHeight, unk17C.z, &mGroundPlane);
	mGroundHeight += 1.0f;
	if (unk17C.y <= 0.05f + mGroundHeight) {
		if (mGroundPlane && mGroundPlane->isLegal()) {
			offLiveFlag(LIVE_FLAG_AIRBORNE);
			mVelocity.set(0.0f, 0.0f, 0.0f);
			unk17C.y = mGroundHeight;
		}
	} else {
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	}
	gpMap->isTouchedOneWallAndMoveXZ(&unk17C.x, unk17C.y + mHeadHeight,
	                                 &unk17C.z, mBodyRadius);
	JGeometry::TVec3<f32> correction = unk17C - old;
	JGeometry::TVec3<f32> movement;
	movement       = next - mPosition;
	mPositionDelta = movement + correction;
}

void TBossHanachan::moveObject()
{
	updateSquareToMario();
	unk188.set(mPositionDelta);
	TLiveActor::moveObject();
	if (!(mSpine->getLatestNerve() == &TNerveBossHanachanGetUp::theNerve()))
		CLBChaseDecrease(&mRotation.z, mBodyParts[0]->mRotation.z, 0.04f, 0.0f);
	mHead->mPosition    = mPosition;
	mHead->mRotation    = mRotation;
	mHead->mGroundPlane = mGroundPlane;
}

void TBossHanachan::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_UNK200))
		return;
	if (checkLiveFlag(LIVE_FLAG_UNK40000)) {
		if ((cue & CUE_MOVE) && (graphics->unk0 & 2)) {
			SMSGetMSound()->startSoundActor(MSD_SE_WT_BOSS_FADEAWAY, &mPosition,
			                                0, nullptr, 0, 4);
			if (!gpMarDirector->isInTalkNow()
			    && (checkLiveFlag(LIVE_FLAG_UNK100000)
			        || gpMSound->unk98->modBgm(1, 1) == 0)) {
				onLiveFlag(LIVE_FLAG_UNK40 | LIVE_FLAG_DEAD);
				gpItemManager->makeShineAppearWithDemo(
				    "シャイン（ボス用）", "ボスシャインカメラ", unk17C.x,
				    unk17C.y + unk1BC->mSLShineAppearOffsetY.get(), unk17C.z);
			}
		}
		return;
	}
	if (cue & CUE_MOVE) {
		if (gpMarDirector->isInTalkNow()) {
			mPositionDelta.zero();
			mRotationDelta.zero();
			if ((graphics->unk0 & 2)
			    && (mSpine->getLatestNerve()
			        == &TNerveBossHanachanDead::theNerve())
			    && !checkLiveFlag(LIVE_FLAG_UNK100000)) {
				onLiveFlag(LIVE_FLAG_UNK100000);
				MSBgm::stopTrackBGM(1, 30);
			}
		} else {
			if (!checkLiveFlag(LIVE_FLAG_UNK80000) && mHitPoints == 3
			    && mMarchSpeed != 0.0f
			    && !(mSpine->getLatestNerve()
			         == &TNerveBossHanachanTumble::theNerve())) {
				if (unk1B8 == -1
				    && gpMarDirector->getState() == TMarDirector::STATE_UNK4)
					unk1B8 = 7200;
				else if (unk1B8 > 0) {
					--unk1B8;
					if (unk1B8 == 0) {
						unk1B8 = 7200;
						gpMarDirector->getConsole()->startAppearBalloon(0xE0006,
						                                                true);
					}
				}
			}
			moveObject();
			const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
			for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
				mBodyParts[i]->unk130 = mBodyParts[i]->unk124;
				mBodyParts[i]->unk124 = mBodyParts[i]->mPosition;
				mBodyParts[i]->unk140 = mBodyParts[i]->unk13C;
				mBodyParts[i]->unk13C = mBodyParts[i]->mRotation.z;
				mBodyParts[i]->unk148 = mBodyParts[i]->unk144;
			}
			s16 angle = CLBDegToShortAngle(mBodyParts[0]->mRotation.y);
			CLBChaseAngleDecrease(&angle, CLBDegToShortAngle(mRotation.y), 20);
			mBodyParts[0]->mRotation.y = (360.0f / 65536.0f) * angle;
			for (int i = 1; i < ARRAY_COUNT(mBodyParts); ++i) {
				TBossHanachanPartsBody* body = mBodyParts[i];
				JGeometry::TVec3<f32> direction
				    = unk178->mPoints[i - 1].mPosition
				      - unk178->mPoints[i].mPosition;
				body->mRotation.y = MsAngleWrap(MsGetRotFromZaxisY(direction));
			}
			unk178->mInitialDirection = mBodyParts[0]->mRotation.y;
			for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i)
				unk178->setDegreeZAndRevisionPosXZ(i,
				                                   mBodyParts[i]->mRotation.z);
			JGeometry::TVec3<f32> position = mPosition;
			position.x -= MsSin(mRotation.y) * unk1BC->mSLHeadLength.get();
			position.z -= MsCos(mRotation.y) * unk1BC->mSLHeadLength.get();
			f32 x, z;
			BHSCalcRevisionDistXZByRotateZ(mRotation.y, mRotation.z,
			                               unk178->mRevisionScale, &x, &z);
			position.x += x;
			position.z += z;
			unk178->moveHead(position);
			for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
				TBossHanachanPartsBody* body = mBodyParts[i];
				BHSCalcRevisionDistXZByRotateZ(body->mRotation.y,
				                               body->mRotation.z,
				                               unk178->mRevisionScale, &x, &z);
				body->mPosition = unk178->mPoints[i].mPosition;
				body->mPosition.x -= x;
				body->mPosition.z -= z;
			}
			bool tumble = false;
			f32 force   = 0.0f;
			if ((mSpine->getLatestNerve()
			     == &TNerveBossHanachanTumble::theNerve())) {
				tumble = true;
				force  = getBodyMaxRotateZ();
			}
			for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
				TBossHanachanPartsBody* body = mBodyParts[i];
				if (!tumble)
					force = BHSCalcCentrifugalForce(body->mPosition,
					                                body->unk124, body->unk130,
					                                body->mRotation.y)
					        * unk1C0->mSLCentrifugalForce.get();
				CLBChaseGeneralConstantSpecifySpeed(
				    &body->unk144, force, unk1C0->mSLCentrifugalSpeed.get());
				body->unk144 = MsClamp(body->unk144, -179.0f, 179.0f);
			}
			if (nerve != &TNerveBossHanachanDown::theNerve()) {
				for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
					TBossHanachanPartsBody* body = mBodyParts[i];
					f32 ground = gpMap->checkGroundIgnoreWaterSurface(
					    body->mPosition.x, 500.0f + body->mPosition.y,
					    body->mPosition.z, &body->mGroundPlane);
					const TLiveActor* sand = body->getSandActor_();
					if (sand) {
						JGeometry::TVec3<f32> difference
						    = sand->mPosition - mPosition;
						if (difference.x * difference.x
						        + difference.z * difference.z
						    <= CLBSquared(50.0f)) {
							body->unk120 = 0.0f;
						} else {
							f32 relative = MsAngleDiff(
							    MsGetRotFromZaxisY(difference), mRotation.y);
							f32 absRelative = CLBAbs(relative);
							if (absRelative <= 15.0f || absRelative >= 165.0f)
								body->unk120 = 0.0f;
							else {
								f32 rise = SMS_GetSandRiseUpRatio(sand);
								if (relative > 15.0f)
									body->unk120 = 70.0f * rise;
								else
									body->unk120 = -70.0f * rise;
							}
						}
					} else {
						JGeometry::TVec3<f32> side(200.0f, 0.0f, 0.0f);
						s16 angle = CLBDegToShortAngle(body->mRotation.y);
						f32 sinY  = JMASSin(angle);
						f32 cosY  = JMASCos(angle);
						side.set(side.x * cosY + side.z * sinY, side.y,
						         -side.x * sinY + side.z * cosY);
						JGeometry::TVec3<f32> rightPosition(
						    side.x + body->mPosition.x,
						    side.y + body->mPosition.y,
						    side.z + body->mPosition.z);
						JGeometry::TVec3<f32> leftPosition(
						    -side.x + body->mPosition.x,
						    -side.y + body->mPosition.y,
						    -side.z + body->mPosition.z);
						const TBGCheckData* plane;
						f32 right = gpMap->checkGroundIgnoreWaterSurface(
						    rightPosition.x, 500.0f + rightPosition.y,
						    rightPosition.z, &plane);
						f32 left = gpMap->checkGroundIgnoreWaterSurface(
						    leftPosition.x, 500.0f + leftPosition.y,
						    leftPosition.z, &plane);
						f32 rightDiff = right - ground;
						f32 leftDiff  = left - ground;
						if (fabsf(rightDiff) < 0.001f
						    && fabsf(leftDiff) < 0.001f)
							body->unk120 = 0.0f;
						else if (fabsf(rightDiff) > fabsf(leftDiff))
							body->unk120 = (360.0f / 65536.0f)
							               * matan(200.0f, rightDiff);
						else
							body->unk120 = -((360.0f / 65536.0f)
							                 * matan(200.0f, leftDiff));
					}
				}
				if (nerve == &TNerveBossHanachanGetUp::theNerve()) {
					mHead->calcRotateZWhenGetUp_();
					mRotation.z = mHead->mRotation.z;
					for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i)
						mBodyParts[i]->calcRotateZWhenGetUp_();
				} else {
					f32 maxRotate = getBodyMaxRotateZ();
					const TNerveBase<TLiveActor>* current
					    = mSpine->getLatestNerve();
					f32 spacing = unk1BC->mSLBodyLength.get();
					f32 damping
					    = unk1C0->mSLWaveDecrease.get() * (1.0f / 120.0f);
					f32 velocity   = unk1C0->mSLWaveVelocity.get();
					f32 invSpacing = 1.0f / (spacing * spacing);
					f32 invDamping = 1.0f / (1.0f + damping);
					f32 oldDamping = 1.0f - damping;
					f32 wave       = (1.0f / 120.0f)
					           * ((1.0f / 120.0f) * (velocity * velocity));
					for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
						TBossHanachanPartsBody* body = mBodyParts[i];
						bool complete = (body->mRotation.z == -179.0f
						                 || body->mRotation.z == 179.0f)
						                    ? true
						                    : false;
						if (complete && body->mRotation.z == maxRotate)
							continue;
						f32 previous = i == 0 ? mBodyParts[i + 1]->unk13C
						                      : mBodyParts[i - 1]->unk13C;
						f32 next     = i == ARRAY_COUNT(mBodyParts) - 1
						                   ? mBodyParts[i - 1]->unk13C
						                   : mBodyParts[i + 1]->unk13C;
						f32 twice    = 2.0f * body->unk13C;
						f32 target
						    = (twice * invDamping)
						      + invDamping
						            * (wave
						               * (invSpacing
						                      * ((next + previous) - twice)
						                  + body->unk148))
						      - invDamping * (body->unk140 * oldDamping);
						target = MsClamp(target, -179.0f, 179.0f);
						CLBChaseGeneralConstantSpecifySpeed(
						    &body->mRotation.z, target,
						    unk1C0->mSLRotateZLeanSpeed.get());
						bool onSand = false;
						if (current
						    == &TNerveBossHanachanGraphWander::theNerve()) {
							if (body->getSandActor_()) {
								onSand = true;
								if (body->unk120 != 0.0f) {
									f32 speed
									    = body->unk120 * mMarchSpeed
									      * unk1C0->mSLSandSlopeForce.get();
									f32 fall = body->unk120 < 0.0f ? -179.0f
									                               : 179.0f;
									CLBChaseGeneralConstantSpecifySpeed(
									    &body->mRotation.z, fall, speed);
								}
							} else {
								CLBChaseGeneralConstantSpecifySpeed(
								    &body->mRotation.z, body->unk120,
								    unk1C0->mSLRotateZRestorationSpeed.get());
							}
						} else if (current
						           == &TNerveBossHanachanTumble::theNerve()) {
							CLBChaseGeneralConstantSpecifySpeed(
							    &body->mRotation.z, unk194, unk198);
						}
						body->mRotation.z
						    = MsClamp(body->mRotation.z, -179.0f, 179.0f);
						if (current
						        == &TNerveBossHanachanGraphWander::theNerve()
						    && (mSpine->getTime()
						            < unk1C0->mSLNotFallDownFrames.get()
						        || !onSand)) {
							f32 limit = unk1C0->mSLMaxRotateZNotSand.get();
							if (body->mRotation.z < -limit)
								CLBChaseGeneralConstantSpecifySpeed(
								    &body->mRotation.z, -limit, 15.0f);
							else if (body->mRotation.z > limit)
								CLBChaseGeneralConstantSpecifySpeed(
								    &body->mRotation.z, limit, 15.0f);
						}
					}
					f32 limit = unk1C0->mSLDiffMaxRotateZ.get();
					for (int i = 1; i < ARRAY_COUNT(mBodyParts); ++i) {
						f32 previous = mBodyParts[i - 1]->mRotation.z;
						f32& current = mBodyParts[i]->mRotation.z;
						if (fabsf(previous - current) > limit) {
							if (current < previous)
								current = previous - limit;
							else
								current = previous + limit;
						}
					}
				}
			}
			bool thrown = false;
			if (mHead->unk100->mWaterHitCounter >= 1)
				--mHead->unk100->mWaterHitCounter;
			for (int j = 0; j < mHead->unk100->getColNum(); ++j)
				if (mHead->unk100->getCollision(j)->isActorType(
				        ACTOR_TYPE_MARIO)) {
					throwMario_(mHead->unk100);
					thrown = true;
					break;
				}
			for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
				TBossHanachanPartsBody* body = mBodyParts[i];
				if (body->unk100->mWaterHitCounter >= 1)
					--body->unk100->mWaterHitCounter;
				if (!thrown)
					for (int j = 0; j < body->unk100->getColNum(); ++j)
						if (body->unk100->getCollision(j)->isActorType(
						        ACTOR_TYPE_MARIO)) {
							throwMario_(body->unk100);
							thrown = true;
							break;
						}
				for (int j = 0; j < ARRAY_COUNT(body->unk118); ++j) {
					TFootHitActor* foot = body->unk118[j];
					if (foot->mWaterHitCounter >= 1)
						--foot->mWaterHitCounter;
					if (!thrown)
						for (int k = 0; k < foot->getColNum(); ++k)
							if (foot->getCollision(k)->isActorType(
							        ACTOR_TYPE_MARIO)) {
								throwMario_(foot);
								thrown = true;
								break;
							}
				}
			}
			if ((graphics->unk0 & 2)
			    && nerve == &TNerveBossHanachanDead::theNerve()) {
				SMSGetMSound()->startSoundActor(MSD_SE_WT_BOSS_FADEAWAY,
				                                &mPosition, 0, nullptr, 0, 4);
				if (!checkLiveFlag(LIVE_FLAG_UNK100000))
					gpMSound->unk98->modBgm(1, 1);
			}
		}
		mHead->moveMapCollision_();
		for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i)
			mBodyParts[i]->moveMapCollision_();
	}
	if (cue & CUE_CALC_ANIM) {
		gpTargetArrow->unk14 = 0;
		if (!gpMarDirector->isInTalkNow()) {
			bool walking = false;
			if ((mSpine->getLatestNerve()
			     == &TNerveBossHanachanGraphWander::theNerve())
			    && mMarchSpeed > 0.001f)
				walking = true;
			JGeometry::TVec3<f32> head(mHead->unk108[0][3], mHead->unk108[1][3],
			                           mHead->unk108[2][3]);
			mHead->unk100->mPosition.set(
			    head.x, head.y - unk1BC->mSLHeadHitOffsetY.get(), head.z);
			if (walking) {
				if (!mHead->unk100->checkHitFilter(HIT_CATEGORY_PLAYER)) {
					mHead->unk100->onHitFilter(HIT_CATEGORY_PLAYER);
					mHead->unk104->remove();
				}
			} else if (mHead->unk100->checkHitFilter(HIT_CATEGORY_PLAYER)) {
				mHead->unk100->offHitFilter(HIT_CATEGORY_PLAYER);
				mHead->unk104->setUpTrans(head);
			}
			f32 footOffset = unk1BC->mSLFootHitOffsetY.get();
			f32 bodyOffset = unk1BC->mSLBodyHitOffsetY.get();
			for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
				TBossHanachanPartsBody* body = mBodyParts[i];
				body->unk154.set(body->unk108[0][3], body->unk108[1][3],
				                 body->unk108[2][3]);
				body->unk100->mPosition.set(body->unk154.x,
				                            body->unk154.y - bodyOffset,
				                            body->unk154.z);
				if (walking) {
					if (!body->unk100->checkHitFilter(HIT_CATEGORY_PLAYER)) {
						body->unk100->onHitFilter(HIT_CATEGORY_PLAYER);
						body->unk104->remove();
					}
				} else if (body->unk100->checkHitFilter(HIT_CATEGORY_PLAYER)) {
					body->unk100->offHitFilter(HIT_CATEGORY_PLAYER);
					body->unk104->setUpTrans(body->unk154);
				}
				for (int j = 0; j < ARRAY_COUNT(body->unk118); ++j) {
					TFootHitActor* foot = body->unk118[j];
					foot->mPosition.set(foot->unk6C[0][3],
					                    foot->unk6C[1][3] - footOffset,
					                    foot->unk6C[2][3]);
					if (walking)
						foot->onHitFilter(HIT_CATEGORY_PLAYER);
					else
						foot->offHitFilter(HIT_CATEGORY_PLAYER);
				}
			}
			emitParticle_();
			emitCamShake_();
		}
		if (checkLiveFlag(LIVE_FLAG_UNK10000)
		    && unk19C->curAnmEndsNext(0, nullptr))
			offLiveFlag(LIVE_FLAG_UNK10000);
		if (!gpMarDirector->isInTalkNow())
			changeAnmRateAndFrameUpdate_();
		mHead->unk110->execMotionBlend(mHead->getMActor());
		for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i)
			mBodyParts[i]->unk110->execMotionBlend(mBodyParts[i]->getMActor());
	}
	if (cue & CUE_CALC_ANIM) {
		execHeadCalcAnim_();
		execBodyCalcAnim_();
		if (!gpMarDirector->isInTalkNow()
		    && ((mSpine->getLatestNerve()
		         == &TNerveBossHanachanTumble::theNerve())
		        || (mSpine->getLatestNerve()
		            == &TNerveBossHanachanDown::theNerve()))) {
			MtxPtr matrix = mBodyParts[unk174]->unk108;
			JGeometry::TVec3<f32> position(matrix[0][3], 400.0f + matrix[1][3],
			                               matrix[2][3]);
			gpTargetArrow->setPos(position);
			gpTargetArrow->unk14 = 1;
		}
	}
	if (cue & CUE_ENTRY) {
		mHead->entryCircleShadow_();
		mHead->setDamageFog_(graphics);
		mHead->drawObject(graphics);
		for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
			TBossHanachanPartsBody* body = mBodyParts[i];
			body->entryCircleShadow_();
			body->setDamageFog_(graphics);
			body->drawObject(graphics);
		}
	}
	if (cue & CUE_CALC_VIEW) {
		mHead->getMActor()->viewCalc();
		for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i)
			mBodyParts[i]->getMActor()->viewCalc();
	}
	if (checkLiveFlag(LIVE_FLAG_UNK10000))
		unk19C->perform(cue, graphics);
}

bool TBossHanachan::isTumbleCompletelyAllBody() const
{
	bool result   = true;
	f32 degreeZ   = mBodyParts[0]->mRotation.z;
	bool complete = (degreeZ == -179.0f || degreeZ == 179.0f) ? true : false;
	if (!complete)
		result = false;
	else
		for (int i = 1; i < ARRAY_COUNT(mBodyParts); ++i)
			if (mBodyParts[i]->mRotation.z != degreeZ) {
				result = false;
				break;
			}
	return result;
}

// TODO: frame offsets and commuted operands of the fall-speed multiply differ.
bool TBossHanachan::checkFallDecideAndSetup()
{
	bool result = false;
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
		TBossHanachanPartsBody* body = mBodyParts[i];
		if (CLBAbs(body->mRotation.z) > unk1C0->mSLFallDecideRotateZ.get()) {
			emitOneTimeSandPillar_(body);
			if (body->mRotation.z > 0.0f)
				unk194 = 179.0f;
			else
				unk194 = -179.0f;
			f32 speed    = CLBAbs(body->unk13C - body->mRotation.z);
			unk198       = unk1C0->mSLWaveFallDownSpeed.get() * speed;
			f32 minSpeed = unk1C0->mSLFallDecideMinSpeed.get();
			if (unk198 < minSpeed)
				unk198 = minSpeed;
			result = true;
			break;
		}
	}
	return result;
}

f32 TBossHanachan::getBodyMaxRotateZ() const
{
	f32 result = 0.0f;
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i)
		if (fabsf(mBodyParts[i]->mRotation.z) > fabsf(result))
			result = mBodyParts[i]->mRotation.z;
	return result;
}

// TODO: recover the out-of-line TVec3::set<f32> call.
bool TBossHanachan::isCanWalk() const
{
	bool result                = true;
	JGeometry::TVec3<f32> goal = unkF4.getPoint();
	f32 squareToGoal = JGeometry::TVec3<f32>(goal.x - mPosition.x, 0.0f,
	                                         goal.z - mPosition.z)
	                       .squared();
	if (squareToGoal < CLBSquared(10.0f))
		result = false;
	return result;
}

void TBossHanachan::execWalk(bool accelerate)
{
	if (accelerate)
		CLBChaseGeneralConstantSpecifySpeed(&mMarchSpeed,
		                                    unk1C0->mSLMaxMarchSpeed.get(),
		                                    unk1C0->mSLMarchAccel.get());
	else
		CLBChaseGeneralConstantSpecifySpeed(&mMarchSpeed, 0.0f,
		                                    unk1C0->mSLMarchDecrease.get());
	mTurnSpeed = unk1C0->mSLWalkTurnSpeed.get();
	if (isCanWalk())
		walkToCurPathNode(mMarchSpeed, mTurnSpeed, 0.0f);
	JGeometry::TVec3<f32> direction = unkF4.getPoint();
	direction.set(direction.x - mPosition.x, 0.0f, direction.z - mPosition.z);
	f32 squareToGoal = direction.squared();
	if (squareToGoal < CLBSquared(100.0f)) {
		if (!unk114.empty())
			switchNextGoalPath();
		else
			goToDirLimitedNextGraphNode(90.0f);
	}
}

void TBossHanachan::execSlip()
{
	CLBChaseGeneralConstantSpecifySpeed(&mMarchSpeed, 0.0f,
	                                    unk1C0->mSLMarchDecrease.get());
	mTurnSpeed = 0.1f;
	if (mMarchSpeed > 0.001f) {
		JGeometry::TVec3<f32> direction = unk188;
		if (mMarchSpeed > 4.0f) {
			f32 degreeZ = getBodyMaxRotateZ();
			JGeometry::TVec3<f32> side(1.0f, 0.0f, 0.0f);
			if (degreeZ > 0.0f)
				side.negate();
			s16 angle = CLBDegToShortAngle(mRotation.y);
			f32 sinY  = JMASSin(angle);
			f32 cosY  = JMASCos(angle);
			JGeometry::TVec3<f32> rotated(side.x * cosY + side.z * sinY, side.y,
			                              -side.x * sinY + side.z * cosY);
			rotated.scale(0.005f * mMarchSpeed);
			direction.add(rotated);
		}
		bool stationary = direction.isZero();
		if (!stationary) {
			MsVECNormalize(&direction, &direction);
			direction.scale(500.0f);
			JGeometry::TVec3<f32> goal = mPosition;
			goal.add(direction);
			setGoalPath(TPathNode(goal));
			walkToCurPathNode(mMarchSpeed, mTurnSpeed, 0.0f);
		}
		gpCameraShake->keepShake(CAM_SHAKE_MODE_UNK9, 1.0f);
		if (SMS_IsMarioTouchGround4cm() && mSpine->getTime() < 120)
			SMSRumbleMgr->start(0x16, (f32*)nullptr);
	}
}

void TBossHanachan::goToInitialRecoverGraphNode()
{
	unk124->reset();
	unk124->reset2();
	int index = unk124->getGraph()->findNearestVisibleIndex(
	    mPosition, mRotation.y, unk1BC->mSLRecoverSearchDist.get(),
	    unk1BC->mSLRecoverSearchDegree.get(), -1);
	if (index < 0) {
		goToShortestNextGraphNode();
		return;
	}
	unk124->setTo(index);
	setGoalPathFromGraph();
	unk128 = 0;
	unk12C = 0.0f;
}

// fabricated
static inline void disableHitActors(TBossHanachan* boss)
{
	boss->mHead->unk100->onHitFilter(HIT_FILTER_NO_COLLISION);
	for (int i = 0; i < ARRAY_COUNT(boss->mBodyParts); ++i) {
		boss->mBodyParts[i]->unk100->onHitFilter(HIT_FILTER_NO_COLLISION);
		boss->mBodyParts[i]->unk118[0]->onHitFilter(HIT_FILTER_NO_COLLISION);
		boss->mBodyParts[i]->unk118[1]->onHitFilter(HIT_FILTER_NO_COLLISION);
	}
}

void TBossHanachan::execDamage()
{
	mSpine->reset();
	decHitPoints();
	if (mHitPoints == 0) {
		disableHitActors(this);
		mSpine->setNext(&TNerveBossHanachanDead::theNerve());
		setAnmTimerWhenDead();
		unk1AC = SMS_GetMarioPos();
		SMSGetMSound()->startSoundActor(MSD_SE_BS_HANA_DOWN, &unk1AC, 0,
		                                nullptr, 0, 4);
	} else {
		mSpine->setNext(&TNerveBossHanachanDamage::theNerve());
		setAnmTimerWhenDamage();
		unk1C0 = ((TBossHanachanManager*)mManager)
		             ->getChangeSaveParams(3 - mHitPoints);
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
		SMSGetMSound()->startSoundActor(MSD_SE_BS_DAMAGE_SYMBL,
		                                &mBodyParts[unk174]->unk154, 0, nullptr,
		                                0, 4);
	}
}

void TBossHanachan::removeAllMapCollision()
{
	mHead->unk104->remove();
	for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i)
		mBodyParts[i]->unk104->remove();
}

BOOL TBossHanachan::hasMapCollision() const { return TRUE; }
