#include <Enemy/AmiNoko.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSound.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionData.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <Player/MarioAccess.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TAmiNokoSaveLoadParams::TAmiNokoSaveLoadParams(const char* path)
    : TWalkerEnemyParams(path)
    , PARAM_INIT(mSLElecRange, 200.0f)
    , PARAM_INIT(mSLMtxRotSpeed, 0.05f)
{
	TParams::load(mPrmPath);
}

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
		{ "aminoko_model1.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (2 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TSpineEnemy* TAmiNokoManager::createEnemyInstance() { return nullptr; }

TAmiHit::TAmiHit(TAmiNoko* param_1, const char* param_2)
    : THitActor(param_2)
    , unk68(param_1)
{
	static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"))
	    ->getChildren()
	    .push_back(this);
	initHitActor(ACTOR_TYPE_AMI_NOKO, 1, HIT_CATEGORY_PLAYER, 120.0f, 240.0f,
	             120.0f, 240.0f);
	offHitFilter(HIT_FILTER_NO_COLLISION);
}

BOOL TAmiHit::receiveMessage(THitActor* param_1, u32 param_2)
{
	return unk68->receiveMessage(param_1, param_2);
}

void TAmiHit::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & CUE_MOVE) {
		JGeometry::TVec3<f32> vec = unk68->unk19C;
		vec.normalize();
		vec.scale(100.0f);

		mPosition = unk68->mPosition;
		mPosition += vec;
		mPosition.y -= 0.5f * getAttackHeight();

		if (!unk68->checkLiveFlag(LIVE_FLAG_DEAD)) {
			for (int i = 0; i < getColNum(); ++i) {
				if (getCollision(i)->getActorType() == ACTOR_TYPE_MARIO)
					unk68->attackToMario();
			}
		}
	}
	THitActor::perform(param_1, param_2);
}

TAmiNoko::TAmiNoko(const char* name)
    : TWalkerEnemy(name)
    , unk194(nullptr)
    , unk198(PLANE_UNK0)
    , unk210(1)
{
	unk19C.set(0.0f, 1.0f, 0.0f);
	unk1A8.set(0.0f, 0.0f, 1.0f);
	unk1B4 = unk19C;
	unk1C0 = unk1A8;
}

void TAmiNoko::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);
	stream >> mCoinId;
}

void TAmiNoko::init(TLiveManager* param_1)
{
	TWalkerEnemy::init(param_1);
	mActorType = ACTOR_TYPE_AMI_NOKO;
	unk150     = 0x11;
	mSpine->initWith(&TNerveAmiNokoWalkOnFence::theNerve());
	unk20C = (TAmiNokoSaveLoadParams*)getSaveParam();
	reset();
	setWalkAnm();
	onLiveFlag(LIVE_FLAG_UNK10);
	initialGraphNode();
	if (SMSGetMarDirector()->getCurrentMap() == 8)
		unk210 = 0;
#ifdef VERSION_GMSP01
	unkE8 = 0;
#endif
	unk208 = new TAmiHit(this);
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

void TAmiNoko::attackToMario()
{
	if (mSpine->getCurrentNerve() == &TNerveSmallEnemyChange::theNerve())
		return;
	if (unk194 == nullptr)
		return;

	BOOL check = TRUE;
	switch (unk198) {
	case PLANE_UNK0: {
		const TBGCheckData* plane = SMS_GetMarioRfPlane();
		if (plane != nullptr && SMS_GetMarioPos().y < mPosition.y)
			check = FALSE;
		break;
	}
	case PLANE_UNK1: {
		const TBGCheckData* plane = SMS_GetMarioGrPlane();
		if (plane != nullptr && 5.0f + SMS_GetMarioPos().y > mPosition.y)
			check = FALSE;
		break;
	}
	case PLANE_UNK2: {
		const TBGCheckData* plane = SMS_GetMarioWlPlane();
		if (plane != nullptr
		    && plane->getNormal().dot(unk194->getNormal()) < 0.0f)
			check = FALSE;
		break;
	}
	}

	if (check && SMS_SendMessageToMario(this, HIT_MESSAGE_ELECTRIC_SHOCK)) {
		if (mSpine->getCurrentNerve() != &TNerveAmiNokoFreeze::theNerve())
			mSpine->pushNerve(&TNerveAmiNokoFreeze::theNerve());
	}
}

void TAmiNoko::setWalkAnm()
{
	if (unk210)
		setBckAnm(5);
	else
		setBckAnm(8);
}

// TODO: frame is 0x58 instead of 0x60; nerve-static relocations also differ.
bool TAmiNoko::isHitValid(u32 param_1)
{
	if (param_1 == HIT_MESSAGE_PUNCH || param_1 == HIT_MESSAGE_HIP_DROP) {
		f32 x = mPosition.x - SMS_GetMarioPos().x;
		f32 z = mPosition.z - SMS_GetMarioPos().z;

		JGeometry::TVec3<f32> diff(x, 0.0f, z);
		f32 normalZ = unk19C.z;
		f32 normalX = unk19C.x;
		matan(normalZ, normalX);

		if (diff.dot(unk19C) > 0.0f || param_1 == HIT_MESSAGE_HIP_DROP)
			mSpine->pushNerve(&TNerveAmiNokoDie::theNerve());
	}

	if (param_1 == HIT_MESSAGE_UNKB)
		return true;
	return false;
}

#ifdef VERSION_GMSP01
void TAmiNoko::calcDirection()
{
	JGeometry::TVec3<f32> pos = unkF4.getPoint();
	pos -= mPosition;
	if (pos.isZero())
		pos.set(1.0f, 0.0f, 0.0f);
	else
		pos.normalize();

	TBGWallCheckRecord record(mPosition.x, mPosition.y, mPosition.z, 10.0f, 4,
	                          0);
	int wallCount = gpMap->isTouchedWallsAndMoveXZ(&record);

	f32 minDist = -1.0f;
	int bestIdx = -1;
	const TBGCheckData* wall;
	for (int i = 0; i < wallCount; ++i) {
		wall     = record.mResultWalls[i];
		f32 dist = fabsf(record.mResultWalls[i]->getNormal().dot(mPosition)
		                 + record.mResultWalls[i]->getPlaneDistance());
		if (bestIdx < 0 || minDist > dist || minDist < 0.0f) {
			minDist = dist;
			unk198  = PLANE_UNK2;
			bestIdx = i;
		}
	}
	const TBGCheckData* bestPlane = record.mResultWalls[bestIdx];

	gpMap->checkGround(mPosition.x, mPosition.y + getHeadHeight(), mPosition.z,
	                   &wall);
	mGroundPlane = wall;
	if (mGroundPlane != nullptr) {
		wall     = mGroundPlane;
		f32 dist = wall->getNormal().dot(mPosition) + wall->getPlaneDistance();
		if (dist >= 0.0f && (minDist > dist || minDist < 0.0f)) {
			minDist   = dist;
			bestPlane = wall;
			unk198    = PLANE_UNK0;
		}
	}

	gpMap->checkRoof(mPosition, &wall);
	if (wall != nullptr) {
		f32 dist = wall->getNormal().dot(mPosition) + wall->getPlaneDistance();
		if (dist >= 0.0f && (minDist > dist || minDist < 0.0f)) {
			bestPlane = wall;
			unk198    = PLANE_UNK1;
		}
	}
	if (bestPlane != nullptr)
		unk194 = bestPlane;

	f32 step = ((TAmiNokoSaveLoadParams*)getSaveParam())->mSLMtxRotSpeed.get();

	JGeometry::TVec3<f32> normal;
	if (unk194 != nullptr)
		normal = unk194->getNormal();
	else
		normal.set(0.0f, 1.0f, 0.0f);

	if (normal.dot(unk19C) <= -1.0f) {
		unk19C = normal;
	} else {
		unk19C.x = unk19C.x < normal.x ? MsMin(unk19C.x + step, normal.x)
		                               : MsMax(unk19C.x - step, normal.x);
		unk19C.y = unk19C.y < normal.y ? MsMin(unk19C.y + step, normal.y)
		                               : MsMax(unk19C.y - step, normal.y);
		unk19C.z = unk19C.z < normal.z ? MsMin(unk19C.z + step, normal.z)
		                               : MsMax(unk19C.z - step, normal.z);
		VECNormalize(&unk19C, &unk19C);
	}

	if (unk1A8.dot(pos) < -0.1f) {
		Mtx rot;
		MTXRotAxisRad(rot, &normal, 1.5707964f);
		MTXMultVec(rot, &pos, &pos);
	}

	unk1A8.x = unk1A8.x < pos.x ? MsMin(unk1A8.x + step, pos.x)
	                            : MsMax(unk1A8.x - step, pos.x);
	unk1A8.y = unk1A8.y < pos.y ? MsMin(unk1A8.y + step, pos.y)
	                            : MsMax(unk1A8.y - step, pos.y);
	unk1A8.z = unk1A8.z < pos.z ? MsMin(unk1A8.z + step, pos.z)
	                            : MsMax(unk1A8.z - step, pos.z);
	VECNormalize(&unk1A8, &unk1A8);

	JGeometry::TVec3<f32> axis;
	axis.cross(unk19C, unk1A8);
	if (!axis.isZero()) {
		unk1B4 = unk19C;
		unk1C0 = unk1A8;
	}
	pos.cross(axis, unk19C);
	if (!pos.isZero())
		unk1A8 = pos;
}

void TAmiNoko::emitEffects()
{
	SMSGetMSound()->startSoundActor(MSD_SE_EN_AMINOKO_SPARK, &mPosition, 0,
	                                nullptr, 0, 4);

	gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_AMN_BIRI, getMActor()->getModel()->getAnmMtx(11), 1, this);
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
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(
		        PARTICLE_MS_DNK_SHIBIRE_A,
		        getMActor()->getModel()->getAnmMtx(0), 1, this))
			emitter->setGlobalScale(JGeometry::TVec3<f32>(2.0f));

		MtxPtr mtx = getMActor()->getModel()->getAnmMtx(6);
		unk1FC.set(mtx[0][3], mtx[1][3], mtx[2][3]);
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToPosPtr(
		        PARTICLE_MS_DNK_HIBANA, &unk1FC, 1, this))
			emitter->setGlobalScale(JGeometry::TVec3<f32>(2.0f));
	}
}

void TAmiNoko::calcRootMatrix()
{
	emitEffects();

	if (isBckAnm(0)) {
		unk1CC[0][3] = mPosition.x;
		unk1CC[1][3] = mPosition.y;
		unk1CC[2][3] = mPosition.z;
		MTXCopy(unk1CC, getModel()->getBaseTRMtx());
		getModel()->setBaseScale(mScaling);
		return;
	}

	MtxPtr rootMtx = getModel()->getBaseTRMtx();

	JGeometry::TVec3<f32> axis;
	JGeometry::TVec3<f32> normal;
	JGeometry::TVec3<f32> pos;
	axis.cross(unk19C, unk1A8);
	if (axis.isZero()) {
		normal = unk1B4;
		pos    = unk1C0;
		axis.cross(normal, pos);
		if (axis.isZero())
			axis.set(1.0f, 0.0f, 0.0f);
	} else {
		normal = unk19C;
		pos    = unk1A8;
	}
	VECNormalize(&axis, &axis);

	rootMtx[0][0] = axis.x;
	rootMtx[1][0] = axis.y;
	rootMtx[2][0] = axis.z;
	rootMtx[0][1] = normal.x;
	rootMtx[1][1] = normal.y;
	rootMtx[2][1] = normal.z;
	rootMtx[0][2] = pos.x;
	rootMtx[1][2] = pos.y;
	rootMtx[2][2] = pos.z;

	rootMtx[0][3] = mPosition.x - 30.0f * unk19C.x;
	rootMtx[1][3] = mPosition.y - 30.0f * unk19C.y;
	rootMtx[2][3] = mPosition.z - 30.0f * unk19C.z;

	getModel()->setBaseScale(mScaling);
	MTXCopy(rootMtx, unk1CC);
}
#else
// TODO: path-node inline, stack frame and nerve-static relocations differ.
void TAmiNoko::calcRootMatrix()
{
	SMSGetMSound()->startSoundActor(MSD_SE_EN_AMINOKO_SPARK, &mPosition, 0,
	                                nullptr, 0, 4);

	gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_AMN_BIRI, getMActor()->getModel()->getAnmMtx(11), 1, this);
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
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(
		        PARTICLE_MS_DNK_SHIBIRE_A,
		        getMActor()->getModel()->getAnmMtx(0), 1, this))
			emitter->setGlobalScale(JGeometry::TVec3<f32>(2.0f));

		MtxPtr mtx = getMActor()->getModel()->getAnmMtx(6);
		unk1FC.set(mtx[0][3], mtx[1][3], mtx[2][3]);
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToPosPtr(
		        PARTICLE_MS_DNK_HIBANA, &unk1FC, 1, this))
			emitter->setGlobalScale(JGeometry::TVec3<f32>(2.0f));
	}

	if (isBckAnm(0)) {
		unk1CC[0][3] = mPosition.x;
		unk1CC[1][3] = mPosition.y;
		unk1CC[2][3] = mPosition.z;
		MTXCopy(unk1CC, getMActor()->getModel()->getBaseTRMtx());
		return;
	}

	MtxPtr rootMtx = getModel()->getBaseTRMtx();

	JGeometry::TVec3<f32> pos = unkF4.getPoint();
	pos -= mPosition;
	if (pos.x == 0.0f && pos.y == 0.0f && pos.z == 0.0f)
		pos.x = 1.0f;
	VECNormalize(&pos, &pos);

	TBGWallCheckRecord record(mPosition.x, mPosition.y, mPosition.z, 10.0f, 4,
	                          0);
	int wallCount = gpMap->isTouchedWallsAndMoveXZ(&record);

	f32 minDist = -1.0f;
	int bestIdx = -1;
	const TBGCheckData* wall;
	for (int i = 0; i < wallCount; ++i) {
		wall     = record.mResultWalls[i];
		f32 dist = fabsf(record.mResultWalls[i]->getNormal().dot(mPosition)
		                 + record.mResultWalls[i]->getPlaneDistance());
		if (bestIdx < 0 || minDist > dist || minDist < 0.0f) {
			minDist = dist;
			unk198  = PLANE_UNK2;
			bestIdx = i;
		}
	}
	const TBGCheckData* bestPlane = record.mResultWalls[bestIdx];

	gpMap->checkGround(mPosition.x, mPosition.y + getHeadHeight(), mPosition.z,
	                   &wall);
	mGroundPlane = wall;
	if (mGroundPlane != nullptr) {
		wall     = mGroundPlane;
		f32 dist = wall->getNormal().dot(mPosition) + wall->getPlaneDistance();
		if (dist >= 0.0f && (minDist > dist || minDist < 0.0f)) {
			minDist   = dist;
			bestPlane = wall;
			unk198    = PLANE_UNK0;
		}
	}

	gpMap->checkRoof(mPosition, &wall);
	if (wall != nullptr) {
		f32 dist = wall->getNormal().dot(mPosition) + wall->getPlaneDistance();
		if (dist >= 0.0f && (minDist > dist || minDist < 0.0f)) {
			bestPlane = wall;
			unk198    = PLANE_UNK1;
		}
	}
	if (bestPlane != nullptr)
		unk194 = bestPlane;

	f32 step = ((TAmiNokoSaveLoadParams*)getSaveParam())->mSLMtxRotSpeed.get();

	JGeometry::TVec3<f32> normal;
	if (unk194 != nullptr)
		normal = unk194->getNormal();
	else
		normal.set(0.0f, 1.0f, 0.0f);

	unk19C.x = unk19C.x < normal.x ? MsMin(unk19C.x + step, normal.x)
	                               : MsMax(unk19C.x - step, normal.x);
	unk19C.y = unk19C.y < normal.y ? MsMin(unk19C.y + step, normal.y)
	                               : MsMax(unk19C.y - step, normal.y);
	unk19C.z = unk19C.z < normal.z ? MsMin(unk19C.z + step, normal.z)
	                               : MsMax(unk19C.z - step, normal.z);

	if (unk1A8.dot(pos) < -0.1f) {
		Mtx rot;
		MTXRotAxisRad(rot, &normal, 1.5707964f);
		MTXMultVec(rot, &pos, &pos);
	}

	unk1A8.x = unk1A8.x < pos.x ? MsMin(unk1A8.x + step, pos.x)
	                            : MsMax(unk1A8.x - step, pos.x);
	unk1A8.y = unk1A8.y < pos.y ? MsMin(unk1A8.y + step, pos.y)
	                            : MsMax(unk1A8.y - step, pos.y);
	unk1A8.z = unk1A8.z < pos.z ? MsMin(unk1A8.z + step, pos.z)
	                            : MsMax(unk1A8.z - step, pos.z);
	VECNormalize(&unk1A8, &unk1A8);

	normal = unk19C;
	pos    = unk1A8;

	JGeometry::TVec3<f32> axis;
	axis.cross(normal, pos);
	if (axis.isZero()) {
		normal = unk1B4;
		pos    = unk1C0;
		axis.cross(normal, pos);
		if (axis.isZero())
			axis.set(1.0f, 0.0f, 0.0f);
	} else {
		unk1B4 = unk19C;
		unk1C0 = unk1A8;
	}
	VECNormalize(&axis, &axis);
	pos.cross(axis, normal);
	if (pos.isZero())
		pos.set(0.0f, 0.0f, 1.0f);
	else
		VECNormalize(&pos, &pos);
	normal.cross(pos, axis);
	if (normal.isZero())
		normal.set(0.0f, 1.0f, 0.0f);
	else
		VECNormalize(&normal, &normal);

	rootMtx[0][0] = axis.x;
	rootMtx[1][0] = axis.y;
	rootMtx[2][0] = axis.z;
	rootMtx[0][1] = normal.x;
	rootMtx[1][1] = normal.y;
	rootMtx[2][1] = normal.z;
	rootMtx[0][2] = pos.x;
	rootMtx[1][2] = pos.y;
	rootMtx[2][2] = pos.z;

	JGeometry::TVec3<f32> offset(0.0f, -200.0f, 0.0f);
	Mtx rot;
	MsMtxSetRotRPH(rot, normal.x, normal.y, normal.z);
	MTXMultVec(rot, &offset, &offset);

	MtxPtr savedMtx = unk1CC;
	rootMtx[0][3]   = mPosition.x - 30.0f * unk19C.x;
	rootMtx[1][3]   = mPosition.y - 30.0f * unk19C.y;
	rootMtx[2][3]   = mPosition.z - 30.0f * unk19C.z;

	MTXCopy(rootMtx, savedMtx);
	getModel()->setBaseScale(mScaling);
}
#endif

// TODO: subtraction temporary is at 0x20 instead of the target's 0x10.
void TAmiNoko::bind()
{
	if (isBckAnm(0)) {
		JGeometry::TVec3<f32> vec = mPosition;
		vec += mPositionDelta;
		vec += mVelocity;
		mVelocity.y -= getGravityY();
		if (mVelocity.y < mVelocityMinY)
			mVelocity.y = mVelocityMinY;
		if (checkLiveFlag(LIVE_FLAG_UNK1000))
			mGroundHeight = gpMap->checkGroundIgnoreWaterSurface(
			    vec.x, vec.y + mHeadHeight, vec.z, &mGroundPlane);
		else
			mGroundHeight = gpMap->checkGround(vec.x, vec.y + mHeadHeight,
			                                   vec.z, &mGroundPlane);
		mGroundHeight += 1.0f;
		if (vec.y <= 0.05f + mGroundHeight) {
			if (mGroundPlane->isIllegalData())
				kill();
			offLiveFlag(LIVE_FLAG_AIRBORNE);
			mVelocity.set(0.0f, 0.0f, 0.0f);
			vec.y = mGroundHeight;
		} else {
			onLiveFlag(LIVE_FLAG_AIRBORNE);
		}
		mPositionDelta = vec - mPosition;
	} else {
		TLiveActor::bind();
	}
}

void TAmiNoko::perform(u32 param_1, JDrama::TGraphics* param_2)
{
#ifdef VERSION_GMSP01
	if (param_1 & CUE_CALC_ANIM)
		calcDirection();
#endif
	TSmallEnemy::perform(param_1, param_2);
	unk208->perform(param_1, param_2);
}

f32 TAmiNoko::getGravityY() const
{
	if (mSpine->getCurrentNerve() == &TNerveAmiNokoDie::theNerve())
		return 0.0f;

	return mGravity;
}

// TODO: non-PAL path-node inline differs.
void TAmiNoko::creepToCurPathNode(f32 param_1)
{
#ifdef VERSION_GMSP01
	JGeometry::TVec3<f32> direction = unkF4.getPoint();
	direction -= mPosition;
	if (!direction.isZero()) {
		f32 speed = MsMin(param_1, direction.length());
		direction.normalize();
		direction *= speed;
		JGeometry::TVec3<f32> velocity = mPositionDelta;
		velocity += direction;
		mPositionDelta = velocity;
	}
#else
	if (isBckAnm(4) || isBckAnm(7) || isBckAnm(10) || isBckAnm(13)) {
		const TPathNode* node           = &unkF4;
		JGeometry::TVec3<f32> direction = node->getPoint();
		direction -= mPosition;
		if (direction.x == 0.0f && direction.y == 0.0f && direction.z == 0.0f)
			direction.x = 1.0f;
		VECNormalize(&direction, &direction);
		direction *= param_1;
		JGeometry::TVec3<f32> velocity = mPositionDelta;
		velocity += direction;
		mPositionDelta = velocity;
	}
#endif
}

bool TAmiNoko::isDeadByWall()
{
	if (gpMap->isTouchedOneWallAndMoveXZ(&mPosition.x, mPosition.y,
	                                     &mPosition.z, 3.0f * mBodyRadius)) {
		JGeometry::TVec3<f32> zero(0.0f, 0.0f, 0.0f);
		mVelocity = zero;
		if (JPABaseEmitter* emitter = gpMarioParticleManager->emitWithRotate(
		        PARTICLE_MS_ENM_WALLHIT, &mPosition, 0,
		        DEG2SHORTANGLE(mRotation.y), 0, 0, nullptr))
			emitter->setGlobalScale(mScaling);
		if (JPABaseEmitter* emitter = gpMarioParticleManager->emitWithRotate(
		        PARTICLE_MS_ENM_WALLHIT_O, &mPosition, 0,
		        DEG2SHORTANGLE(mRotation.y), 0, 0, nullptr))
			SMSSetEmitterPolColor(emitter, 6);
		return true;
	}
	return false;
}

const char** TAmiNoko::getBasNameTable() const { return amiNoko_bastable; }

// TODO: fabricated; recover the distance inline shared by enemy code.
// By-value copy preserves the vector at 0x34 and the out-of-line sqrt call.
static inline f32 dist(JGeometry::TVec3<f32> pos,
                       const JGeometry::TVec3<f32>& b)
{
	pos.sub(b);
	return pos.length();
}

DEFINE_NERVE(TNerveAmiNokoWalkOnFence, TLiveActor)
{
	TAmiNoko* self = (TAmiNoko*)spine->getBody();
	if (spine->getTime() == 0)
		self->setWalkAnm();

	if (self->isBckAnm(5) || self->isBckAnm(8)) {
		if (self->checkCurAnmEnd(0)) {
			if (self->unk210 != 0)
				self->setBckAnm(4);
			else
				self->setBckAnm(7);
		}
	}

	const JGeometry::TVec3<f32>& pos = self->unkF4.getPoint();
	if (dist(pos, self->mPosition) < 1.5f && self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(3) || self->isBckAnm(6)) {
			self->goToRandomNextGraphNode();
			spine->pushAfterCurrent(&TNerveAmiNokoTurn::theNerve());
			return true;
		}
		if (self->isBckAnm(4))
			self->setBckAnm(3);
		else if (self->isBckAnm(7))
			self->setBckAnm(6);
	}

	self->creepToCurPathNode(3.0f);
	return false;
}

// TODO: getPoint inline reloads the actor pointer on the non-null branch.
DEFINE_NERVE(TNerveAmiNokoTurn, TLiveActor)
{
	TAmiNoko* self = (TAmiNoko*)spine->getBody();
	if (spine->getTime() == 0) {
		if (self->unk210 != 0)
			self->setBckAnm(11);
		else
			self->setBckAnm(14);
	}

	if (self->isBckAnm(11) || self->isBckAnm(14)) {
		if (self->checkCurAnmEnd(0)) {
			if (self->unk210 != 0)
				self->setBckAnm(10);
			else
				self->setBckAnm(13);
		}
	}

	const TPathNode* node     = &self->getUnkF4();
	JGeometry::TVec3<f32> pos = node->getPoint();
	pos -= self->mPosition;
	if (pos.x == 0.0f && pos.y == 0.0f && pos.z == 0.0f)
		pos.x = 1.0f;
	VECNormalize(&pos, &pos);
	if (pos.dot(self->unk1A8) > 0.8f && self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(9) || self->isBckAnm(12)) {
			if (self->unk210 != 0)
				self->setBckAnm(15);
			else
				self->setBckAnm(15);
		} else if (self->isBckAnm(15)) {
			spine->pushAfterCurrent(&TNerveAmiNokoWalkOnFence::theNerve());
			return true;
		} else {
			if (self->unk210 != 0)
				self->setBckAnm(9);
			else
				self->setBckAnm(12);
		}
	}

	self->creepToCurPathNode(0.0f);
	return false;
}

DEFINE_NERVE(TNerveAmiNokoAttack, TLiveActor) { return false; }

// TODO: vector inline stack slots and magnitude contraction still differ.
DEFINE_NERVE(TNerveAmiNokoDie, TLiveActor)
{
	TAmiNoko* self = (TAmiNoko*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setBckAnm(1);
		self->onHitFilter(HIT_FILTER_NO_COLLISION);
#ifdef VERSION_GMSP01
		self->unk208->onHitFilter(HIT_FILTER_NO_COLLISION);
#endif
	}
	if (self->checkCurAnmEnd(0) && self->isBckAnm(1)) {
		JGeometry::TVec3<f32> direction = self->mPosition - SMS_GetMarioPos();
		if (direction.x == 0.0f && direction.y == 0.0f && direction.z == 0.0f)
			direction.x = 1.0f;
		MtxPtr mtx  = self->getMActor()->getModel()->getBaseTRMtx();
		direction.x = mtx[0][1];
		direction.y = mtx[1][1];
		direction.z = mtx[2][1];
		MsVECNormalize(&direction, &direction);
		direction *= 20.0f;
		self->mPosition.y += 10.0f;
		self->mVelocity = direction;
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
		self->setBckAnm(0);
	}
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_KIL_SMOKE, self->getMActor()->getModel()->getBaseTRMtx(), 1,
	    self);
	if (self->isBckAnm(0) && spine->getTime() > 30) {
		JGeometry::TVec3<f32> direction = self->mPosition - SMS_GetMarioPos();
		if (self->isDeadByWall() || !self->isAirborne()
		    || JGeometry::TUtil<f32>::sqrt(direction.squared()) > 10000.0f) {
			self->onHitFilter(HIT_FILTER_NO_COLLISION);
			self->onLiveFlag(LIVE_FLAG_DEAD);
			self->onLiveFlag(LIVE_FLAG_UNK8);
			self->offLiveFlag(LIVE_FLAG_HIDDEN);
			self->offLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
			self->mHolder = nullptr;
			self->stopAnmSound();
			spine->reset();
			spine->setNext(&TNerveSmallEnemyDie::theNerve());
			spine->pushAfterCurrent(spine->getDefault());
			self->genRandomItem();
			return true;
		}
	}
	return false;
}

DEFINE_NERVE(TNerveAmiNokoFreeze, TLiveActor)
{
	TAmiNoko* self = (TAmiNoko*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(2);
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(
		        PARTICLE_MS_DNK_SHIBIRE_B,
		        self->getMActor()->getModel()->getAnmMtx(0), 0, nullptr))
			emitter->setGlobalScale(JGeometry::TVec3<f32>(2.0f));
	}

	if (self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(2))
			self->setBckAnm(15);
		else if (spine->getTime() > self->getSaveParams()->getSLFreezeWait())
			return true;
	}
	return false;
}
