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
	else if (getActorType() == 0x4000016)
		jointName = kinoppio;
	else
		jointName = nullptr;

	if (jointName != nullptr) {
		s32 jointIdx       = tab->getIndex(jointName);
		mHappyEffectMtxPtr = getModel()->getAnmMtx((u16)jointIdx);
	}
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

	if (jointName) {
		s32 jointIdx      = tab->getIndex(jointName);
		mNoteEffectMtxPtr = getModel()->getAnmMtx((u16)jointIdx);
	}
}

void TBaseNPC::setPollutionEffectMtxPtr_(const JUTNameTab* tab)
{
	const char* koshiNullJoint = "koshi_null";
	const char* koshiJoint     = "koshi";
	const char* bodyJoint      = "jnt_body";
	const char* leftFootJoint  = "footL_jnt";
	const char* rightFootJoint = "footR_jnt";

	const char* pcVar5;
	if (isNormalMonte()) {
		s32 leftFootIdx  = tab->getIndex(leftFootJoint);
		unk200           = getModel()->getAnmMtx((u16)leftFootIdx);
		s32 rightFootIdx = tab->getIndex(rightFootJoint);
		unk204           = getModel()->getAnmMtx((u16)rightFootIdx);
		pcVar5           = koshiNullJoint;
	} else if (isNormalMare()) {
		pcVar5 = koshiJoint;
	} else if (getActorType() == 0x4000016) {
		pcVar5 = bodyJoint;
	} else {
		pcVar5 = nullptr;
	}

	if (pcVar5) {
		s32 jointIdx           = tab->getIndex(pcVar5);
		mPollutionEffectMtxPtr = getModel()->getAnmMtx((u16)jointIdx);
	}
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
	u16 jointIdx = model->getModelData()->getJointName()->getIndex(pcVar3);
	mSmokeEffectMtxPtr = model->getAnmMtx(jointIdx);
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
	if (checkLiveFlag(LIVE_FLAG_SINK_BOTTOM))
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
	} else if (isNormalMare() || getActorType() == 0x4000016) {
		SMS_EasyEmitParticle(PARTICLE_MS_MARE_KIRA, mHappyEffectMtxPtr, this,
		                     scale);
	}
}

inline void TBaseNPC::emitPollutionParticle_(int particle, MtxPtr mtx)
{
	if (JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	        particle, mtx, 0, nullptr)) {
		emitter->setGlobalScale(getEffectScale_());
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
			emitter->setGlobalScale(getEffectScale_());
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
	} else if (getActorType() == 0x4000016) {
		static const f32 sCheckFrameKino[] = { 22.0f, 44.0f, -1.0f };
		checkFrames                        = sCheckFrameKino;
	}

	if (checkFrames)
		result = IsCheckPassFrame(getMActor()->getFrameCtrl(ANM_TYPE_BCK),
		                          checkFrames);

	return result;
}

inline bool TBaseNPC::isPolWaitLEffectEmitTime_() const
{
	static const f32 sCheckFrameMonte[] = { 28.0f, 52.0f, 76.0f, -1.0f };
	return IsCheckPassFrame(mMActor->getFrameCtrl(ANM_TYPE_BCK),
	                        sCheckFrameMonte);
}

inline bool TBaseNPC::isPolWaitREffectEmitTime_() const
{
	static const f32 sCheckFrameMonte[] = { 128.0f, 152.0f, 176.0f, -1.0f };
	return IsCheckPassFrame(mMActor->getFrameCtrl(ANM_TYPE_BCK),
	                        sCheckFrameMonte);
}

// TODO: frame-exact (0xf0) and instruction-exact, but 75 slot offsets still
// differ. Retail's 12-byte pool runs 0x68, 0x78, 0x84, <12-byte hole at 0x90>,
// 0x9c, 0xa8, 0xb4, 0xc0, 0xcc; ours runs 0x5c, <24-byte hole>, 0x80, 0x8c,
// 0x98, 0xa4, 0xb0, 0xbc, 0xc8. So retail puts the third block's `scale`
// (0x78) *below* that block's getEffectScale_ return buffer (0x84) while every
// declaration order we tried puts it at the top of the pool, and retail keeps
// a 12-byte hole above the second block's buffer. `doEmit` is r30 in retail
// and r29 here. Frame reached 0xf0 with the accessor levers below
// (getActorType x3 = +16 saturating, SMSGetMarDirector()->getCurrentMap() =
// +16, getPosition() at the y guard = +8 and at mWaveParticlePos.set = +8);
// declaration-order permutations of dVar11/doEmit/scale move nothing.
// cc26: with the NpcEmitWaveParticle level below, the residue is 8 lines:
// retail's wave block runs ret 0x84 / scale 0x78 / switch ret 0x68, ours
// 0x90 / 0x74 / 0x64, and doEmit is r30 in retail, r29 here. As a diagnostic,
// the same body as a TBaseNPC *member* (getEffectScale_ called inside, so the
// return buffer is the callee's too; needs a declaration in NpcBase.hpp, not
// made) puts the three at 0x80 / 0x74 / 0x64 -- uniformly 4 low. Removing an
// accessor from the helper moves all three a further 4 low; adding one
// (getPosition() at the height test or getLinearVelocity()) costs +8 frame;
// one in the getWaveHeight arguments is inert. Also tried: scale by value,
// a named `const TVec3&`/value binding of getEffectScale_() in the caller,
// argument/declaration order of the references, C-style top declarations.
// Fabricated: the wave-ripple block as a TU-local level. Taking the caller's
// `height`/`doEmit` by reference keeps them the caller's f31/r30-class locals
// (initialised before getEffectScale_ is called, as retail does), while the
// scale copy becomes this callee's block object and moves below the
// getEffectScale_ return buffer, where retail has it (99.80 -> 99.9, 75 -> 8
// differing lines).
static inline void NpcEmitWaveParticle(TBaseNPC* npc, f32& height,
                                       bool& doEmit,
                                       const JGeometry::TVec3<f32>& baseScale)
{
	JGeometry::TVec3<f32> scale = baseScale;
	if (npc->getActorType() == 0x4000007) {
		doEmit = true;
		scale *= 1.5f;
	} else if (npc->getPosition().y <= 30.0f
	           && (npc->mLinearVelocity.x != 0.0f
	               || npc->mLinearVelocity.z != 0.0f)) {
		height = gpMapObjWave->getWaveHeight(npc->mPosition.x,
		                                     npc->mPosition.z);
		if (npc->mPosition.y <= height)
			doEmit = true;
	}

	if (doEmit) {
		npc->mWaveParticlePos.set(npc->getPosition().x, height,
		                          npc->getPosition().z);
		SMS_EasyEmitParticle(PARTICLE_MS_NPC_HAMON_B, &npc->mWaveParticlePos,
		                     npc, scale);
		SMS_EasyEmitParticle(PARTICLE_MS_NPC_HAMON_A, &npc->mWaveParticlePos,
		                     npc, scale);
	}
}

void TBaseNPC::emitParticle_()
{
	if (mSmokeEffectMtxPtr != nullptr && checkActionFlag(NPC_ACTION_BURNING)) {
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
	    && (getActorType() != 0x4000012
	        || unkD0->getCurrentAnmKind() != NPC_ANM_KIND_UNK5)) {
		JGeometry::TVec3<f32> scale = getEffectScale_();
		scale *= 0.75f;

		unk1F0.set(mNoteEffectMtxPtr[0][3], mNoteEffectMtxPtr[1][3],
		           mNoteEffectMtxPtr[2][3]);
		SMS_EasyEmitParticle(PARTICLE_MS_YNB_ONPU, &unk1F0, this, scale);
	}

	if (getActorType() == 0x4000007
	    || SMSGetMarDirector()->getCurrentMap() == 4) {
		f32 height  = 0.0f;
		bool doEmit = false;
		NpcEmitWaveParticle(this, height, doEmit, getEffectScale_());
	}

	switch (unkD0->getCurrentAnmKind()) {
	case NPC_ANM_KIND_DIRTY:
		emitDirtyEffect_();
		break;

	case NPC_ANM_KIND_WASH:
		emitWashEffect_();
		break;
	}
}
