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
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <Strategic/Spine.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Map/PollutionManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <System/ParamInst.hpp>
#include <System/Resolution.hpp>
#include <MarioUtil/ReinitGX.hpp>
#include <GC2D/GCConsole2.hpp>
#include <System/MarDirector.hpp>
#include <System/FlagManager.hpp>
#include <MSound/MSoundBGM.hpp>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

DEFINE_NERVE(TNerveMantaMove, TLiveActor)
{
	TBossManta* self = (TBossManta*)spine->getBody();
	TGraphWeb* graph = self->getTracer()->getGraph();

	if (spine->getTime() == 0) {
		if (self->getMActor()->getAnmBck())
			self->getMActor()->getAnmBck()->initNormalMotionBlend();
		self->getMActor()->setBckFromIndex(3);

		// TODO: WIP - old-motion-blend anim pointer chain
		// (mMActorKeeper->unkC->unk2C) hits an undocumented
		// TMActorKeeper-internal field; oldAnm left null.
		J3DAnmTransform* oldAnm = nullptr;
		if (self->getMActor()->getAnmBck())
			self->getMActor()->getAnmBck()->setOldMotionBlendAnmPtr(oldAnm);
		if (self->getMActor()->getAnmBck())
			self->getMActor()->getAnmBck()->setMotionBlendRatio(0.5f);

		self->getMActor()->setFrameRate(
		    TBossManta::sFrameRate[self->unk18C] * SMSGetAnmFrameRate(), 0);

		JGeometry::TVec3<f32> pt
		    = graph->indexToPoint((int)((f32)graph->unk8 * MsRandF()));
		self->unk158 = pt.x + 1.0f * (gpMarioPos->x - pt.x);
		self->unk15C = pt.y + 1.0f * (gpMarioPos->y - pt.y);
		self->unk160 = pt.z + 1.0f * (gpMarioPos->z - pt.z);
	}

	if (self->unk18C < 3 && gpMSound->gateCheck(MSD_SE_BS_MANTA_DRAW_LV))
		MSoundSESystem::MSoundSE::startSoundActor(MSD_SE_BS_MANTA_DRAW_LV,
		                                          (const Vec*)&self->mPosition,
		                                          0, nullptr, 0, 4);

	if (self->unk18C == 5 && spine->getTime() % 96 == self->getInstanceIndex())
		gpMSound->startSoundSet(MSD_SE_BS_MANTA_ATTACK,
		                        (const Vec*)&self->mPosition, 0, 0.0f, 0, 0, 4);

	JGeometry::TVec3<f32> toTarget;
	toTarget.x = self->mPosition.x - self->unk158;
	toTarget.y = self->mPosition.y - self->unk15C;
	toTarget.z = self->mPosition.z - self->unk160;

	if (JGeometry::TUtil<f32>::sqrt(toTarget.dot(toTarget)) < 500.0f
	    || spine->getTime() % 150 == 0) {
		JGeometry::TVec3<f32> pt
		    = graph->indexToPoint((int)((f32)graph->unk8 * MsRandF()));

		static const f32 chaseSpeed[6] = { 1.0f, 0.8f, 0.8f, 0.6f, 0.8f, 1.0f };
		f32 speed = self->unk1A4 ? 1.0f : chaseSpeed[self->unk18C];

		pt.x += speed * (gpMarioPos->x - pt.x);
		pt.y += speed * (gpMarioPos->y - pt.y);
		pt.z += speed * (gpMarioPos->z - pt.z);
		self->unk158 = pt.x;
		self->unk15C = pt.y;
		self->unk160 = pt.z;
	}

	if (spine->getTime() % self->unk188 == 0)
		self->unk1A4 ^= 1;

	if (spine->getTime() % 100 == 0)
		self->updateAttractor();

	JGeometry::TVec3<f32> dir(self->unk164, self->unk168, self->unk16C);
	f32 lenSq = dir.dot(dir);
	if (lenSq == 0.0000038146973f) {
		dir.set(0.0f, 0.0f, 0.0f);
	} else {
		f32 invLen = 1.0f * JGeometry::TUtil<f32>::inv_sqrt(lenSq);
		dir.x *= invLen;
		dir.y *= invLen;
		dir.z *= invLen;
	}

	f32 speed2 = self->unk194;
	self->unk170 += dir.x * speed2;
	self->unk174 += dir.y * speed2;
	self->unk178 += dir.z * speed2;

	JGeometry::TVec3<f32> facing(self->unk170, self->unk174, self->unk178);
	f32 facingLenSq = facing.dot(facing);
	if (facingLenSq == 0.0000038146973f) {
		self->unk178 = 0.0f;
		self->unk174 = 0.0f;
		self->unk170 = 0.0f;
	} else {
		f32 invLen = 1.0f * JGeometry::TUtil<f32>::inv_sqrt(facingLenSq);
		self->unk170 *= invLen;
		self->unk174 *= invLen;
		self->unk178 *= invLen;
	}

	return FALSE;
}
DEFINE_NERVE(TNerveMantaHitWater, TLiveActor)
{
	TBossManta* self = (TBossManta*)spine->getBody();

	if (spine->getTime() == 0) {
		self->getMActor()->setBckFromIndex(1);
		self->getMActor()->setFrameRate(
		    TBossManta::sFrameRate[self->unk18C] * SMSGetAnmFrameRate(), 0);
		if (self->getMActor()->getAnmBck())
			self->getMActor()->getAnmBck()->setMotionBlendRatio(0.0f);

		static const u32 hitSounds[6]
		    = { MSD_SE_BS_MANTA_DAMAGE_1, MSD_SE_BS_MANTA_DAMAGE_2,
			    MSD_SE_BS_MANTA_DAMAGE_3, MSD_SE_BS_MANTA_DAMAGE_4,
			    MSD_SE_BS_MANTA_DAMAGE_5, MSD_SE_BS_MANTA_DAMAGE_5 };
		u32 snd = hitSounds[self->unk18C];
		if (gpMSound->gateCheck(snd))
			MSoundSESystem::MSoundSE::startSoundActor(
			    snd, (const Vec*)&self->mPosition, 0, nullptr, 0, 4);
	}

	int effectCount
	    = ((TBossMantaParams*)self->getSaveParam())->mSLDamageEffectNum.get();

	const int particles[6][3] = {
		{ 0x1CE, 0x1CF, 0x1D0 }, { 0x1CB, 0x1CC, 0x1CD }, { 0x1C9, -1, 0x1CA },
		{ 0x1C7, -1, 0x1C8 },    { 0x1C7, -1, 0x1C8 },    { 0x1C7, -1, 0x1C8 },
	};

	for (int j = 0; j < 3; ++j) {
		if (particles[self->unk18C][j] > 0) {
			for (int i = 0; i < effectCount; ++i) {
				gpMarioParticleManager->emitAndBindToPosPtr(
				    particles[self->unk18C][j], &self->unk17C, 1,
				    (u8*)self + i * sizeof(TBossManta));
				if (j == 2)
					break;
			}
		}
	}

	if (self->checkCurAnmEnd(0)) {
		self->unk150 = 0.5f;
		self->unk154 = 0;
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
		static const int particles[] = { 0xFC, 0xFB, 0xFA, 0xF9 };
		gpMarioParticleManager->emitAndBindToPosPtr(particles[self->unk18C],
		                                            &self->unk17C, 0, self);
		static const u32 sounds[] = { 0x8994, 0x8995, 0x8996, 0x8997 };
		u32 snd                   = sounds[self->unk18C];
		if (gpMSound->gateCheck(snd))
			MSoundSESystem::MSoundSE::startSoundActor(
			    snd, (const Vec*)&self->mPosition, 0, nullptr, 0, 4);
		((TBossMantaManager*)self->getManager())
		    ->spawn(self->unk18C + 1, self->mPosition);
	}

	if (spine->getTime() == 0x1E)
		return TRUE;
	return FALSE;
}
DEFINE_NERVE(TNerveMantaDeath, TLiveActor)
{
	TBossManta* self = (TBossManta*)spine->getBody();

	if (spine->getTime() == 0) {
		self->getMActor()->setBckFromIndex(0);
		self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 0);
		MActorAnmBck* bck = self->getMActor()->getAnmBck();
		if (bck != nullptr)
			bck->setMotionBlendRatio(0.0f);
	}

	if (self->checkCurAnmEnd(0)) {
		gpMarioParticleManager->emitAndBindToPosPtr(0xF8, &self->unk17C, 0,
		                                            self);
		if (gpMSound->gateCheck(MSD_SE_BS_MANTA_VANISH))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_BS_MANTA_VANISH, (const Vec*)&self->mPosition, 0,
			    nullptr, 0, 4);
		self->kill();
	}

	return FALSE;
}
DEFINE_NERVE(TNerveMantaAppearDemo, TLiveActor)
{
	TBossManta* self         = (TBossManta*)spine->getBody();
	TBossMantaParams* params = (TBossMantaParams*)self->getSaveParam();

	if (spine->getTime() == 0) {
		self->mPosition.x = 0.0f;
		self->mPosition.y = 0.0f;
		self->mPosition.z = params->mSLAppearDemoInitialZ.get();
		self->unk170      = 0.0f;
		self->unk174      = 0.0f;
		self->unk178      = -1.0f;
		self->unk158      = 0.0f;
		self->unk15C      = 0.0f;
		self->unk160      = -10000.0f;

		if (self->getMActor()->getAnmBck())
			self->getMActor()->getAnmBck()->initNormalMotionBlend();
		self->getMActor()->setBckFromIndex(3);

		// TODO: WIP - old-motion-blend anim pointer chain
		// (mMActorKeeper->unkC->unk2C) hits an undocumented
		// TMActorKeeper-internal field; oldAnm left null.
		J3DAnmTransform* oldAnm = nullptr;
		if (self->getMActor()->getAnmBck())
			self->getMActor()->getAnmBck()->setOldMotionBlendAnmPtr(oldAnm);
		if (self->getMActor()->getAnmBck())
			self->getMActor()->getAnmBck()->setMotionBlendRatio(0.5f);

		self->getMActor()->setFrameRate(
		    TBossManta::sFrameRate[self->unk18C] * SMSGetAnmFrameRate(), 0);
	}

	if (spine->getTime() == 0x2D0)
		MSBgm::startBGM(0x8001002B);

	if (spine->getTime() == 0x690)
		return TRUE;

	if (gpMSound->gateCheck(MSD_SE_BS_MANTA_DRAW_DEMO))
		MSoundSESystem::MSoundSE::startSoundActor(MSD_SE_BS_MANTA_DRAW_DEMO,
		                                          (const Vec*)&self->mPosition,
		                                          0, nullptr, 0, 4);

	self->unk190 = params->mSLAppearDemoWalkSpeed.get();
	return FALSE;
}

// UNUSED (AttackMario__9@unnamed@FP9THitActor, mario.MAP, 0x144, fully
// inlined at every call site) - moveObject() and
// TBossMantaAdditionalCollision::perform() both duplicate this exact
// dir-normalize + SMS_SendMessageToMario loop, which is almost certainly
// this anonymous-namespace helper. A direct extraction regressed both
// callers (MWCC didn't fold the call away as it does in retail), so the
// duplication is kept inline for now. TODO: figure out what retail did
// differently to get this to inline away.

TBossManta::TBossManta(const char* name)
    : TSpineEnemy(name)
{
	unk158   = 0.0f;
	unk15C   = 0.0f;
	unk160   = 0.0f;
	unk164   = 0.0f;
	unk168   = 0.0f;
	unk16C   = 0.0f;
	unk170   = 0.0f;
	unk174   = 0.0f;
	unk178   = 1.0f;
	unk17C.x = 0.0f;
	unk17C.y = 0.0f;
	unk17C.z = 0.0f;
	unk188   = 0;
	unk18C   = 0;
	unk190   = 0.0f;
	unk194   = 0.0f;
	unk198   = 0.0f;
	unk19C   = 0;
	unk1A0   = 0;
}
void TBossManta::drawObject(JDrama::TGraphics*) { }
void TBossManta::reset() { }
BOOL TBossManta::getIntoGraphVec(JGeometry::TVec3<f32>* out)
{
	TGraphWeb* graph = getTracer()->getGraph();
	for (int idx = 0; idx <= 12; ++idx) {
		JGeometry::TVec3<f32> a = graph->indexToPoint(idx);
		JGeometry::TVec3<f32> b = graph->indexToPoint(idx + 1);
		JGeometry::TVec3<f32> c = graph->indexToPoint(idx);

		f32 ax = a.x - mPosition.x;
		f32 az = a.z - mPosition.z;
		f32 dx = c.x - b.x;
		f32 dz = c.z - b.z;

		if (ax * dz - az * dx < 0.0f) {
			out->x = dz;
			out->y = 0.0f;
			out->z = -dx;
			if (out->dot(*out) == 0.0000038146973f)
				out->set(0.0f, 0.0f, 0.0f);
			else
				out->setLength(1.0f);
			return TRUE;
		}
	}
	return FALSE;
}
void TBossManta::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("manta.bmd", 0);
	getTracer()->setGraph(gpConductor->getGraphByName("main"));

	// TODO: unconfirmed - the retail source checks something off
	// getSaveParam() to seed mHitPoints; defaulted to the params-absent
	// branch's value (1) rather than guess the condition.
	mHitPoints = 1;

	mHeadHeight = 5000.0f;
	initHitActor(0x08000004, 1, 0x80000000, 0.0f, 0.0f, 0.0f, 0.0f);
	unk170 = 0.0f;
	unk174 = 0.0f;
	unk178 = 1.0f;
	calcRootMatrix();
	kill();

	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);

	if (getInstanceIndex() == 0) {
		JUTNameTab* jointNames = getModel()->getModelData()->getJointName();
		sCenterJointIndex      = jointNames->getIndex("center");
		sBodyJointIndex        = jointNames->getIndex("jnt_body");
		sRwingJointIndex       = jointNames->getIndex("jnt_Rwing2");
		sLwingJointIndex       = jointNames->getIndex("jnt_Lwing2");

		TBossMantaManager* manager = (TBossMantaManager*)mManager;
		for (int i = 0; i < 8; ++i)
			manager->mCollisionSets[i] = new TBossMantaAdditionalCollisionSet();
	}

	mLiveFlag |= LIVE_FLAG_UNK8;
	mLiveFlag &= ~0x100;
	unk1A4 = 0;
	unk150 = 0.5f;
}
void TBossManta::moveObject()
{
	// TODO: WIP - pollution stamp + push-away loop; vector codegen needs work.
	TLiveActor::moveObject();

	if (isPolluting())
		gpPollution->stamp(1, mPosition.x, mPosition.y, mPosition.z,
		                   getPolluteRadius());

	for (int i = 0; i < mColCount; ++i) {
		if (mCollisions[i]->getActorType() != 0x80000001)
			continue;
		JGeometry::TVec3<f32> dir;
		dir.x = gpMarioPos->x - mPosition.x;
		dir.y = gpMarioPos->y - mPosition.y;
		dir.z = gpMarioPos->z - mPosition.z;
		if (dir.dot(dir) == 0.0000038146973f)
			dir.set(0.0f, 0.0f, 0.0f);
		else
			dir.setLength(1.0f);
		dir.x *= 2.0f;
		dir.y = dir.y * 2.0f + 1.0f;
		dir.z *= 2.0f;
		SMS_SendMessageToMario(this, 0xE);
		SMS_SendMessageToMario(this, 7);
		SMS_ThrowMario(dir, 60.0f);
	}
}
BOOL TBossManta::collidedWithWater()
{
	BOOL allowed;
	if (unk1A0 > 0) {
		allowed = FALSE;
	} else {
		allowed = TRUE;
		if (mSpine->getLatestNerve() != &TNerveMantaMove::theNerve()) {
			if (mSpine->getLatestNerve() != &TNerveMantaHitWater::theNerve())
				allowed = FALSE;
		}
	}

	if (!allowed)
		return FALSE;

	BOOL isHitWater
	    = mSpine->getLatestNerve() == &TNerveMantaHitWater::theNerve();

	static const int hitCounts[6] = { 16, 8, 4, 2, 1, 1 };
	if (unk19C < hitCounts[unk18C]) {
		unk1A0 = 0x1E;
		if (!isHitWater)
			mSpine->setNext(&TNerveMantaHitWater::theNerve());
		unk19C++;

		if (unk19C == hitCounts[unk18C]) {
			if (unk18C >= 4)
				mSpine->pushAfterCurrent(&TNerveMantaDeath::theNerve());
			else
				mSpine->pushAfterCurrent(&TNerveMantaSpawn::theNerve());
		} else if (mSpine->getVertebraeCount() < 7) {
			mSpine->pushAfterCurrent(&TNerveMantaMove::theNerve());
		}
	}
	return TRUE;
}
BOOL TBossManta::receiveMessage(THitActor* sender, u32 message)
{
	if (message == 0xF
	    && gpModelWaterManager
	               ->mParticleFlagSOA[((TWaterHitActor*)sender)->unk68]
	           & 0x40)
		return collidedWithWater();
	return FALSE;
}
f32 TBossManta::sScale[] = { 20.0f, 10.0f, 5.0f, 2.0f, 1.0f, 1.0f };

void TBossManta::initNthGeneration(int gen)
{
	const f32 heights[6] = { 10.0f, 5.0f, 1.0f, 0.42f, 0.42f, 0.42f };

	unk18C     = gen;
	f32 s      = sScale[unk18C];
	mScaling.z = s;
	mScaling.x = s;
	mScaling.y = heights[unk18C];
	unk19C     = 0;
	unk150     = 0.5f;
	unk154     = 0x31;

	switch (unk18C) {
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

	if (unk18C <= 2) {
		setHitParams(0.0f, 0.0f, 0.0f, 0.0f);
	} else {
		f32 r = 80.0f * mScaling.x;
		setHitParams(r, 100.0f, r, 100.0f);
	}

	offHitFlag(HIT_FLAG_NO_COLLISION);
	offLiveFlag(LIVE_FLAG_DEAD);

	if (unk18C <= 2) {
		TBossMantaManager* manager = (TBossMantaManager*)mManager;
		for (int i = 0; i < 8; ++i) {
			if (!manager->mCollisionSets[i]->isUsed()) {
				manager->mCollisionSets[i]->adapt(this);
				return;
			}
		}
	}
}
void TBossManta::control()
{
	if (unk1A0 > 0)
		unk1A0--;

	if (getMActor()->checkCurBckFromIndex(3)) {
		JGeometry::TVec3<f32> f(unk164, unk168, unk16C);
		if (f.dot(f) == 0.0000038146973f)
			f.set(0.0f, 0.0f, 0.0f);
		else
			f.setLength(1.0f);

		static const f32 blend[]
		    = { 0.005f, 0.008f, 0.01f, 0.03f, 0.05f, 0.05f };
		f32 b    = blend[unk18C];
		f32 turn = -(0.4f * (unk178 * f.x - unk170 * f.z) - 0.5f);
		unk150   = (1.0f - b) * unk150 + b * turn;

		const f32 animBlend[50] = {
			0.0f,    0.046f,  0.107f,  0.177f,  0.227f,  0.256f,  0.273f,
			0.279f,  0.274f,  0.26f,   0.237f,  0.209f,  0.175f,  0.138f,
			0.1f,    0.061f,  0.024f,  -0.011f, -0.043f, -0.07f,  -0.092f,
			-0.108f, -0.12f,  -0.126f, -0.127f, -0.123f, -0.115f, -0.104f,
			-0.09f,  -0.074f, -0.057f, -0.04f,  -0.022f, -0.005f, 0.01f,
			0.024f,  0.035f,  0.045f,  0.051f,  0.056f,  0.058f,  0.057f,
			0.055f,  0.051f,  0.046f,  0.039f,  0.031f,  0.023f,  0.015f,
			0.007f,
		};

		f32 blendedFrame = unk150 + animBlend[unk154];
		f32 ratio;
		if (blendedFrame < 0.0f)
			ratio = 0.0f;
		else if (blendedFrame > 1.0f)
			ratio = 1.0f;
		else
			ratio = blendedFrame;

		if (getMActor()->getAnmBck())
			getMActor()->getAnmBck()->setMotionBlendRatio(ratio);
	} else {
		if (getMActor()->getAnmBck())
			getMActor()->getAnmBck()->setMotionBlendRatio(0.0f);
	}

	mVelocity.x = unk170 * unk190;
	mVelocity.y = unk174 * unk190;
	mVelocity.z = unk178 * unk190;

	TLiveActor::control();
}
void TBossManta::calcRootMatrix()
{
	if (unk154 < 0x31)
		unk154++;

	Mtx m;
	m[0][3] = mPosition.x;
	m[1][3] = mPosition.y;
	m[2][3] = mPosition.z;
	m[0][0] = 1.0f * unk178 - 0.0f * unk174;
	m[1][0] = 0.0f * unk170 - 0.0f * unk178;
	m[2][0] = -(1.0f * unk170 - 0.0f * unk174);
	m[0][1] = 0.0f;
	m[1][1] = 1.0f;
	m[2][1] = 0.0f;
	m[0][2] = unk170;
	m[1][2] = unk174;
	m[2][2] = unk178;

	J3DModel* model    = getModel();
	MtxPtr joint       = model->mNodeMatrices[sCenterJointIndex];
	unk17C.x           = joint[0][3];
	unk17C.y           = mPosition.y;
	unk17C.z           = joint[2][3];
	(Vec&)model->unk14 = (Vec&)mScaling;
	MTXCopy(m, model->unk20);
}
BOOL TBossManta::isPolluting()
{
	const u8 pollute[6] = { 1, 1, 1, 1, 1, 1 };
	return pollute[unk18C];
}
f32 TBossManta::getPolluteRadius()
{
	if (unk18C < 4) {
		if (unk18C < 0)
			return 0.0f;
		return ((TBossMantaParams*)getSaveParam())->mSLPolluteRadius.get()
		       * mScaling.x;
	}
	if (unk18C < 6)
		return 100.0f;
	return 0.0f;
}
void TBossManta::updateAttractor()
{
	TBossMantaParams* params = (TBossMantaParams*)getSaveParam();

	JGeometry::TVec3<f32> force;
	force.x = mPosition.x - unk158;
	force.y = mPosition.y - unk15C;
	force.z = mPosition.z - unk160;
	force.y = 0.0f;
	if (force.dot(force) == 0.0000038146973f)
		force.set(0.0f, 0.0f, 0.0f);
	else
		force.setLength(1.0f);
	force.x *= params->mSLAttractorPower.get();
	force.y *= params->mSLAttractorPower.get();
	force.z *= params->mSLAttractorPower.get();

	JGeometry::TVec3<f32> facing(unk170, unk174, unk178);
	facing.x *= params->mSLPusherPower.get();
	facing.y *= params->mSLPusherPower.get();
	facing.z *= params->mSLPusherPower.get();

	JGeometry::TVec3<f32> selfPos;
	selfPos.x = mPosition.x + facing.x;
	selfPos.y = mPosition.y + facing.y;
	selfPos.z = mPosition.z + facing.z;

	TEnemyManager* manager = (TEnemyManager*)mManager;
	for (int i = 0; i < manager->getActiveObjNum(); ++i) {
		TBossManta* other = (TBossManta*)manager->getObj(i);
		if (checkLiveFlag(LIVE_FLAG_DEAD)
		    || other->checkLiveFlag(LIVE_FLAG_DEAD)
		    || other->getInstanceIndex() == getInstanceIndex())
			continue;

		JGeometry::TVec3<f32> otherFacing(other->unk170, other->unk174,
		                                  other->unk178);
		otherFacing.x *= params->mSLPusherPower.get();
		otherFacing.y *= params->mSLPusherPower.get();
		otherFacing.z *= params->mSLPusherPower.get();

		JGeometry::TVec3<f32> otherPos;
		otherPos.x = other->mPosition.x + otherFacing.x;
		otherPos.y = other->mPosition.y + otherFacing.y;
		otherPos.z = other->mPosition.z + otherFacing.z;

		JGeometry::TVec3<f32> delta;
		delta.x = selfPos.x - otherPos.x;
		delta.y = selfPos.y - otherPos.y;
		delta.z = selfPos.z - otherPos.z;
		delta.y = 0.0f;

		f32 distSq = delta.dot(delta);
		f32 dist   = JGeometry::TUtil<f32>::sqrt(distSq);
		if (dist > 0.1f) {
			dist = JGeometry::TUtil<f32>::sqrt(distSq);
			if (dist < params->mSLEscapeRegion.get()) {
				if (distSq == 0.0000038146973f)
					delta.set(0.0f, 0.0f, 0.0f);
				else
					delta.setLength(1.0f);
				f32 pushScale = params->mSLPusherPower.get()
				                / JGeometry::TUtil<f32>::sqrt(delta.dot(delta));
				delta.x *= pushScale;
				delta.y *= pushScale;
				delta.z *= pushScale;
				force.x += delta.x;
				force.y += delta.y;
				force.z += delta.z;
			}
		}
	}

	JGeometry::TVec3<f32> toMario;
	toMario.x     = mPosition.x - gpMarioPos->x;
	toMario.y     = mPosition.y - gpMarioPos->y;
	toMario.z     = mPosition.z - gpMarioPos->z;
	toMario.y     = 0.0f;
	f32 marioDist = JGeometry::TUtil<f32>::sqrt(toMario.dot(toMario));
	if ((sEscapeFromMario != 0 || unk18C == 4) && marioDist < 6000.0f) {
		force.x += toMario.x;
		force.y += toMario.y;
		force.z += toMario.z;
	}

	JGeometry::TVec3<f32> graphVec;
	if (getIntoGraphVec(&graphVec)) {
		graphVec.x *= 10000.0f;
		graphVec.y *= 10000.0f;
		graphVec.z *= 10000.0f;
		force.x += graphVec.x;
		force.y += graphVec.y;
		force.z += graphVec.z;
	}

	unk164 = force.x;
	unk168 = force.y;
	unk16C = force.z;
}

int TBossManta::sCenterJointIndex;
int TBossManta::sRwingJointIndex;
int TBossManta::sLwingJointIndex;
u8 TBossManta::sEscapeFromMario;
f32 TBossManta::sFrameRate[6] = { 0.3f, 0.5f, 1.2f, 2.0f, 5.0f, 5.3f };

static JAISound* sDefeatSE;

void TBossMantaManager::TMantaBattleState::update()
{
	switch (unk4) {
	case 0:
		if (TFlagManager::getInstance()->getBool(0x50007)) {
			gpMarDirector->fireStartDemoCamera("sirena_manta", nullptr, -1,
			                                   0.0f, true, nullptr, 0, nullptr,
			                                   JDrama::TFlagT<u16>(0));
			((TBossManta*)unk0->getObj(0))->initNthGeneration(0);
			MSBgm::stopTrackBGMs(7, 10);
			unk4++;
		}
		break;
	case 1: {
		bool allMaxGen = true;
		for (int i = 0; i < unk0->getActiveObjNum(); ++i) {
			TBossManta* m = (TBossManta*)unk0->getObj(i);
			if (m->checkLiveFlag(LIVE_FLAG_DEAD))
				continue;
			if (m->unk18C != 4) {
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
			unk4++;
		}
		break;
	}
	case 2: {
		bool victory = true;
		for (int i = 0; i < unk0->getActiveObjNum(); ++i) {
			TBossManta* m = (TBossManta*)unk0->getObj(i);
			if (m->unk18C != 5)
				continue;
			if (m->checkLiveFlag(LIVE_FLAG_DEAD))
				continue;
			victory = false;
			break;
		}
		if (victory) {
			MSBgm::stopTrackBGMs(7, 10);
			sDefeatSE = nullptr;
			if (gpMSound->gateCheck(0x898F))
				MSoundSESystem::MSoundSE::startSoundActor(0x898F, nullptr, 0,
				                                          &sDefeatSE, 0, 4);
			unk4++;
		}
		break;
	}
	case 3:
		if (sDefeatSE == nullptr) {
			// TODO: WIP - post-boss event flag trigger (name lookup + flag
			// set, exact field/type unconfirmed).
			unk4++;
		}
		break;
	default:
		break;
	}
}
void TBossMantaManager::TMantaMessageState::update()
{
	switch (unk4) {
	case 0:
		if (unk0->getObj(0)->mSpine->getLatestNerve()
		    == &TNerveMantaSpawn::theNerve()) {
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
		if (unk0->unk88.unk4 == 2) {
			gpMarDirector->getConsole()->startAppearBalloon(0xE000E, true);
			unk4++;
		}
		break;
	default:
		break;
	}
}

// UNUSED (__ct__29TBossMantaAdditionalCollisionFPCc, mario.MAP, 0x114,
// fully inlined at its only call site below).
TBossMantaAdditionalCollision::TBossMantaAdditionalCollision(const char* name)
    : THitActor(name)
    , unk68(nullptr)
{
	initHitActor(0x08000004, 1, 0x80000000, 0.0f, 0.0f, 0.0f, 0.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);

	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);
}

TBossMantaAdditionalCollisionSet::TBossMantaAdditionalCollisionSet()
{
	unkC = nullptr;
	for (int i = 0; i < 3; ++i)
		(&unk0)[i] = new TBossMantaAdditionalCollision("マンタ追加コリジョン");
}
BOOL TBossMantaAdditionalCollisionSet::isUsed()
{
	return unkC != nullptr && !unkC->checkLiveFlag(LIVE_FLAG_DEAD);
}
void TBossMantaAdditionalCollisionSet::adapt(TBossManta* manta)
{
	unkC = manta;
	unk0->setHitParams(54.0f * unkC->mScaling.x, 100.0f,
	                   54.0f * unkC->mScaling.x, 100.0f);
	unk4->setHitParams(26.0f * unkC->mScaling.x, 100.0f,
	                   26.0f * unkC->mScaling.x, 100.0f);
	unk8->setHitParams(26.0f * unkC->mScaling.x, 100.0f,
	                   26.0f * unkC->mScaling.x, 100.0f);
	unk0->unk68 = unkC;
	unk4->unk68 = unkC;
	unk8->unk68 = unkC;
}
void TBossMantaAdditionalCollisionSet::update(u32 flags,
                                              JDrama::TGraphics* graphics)
{
	if (unkC != nullptr) {
		if (unkC->mLiveFlag & 1) {
			unkC = nullptr;
			return;
		}
		for (int i = 0; i < 3; ++i)
			(&unk0)[i]->perform(flags, graphics);

		MtxPtr centerMtx
		    = unkC->getModel()->mNodeMatrices[TBossManta::sCenterJointIndex];
		f32 centerX = centerMtx[0][3];
		f32 centerY = centerMtx[1][3];
		f32 centerZ = centerMtx[2][3];

		MtxPtr bodyMtx
		    = unkC->getModel()->mNodeMatrices[TBossManta::sBodyJointIndex];
		f32 bodyX = bodyMtx[0][3];
		f32 bodyY = bodyMtx[1][3];
		f32 bodyZ = bodyMtx[2][3];

		MtxPtr rwingMtx
		    = unkC->getModel()->mNodeMatrices[TBossManta::sRwingJointIndex];
		f32 rwingX = rwingMtx[0][3];
		f32 rwingY = rwingMtx[1][3];
		f32 rwingZ = rwingMtx[2][3];

		MtxPtr lwingMtx
		    = unkC->getModel()->mNodeMatrices[TBossManta::sLwingJointIndex];
		f32 lwingX = lwingMtx[0][3];
		f32 lwingY = lwingMtx[1][3];
		f32 lwingZ = lwingMtx[2][3];

		unk0->mPosition.x = -0.15f * (bodyX - centerX) + centerX;
		unk0->mPosition.y = -0.15f * (bodyY - centerY) + centerY;
		unk0->mPosition.z = -0.15f * (bodyZ - centerZ) + centerZ;

		unk4->mPosition.x = 0.75f * (rwingX - centerX) + centerX;
		unk4->mPosition.y = 0.75f * (rwingY - centerY) + centerY;
		unk4->mPosition.z = 0.75f * (rwingZ - centerZ) + centerZ;

		unk8->mPosition.x = 0.75f * (lwingX - centerX) + centerX;
		unk8->mPosition.y = 0.75f * (lwingY - centerY) + centerY;
		unk8->mPosition.z = 0.75f * (lwingZ - centerZ) + centerZ;
	}
}
BOOL TBossMantaAdditionalCollision::receiveMessage(THitActor* sender,
                                                   u32 message)
{
	if (unk68 == nullptr)
		return FALSE;
	return unk68->receiveMessage(sender, message);
}
void TBossMantaAdditionalCollision::perform(u32 flags,
                                            JDrama::TGraphics* graphics)
{
	// TODO: WIP - push-away response; vector-op codegen needs work.
	THitActor::perform(flags, graphics);
	if (flags & 1) {
		for (int i = 0; i < mColCount; ++i) {
			if (mCollisions[i]->getActorType() != 0x80000001)
				continue;
			JGeometry::TVec3<f32> dir;
			dir.x = gpMarioPos->x - mPosition.x;
			dir.y = gpMarioPos->y - mPosition.y;
			dir.z = gpMarioPos->z - mPosition.z;
			if (dir.dot(dir) == 0.0000038146973f)
				dir.set(0.0f, 0.0f, 0.0f);
			else
				dir.setLength(1.0f);
			dir.x *= 2.0f;
			dir.y = dir.y * 2.0f + 1.0f;
			dir.z *= 2.0f;
			SMS_SendMessageToMario(this, 0xE);
			SMS_SendMessageToMario(this, 7);
			SMS_ThrowMario(dir, 60.0f);
		}
	}
}

TBossMantaManager::TBossMantaManager(const char* name)
    : TEnemyManager(name)
{
	unk7C      = 0;
	unk80      = 0;
	unk88.unk0 = this;
	unk88.unk4 = 0;
	unk90.unk0 = this;
	unk90.unk4 = 0;
	unk7C = new (0x20) u8[SMSGetGameRenderWidth() * SMSGetGameRenderHeight()];
	unk74 = new JGeometry::TVec3<f32>[7];
	unk78 = new JGeometry::TVec3<f32>[2];
}
void TBossMantaManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TBossMantaParams("/enemy/bossmanta.prm");
	TEnemyManager::load(stream);
}
void TBossMantaManager::loadAfter()
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

	for (int i = 0; i < 7; ++i) {
		char name[0x40];
		snprintf(name, 0x40, "palmOugi %d", i);
		TLiveActor* palm = JDrama::TNameRefGen::search<TLiveActor>(name);
		unk74[i].x       = palm->mPosition.x;
		unk74[i].y       = 0.0f;
		unk74[i].z       = palm->mPosition.z;
	}

	unk78[0].x = 1549.0f;
	unk78[0].y = 0.0f;
	unk78[0].z = -435.0f;
	unk78[1].x = -1457.0f;
	unk78[1].y = 0.0f;
	unk78[1].z = -533.0f;

	unk88.unk4                   = 0;
	unk90.unk4                   = 0;
	unk84                        = 0;
	TBossManta::sEscapeFromMario = 0;

	TEnemyManager::loadAfter();
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
TSpineEnemy* TBossMantaManager::createEnemyInstance()
{
	return new TBossManta("マンタ");
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
	           (f32)SMSGetGameRenderWidth(), 0.0f, 1.0f);
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

	if (unk88.unk4 == 2) {
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
	GXSetTevKColor(GX_KCOLOR0, unk80);

	GXLoadTexObj(&texObj, GX_TEXMAP0);
	GXSetDstAlpha(GX_TRUE, 0xFF);
	GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
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
	PSMTXIdentity(m);
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

	f32 marioX = gpMarioPos->x;
	f32 marioZ = gpMarioPos->z;

	int count                = 7;
	JGeometry::TVec3<f32>* p = unk74;
	do {
		f32 dx = p->x - marioX;
		f32 dz = p->z - marioZ;
		if (JGeometry::TUtil<f32>::sqrt(dx * dx + dz * dz) < 350.0f)
			TBossManta::sEscapeFromMario = 1;
		++p;
	} while (--count);

	count = 2;
	p     = unk78;
	do {
		f32 dx = p->x - marioX;
		f32 dz = p->z - marioZ;
		if (JGeometry::TUtil<f32>::sqrt(dx * dx + dz * dz) < 820.0f)
			TBossManta::sEscapeFromMario = 1;
		++p;
	} while (--count);
}
void TBossMantaManager::perform(u32 flags, JDrama::TGraphics* graphics)
{
	TEnemyManager::perform(flags, graphics);
	if (flags & 8)
		drawMantaShadow(graphics);
	if (flags & 1) {
		unk88.update();
		unk90.update();
		updateMantaEscape();
	}
	for (int i = 0; i < 8; ++i)
		mCollisionSets[i]->update(flags, graphics);
}
static const GXColor sMantaEfbMatColor = { 0, 0, 0, 0xFF };

void TBossMantaManager::setupEfbAlpha(JDrama::TGraphics* graphics)
{
	ReInitializeGX();

	Mtx44 proj;
	C_MTXOrtho(proj, (f32)SMSGetGameRenderHeight(), 0.0f, 0.0f,
	           (f32)SMSGetGameRenderWidth(), 0.0f, 1.0f);
	GXSetProjection(proj, GX_ORTHOGRAPHIC);
	GXSetColorUpdate(GX_FALSE);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetDstAlpha(GX_TRUE, 0);
	GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);

	Mtx m;
	PSMTXIdentity(m);
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
	GXColor matColor = sMantaEfbMatColor;
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
	for (int i = 0; i < num; ++i) {
		TSpineEnemy* enemy = createEnemyInstance();
		if (enemy != nullptr) {
			JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
			    ->getChildren()
			    .push_back(enemy);
			enemy->init(this);
		}
	}
}
void TBossMantaManager::spawn(int gen, const JGeometry::TVec3<f32>& pos)
{
	// TODO: logic correct (ring spawn of counts[gen] manta), but the
	// Y-rotation matrix-mult codegen shape needs work.
	static const int counts[] = { 1, 2, 3, 4, 4 };

	int count     = counts[gen];
	f32 baseAngle = 3.1415927f * (2.0f * MsRandF());
	for (int i = 0; i < count; ++i) {
		TBossManta* manta = (TBossManta*)getDeadEnemy();
		if (manta != nullptr) {
			JGeometry::TVec3<f32> dir(0.0f, 0.0f, 1.0f);
			f32 angle = baseAngle + (2.0f * (f32)i * 3.1415927f) / (f32)count;
			Mtx m;
			MTXRotRad(m, 'y', angle);
			MTXMultVec(m, &dir, &dir);
			manta->unk170    = dir.x;
			manta->unk174    = dir.y;
			manta->unk178    = dir.z;
			manta->mPosition = pos;
			manta->initNthGeneration(gen);
		}
	}
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
