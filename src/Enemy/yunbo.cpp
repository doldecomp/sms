#include <Enemy/Yunbo.hpp>
#include <Enemy/Enemy.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/ObjModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Map/Map.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JGeometry/JGQuat4.hpp>
#include <System/Particles.hpp>
#include <System/EmitterViewObj.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static const char* sambohead_bastable[] = {
	"/scene/sambohead/bas/flower_shoot.bas",
	"/scene/sambohead/bas/samboHead_crash.bas",
	"/scene/sambohead/bas/samboHead_dance.bas",
	"/scene/sambohead/bas/samboHead_down.bas",
	"/scene/sambohead/bas/samboHead_Fhide.bas",
	"/scene/sambohead/bas/samboHead_hit.bas",
	"/scene/sambohead/bas/samboHead_hit_end.bas",
	"/scene/sambohead/bas/samboHead_jump_end.bas",
	"/scene/sambohead/bas/samboHead_jump_start.bas",
	nullptr,
	"/scene/sambohead/bas/samboHead_set.bas",
	"/scene/sambohead/bas/samboHead_turn.bas",
	nullptr,
};

TYumboSeed::TYumboSeed(MActor* actor, const TYumbo& yumbo)
    : THitActor("ユンボ種")
    , mYumbo(&yumbo)
    , mActor(actor)
    , mSeedFlags(SEED_FLAG_SPENT)
{
}

void TYumboSeed::init()
{
	initHitActor(0x1000002A, 1, ACTOR_TYPE_PLAYER, 30.0f, 30.0f, 0.0f, 0.0f);

	TIdxGroupObj* group
	    = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");
	group->getChildren().push_back(this);
}

// TODO: instruction-identical, but `mtx` sits 4 bytes low: one 4-byte object
// of ours is ordered above it where the retail frame has none. Frame total and
// every instruction already agree.
void TYumboSeed::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mSeedFlags & SEED_FLAG_SPENT)
		return;

	if (cue & CUE_CALC_ANIM) {
		Mtx mtx;
		mtx[0][0] = 1.0f;
		mtx[1][0] = 0.0f;
		mtx[2][0] = 0.0f;
		mtx[0][1] = 0.0f;
		mtx[1][1] = 1.0f;
		mtx[2][1] = 0.0f;
		mtx[0][2] = 0.0f;
		mtx[1][2] = 0.0f;
		mtx[2][2] = 1.0f;
		mtx[0][3] = getPosition().x;
		mtx[1][3] = mPosition.y;
		mtx[2][3] = mPosition.z;

		mActor->getModel()->setBaseScale(mScaling);
		MTXCopy(mtx, mActor->getModel()->getBaseTRMtx());
		mActor->getModel()->calc();
	}

	if (cue & CUE_MOVE) {
		// Reading the TParamRT directly rather than through the getters:
		// the extra inline level the getters add costs 8 bytes of frame the
		// retail object does not have.
		mVelocity.y -= mYumbo->getSaveParams()->mSeedGravityY.get();
		mPosition.add(mVelocity);
		mVelocity.scale(mYumbo->getSaveParams()->mSeedAirFric.get());

		checkHitActors();

		if (--mLifeTimer == 0) {
			mSeedFlags |= SEED_FLAG_SPENT;
			onHitFlag(HIT_FLAG_NO_COLLISION);
		}
	}

	if (!(mSeedFlags & SEED_FLAG_HIDDEN))
		mActor->perform(cue, graphics);
}

void TYumboSeed::checkHitActors()
{
	THitActor** end;
	THitActor** it;

	it  = mCollisions;
	end = mCollisions + mColCount;
	for (; it != end; ++it) {
		switch ((*it)->getActorType()) {
		case 0x80000001:
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
			mSeedFlags |= SEED_FLAG_SPENT;
			break;
		}
	}
}

void TYumboSeed::startToMove(const JGeometry::TVec3<f32>& pos,
                             const JGeometry::TVec3<f32>& vel, int life)
{
	mSeedFlags &= ~SEED_FLAG_SPENT;
	mPosition  = pos;
	mVelocity  = vel;
	mLifeTimer = life;
	mScaling.set(2.0f, 2.0f, 2.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	onHitFlag(HIT_FLAG_CANNOT_GET_HIT);
}

TYumbo::TYumbo(const char* name)
    : TSmallEnemy(name)
{
	onLiveFlag(LIVE_FLAG_UNK10);
}

// TODO: instruction-identical, but 0x20 bytes of frame short. The inlined
// initMActorAndKeeper body is now map-size exact (0xbc); its remaining frame
// residue is an unaccounted inline temporary.
void TYumbo::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);

	initMActorAndKeeper();

	mSpine->initWith(&TNerveYumboDancing::theNerve());

	TYumboSeed** end = &mSeeds[16];
	for (TYumboSeed** seed = &mSeeds[0]; seed != end; ++seed) {
		*seed = new TYumboSeed(
		    mMActorKeeper->createMActor("samboSeed.bmd", 3), *this);
		(*seed)->init();
	}

	initCollision();

	initAnmSound();

	mCenterJntIndex
	    = getModel()->getModelData()->getJointName()->getIndex("center");
}

void TYumbo::reset() { }

void TYumbo::initMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 0x12);

	MActor* yumbo = mMActorKeeper->createMActor("yumbo.bmd", 0);
	TMActorKeeper* keeper = mMActorKeeper;
	MActor* flower = keeper->createMActor("flower.bmd", 0);
	setMaterialToMActor(flower,
	                    ((TYumboManager*)mManager)->mMaterialTable);

	mMActor = yumbo;
}

void TYumbo::setMaterialToMActor(MActor* actor, J3DMaterialTable* table)
{
	actor->getModel()->getModelData()->setMaterialTable(table,
	                                                    J3DMatCopyFlag_All);
	actor->initDL();
	actor->getModel()->lock();
}

void TYumbo::initCollision()
{
	initHitActor(0x1000002A, 1, ACTOR_TYPE_PLAYER, 97.5f, 225.0f, 90.0f,
	             225.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mGroundHeight = gpMap->checkGround(
	    mPosition.x, mPosition.y + mHeadHeight, mPosition.z, &mGroundPlane);
	mScaledBodyRadius = 75.0f;
	mScaling.set(1.5f, 1.5f, 1.5f);
}

// Binding level worth +8 of low region, landing TYumbo::receiveMessage's
// frame at 0x30 (batch 124).
static inline bool YunboCheckLiveFlag(const TYumbo* p, u32 i)
{
	bool liveFlag = p->checkLiveFlag(i);
	return liveFlag;
}

// TODO: instruction-identical, 8 bytes of frame short.
BOOL TYumbo::receiveMessage(THitActor* sender, u32 message)
{
	if (YunboCheckLiveFlag(this, LIVE_FLAG_DEAD))
		return FALSE;

	switch (message) {
	case HIT_MESSAGE_TRAMPLE:
	case HIT_MESSAGE_HIP_DROP:
		if (!isFreeze())
			return FALSE;
		behaveHitAttack();
		return TRUE;

	default:
		return TSmallEnemy::receiveMessage(sender, message);
	}
}

// TODO: instruction-identical, 0x10 bytes of frame short. updateEffect and
// updateCollision are both size-exact against the map, so the missing stack is
// inline-boundary temporaries, not a wrong body.
void TYumbo::moveObject()
{
	updateEffect();
	updateCollision();
	updateSquareToMario();
	TSmallEnemy::moveObject();
}

void TYumbo::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);

	TYumboSeed** end = &mSeeds[16];
	for (TYumboSeed** seed = &mSeeds[0]; seed != end; ++seed)
		(*seed)->perform(cue, graphics);
}

void TYumbo::behaveToWater(THitActor*)
{
	if (!isWaterproof()) {
		mSpine->reset();
		mSpine->setNext(&TNerveYumboFreeze::theNerve());
	}
}

void TYumbo::behaveHitAttack()
{
	mSpine->reset();
	mSpine->setNext(&TNerveSmallEnemyDie::theNerve());
}

void TYumbo::updateCollision()
{
	if (isFreeze() || isDead())
		onHitFlag(HIT_FLAG_CANNOT_ATTACK);
	else
		offHitFlag(HIT_FLAG_CANNOT_ATTACK);
}

// A second binding level over the raw global, TU-local because the one beside
// TNerveYumboAppearing is declared further down the file.
static inline TMarioParticleManager* YunboEffectParticleManager()
{
	TMarioParticleManager* marioParticleManager = gpMarioParticleManager;
	return marioParticleManager;
}

void TYumbo::updateEffect()
{
	if (isChangedBlock())
		return;

	if (isFreeze()) {
		JPABaseEmitter* emitter
		    = YunboEffectParticleManager()->emitAndBindToPosPtr(
		    PARTICLE_MS_POI_KIZETSU, &mPosition, 1, this);
		if (emitter)
			emitter->setGlobalScale(mScaling);
	}

	if (mMActor->checkCurAnm("sambohead_dance", 0)) {
		YunboEffectParticleManager()->emitAndBindToMtxPtr(
		    PARTICLE_MS_YNB_ONPU, getModel()->getAnmMtx(mCenterJntIndex), 1,
		    this);
	}
}

bool TYumbo::isFindOutMario() const
{
	f32 searchHeight = getSaveParams()->getSLSearchHeight();
	if (fabsf(gpMarioPos->y - mPosition.y) < searchHeight) {
		JGeometry::TVec3<f32> pos(gpMarioPos->x, mPosition.y,
		                          gpMarioPos->z);
		f32 searchLength = getSaveParams()->getSLSearchLength();
		f32 searchAngle  = getSaveParams()->getSLSearchAngle();
		return isInSight(pos, searchLength, searchAngle,
		                 getSaveParams()->getSLSearchAware())
		    ? true
		    : false;
	}
	return false;
}

bool TYumbo::isWantToAppear() const
{
	if (getSaveParams()->getSLGiveUpHeight()
	    <= fabsf(gpMarioPos->y - mPosition.y))
		return true;

	f32 giveUp;
	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario.x -= mPosition.x;
	toMario.y -= mPosition.y;
	toMario.z -= mPosition.z;
	toMario.y = 0.0f;

	giveUp = getSaveParams()->getSLGiveUpLength();
	return giveUp * giveUp < toMario.squared();
}

bool TYumbo::isAllSeedBroken() const
{
	for (TYumboSeed* const* seed = &mSeeds[0]; seed != &mSeeds[16]; ++seed)
		if (!((*seed)->mSeedFlags & TYumboSeed::SEED_FLAG_SPENT))
			return false;

	return true;
}

bool TYumbo::isChangedBlock() const
{
	return mSpine->getLatestNerve() == &TNerveSmallEnemyChange::theNerve();
}

// TODO: 0x48 bytes of frame too big. Every instruction but the register
// numbering around the first rotate matches; the retail build keeps dir.z in
// f31 across matan/sinf/cosf and reuses it as the rotate's vz, ours reloads it.
// The excess frame is the two JGeometry::TQuat4<f32>::rotate expansions, whose
// two TQuat4 temporaries are worth exactly 0x40 of it; dropping them makes this
// frame exact but renumbers registers at six other inline sites, so the header
// was left alone. The measurements are tabulated next to rotate() in
// JGQuat4.hpp.
void TYumbo::shotSeeds()
{
	TYumboSeed* seed = getUnusedSeed();
	if (!seed)
		return;

	mMActor->setBck(0);
	setCurAnmSound();

	JGeometry::TVec3<f32> dir = *gpMarioPos;
	dir.x -= mPosition.x;
	dir.y -= mPosition.y;
	dir.z -= mPosition.z;
	dir.y += 200.0f * (0.5f + MsRandF());
	dir.setLength(getSaveParams()->getSLShootSpeed());

	f32 yawRad = -(0.017453294f * MsGetRotFromZaxisY(dir));
	JGeometry::TQuat4<f32> q;
	q.setEulerY(yawRad);
	q.rotate(dir, dir);

	JGeometry::TQuat4<f32> roll;
	roll.setEulerY(6.2831855f * MsRandF());
	JGeometry::TQuat4<f32> pitch;
	pitch.setEulerX(-3.1415927f * getSaveParams()->getSLShootAngleX());
	q.mul(roll, pitch);
	q.rotate(dir, dir);

	seed->startToMove(mPosition, dir, getSaveParams()->getSLSeedLife());
}

void TYumbo::lookatMario()
{
	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario.sub(mPosition);
	f32 yaw     = MsGetRotFromZaxisY(toMario);
	mRotation.y = yaw;
}

void TYumbo::changeToFlower()
{
	mMActor = mMActorKeeper->getMActor("flower.bmd");
}

void TYumbo::changeToYumbo()
{
	mMActor = mMActorKeeper->getMActor("yumbo.bmd");
}

bool TYumbo::isWaterproof() const
{
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	return ((nerve == &TNerveYumboFreeze::theNerve()
	         || nerve == &TNerveSmallEnemyDie::theNerve())
	        || nerve == &TNerveYumboHiding::theNerve())
	    || nerve == &TNerveYumboAttack::theNerve();
}

bool TYumbo::isFreeze() const
{
	return mSpine->getLatestNerve() == &TNerveYumboFreeze::theNerve();
}

bool TYumbo::isDead() const
{
	return mSpine->getLatestNerve() == &TNerveSmallEnemyDie::theNerve();
}

TYumboSeed* TYumbo::getUnusedSeed()
{
	TYumboSeed** end = &mSeeds[16];
	for (TYumboSeed** seed = &mSeeds[0]; seed != end; ++seed)
		if ((*seed)->mSeedFlags & TYumboSeed::SEED_FLAG_SPENT)
			return *seed;

	return nullptr;
}

void TYumbo::setDeadAnm() { setBckAnm(3); }

bool TYumbo::doKeepDistance() { return isFreeze(); }

void TYumbo::attackToMario()
{
	if (!isFreeze())
		sendAttackMsgToMario();
}

TYumboParams::TYumboParams(const char* prm)
    : TSmallEnemyParams(prm)
    , PARAM_INIT(mRecoverTimer, 600)
    , PARAM_INIT(mShootSpeed, 55.0f)
    , PARAM_INIT(mShootAngleX, 0.2f)
    , PARAM_INIT(mSeedLife, 80)
    , PARAM_INIT(mSeedAirFric, 0.94f)
    , PARAM_INIT(mSeedGravityY, 0.9f)
{
	TParams::load(mPrmPath);
}

TYumboManager::TYumboManager(const char* name)
    : TSmallEnemyManager(name)
    , mMaterialTable(nullptr)
{
}

void TYumboManager::load(JSUMemoryInputStream& stream)
{
	TYumboParams* params = new TYumboParams("/enemy/Yumbo.prm");
	unk38                = params;

	TSmallEnemyManager::load(stream);

	params->mSLAttackRadius.set(97);
	params->mSLAttackHeight.set(225);
	params->mSLDamageRadius.set(90);
	params->mSLDamageHeight.set(225);

	loadMaterialTable(&mMaterialTable, "/scene/samboHead/flower_blue.bmt");
}

void TYumboManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "yumbo.bmd", 0x10210000, 0 },
		{ "flower.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TYumboManager::loadMaterialTable(J3DMaterialTable** table,
                                      const char* path)
{
	*table = J3DModelLoaderDataBase::loadMaterialTable(
	    JKRFileLoader::getGlbResource(path));
}

const char** TYumbo::getBasNameTable() const { return sambohead_bastable; }

// TODO: instruction-identical, 8 bytes of frame short -- the retail nerve
// gets lookatMario()'s inline-boundary temporaries that spelling its body out
// here cannot reproduce. Same for TNerveYumboAppearing (8 bytes).
DEFINE_NERVE(TNerveYumboDancing, TLiveActor)
{
	TYumbo* yumbo = (TYumbo*)spine->getBody();

	if (spine->getTime() == 0)
		yumbo->setBckAnm(2);

	// TODO: this is TYumbo::lookatMario() spelled out. Calling the method
	// instead puts MsGetRotFromZaxisY at inline depth 2, where MWCC stops
	// expanding it and emits a weak out-of-line copy the retail object does
	// not have -- so the original cannot have gone through the method here.
	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario.sub(yumbo->mPosition);
	f32 yaw            = MsGetRotFromZaxisY(toMario);
	yumbo->mRotation.y = yaw;

	if (yumbo->isFindOutMario()) {
		spine->pushAfterCurrent(&TNerveYumboHiding::theNerve());
		return TRUE;
	}
	return FALSE;
}

// Binding level over a raw member read, worth +8 of low region in
// TNerveYumboHiding::execute (batch 127).
static inline MSound* YunboGetMSound()
{
	MSound* mSound = gpMSound;
	return mSound;
}

// TODO: instruction-identical, 8 bytes of frame short.
DEFINE_NERVE(TNerveYumboHiding, TLiveActor)
{
	TYumbo* yumbo = (TYumbo*)spine->getBody();

	if (spine->getTime() == 0) {
		yumbo->setBckAnm(4);
		yumbo->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->setRate(
		    3.0f * SMSGetAnmFrameRate());
		yumbo->mHideEffectDone = false;
		if (YunboGetMSound()->gateCheck(MSD_SE_EN_YUMBO_SINK))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_EN_YUMBO_SINK, &yumbo->mPosition, 0, nullptr, 0, 4);
	}

	if (!yumbo->mHideEffectDone
	    && yumbo->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(34.0f)) {
		gpMarioParticleManager->emit(PARTICLE_MS_SMB_AP_ROCK,
		                             &yumbo->mPosition, 0, nullptr);
		gpMarioParticleManager->emit(PARTICLE_MS_SMB_AP_SMOKE,
		                             &yumbo->mPosition, 0, nullptr);
		yumbo->mHideEffectDone = true;
	}

	if (yumbo->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveYumboAttack::theNerve());
		return TRUE;
	}
	return FALSE;
}

// Binding level over a raw member read, worth +8 of low region in
// TNerveYumboAppearing::execute (batch 127).
static inline TMarioParticleManager* YunboGetMarioParticleManager()
{
	TMarioParticleManager* marioParticleManager = gpMarioParticleManager;
	return marioParticleManager;
}

DEFINE_NERVE(TNerveYumboAppearing, TLiveActor)
{
	TYumbo* yumbo = (TYumbo*)spine->getBody();

	if (spine->getTime() == 0) {
		yumbo->changeToYumbo();
		yumbo->setBckAnm(10);
		YunboGetMarioParticleManager()->emit(PARTICLE_MS_SMB_AP_ROCK,
		                             &yumbo->mPosition, 0, nullptr);
		gpMarioParticleManager->emit(PARTICLE_MS_SMB_AP_SMOKE,
		                             &yumbo->mPosition, 0, nullptr);
	}

	// TODO: see TNerveYumboDancing -- lookatMario() spelled out so that
	// MsGetRotFromZaxisY still expands inline.
	JGeometry::TVec3<f32> toMario = *gpMarioPos;
	toMario.sub(yumbo->mPosition);
	f32 yaw            = MsGetRotFromZaxisY(toMario);
	yumbo->mRotation.y = yaw;

	if (yumbo->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveYumboDancing::theNerve());
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveYumboAttack, TLiveActor)
{
	TYumbo* yumbo = (TYumbo*)spine->getBody();

	if (spine->getTime() == 0) {
		yumbo->changeToFlower();
		yumbo->shotSeeds();
	}

	if (yumbo->isWantToAppear()) {
		spine->pushAfterCurrent(&TNerveYumboAppearing::theNerve());
		return TRUE;
	}

	s32 seedLife = yumbo->getSaveParams()->getSLSeedLife();
	if (seedLife / 16 < spine->getTime()) {
		spine->pushAfterCurrent(this);
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveYumboFreeze, TLiveActor)
{
	TYumbo* yumbo = (TYumbo*)spine->getBody();

	if (spine->getTime() == 0) {
		yumbo->setBckAnm(11);
		if (gpMSound->gateCheck(MSD_SE_EN_COMMON_TWINKLE))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_EN_COMMON_TWINKLE, &yumbo->mPosition, 0, nullptr, 0,
			    4);
	}

	s32 recoverTimer = yumbo->getSaveParams()->getSLRecoverTimer();
	if (recoverTimer < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveYumboDancing::theNerve());
		return TRUE;
	}
	return FALSE;
}
