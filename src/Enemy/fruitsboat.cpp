#include <algorithm>

#include <Enemy/FruitsBoat.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Conductor.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/question.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorData.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <MoveBG/MapObjWave.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Yoshi.hpp>
#include <System/Params.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// fabricated. Both call sites keep the multiplication by the length argument,
// even the one that passes 1.0f, and MWCC only leaves a multiply by literal
// one alone when the constant arrives across an inline boundary -- so the
// direction vector was built by a helper taking the length. It is local to the
// TU because the map lists no symbol for it anywhere.
static inline JGeometry::TVec3<f32> MsGetVecFromRotY(f32 rot_y, f32 length)
{
	return JGeometry::TVec3<f32>(length * JMASSin(DEG2SHORTANGLE(rot_y)),
	                             0.0f,
	                             length * JMASCos(DEG2SHORTANGLE(rot_y)));
}

TFruitsBoatParams::TFruitsBoatParams(const char* prm)
    : TSpineEnemyParams(prm)
    , PARAM_INIT(mSLMoveSpeed, 4.0f)
    , PARAM_INIT(mSLRotSpeed, 0.1f)
    , PARAM_INIT(mSLBckMoveSpeed, 0.2f)
{
	TParams::load(mPrmPath);
}

TFruitsBoat::TFruitsBoat(const char* name)
    : TSpineEnemy(name)
    , mReversed(0)
    , mShadowRadiusX(800.0f)
    , mShadowRadiusZ(800.0f)
    , mBckTrack(nullptr)
    , mBckTrackCtrl(nullptr)
    , mRollAxis(1.0f, 0.0f, 0.0f)
    , mRollAngle(0.0f)
    , mRollSpeed(0.0f)
{
	onLiveFlag(LIVE_FLAG_UNK10);
}

// Looks the requested animation up in the manager's shared .bck table and, if
// it is there, drives it by hand through our own J3DFrameCtrl. Returns 0 on
// success, -1 when the table has no such animation.
int TFruitsBoat::setBckTrack(const char* name)
{
	MActorAnmDataEach<J3DAnmTransformKey>* bcks
	    = mManager->getMActorAnmData()->getUnk2C();

	for (int i = 0; i < bcks->getAnmNum(); ++i) {
		if (strcmp(name, bcks->getName(i)) == 0) {
			mBckTrack     = bcks->getAnmPtr(i);
			mBckTrackCtrl = new J3DFrameCtrl();
			mBckTrackCtrl->init(mBckTrack->getFrameMax());
			mBckTrackCtrl->setAttribute(mBckTrack->getAttribute());
			// TODO: instruction-identical, frame 0x60 against the ROM's
			// 0x58. `speed` is what keeps the param load in f31 ahead of
			// SMSGetAnmFrameRate() (without it the load sinks past the
			// call), but it also costs 8 bytes the ROM does not spend, so
			// one of the inlined accessor temporaries above is one object
			// too many. Dropping the getSaveParams()/get() wrapper levels,
			// reading mAnmNum/mAnmNames directly, naming the frame rate or
			// the product, and hoisting the declaration all left it at 0x60.
			f32 speed;
			speed = getSaveParams()->getSLBckMoveSpeed();
			mBckTrackCtrl->setRate(speed * SMSGetAnmFrameRate());
			return 0;
		}
	}

	return -1;
}

// TODO: guessed body, 0xc4 against the map's 0xb8. UNUSED and referenced from
// nowhere inside the TU, so the only evidence is the size and the single
// private float literal it owns (@3017, dead-stripped along with it).
// Modelled on moveObject()'s "Mario came aboard" branch, which kicks
// mRollSpeed by the distance he is standing off-centre the same way; the
// constant is a guess. Measured shapes: with an `if (dist != 0.0f)` guard
// 0xd0, without the y flatten 0xc0, and with the two-argument sub() instead
// of copy-then-sub only 0x7c.
void TFruitsBoat::setJumpReaction()
{
	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario.sub(mPosition);
	toMario.y = 0.0f;

	mRollSpeed += 0.002f * toMario.length();
}

// Replays mBckTrack by hand: joint 0 and joint 1 of the track carry the boat's
// position, rotation and scaling as a sum/product pair.
void TFruitsBoat::traceBckTrack()
{
	mBckTrackCtrl->update();
	mBckTrack->setFrame(mBckTrackCtrl->getFrame());

	J3DTransformInfo base;
	mBckTrack->getTransform(0, &base);

	J3DTransformInfo offset;
	mBckTrack->getTransform(1, &offset);

	mPosition.x = base.mTranslate.x + offset.mTranslate.x;
	mPosition.y = base.mTranslate.y + offset.mTranslate.y;
	mPosition.z = base.mTranslate.z + offset.mTranslate.z;

	mRotation.x
	    = (360.0f / 65536.0f) * (base.mRotation.x + offset.mRotation.x);
	mRotation.y
	    = (360.0f / 65536.0f) * (base.mRotation.y + offset.mRotation.y);
	mRotation.z
	    = (360.0f / 65536.0f) * (base.mRotation.z + offset.mRotation.z);

	mScaling.x = base.mScale.x * offset.mScale.x;
	mScaling.y = base.mScale.y * offset.mScale.y;
	mScaling.z = base.mScale.z * offset.mScale.z;
}

int TFruitsBoat::getBoatType() const
{
	return ((TFruitsBoatManager*)mManager)->getBoatType();
}

Mtx* TFruitsBoat::getRootJointMtx() const
{
	return (Mtx*)getMActor()->getModel()->getAnmMtx(0);
}

// Rows one step towards the current path node: along the graph's spline if it
// has one, otherwise by walking the graph link.
//
// UNUSED (0x17c in the map) and dead: TNerveFruitsBoatGraphWander::execute
// spells the same body out at both of its call sites instead of calling this,
// and both of those copies are byte-identical to the ROM's. This out-of-line
// copy compiles to 0x154, so the original's dead version is not exactly what
// the nerve pastes -- 0x28 of the difference is unaccounted for.
void TFruitsBoat::rowToCurPathNode(f32 turn_speed)
{
	if (checkLiveFlag(LIVE_FLAG_UNK10000))
		return;

	if (getTracer()->getGraph()->getSplineRail() != nullptr) {
		f32 speed = getTracer()->calcSplineSpeed(mMarchSpeed);
		getTracer()->traceSpline(speed);

		JGeometry::TVec3<f32> rot;
		JGeometry::TVec3<f32> pos;
		getTracer()->getGraph()->getSplineRail()->getPosAndRot(
		    getTracer()->unk14, &pos, &rot);

		pos.sub(mPosition);
		mLinearVelocity.add(pos);

		mRotation.y = rot.y;
		if (speed < 0.0f)
			mRotation.y = MsAngleWrap(180.0f + mRotation.y);
	} else {
		walkToCurPathNode(mMarchSpeed, turn_speed, 0.0f);
	}

	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_PONPONSEN, &mPosition, 0,
	                                nullptr, 0, 4);
}

void TFruitsBoat::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);

	char managerName[256];
	stream.readString(managerName, 256);
	TLiveManager* manager
	    = JDrama::TNameRefGen::search<TLiveManager>(managerName);

	char graphName[256];
	stream.readString(graphName, 256);
	TGraphWeb* web = gpConductor->getGraphByName(graphName);
	unk124->init(web);

	mGroundPlane = TMap::getIllegalCheckData();

	init(manager);

	// No graph of that name: the route is a .bck animation instead.
	if (web == nullptr || web->isDummy()) {
		if (setBckTrack(graphName) == 0)
			mSpine->initWith(&TNerveFruitsBoatBckTrace::theNerve());
	}
}

// TODO: instruction-identical, frame 0xf0 against the ROM's 0xf8. Eight bytes
// of locals are missing: routing more member reads through their accessors
// (getMapCollisionManager, getMActor, getTracer) does not add any further
// temporaries, so the two missing objects are named locals the original
// declared and this reconstruction does not need.
void TFruitsBoat::init(TLiveManager* manager)
{
	mManager = manager;
	getManager()->manageActor(this);

	mMActorKeeper = new TMActorKeeper(getManager(), 1);

	initHitActor(0x4000007B, 1, ACTOR_TYPE_UNK40000000 | ACTOR_TYPE_PLAYER,
	             0.0f, 0.0f, 0.0f, 0.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);

	getTracer()->reset();
	goToShortestNextGraphNode();

	if (getTracer()->getGraph()->getSplineRail()) {
		mPosition = getTracer()->getCurrentPos();
		getTracer()->moveToShortestNext();
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

	mMapCollisionManager->setUpUnk8TRS(getPosition(), getRotation(), mScaling);

	mSpine->initWith(&TNerveFruitsBoatGraphWander::theNerve());

	if (getTracer()->getGraph() == nullptr)
		onLiveFlag(LIVE_FLAG_UNK10000);
	else if (getTracer()->getCurrent().getRailNode()->mFlags & 0x80)
		onLiveFlag(LIVE_FLAG_UNK10000);
	else
		offLiveFlag(LIVE_FLAG_UNK10000);

	mMarchSpeed = getSaveParams()->getSLMoveSpeed();
	mTurnSpeed  = getSaveParams()->getSLRotSpeed();

	offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
	onLiveFlag(LIVE_FLAG_UNK20);
	offLiveFlag(LIVE_FLAG_UNK100);

	getMActor()->setLightType(2);
	calcRootMatrix();
	getMActor()->calc();
}

BOOL TFruitsBoat::receiveMessage(THitActor* sender, u32 message)
{
	return FALSE;
}

// TODO: instruction-identical apart from `toMario` sitting at 0x44 where the
// ROM has it at 0x40; the frame total already agrees, so one 4-byte named
// local of the original is still missing from below it. Naming the distance
// instead of testing it inline changes the sqrt codegen, and naming the
// getUnk8() result adds an `mr r3, r0`.
void TFruitsBoat::setGroundCollision()
{
	// `model` and `mtx` have to be separate locals: `mtx` alone puts the
	// matrix fetch after the null test, and folding both into the call
	// expression re-orders the argument setup.
	J3DModel* model;
	MtxPtr mtx;

	JGeometry::TVec3<f32> toMario = mPosition;
	toMario.sub(*gpMarioPos);

	// Nothing is touching us and Mario is far away: only keep the collision
	// alive while Yoshi could still be standing on the deck.
	if (mColCount == 0 && !(toMario.length() < 1000.0f)) {
		if (!SMS_GetYoshi()->isHatched())
			return;
		if (!(mPosition.x - 1000.0f < SMS_GetYoshi()->getTranslation().x))
			return;
		if (!(1000.0f + mPosition.x > SMS_GetYoshi()->getTranslation().x))
			return;
		if (!(mPosition.z - 1000.0f < SMS_GetYoshi()->getTranslation().z))
			return;
		if (!(1000.0f + mPosition.z > SMS_GetYoshi()->getTranslation().z))
			return;
	}

	model = getModel();
	mtx   = model->getAnmMtx(0);
	if (mMapCollisionManager->getUnk8())
		mMapCollisionManager->getUnk8()->moveMtx(mtx);
}

void TFruitsBoat::calcRootMatrix()
{
	J3DModel* model = getModel();
	MtxPtr mtx      = model->getBaseTRMtx();

	MsMtxSetRotRPH(mtx, mRotation.x, mRotation.y, mRotation.z);

	Mtx roll;
	MTXRotAxisRad(roll, &mRollAxis, 0.017453292f * mRollAngle);
	MTXConcat(roll, mtx, mtx);
	MTXTransApply(mtx, mtx, mPosition.x, mPosition.y, mPosition.z);

	model->setBaseScale(mScaling);
}

// TODO: 93.7%. Everything matches except the "Mario is aboard" distance: the
// ROM keeps the squared length in f1 and lets the following normalize() reuse
// it, so it spends one `fmr f31, f1` and no second squared(); ours coalesces
// the sum into f31, the value dies, and both normalize() sites recompute
// squared() (nine instructions each). Spelling setLength() out by hand so the
// two share a named `dist2` removes the recompute but expands inv_sqrt, which
// the ROM calls. The knock-on is the `dir` component reloads in the bow/stern
// add/sub and the register numbering of the first cross product.
void TFruitsBoat::moveObject()
{
	// Pitch: sample the wave surface 300 units ahead of and behind the hull
	// and turn the line between the two samples into a rotation.
	JGeometry::TVec3<f32> dir = MsGetVecFromRotY(mRotation.y, 300.0f);

	JGeometry::TVec3<f32> center = getPosition();

	JGeometry::TVec3<f32> bow = center;
	bow.add(dir);

	JGeometry::TVec3<f32> stern = center;
	stern.sub(dir);

	bow.y   = gpMapObjWave->getWaveHeight(bow.x, bow.z);
	stern.y = gpMapObjWave->getWaveHeight(stern.x, stern.z);

	JGeometry::TVec3<f32> slope = bow - stern;

	JGeometry::TVec3<f32> rot = MsGetRotFromZaxis(slope);
	rot.x                     = 0.5f * rot.x;

	// TODO: the ROM re-reads rot.x from the stack after the MsWrap call, which
	// a `const f32&` first parameter on MsAngleDiff would produce -- but that
	// signature was tried in MathUtil.hpp and regresses nine functions in six
	// other units (the table is next to the declaration there). Spelling
	// MsAngleDiff out here as `rot.x - MsWrap(mRotation.x, rot.x - 180.0f,
	// rot.x + 180.0f)` does give the reload and is worse still (93.95% ->
	// 89.82%), so neither the helper's signature nor this statement is the
	// reason; something earlier has to leave rot.x live in memory.
	f32 pitchStep = MsAngleDiff(rot.x, mRotation.x);
	// The limits go through std::min/std::max rather than a ternary because the
	// ROM keeps 1.0f and -1.0f in .sdata, and a literal only lands there when
	// it has to be materialised to bind to a const reference.
	if (pitchStep >= 0.0f)
		pitchStep = std::min(pitchStep, 1.0f);
	else
		pitchStep = std::max(pitchStep, -1.0f);
	mRotation.x += pitchStep;

	const TBGCheckData* ground = SMS_GetMarioGrPlane();

	if (!checkLiveFlag(LIVE_FLAG_UNK20000)) {
		if (ground != nullptr && ground->mActor == this
		    && SMS_IsMarioTouchGround4cm()) {
			JGeometry::TVec3<f32> toMario = *gpMarioPos;
			toMario.sub(getPosition());
			toMario.y = 0.0f;

			f32 offCenter = toMario.length();
			if (offCenter != 0.0f) {
				static JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);

				toMario.normalize();
				mRollAxis.cross(up, toMario);
				mRollAxis.normalize();

				mRollSpeed += 0.0003f * offCenter;
			}

			onLiveFlag(LIVE_FLAG_UNK20000);
			offLiveFlag(LIVE_FLAG_UNK10000);

			if (ground->isBounceOnLanding()) {
				const char* anm;
				switch (getBoatType()) {
				case 0:
					anm = "shipdolpic";
					break;
				case 1:
					anm = "shipdolpic2";
					break;
				case 2:
					anm = "shipdolpic3";
					break;
				// Boat type 3 (ShipDolpic4) has no deck animation. The
				// ROM reaches the code after the animation block from the
				// default case directly, which is what the jump is for.
				case 3:
				default:
					goto noDeckAnm;
				}

				if (!getMActor()->checkCurAnm(anm, ANM_TYPE_BCK)
				    || getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
					getMActor()->setBck(anm);

			noDeckAnm:
				offLiveFlag(LIVE_FLAG_UNK20000);
			}
		}
	} else if (ground == nullptr || ground->mActor != this
	           || !SMS_IsMarioTouchGround4cm()) {
		offLiveFlag(LIVE_FLAG_UNK20000);
	}

	// Roll: while Mario is aboard, steer the roll axis towards the direction
	// he is standing off-centre.
	if (checkLiveFlag(LIVE_FLAG_UNK20000)) {
		JGeometry::TVec3<f32> toMario = *gpMarioPos;
		toMario.sub(mPosition);
		toMario.y = 0.0f;

		if (toMario.length() != 0.0f) {
			toMario.normalize();

			static JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);

			JGeometry::TVec3<f32> axis;
			axis.cross(up, toMario);
			axis.normalize();

			f32 dx = 0.1f * (axis.x - mRollAxis.x);
			mRollAxis.x += dx;
			f32 dy = 0.1f * (axis.y - mRollAxis.y);
			mRollAxis.y += dy;
			f32 dz = 0.1f * (axis.z - mRollAxis.z);
			mRollAxis.z += dz;
		}
	}

	mRollSpeed += 0.01f * -JMASin(mRollAngle);
	mRollAngle += mRollSpeed;

	if (mRollAngle < -8.0f) {
		mRollAngle = -8.0f;
		mRollSpeed = -mRollSpeed;
	} else if (mRollAngle > 8.0f) {
		mRollAngle = 8.0f;
		mRollSpeed = -mRollSpeed;
	}

	mRollSpeed *= 0.99f;

	TLiveActor::moveObject();
}

void TFruitsBoat::requestShadow()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN | LIVE_FLAG_UNK8))
		return;

	if (!checkLiveFlag(LIVE_FLAG_UNK200 | LIVE_FLAG_CLIPPED_OUT)
	    || checkLiveFlag(LIVE_FLAG_UNK400)) {
		TCircleShadowRequest request;

		request.mPosition   = mPosition;
		request.mRadiusX    = mShadowRadiusX;
		request.mRadiusZ    = mShadowRadiusZ;
		request.mShadowType = SHADOW_TYPE_SHIP;
		request.mRotationY  = (s16)mRotation.y;

		if (checkLiveFlag(LIVE_FLAG_UNK400))
			gpBindShadowManager->forceRequest(request, getActorType());
		else
			gpBindShadowManager->request(request, getActorType());
	}

	if (!checkLiveFlag(LIVE_FLAG_UNK200 | LIVE_FLAG_CLIPPED_OUT)
	    && !checkActorType(ACTOR_TYPE_UNK40000000)) {
		gpQuestionManager->request(mPosition, mScaledBodyRadius);
	}
}

TFruitsBoatManager::TFruitsBoatManager(int boat_type, const char* name)
    : TEnemyManager(name)
    , mBoatType(boat_type)
{
}

void TFruitsBoatManager::createModelData()
{
	switch (mBoatType) {
	case 0: {
		static const TModelDataLoadEntry entry[] = {
			{ "ShipDolpic.bmd", 0x10210000, 0 },
			{ nullptr, 0, 0 },
		};
		createModelDataArray(entry);
		break;
	}
	case 1: {
		static const TModelDataLoadEntry entry[] = {
			{ "ShipDolpic2.bmd", 0x10210000, 0 },
			{ nullptr, 0, 0 },
		};
		createModelDataArray(entry);
		break;
	}
	case 2: {
		static const TModelDataLoadEntry entry[] = {
			{ "ShipDolpic3.bmd", 0x10210000, 0 },
			{ nullptr, 0, 0 },
		};
		createModelDataArray(entry);
		break;
	}
	case 3:
	default: {
		static const TModelDataLoadEntry entry[] = {
			{ "ShipDolpic4.bmd", 0x10210000, 0 },
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

// TODO: instruction-identical, frame 0x140 against the ROM's 0x120. The ROM
// puts the direction vector above both pos/rot pairs, which named body locals
// cannot do, so 0x20 of its locals belong to objects this reconstruction does
// not create. Declare-then-assign on either `speed` did not move it.
DEFINE_NERVE(TNerveFruitsBoatGraphWander, TLiveActor)
{
	TFruitsBoat* boat = (TFruitsBoat*)spine->getBody();

	if (boat->getTracer()->getGraph() == nullptr
	    || boat->getTracer()->getGraph()->isDummy())
		return FALSE;

	// NOTE: the rowing step below is TFruitsBoat::rowToCurPathNode(), which
	// the map has as an UNUSED symbol -- but it is spelled out at both sites
	// here rather than called, exactly like TYumbo::lookatMario. Both copies
	// are byte-identical to the ROM's, and the dead out-of-line copy is a
	// different length, so the original really did paste it twice.
	if (boat->isReachedToGoal()) {
		TGraphNode& node = boat->getTracer()->getCurrent();

		if (node.getRailNode()->mFlags & 0x100)
			boat->onLiveFlag(TFruitsBoat::LIVE_FLAG_UNK10000);
		if (node.getRailNode()->mFlags & 0x400)
			boat->mReversed ^= 1;

		boat->goToDirectedNextGraphNode(
		    MsGetVecFromRotY(boat->mRotation.y, 1.0f));

		if (!boat->checkLiveFlag(TFruitsBoat::LIVE_FLAG_UNK10000)) {
			f32 marchSpeed = boat->mMarchSpeed;
			if (boat->getTracer()->getGraph()->getSplineRail() ? TRUE
			                                                  : FALSE) {
				f32 speed = boat->getTracer()->calcSplineSpeed(marchSpeed);
				boat->getTracer()->traceSpline(speed);

				JGeometry::TVec3<f32> rot;
				JGeometry::TVec3<f32> pos;
				boat->getTracer()->getGraph()->getSplineRail()->getPosAndRot(
				    boat->getTracer()->unk14, &pos, &rot);

				pos.sub(boat->mPosition);
				boat->mLinearVelocity.add(pos);

				boat->mRotation.y = rot.y;
				if (speed < 0.0f)
					boat->mRotation.y
					    = MsAngleWrap(180.0f + boat->mRotation.y);
			} else {
				boat->walkToCurPathNode(marchSpeed, boat->mTurnSpeed,
				                        0.0f);
			}

			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_PONPONSEN,
			                                &boat->mPosition, 0, nullptr, 0,
			                                4);
		}

		spine->pushAfterCurrent(&theNerve());
		return TRUE;
	}

	if (boat->checkLiveFlag(TFruitsBoat::LIVE_FLAG_UNK10000))
		return FALSE;

	f32 marchSpeed = boat->mMarchSpeed;
	if (boat->getTracer()->getGraph()->getSplineRail() ? TRUE : FALSE) {
		f32 speed = boat->getTracer()->calcSplineSpeed(marchSpeed);
		boat->getTracer()->traceSpline(speed);

		JGeometry::TVec3<f32> rot;
		JGeometry::TVec3<f32> pos;
		boat->getTracer()->getGraph()->getSplineRail()->getPosAndRot(
		    boat->getTracer()->unk14, &pos, &rot);

		pos.sub(boat->mPosition);
		boat->mLinearVelocity.add(pos);

		boat->mRotation.y = rot.y;
		if (speed < 0.0f)
			boat->mRotation.y = MsAngleWrap(180.0f + boat->mRotation.y);
	} else {
		boat->walkToCurPathNode(marchSpeed, boat->mTurnSpeed, 0.0f);
	}

	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_PONPONSEN, &boat->mPosition, 0,
	                                nullptr, 0, 4);
	return FALSE;
}

// TODO: instruction-identical, frame 0x88 against the ROM's 0x90 -- eight
// bytes of locals short. traceBckTrack()'s own out-of-line copy is size-exact
// against the map (0x15c), so the missing objects belong to the nerve, not to
// the body.
DEFINE_NERVE(TNerveFruitsBoatBckTrace, TLiveActor)
{
	TFruitsBoat* boat = (TFruitsBoat*)spine->getBody();

	boat->traceBckTrack();

	return FALSE;
}
