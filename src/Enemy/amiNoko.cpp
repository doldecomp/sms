#include <Enemy/AmiNoko.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/PathNode.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <Player/MarioAccess.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// The model's .bck list, in the alphabetical order the model data gives it.
// Every ground animation exists twice, three indices apart: TAmiNoko::
// mUseAnmSet1 picks between the "1" and the "2" variant.
enum {
	AMINOKO_ANM_FLYING1_LOOP  = 0,
	AMINOKO_ANM_FLYING1_START = 1,
	AMINOKO_ANM_HIT1          = 2,
	AMINOKO_ANM_RUN1_END      = 3,
	AMINOKO_ANM_RUN1_LOOP     = 4,
	AMINOKO_ANM_RUN1_START    = 5,
	AMINOKO_ANM_RUN2_END      = 6,
	AMINOKO_ANM_RUN2_LOOP     = 7,
	AMINOKO_ANM_RUN2_START    = 8,
	AMINOKO_ANM_TURN1_END     = 9,
	AMINOKO_ANM_TURN1_LOOP    = 10,
	AMINOKO_ANM_TURN1_START   = 11,
	AMINOKO_ANM_TURN2_END     = 12,
	AMINOKO_ANM_TURN2_LOOP    = 13,
	AMINOKO_ANM_TURN2_START   = 14,
	AMINOKO_ANM_WAIT1         = 15,
};

static const char* amiNoko_bastable[] = {
	nullptr,
	"/scene/amiNoko/bas/aminoko_flying1_start.bas",
	"/scene/amiNoko/bas/aminoko_hit1.bas",
	nullptr,
	"/scene/amiNoko/bas/aminoko_run1_loop.bas",
	nullptr,
	nullptr,
	"/scene/amiNoko/bas/aminoko_run2_loop.bas",
	nullptr,
	nullptr,
	"/scene/amiNoko/bas/aminoko_turn1_loop.bas",
	nullptr,
	nullptr,
	"/scene/amiNoko/bas/aminoko_turn2_loop.bas",
	nullptr,
	nullptr,
};

TAmiNokoSaveLoadParams::TAmiNokoSaveLoadParams(const char* prm)
    : TWalkerEnemyParams(prm)
    , PARAM_INIT(mSLElecRange, 200.0f)
    , PARAM_INIT(mSLMtxRotSpeed, 0.05f)
{
	TParams::load(mPrmPath);
}

TAmiNokoManager::TAmiNokoManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TAmiNokoManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TAmiNokoSaveLoadParams("/enemy/amiNoko.prm");
	TSmallEnemyManager::load(stream);
}

void TAmiNokoManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "aminoko_model1.bmd", 0x10220000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

// The amiNoko is never spawned from the manager; the fence it rides creates it.
TSpineEnemy* TAmiNokoManager::createEnemyInstance() { return nullptr; }

TAmiHit::TAmiHit(TAmiNoko* owner, const char* name)
    : THitActor(name)
    , mOwner(owner)
{
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);
	initHitActor(0x10000021, 1, 0x80000000, 120.0f, 240.0f, 120.0f, 240.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

BOOL TAmiHit::receiveMessage(THitActor* sender, u32 message)
{
	return mOwner->receiveMessage(sender, message);
}

// TODO: instruction-identical, but our local vector lands 4 bytes low, i.e.
// the original allocates one more 4-byte compiler temporary in here.
void TAmiHit::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		JGeometry::TVec3<f32> up = mOwner->mUp;
		up.normalize();
		up.scale(100.0f);

		mPosition = mOwner->mPosition;
		mPosition.add(up);
		mPosition.y -= 0.5f * getAttackHeight();

		if (!mOwner->checkLiveFlag(LIVE_FLAG_DEAD)) {
			for (int i = 0; i < getColNum(); ++i) {
				if (getCollision(i)->getActorType() == 0x80000001)
					mOwner->attackToMario();
			}
		}
	}
	THitActor::perform(cue, graphics);
}

TAmiNoko::TAmiNoko(const char* name)
    : TWalkerEnemy(name)
{
	mFencePlane  = nullptr;
	mFenceKind   = AMINOKO_SURFACE_GROUND;
	mUseAnmSet1  = 1;
	mUp.set(0.0f, 1.0f, 0.0f);
	mFront.set(0.0f, 0.0f, 1.0f);
	mPrevUp    = mUp;
	mPrevFront = mFront;
}

void TAmiNoko::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);
	stream >> mCoinId;
}

// Binding level worth +8 of low region, landing TAmiNoko::init's frame at
// 0x98 (batch 121).
static inline u8 AmiNokoGetCurrentMap(TMarDirector* p)
{
	u8 currentMap = p->getCurrentMap();
	return currentMap;
}

// TODO: instruction-identical, frame 8 bytes short.
void TAmiNoko::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);
	mActorType = 0x10000021;
	unk150     = 0x11;
	getSpine()->initWith(&TNerveAmiNokoWalkOnFence::theNerve());
	mSaveParams = (TAmiNokoSaveLoadParams*)getSaveParam();
	reset();
	setWalkAnm();
	onLiveFlag(LIVE_FLAG_UNK10);
	initialGraphNode();
	if (AmiNokoGetCurrentMap(SMSGetMarDirector()) == 8)
		mUseAnmSet1 = 0;
	unkE8   = 0;
	mAmiHit = new TAmiHit(this, "アミノコ当り判定");
}

void TAmiNoko::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("aminoko_model1.bmd", 3);
}

void TAmiNoko::reset() { TWalkerEnemy::reset(); }

void TAmiNoko::behaveToWater(THitActor* param_1)
{
	if (mSpine->getCurrentNerve() != &TNerveAmiNokoFreeze::theNerve()) {
		mSpine->pushNerve(&TNerveAmiNokoFreeze::theNerve());
		mSprayedByWaterCooldown = 0;
	}
}

// TODO: instruction-identical, frame 8 bytes short.
void TAmiNoko::attackToMario()
{
	if (mSpine->getCurrentNerve() == &TNerveSmallEnemyChange::theNerve())
		return;
	if (mFencePlane == nullptr)
		return;

	BOOL canAttack = TRUE;
	switch (mFenceKind) {
	case AMINOKO_SURFACE_GROUND:
		// Standing on the floor: Mario has to be above us to get zapped.
		if (SMS_GetMarioRfPlane() != nullptr && gpMarioPos->y < getPosition().y)
			canAttack = FALSE;
		break;
	case AMINOKO_SURFACE_ROOF:
		if (SMS_GetMarioGrPlane() != nullptr
		    && 5.0f + gpMarioPos->y > getPosition().y)
			canAttack = FALSE;
		break;
	case AMINOKO_SURFACE_WALL: {
		// Only Mario on the same side of the wall can be reached.
		const TBGCheckData* wall = SMS_GetMarioWlPlane();
		if (wall != nullptr
		    && wall->getNormal().dot(mFencePlane->getNormal()) < 0.0f)
			canAttack = FALSE;
		break;
	}
	}

	if (canAttack) {
		if (SMS_SendMessageToMario(this, HIT_MESSAGE_ELECTRIC_SHOCK)) {
			if (mSpine->getCurrentNerve()
			    != &TNerveAmiNokoFreeze::theNerve()) {
				mSpine->pushNerve(&TNerveAmiNokoFreeze::theNerve());
			}
		}
	}
}

void TAmiNoko::setWalkAnm()
{
	if (mUseAnmSet1)
		setBckAnm(AMINOKO_ANM_RUN1_START);
	else
		setBckAnm(AMINOKO_ANM_RUN2_START);
}

// Binding level over the address of a struct member, worth +16 of low region
// in TAmiNoko::isHitValid (batch 130).
static inline const JGeometry::TVec3<f32>* AmiNokoUp(const TAmiNoko* p)
{
	const JGeometry::TVec3<f32>* up = &p->mUp;
	return up;
}

static inline f32 AmiNokoSqrt(f32 value)
{
	return JGeometry::TUtil<f32>::sqrt(value);
}

// TODO: 99.7%, frame 0x60 exact (the AmiNokoUp binder above pays the 16
// bytes). The whole residue is one volatile-FPR block trade: retail loads
// mPosition.x/gpMarioPos->x/mPosition.z into f5/f4/f3 where we use f3/f1/f2,
// with the identical load order, the identical `fsubs f31/f30` destinations
// and matan's f1/f2 argument loads interleaved the same way in both. Retail's
// block simply starts two registers higher, as if f1/f2 were reserved for the
// pending matan call. Re-pass II measured and rejected, all with the frame
// still exact: naming matan's discarded result (byte-identical), moving the
// matan call above `toMario` (byte-identical), building `toMario` straight
// from the two subtractions (evaluates z first -- right-to-left arguments --
// and swaps f31/f30, 99.5), `SMS_GetMarioX()/SMS_GetMarioZ()` in place of
// `gpMarioPos->` (two global forks, frame 0x68, 99.5) and a named
// `const TVec3<f32>& up = mUp;` feeding both matan and the dot (one more
// callee-saved GPR, frame 0x58, 95.7). Research 171 says naming is the only
// knob on a volatile block and both values are already named, so this is the
// block-trade class (cf. MSHandle::calcDolby).
bool TAmiNoko::isHitValid(u32 message)
{
	if (message == HIT_MESSAGE_PUNCH || message == HIT_MESSAGE_HIP_DROP) {
		f32 dx = mPosition.x - gpMarioPos->x;
		f32 dz = mPosition.z - gpMarioPos->z;
		JGeometry::TVec3<f32> toMario(dx, 0.0f, dz);
		// TODO: the result is discarded; presumably a leftover from an earlier
		// version that compared the fence facing against this angle.
		matan(mUp.z, mUp.x);
		if (toMario.dot(*AmiNokoUp(this)) > 0.0f
		    || message == HIT_MESSAGE_HIP_DROP)
			mSpine->pushNerve(&TNerveAmiNokoDie::theNerve());
	}
	return message == HIT_MESSAGE_UNKB ? true : false;
}

// TODO: three loads left: `found = plane` re-reads the (address-taken) local
// instead of reusing the register, and the wall loop reloads it once. The frame
// is also 0x70 short: the original reserves far more compiler temporaries here.
void TAmiNoko::calcDirection()
{
	JGeometry::TVec3<f32> toGoal = getUnkF4().getPoint();
	toGoal.sub(mPosition);
	if (toGoal.isZero())
		toGoal.set(1.0f, 0.0f, 0.0f);
	else
		toGoal.normalize();

	// Find the surface we are crawling on: the nearest wall around us, the
	// ground under us or the roof above us, whichever is closest.
	const TBGCheckData* plane;

	TBGWallCheckRecord record(mPosition.x, mPosition.y, mPosition.z, 10.0f, 4,
	                          0);
	int wallNum    = gpMap->isTouchedWallsAndMoveXZ(&record);
	f32 nearest    = -1.0f;
	int nearestIdx = -1;
	for (int i = 0; i < wallNum; ++i) {
		plane    = record.mResultWalls[i];
		f32 dist = fabsf(plane->getNormal().dot(mPosition)
		                 + plane->mPlaneDistance);
		if (nearestIdx < 0 || nearest > dist || nearest < 0.0f) {
			mFenceKind = AMINOKO_SURFACE_WALL;
			nearest    = dist;
			nearestIdx = i;
		}
	}
	const TBGCheckData* found = record.mResultWalls[nearestIdx];

	gpMap->checkGround(mPosition.x, mPosition.y + getHeadHeight(),
	                   mPosition.z, &plane);
	mGroundPlane = plane;
	if (mGroundPlane != nullptr) {
		plane    = mGroundPlane;
		f32 dist = plane->getNormal().dot(mPosition) + plane->mPlaneDistance;
		if (dist >= 0.0f) {
			if (nearest > dist || nearest < 0.0f) {
				mFenceKind = AMINOKO_SURFACE_GROUND;
				nearest    = dist;
				found      = plane;
			}
		}
	}

	gpMap->checkRoof(mPosition, &plane);
	if (plane != nullptr) {
		f32 dist = plane->getNormal().dot(mPosition) + plane->mPlaneDistance;
		if (dist >= 0.0f) {
			if (nearest > dist || nearest < 0.0f) {
				mFenceKind = AMINOKO_SURFACE_ROOF;
				found      = plane;
			}
		}
	}
	if (found != nullptr)
		mFencePlane = found;

	f32 rotSpeed = getSaveParams()->getSLMtxRotSpeed();

	// Creep the up vector towards the surface normal.
	JGeometry::TVec3<f32> normal;
	if (mFencePlane != nullptr)
		normal = mFencePlane->getNormal();
	else
		normal.set(0.0f, 1.0f, 0.0f);

	if (normal.dot(mUp) <= -1.0f) {
		mUp = normal;
	} else {
		mUp.x = mUp.x < normal.x
		            ? (mUp.x + rotSpeed > normal.x ? normal.x
		                                           : mUp.x + rotSpeed)
		            : (mUp.x - rotSpeed > normal.x ? mUp.x - rotSpeed
		                                           : normal.x);
		mUp.y = mUp.y < normal.y
		            ? (mUp.y + rotSpeed > normal.y ? normal.y
		                                           : mUp.y + rotSpeed)
		            : (mUp.y - rotSpeed > normal.y ? mUp.y - rotSpeed
		                                           : normal.y);
		mUp.z = mUp.z < normal.z
		            ? (mUp.z + rotSpeed > normal.z ? normal.z
		                                           : mUp.z + rotSpeed)
		            : (mUp.z - rotSpeed > normal.z ? mUp.z - rotSpeed
		                                           : normal.z);
		VECNormalize(mUp, mUp);
	}

	// Creep the facing direction towards the goal. Going straight backwards
	// would be ambiguous, so turn sideways around the surface normal first.
	if (mFront.dot(toGoal) < -0.1f) {
		Mtx rot;
		MTXRotAxisRad(rot, normal, 1.5707964f);
		MTXMultVec(rot, toGoal, toGoal);
	}
	mFront.x = mFront.x < toGoal.x
	               ? (mFront.x + rotSpeed > toGoal.x
	                      ? toGoal.x
	                      : mFront.x + rotSpeed)
	               : (mFront.x - rotSpeed > toGoal.x
	                      ? mFront.x - rotSpeed
	                      : toGoal.x);
	mFront.y = mFront.y < toGoal.y
	               ? (mFront.y + rotSpeed > toGoal.y
	                      ? toGoal.y
	                      : mFront.y + rotSpeed)
	               : (mFront.y - rotSpeed > toGoal.y
	                      ? mFront.y - rotSpeed
	                      : toGoal.y);
	mFront.z = mFront.z < toGoal.z
	               ? (mFront.z + rotSpeed > toGoal.z
	                      ? toGoal.z
	                      : mFront.z + rotSpeed)
	               : (mFront.z - rotSpeed > toGoal.z
	                      ? mFront.z - rotSpeed
	                      : toGoal.z);
	VECNormalize(mFront, mFront);

	// Re-orthogonalise: remember the last frame that had a valid frame, then
	// rebuild the facing direction from the side vector and the up vector.
	JGeometry::TVec3<f32> side;
	side.cross(mUp, mFront);
	if (!side.isZero()) {
		mPrevUp    = mUp;
		mPrevFront = mFront;
	}
	toGoal.cross(side, mUp);
	if (!toGoal.isZero())
		mFront = toGoal;
}

void TAmiNoko::emitEffects()
{
	SMSGetMSound()->startSoundActor(MSD_SE_EN_AMINOKO_SPARK, &mPosition, 0,
	                                nullptr, 0, 4);

	// Three arcs bound to three joints. The owner key has to differ per arc
	// or the particle manager would reuse a single slot for all of them.
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_AMN_BIRI, getMActor()->getModel()->getAnmMtx(11), 1,
	    this);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_AMN_BIRI, getMActor()->getModel()->getAnmMtx(10), 1,
	    this + 1);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_AMN_BIRI, getMActor()->getModel()->getAnmMtx(9), 1,
	    this + 2);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_AMN_SPARK_R, getMActor()->getModel()->getAnmMtx(11), 1,
	    this);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_AMN_SPARK_L, getMActor()->getModel()->getAnmMtx(11), 1,
	    this);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_AMN_SPARK_M, getMActor()->getModel()->getAnmMtx(11), 1,
	    this);

	if (mSpine->getCurrentNerve() == &TNerveAmiNokoFreeze::theNerve()
	    && mSpine->getTime() < 46) {
		JGeometry::TVec3<f32> scale(2.0f, 2.0f, 2.0f);

		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_DNK_SHIBIRE_A, getMActor()->getModel()->getAnmMtx(0),
		    1, this);
		if (emitter)
			emitter->setGlobalScale(scale);

		MtxPtr mtx = getMActor()->getModel()->getAnmMtx(6);
		mSparkPos.set(mtx[0][3], mtx[1][3], mtx[2][3]);
		emitter = gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_DNK_HIBANA, &mSparkPos, 1, this);
		if (emitter)
			emitter->setGlobalScale(scale);
	}
}

void TAmiNoko::calcRootMatrix()
{
	emitEffects();

	// TODO: the original keeps this pointer in r31 and `this` in r30; we get
	// the opposite, plus the register numbering of the second cross product.
	MtxPtr mtx;
	if (isBckAnm(AMINOKO_ANM_FLYING1_LOOP)) {
		// While falling the orientation is frozen, only the position moves.
		mRootMtx[0][3] = mPosition.x;
		mRootMtx[1][3] = mPosition.y;
		mRootMtx[2][3] = mPosition.z;
		mtx            = mRootMtx;
		getModel()->setBaseTRMtx(mtx);
		getModel()->setBaseScale(mScaling);
		return;
	}

	mtx = getModel()->getBaseTRMtx();

	JGeometry::TVec3<f32> up;
	JGeometry::TVec3<f32> front;
	JGeometry::TVec3<f32> side;
	side.cross(mUp, mFront);
	if (side.isZero()) {
		up    = mPrevUp;
		front = mPrevFront;
		side.cross(up, front);
		if (side.isZero())
			side.set(1.0f, 0.0f, 0.0f);
	} else {
		up    = mUp;
		front = mFront;
	}
	VECNormalize(side, side);

	mtx[0][0] = side.x;
	mtx[1][0] = side.y;
	mtx[2][0] = side.z;
	mtx[0][1] = up.x;
	mtx[1][1] = up.y;
	mtx[2][1] = up.z;
	mtx[0][2] = front.x;
	mtx[1][2] = front.y;
	mtx[2][2] = front.z;
	mtx[0][3] = mPosition.x - 30.0f * mUp.x;
	mtx[1][3] = mPosition.y - 30.0f * mUp.y;
	mtx[2][3] = mPosition.z - 30.0f * mUp.z;

	getModel()->setBaseScale(mScaling);
	MTXCopy(mtx, mRootMtx);
}

// TODO: instruction-identical; the operator- temporary lands at 0x20 instead
// of 0x10, so the original has fewer temporaries below it.
void TAmiNoko::bind()
{
	if (isBckAnm(AMINOKO_ANM_FLYING1_LOOP)) {
	// Same as TLiveActor::bind() minus the wall push-out: while the amiNoko is
	// tumbling off its fence it should fall straight through walls.
	JGeometry::TVec3<f32> nextPos = mPosition;
	nextPos += mLinearVelocity;
	nextPos += mVelocity;

	mVelocity.y -= getGravityY();
	if (mVelocity.y < TLiveActor::mVelocityMinY)
		mVelocity.y = TLiveActor::mVelocityMinY;

	if (checkLiveFlag(LIVE_FLAG_UNK1000)) {
		mGroundHeight = gpMap->checkGroundIgnoreWaterSurface(
		    nextPos.x, nextPos.y + mHeadHeight, nextPos.z, &mGroundPlane);
	} else {
		mGroundHeight = gpMap->checkGround(
		    nextPos.x, nextPos.y + mHeadHeight, nextPos.z, &mGroundPlane);
	}
	mGroundHeight += 1.0f;

	if (nextPos.y <= mGroundHeight + 0.05f) {
		if (mGroundPlane->checkFlag(BG_CHECK_FLAG_ILLEGAL))
			kill();
		offLiveFlag(LIVE_FLAG_AIRBORNE);
		mVelocity.set(0.0f, 0.0f, 0.0f);
		nextPos.y = mGroundHeight;
	} else {
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	mLinearVelocity = nextPos - mPosition;
	} else {
		TLiveActor::bind();
	}
}

void TAmiNoko::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM)
		calcDirection();

	TSmallEnemy::perform(cue, graphics);
	mAmiHit->perform(cue, graphics);
}

f32 TAmiNoko::getGravityY() const
{
	if (mSpine->getCurrentNerve() == &TNerveAmiNokoDie::theNerve())
		return 0.0f;
	return mGravity;
}

// Tiny size mismatch: 516 bytes compiled, the map records 492.
// TODO: every local of this inlined body lands 4 bytes above the original's
// in both nerves, i.e. we allocate one 4-byte compiler temporary too many
// somewhere in here. The instruction stream is otherwise identical.
void TAmiNoko::creepToCurPathNode(f32 max_speed)
{
	JGeometry::TVec3<f32> toGoal = getUnkF4().getPoint();
	toGoal.sub(mPosition);
	if (toGoal.isZero())
		return;

	f32 speed = JGeometry::TUtil<f32>::sqrt(toGoal.squared());
	speed     = max_speed > speed ? speed : max_speed;

	toGoal.normalize();
	toGoal.scale(speed);

	JGeometry::TVec3<f32> velocity = mLinearVelocity;
	velocity.add(toGoal);
	mLinearVelocity = velocity;
}

// Tiny size mismatch
bool TAmiNoko::isDeadByWall()
{
	if (gpMap->isTouchedOneWallAndMoveXZ(&mPosition.x, mPosition.y,
	                                     &mPosition.z, 3.0f * mBodyRadius)) {
		JGeometry::TVec3<f32> stop(0.0f, 0.0f, 0.0f);
		mVelocity = stop;

		JPABaseEmitter* emitter = gpMarioParticleManager->emitWithRotate(
		    PARTICLE_MS_ENM_WALLHIT, &mPosition, 0,
		    (s16)(182.04445f * mRotation.y), 0, 0, nullptr);
		if (emitter)
			emitter->setGlobalScale(mScaling);

		emitter = gpMarioParticleManager->emitWithRotate(
		    PARTICLE_MS_ENM_WALLHIT_O, &mPosition, 0,
		    (s16)(182.04445f * mRotation.y), 0, 0, nullptr);
		if (emitter)
			SMSSetEmitterPolColor(emitter, 6);

		return true;
	}
	return false;
}

const char** TAmiNoko::getBasNameTable() const { return amiNoko_bastable; }

DEFINE_NERVE(TNerveAmiNokoWalkOnFence, TLiveActor)
{
	TAmiNoko* amiNoko = (TAmiNoko*)spine->getBody();

	if (spine->getTime() == 0)
		amiNoko->setWalkAnm();

	if (amiNoko->isBckAnm(AMINOKO_ANM_RUN1_START)
	    || amiNoko->isBckAnm(AMINOKO_ANM_RUN2_START)) {
		if (amiNoko->checkCurAnmEnd(0)) {
			if (amiNoko->mUseAnmSet1)
				amiNoko->setBckAnm(AMINOKO_ANM_RUN1_LOOP);
			else
				amiNoko->setBckAnm(AMINOKO_ANM_RUN2_LOOP);
		}
	}

	JGeometry::TVec3<f32> toGoal = amiNoko->unkF4.getPoint();
	toGoal.sub(amiNoko->mPosition);
	if (AmiNokoSqrt(toGoal.squared()) < 1.5f
	    && amiNoko->checkCurAnmEnd(0)) {
		if (amiNoko->isBckAnm(AMINOKO_ANM_RUN1_END)
		    || amiNoko->isBckAnm(AMINOKO_ANM_RUN2_END)) {
			amiNoko->goToRandomNextGraphNode();
			spine->pushAfterCurrent(&TNerveAmiNokoTurn::theNerve());
			return TRUE;
		}
		if (amiNoko->isBckAnm(AMINOKO_ANM_RUN1_LOOP))
			amiNoko->setBckAnm(AMINOKO_ANM_RUN1_END);
		else if (amiNoko->isBckAnm(AMINOKO_ANM_RUN2_LOOP))
			amiNoko->setBckAnm(AMINOKO_ANM_RUN2_END);
	}

	amiNoko->creepToCurPathNode(3.0f);
	return FALSE;
}

DEFINE_NERVE(TNerveAmiNokoTurn, TLiveActor)
{
	TAmiNoko* amiNoko = (TAmiNoko*)spine->getBody();

	if (spine->getTime() == 0) {
		if (amiNoko->mUseAnmSet1)
			amiNoko->setBckAnm(AMINOKO_ANM_TURN1_START);
		else
			amiNoko->setBckAnm(AMINOKO_ANM_TURN2_START);
	}

	if (amiNoko->isBckAnm(AMINOKO_ANM_TURN1_START)
	    || amiNoko->isBckAnm(AMINOKO_ANM_TURN2_START)) {
		if (amiNoko->checkCurAnmEnd(0)) {
			if (amiNoko->mUseAnmSet1)
				amiNoko->setBckAnm(AMINOKO_ANM_TURN1_LOOP);
			else
				amiNoko->setBckAnm(AMINOKO_ANM_TURN2_LOOP);
		}
	}

	JGeometry::TVec3<f32> toGoal = amiNoko->getUnkF4().getPoint();
	toGoal.sub(amiNoko->mPosition);
	if (toGoal.x == 0.0f && toGoal.y == 0.0f && toGoal.z == 0.0f)
		toGoal.x = 1.0f;
	VECNormalize(toGoal, toGoal);

	if (toGoal.dot(amiNoko->mFront) > 0.8f && amiNoko->checkCurAnmEnd(0)) {
		if (amiNoko->isBckAnm(AMINOKO_ANM_TURN1_END)
		    || amiNoko->isBckAnm(AMINOKO_ANM_TURN2_END)) {
			if (amiNoko->mUseAnmSet1)
				amiNoko->setBckAnm(AMINOKO_ANM_WAIT1);
			else
				amiNoko->setBckAnm(AMINOKO_ANM_WAIT1);
		} else if (amiNoko->isBckAnm(AMINOKO_ANM_WAIT1)) {
			spine->pushAfterCurrent(
			    &TNerveAmiNokoWalkOnFence::theNerve());
			return TRUE;
		} else if (amiNoko->mUseAnmSet1) {
			amiNoko->setBckAnm(AMINOKO_ANM_TURN1_END);
		} else {
			amiNoko->setBckAnm(AMINOKO_ANM_TURN2_END);
		}
	}

	amiNoko->creepToCurPathNode(0.0f);
	return FALSE;
}

// TODO: fabricated. Every symbol of this nerve is UNUSED, so only the compiled
// size (0x6C) constrains the body; nothing ever pushes it.
DEFINE_NERVE(TNerveAmiNokoAttack, TLiveActor)
{
	TAmiNoko* amiNoko = (TAmiNoko*)spine->getBody();

	if (spine->getTime() == 0)
		amiNoko->setBckAnm(AMINOKO_ANM_HIT1);

	if (amiNoko->checkCurAnmEnd(0))
		return TRUE;

	return FALSE;
}

DEFINE_NERVE(TNerveAmiNokoDie, TLiveActor)
{
	TAmiNoko* amiNoko = (TAmiNoko*)spine->getBody();

	if (spine->getTime() == 0) {
		amiNoko->setBckAnm(AMINOKO_ANM_FLYING1_START);
		amiNoko->onHitFlag(HIT_FLAG_NO_COLLISION);
		amiNoko->mAmiHit->onHitFlag(HIT_FLAG_NO_COLLISION);
	}

	if (amiNoko->checkCurAnmEnd(0)) {
		if (amiNoko->isBckAnm(AMINOKO_ANM_FLYING1_START)) {
			// TODO: this direction away from Mario is overwritten right away
			// by the model's own Y axis; the original kept an older attempt at
			// picking the tumble direction.
			JGeometry::TVec3<f32> jump(amiNoko->getPosition() - *gpMarioPos);
			if (jump.x == 0.0f && jump.y == 0.0f && jump.z == 0.0f)
				jump.x = 1.0f;

			MtxPtr mtx = amiNoko->getMActor()->getModel()->getBaseTRMtx();
			jump.x     = mtx[0][1];
			jump.y     = mtx[1][1];
			jump.z     = mtx[2][1];
			MsVECNormalize(jump, jump);
			jump.scale(20.0f);

			amiNoko->mPosition.y += 10.0f;
			amiNoko->mVelocity = jump;
			amiNoko->onLiveFlag(LIVE_FLAG_AIRBORNE);
			amiNoko->setBckAnm(AMINOKO_ANM_FLYING1_LOOP);
		}
	}

	gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_KIL_SMOKE,
	    amiNoko->getMActor()->getModel()->getBaseTRMtx(), 1, amiNoko);

	if (amiNoko->isBckAnm(AMINOKO_ANM_FLYING1_LOOP)
	    && spine->getTime() > 30) {
		JGeometry::TVec3<f32> toMario(amiNoko->mPosition - *gpMarioPos);
		if (amiNoko->isDeadByWall() || !amiNoko->isAirborne()
		    || toMario.length() > 10000.0f) {
			amiNoko->onHitFlag(HIT_FLAG_NO_COLLISION);
			amiNoko->onLiveFlag(LIVE_FLAG_DEAD);
			amiNoko->onLiveFlag(LIVE_FLAG_UNK8);
			amiNoko->offLiveFlag(LIVE_FLAG_HIDDEN);
			amiNoko->offLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
			amiNoko->mHolder = nullptr;
			amiNoko->stopAnmSound();
			spine->reset();
			spine->setNext(&TNerveSmallEnemyDie::theNerve());
			spine->pushAfterCurrent(spine->getDefault());
			amiNoko->genRandomItem();
			return TRUE;
		}
	}
	return FALSE;
}

DEFINE_NERVE(TNerveAmiNokoFreeze, TLiveActor)
{
	TAmiNoko* amiNoko = (TAmiNoko*)spine->getBody();

	if (spine->getTime() == 0) {
		amiNoko->setBckAnm(AMINOKO_ANM_HIT1);
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_DNK_SHIBIRE_B,
		    amiNoko->getMActor()->getModel()->getAnmMtx(0), 0, nullptr);
		if (emitter)
			emitter->setGlobalScale(JGeometry::TVec3<f32>(2.0f, 2.0f, 2.0f));
	}

	if (amiNoko->checkCurAnmEnd(0)) {
		if (amiNoko->isBckAnm(AMINOKO_ANM_HIT1)) {
			amiNoko->setBckAnm(AMINOKO_ANM_WAIT1);
		} else if (spine->getTime()
		           > amiNoko->getSaveParams()->getSLFreezeWait()) {
			return TRUE;
		}
	}
	return FALSE;
}
