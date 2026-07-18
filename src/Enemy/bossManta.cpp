#include <Enemy/BossManta.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Graph.hpp>
#include <Strategic/Strategy.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorData.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <Strategic/Spine.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Map/PollutionManager.hpp>
#include <Map/MapEventSirena.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <System/ParamInst.hpp>
#include <System/Resolution.hpp>
#include <MarioUtil/ReinitGX.hpp>
#include <GC2D/GCConsole2.hpp>
#include <System/MarDirector.hpp>
#include <System/FlagManager.hpp>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

f32 TBossManta::sScale[] = { 20.0f, 10.0f, 5.0f, 2.0f, 1.0f, 1.0f };
int TBossManta::sCenterJointIndex;
int TBossManta::sBodyJointIndex;
int TBossManta::sRwingJointIndex;
int TBossManta::sLwingJointIndex;
u8 TBossManta::sEscapeFromMario;
f32 TBossManta::sFrameRate[6] = { 0.3f, 0.5f, 1.2f, 2.0f, 5.0f, 5.3f };

namespace {

void AttackMario(THitActor* attacker)
{
	JGeometry::TVec3<f32> dir;
	dir.set(SMS_GetMarioPos());
	dir -= attacker->mPosition;
	dir.normalize();
	dir *= 2.0f;
	dir.y += 1.0f;
	SMS_SendMessageToMario(attacker, HIT_MESSAGE_ATTACK);
	SMS_SendMessageToMario(attacker, HIT_MESSAGE_THROWN);
	SMS_ThrowMario(dir, 60.0f);
}

} // namespace

static inline void lerp_hack(f32& value, f32 target, f32 progress)
{
	f32 step = progress * (target - value);
	value += step;
}

DEFINE_NERVE(TNerveMantaMove, TLiveActor)
{
	TBossManta* self = (TBossManta*)spine->getBody();
	s32 time         = spine->getTime();
	TGraphWeb* graph = self->getTracer()->getGraph();

	if (time == 0) {
		self->startWalkAnim();

		JGeometry::TVec3<f32> pt
		    = graph->indexToPoint((int)(MsRandF() * graph->getNodeNum()));
		lerp_hack(pt.x, gpMarioPos->x, 1.0f);
		lerp_hack(pt.y, gpMarioPos->y, 1.0f);
		lerp_hack(pt.z, gpMarioPos->z, 1.0f);
		self->unk158 = pt;
	}

	if (self->mGeneration < 3)
		SMSGetMSound()->startSoundActor(MSD_SE_BS_MANTA_DRAW_LV,
		                                &self->mPosition, 0, nullptr, 0, 4);

	if (self->mGeneration == 5 && time % 96 == self->getInstanceIndex())
		SMSGetMSound()->startSoundSet(MSD_SE_BS_MANTA_ATTACK, &self->mPosition,
		                              0, 0.0f, 0, 0, 4);

	JGeometry::TVec3<f32> toTarget;
	toTarget.sub(self->mPosition, self->unk158);

	if (toTarget.length() < 500.0f || time % 150 == 0) {
		JGeometry::TVec3<f32> pt
		    = graph->indexToPoint((int)(MsRandF() * graph->getNodeNum()));

		if (self->unk1A4) {
			switch (self->mGeneration) {
			case 0:
				lerp_hack(pt.x, gpMarioPos->x, 1.0f);
				lerp_hack(pt.y, gpMarioPos->y, 1.0f);
				lerp_hack(pt.z, gpMarioPos->z, 1.0f);
				break;
			case 1:
				lerp_hack(pt.x, gpMarioPos->x, 1.0f);
				lerp_hack(pt.y, gpMarioPos->y, 1.0f);
				lerp_hack(pt.z, gpMarioPos->z, 1.0f);
				break;
			case 2:
				lerp_hack(pt.x, gpMarioPos->x, 1.0f);
				lerp_hack(pt.y, gpMarioPos->y, 1.0f);
				lerp_hack(pt.z, gpMarioPos->z, 1.0f);
				break;
			case 3:
				lerp_hack(pt.x, gpMarioPos->x, 1.0f);
				lerp_hack(pt.y, gpMarioPos->y, 1.0f);
				lerp_hack(pt.z, gpMarioPos->z, 1.0f);
				break;
			case 4:
				lerp_hack(pt.x, gpMarioPos->x, 1.0f);
				lerp_hack(pt.y, gpMarioPos->y, 1.0f);
				lerp_hack(pt.z, gpMarioPos->z, 1.0f);
				break;
			case 5:
				lerp_hack(pt.x, gpMarioPos->x, 1.0f);
				lerp_hack(pt.y, gpMarioPos->y, 1.0f);
				lerp_hack(pt.z, gpMarioPos->z, 1.0f);
				break;
			}
		} else {
			switch (self->mGeneration) {
			case 0:
				lerp_hack(pt.x, gpMarioPos->x, 1.0f);
				lerp_hack(pt.y, gpMarioPos->y, 1.0f);
				lerp_hack(pt.z, gpMarioPos->z, 1.0f);
				break;
			case 1:
				lerp_hack(pt.x, gpMarioPos->x, 0.8f);
				lerp_hack(pt.y, gpMarioPos->y, 0.8f);
				lerp_hack(pt.z, gpMarioPos->z, 0.8f);
				break;
			case 2:
				lerp_hack(pt.x, gpMarioPos->x, 0.8f);
				lerp_hack(pt.y, gpMarioPos->y, 0.8f);
				lerp_hack(pt.z, gpMarioPos->z, 0.8f);
				break;
			case 3:
				lerp_hack(pt.x, gpMarioPos->x, 0.6f);
				lerp_hack(pt.y, gpMarioPos->y, 0.6f);
				lerp_hack(pt.z, gpMarioPos->z, 0.6f);
				break;
			case 4:
				lerp_hack(pt.x, gpMarioPos->x, 0.8f);
				lerp_hack(pt.y, gpMarioPos->y, 0.8f);
				lerp_hack(pt.z, gpMarioPos->z, 0.8f);
				break;
			case 5:
				lerp_hack(pt.x, gpMarioPos->x, 1.0f);
				lerp_hack(pt.y, gpMarioPos->y, 1.0f);
				lerp_hack(pt.z, gpMarioPos->z, 1.0f);
				break;
			}
		}
		self->unk158 = pt;
	}

	if (time % self->unk188 == 0)
		self->unk1A4 ^= 1;

	if (time % 100 == 0)
		self->updateAttractor();

	JGeometry::TVec3<f32> dir = self->unk164;
	dir.normalize();

	dir *= self->unk194;
	self->unk170 += dir;

	self->unk170.normalize();

	return FALSE;
}

DEFINE_NERVE(TNerveMantaHitWater, TLiveActor)
{
	TBossManta* self = (TBossManta*)spine->getBody();

	if (spine->getTime() == 0) {
		self->startDamageAnim();

		const u32 hitSounds[6]
		    = { MSD_SE_BS_MANTA_DAMAGE_1, MSD_SE_BS_MANTA_DAMAGE_2,
			    MSD_SE_BS_MANTA_DAMAGE_3, MSD_SE_BS_MANTA_DAMAGE_4,
			    MSD_SE_BS_MANTA_DAMAGE_5, MSD_SE_BS_MANTA_DAMAGE_5 };
		u32 snd = hitSounds[self->mGeneration];
		SMSGetMSound()->startSoundActor(snd, &self->mPosition, 0, nullptr, 0,
		                                4);
	}

	int effectCount = self->getSaveParams()->mSLDamageEffectNum.get();

	const int particles[6][3] = {
		{ SCENE_MANTA_JPA_MS_MAN_HIT4_A1, SCENE_MANTA_JPA_MS_MAN_HIT4_A2,
		  SCENE_MANTA_JPA_MS_MAN_HIT4_B },
		{ SCENE_MANTA_JPA_MS_MAN_HIT3_A1, SCENE_MANTA_JPA_MS_MAN_HIT3_A2,
		  SCENE_MANTA_JPA_MS_MAN_HIT3_B },
		{ SCENE_MANTA_JPA_MS_MAN_HIT2_A, -1, SCENE_MANTA_JPA_MS_MAN_HIT2_B },
		{ SCENE_MANTA_JPA_MS_MAN_HIT1_A, -1, SCENE_MANTA_JPA_MS_MAN_HIT1_B },
		{ SCENE_MANTA_JPA_MS_MAN_HIT1_A, -1, SCENE_MANTA_JPA_MS_MAN_HIT1_B },
		{ SCENE_MANTA_JPA_MS_MAN_HIT1_A, -1, SCENE_MANTA_JPA_MS_MAN_HIT1_B },
	};

	for (int j = 0; j < 3; ++j) {
		if (particles[self->mGeneration][j] > 0) {
			for (int i = 0; i < effectCount; ++i) {
				gpMarioParticleManager->emitAndBindToPosPtr(
				    particles[self->mGeneration][j], &self->unk17C, 1,
				    (u8*)self + i * sizeof(TBossManta));
				if (j == 2)
					break;
			}
		}
	}

	if (self->checkCurAnmEnd(0)) {
		self->startDamageAnimEpilogue();
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveMantaSpawn, TLiveActor)
{
	TBossManta* self = (TBossManta*)spine->getBody();

	if (spine->getTime() < 5) {
		self->mScaling.x *= 1.01f;
		self->mScaling.y *= 1.01f;
		self->mScaling.z *= 1.01f;
	} else {
		self->mScaling.x *= 0.9f;
		self->mScaling.y *= 0.9f;
		self->mScaling.z *= 0.9f;
	}

	if (spine->getTime() == 0) {
		const int particles[4]
		    = { SCENE_MANTA_JPA_MS_MAN_DIV4, SCENE_MANTA_JPA_MS_MAN_DIV3,
			    SCENE_MANTA_JPA_MS_MAN_DIV2, SCENE_MANTA_JPA_MS_MAN_DIV1 };
		gpMarioParticleManager->emitAndBindToPosPtr(
		    particles[self->mGeneration], &self->unk17C, 0, self);

		const u32 sounds[4]
		    = { MSD_SE_BS_MANTA_SEGMENT_1, MSD_SE_BS_MANTA_SEGMENT_2,
			    MSD_SE_BS_MANTA_SEGMENT_3, MSD_SE_BS_MANTA_SEGMENT_4 };
		u32 snd = sounds[self->mGeneration];
		SMSGetMSound()->startSoundActor(snd, &self->mPosition, 0, nullptr, 0,
		                                4);
		self->getManager()->spawn(self->mGeneration + 1, self->mPosition);
	}

	if (spine->getTime() == 0x1E) {
		self->kill();
		return TRUE;
	}
	return FALSE;
}

DEFINE_NERVE(TNerveMantaDeath, TLiveActor)
{
	TBossManta* self = (TBossManta*)spine->getBody();

	if (spine->getTime() == 0) {
		self->getMActor()->setBckFromIndex(0);
		self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 0);
		self->getMActor()->setMotionBlendRatioForBck(0.0f);
	}

	if (self->checkCurAnmEnd(0)) {
		gpMarioParticleManager->emitAndBindToPosPtr(SCENE_MANTA_JPA_MS_MAN_DEAD,
		                                            &self->unk17C, 0, self);
		SMSGetMSound()->startSoundActor(MSD_SE_BS_MANTA_VANISH,
		                                &self->mPosition, 0, nullptr, 0, 4);
		self->kill();
	}

	return FALSE;
}

DEFINE_NERVE(TNerveMantaAppearDemo, TLiveActor)
{
	s32 time         = spine->getTime();
	TBossManta* self = (TBossManta*)spine->getBody();

	if (time == 0) {
		self->mPosition.x = 0.0f;
		self->mPosition.y = 0.0f;
		self->mPosition.z = self->getSaveParams()->mSLAppearDemoInitialZ.get();
		self->unk170.x    = 0.0f;
		self->unk170.y    = 0.0f;
		self->unk170.z    = -1.0f;
		self->unk158.set(0.0f, 0.0f, -10000.0f);

		self->startWalkAnim();
	}

	if (time == 0x2D0)
		MSBgm::startBGM(0x8001002B);

	if (time == 0x690)
		return TRUE;

	SMSGetMSound()->startSoundActor(MSD_SE_BS_MANTA_DRAW_DEMO, &self->mPosition,
	                                0, nullptr, 0, 4);

	self->unk190 = self->getSaveParams()->mSLAppearDemoWalkSpeed.get();
	return FALSE;
}

TBossManta::TBossManta(const char* name)
    : TSpineEnemy(name)
{
	unk158.set(0.0f, 0.0f, 0.0f);
	unk164.set(0.0f, 0.0f, 0.0f);
	unk170.set(0.0f, 0.0f, 1.0f);
	unk17C.set(0.0f, 0.0f, 0.0f);
	unk188      = 0;
	mGeneration = 0;
	unk190      = 0.0f;
	unk194      = 0.0f;
	unk198      = 0.0f;
	unk19C      = 0;
	unk1A0      = 0;
}

f32 TBossManta::getTailAnimSpeed()
{
	const f32 blend[6] = { 0.005f, 0.008f, 0.01f, 0.03f, 0.05f, 0.05f };
	return blend[mGeneration];
}

void TBossManta::setCollision()
{
	if (mGeneration <= 2) {
		setHitParams(0.0f, 0.0f, 0.0f, 0.0f);
	} else {
		f32 r = 80.0f * mScaling.x;
		setHitParams(r, 100.0f, r, 100.0f);
	}

	offHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBossManta::startWalkAnim()
{
	getMActor()->initNormalMotionBlend();
	getMActor()->setBckFromIndex(3);

	J3DAnmTransform* oldAnm
	    = getActorKeeper()->getMActorAnmData()->getUnk2C()->getAnmPtr(4);
	getMActor()->setBckOldMotionBlendAnmPtr(oldAnm);
	getMActor()->setMotionBlendRatioForBck(0.5f);

	getMActor()->setFrameRate(
	    TBossManta::sFrameRate[mGeneration] * SMSGetAnmFrameRate(), 0);
}

void TBossManta::startDamageAnim()
{
	getMActor()->setBckFromIndex(1);
	getMActor()->setFrameRate(
	    TBossManta::sFrameRate[mGeneration] * SMSGetAnmFrameRate(), 0);
	getMActor()->setMotionBlendRatioForBck(0.0f);
}

void TBossManta::updateAnimBlend()
{
	if (getMActor()->checkCurBckFromIndex(3)) {
		JGeometry::TVec3<f32> local_134 = unk164;
		local_134.normalize();

		f32 b = getTailAnimSpeed();

		JGeometry::TVec3<f32> cross1;
		cross1.cross(unk170, local_134);

		// TODO: HACK, maybe they did one more cross product?
		volatile f32 turn = 0.5f - 0.4f * cross1.y;

		unk150 = (1.0f - b) * unk150 + b * turn;

		getMActor()->setMotionBlendRatioForBck(JGeometry::TUtil<f32>::clamp(
		    unk150 + getEpilogueValue(), 0.0f, 1.0f));
	} else {
		getMActor()->setMotionBlendRatioForBck(0.0f);
	}
}

bool TBossManta::getIntoGraphVec(JGeometry::TVec3<f32>* out)
{
	TGraphWeb* graph = getTracer()->getGraph();
	for (int idx = 0; idx < 12; ++idx) {
		JGeometry::TVec3<f32> a;
		a.sub(graph->indexToPoint(idx), mPosition);
		a.y = 0.0f;

		JGeometry::TVec3<f32> d;
		d.sub(graph->indexToPoint(idx), graph->indexToPoint(idx + 1));
		d.y = 0.0f;

		if (a.z * d.x - a.x * d.z < 0.0f) {
			JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
			out->cross(up, d);
			out->normalize();
			return true;
		}
	}

	JGeometry::TVec3<f32> a;
	a.sub(graph->indexToPoint(12), mPosition);
	a.y = 0.0f;

	JGeometry::TVec3<f32> d;
	d.sub(graph->indexToPoint(12), graph->indexToPoint(0));
	d.y = 0.0f;

	if (a.z * d.x - a.x * d.z < 0.0f) {
		JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
		out->cross(up, d);
		out->normalize();
		return true;
	}

	return false;
}

void TBossManta::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("manta.bmd", 0);
	unk124->setGraph(gpConductor->getGraphByName("main"));

	mHitPoints = getMaxHitPoints();

	mHeadHeight = 5000.0f;
	initHitActor(0x8000004, 1, 0x80000000, 0.0f, 0.0f, 0.0f, 0.0f);
	unk170.set(0.0f, 0.0f, 1.0f);
	calcRootMatrix();
	kill();

	JDrama::TNameRefGen::search<TIdxGroupObj>("オブジェクトグループ")
	    ->getChildren()
	    .push_back(this);

	if (getInstanceIndex() == 0) {
		JUTNameTab* jointNames = getModel()->getModelData()->getJointName();
		sCenterJointIndex      = jointNames->getIndex("center");
		sBodyJointIndex        = jointNames->getIndex("jnt_body");
		sRwingJointIndex       = jointNames->getIndex("jnt_Rwing2");
		sLwingJointIndex       = jointNames->getIndex("jnt_Lwing2");

		getManager()->initAdditionalCollision();
	}

	onLiveFlag(LIVE_FLAG_UNK8);
	offLiveFlag(LIVE_FLAG_UNK100);
	unk1A4 = 0;
	unk150 = 0.5f;
}

void TBossManta::moveObject()
{
	TLiveActor::moveObject();

	if (isPolluting())
		gpPollution->pollute(mPosition.x, mPosition.y, mPosition.z,
		                     getPolluteRadius());

	for (int i = 0; i < mColCount; ++i)
		if (mCollisions[i]->isActorType(0x80000001))
			AttackMario(mCollisions[i]);
}

BOOL TBossManta::isSpawnState()
{
	return mSpine->getLatestNerve() == &TNerveMantaSpawn::theNerve();
}

bool TBossManta::collidedWithWater()
{
	if (isDamageable()) {
		BOOL isHitWater;
		if (mSpine->getLatestNerve() == &TNerveMantaMove::theNerve())
			isHitWater = false;
		else if (mSpine->getLatestNerve() == &TNerveMantaHitWater::theNerve())
			isHitWater = true;
		else
			isHitWater = false;

		const int hitCounts[6] = { 16, 8, 4, 2, 1, 1 };
		if (unk19C < hitCounts[mGeneration]) {
			unk1A0 = 0x1E;
			if (!isHitWater) {
				mSpine->reset();
				mSpine->setNext(&TNerveMantaHitWater::theNerve());
			}
			unk19C++;

			const int hitCounts2[6] = { 16, 8, 4, 2, 1, 1 };
			if (unk19C == hitCounts2[mGeneration]) {
				if (mGeneration >= 4)
					mSpine->pushAfterCurrent(&TNerveMantaDeath::theNerve());
				else
					mSpine->pushAfterCurrent(&TNerveMantaSpawn::theNerve());
			} else if (mSpine->getVertebraeCount() < 7) {
				mSpine->pushAfterCurrent(&TNerveMantaMove::theNerve());
			}
		}
		return TRUE;
	}

	return FALSE;
}

BOOL TBossManta::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER
	    && gpModelWaterManager
	               ->mParticleFlagSOA[((TWaterHitActor*)sender)->unk68]
	           & 0x40)
		return collidedWithWater();
	return FALSE;
}

void TBossManta::initNthGeneration(int gen)
{
	const f32 heights[6] = { 10.0f, 5.0f, 1.0f, 0.42f, 0.42f, 0.42f };

	mGeneration = gen;
	f32 s       = sScale[mGeneration];
	mScaling.z  = s;
	mScaling.x  = s;
	mScaling.y  = heights[mGeneration];
	unk19C      = 0;
	resetDamageAnimEpilogue();

	switch (mGeneration) {
	case 0:
		mSpine->initWith(&TNerveMantaAppearDemo::theNerve());
		mSpine->pushAfterCurrent(&TNerveMantaMove::theNerve());
		unk188 = 0x258;
		unk190 = 2.0f;
		unk194 = 0.009f;
		unk198 = 1000.0f;
		unk1A0 = 0;
		break;
	case 1:
		mSpine->initWith(&TNerveMantaMove::theNerve());
		unk188 = (s32)(100.0f * MsRandF()) + 0x258;
		unk190 = 2.0f;
		unk194 = 0.009f;
		unk198 = 1000.0f;
		unk1A0 = 0x78;
		break;
	case 2:
		mSpine->initWith(&TNerveMantaMove::theNerve());
		unk188 = (s32)(100.0f * MsRandF()) + 0xC8;
		unk190 = 3.0f;
		unk194 = 0.009f;
		unk198 = 1000.0f;
		unk1A0 = 0x78;
		break;
	case 3:
		mSpine->initWith(&TNerveMantaMove::theNerve());
		unk188 = (s32)(100.0f * MsRandF()) + 0x64;
		unk190 = 4.0f;
		unk194 = 0.019f;
		unk198 = 700.0f;
		unk1A0 = 0x78;
		break;
	case 4:
		mSpine->initWith(&TNerveMantaMove::theNerve());
		unk188 = (s32)(100.0f * MsRandF()) + 0x64;
		unk190 = 7.0f;
		unk194 = 0.03f;
		unk198 = 400.0f;
		unk1A0 = 0x168;
		break;
	case 5:
		mSpine->initWith(&TNerveMantaMove::theNerve());
		unk188 = (s32)(100.0f * MsRandF()) + 0x64;
		unk190 = 3.0f;
		unk194 = 0.03f;
		unk198 = 200.0f;
		unk1A0 = 0;
		break;
	}

	setCollision();

	offLiveFlag(LIVE_FLAG_DEAD);
	if (mGeneration <= 2)
		getManager()->adaptAdditionalCollision(this);
}

void TBossManta::control()
{
	if (unk1A0 > 0)
		unk1A0 -= 1;

	updateAnimBlend();

	JGeometry::TVec3<f32> vel(unk170);
	vel *= unk190;
	JGeometry::TVec3<f32> curVel(mVelocity);
	vel.y     = curVel.y;
	mVelocity = vel;

	TLiveActor::control();
}

void TBossManta::startDamageAnimEpilogue()
{
	unk150 = 0.5f;
	unk154 = 0;
}

void TBossManta::resetDamageAnimEpilogue()
{
	unk150 = 0.5f;
	unk154 = 49;
}

f32 TBossManta::getEpilogueValue()
{
	const f32 animBlend[50] = {
		0.0f,    0.046f,  0.107f,  0.177f,  0.227f,  0.256f,  0.273f,  0.279f,
		0.274f,  0.26f,   0.237f,  0.209f,  0.175f,  0.138f,  0.1f,    0.061f,
		0.024f,  -0.011f, -0.043f, -0.07f,  -0.092f, -0.108f, -0.12f,  -0.126f,
		-0.127f, -0.123f, -0.115f, -0.104f, -0.09f,  -0.074f, -0.057f, -0.04f,
		-0.022f, -0.005f, 0.01f,   0.024f,  0.035f,  0.045f,  0.051f,  0.056f,
		0.058f,  0.057f,  0.055f,  0.051f,  0.046f,  0.039f,  0.031f,  0.023f,
		0.015f,  0.007f,
	};

	return animBlend[unk154];
}

void TBossManta::updateEpilogueFrame()
{
	if (unk154 < 49)
		unk154++;
}

void TBossManta::calcRootMatrix()
{
	updateEpilogueFrame();

	TPosition3f m;
	m.setTrans(mPosition);

	JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
	JGeometry::TVec3<f32> side;
	side.cross2(up, unk170);
	m.setXDir(side);
	m.setYDir(up);
	m.setZDir(unk170);

	MtxPtr joint = getModel()->getAnmMtx(sCenterJointIndex);
	unk17C.set(joint[0][3], mPosition.y, joint[2][3]);
	getModel()->setBaseScale(mScaling);
	getModel()->setBaseTRMtx(m);
}

bool TBossManta::isDamageable()
{
	if (unk1A0 > 0)
		return FALSE;

	return mSpine->getLatestNerve() == &TNerveMantaMove::theNerve()
	       || mSpine->getLatestNerve() == &TNerveMantaHitWater::theNerve();
}

bool TBossManta::isPolluting()
{
	const u8 pollute[6] = { 1, 1, 1, 1, 1, 1 };
	return pollute[mGeneration];
}

f32 TBossManta::getPolluteRadius()
{
	switch (mGeneration) {
	case 0:
	case 1:
	case 2:
	case 3:
		return getSaveParams()->mSLPolluteRadius.get() * mScaling.x;
	case 4:
	case 5:
		return 100.0f;
	}
	return 0.0f;
}

void TBossManta::updateAttractor()
{
	JGeometry::TVec3<f32> local_108 = mPosition;
	local_108 -= unk158;
	local_108.y = 0.0f;
	local_108.normalize();
	local_108 *= getSaveParams()->mSLAttractorPower.get();

	JGeometry::TVec3<f32> facing = unk170;
	facing *= getSaveParams()->mSLPusherPower.get();

	JGeometry::TVec3<f32> selfPos = mPosition;
	selfPos += facing;

	for (int i = 0; i < getManager()->getActiveObjNum(); ++i) {
		TBossManta* other = (TBossManta*)getManager()->getObj(i);
		if (checkLiveFlag(LIVE_FLAG_DEAD)
		    || other->checkLiveFlag(LIVE_FLAG_DEAD)
		    || other->getInstanceIndex() == getInstanceIndex())
			continue;

		JGeometry::TVec3<f32> otherFacing = other->unk170;
		otherFacing *= getSaveParams()->mSLPusherPower.get();

		JGeometry::TVec3<f32> otherPos = mPosition;
		otherPos += otherFacing;

		JGeometry::TVec3<f32> delta;
		delta.sub(selfPos, otherPos);
		delta.y = 0.0f;

		if (0.1f < delta.length()
		    && delta.length() < getSaveParams()->mSLEscapeRegion.get()) {
			JGeometry::TVec3<f32> thing;
			thing.set(delta);
			thing.normalize();
			thing *= getSaveParams()->mSLPusherPower.get() / thing.length();
			local_108 += thing;
		}
	}

	JGeometry::TVec3<f32> local_C0 = mPosition;
	local_C0 -= SMS_GetMarioPos();
	local_C0.y = 0.0f;

	f32 marioDist = local_C0.length();
	if ((sEscapeFromMario != 0 || mGeneration == 4) && marioDist < 6000.0f) {
		local_108 += local_C0;
	}

	local_108.y = 0.0f;
	JGeometry::TVec3<f32> graphVec;
	if (getIntoGraphVec(&graphVec)) {
		graphVec.y = 0.0f;
		graphVec *= 10000.0f;
		local_108 += graphVec;
	}

	unk164 = local_108;
}

void TBossMantaManager::TMantaBattleState::update()
{
	static JAISound* sDefeatSE;

	switch (mState) {
	case 0:
		if (TFlagManager::getInstance()->getBool(0x50007)) {
			SMSGetMarDirector()->fireStartDemoCamera(
			    "sirena_manta", nullptr, -1, 0.0f, true, nullptr, 0, nullptr,
			    JDrama::TFlagT<u16>(0));
			((TBossManta*)unk0->getObj(0))->initNthGeneration(0);
			MSBgm::stopTrackBGMs(7, 10);
			mState++;
		}
		break;
	case 1: {
		bool allMaxGen = true;
		for (int i = 0; i < unk0->getActiveObjNum(); ++i) {
			TBossManta* m = (TBossManta*)unk0->getObj(i);
			if (m->checkLiveFlag(LIVE_FLAG_DEAD))
				continue;
			if (m->mGeneration != 4) {
				allMaxGen = false;
				break;
			}
		}
		if (allMaxGen) {
			for (int i = 0; i < unk0->getActiveObjNum(); ++i) {
				TBossManta* m = (TBossManta*)unk0->getObj(i);
				if (!m->checkLiveFlag(LIVE_FLAG_DEAD))
					m->initNthGeneration(5);
			}
			mState++;
		}
		break;
	}
	case 2: {
		bool victory = true;
		for (int i = 0; i < unk0->getActiveObjNum(); ++i) {
			TBossManta* m = (TBossManta*)unk0->getObj(i);
			if (m->mGeneration != 5)
				continue;
			if (m->checkLiveFlag(LIVE_FLAG_DEAD))
				continue;
			victory = false;
			break;
		}
		if (victory) {
			MSBgm::stopTrackBGMs(7, 10);
			sDefeatSE = nullptr;
			SMSGetMSound()->startSoundActor(MSD_SE_BS_MANTA_ALL_DEATH, nullptr,
			                                0, &sDefeatSE, 0, 4);
			mState++;
		}
		break;
	}
	case 3:
		if (sDefeatSE == nullptr) {
			JDrama::TNameRefGen::search<TMapEventSirenaSink>(
			    "イベント（ホテル沈む）")
			    ->unk64
			    = true;
			mState++;
		}
		break;
	case 4:
		break;
	}
}

void TBossMantaManager::TMantaMessageState::update()
{
	switch (unk4) {
	case 0:
		if (((TBossManta*)unk0->getObj(0))->isSpawnState()) {
			gpMarDirector->getConsole()->startAppearBalloon(0xE000C, true);
			unk4++;
		}
		break;
	case 1: {
		int i;
		int aliveCount = 0;
		for (i = 0; i < unk0->getActiveObjNum(); ++i) {
			if (!unk0->getObj(i)->checkLiveFlag(LIVE_FLAG_DEAD))
				aliveCount++;
		}
		if (aliveCount > 50) {
			gpMarDirector->getConsole()->startAppearBalloon(0xE000D, true);
			unk4++;
		}
		break;
	}
	case 2:
		if (unk0->unk88.mState == 2) {
			gpMarDirector->getConsole()->startAppearBalloon(0xE000E, true);
			unk4++;
		}
		break;
	case 3:
		break;
	}
}

TBossMantaAdditionalCollisionSet::TBossMantaAdditionalCollisionSet()
{
	unkC = nullptr;
	for (int i = 0; i < 3; ++i)
		unk0[i] = new TBossMantaAdditionalCollision("マンタ追加コリジョン");
}

void TBossMantaAdditionalCollisionSet::adapt(TBossManta* manta)
{
	unkC = manta;

	unk0[0]->setHitParams(54.0f * unkC->mScaling.x, 100.0f,
	                      54.0f * unkC->mScaling.x, 100.0f);
	unk0[1]->setHitParams(26.0f * unkC->mScaling.x, 100.0f,
	                      26.0f * unkC->mScaling.x, 100.0f);
	unk0[2]->setHitParams(26.0f * unkC->mScaling.x, 100.0f,
	                      26.0f * unkC->mScaling.x, 100.0f);

	for (int i = 0; i < 3; ++i)
		unk0[i]->setManta(unkC);
}

bool TBossMantaAdditionalCollisionSet::isUsed()
{
	if (unkC != nullptr && !unkC->checkLiveFlag(LIVE_FLAG_DEAD))
		return true;
	return false;
}

void TBossMantaAdditionalCollisionSet::update(u32 cue,
                                              JDrama::TGraphics* graphics)
{
	if (unkC != nullptr) {
		if (unkC->checkLiveFlag(LIVE_FLAG_DEAD)) {
			unkC = nullptr;
			return;
		}
		for (int i = 0; i < 3; ++i)
			unk0[i]->perform(cue, graphics);

		int centerIdx    = TBossManta::sCenterJointIndex;
		MtxPtr centerMtx = unkC->getModel()->mNodeMatrices[centerIdx];
		f32 centerX      = centerMtx[0][3];
		f32 centerY      = centerMtx[1][3];
		f32 centerZ      = centerMtx[2][3];

		int bodyIdx    = TBossManta::sBodyJointIndex;
		MtxPtr bodyMtx = unkC->getModel()->mNodeMatrices[bodyIdx];
		f32 bodyX      = bodyMtx[0][3];
		f32 bodyY      = bodyMtx[1][3];
		f32 bodyZ      = bodyMtx[2][3];

		int rwingIdx    = TBossManta::sRwingJointIndex;
		MtxPtr rwingMtx = unkC->getModel()->mNodeMatrices[rwingIdx];
		f32 rwingX      = rwingMtx[0][3];
		f32 rwingY      = rwingMtx[1][3];
		f32 rwingZ      = rwingMtx[2][3];

		int lwingIdx    = TBossManta::sLwingJointIndex;
		MtxPtr lwingMtx = unkC->getModel()->mNodeMatrices[lwingIdx];
		f32 lwingX      = lwingMtx[0][3];
		f32 lwingY      = lwingMtx[1][3];
		f32 lwingZ      = lwingMtx[2][3];

		unk0[0]->mPosition.set(-0.15f * (bodyX - centerX) + centerX,
		                       -0.15f * (bodyY - centerY) + centerY,
		                       -0.15f * (bodyZ - centerZ) + centerZ);

		unk0[1]->mPosition.set(0.75f * (rwingX - centerX) + centerX,
		                       0.75f * (rwingY - centerY) + centerY,
		                       0.75f * (rwingZ - centerZ) + centerZ);

		unk0[2]->mPosition.set(0.75f * (lwingX - centerX) + centerX,
		                       0.75f * (lwingY - centerY) + centerY,
		                       0.75f * (lwingZ - centerZ) + centerZ);
	}
}

TBossMantaAdditionalCollision::TBossMantaAdditionalCollision(const char* name)
    : THitActor(name)
    , unk68(nullptr)
{
	initHitActor(0x08000004, 1, 0x80000000, 0.0f, 0.0f, 0.0f, 0.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);

	JDrama::TNameRefGen::search<TIdxGroupObj>("オブジェクトグループ")
	    ->insert(this);
}

BOOL TBossMantaAdditionalCollision::receiveMessage(THitActor* sender,
                                                   u32 message)
{
	if (unk68 == nullptr)
		return FALSE;
	return unk68->receiveMessage(sender, message);
}

void TBossMantaAdditionalCollision::setManta(TBossManta* manta)
{
	unk68 = manta;
}

void TBossMantaAdditionalCollision::perform(u32 cue,
                                            JDrama::TGraphics* graphics)
{
	THitActor::perform(cue, graphics);

	if (cue & CUE_MOVE) {
		for (int i = 0; i < mColCount; ++i)
			if (mCollisions[i]->isActorType(0x80000001))
				AttackMario(mCollisions[i]);
	}
}

TBossMantaManager::TBossMantaManager(const char* name)
    : TEnemyManager(name)
    , unk7C(0)
    , unk80(0)
    , unk88(this)
    , unk90(this)
{
	unk7C = new (0x20) u8[SMSGetGameRenderWidth() * SMSGetGameRenderHeight()];
	unk74 = new JGeometry::TVec3<f32>[7];
	unk78 = new JGeometry::TVec3<f32>[2];
}

void TBossMantaManager::adaptAdditionalCollision(TBossManta* manta)
{
	for (int i = 0; i < 8; ++i) {
		if (!mCollisionSets[i]->isUsed()) {
			mCollisionSets[i]->adapt(manta);
			return;
		}
	}
}

void TBossMantaManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TBossMantaParams("/enemy/bossmanta.prm");
	TEnemyManager::load(stream);
}

void TBossMantaManager::loadEffects()
{
	static const char* onetimeFilenames[5] = {
		"/scene/manta/jpa/ms_man_dead.jpa", "/scene/manta/jpa/ms_man_div1.jpa",
		"/scene/manta/jpa/ms_man_div2.jpa", "/scene/manta/jpa/ms_man_div3.jpa",
		"/scene/manta/jpa/ms_man_div4.jpa",
	};
	for (int i = 0; i < 5; ++i)
		SMS_LoadParticle(onetimeFilenames[i], 0xF8 + i);

	static const char* loopFilenames[10] = {
		"/scene/manta/jpa/ms_man_hit1_a.jpa",
		"/scene/manta/jpa/ms_man_hit1_b.jpa",
		"/scene/manta/jpa/ms_man_hit2_a.jpa",
		"/scene/manta/jpa/ms_man_hit2_b.jpa",
		"/scene/manta/jpa/ms_man_hit3_a1.jpa",
		"/scene/manta/jpa/ms_man_hit3_a2.jpa",
		"/scene/manta/jpa/ms_man_hit3_b.jpa",
		"/scene/manta/jpa/ms_man_hit4_a1.jpa",
		"/scene/manta/jpa/ms_man_hit4_a2.jpa",
		"/scene/manta/jpa/ms_man_hit4_b.jpa",
	};
	for (int i = 0; i < 10; ++i)
		SMS_LoadParticle(loopFilenames[i], 0x1C7 + i);
}

void TBossMantaManager::loadAfter()
{
	loadEffects();

	for (int i = 0; i < 7; ++i) {
		char name[0x40];
		snprintf(name, 0x40, "palmOugi %d", i);
		TLiveActor* palm = JDrama::TNameRefGen::search<TLiveActor>(name);
		unk74[i].set(palm->mPosition.x, 0.0f, palm->mPosition.z);
	}

	unk78[0].set(1549.0f, 0.0f, -435.0f);
	unk78[1].set(-1457.0f, 0.0f, -533.0f);

	unk88.mState                 = 0;
	unk90.unk4                   = 0;
	unk84                        = 0;
	TBossManta::sEscapeFromMario = 0;
}

void TBossMantaManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "manta.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (1 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TSpineEnemy* TBossMantaManager::createEnemyInstance() { return new TBossManta; }

// TODO: incorrect
const JUtility::TColor& TBossMantaManager::getMantaColor()
{
	if (unk88.mState == 2) {
		unk84 = (unk84 + 1 > 15) ? 15 : unk84 + 1;
	}

	TBossMantaParams* params = (TBossMantaParams*)getSaveParam();
	f32 t                    = (f32)unk84 / 15.0f;
	unk80.r                  = (u8)((1.0f - t) * (f32)params->mSLMantaRed.get()
                   + t * (f32)params->mSLAngryMantaRed.get());
	unk80.g = (u8)((1.0f - t) * (f32)params->mSLMantaGreen.get()
	               + t * (f32)params->mSLAngryMantaGreen.get());
	unk80.b = (u8)((1.0f - t) * (f32)params->mSLMantaBlue.get()
	               + t * (f32)params->mSLAngryMantaBlue.get());
	unk80.a = (u8)((1.0f - t) * (f32)params->mSLMantaAlpha.get()
	               + t * (f32)params->mSLAngryMantaAlpha.get());

	return unk80;
}

void TBossMantaManager::drawMantaShadow(JDrama::TGraphics* graphics)
{
	setupEfbAlpha(graphics);

	for (int i = 0; i < getActiveObjNum(); ++i) {
		TSpineEnemy* enemy = getObj(i);
		if (enemy->checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_CLIPPED_OUT))
			continue;

		GXSetCullMode(GX_CULL_FRONT);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
		for (u16 j = 0; j < enemy->getModel()->getModelData()->getShapeNum();
		     ++j)
			enemy->getModel()->getShapePacket(j)->draw();

		GXSetCullMode(GX_CULL_BACK);
		GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
		for (u16 j = 0; j < enemy->getModel()->getModelData()->getShapeNum();
		     ++j)
			enemy->getModel()->getShapePacket(j)->draw();
	}

	GXSetTexCopySrc(0, 0, (u16)SMSGetGameRenderWidth(),
	                (u16)SMSGetGameRenderHeight());
	GXSetTexCopyDst((u16)SMSGetGameRenderWidth(), (u16)SMSGetGameRenderHeight(),
	                GX_TF_A8, GX_FALSE);
	GXCopyTex(unk7C, GX_FALSE);
	GXPixModeSync();

	Mtx44 proj;
	C_MTXOrtho(proj, (f32)SMSGetGameRenderHeight(), 0.0f, 0.0f,
	           (f32)SMSGetGameRenderWidth(), 0.0f, 1000.0f);
	GXSetProjection(proj, GX_ORTHOGRAPHIC);
	GXSetNumTevStages(1);
	GXSetNumChans(0);
	GXSetNumTexGens(1);
	GXSetZCompLoc(GX_FALSE);

	GXTexObj texObj;
	GXInitTexObj(&texObj, unk7C, (u16)SMSGetGameRenderWidth(),
	             (u16)SMSGetGameRenderHeight(), GX_TF_I8, GX_CLAMP, GX_CLAMP,
	             GX_FALSE);
	GXInitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE,
	                GX_FALSE, GX_ANISO_1);
	GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
	GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);

	GXSetTevKColor(GX_KCOLOR0, getMantaColor());

	GXLoadTexObj(&texObj, GX_TEXMAP0);
	GXSetDstAlpha(GX_TRUE, 0xFF);
	GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
	                  GX_FALSE, GX_PTIDENTITY);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_KONST,
	                GX_CA_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_R8_GT, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXA, GX_CC_ZERO, GX_CC_KONST,
	                GX_CC_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_COMP_R8_GT, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetDstAlpha(GX_FALSE, 0);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_INVSRCALPHA, GX_LO_NOOP);
	GXSetAlphaUpdate(GX_FALSE);
	GXSetColorUpdate(GX_TRUE);

	Mtx m;
	MTXIdentity(m);
	GXLoadPosMtxImm(m, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetCullMode(GX_CULL_NONE);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(0.0f, (f32)SMSGetGameRenderHeight(), -10.0f);
	GXTexCoord2f32(0.0f, 0.0f);
	GXPosition3f32((f32)SMSGetGameRenderWidth(), (f32)SMSGetGameRenderHeight(),
	               -10.0f);
	GXTexCoord2f32(1.0f, 0.0f);
	GXPosition3f32((f32)SMSGetGameRenderWidth(), 0.0f, -10.0f);
	GXTexCoord2f32(1.0f, 1.0f);
	GXPosition3f32(0.0f, 0.0f, -10.0f);
	GXTexCoord2f32(0.0f, 1.0f);
	GXEnd();

	GXSetProjection(graphics->mProjMtx.mMtx, GX_PERSPECTIVE);
}

void TBossMantaManager::updateMantaEscape()
{
	TBossManta::sEscapeFromMario = 0;

	JGeometry::TVec3<f32> marioPos2 = SMS_GetMarioPos();
	JGeometry::TVec3<f32> marioPos(marioPos2.x, 0.0f, marioPos2.z);

	for (int i = 0; i < 7; ++i) {
		if (unk74[i].distance(marioPos) < 350.0f)
			TBossManta::sEscapeFromMario = 1;
	}

	for (int i = 0; i < 2; ++i) {
		if (unk78[i].distance(marioPos) < 820.0f)
			TBossManta::sEscapeFromMario = 1;
	}
}

void TBossMantaManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TEnemyManager::perform(cue, graphics);
	if (cue & CUE_DRAW)
		drawMantaShadow(graphics);
	if (cue & CUE_MOVE) {
		unk88.update();
		unk90.update();
		updateMantaEscape();
	}
	for (int i = 0; i < 8; ++i)
		mCollisionSets[i]->update(cue, graphics);
}

void TBossMantaManager::setupEfbAlpha(JDrama::TGraphics* graphics)
{
	ReInitializeGX();

	Mtx44 proj;
	C_MTXOrtho(proj, (f32)SMSGetGameRenderHeight(), 0.0f, 0.0f,
	           (f32)SMSGetGameRenderWidth(), 0.0f, 1000.0f);
	GXSetProjection(proj, GX_ORTHOGRAPHIC);
	GXSetColorUpdate(GX_FALSE);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetDstAlpha(GX_TRUE, 0);
	GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);

	Mtx m;
	MTXIdentity(m);
	GXLoadPosMtxImm(m, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetCullMode(GX_CULL_NONE);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);

	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(0.0f, (f32)SMSGetGameRenderHeight(), -1.0f);
	GXPosition3f32((f32)SMSGetGameRenderWidth(), (f32)SMSGetGameRenderHeight(),
	               -1.0f);
	GXPosition3f32((f32)SMSGetGameRenderWidth(), 0.0f, -1.0f);
	GXPosition3f32(0.0f, 0.0f, -1.0f);
	GXEnd();

	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG,
	              GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG,
	              GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXColor matColor = (GXColor) { 0, 0, 0, 0x4 };
	GXSetChanMatColor(GX_COLOR0A0, matColor);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetDstAlpha(GX_FALSE, 0);
	GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetProjection(graphics->mProjMtx.mMtx, GX_PERSPECTIVE);
}

void TBossMantaManager::createEnemies(int num)
{
	if (num + getObjNum() > getCapacity())
		num = getCapacity() - getObjNum();

	if (unk38 != nullptr) {
		u8 limit = unk38->mSLActiveEnemyNum.get();
		if (num + getObjNum() > limit)
			num = limit - getObjNum();
	}

	if (num >= 0)
		for (int i = 0; i < num; ++i)
			createEnemy();
}

void TBossMantaManager::spawn(int gen, const JGeometry::TVec3<f32>& pos)
{
	const int counts[] = { 1, 2, 3, 4, 4 };
	const int& count   = counts[gen];

	f32 baseAngle = M_PI * (2.0f * MsRandF());
	for (int i = 0; i < count; ++i) {
		TBossManta* manta = (TBossManta*)getDeadEnemy();
		if (manta == nullptr)
			return;

		JGeometry::TVec3<f32> dir(0.0f, 0.0f, 1.0f);
		(void)&dir;

		f32 angle = baseAngle + (2.0f * (f32)i * M_PI) / (f32)count;

		TPosition3f mtx;
		mtx.setTrans(0.0f, 0.0f, 0.0f);
		mtx.setEularY(angle);
		mtx.mult(dir, dir);

		manta->unk170 = dir;
		manta->mPosition.set(pos);
		manta->initNthGeneration(gen);
	}
}

void TBossMantaManager::createEnemy()
{
	TSpineEnemy* enemy = createEnemyInstance();
	if (enemy != nullptr) {
		JDrama::TNameRefGen::search<TIdxGroupObj>("オブジェクトグループ")
		    ->getChildren()
		    .push_back(enemy);
		enemy->init(this);
	}
}

void TBossMantaManager::initAdditionalCollision()
{
	for (int i = 0; i < 8; ++i)
		mCollisionSets[i] = new TBossMantaAdditionalCollisionSet;
}

TBossMantaParams::TBossMantaParams(const char* name)
    : TSpineEnemyParams(name)
    , PARAM_INIT(mSLPolluteRadius, 100.0f)
    , PARAM_INIT(mSLDamageEffectNum, 3)
    , PARAM_INIT(mSLAppearDemoInitialZ, 7600.0f)
    , PARAM_INIT(mSLAppearDemoWalkSpeed, 3.0f)
    , PARAM_INIT(mSLMantaRed, 0xC4)
    , PARAM_INIT(mSLMantaGreen, 0x80)
    , PARAM_INIT(mSLMantaBlue, 0x5F)
    , PARAM_INIT(mSLMantaAlpha, 0x80)
    , PARAM_INIT(mSLAngryMantaRed, 0xD2)
    , PARAM_INIT(mSLAngryMantaGreen, 0x1E)
    , PARAM_INIT(mSLAngryMantaBlue, 0x5A)
    , PARAM_INIT(mSLAngryMantaAlpha, 0x80)
    , PARAM_INIT(mSLAttractorPower, 622.0f)
    , PARAM_INIT(mSLPusherPower, 23559.0f)
    , PARAM_INIT(mSLEscapeLookPoint, 1000.0f)
    , PARAM_INIT(mSLEscapeLookedPoint, 1000.0f)
    , PARAM_INIT(mSLEscapeRegion, 500.0f)
{
	TParams::load(mPrmPath);
}
