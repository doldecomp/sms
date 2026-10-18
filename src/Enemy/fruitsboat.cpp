#include <Enemy/FruitsBoat.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Graph.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorData.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <MoveBG/MapObjWave.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Yoshi.hpp>
#include <Strategic/ActorTypes.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/question.hpp>
#include <Strategic/Spine.hpp>
#include <System/Application.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <string.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

TFruitsBoatParams::TFruitsBoatParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mSLMoveSpeed, 4.0f)
    , PARAM_INIT(mSLRotSpeed, 0.1f)
    , PARAM_INIT(mSLBckMoveSpeed, 0.2f)
{
	TParams::load(mPrmPath);
}

TFruitsBoat::TFruitsBoat(const char* name)
    : TSpineEnemy(name)
    , unk150(0)
    , unk154(800.0f)
    , unk158(800.0f)
    , unk15C(nullptr)
    , unk160(nullptr)
    , unk164(1.0f, 0.0f, 0.0f)
    , unk170(0.0f)
    , unk174(0.0f)
{
	onLiveFlag(LIVE_FLAG_UNK10);
}

int TFruitsBoat::setBckTrack(const char* name)
{
	MActorAnmDataEach<J3DAnmTransformKey>* bcks
	    = mManager->getMActorAnmData()->mBckAnms;

	for (int i = 0; i < bcks->mAnmNum; ++i) {
		if (strcmp(name, bcks->mAnmNames[i]) == 0) {
			unk15C = bcks->getAnmPtr(i);
			unk160 = new J3DFrameCtrl;
			unk160->init(unk15C->getFrameMax());
			unk160->setAttribute(unk15C->getAttribute());
			f32 speed
			    = ((TFruitsBoatParams*)getSaveParam())->mSLBckMoveSpeed.get();
			unk160->setRate(speed * SMSGetAnmFrameRate());
			return 0;
		}
	}

	return -1;
}

void TFruitsBoat::setJumpReaction()
{
	const char* bck;
	switch (getBoatType()) {
	case 0:
		bck = "shipdolpic";
		break;
	case 1:
		bck = "shipdolpic2";
		break;
	case 2:
		bck = "shipdolpic3";
		break;
	case 3:
	default:
		return;
	}

	if (!mMActor->checkCurAnm(bck, ANM_TYPE_BCK) || mMActor->curAnmEndsNext())
		mMActor->setBck(bck);
}

void TFruitsBoat::traceBckTrack()
{
	unk160->update();
	unk15C->setFrame(unk160->getFrame());

	J3DTransformInfo info0;
	J3DTransformInfo info1;
	unk15C->getTransform(0, &info0);
	unk15C->getTransform(1, &info1);

	mPosition.x = info0.mTranslate.x + info1.mTranslate.x;
	mPosition.y = info0.mTranslate.y + info1.mTranslate.y;
	mPosition.z = info0.mTranslate.z + info1.mTranslate.z;

	mRotation.x = (360.0f / 65536.0f) * (info0.mRotation.x + info1.mRotation.x);
	mRotation.y = (360.0f / 65536.0f) * (info0.mRotation.y + info1.mRotation.y);
	mRotation.z = (360.0f / 65536.0f) * (info0.mRotation.z + info1.mRotation.z);

	mScaling.x = info0.mScale.x * info1.mScale.x;
	mScaling.y = info0.mScale.y * info1.mScale.y;
	mScaling.z = info0.mScale.z * info1.mScale.z;
}

int TFruitsBoat::getBoatType() const
{
	return ((TFruitsBoatManager*)mManager)->unk54;
}

Mtx* TFruitsBoat::getRootJointMtx() const
{
	return (Mtx*)mMActor->getModel()->getAnmMtx(0);
}

void TFruitsBoat::rowToCurPathNode(f32 speed)
{
	if (unk124->unk0->unk14 ? TRUE : FALSE) {
		f32 splineSpeed = unk124->calcSplineSpeed(speed);
		unk124->traceSpline(splineSpeed);

		JGeometry::TVec3<f32> pos;
		JGeometry::TVec3<f32> rot;
		unk124->unk0->unk14->getPosAndRot(unk124->unk14, &pos, &rot);
		pos.sub(mPosition);
		mPositionDelta.add(pos);
		mRotation.y = rot.y;

		if (splineSpeed < 0.0f)
			mRotation.y = MsWrap(180.0f + mRotation.y, 0.0f, 360.0f);
	} else {
		walkToCurPathNode(speed, mTurnSpeed, 0.0f);
	}

	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_PONPONSEN, &mPosition, 0,
	                                nullptr, 0, 4);
}

void TFruitsBoat::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);

	char buffer[256];
	stream.readString(buffer, 256);
	TLiveManager* mgr
	    = static_cast<TLiveManager*>(JDrama::TNameRefGen::search(buffer));

	char buffer2[256];
	stream.readString(buffer2, 256);
	TGraphWeb* graph = gpConductor->getGraphByName(buffer2);
	unk124->init(graph);

	mGroundPlane = TMap::getIllegalCheckData();

	init(mgr);

	if (!graph || graph->isDummy()) {
		if (setBckTrack(buffer2) == 0)
			mSpine->initWith(&TNerveFruitsBoatBckTrace::theNerve());
	}
}

void TFruitsBoat::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 1);

	initHitActor(ACTOR_TYPE_MAP_OBJECT_UNK7B, 1,
	             HIT_CATEGORY_PLAYER | HIT_CATEGORY_MAP_OBJECT, 0.0f, 0.0f,
	             0.0f, 0.0f);
	offHitFilter(HIT_FILTER_NO_COLLISION);

	unk124->reset();
	goToShortestNextGraphNode();
	if (unk124->unk0->unk14) {
		mPosition = unk124->getCurrentPos();
		unk124->moveToShortestNext();
	}

	switch (getBoatType()) {
	case 0:
		mMapCollisionManager
		    = new TMapCollisionManager(1, "/scene/fruitsboat", this);
		mMActor = mMActorKeeper->createMActor("ShipDolpic.bmd", 0);
		mMapCollisionManager->init("ShipDolpic.col", 1, nullptr);
		setAttackRadius(850.0f);
		setAttackHeight(600.0f);
		break;
	case 1:
		mMapCollisionManager
		    = new TMapCollisionManager(1, "/scene/fruitsboatb", this);
		mMActor = mMActorKeeper->createMActor("ShipDolpic2.bmd", 0);
		mMapCollisionManager->init("ShipDolpic2.col", 1, nullptr);
		setAttackRadius(750.0f);
		setAttackHeight(480.0f);
		break;
	case 2:
		mMapCollisionManager
		    = new TMapCollisionManager(1, "/scene/fruitsboatc", this);
		mMActor = mMActorKeeper->createMActor("ShipDolpic3.bmd", 0);
		mMapCollisionManager->init("ShipDolpic3.col", 1, nullptr);
		setAttackRadius(1000.0f);
		setAttackHeight(300.0f);
		break;
	case 3:
	default:
		mMapCollisionManager
		    = new TMapCollisionManager(1, "/scene/fruitsboatd", this);
		mMActor = mMActorKeeper->createMActor("ShipDolpic4.bmd", 0);
		mMapCollisionManager->init("ShipDolpic4.col", 1, nullptr);
		setAttackRadius(760.0f);
		setAttackHeight(270.0f);
		break;
	}

	mMapCollisionManager->setUpActiveCollisionTRS(mPosition, mRotation,
	                                              mScaling);
	mSpine->initWith(&TNerveFruitsBoatGraphWander::theNerve());

	if (!unk124->unk0)
		onLiveFlag(LIVE_FLAG_UNK8000);
	else if (unk124->getCurrent().checkFlag(0x80))
		onLiveFlag(LIVE_FLAG_UNK8000);
	else
		offLiveFlag(LIVE_FLAG_UNK8000);

	mMarchSpeed = ((TFruitsBoatParams*)getSaveParam())->mSLMoveSpeed.get();
	mTurnSpeed  = ((TFruitsBoatParams*)getSaveParam())->mSLRotSpeed.get();

	offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
	onLiveFlag(LIVE_FLAG_UNK20);
	offLiveFlag(LIVE_FLAG_ENABLE_CLIPPING);
	mMActor->setLightType(2);
	calcRootMatrix();
	mMActor->calc();
}

BOOL TFruitsBoat::receiveMessage(THitActor* sender, u32 message)
{
	return FALSE;
}

void TFruitsBoat::setGroundCollision()
{
	JGeometry::TVec3<f32> diff = mPosition;
	diff.sub(*gpMarioPos);

	if (mColCount != 0 || diff.length() < 1000.0f
	    || (SMS_GetYoshi()->isHatched()
	        && mPosition.x - 1000.0f < SMS_GetYoshi()->getTranslation().x
	        && mPosition.x + 1000.0f > SMS_GetYoshi()->getTranslation().x
	        && mPosition.z - 1000.0f < SMS_GetYoshi()->getTranslation().z
	        && mPosition.z + 1000.0f > SMS_GetYoshi()->getTranslation().z)) {
		mMapCollisionManager->moveActiveCollisionMtx(getModel()->getAnmMtx(0));
	}
}

void TFruitsBoat::calcRootMatrix()
{
	J3DModel* model = getModel();
	MtxPtr mtx      = model->getBaseTRMtx();
	MsMtxSetRotRPH(mtx, mRotation.x, mRotation.y, mRotation.z);

	Mtx rot;
	MTXRotAxisRad(rot, &unk164, 0.017453292f * unk170);
	MTXConcat(rot, mtx, mtx);
	MTXTransApply(mtx, mtx, mPosition.x, mPosition.y, mPosition.z);
	model->setBaseScale(mScaling);
}

static inline JGeometry::TVec3<f32> polarXZ(f32 theta, f32 radius)
{
	f32 c = radius * MsCos(theta);
	f32 s = radius * MsSin(theta);
	return JGeometry::TVec3<f32>(s, 0.0f, c);
}

void TFruitsBoat::moveObject()
{
	JGeometry::TVec3<f32> offset = polarXZ(mRotation.y, 300.0f);
	JGeometry::TVec3<f32> pos    = mPosition;
	JGeometry::TVec3<f32> front  = pos;
	front.add(offset);
	JGeometry::TVec3<f32> back = pos;
	back.sub(offset);
	front.y = gpMapObjWave->getWaveHeight(front.x, front.z);
	back.y  = gpMapObjWave->getWaveHeight(back.x, back.z);

	JGeometry::TVec3<f32> dir = front - back;
	JGeometry::TVec3<f32> rot = MsGetRotFromZaxis(dir);
	rot.x *= 0.5f;

	f32 step = MsAngleDiff(rot.x, mRotation.x);
	if (step >= 0.0f)
		step = MsMin(step, 1.0f);
	else
		step = MsMax(step, -1.0f);
	mRotation.x += step;

	const TBGCheckData* plane = SMS_GetMarioGrPlane();
	if (!checkLiveFlag(LIVE_FLAG_UNK10000)) {
		if (plane && plane->mActor == this && SMS_IsMarioTouchGround4cm()) {
			JGeometry::TVec3<f32> toMario = *gpMarioPos;
			toMario.sub(mPosition);
			toMario.y = 0.0f;
			f32 dist  = toMario.length();
			if (dist != 0.0f) {
				static JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
				toMario.normalize();
				unk164.cross(up, toMario);
				unk164.normalize();
				unk174 += 0.0003f * dist;
			}
			onLiveFlag(LIVE_FLAG_UNK10000);
			offLiveFlag(LIVE_FLAG_UNK8000);
			if (plane->isBounceOnLanding()) {
				setJumpReaction();
				offLiveFlag(LIVE_FLAG_UNK10000);
			}
		}
	} else if (!plane || plane->mActor != this
	           || !SMS_IsMarioTouchGround4cm()) {
		offLiveFlag(LIVE_FLAG_UNK10000);
	}

	if (checkLiveFlag(LIVE_FLAG_UNK10000)) {
		JGeometry::TVec3<f32> toMario = *gpMarioPos;
		toMario.sub(mPosition);
		toMario.y = 0.0f;
		if (toMario.length() != 0.0f) {
			toMario.normalize();
			static JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
			JGeometry::TVec3<f32> axis;
			axis.cross(up, toMario);
			axis.normalize();
			axis.sub(unk164);
			axis.scale(0.1f);
			unk164.add(axis);
		}
	}

	unk174 += 0.01f * -MsSin(unk170);
	unk170 += unk174;
	if (unk170 < -8.0f) {
		unk170 = -8.0f;
		unk174 = -unk174;
	} else if (unk170 > 8.0f) {
		unk170 = 8.0f;
		unk174 = -unk174;
	}
	unk174 *= 0.99f;

	TLiveActor::moveObject();
}

void TFruitsBoat::requestShadow()
{
	if (mLiveFlag & (LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN | LIVE_FLAG_UNK8))
		return;

	if (!(mLiveFlag & (LIVE_FLAG_UNK200 | LIVE_FLAG_CLIPPED_OUT))
	    || (mLiveFlag & LIVE_FLAG_FORCE_SHADOW)) {
		TCircleShadowRequest shadow;
		shadow.mPosition   = mPosition;
		shadow.mRadiusX    = unk154;
		shadow.mRadiusZ    = unk158;
		shadow.mShadowType = SHADOW_TYPE_SHIP;
		shadow.mRotationY  = (s16)mRotation.y;

		if (mLiveFlag & LIVE_FLAG_FORCE_SHADOW)
			gpBindShadowManager->forceRequest(shadow, getActorType());
		else
			gpBindShadowManager->request(shadow, getActorType());
	}

	if (!(mLiveFlag & (LIVE_FLAG_UNK200 | LIVE_FLAG_CLIPPED_OUT))
	    && !isHitCategory(HIT_CATEGORY_MAP_OBJECT)) {
		gpQuestionManager->request(mPosition, mScaledBodyRadius);
	}
}

TFruitsBoatManager::TFruitsBoatManager(int type, const char* name)
    : TEnemyManager(name)
    , unk54(type)
{
}

void TFruitsBoatManager::createModelData()
{
	switch (unk54) {
	case 0: {
		static const TModelDataLoadEntry entry[] = {
			{ "ShipDolpic.bmd",
			  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
			      | (1 << J3DMLF_TevStageNumShift),
			  0 },
			{ nullptr, 0, 0 },
		};
		createModelDataArray(entry);
		break;
	}
	case 1: {
		static const TModelDataLoadEntry entry[] = {
			{ "ShipDolpic2.bmd",
			  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
			      | (1 << J3DMLF_TevStageNumShift),
			  0 },
			{ nullptr, 0, 0 },
		};
		createModelDataArray(entry);
		break;
	}
	case 2: {
		static const TModelDataLoadEntry entry[] = {
			{ "ShipDolpic3.bmd",
			  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
			      | (1 << J3DMLF_TevStageNumShift),
			  0 },
			{ nullptr, 0, 0 },
		};
		createModelDataArray(entry);
		break;
	}
	case 3:
	default: {
		static const TModelDataLoadEntry entry[] = {
			{ "ShipDolpic4.bmd",
			  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
			      | (1 << J3DMLF_TevStageNumShift),
			  0 },
			{ nullptr, 0, 0 },
		};
		createModelDataArray(entry);
		break;
	}
	}
}

void TFruitsBoatManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TFruitsBoatParams("/enemy/fruitsBoat.prm");
	TEnemyManager::load(stream);
}

TSpineEnemy* TFruitsBoatManager::createEnemyInstance() { return nullptr; }

DEFINE_NERVE(TNerveFruitsBoatGraphWander, TLiveActor)
{
	TFruitsBoat* self = (TFruitsBoat*)spine->getBody();

	if (!self->unk124->unk0 || self->unk124->unk0->isDummy())
		return false;

	if (self->isReachedToGoal()) {
		TGraphNode& node = self->unk124->getCurrent();
		if (node.checkFlag(0x100))
			self->onLiveFlag(LIVE_FLAG_UNK8000);
		if (node.checkFlag(0x400))
			self->unk150 ^= 1;

		self->goToDirectedNextGraphNode(polarXZ(self->mRotation.y, 1.0f));

		if (!self->checkLiveFlag(LIVE_FLAG_UNK8000))
			self->rowToCurPathNode(self->mMarchSpeed);

		spine->pushAfterCurrent(&TNerveFruitsBoatGraphWander::theNerve());
		return true;
	}

	if (self->checkLiveFlag(LIVE_FLAG_UNK8000))
		return false;

	self->rowToCurPathNode(self->mMarchSpeed);
	return false;
}

DEFINE_NERVE(TNerveFruitsBoatBckTrace, TLiveActor)
{
	TFruitsBoat* self = (TFruitsBoat*)spine->getBody();
	self->traceBckTrack();
	return false;
}
