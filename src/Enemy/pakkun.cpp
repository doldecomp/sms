#include <Enemy/Pakkun.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Camera/Camera.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/PollutionManager.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/WaterGun.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

f32 TPakkunManager::mRootExplosionScaleRate = 2.0f;
f32 TPakkunManager::mTestFlyAngX            = 30.0f;
f32 TPakkunManager::mIgnoreHitWaterY        = 50.0f;

static TPakkun* gpCurPakkun;
static TPakkunSeed* gpCurPakkunSeed;
u8 TPakkun::mHeadJntIndex;

static int PakkunSeedCallback(J3DNode* node, int type)
{
	if (type == 0) {
		if (gpCurPakkunSeed == nullptr || gpCurPakkunSeed->unk168
		    || gpCurPakkunSeed->unk16C->unk1B1) {
			return true;
		}

		J3DJoint* joint = (J3DJoint*)node;
		MtxPtr anmMtx   = gpCurPakkunSeed->getMActor()->getModel()->getAnmMtx(
            joint->getJntNo());

		Mtx rotation;
		MsMtxSetRotZ(rotation, gpCurPakkunSeed->unk170);

		MTXConcat(anmMtx, rotation, anmMtx);
		MTXConcat(J3DSys::mCurrentMtx, rotation, J3DSys::mCurrentMtx);
	}

	return true;
}
static int PakkunRootCallback(J3DNode* node, int type)
{
	if (type == 0) {
		TPakkun* pakkun = gpCurPakkun;
		if (pakkun != nullptr) {
			u8 maxHitPoints = pakkun->getSaveParam()
			                      ? pakkun->getSaveParam()->mSLHitPointMax.get()
			                      : 1;
			if (gpCurPakkun->mHitPoints == maxHitPoints) {
				return true;
			}

			J3DJoint* joint = (J3DJoint*)node;
			MtxPtr anmMtx   = gpCurPakkun->getMActor()->getModel()->getAnmMtx(
                joint->getJntNo());

			TRotation3f scaling;
			scaling.ref(0, 3) = 0.0f;
			scaling.ref(1, 3) = 0.0f;
			scaling.ref(2, 3) = 0.0f;

			f32 scale  = gpCurPakkun->unk1B8;
			f32 scaleY = scale;
			f32 scaleZ = scale;
			if (scale > 1.0f) {
				scaleY *= 1.5f;
				scaleZ *= 1.5f;
			}

			scaling.setScale(scale, scaleY, scaleZ);
			MTXConcat(scaling, anmMtx, anmMtx);

			scaling.setScale(scale, scale, scale);
			MTXConcat(J3DSys::mCurrentMtx, scaling, J3DSys::mCurrentMtx);
		}
	}

	return true;
}

static int PakkunRootCallback2(J3DNode* node, int type)
{
	if (type == 0) {
		if (gpCurPakkun == nullptr) {
			return true;
		}

		MtxPtr anmMtx = gpCurPakkun->getMActor()->getModel()->getAnmMtx(
		    ((J3DJoint*)node)->getJntNo());

		TRotation3f scaling;
		scaling.ref(0, 3) = 0.0f;
		scaling.ref(1, 3) = 0.0f;
		scaling.ref(2, 3) = 0.0f;

		f32 scale = 1.0f / gpCurPakkun->unk1B8;
		scaling.setScale(scale, scale, scale);
		MTXConcat(anmMtx, scaling, anmMtx);
		MTXConcat(J3DSys::mCurrentMtx, scaling, J3DSys::mCurrentMtx);
	}

	return true;
}

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

// UNUSED
TPakkunSaveLoadParams::TPakkunSaveLoadParams(const char* path)
    : TSmallEnemyParams(path)
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
    , unk64(nullptr)
    , unk68(nullptr)
{
	gpCurPakkun     = nullptr;
	gpCurPakkunSeed = nullptr;
	unk5C           = 0;
}

void TPakkunManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TPakkunSaveLoadParams("/enemy/pakkun.prm");
	unk60 = new TPakkunSaveLoadParams("/enemy/staypakkun.prm");
	unk64 = new TWaterEmitInfo("/enemy/pakkunwater.prm");
	unk68 = new TWaterEmitInfo("/enemy/pakkunhide.prm");
}

void TPakkunManager::loadAfter() { TSmallEnemyManager::loadAfter(); }

void TPakkunManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "pakun.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (16 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TSmallEnemy* TPakkunManager::createEnemyInstance() { return new TPakkun(); }

void TPakkunManager::clipEnemies(JDrama::TGraphics* graphics)
{
	char trash[0x10];
	// Possibly an inline?
	f32 radius;
	f32 farClip;
	if (unk38 == nullptr) {
		farClip = gpConductor->getCondParams().mEnemyFarClip.get();
		radius  = 300.0f;
	} else {
		farClip = unk38->mSLFarClip.get();
		radius  = unk38->mSLClipRadius.get();
	}

	SetViewFrustumClipCheckPerspective(gpCamera->getFovy(),
	                                   gpCamera->getAspect(),
	                                   graphics->getNearPlane(), farClip);

	for (int i = 0; i < mObjNum; ++i) {
		TPakkun* pakkun = (TPakkun*)unk18[i];
		if (pakkun->unk199) {
			pakkun->updateSquareToMario();
			if (pakkun->getDistToMarioSquared() < farClip * farClip)
				continue;
		}

		if (ViewFrustumClipCheck(graphics, &pakkun->mPosition, radius)) {
			pakkun->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		} else {
			pakkun->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		}

		if (!pakkun->unk194->isState(PAKKUN_SEED_STATE_HIDE)) {
			if (ViewFrustumClipCheck(graphics, &pakkun->unk194->mPosition,
			                         radius))
				pakkun->unk194->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
			else
				pakkun->unk194->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		}

		if (pakkun->unk199) {
			for (int j = 0; j < 2; ++j) {
				if (!pakkun->unk19C[j]->isState(PAKKUN_SEED_STATE_HIDE)) {
					if (ViewFrustumClipCheck(
					        graphics, &pakkun->unk19C[j]->mPosition, radius))
						pakkun->unk19C[j]->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
					else
						pakkun->unk19C[j]->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
				}
			}
		}
	}
}

TPakkun::TPakkun(const char* name)
    : TSmallEnemy(name)
    , unk194(nullptr)
    , unk198(0)
    , unk199(0)
    , unk19C(nullptr)
    , unk1B8(1.0f)
    , unk1BC(0)
{
}

void TPakkun::load(JSUMemoryInputStream& stream)
{
	TSmallEnemy::load(stream);
	reset();
	TPathNode marioNode((THitActor*)gpMarioAddress);
	setGoalPath(marioNode);
}

void TPakkun::init(TLiveManager* manager)
{
	TSmallEnemy::init(manager);
	mActorType = ACTOR_TYPE_ENEMY | 4;
	unk150     = 17;
	unk1A0     = (TPakkunSaveLoadParams*)getSaveParam();
	mSpine->initWith(&TNervePakkunGenerate::theNerve());

	TPathNode marioNode((THitActor*)gpMarioAddress);
	setGoalPath(marioNode);

	if (mInstanceIndex == 0) {
		for (u8 i = 0; i < getModel()->getModelData()->getJointNum(); ++i) {
			const char* name
			    = getModel()->getModelData()->getJointName()->getName(i);
			if (strcmp(name, "null_seed") == 0)
				mHeadJntIndex = i;
		}
	}

	unk194 = new TPakkunSeed;
	unk194->loadInit(this, "seed.bmd");
	unk194->unk164 = mBodyScale;

	ResTIMG* mask = (ResTIMG*)JKRFileLoader::getGlbResource(
	    "/scene/map/pollution/H_ma_rak.bti");
	if (mask) {
		SMS_ChangeTextureAll(mMActor->getModel()->getModelData(),
		                     "H_ma_rak_dummy", *mask);
	}

	for (u16 i = 0; i < mMActor->getModel()->getModelData()->getMaterialNum();
	     ++i) {
		SMS_InitPacket_OneTevKColor(mMActor->getModel(), i, GX_KCOLOR0,
		                            &unk1B2);
	}

	mMActor->setJointCallback(1, PakkunRootCallback);
	mMActor->setJointCallback(2, PakkunRootCallback2);
}

BOOL TPakkun::isInhibitedForceMove() { return TRUE; }

void TPakkun::kill()
{
	TSmallEnemy::kill();
	unk194->kill();
}

void TPakkun::setWaitAnm() { setBckAnm(PAKKUN_ANM_WAIT); }

void TPakkun::setFreezeAnm() { setBckAnm(PAKKUN_ANM_DAMAGE); }

void TPakkun::setDeadAnm()
{
	if (!checkLiveFlag(LIVE_FLAG_HIDDEN)) {
		setBckAnm(PAKKUN_ANM_DOWN);
	}
}

// UNUSED
bool TPakkun::isHideEnd() const { return false; }

void TPakkun::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!checkLiveFlag(LIVE_FLAG_UNK200)) {
		unk194->perform(cue, graphics);
		if (!checkLiveFlag(LIVE_FLAG_DEAD)) {
			if (cue & CUE_MOVE) {
				if (unk199)
					offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
				if (!gpMap->isInArea(mPosition.x, mPosition.z))
					kill();
				moveObject();
			}

			if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
				if (cue & CUE_CALC_ANIM) {
					mMActor->frameUpdate();
				}
			} else {
				if (cue & CUE_CALC_ANIM) {
					calcRootMatrix();
					updateAnmSound();
					mMActor->calcAnm();
				}
				if (checkLiveFlag(LIVE_FLAG_HIDDEN)) {
					return;
				}
				if (cue & CUE_CALC_VIEW) {
					mMActor->viewCalc();
				}
				if (cue & CUE_ENTRY) {
					drawObject(graphics);
				}
			}
		}
	}
}

// UNUSED
// TODO: Reconstruct the original 0x98-byte body.
void TPakkun::createPakkunSmoke(JGeometry::TVec3<f32>&) { }

// UNUSED
void TPakkun::seedPollute(JGeometry::TVec3<f32>& position)
{
	TSmallEnemyManager* manager = (TSmallEnemyManager*)getManager();
	gpPollution->stamp(manager->getUnk58(), position.x, position.y, position.z,
	                   32.0f * manager->getSaveParam2()->getSLStampRange()
	                       * unk158);
}

void TPakkun::onShootLiner(JGeometry::TVec3<f32>& direction)
{
	JGeometry::TVec3<f32> goal = mPosition;
	goal.x += 200.0f * direction.x;
	goal.z += 200.0f * direction.z;
	setGoalPath(TPathNode(goal));
	unk198 = 0;

	if (direction.x == 0.0f && direction.y == 0.0f && direction.z == 0.0f)
		direction.x += 1.0f;

	MsVECNormalize(&direction, &direction);
	f32 speed = unk1A0->mSLSeedSpeedS.get();
	direction.x *= speed;
	direction.y = -5.0f;
	direction.z *= speed;
	unk194->setVelocity(direction);
}

// UNUSED
void TPakkun::onShootCurve(JGeometry::TVec3<f32>& goal)
{
	setGoalPath(TPathNode(goal));
	f32 speed   = unk1A0->mSLSeedSpeedC.get();
	f32 gravity = unk1A0->mSLSeedGravityC.get();
	JGeometry::TVec3<f32> velocity
	    = calcVelocityToJumpToY(goal, speed, gravity);
	unk198 = 1;
	unk194->setVelocity(velocity);
	unk194->mRotation.set(TPakkunManager::mTestFlyAngX, 0.0f, 0.0f);
}

void TPakkun::behaveToWater(THitActor* hit_actor)
{
	mSprayedByWaterCooldown = 0;
	if (hit_actor->getPosition().y
	    > mGroundHeight + TPakkunManager::mIgnoreHitWaterY) {
		unk165 = true;
		if (mHitPoints > 5) {
			mHitPoints -= 4;
		}

		if (mSpine->getCurrentNerve() != &TNervePakkunFreeze::theNerve()
		    && mSpine->getCurrentNerve() != &TNerveStayPakkunAppear::theNerve()
		    && mSpine->getCurrentNerve() != &TNerveStayPakkunHide::theNerve()) {
			mSpine->pushNerve(&TNervePakkunFreeze::theNerve());
			if (unk194->isState(PAKKUN_SEED_STATE_HIDE)) {
				unk194->kill();
			}
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
	unk1B1   = 1;
	unk1B2.a = 0;
	unk1B8   = 1.0f;
}

void TPakkun::shootIn()
{
	unk194->appear();
	unk194->set();
}

void TPakkun::shoot() { unk194->shoot(); }

const char** TPakkun::getBasNameTable() const { return pakkun_bastable; }

// UNUSED
TPakkunSeed::TPakkunSeed(const char* name)
    : TEnemyAttachment(name)
    , unk16C(nullptr)
    , unk170(0.0f)
{
}

void TPakkunSeed::loadInit(TSpineEnemy* host, const char* model_name)
{
	unk160        = host;
	mMActorKeeper = new TMActorKeeper(unk160->getManager(), 1);
	mMActorKeeper->mModelLoaderFlags = J3DMLF_MaterialPEFull
	                                   | J3DMLF_UseUniqueMaterials
	                                   | (2 << J3DMLF_TevStageNumShift);
	mMActor = mMActorKeeper->createMActor(model_name, 3);
	unk16C  = (TPakkun*)unk160;

	TIdxGroupObj* group = static_cast<TIdxGroupObj*>(
	    JDrama::TNameRefGen::search("オブジェクトグループ"));
	group->getChildren().push_back(this);

	THitActor::initHitActor(ACTOR_TYPE_ENEMY | 6, 1, ACTOR_TYPE_PLAYER, 20.0f,
	                        20.0f, 20.0f, 20.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	unk150       = 0;
	mGroundPlane = TMap::getIllegalCheckData();
	mMActor->getModel()->getModelData()->getJointNodePointer(0)->setCallBack(
	    PakkunSeedCallback);
}

void TPakkunSeed::moveObject()
{
	TEnemyAttachment::moveObject();

	if (!unk168) {
		unk170 = MsWrap(unk170 + 5.0f, 0.0f, 360.0f);
		if (mPosition.y > mGroundHeight + 20.0f) {
			JGeometry::TVec3<f32> velocity = mVelocity;
			mRotation.x                    = MsGetRotFromZaxis(velocity).x;
		}
	} else {
		unk170 = MsClamp(unk170 + 5.0f, 0.0f, 360.0f);
		mRotation.x *= 0.8f;
	}
}

void TPakkunSeed::behaveToHost()
{
	if (!unk16C->unk199) {
		unk160->offLiveFlag(LIVE_FLAG_HIDDEN);
	}
}

void TPakkunSeed::behaveToHitWall(const TBGCheckData* ground)
{
	f32 reflect = -(1.5f * mVelocity.dot(ground->getNormal()));
	mVelocity.x += reflect * ground->mNormal.x;
	mVelocity.y += reflect * ground->mNormal.y;
	if (unk150 == PAKKUN_SEED_STATE_SHOOT) {
		mVelocity.y = -5.0f;
	}
	mVelocity.z += reflect * ground->mNormal.z;
	unk16C->unk1B0 = 1;
}

void TPakkunSeed::calcRootMatrix()
{
	char trash[8];
	SMSGetMSound()->startSoundActor(MSD_SE_EN_PAKKUN_SEED_FLY, &mPosition, 0,
	                                nullptr, 0, 4);

	TEnemyAttachment::calcRootMatrix();
	gpCurPakkunSeed = this;
}

void TPakkunSeed::behaveToHitGround()
{
	if (fabsf(mVelocity.y) < 1.0f || mGroundPlane->isWaterSurface()) {
		unk168 = 1;
		offLiveFlag(LIVE_FLAG_AIRBORNE);
		mVelocity.set(0.0f, -0.3f, 0.0f);
		rebirth();
	} else if (unk150 == PAKKUN_SEED_STATE_SHOOT) {
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
	    = (TPakkunSaveLoadParams*)unk16C->getSaveParam();
	f32 gravity = params->mSLSeedGravityS.get();
	if (unk150 == PAKKUN_SEED_STATE_CIRCLE) {
		gravity = params->mSLSeedGravityC.get();
	}
	return gravity;
}

void TPakkunSeed::appear()
{
	unk150         = PAKKUN_SEED_STATE_APPEAR;
	unk16C->unk1B0 = 0;
	mScaling.x = mScaling.y = mScaling.z = 0.5f;
}

void TPakkunSeed::shoot()
{
	switch (unk16C->unk198) {
	case 0:
		unk150 = PAKKUN_SEED_STATE_SHOOT;
		break;
	case 1:
		unk150 = PAKKUN_SEED_STATE_CIRCLE;
		break;
	}

	mScaling.x = 0.1f;
	mScaling.y = 0.1f;
	mScaling.z = 0.1f;
	unk168     = 0;
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

void TPakkunSeed::rebirth()
{
	if (unk16C->unk199) {
		unk150 = 0;
		unk158 = 0;
		onHitFlag(HIT_FLAG_NO_COLLISION);

		unk16C->seedPollute(mPosition);

		SMSGetMSound()->startSoundActor(MSD_SE_EN_PAKKUN_SEED_SINK, &mPosition,
		                                0, nullptr, 0, 4);
		return;
	}

	++unk158;
	if (unk158 > unk16C->unk1A0->mSLGenerateSeedTime.get() || unk16C->unk1B1) {
		unk150 = 0;
		unk158 = 0;
		onHitFlag(HIT_FLAG_NO_COLLISION);
	}

	if (mPosition.y < mGroundHeight - 70.0f) {
		mVelocity.y = 0.0f;
		onHitFlag(HIT_FLAG_NO_COLLISION);
		return;
	}

	if (!mGroundPlane->isWaterSurface()) {
		TPakkun* owner = unk16C;
		SMSGetMSound()->startSoundActor(MSD_SE_EN_PAKKUN_SEED_SINK, &mPosition,
		                                0, nullptr, 0, 4);

		gpMarioParticleManager->emit(PARTICLE_MS_PACKN_HD_ROCK, &mPosition, 1,
		                             owner->unk194);
		gpMarioParticleManager->emit(PARTICLE_MS_PACKN_HD_SMOKE, &mPosition, 1,
		                             owner->unk194);
		return;
	}

	TEffectColumWater* effect
	    = (TEffectColumWater*)gpConductor->makeOneEnemyAppear(
	        mPosition, "エフェクト水柱マネージャー", 0);
	if (effect)
		effect->generate(mPosition, mScaling);
}

void TPakkunSeed::set()
{
	TEnemyAttachment::set();
	if (unk160->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		mPosition.x = unk160->mPosition.x;
		mPosition.y = unk160->mPosition.y + 80.0f;
		mPosition.z = unk160->mPosition.z;
		return;
	}

	MtxPtr mtx  = unk16C->getModel()->getAnmMtx(TPakkun::mHeadJntIndex);
	mPosition.x = mtx[0][3];
	mPosition.y = mtx[1][3] - 50.0f;
	mPosition.z = mtx[2][3];
}

// UNUSED
void TPakkunSeed::seedSet()
{
	TEnemyAttachment::set();
	mScaling.x = mScaling.y = mScaling.z = unk164;
}

void TPakkunSeed::forceKill()
{
	char trash[8];
	if (!mGroundPlane->isPool() && !mGroundPlane->isIllegalData()
	    && gpMap->isInArea(mPosition.x, mPosition.z))
		return;

	kill();
	if (!unk16C->unk199 && unk160->checkLiveFlag(LIVE_FLAG_HIDDEN)) {
		unk160->kill();
		unk160->onLiveFlag(LIVE_FLAG_UNK20000);
	}
}

void TStayPakkun::load(JSUMemoryInputStream& stream)
{
	TPakkun::load(stream);
	unk199 = 1;
}

void TStayPakkun::init(TLiveManager* manager)
{
	TPakkun::init(manager);
	unk19C = new TPakkunSeed*[2];
	for (int i = 0; i < 2; ++i) {
		unk19C[i] = new TPakkunSeed;
		unk19C[i]->loadInit(this, "seed.bmd");
	}
	mSpine->initWith(&TNerveStayPakkunHide::theNerve());
}

void TStayPakkun::reset()
{
	gpCurPakkun = this;
	TSmallEnemy::reset();
	unk1B1   = 1;
	unk1B2.a = 0;
	unk1B8   = 1.0f;
	unk1B2.a = 255;
	offLiveFlag(LIVE_FLAG_UNK800);
	onLiveFlag(LIVE_FLAG_HIDDEN);
}

void TStayPakkun::setDeadAnm() { setBckAnm(PAKKUN_ANM_DOWN); }

void TStayPakkun::genRandomItem()
{
	TPakkunManager* manager = (TPakkunManager*)mManager;
	unk1A4                  = mPosition;
	unk1A4.y += 100.0f;
	manager->unk64->mPos.value = unk1A4;
	gpModelWaterManager->emitRequest(*manager->unk64);

	s32 maxWater = SMS_GetMarioWaterGun()->getMaxWater();
	if (SMS_GetMarioWaterGun()->getCurrentWater() * 4 < maxWater) {
		gpItemManager->makeObjAppear(mPosition.x, mPosition.y, mPosition.z,
		                             0x20000002, true);
	} else {
		unk18C = 3;
		genEventCoin();
	}

	JPABaseEmitter* emitter = gpMarioParticleManager->emit(
	    PARTICLE_MS_POPO_BOMB_A, &unk1A4, 0, nullptr);
	if (emitter) {
		emitter->setGlobalDynamicsScale(JGeometry::TVec3<f32>(1.5f));
		emitter->setGlobalParticleScale(JGeometry::TVec3<f32>(1.5f));
	}

	emitter = gpMarioParticleManager->emit(PARTICLE_MS_POPO_BOMB_B, &unk1A4, 0,
	                                       nullptr);
	if (emitter) {
		emitter->setGlobalDynamicsScale(JGeometry::TVec3<f32>(1.5f));
		emitter->setGlobalParticleScale(JGeometry::TVec3<f32>(1.5f));
	}
}

void TStayPakkun::calcRootMatrix()
{
	gpCurPakkun = this;
	TSpineEnemy::calcRootMatrix();
}

void TStayPakkun::setBehavior()
{
	if (isBckAnm(PAKKUN_ANM_UNK4))
		--mHitPoints;

	u8 maxHitPoints = getSaveParam() ? getSaveParam()->mSLHitPointMax.get() : 1;
	unk1B2.a        = mHitPoints * 255 / maxHitPoints;
	unk1B8
	    = 1.0f
	      + TPakkunManager::mRootExplosionScaleRate * (255 - unk1B2.a) / 255.0f;

	if (mHitPoints < 2) {
		SMSGetMSound()->startSoundActor(MSD_SE_EN_PAKKUN_RIP, &mPosition, 0,
		                                nullptr, 0, 4);
		mHitPoints = 1;
		kill();
	}
}

bool TStayPakkun::isHitValid(u32 message)
{
	if (message == HIT_MESSAGE_UNKB) {
		onLiveFlag(LIVE_FLAG_DEAD);
		onLiveFlag(LIVE_FLAG_UNK20000);
		return true;
	}

	if (mSpine->getCurrentNerve() != &TNerveStayPakkunAppear::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveStayPakkunHide::theNerve()) {
		mSpine->setNext(&TNerveStayPakkunHide::theNerve());
		unk1BC = 1;

		gpPollution->clean(mPosition.x, mGroundHeight, mPosition.z,
		                   32.0f * getSaveParam()->mSLPolluteRange.get());
		if (unk194->isState(PAKKUN_SEED_STATE_HIDE))
			unk194->kill();
		setBckAnm(PAKKUN_ANM_CRUSH_TO_HIDE);
	}

	return false;
}

void TStayPakkun::shootIn()
{
	unk194->appear();
	unk194->set();

	for (int i = 0; i < 2; ++i) {
		unk19C[i]->appear();
		unk19C[i]->set();

		JGeometry::TVec3<f32> velocity = unk194->getVelocity();
		f32 angle                      = -10.0f;
		if (i != 0) {
			angle *= -1.0f;
		}

		Mtx rotation;
		MsMtxSetRotRPH(rotation, 0.0f, angle, 0.0f);
		MTXMultVec(rotation, &velocity, &velocity);
		unk19C[i]->setVelocity(velocity);
	}
}

void TStayPakkun::kill()
{
	TSmallEnemy::kill();
	unk194->kill();
	for (int i = 0; i < 2; ++i)
		unk19C[i]->kill();
}

void TStayPakkun::shoot() { unk194->shoot(); }

TPakkunSaveLoadParams* TStayPakkun::getSaveParam() const
{
	return ((TPakkunManager*)mManager)->unk60;
}

DEFINE_NERVE(TNervePakkunGenerate, TLiveActor)
{
	TPakkun* self = (TPakkun*)spine->getBody();
	if (spine->getTime() == 0) {
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
		self->onLiveFlag(LIVE_FLAG_HIDDEN);
		self->unk194->appear();
	}

	if (self->mHolder != nullptr)
		return false;

	TPakkunSeed* seed = self->unk194;

	if (seed->isState(PAKKUN_SEED_STATE_APPEAR)) {
		seed->seedSet();

		if (spine->getTime() % 5 == 0) {
			self->updateSquareToMario();
			if (self->getDistToMarioSquared()
			    < self->unk1A0->mSLGenerateSeedDist.get()
			          * self->unk1A0->mSLGenerateSeedDist.get())
				self->unk194->unk150 = PAKKUN_SEED_STATE_SET;
		}
	}

	if (self->unk194->isState(PAKKUN_SEED_STATE_HIDE)) {
		self->mPosition   = self->unk194->getPosition();
		self->mPosition.y = self->unk194->getGroundHeight();
		spine->pushAfterCurrent(&TNervePakkunAppear::theNerve());
		self->setBckAnm(PAKKUN_ANM_SET);
		return true;
	}

	return false;
}

DEFINE_NERVE(TNervePakkunStay, TLiveActor)
{
	TPakkun* self = (TPakkun*)spine->getBody();
	if (spine->getTime() == 0)
		self->setWaitAnm();

	s32 waitTime  = ((TSmallEnemyParams*)self->getSaveParam())->getSLWaitTime();
	s32 readyTime = self->unk1A0->mSLReadyTime.get();

	if (self->unk194->isState(PAKKUN_SEED_STATE_HIDE)
	    && self->checkCurAnmEnd(ANM_TYPE_BCK)
	    && (spine->getTime() >= readyTime || spine->getTime() >= waitTime
	        || self->unk1B1)) {
		f32 goalDistance = self->unk104.getPoint().distance(self->mPosition);
		f32 shootRange   = self->unk1A0->mSLShootRange.get();
		f32 distanceRate = 1.0f;
		if (self->unk199) {
			distanceRate = 3.0f;
		}
		JGeometry::TVec3<f32> marioPos = SMS_GetMarioPos();

		if (goalDistance < shootRange * distanceRate || self->unk199) {
			if (fabsf(marioPos.y - self->mPosition.y)
			        < ((TSmallEnemyParams*)self->getSaveParam())
			                  ->getSLSearchHeight()
			              * distanceRate
			    && self->isInSight(marioPos,
			                       ((TSmallEnemyParams*)self->getSaveParam())
			                               ->getSLSearchLength()
			                           * distanceRate,
			                       ((TSmallEnemyParams*)self->getSaveParam())
			                               ->getSLSearchAngle()
			                           * distanceRate,
			                       ((TSmallEnemyParams*)self->getSaveParam())
			                               ->getSLSearchAware()
			                           * distanceRate)) {
				spine->pushAfterCurrent(&TNervePakkunStay::theNerve());
				spine->pushAfterCurrent(&TNervePakkunShoot::theNerve());
				self->unk1B1 = 0;

				if (!self->unk1B0 || self->unk199) {
					JGeometry::TVec3<f32> direction(
					    SMS_GetMarioPos().x - self->mPosition.x,
					    SMS_GetMarioPos().y - self->mPosition.y,
					    SMS_GetMarioPos().z - self->mPosition.z);
					self->onShootLiner(direction);
				} else {
					self->unk1B0               = 0;
					JGeometry::TVec3<f32> goal = self->unk104.getPoint();
					self->onShootCurve(goal);
				}
				return true;
			}
		} else if (spine->getTime() >= waitTime) {
			spine->pushAfterCurrent(&TNervePakkunHide::theNerve());
			spine->pushAfterCurrent(&TNervePakkunShoot::theNerve());

			int angle                  = (int)MsRandF(0.0f, 36000.0f);
			JGeometry::TVec3<f32> goal = self->unk104.getPoint();
			if (goal.distance(self->mPosition)
			    > self->unk1A0->mSLLimitMove.get()) {
				goal.x = SMS_GetMarioPos().x - self->mPosition.x;
				goal.y = 0.0f;
				goal.z = SMS_GetMarioPos().z - self->mPosition.z;
				if (goal.x == 0.0f && goal.y == 0.0f && goal.z == 0.0f)
					goal.x += 1.0f;
				MsVECNormalize(&goal, &goal);
				f32 moveDistance = self->unk1A0->mSLMoveDist.get();
				goal.x           = goal.x * moveDistance + self->mPosition.x;
				goal.z           = goal.z * moveDistance + self->mPosition.z;

				self->onShootCurve(goal);
			} else {
				f32 circle = self->unk1A0->mSLMarioCircle.get();
				goal.x += circle * JMASCos(angle);
				goal.z += circle * JMASSin(angle);

				self->onShootCurve(goal);
			}
			return true;
		}
	}

	self->walkToCurPathNode(0.0f, self->getTurnSpeed(), 0.0f);
	if (self->unk199 && !self->isFindMario(1.0f)) {
		f32 giveUpLength
		    = ((TSmallEnemyParams*)self->getSaveParam())->getSLGiveUpLength();
		if (self->unk104.getPoint().distance(self->mPosition) > giveUpLength) {
			spine->pushAfterCurrent(&TNerveStayPakkunHide::theNerve());
			return true;
		}
	}

	return false;
}

DEFINE_NERVE(TNervePakkunAppear, TLiveActor)
{
	TPakkun* self = (TPakkun*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setBckAnm(PAKKUN_ANM_SET);
		self->offHitFlag(HIT_FLAG_NO_COLLISION);
	}

	if (self->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(100.0f)) { }
	if (self->checkCurAnmEnd(ANM_TYPE_BCK)) {
		spine->pushAfterCurrent(&TNervePakkunStay::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNervePakkunHide, TLiveActor)
{
	TPakkun* self = (TPakkun*)spine->getBody();
	if (spine->getTime() == 0)
		self->setBckAnm(PAKKUN_ANM_HIDE);

	if (self->checkCurAnmEnd(ANM_TYPE_BCK)) {
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
		self->onLiveFlag(LIVE_FLAG_HIDDEN);
	}

	if (self->unk194->isState(PAKKUN_SEED_STATE_HIDE)) {
		self->mPosition   = self->unk194->getPosition();
		self->mPosition.y = self->unk194->getGroundHeight();
		spine->pushAfterCurrent(&TNervePakkunAppear::theNerve());
		self->setBckAnm(PAKKUN_ANM_SET);
		return true;
	}

	return false;
}

DEFINE_NERVE(TNervePakkunShoot, TLiveActor)
{
	TPakkun* self = (TPakkun*)spine->getBody();
	if (spine->getTime() == 0)
		self->setBckAnm(PAKKUN_ANM_SHOOT);

	if (self->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(60.0f))
		self->shootIn();
	if (self->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(70.0f))
		self->shoot();

	self->walkToCurPathNode(0.0f, self->getTurnSpeed(), 0.0f);
	if (self->checkCurAnmEnd(ANM_TYPE_BCK)) {
		TPathNode marioNode((THitActor*)gpMarioAddress);
		self->setGoalPath(marioNode);
		return true;
	}

	return false;
}

DEFINE_NERVE(TNervePakkunFreeze, TLiveActor)
{
	TPakkun* self = (TPakkun*)spine->getBody();
	if (spine->getTime() == 0)
		self->setBckAnm(PAKKUN_ANM_UNK6);

	if (self->checkCurAnmEnd(ANM_TYPE_BCK)) {
		if (self->isBckAnm(PAKKUN_ANM_UNK6)) {
			if (self->unsetUnk165())
				self->setBckAnm(PAKKUN_ANM_UNK4);
		} else if (self->unsetUnk165()) {
			self->setBckAnm(PAKKUN_ANM_UNK4);
		} else if (self->isBckAnm(PAKKUN_ANM_UNK4)) {
			self->setBckAnm(PAKKUN_ANM_UNK5);
		} else {
			return true;
		}
	}

	return false;
}

DEFINE_NERVE(TNerveStayPakkunHide, TLiveActor)
{
	char trash[16];
	TStayPakkun* self = (TStayPakkun*)spine->getBody();
	if (spine->getTime() == 0) {
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
		if (!self->isBckAnm(PAKKUN_ANM_CRUSH_TO_HIDE)) {
			self->setBckAnm(PAKKUN_ANM_HIDE);
		}
		self->unk194->kill();
	} else {
		if (self->unk1BC
		    && spine->getTime() > self->unk1A0->mSLDamageHideTime.get()) {
			self->unk1BC = 0;
		}

		if (self->checkCurAnmEnd(ANM_TYPE_BCK)) {
			self->onLiveFlag(LIVE_FLAG_HIDDEN);
			if (!self->unk1BC
			    && gpPollution->isPolluted(self->mPosition.x, self->mPosition.y,
			                               self->mPosition.z)
			    && self->isFindMario(0.9f)) {
				self->mHitPoints
				    = self->getSaveParam()
				          ? self->getSaveParam()->mSLHitPointMax.get()
				          : 1;
				spine->pushAfterCurrent(&TNerveStayPakkunAppear::theNerve());
				return true;
			}
		}
	}

	f32 frame = self->getCurAnmFrameNo(ANM_TYPE_BCK);
	if (frame > 47.0f && frame < 80.0f
	    && !self->checkLiveFlag(LIVE_FLAG_HIDDEN | LIVE_FLAG_CLIPPED_OUT)) {
		if (!gpPollution->isPolluted(self->mPosition.x, self->mPosition.y,
		                             self->mPosition.z)) {
			gpMarioParticleManager->emit(PARTICLE_MS_PACKN_HD_ROCK,
			                             &self->mPosition, 1, self);
			gpMarioParticleManager->emit(PARTICLE_MS_PACKN_HD_SMOKE,
			                             &self->mPosition, 1, self);
		} else {
			JPABaseEmitter* emitter = gpMarioParticleManager->emit(
			    PARTICLE_MS_GENE_HIT, &self->mPosition, 1, self);
			if (emitter) {
				SMSSetEmitterPolColor(emitter, 6);
			}
		}
	}

	self->walkToCurPathNode(0.0f, self->mTurnSpeed * 3.0f, 0.0f);
	return false;
}

DEFINE_NERVE(TNerveStayPakkunAppear, TLiveActor)
{
	TStayPakkun* self = (TStayPakkun*)spine->getBody();
	if (spine->getTime() == 0) {
		self->offLiveFlag(LIVE_FLAG_HIDDEN);
		self->setBckAnm(PAKKUN_ANM_SET);
		self->offHitFlag(HIT_FLAG_NO_COLLISION);
		self->unk1B1 = 1;
		self->unk194->kill();
	}

	f32 frame = self->getCurAnmFrameNo(ANM_TYPE_BCK);
	if (frame > 0.0f && frame < 25.0f
	    && !self->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)
	    && gpPollution->isPolluted(self->mPosition.x, self->mPosition.y,
	                               self->mPosition.z)) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emit(
		    PARTICLE_MS_GENE_HIT, &self->mPosition, 1, self);
		if (emitter) {
			JGeometry::TVec3<f32> scale(1.5f);
			emitter->setGlobalDynamicsScale(scale);
			emitter->setGlobalParticleScale(scale);
			SMSSetEmitterPolColor(emitter, 6);
		}
	}

	if (self->checkCurAnmEnd(ANM_TYPE_BCK)) {
		spine->pushAfterCurrent(&TNervePakkunStay::theNerve());
		return true;
	}

	return false;
}
