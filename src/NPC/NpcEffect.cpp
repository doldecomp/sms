#include <NPC/NpcBase.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/EffectUtil.hpp>
#include <MoveBG/MapObjWave.hpp>
#include <Map/MapData.hpp>
#include <NPC/NpcParts.hpp>

// rogue
#include <M3DUtil/InfectiousStrings.hpp>
static const char dummy[] = "__ROOT_JOINT__";

void TBaseNPC::setHappyEffectMtxPtr_(const JUTNameTab* tab)
{
	const char* monte    = "koshi_null";
	const char* mare     = "koshi";
	const char* kinoppio = "jnt_body";

	const char* jointName;
	if (isNormalMonte())
		jointName = monte;
	else if (isNormalMare())
		jointName = mare;
	else if (mActorType == 0x4000016)
		jointName = kinoppio;
	else
		jointName = nullptr;

	if (jointName != nullptr)
		mHappyEffectMtxPtr = getModel()->getAnmMtx(tab->getIndex(jointName));
}

void TBaseNPC::setNoteEffectMtxPtr_(const JUTNameTab* tab)
{
	const char* noseJnt  = "nose_jnt";
	const char* mouthJnt = "kuchi";

	const char* jointName = nullptr;

	switch (mActorType) {
	case 0x4000009:
		jointName = noseJnt;
		break;

	case 0x4000012:
		jointName = mouthJnt;
		break;
	}

	if (jointName)
		mNoteEffectMtxPtr = getModel()->getAnmMtx(tab->getIndex(jointName));
}

void TBaseNPC::setPollutionEffectMtxPtr_(const JUTNameTab* tab)
{
	const char* koshiNullJoint = "koshi_null";
	const char* bodyJoint      = "jnt_body";
	const char* leftFootJoint  = "footL_jnt";
	const char* rightFootJoint = "footR_jnt";
	const char* koshiJoint     = "koshi";

	const char* pcVar5;
	if (isNormalMonte()) {
		unk200 = getModel()->getAnmMtx(tab->getIndex(leftFootJoint));
		unk204 = getModel()->getAnmMtx(tab->getIndex(rightFootJoint));
		pcVar5 = koshiNullJoint;
	} else if (isNormalMare()) {
		pcVar5 = koshiJoint;
	} else if (mActorType == 0x4000016) {
		pcVar5 = bodyJoint;
	} else {
		pcVar5 = nullptr;
	}

	if (pcVar5)
		mPollutionEffectMtxPtr = getModel()->getAnmMtx(tab->getIndex(pcVar5));
}

void TBaseNPC::setSmokeEffectMtxPtr_(bool param_1)
{
	J3DModel* model;
	const char* pcVar3;
	if (param_1) {
		model  = unk168->getPartsMActor(0xB, 0)->getModel();
		pcVar3 = "ef_null";
	} else {
		model  = getModel();
		pcVar3 = "yashi_jnt";
	}
	mSmokeEffectMtxPtr = model->getAnmMtx(
	    model->getModelData()->getJointName()->getIndex(pcVar3));
}

static bool IsCheckPassFrame(J3DFrameCtrl* param_1, const f32* param_2)
{
	bool result = false;
	for (int i = 0; param_2[i] >= 0.0f; ++i) {
		if (param_1->checkPass(param_2[i])) {
			result = true;
			break;
		}
	}
	return result;
}

JGeometry::TVec3<f32> TBaseNPC::getEffectScale_() const
{
	switch (mActorType) {
	case 0x4000016:
	case 0x4000017:
		return JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f);
	}
	return mInitialScale;
}

void TBaseNPC::emitSinkEffect_()
{
	bool bVar1 = true;
	JGeometry::TVec3<f32> pos(mPosition.x, unk1C4, mPosition.z);
	if (checkLiveFlag(LIVE_FLAG_UNK800000))
		bVar1 = false;
	SMS_EmitSinkInPollutionEffect(pos, mGroundPlane->getNormal(), bVar1);
}

void TBaseNPC::emitHappyEffect_()
{
	JGeometry::TVec3<f32> scale = getEffectScale_();
	scale *= mPtrSaveNormal->mSLCleanEffectScale.get();
	if (isNormalMonte()) {
		SMS_EasyEmitParticle(PARTICLE_MS_MNT_KIRA, mHappyEffectMtxPtr, this,
		                     scale);
	} else if (isNormalMare() || mActorType == 0x4000016) {
		SMS_EasyEmitParticle(PARTICLE_MS_MARE_KIRA, mHappyEffectMtxPtr, this,
		                     scale);
	}
}

inline void TBaseNPC::emitPollutionParticle_(int particle, MtxPtr mtx)
{
	if (JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	        particle, mtx, 0, nullptr)) {
		emitter->setScale(getEffectScale_());
		SMSSetEmitterPolColor(emitter, 6);
	}
}

inline void TBaseNPC::emitDirtyEffect_()
{
	if (isPolWaitCEffectEmitTime_()) {
		int particle = -1;
		if (isNormalMonte())
			particle = PARTICLE_MS_MNT_YOGORE_B;
		else if (isNormalMareW())
			particle = PARTICLE_MS_MARE_YOGORE;
		else if (mActorType == 0x4000016)
			particle = PARTICLE_MS_KINO_YOGORE;

		if (particle != -1) {
			emitPollutionParticle_(particle, mPollutionEffectMtxPtr);
		}
	}

	if (isNormalMonte()) {
		if (isPolWaitLEffectEmitTime_())
			emitPollutionParticle_(PARTICLE_MS_MNT_YOGORE_F, unk200);

		if (isPolWaitREffectEmitTime_())
			emitPollutionParticle_(PARTICLE_MS_MNT_YOGORE_F, unk204);
	}
}

inline void TBaseNPC::emitWashEffect_()
{
	int particle = -1;
	if (isNormalMonte())
		particle = PARTICLE_MS_MNT_WASH;
	else if (isNormalMare() || mActorType == 0x4000016)
		particle = PARTICLE_MS_MARE_WASH;

	if (particle != -1) {
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(
		        particle, mPollutionEffectMtxPtr, 1, this)) {
			emitter->setScale(getEffectScale_());
			SMSSetEmitterPolColor(emitter, 6);
		}
	}
}

inline bool TBaseNPC::isPolWaitCEffectEmitTime_() const
{
	bool result = false;

	const f32* checkFrames = nullptr;
	if (isNormalMonte()) {
		static const f32 sCheckFrameMonte[]
		    = { 28.0f, 52.0f, 76.0f, 128.0f, 152.0f, 176.0f, -1.0f };
		checkFrames = sCheckFrameMonte;
	} else if (isNormalMare()) {
		static const f32 sCheckFrameMare[] = { 126.0f, 156.0f, -1.0f };
		checkFrames                        = sCheckFrameMare;
	} else if (mActorType == 0x4000016) {
		static const f32 sCheckFrameKino[] = { 22.0f, 44.0f, -1.0f };
		checkFrames                        = sCheckFrameKino;
	}

	if (checkFrames)
		result = IsCheckPassFrame(mMActor->getFrameCtrl(0), checkFrames);

	return result;
}

inline bool TBaseNPC::isPolWaitLEffectEmitTime_() const
{
	static const f32 sCheckFrameMonte[] = { 28.0f, 52.0f, 76.0f, -1.0f };
	return IsCheckPassFrame(mMActor->getFrameCtrl(0), sCheckFrameMonte);
}

inline bool TBaseNPC::isPolWaitREffectEmitTime_() const
{
	static const f32 sCheckFrameMonte[] = { 128.0f, 152.0f, 176.0f, -1.0f };
	return IsCheckPassFrame(mMActor->getFrameCtrl(0), sCheckFrameMonte);
}

void TBaseNPC::emitParticle_()
{
	if (mSmokeEffectMtxPtr != nullptr && (mActionFlag & NPC_ACTION_BURNING)) {
		JGeometry::TVec3<f32> scale = getEffectScale_();
		mFireParticlePos.set(mSmokeEffectMtxPtr[0][3], mSmokeEffectMtxPtr[1][3],
		                     mSmokeEffectMtxPtr[2][3]);
		SMS_EasyEmitParticle(PARTICLE_MS_NPC_KOKUEN, &mFireParticlePos, this,
		                     scale);
		JGeometry::TVec3<f32> local_30 = scale;
		local_30 *= mBurnStrength;
		SMS_EasyEmitParticle(PARTICLE_MS_MOE_FIRE_C, &mFireParticlePos, this,
		                     local_30);
		SMS_EasyEmitParticle(PARTICLE_MS_MOE_FIRE_A, &mFireParticlePos, this,
		                     local_30);
		SMS_EasyEmitParticle(PARTICLE_MS_MOE_FIRE_B, &mFireParticlePos, this,
		                     local_30);
	}

	if (mNoteEffectMtxPtr != nullptr
	    && (mActorType != 0x4000012
	        || unkD0->getCurrentAnmKind() != NPC_ANM_KIND_UNK5)) {
		JGeometry::TVec3<f32> scale = getEffectScale_();
		scale *= 0.75f;

		unk1F0.set(mNoteEffectMtxPtr[0][3], mNoteEffectMtxPtr[1][3],
		           mNoteEffectMtxPtr[2][3]);
		SMS_EasyEmitParticle(PARTICLE_MS_YNB_ONPU, &unk1F0, this, scale);
	}

	if (mActorType == 0x4000007 || gpMarDirector->mMap == 4) {
		f32 dVar11                  = 0.0f;
		bool doEmit                 = false;
		JGeometry::TVec3<f32> scale = getEffectScale_();

		if (mActorType == 0x4000007) {
			doEmit = true;
			scale *= 1.5f;
		} else if (mPosition.y <= 30.0f
		           && (mLinearVelocity.x != 0.0f
		               || mLinearVelocity.z != 0.0f)) {
			dVar11 = gpMapObjWave->getWaveHeight(mPosition.x, mPosition.z);
			if (mPosition.y <= dVar11)
				doEmit = true;
		}

		if (doEmit) {
			mWaveParticlePos.set(mPosition.x, dVar11, mPosition.z);
			SMS_EasyEmitParticle(PARTICLE_MS_NPC_HAMON_B, &mWaveParticlePos,
			                     this, scale);
			SMS_EasyEmitParticle(PARTICLE_MS_NPC_HAMON_A, &mWaveParticlePos,
			                     this, scale);
		}
	}

	switch (unkD0->getCurrentAnmKind()) {
	case NPC_ANM_KIND_UNKF:
		emitDirtyEffect_();
		break;

	case NPC_ANM_KIND_UNK19:
		emitWashEffect_();
		break;
	}
}
