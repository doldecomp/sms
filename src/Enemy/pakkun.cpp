#include <Enemy/Pakkun.hpp>
// The pakkun nerves are declared next to the boss pakkun ones.
#include <Enemy/BossPakkun.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/EnemyAttachment.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/PollutionManager.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Camera/Camera.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/WaterGun.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// pakkun_bastable names slots 0-3, 7 and 8; the .bck files are indexed
// alphabetically, so 9 after "shoot" is the wait animation. 4, 5 and 6 sit
// between "hide" and "set" and are only reachable through the freeze nerve,
// which plays 6, then 4, then 5 as the water damage runs its course.
// TODO: confirm 4, 5 and 6 against the model data.
enum {
	PAKKUN_ANM_CRUSH_TO_HIDE = 0, // "pakun_crush_to_hide"
	PAKKUN_ANM_DAMAGE        = 1, // "pakun_damage"
	PAKKUN_ANM_DOWN          = 2, // "pakun_down"
	PAKKUN_ANM_HIDE          = 3, // "pakun_hide"
	PAKKUN_ANM_UNK4          = 4,
	PAKKUN_ANM_UNK5          = 5,
	PAKKUN_ANM_UNK6          = 6,
	PAKKUN_ANM_SET           = 7, // "pakun_set"
	PAKKUN_ANM_SHOOT         = 8, // "pakun_shoot"
	PAKKUN_ANM_WAIT          = 9,
};

static const char* pakkun_bastable[] = {
	"/scene/pakkun/bas/pakun_crush_to_hide.bas",
	"/scene/pakkun/bas/pakun_damage.bas",
	"/scene/pakkun/bas/pakun_down.bas",
	"/scene/pakkun/bas/pakun_hide.bas",
	nullptr,
	nullptr,
	nullptr,
	"/scene/pakkun/bas/pakun_set.bas",
	"/scene/pakkun/bas/pakun_shoot.bas",
	nullptr,
};

static TPakkun* gpCurPakkun;
static TPakkunSeed* gpCurPakkunSeed;

u8 TPakkun::mHeadJntIndex;

f32 TPakkunManager::mRootExplosionScaleRate = 2.0f;
f32 TPakkunManager::mTestFlyAngX            = 30.0f;
f32 TPakkunManager::mIgnoreHitWaterY        = 50.0f;

static int PakkunSeedCallback(J3DNode* node, int param);
static int PakkunRootCallback(J3DNode* node, int param);
static int PakkunRootCallback2(J3DNode* node, int param);

// The seed spins about its own axis while it flies.
// TODO: 8 bytes of frame short, and the 0.0f/1.0f the matrix is filled with
// are hoisted above the sine table lookups instead of loaded at their first
// use. MsSin/MsCos, swapping the sine and cosine locals and naming the model
// all leave it unchanged.
static int PakkunSeedCallback(J3DNode* node, int param)
{
	if (param == 0) {
		if (gpCurPakkunSeed == nullptr || gpCurPakkunSeed->unk168 != 0
		    || gpCurPakkunSeed->mPakkun->mIsHoldingSeed != 0) {
			return 1;
		}

		J3DJoint* joint = (J3DJoint*)node;
		MtxPtr anmMtx   = gpCurPakkunSeed->getMActor()->getModel()->getAnmMtx(
		    joint->getJntNo());
		f32 s = JMASin(gpCurPakkunSeed->mSpinAngle);
		f32 c = JMACos(gpCurPakkunSeed->mSpinAngle);

		Mtx spin;
		spin[0][0] = c;
		spin[0][1] = -s;
		spin[0][2] = 0.0f;
		spin[0][3] = 0.0f;
		spin[1][0] = s;
		spin[1][1] = c;
		spin[1][2] = 0.0f;
		spin[1][3] = 0.0f;
		spin[2][0] = 0.0f;
		spin[2][1] = 0.0f;
		spin[2][2] = 1.0f;
		spin[2][3] = 0.0f;

		MtxPtr spinMtx = spin;
		MTXConcat(anmMtx, spinMtx, anmMtx);
		MTXConcat(J3DSys::mCurrentMtx, spinMtx, J3DSys::mCurrentMtx);
	}
	return 1;
}

static inline f32 PakkunExplosionScale(const TPakkun* p)
{
	return p->mExplosionScale;
}

static inline TPakkun* PakkunCur()
{
	TPakkun* p = gpCurPakkun;
	return p;
}

static inline J3DModel* PakkunModelOf(TPakkun* p)
{
	return p->getMActor()->getModel();
}

static inline f32 PakkunCurExplosionScale()
{
	return PakkunExplosionScale(PakkunCur());
}

// The head swells as the stay pakkun takes damage. The y and z axes are
// stretched half again as much once it is past its original size.
// TODO: instruction-identical bar the u8 note below, 32 bytes of frame short.
// PakkunRootCallback2 and TPakkun::init are short by the same 32 and 24.
static int PakkunRootCallback(J3DNode* node, int param)
{
	if (param == 0) {
		if (PakkunCur() == nullptr
		    || gpCurPakkun->getHitPoints()
		           == gpCurPakkun->getMaxHitPoints())
			return 1;

		J3DJoint* joint = (J3DJoint*)node;
		MtxPtr anmMtx
		    = PakkunModelOf(PakkunCur())->getAnmMtx(joint->getJntNo());

		Mtx scale;
		scale[0][3] = 0.0f;
		scale[1][3] = 0.0f;
		scale[2][3] = 0.0f;

		f32 s  = PakkunCur()->mExplosionScale;
		f32 sy = s;
		f32 sz = s;
		if (s > 1.0f) {
			sy *= 1.5f;
			sz *= 1.5f;
		}

		scale[0][0] = s;
		scale[0][1] = 0.0f;
		scale[0][2] = 0.0f;
		scale[1][0] = 0.0f;
		scale[1][1] = sy;
		scale[1][2] = 0.0f;
		scale[2][0] = 0.0f;
		scale[2][1] = 0.0f;
		scale[2][2] = sz;
		MTXConcat(anmMtx, scale, anmMtx);

		scale[0][0] = s;
		scale[0][1] = 0.0f;
		scale[0][2] = 0.0f;
		scale[1][0] = 0.0f;
		scale[1][1] = s;
		scale[1][2] = 0.0f;
		scale[2][0] = 0.0f;
		scale[2][1] = 0.0f;
		scale[2][2] = s;
		MTXConcat(J3DSys::mCurrentMtx, scale, J3DSys::mCurrentMtx);
	}
	return 1;
}

// The joint under the swelling head has to be scaled back down so that the
// stem keeps its shape.
// TODO: frame exact; retail loads the 1.0f into f0 and the scale into f2 for
// the divide (ours swapped). Inert: a named divisor, `s = s / x`, `1.0f / x`.
static int PakkunRootCallback2(J3DNode* node, int param)
{
	if (param == 0) {
		if (gpCurPakkun == nullptr)
			return 1;

		J3DJoint* joint = (J3DJoint*)node;
		MtxPtr anmMtx
		    = PakkunModelOf(gpCurPakkun)->getAnmMtx(joint->getJntNo());

		Mtx scale;
		scale[0][3] = 0.0f;
		scale[1][3] = 0.0f;
		scale[2][3] = 0.0f;

		f32 s = 1.0f;
		s /= PakkunCurExplosionScale();
		scale[0][0] = s;
		scale[0][1] = 0.0f;
		scale[0][2] = 0.0f;
		scale[1][0] = 0.0f;
		scale[1][1] = s;
		scale[1][2] = 0.0f;
		scale[2][0] = 0.0f;
		scale[2][1] = 0.0f;
		scale[2][2] = s;

		MTXConcat(anmMtx, scale, anmMtx);
		MTXConcat(J3DSys::mCurrentMtx, scale, J3DSys::mCurrentMtx);
	}
	return 1;
}

TPakkunSaveLoadParams::TPakkunSaveLoadParams(const char* prm)
    : TSmallEnemyParams(prm)
    , PARAM_INIT(mSLReadyTime, 100)
    , PARAM_INIT(mSLShootRange, 1000.0f)
    , PARAM_INIT(mSLSeedGravityS, 0.01f)
    , PARAM_INIT(mSLSeedSpeedS, 5.0f)
    , PARAM_INIT(mSLSeedGravityC, 0.01f)
    , PARAM_INIT(mSLSeedSpeedC, 10.0f)
    , PARAM_INIT(mSLMarioCircle, 400.0f)
    , PARAM_INIT(mSLLimitMove, 1000.0f)
    , PARAM_INIT(mSLMoveDist, 1200.0f)
    , PARAM_INIT(mSLGenerateSeedTime, 300)
    , PARAM_INIT(mSLGenerateSeedDist, 300.0f)
    , PARAM_INIT(mSLDamageHideTime, 300)
{
	TParams::load(mPrmPath);
}

TPakkunManager::TPakkunManager(const char* name)
    : TSmallEnemyManager(name)
    , mRipWater(nullptr)
    , mHideWater(nullptr)
{
	gpCurPakkun     = nullptr;
	gpCurPakkunSeed = nullptr;
	unk5C           = 0;
}

void TPakkunManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38       = new TPakkunSaveLoadParams("/enemy/pakkun.prm");
	mStayParams = new TPakkunSaveLoadParams("/enemy/staypakkun.prm");
	mRipWater   = new TWaterEmitInfo("/enemy/pakkunwater.prm");
	mHideWater  = new TWaterEmitInfo("/enemy/pakkunhide.prm");
}

void TPakkunManager::loadAfter() { TSmallEnemyManager::loadAfter(); }

void TPakkunManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "pakun.bmd", 0x10300000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

// TODO: the original calls TPakkun's constructor here; ours inlines it. The
// ctor is byte-exact and this is its only call site, so it is the same case as
// TLimitKoopa::startHipDrop in docs/catalog/codegen-tells.md. Correction to
// the earlier note here: statement count *is* the lever -- exactly five more
// statements in the ctor body flip it to a `bl` and land this function at
// 100% (four or fewer make it worse, and moving the six member initialisers
// from the list into the body is inert, so the list and the body cost the
// same). What is missing is therefore five real statements in the ctor, and
// the byte-exact 0x64 body leaves no room for them.
TSpineEnemy* TPakkunManager::createEnemyInstance()
{
	return new TPakkun;
}

// Each pakkun clips its seeds along with itself, so the seeds keep drawing
// while they are in the air but stop the moment the plant goes out of view.
void TPakkunManager::clipEnemies(JDrama::TGraphics* graphics)
{
	f32 clipRadius;
	f32 farClip;
	if (unk38 == nullptr) {
		clipRadius = 300.0f;
		farClip    = gpConductor->getCondParams().getEnemyFarClip();
	} else {
		farClip    = unk38->mSLFarClip.get();
		clipRadius = unk38->mSLClipRadius.get();
	}

	SetViewFrustumClipCheckPerspective(SMSGetCamera()->getFovy(),
	                                   SMSGetCamera()->getAspect(),
	                                   graphics->mNearPlane, farClip);

	for (int i = 0; i < getObjNum(); i++) {
		TPakkun* pakkun = (TPakkun*)getObj(i);

		if (pakkun->mIsStay != 0) {
			pakkun->updateSquareToMario();
			if (pakkun->getDistToMarioSquared() < farClip * farClip)
				continue;
		}

		if (ViewFrustumClipCheck(graphics, &pakkun->mPosition, clipRadius))
			pakkun->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		else
			pakkun->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);

		TPakkunSeed* seed = pakkun->mSeed;
		if (!seed->isUnk150Zero()) {
			if (ViewFrustumClipCheck(graphics, &seed->mPosition, clipRadius))
				pakkun->mSeed->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
			else
				pakkun->mSeed->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		}

		if (pakkun->mIsStay != 0) {
			for (int j = 0; j < 2; j++) {
				TPakkunSeed* subSeed = pakkun->mSubSeeds[j];
				if (!subSeed->isUnk150Zero()) {
					if (ViewFrustumClipCheck(graphics, &subSeed->mPosition,
					                         clipRadius)) {
						pakkun->mSubSeeds[j]->offLiveFlag(
						    LIVE_FLAG_CLIPPED_OUT);
					} else {
						pakkun->mSubSeeds[j]->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
					}
				}
			}
		}
	}
}

TPakkun::TPakkun(const char* name)
    : TSmallEnemy(name)
    , mSeed(nullptr)
    , mIsCurveShot(0)
    , mIsStay(0)
    , mSubSeeds(nullptr)
    , mExplosionScale(1.0f)
    , mDamageHiding(0)
{
}

void TPakkun::load(JSUMemoryInputStream& stream)
{
	TSmallEnemy::load(stream);
	reset();
	setGoalPathMario();
}

static inline J3DModelData* PakkunModelData(const TPakkun* p)
{
	J3DModelData* data = p->getModel()->getModelData();
	return data;
}

// TODO: frame 8 long. Raw mMActor at either material-loop site lands the frame
// but leaves setGoalPathMario's path node 4 low; accessor swaps elsewhere are worse.
void TPakkun::init(TLiveManager* manager)
{
	TSmallEnemy::init(manager);
	mActorType = 0x10000004;
	unk150     = 0x11;
	mSaveParams = (TPakkunSaveLoadParams*)getSaveParam();

	mSpine->initWith(&TNervePakkunGenerate::theNerve());
	setGoalPathMario();

	if (mInstanceIndex == 0) {
		for (u8 i = 0;
		     i < PakkunModelData(this)->getJointNum(); i++) {
			if (strcmp(PakkunModelData(this)->getJointName()->getName(i),
			           "null_seed")
			    == 0) {
				mHeadJntIndex = i;
			}
		}
	}

	mSeed = new TPakkunSeed("パックン種");
	mSeed->loadInit(this, "seed.bmd");
	mSeed->unk164 = mBodyScale;

	const ResTIMG* dummy = (const ResTIMG*)JKRFileLoader::getGlbResource(
	    "/scene/map/pollution/H_ma_rak.bti");
	if (dummy) {
		SMS_ChangeTextureAll(mMActor->getModel()->getModelData(),
		                     "H_ma_rak_dummy", *dummy);
	}

	for (u16 i = 0;
	     i < getMActor()->getModel()->getModelData()->getMaterialNum(); i++) {
		SMS_InitPacket_OneTevKColor(getMActor()->getModel(), i, GX_KCOLOR0,
		                            &mBodyColor);
	}

	mMActor->setJointCallback(1, PakkunRootCallback);
	mMActor->setJointCallback(2, PakkunRootCallback2);
}

// The plant is rooted in the ground, so nothing may shove it around.
BOOL TPakkun::isInhibitedForceMove() { return TRUE; }

void TPakkun::kill()
{
	TSmallEnemy::kill();
	mSeed->kill();
}

void TPakkun::setWaitAnm() { setBckAnm(PAKKUN_ANM_WAIT); }

void TPakkun::setFreezeAnm() { setBckAnm(PAKKUN_ANM_DAMAGE); }

void TPakkun::setDeadAnm()
{
	if (!checkLiveFlag(LIVE_FLAG_HIDDEN))
		setBckAnm(PAKKUN_ANM_DOWN);
}

// TODO: dead code. 0x20 bytes is the two-statement predicate shape; the one
// the hide nerve wants (the animation is over and the plant is out of sight)
// is a guess.
bool TPakkun::isHideEnd() const
{
	if (checkLiveFlag(LIVE_FLAG_HIDDEN))
		return true;
	return false;
}

// TODO: instruction-identical but with cue/graphics/this in r30/r29/r31
// where the original uses r31/r30/r29 -- pure callee-saved renumbering.
void TPakkun::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (checkLiveFlag(LIVE_FLAG_UNK200))
		return;

	mSeed->perform(cue, graphics);

	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	if (cue & CUE_MOVE) {
		if (mIsStay != 0)
			offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		if (!gpMap->isInArea(mPosition.x, mPosition.z))
			kill();
		moveObject();
	}

	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		if (cue & CUE_CALC_ANIM)
			getMActor()->frameUpdate();
	} else {
		if (cue & CUE_CALC_ANIM) {
			calcRootMatrix();
			updateAnmSound();
			getMActor()->calcAnm();
		}
		if (!checkLiveFlag(LIVE_FLAG_HIDDEN)) {
			if (cue & CUE_CALC_VIEW)
				getMActor()->viewCalc();
			if (cue & CUE_ENTRY)
				drawObject(graphics);
		}
	}
}

// The goop puff the plant throws up while it moves through dirty ground.
// TNerveStayPakkunAppear spells the same block out, which is where the shape
// comes from; adding the pollution test in front of it gives 0x94 against the
// map's 0x98, so one statement is still missing. The two-emit form the hide
// nerve uses instead is 0xac, too big.
// TODO: 4 bytes short.
void TPakkun::createPakkunSmoke(JGeometry::TVec3<f32>& pos)
{
	if (gpPollution->isPolluted(pos.x, pos.y, pos.z)) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emit(
		    PARTICLE_MS_GENE_HIT, &pos, 1, this);
		if (emitter) {
			emitter->setGlobalScale(
			    JGeometry::TVec3<f32>(1.5f, 1.5f, 1.5f));
			SMSSetEmitterPolColor(emitter, 6);
		}
	}
}

// TODO: dead code. 0x6c bytes is one stamp call with the manager's stamp type
// and the body-scaled range, as TSmallEnemy's death stamp does.
void TPakkun::seedPollute(JGeometry::TVec3<f32>& pos)
{
	TSmallEnemyManager* manager = (TSmallEnemyManager*)mManager;
	gpPollution->stamp(manager->getUnk58(), pos.x, pos.y, pos.z,
	                   manager->getSaveParam2()->getSLStampRange() * 32.0f
	                       * unk158);
}

// Spits the seed straight at Mario: the goal path is pushed out 200 units
// along the direction so the plant keeps facing the shot.
void TPakkun::onShootLiner(JGeometry::TVec3<f32>& dir)
{
	JGeometry::TVec3<f32> goal(getPosition());
	goal.x += 200.0f * dir.x;
	goal.z += 200.0f * dir.z;
	setGoalPath(TPathNode(goal));
	mIsCurveShot = 0;

	if (dir.x == 0.0f && dir.y == 0.0f && dir.z == 0.0f)
		dir.x += 1.0f;
	MsVECNormalize(&dir, &dir);

	// Read raw: the const f32& from TParamT::get() is 0x10 of frame here.
	// TODO: frame is exact now, but every stack temporary still sits 4 bytes
	// low (goal at 0x28 against retail's 0x2c) -- 4 more bytes of low pool
	// are missing; a named TPathNode and a const f32& binding both fail.
	f32 speed = mSaveParams->mSLSeedSpeedS.value;
	dir.x *= speed;
	dir.y = -5.0f;
	dir.z *= speed;

	mSeed->mVelocity = dir;
}

// Lobs the seed at a goal point instead of spitting it straight: the arc is
// the one that lands on the goal from where the plant stands. Size-exact
// against the map's 0xfc, which is what fixes the body: the goal feeds
// both setGoalPath and the jump velocity (retail passes the goal copy, not
// mPosition, at every inlined site). The map
// dead-strips it because TNervePakkunStay inlines it at all three lob sites,
// where the velocity is a word copy (`=`, not set(x, y, z)).
void TPakkun::onShootCurve(JGeometry::TVec3<f32>& goal)
{
	setGoalPath(TPathNode(goal));
	JGeometry::TVec3<f32> vel
	    = calcVelocityToJumpToY(goal, getSaveLoadParam()->getSLSeedSpeedC(),
	                            getSaveLoadParam()->getSLSeedGravityC());
	mIsCurveShot = 1;
	mSeed->mVelocity = vel;
	mSeed->mRotation.set(TPakkunManager::mTestFlyAngX, 0.0f, 0.0f);
}

// Being sprayed knocks four points off and sends the plant into the freeze
// animation, unless it is already reacting to water.
// TODO: instruction-exact with an 8-byte frame gap. getHitPoints(),
// getGroundHeight() and naming the current nerve all change instructions
// instead of only the frame.
void TPakkun::behaveToWater(THitActor* sender)
{
	mSprayedByWaterCooldown = 0;

	if (sender->getPosition().y
	    > mGroundHeight + TPakkunManager::mIgnoreHitWaterY) {
		unk165 = true;
		if (mHitPoints > 5)
			mHitPoints -= 4;

		if (mSpine->getCurrentNerve() != &TNervePakkunFreeze::theNerve()
		    && mSpine->getCurrentNerve()
		           != &TNerveStayPakkunAppear::theNerve()
		    && mSpine->getCurrentNerve()
		           != &TNerveStayPakkunHide::theNerve()) {
			mSpine->pushNerve(&TNervePakkunFreeze::theNerve());
			if (mSeed->isUnk150Zero())
				mSeed->kill();
		}
	}
}

void TPakkun::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("pakun.bmd", 3);
}

void TPakkun::reset()
{
	gpCurPakkun = this;
	TSmallEnemy::reset();
	mIsHoldingSeed  = 1;
	mBodyColor.a    = 0;
	mExplosionScale = 1.0f;
}

void TPakkun::shootIn()
{
	mSeed->appear();
	mSeed->set();
}

void TPakkun::shoot() { mSeed->shoot(); }

const char** TPakkun::getBasNameTable() const { return pakkun_bastable; }

TPakkunSeed::TPakkunSeed(const char* name)
    : TEnemyAttachment(name)
    , mPakkun(nullptr)
    , mSpinAngle(0.0f)
{
}

void TPakkunSeed::loadInit(TSpineEnemy* host, const char* model)
{
	unk160        = host;
	mMActorKeeper = new TMActorKeeper(unk160->getManager(), 1);
	getActorKeeper()->mModelLoaderFlags = 0x10220000;
	mMActor       = getActorKeeper()->createMActor(model, 3);
	mPakkun       = (TPakkun*)unk160;

	TIdxGroupObj* group
	    = JDrama::TNameRefGen::search<TIdxGroupObj>("オブジェクトグループ");
	group->getChildren().push_back(this);

	initHitActor(0x10000006, 1, -0x80000000, 20.0f, 20.0f, 20.0f, 20.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	unk150       = SEED_STATE_DEAD;
	mGroundPlane = gpMap->getIllegalCheckData();

	getMActor()->getModel()->getModelData()->getJointNodePointer(0)
	    ->setCallBack(PakkunSeedCallback);
}

void TPakkunSeed::moveObject()
{
	TEnemyAttachment::moveObject();
	if (unk168 == 0) {
		mSpinAngle = MsWrap(5.0f + mSpinAngle, 0.0f, 360.0f);
		if (mPosition.y > 20.0f + mGroundHeight) {
			mRotation.x = MsGetRotFromZaxis(JGeometry::TVec3<f32>(getVelocity())).x;
		}
	} else {
		mSpinAngle = MsClamp(5.0f + mSpinAngle, 0.0f, 360.0f);
		mRotation.x *= 0.8f;
	}
}

void TPakkunSeed::behaveToHost()
{
	if (mPakkun->mIsStay == 0)
		unk160->offLiveFlag(LIVE_FLAG_HIDDEN);
}

void TPakkunSeed::behaveToHitWall(const TBGCheckData* wall)
{
	f32 bounce = -(1.5f * mVelocity.dot(wall->getNormal()));
	mVelocity.x += bounce * wall->getNormal().x;
	mVelocity.y += bounce * wall->getNormal().y;
	if (unk150 == SEED_STATE_LINER)
		mVelocity.y = -5.0f;
	mVelocity.z += bounce * wall->getNormal().z;
	mPakkun->mSeedHitWall = 1;
}

void TPakkunSeed::calcRootMatrix()
{
	gpMSound->startSoundActor(MSD_SE_EN_PAKKUN_SEED_FLY, &mPosition);
	TEnemyAttachment::calcRootMatrix();
	gpCurPakkunSeed = this;
}

void TPakkunSeed::behaveToHitGround()
{
	if (fabsf(mVelocity.y) < 1.0f || mGroundPlane->isWaterSurface()) {
		unk168 = 1;
		offLiveFlag(LIVE_FLAG_AIRBORNE);
		mVelocity.x = 0.0f;
		mVelocity.y = -0.3f;
		mVelocity.z = 0.0f;
		rebirth();
		return;
	}

	if (unk150 == SEED_STATE_LINER) {
		mVelocity.x *= 0.8f;
		mVelocity.z *= 0.8f;
		mVelocity.y = 0.1f * fabsf(mVelocity.y);
	} else {
		mVelocity.x *= 0.4f;
		mVelocity.z *= 0.4f;
		mVelocity.y = 0.4f * fabsf(mVelocity.y);
	}
}

f32 TPakkunSeed::getNowGravity()
{
	TPakkunSaveLoadParams* params
	    = (TPakkunSaveLoadParams*)mPakkun->getSaveParam();
	f32 gravity = params->mSLSeedGravityS.get();
	if (unk150 == SEED_STATE_CURVE)
		gravity = params->mSLSeedGravityC.get();
	return gravity;
}

void TPakkunSeed::appear()
{
	unk150                   = SEED_STATE_HELD;
	mPakkun->mSeedHitWall    = 0;
	mScaling.x = mScaling.y = mScaling.z = 0.5f;
}

void TPakkunSeed::shoot()
{
	switch (mPakkun->mIsCurveShot) {
	case 0:
		unk150 = SEED_STATE_LINER;
		break;
	case 1:
		unk150 = SEED_STATE_CURVE;
		break;
	}
	mScaling.x = 0.1f;
	mScaling.y = 0.1f;
	mScaling.z = 0.1f;
	unk168     = 0;
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

// Where the seed ends up once it has come to rest: a stay pakkun's seeds just
// stain the ground, everyone else's sprout a new pakkun or a water column.
void TPakkunSeed::rebirth()
{
	if (mPakkun->mIsStay != 0) {
		unk150 = SEED_STATE_DEAD;
		unk158 = 0;
		onHitFlag(HIT_FLAG_NO_COLLISION);

		TSmallEnemyManager* manager
		    = (TSmallEnemyManager*)mPakkun->getManager();
		gpPollution->stamp(manager->getUnk58(), mPosition.x, mPosition.y,
		                   mPosition.z,
		                   manager->getSaveParam2()->getSLStampRange() * 32.0f
		                       * mPakkun->getUnk158());

		gpMSound->startSoundActor(MSD_SE_EN_PAKKUN_SEED_SINK, &mPosition, 0,
		                          nullptr, 0, 4);
		return;
	}

	unk158++;
	if (unk158 > mPakkun->getSaveLoadParam()->getSLGenerateSeedTime()
	    || mPakkun->mIsHoldingSeed != 0) {
		unk150 = SEED_STATE_DEAD;
		unk158 = 0;
		onHitFlag(HIT_FLAG_NO_COLLISION);
	}

	if (mPosition.y < mGroundHeight - 70.0f) {
		mVelocity.y = 0.0f;
		onHitFlag(HIT_FLAG_NO_COLLISION);
		return;
	}

	if (!mGroundPlane->isWaterSurface()) {
		TPakkun* pakkun = mPakkun;
		gpMSound->startSoundActor(MSD_SE_EN_PAKKUN_SEED_SINK, &mPosition, 0,
		                          nullptr, 0, 4);
		// TODO: name 0x13E/0x13F in System/Particles.hpp; they are the two
		// halves of the seed sinking into the ground.
		gpMarioParticleManager->emit(0x13E, &mPosition, 1, pakkun->mSeed);
		gpMarioParticleManager->emit(0x13F, &mPosition, 1, pakkun->mSeed);
		return;
	}

	TEffectColumWater* effect = (TEffectColumWater*)
	    gpConductor->makeOneEnemyAppear(mPosition, "エフェクト水柱マネージャー",
	                                    0);
	if (effect)
		effect->generate(mPosition, mScaling);
}

// Parks the seed on the joint it is held by, or on the plant itself while the
// plant is clipped out and there is no animation matrix to read.
void TPakkunSeed::set()
{
	TEnemyAttachment::set();

	if (unk160->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		mPosition.x = unk160->mPosition.x;
		mPosition.y = 80.0f + unk160->mPosition.y;
		mPosition.z = unk160->mPosition.z;
		return;
	}

	u8 joint     = TPakkun::mHeadJntIndex;
	MtxPtr mtx   = mPakkun->getModel()->getAnmMtx(joint);
	mPosition.x = mtx[0][3];
	mPosition.y = mtx[1][3] - 50.0f;
	mPosition.z = mtx[2][3];
}

// Puts the seed back on its joint without repositioning it, and grows it back
// to full size. Inlined into TNervePakkunGenerate, which is where the direct
// call to the base set() comes from: the seed's own override would have been a
// vtable dispatch, and the map's 0x3c leaves no room for one.
void TPakkunSeed::seedSet()
{
	TEnemyAttachment::set();
	mScaling.x = mScaling.y = mScaling.z = unk164;
}

void TPakkunSeed::forceKill()
{
	if (getGroundPlane()->isPool() || getGroundPlane()->isIllegalData()
	    || !gpMap->isInArea(getPosition().x, getPosition().z)) {
		kill();
		if (mPakkun->mIsStay == 0) {
			if (unk160->checkLiveFlag(LIVE_FLAG_HIDDEN)) {
				unk160->kill();
				unk160->onLiveFlag(LIVE_FLAG_UNK20000);
			}
		}
	}
}

// TODO: the original calls JGeometry::TVec3<f>::set<f>(0, 0, 0) out of line
// from TPathNode(THitActor*) here, while TPakkun::load two functions up
// expands it. docs/catalog/codegen-tells.md rules out spelling the zero as
// an initialiser in PathNode.hpp (it fixes this site and regresses about
// fifteen other units), so the whole 20% gap stays per-call-site.
// TODO: shared-header need, measured. Retail's body here `bl`s
// JGeometry::TVec3<f>::set<f>(0,0,0) inside the inlined TPathNode(THitActor*)
// constructor -- that call is this TU's one MISSING map symbol -- and keeps
// the Mario pointer in r31 across it, which is the whole 20% gap. The chain
// is setGoalPathMario (depth 1) -> setGoalPath (2) -> the implicit TPathNode
// conversion (3) -> the zero-init (4), and a 3-statement in-class TVec3
// member is a `bl` at depth 4. Our PathNode.hpp constructor stores the three
// zeros in place instead of calling `unk4.set(0.0f, 0.0f, 0.0f)`, so there is
// nothing to call. Refuted in header round 52: spelling the set there is NOT
// the fix. The conversion temporary is built in setGoalPathMario's own body,
// not inside setGoalPath (which takes the node by const reference), so the
// zero-init lands at depth 3 and MWCC expands it either way -- this site
// stays at 80.1% and the MISSING symbol stays, with the whole tree
// byte-identical. Also refuted: delegating to TPakkun::load, which pushes the
// constructor itself to depth 4 and turns *it* into a bl.
// Round n6: TPakkun::load has the identical call (setGoalPathMario after
// reset) and retail expands the zero there, so the choice is made by the
// caller, not the chain. With the initialiser, this site is 100% and
// TPakkun::load spelled `setGoalPath(mario)` directly is 99.7% (node slot
// 4 low); the ~20 other TPathNode(THitActor*) sites in 14 units still regress.
void TStayPakkun::load(JSUMemoryInputStream& stream)
{
	TSmallEnemy::load(stream);
	reset();
	setGoalPathMario();
	mIsStay = 1;
}

void TStayPakkun::init(TLiveManager* manager)
{
	TPakkun::init(manager);
	mSubSeeds = new TPakkunSeed*[2];
	for (int i = 0; i < 2; i++) {
		mSubSeeds[i] = new TPakkunSeed("パックン種");
		mSubSeeds[i]->loadInit(this, "seed.bmd");
	}
	mSpine->initWith(&TNerveStayPakkunHide::theNerve());
}

void TStayPakkun::reset()
{
	TPakkun::reset();
	mBodyColor.a = 0xFF;
	offLiveFlag(LIVE_FLAG_UNK800);
	onLiveFlag(LIVE_FLAG_HIDDEN);
}

void TStayPakkun::setDeadAnm() { setBckAnm(PAKKUN_ANM_DOWN); }

// A sprayed stay pakkun coughs up either a full water refill or a coin,
// depending on how much water Mario has left.
void TStayPakkun::genRandomItem()
{
	TPakkunManager* manager = (TPakkunManager*)mManager;

	mEffectPos = mPosition;
	mEffectPos.y += 100.0f;
	manager->mRipWater->mPos.value = mEffectPos;
	gpModelWaterManager->emitRequest(*manager->mRipWater);

	s32 maxWater = SMS_GetMarioWaterGun()->getMaxWater();
	if (SMS_GetMarioWaterGun()->mCurrentWater * 4 < maxWater) {
		gpItemManager->makeObjAppear(mPosition.x, mPosition.y, mPosition.z,
		                             0x20000002, true);
	} else {
		unk18C = 3;
		genEventCoin();
	}

	// TODO: name 0xA1 in System/Particles.hpp; it pairs with
	// PARTICLE_MS_POPO_BOMB_B.
	JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(0xA1, &mEffectPos, 0, nullptr);
	if (emitter)
		emitter->setGlobalScale(JGeometry::TVec3<f32>(1.5f, 1.5f, 1.5f));

	emitter = gpMarioParticleManager->emit(PARTICLE_MS_POPO_BOMB_B,
	                                       &mEffectPos, 0, nullptr);
	if (emitter)
		emitter->setGlobalScale(JGeometry::TVec3<f32>(1.5f, 1.5f, 1.5f));
}

void TStayPakkun::calcRootMatrix()
{
	gpCurPakkun = this;
	TSpineEnemy::calcRootMatrix();
}

// Called once a frame while the plant is out: it bleeds a point every time
// the damage animation comes round, and the head swells as the alpha drops.
void TStayPakkun::setBehavior()
{
	if (isBckAnm(PAKKUN_ANM_UNK4))
		mHitPoints--;

	mBodyColor.a = mHitPoints * 255 / getMaxHitPoints();
	mExplosionScale
	    = 1.0f
	      + TPakkunManager::mRootExplosionScaleRate * (255 - mBodyColor.a)
	            / 255.0f;

	if (mHitPoints < 2) {
		gpMSound->startSoundActor(MSD_SE_EN_PAKKUN_RIP, &mPosition);
		mHitPoints = 1;
		kill();
	}
}

bool TStayPakkun::isHitValid(u32 message)
{
	if (message == 0xB) {
		onLiveFlag(LIVE_FLAG_DEAD);
		onLiveFlag(LIVE_FLAG_UNK20000);
		return true;
	}

	if (mSpine->getCurrentNerve() != &TNerveStayPakkunAppear::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveStayPakkunHide::theNerve()) {
		mSpine->setNext(&TNerveStayPakkunHide::theNerve());
		mDamageHiding = 1;
		f32 range = getSaveParams()->getSLPolluteRange();
		gpPollution->clean(mPosition.x, mGroundHeight, mPosition.z,
		                   32.0f * range);
		if (mSeed->isUnk150Zero())
			mSeed->kill();
		setBckAnm(PAKKUN_ANM_CRUSH_TO_HIDE);
	}
	return false;
}

// Binding level over a raw member read, worth +8 of low region in
// TStayPakkun::shootIn (batch 127).
static inline TPakkunSeed* PakkunSeed(const TStayPakkun* p)
{
	TPakkunSeed* seed = p->mSeed;
	return seed;
}

// All three seeds come out at once, the outer two fanned ten degrees either
// side of the middle one.
void TStayPakkun::shootIn()
{
	PakkunSeed(this)->appear();
	mSeed->set();

	for (int i = 0; i < 2; i++) {
		mSubSeeds[i]->appear();
		mSubSeeds[i]->set();

		JGeometry::TVec3<f32> vel = mSeed->mVelocity;
		f32 yaw                   = -10.0f;
		if (i != 0)
			yaw *= -1.0f;

		Mtx rot;
		MsMtxSetRotRPH(rot, 0.0f, yaw, 0.0f);
		MTXMultVec(rot, &vel, &vel);
		mSubSeeds[i]->mVelocity = vel;
	}
}

void TStayPakkun::kill()
{
	TSmallEnemy::kill();
	mSeed->kill();
	for (int i = 0; i < 2; i++)
		mSubSeeds[i]->kill();
}

void TStayPakkun::shoot()
{
	mSeed->shoot();
}

TSpineEnemyParams* TStayPakkun::getSaveParam() const
{
	return ((TPakkunManager*)mManager)->mStayParams;
}

static inline TPakkunSeed* PakkunSeedB(const TPakkun* p)
{
	TPakkunSeed* seed = p->mSeed;
	return seed;
}

static inline TPakkun* PakkunBody(TSpineBase<TLiveActor>* spine)
{
	TLiveActor* body = spine->getBody();
	TPakkun* self    = (TPakkun*)body;
	return self;
}

// Waits underground with the seed in its mouth until Mario walks close
// enough, then drops the seed and opens up.
DEFINE_NERVE(TNervePakkunGenerate, TLiveActor)
{
	TPakkun* self = PakkunBody(spine);

	if (spine->getTime() == 0) {
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
		self->onLiveFlag(LIVE_FLAG_HIDDEN);
		PakkunSeedB(self)->appear();
	}

	if (self->mHolder != nullptr)
		return FALSE;

	if (PakkunSeedB(self)->isHeld()) {
		PakkunSeedB(self)->seedSet();

		if (spine->getTime() % 5 == 0) {
			self->updateSquareToMario();
			f32 dist = self->getSaveLoadParam()->getSLGenerateSeedDist();
			dist *= dist;
			if (self->getDistToMarioSquared() < dist)
				self->mSeed->unk150 = SEED_STATE_DROP;
		}
	}

	if (self->mSeed->isUnk150Zero()) {
		self->mPosition     = self->mSeed->mPosition;
		self->mPosition.y   = self->mSeed->mGroundHeight;
		spine->pushAfterCurrent(&TNervePakkunAppear::theNerve());
		self->setBckAnm(PAKKUN_ANM_SET);
		return TRUE;
	}
	return FALSE;
}

// The plant is out of the ground and turning to face Mario; it decides here
// whether to spit, to lob or to duck back down. The three distance tests are
// TSpineEnemy::calcDist, which is what keeps TUtil<f32>::sqrt a call.
// TODO: instruction-exact, frame 0x70 long (0x340 vs 0x2d0). marioPos sits
// 0x68 high, the lob goal and the liner dir are allocated in the opposite
// order to retail, and the low region is 0x38-0x40 deep. Inert: dir built by
// set(), the lob goal declared above the if, a raw *gpMarioPos copy.
DEFINE_NERVE(TNervePakkunStay, TLiveActor)
{
	TPakkun* self = (TPakkun*)spine->getBody();

	if (spine->getTime() == 0)
		self->setWaitAnm();

	int waitTime  = self->getSaveParams()->getSLWaitTime();
	s32 readyTime = self->getSaveLoadParam()->getSLReadyTime();

	if (self->mSeed->isUnk150Zero() && self->checkCurAnmEnd(0)
	    && (!(spine->getTime() < readyTime) || spine->getTime() >= waitTime
	        || self->mIsHoldingSeed != 0)) {
		f32 dist = self->calcDist(self->unk104.getPoint(), self->mPosition);

		f32 range = self->getSaveLoadParam()->getSLShootRange();
		f32 rate = 1.0f;
		if (self->mIsStay != 0)
			rate = 3.0f;

		JGeometry::TVec3<f32> marioPos(SMS_GetMarioPos());
		if (dist < range * rate
		    || self->mIsStay != 0) {
			f32 searchHeight
			    = self->getSaveParams()->getSLSearchHeight() * rate;
			if (fabsf(SMS_GetMarioPos().y - self->mPosition.y)
			    < searchHeight) {
				f32 length = self->getSaveParams()->getSLSearchLength();
				f32 angle  = self->getSaveParams()->getSLSearchAngle();
				if (self->isInSight(marioPos, length * rate, angle * rate,
				                    self->getSaveParams()->getSLSearchAware()
				                        * rate)) {
					spine->pushAfterCurrent(&TNervePakkunStay::theNerve());
					spine->pushAfterCurrent(&TNervePakkunShoot::theNerve());
					self->mIsHoldingSeed = 0;

					if (self->mSeedHitWall == 0 || self->mIsStay != 0) {
						JGeometry::TVec3<f32> dir(
						    SMS_GetMarioPos().x - self->mPosition.x,
						    SMS_GetMarioPos().y - self->mPosition.y,
						    SMS_GetMarioPos().z - self->mPosition.z);
						self->onShootLiner(dir);
					} else {
						self->mSeedHitWall = 0;
						JGeometry::TVec3<f32> goal(self->getUnk104().getPoint());
						self->onShootCurve(goal);
					}
					return TRUE;
				}
			}
		} else if (spine->getTime() >= waitTime) {
			spine->pushAfterCurrent(&TNervePakkunHide::theNerve());
			spine->pushAfterCurrent(&TNervePakkunShoot::theNerve());

			TMsRange<f32> yawRange(0.0f, 36000.0f);
			s32 yaw = yawRange.rand();

			JGeometry::TVec3<f32> goal(self->getUnk104().getPoint());

			if (self->calcDist(self->unk104.getPoint(), self->mPosition)
			    > self->getSaveLoadParam()->getSLLimitMove()) {
				goal.set(SMS_GetMarioPos().x - self->mPosition.x, 0.0f,
				         SMS_GetMarioPos().z - self->mPosition.z);
				if (goal.x == 0.0f && goal.y == 0.0f && goal.z == 0.0f)
					goal.x += 1.0f;
				MsVECNormalize(goal, goal);
				f32 dist = self->getSaveLoadParam()->getSLMoveDist();
				goal.x   = goal.x * dist + self->mPosition.x;
				goal.z   = goal.z * dist + self->mPosition.z;
				self->onShootCurve(goal);
			} else {
				f32 circle = self->getSaveLoadParam()->getSLMarioCircle();
				goal.x += circle * JMASCos(yaw);
				goal.z += circle * JMASSin(yaw);
				self->onShootCurve(goal);
			}
			return TRUE;
		}
	}

	self->walkToCurPathNode(0.0f, self->getTurnSpeed(), 0.0f);

	if (self->mIsStay != 0 && !self->isFindMario(1.0f)) {
		f32 giveUp = self->getSaveParams()->getSLGiveUpLength();
		if (self->calcDist(self->unk104.getPoint(), self->mPosition) > giveUp) {
			spine->pushAfterCurrent(&TNerveStayPakkunHide::theNerve());
			return TRUE;
		}
	}
	return FALSE;
}

DEFINE_NERVE(TNervePakkunAppear, TLiveActor)
{
	TPakkun* self = (TPakkun*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(PAKKUN_ANM_SET);
		self->offHitFlag(HIT_FLAG_NO_COLLISION);
	}

	// TODO: the original compares this result and throws it away (a dead
	// `cmpwi r3, 0` with no consumer).  The construct is now known -- see
	// TCoasterKillerManager::load and TKoopaJr::init: a discarded call to a
	// value-returning inline whose *first* statement is a guard returning a
	// side-effect-free expression, e.g.
	//     if (getMActor()->getFrameCtrl(0)->checkPass(100.0f)) return a;
	//     return b;
	// Throwing the result away kills everything but the guard's compare,
	// whose branch then lands on the fall-through and is dropped; a probe of
	// that shape takes this function to 100% at zero frame cost.  What is
	// missing is the predicate's name and its two arms, so it is not written
	// here.  Ruled out: an empty `if` (nested or with `&&`), an unused
	// `bool`, an explicit cast, a `bool`-returning inline that just forwards
	// checkPass, and an empty inline taking a `bool` argument -- MWCC deletes
	// the compare with the value in all of them.
	self->getMActor()->getFrameCtrl(0)->checkPass(100.0f);

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNervePakkunStay::theNerve());
		return TRUE;
	}
	return FALSE;
}

// Ducks back into the ground; once the seed is gone the plant follows it to
// wherever it landed and pops back up there.
DEFINE_NERVE(TNervePakkunHide, TLiveActor)
{
	TPakkun* self = PakkunBody(spine);

	if (spine->getTime() == 0)
		self->setBckAnm(PAKKUN_ANM_HIDE);

	if (self->checkCurAnmEnd(0)) {
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
		self->onLiveFlag(LIVE_FLAG_HIDDEN);
	}

	TPakkunSeed* seed = self->mSeed;
	if (seed->isUnk150Zero()) {
		self->mPosition   = seed->getPosition();
		self->mPosition.y = PakkunSeedB(self)->mGroundHeight;
		spine->pushAfterCurrent(&TNervePakkunAppear::theNerve());
		self->setBckAnm(PAKKUN_ANM_SET);
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNervePakkunShoot, TLiveActor)
{
	TPakkun* self = (TPakkun*)spine->getBody();

	if (spine->getTime() == 0)
		self->setBckAnm(PAKKUN_ANM_SHOOT);

	if (self->getMActor()->getFrameCtrl(0)->checkPass(60.0f))
		self->shootIn();

	if (self->getMActor()->getFrameCtrl(0)->checkPass(70.0f))
		self->shoot();

	self->walkToCurPathNode(0.0f, self->getTurnSpeed(), 0.0f);

	if (self->checkCurAnmEnd(0)) {
		self->setGoalPathMario();
		return TRUE;
	}
	return FALSE;
}

// The water reaction: animation 6 leads into 4 while the spray keeps coming,
// and 5 finishes it off once it stops.
DEFINE_NERVE(TNervePakkunFreeze, TLiveActor)
{
	TPakkun* self = (TPakkun*)spine->getBody();

	if (spine->getTime() == 0)
		self->setBckAnm(PAKKUN_ANM_UNK6);

	if (self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(PAKKUN_ANM_UNK6)) {
			if (self->unsetUnk165())
				self->setBckAnm(PAKKUN_ANM_UNK4);
		} else if (self->unsetUnk165()) {
			self->setBckAnm(PAKKUN_ANM_UNK4);
		} else if (self->isBckAnm(PAKKUN_ANM_UNK4)) {
			self->setBckAnm(PAKKUN_ANM_UNK5);
		} else {
			return TRUE;
		}
	}
	return FALSE;
}

// The hand-placed variant crawls along its path while it is hidden and comes
// back up once the ground it is standing on is dirty again.
DEFINE_NERVE(TNerveStayPakkunHide, TLiveActor)
{
	TStayPakkun* self = (TStayPakkun*)spine->getBody();

	if (spine->getTime() == 0) {
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
		if (!self->isBckAnm(PAKKUN_ANM_CRUSH_TO_HIDE))
			self->setBckAnm(PAKKUN_ANM_HIDE);
		self->mSeed->kill();
	} else {
		if (self->mDamageHiding != 0
		    && spine->getTime()
		           > self->getSaveLoadParam()->getSLDamageHideTime())
			self->mDamageHiding = 0;

		if (self->checkCurAnmEnd(0)) {
			self->onLiveFlag(LIVE_FLAG_HIDDEN);
			if (self->mDamageHiding == 0
			    && gpPollution->isPolluted(self->mPosition.x,
			                               self->mPosition.y,
			                               self->mPosition.z)
			    && self->isFindMario(0.9f)) {
				self->mHitPoints = self->getMaxHitPoints();
				spine->pushAfterCurrent(
				    &TNerveStayPakkunAppear::theNerve());
				return TRUE;
			}
		}
	}

	f32 frame = self->getCurAnmFrameNo(0);
	if (frame > 47.0f && frame < 80.0f
	    && !self->checkLiveFlag(LIVE_FLAG_HIDDEN | LIVE_FLAG_CLIPPED_OUT)) {
		if (!gpPollution->isPolluted(self->mPosition.x, self->mPosition.y,
		                             self->mPosition.z)) {
			// TODO: name 0x13E/0x13F in System/Particles.hpp.
			gpMarioParticleManager->emit(0x13E, &self->mPosition, 1, self);
			gpMarioParticleManager->emit(0x13F, &self->mPosition, 1, self);
		} else {
			JPABaseEmitter* emitter = gpMarioParticleManager->emit(
			    PARTICLE_MS_GENE_HIT, &self->mPosition, 1, self);
			if (emitter)
				SMSSetEmitterPolColor(emitter, 6);
		}
	}

	self->walkToCurPathNode(0.0f, 3.0f * self->mTurnSpeed, 0.0f);
	return FALSE;
}

DEFINE_NERVE(TNerveStayPakkunAppear, TLiveActor)
{
	TStayPakkun* self = (TStayPakkun*)spine->getBody();

	if (spine->getTime() == 0) {
		self->offLiveFlag(LIVE_FLAG_HIDDEN);
		self->setBckAnm(PAKKUN_ANM_SET);
		self->offHitFlag(HIT_FLAG_NO_COLLISION);
		self->mIsHoldingSeed = 1;
		self->mSeed->kill();
	}

	f32 frame = self->getCurAnmFrameNo(0);
	if (frame > 0.0f && frame < 25.0f
	    && !self->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)
	    && gpPollution->isPolluted(self->mPosition.x, self->mPosition.y,
	                               self->mPosition.z)) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emit(
		    PARTICLE_MS_GENE_HIT, &self->mPosition, 1, self);
		if (emitter) {
			JGeometry::TVec3<f32> scale(1.5f, 1.5f, 1.5f);
			emitter->setGlobalScale(scale);
			SMSSetEmitterPolColor(emitter, 6);
		}
	}

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNervePakkunStay::theNerve());
		return TRUE;
	}
	return FALSE;
}
