#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <JSystem/JParticle/JPAParticle.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JParticle/JPASweepShape.hpp>
#include <JSystem/JParticle/JPAMath.hpp>
#include <JSystem/JParticle/JPAEmitterLoader.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>

static JPAEmitterInfo JPAEmitterInfoObj;

JPAEmitterInfo* JPAGetEmitterInfoPtr() { return &JPAEmitterInfoObj; }

JPABaseEmitter::JPABaseEmitter()
    : unk0(this)
    , mRng(0)
{
	MTXIdentity(unk124);

	unk154.set(1.0f, 1.0f, 1.0f);

	unk174.set(1.0f, 1.0f, 1.0f);

	unk16C.zero();
	unk160.zero();

	mChildSpawnTimer = 0.0f;

	mEmitTimer = 0.0f;

	mEmitRate = 1.0f;

	unk110                = nullptr;
	unk114                = nullptr;
	mEmitterDataBlockInfo = nullptr;

	unk172 = 0;

	initStatus(0);
	setStatus(STATUS_FIRST_EMIT | STATUS_EMIT_NEXT_FRAME);
	mRng.setSeed(JPAEmitterInfoObj.unk8.get());
}

f32 JPABaseEmitter::getFovy() { return JPAEmitterInfoObj.mFovy; }

f32 JPABaseEmitter::getAspect() { return JPAEmitterInfoObj.mAspect; }

JPABaseParticle* JPABaseEmitter::newParticle()
{
	JPAParticle* particle
	    = (JPAParticle*)mManager->unk0.getFirst()->getObject();

	mManager->unk0.remove(particle->getLinkBufferPtr());
	particle->setStatus(JPABaseParticle::FLAG_JUST_BORN);

	return particle;
}

void JPABaseEmitter::deleteBaseParticle(JPABaseParticle* particle,
                                        JSUList<JPABaseParticle>* list)
{
	particle->init();
	list->remove(particle->getLinkBufferPtr());
	mManager->unk0.prepend(particle->getLinkBufferPtr());
}

void JPABaseEmitter::deleteParticle(JPABaseParticle* particle) { }

void JPABaseEmitter::deleteAllParticle()
{
	// TODO: this is hella fake
	JSUList<JPABaseParticle>* list;
	JSULink<JPABaseParticle>* it;

	list = &mParticleList;
	it   = list->getFirst();
	while (it) {
		JSULink<JPABaseParticle>* next = it->getNext();
		deleteBaseParticle(it->getObject(), list);
		it = next;
	}

	list = &mChildParticleList;
	it   = list->getFirst();
	while (it) {
		JPABaseParticle* particle = it->getObject();
		it                        = it->getNext();
		deleteBaseParticle(particle, list);
	}
}

void JPABaseEmitter::createChildParticle(JPABaseParticle* parent)
{
	JGeometry::TVec3<f32> local_c0(0.0f, 0.0f, 0.0f);

	JPASweepShape* sweepShape = mEmitterDataBlockInfo->getSweepShape();
	int count                 = sweepShape->unk42;

	JGeometry::TVec3<f32> parentUnk20;
	parentUnk20.set(parent->mLocalPosition);
	JGeometry::TVec3<f32> parentUnk14;
	parentUnk14.set(parent->unk14);

	bool ignoreFields = false;
	if (!JPAEmitterInfoObj.mChildrenAffectedByFields)
		ignoreFields = true;

	for (int i = 0; i < count; ++i) {
		if (mManager->unk0.getNumLinks() == 0)
			break;

		JPAParticle* particle = (JPAParticle*)newParticle();
		mChildParticleList.prepend(particle->getLinkBufferPtr());

		particle->setStatus(JPABaseParticle::FLAG_UNK4);
		particle->mLifeProgress = 0.0f;
		particle->mLifetime     = sweepShape->unk40;
		particle->unk50         = unk114;

		if (ignoreFields) {
			particle->setStatus(JPABaseParticle::FLAG_IGNORE_FIELDS);
		} else {
			particle->mDragForce        = parent->getDragForce();
			particle->mCurrentDragForce = parent->getCurrentDragForce();
		}

		particle->mFieldVelocity.set(local_c0);

		f32 sweepShapeUnk1C = sweepShape->unk1C;
		if (sweepShapeUnk1C != 0.0f) {
			JGeometry::TVec3<f32> baseVel;
			baseVel.scale(sweepShapeUnk1C, parent->accessBaseVelVec());
			JGeometry::TVec3<f32> fVel;
			fVel.scale(sweepShapeUnk1C, parent->accessFVelVec());
			particle->setBaseVelVec(baseVel);
			particle->mFieldAcceleration.set(fVel);
		} else {
			particle->setBaseVelVec(local_c0);
			particle->mFieldAcceleration.set(local_c0);
		}

		if (sweepShape->unkC != 0.0f) {
			f32 f18 = getRandomRF() * (sweepShape->unkC * sweepShape->unk20);
			f18 += sweepShape->unkC;

			JGeometry::TVec3<f32> vec(getRandomSF(), getRandomSF(),
			                          getRandomSF());
			vec.setLength(f18);
			particle->addBaseVelVec(vec);
		}

		particle->unk68.set(0.0f, -1.0f, 0.0f);

		particle->unk78           = sweepShape->unk24;
		particle->mAirResistance  = parent->getAirResistance();
		particle->mDynamicsWeight = parent->getDynamicsWeight();
		particle->setVelocity();

		if (checkFlag(EMIT_FLAG_FOLLOW_EMITTER_CHILD))
			particle->setStatus(JPABaseParticle::FLAG_FOLLOW_EMITTER);

		particle->unk14.set(parentUnk14);
		particle->mLocalPosition.set(parentUnk20);

		f32 sweepShapeUnk8 = sweepShape->unk8;
		if (sweepShapeUnk8 != 0.0f) {
			Mtx rotMtx;
			JPAGetXYRotateMtx(mRng.get(), mRng.get(), rotMtx);
			JGeometry::TVec3<f32> vec(0.0f, 0.0f,
			                          getRandomF() * sweepShapeUnk8);
			MTXMultVec(rotMtx, vec, vec);
			particle->mLocalPosition.add(vec);
		}

		mDraw.initChild(parent, particle);
		particle->unk54 = parent->unk54;
	}
}

void JPABaseEmitter::getEmitterGlobalTranslation(JGeometry::TVec3<f32>& vec)
{
	TPosition3f local_3c;
	MTXScale(local_3c, unk154.x, unk154.y, unk154.z);
	MTXConcat(unk124, local_3c, local_3c);
	local_3c.setTrans(unk160);
	local_3c.mult(mTrans, vec);
}

void JPABaseEmitter::calcEmitterGlobalParams()
{
	JPAEmitterInfoObj.mCurrentEmitter      = this;
	JPAEmitterInfoObj.mCurrentFieldManager = &mFieldManager;

	if (mEmitterDataBlockInfo->getSweepShape() != nullptr
	    && mEmitterDataBlockInfo->getSweepShape()->mChildrenAffectedByFields)
		JPAEmitterInfoObj.mChildrenAffectedByFields = true;
	else
		JPAEmitterInfoObj.mChildrenAffectedByFields = false;

	JPAGetXYZRotateMtx(mRot.x * 182, mRot.y * 182, mRot.z * 182,
	                   JPAEmitterInfoObj.unkCC);
	Mtx afStack_48;
	MTXCopy(JPAEmitterInfoObj.unkCC, afStack_48);

	JPAEmitterInfoObj.unk30.set(mScale);

	TPosition3f local_A0;
	MTXScale(local_A0, mScale.x, mScale.y, mScale.z);
	MTXConcat(JPAEmitterInfoObj.unkCC, local_A0, JPAEmitterInfoObj.unkFC);
	local_A0.setTrans(mTrans);

	JPAEmitterInfoObj.unk18.set(unk154);
	JPAEmitterInfoObj.unk30.mul(unk154);

	JPAEmitterInfoObj.unkC.scale(1.0f, JPAEmitterInfoObj.unk18);

	MTXScale(JPAEmitterInfoObj.unk9C, unk154.x, unk154.y, unk154.z);

	MTXCopy(unk124, JPAEmitterInfoObj.unk6C);
	MTXConcat(JPAEmitterInfoObj.unk6C, JPAEmitterInfoObj.unkCC,
	          JPAEmitterInfoObj.unkCC);
	MTXConcat(JPAEmitterInfoObj.unk6C, JPAEmitterInfoObj.unkFC,
	          JPAEmitterInfoObj.unkFC);
	MTXConcat(JPAEmitterInfoObj.unk6C, JPAEmitterInfoObj.unk9C,
	          JPAEmitterInfoObj.unk9C);

	JPAEmitterInfoObj.unk9C.setTrans(unk160);

	MTXConcat(JPAEmitterInfoObj.unk9C, afStack_48, afStack_48);
	local_A0.getTrans(JPAEmitterInfoObj.unk24);

	JGeometry::TVec3<f32> xDir;
	JPAEmitterInfoObj.unkCC.getXDir(xDir);
	JPAEmitterInfoObj.unk48[0][0] = xDir.x;
	JPAEmitterInfoObj.unk48[0][1] = xDir.y;
	JPAEmitterInfoObj.unk48[0][2] = xDir.z;

	JGeometry::TVec3<f32> yDir;
	JPAEmitterInfoObj.unkCC.getXDir(yDir);
	JPAEmitterInfoObj.unk48[1][0] = yDir.x;
	JPAEmitterInfoObj.unk48[1][1] = yDir.y;
	JPAEmitterInfoObj.unk48[1][2] = yDir.z;

	JGeometry::TVec3<f32> zDir;
	JPAEmitterInfoObj.unkCC.getXDir(zDir);
	JPAEmitterInfoObj.unk48[2][0] = zDir.x;
	JPAEmitterInfoObj.unk48[2][1] = zDir.y;
	JPAEmitterInfoObj.unk48[2][2] = zDir.z;

	JGeometry::TVec3<f32> local_84(0.0f, 0.0f, 1.0f);
	JGeometry::TVec3<f32> local_90;
	local_90.normalize(mEmitterDirection);

	JPAEmitterInfoObj.unk3C.set(local_90);
	if (local_90 == local_84) {
		MTXIdentity(JPAEmitterInfoObj.unk12C);
	} else {
		JPAVecToRotaMtx(JPAEmitterInfoObj.unk12C, local_84, local_90);
	}
}

void JPABaseEmitter::loadBaseEmitterBlock(JPADataBlock* block)
{
	JSUMemoryInputStream stream2(block->mRawData,
	                             *(u32*)((u8*)block->mRawData + 4));

	JSUInputStream& stream = stream2;
	stream.skip(0xC);
	stream.read(&mScale, 0xC);
	stream.read(&mTrans, 0xC);
	stream.read(&mRot, 6);
	stream.read(&mVolumeType, 1);
	stream.read(&mEmitInterval, 1);
	stream.skip(2);
	stream.read(&mVolumeSubdivision, 2);
	stream.read(&mChildSpawnRate, 4);
	mChildSpawnRateVariance = JPAConvertFixToFloat(stream.readS16());
	mMaxFrame               = stream.readS16();
	stream.read(&mStartFrame, 2);
	stream.read(&mVolumeSize, 2);
	mVolumeYawSweep  = JPAConvertFixToFloat(stream.readS16());
	mVolumeMinRadius = JPAConvertFixToFloat(stream.readS16());
	stream.read(&mBaseLifetime, 2);
	mLifetimeRandomScale   = JPAConvertFixToFloat(stream.readS16());
	mBaseWeight            = JPAConvertFixToFloat(stream.readS16());
	mWeightRandomScale     = JPAConvertFixToFloat(stream.readS16());
	unk1C4                 = JPAConvertFixToFloat(stream.readS16());
	unk1CC                 = JPAConvertFixToFloat(stream.readS16());
	mBaseAirResistance     = JPAConvertFixToFloat(stream.readS16());
	mAirResistanceVariance = JPAConvertFixToFloat(stream.readS16());
	stream.read(&unk1FC, 4);
	stream.read(&unk200, 4);
	stream.read(&unk204, 4);
	stream.read(&unk208, 4);
	stream.read(&unk1C8, 4);

	JGeometry::TVec3<s16> fixVec;
	stream.read(&fixVec, 6);
	JGeometry::TVec3<f32> floatVec;
	JPAConvertFixVecToFloatVec(floatVec, fixVec);
	mEmitterDirection.normalize(floatVec);
	unk1E4 = JPAConvertFixToFloat(stream.readS16());
	stream.read(&mEmitFlags, 4);
	stream.read(&mKeyAnmTypeMask, 4);
}

void JPABaseEmitter::executeBeforeCallBack()
{
	if (unk110)
		unk110->execute(this);
}

void JPABaseEmitter::executeAfterCallBack()
{
	if (unk110)
		unk110->executeAfter(this);
}

void JPABaseEmitter::drawEmitterCallBack()
{
	if (unk110)
		unk110->draw(this);
}

JPABaseParticle* JPABaseEmitter::createParticle()
{
	if (mManager->unk0.getNumLinks() != 0) {

		JPAParticle* particle = (JPAParticle*)newParticle();
		mParticleList.prepend(particle->getLinkBufferPtr());

		if (checkFlag(EMIT_FLAG_FOLLOW_EMITTER))
			particle->setStatus(JPABaseParticle::FLAG_FOLLOW_EMITTER);

		particle->unk14.set(JPAEmitterInfoObj.unk24);

		s16 r24_param174;
		s16 r25_param176;
		s16 r26_param164;
		s32 r27_param168;

		if (checkFlag(EMIT_FLAG_FIXED_INTERVAL)) {
			r27_param168 = JPAEmitterInfoObj.mVolumeEmitIdx;
			r26_param164 = JPAEmitterInfoObj.mEmitCount;

			if (mVolumeType == VOLUME_TYPE_SPHERE) {
				r24_param174 = JPAEmitterInfoObj.mSphereCurrentPitch;
				r25_param176 = JPAEmitterInfoObj.mSphereCurrentYaw;

				JPAEmitterInfoObj.mSphereParticlesEmittedForCurrentLayer++;
				JPAEmitterInfoObj.mSphereCurrentYaw
				    += JPAEmitterInfoObj.mSphereYawStep;

				if (JPAEmitterInfoObj.mSphereParticlesEmittedForCurrentLayer
				    == JPAEmitterInfoObj.mSphereParticlesInCurrentLayer) {

					JPAEmitterInfoObj.mSphereParticlesEmittedForCurrentLayer
					    = 0;

					if (JPAEmitterInfoObj.mHemisphereFlipFlop) {
						JPAEmitterInfoObj.mSphereParticlesInCurrentLayer -= 4;
						if (JPAEmitterInfoObj.mSphereParticlesInCurrentLayer
						    == 0) {
							JPAEmitterInfoObj.mSphereParticlesInCurrentLayer
							    = 1;
						} else {
							JPAEmitterInfoObj.mSphereYawStep
							    = (s16)((int)(mVolumeYawSweep * 65536.0f)
							            / (int)JPAEmitterInfoObj
							                  .mSphereParticlesInCurrentLayer);
						}
						JPAEmitterInfoObj.mSphereCurrentPitch
						    = -JPAEmitterInfoObj.mSphereCurrentPitch;
						JPAEmitterInfoObj.mSphereCurrentPitch
						    += JPAEmitterInfoObj.mSpherePitchStep;
						JPAEmitterInfoObj.mHemisphereFlipFlop = false;
					} else {
						JPAEmitterInfoObj.mSphereCurrentPitch
						    = -JPAEmitterInfoObj.mSphereCurrentPitch;
						JPAEmitterInfoObj.mHemisphereFlipFlop = true;
					}
				}
				JPAEmitterInfoObj.mSphereCurrentYaw = 0;
			}
		}

		JGeometry::TVec3<f32> local_468;
		JGeometry::TVec3<f32> f31_f30_f29;

		f32 volumeSize = (f32)mVolumeSize;

		particle->unk68.zero();

		switch (mVolumeType) {
		case VOLUME_TYPE_POINT: {
			local_468.zero();
			break;
		}
		case VOLUME_TYPE_LINE: {
			f32 z;
			if (checkFlag(EMIT_FLAG_FIXED_INTERVAL)) {
				z = (volumeSize * (f32)r27_param168) / (f32)(r26_param164 - 1)
				    - volumeSize * 0.5f;
			} else {
				z = volumeSize * getRandomSF();
			}
			local_468.set(0.0f, 0.0f, z);
			break;
		}
		case VOLUME_TYPE_CIRCLE: {
			Mtx local_438;
			s16 angle;

			if (checkFlag(EMIT_FLAG_FIXED_INTERVAL)) {
				angle = (r27_param168 << 16) / r26_param164;
			} else {
				u32 uVar25 = mRng.get();
				u32 uVar26 = mRng.get();
				angle      = uVar25;
			}

			if (mVolumeYawSweep < 1.0f) {
				JPAGetYRotateMtx(angle * mVolumeYawSweep, local_438);
			} else {
				JPAGetYRotateMtx(angle, local_438);
			}

			f32 distance = getRandomF();
			if (checkFlag(EMIT_FLAG_FIXED_DENSITY)) {
				distance = JPASqrtf(distance);
			}

			f32 r = volumeSize
			        * (mVolumeMinRadius + distance * (1.0f - mVolumeMinRadius));
			local_468.set(0.0f, 0.0f, r);

			MTXMultVec(local_438, &local_468, &local_468);
			break;
		}
		case VOLUME_TYPE_CUBE: {
			local_468.set(getRandomSF() * volumeSize,
			              getRandomSF() * volumeSize,
			              getRandomSF() * volumeSize);
			break;
		}
		case VOLUME_TYPE_SPHERE: {
			Mtx local_408;

			if (checkFlag(EMIT_FLAG_FIXED_INTERVAL)) {
				JPAGetXYRotateMtx(r24_param174, r25_param176, local_408);
			} else {
				if (mVolumeYawSweep < 1.0f) {
					f32 y = mVolumeYawSweep * getRandomSS();
					JPAGetXYRotateMtx(getRandomRS(), y, local_408);
				} else {
					JPAGetXYZRotateMtx(mRng.get(), mRng.get(), mRng.get(),
					                   local_408);
				}
			}

			f32 distance = getRandomF();
			if (checkFlag(EMIT_FLAG_FIXED_DENSITY))
				distance = JPASqrtf(distance);

			f32 r = volumeSize
			        * (mVolumeMinRadius + distance * (1.0f - mVolumeMinRadius));
			local_468.set(0.0f, 0.0f, r);

			MTXMultVec(local_408, &local_468, &local_468);
			break;
		}
		case VOLUME_TYPE_CYLINDER: {
			s16 uVar11;
			if (mVolumeYawSweep < 1.0f) {
				uVar11 = mVolumeYawSweep * getRandomSS();
			} else {
				uVar11 = mRng.get();
			}

			f32 distance = getRandomF();
			if (checkFlag(EMIT_FLAG_FIXED_DENSITY)) {
				distance = JPASqrtf(distance);
			}

			f32 r = volumeSize
			        * (mVolumeMinRadius + distance * (1.0f - mVolumeMinRadius));

			local_468.set(r * JMASSin(uVar11), volumeSize * getRandomRF(),
			              r * JMASCos(uVar11));
			break;
		}
		case VOLUME_TYPE_TORUS: {
			s16 theta;
			if (mVolumeYawSweep < 1.0f) {
				theta = mVolumeYawSweep * getRandomSS();
			} else {
				theta = mRng.get();
			}

			Mtx afStack_170;
			JPAGetYRotateMtx(theta, afStack_170);

			local_468.set(volumeSize * JMASSin(theta), 0.0f,
			              volumeSize * JMASCos(theta));

			JGeometry::TVec3<f32> local_3C8;

			f32 rad = volumeSize * mVolumeMinRadius;
			s16 phi = mRng.get();

			local_3C8.set(0.0f, rad * JMASSin(phi), rad * JMASCos(phi));

			MTXMultVec(afStack_170, &local_3C8, &local_3C8);
			local_468 += local_3C8;
			f31_f30_f29.set(local_3C8);
			break;
		}
		}

		if (mVolumeType != VOLUME_TYPE_TORUS)
			f31_f30_f29.set(local_468.x, 0.0f, local_468.z);

		JGeometry::TVec3<f32> f21_f20_f19;
		f21_f20_f19.mul(local_468, JPAEmitterInfoObj.unk30);

		MTXMultVec(JPAEmitterInfoObj.unkFC.mMtx, &local_468,
		           &particle->mLocalPosition);

		f32 random_scale = unk1C4 * getRandomRF() + 1.0f;

		JGeometry::TVec3<f32> local_398(0.0f, 0.0f, 0.0f);
		JGeometry::TVec3<f32> f26_f25_f28(0.0f, 0.0f, 0.0f);
		if (unk1FC != 0.0f) {
			if (mVolumeType == VOLUME_TYPE_POINT) {
				f26_f25_f28.set(getRandomSF(), getRandomSF(), getRandomSF());
				f26_f25_f28.setLength(unk1FC);
			} else {
				f26_f25_f28.set(f21_f20_f19);
				f26_f25_f28.setLength(unk1FC);
			}
		}

		JGeometry::TVec3<f32> f22_f23_f24(0.0f, 0.0f, 0.0f);
		if (unk200 != 0.0f) {
			if (mVolumeType == VOLUME_TYPE_POINT) {
				if (checkFlag(EMIT_FLAG_FIXED_INTERVAL)) {
					s16 ang = (s16)((r27_param168 << 16) / r26_param164);
					f22_f23_f24.set(JMASSin(ang), 0.0f, JMASCos(ang));
				} else {
					f22_f23_f24.x = getRandomSF();
					f22_f23_f24.y = 0.0f;
					f22_f23_f24.z = getRandomSF();
				}
				f22_f23_f24.setLength(unk200);
			} else {
				f22_f23_f24.setLength(f31_f30_f29, unk200);
			}
		}

		JGeometry::TVec3<f32> add3_vec(0.0f, 0.0f, 0.0f);
		if (unk204 != 0.0f) {
			add3_vec.x = getRandomSF() * unk204;
			add3_vec.y = getRandomSF() * unk204;
			add3_vec.z = getRandomSF() * unk204;
		}

		if (unk208 != 0.0f) {
			if (unk1E4 > 0.0f) {
				local_398.set(0.0f, 0.0f, 1.0f);
				Mtx temp;
				s16 y = (32768.0f * getRandomRF()) * unk1E4;
				JPAGetYZRotateMtx(y, mRng.get(), temp);
				MTXConcat(JPAEmitterInfoObj.unk12C.mMtx, temp, temp);
				MTXMultVec(temp, &local_398, &local_398);
			} else {
				local_398.set(JPAEmitterInfoObj.unk3C);
			}
			local_398 *= unk208;
		}

		JGeometry::TVec3<f32> local_35c;
		local_35c.x = local_398.x + add3_vec.x + f26_f25_f28.x + f22_f23_f24.x;
		local_35c.y = local_398.y + add3_vec.y + f26_f25_f28.y + f22_f23_f24.y;
		local_35c.z = local_398.z + add3_vec.z + f26_f25_f28.z + f22_f23_f24.z;

		if (unk1C4 != 0.0f)
			local_35c *= random_scale;

		if (checkFlag(EMIT_FLAG_INHERIT_SCALE)) {
			local_35c.mul(mScale);
		}

		MTXMultVec(JPAEmitterInfoObj.unkCC.mMtx, &local_35c, &local_35c);
		particle->setBaseVelVec(local_35c);

		JGeometry::TVec3<f32> local_350;
		local_350.normalize(local_35c);
		JGeometry::TVec3<f32> local_340 = local_350;
		particle->unk68.set(local_340);

		f32 dynamics_scale = 0.0f;
		if (unk1C8 != 0.0f) {
			dynamics_scale = unk1C8 * (unk1CC * getRandomRF() + 1.0f);
		}
		particle->unk78 = dynamics_scale;

		f32 air = mBaseAirResistance + mAirResistanceVariance * getRandomSF();
		if (air > 1.0f)
			air = 1.0f;
		particle->mAirResistance = air;

		particle->mDynamicsWeight
		    = mBaseWeight * (1.0f - mWeightRandomScale * getRandomF());

		particle->initGlobalPosition();
		particle->mLifeProgress = 0.0f;

		f32 life = (f32)mBaseLifetime;
		if (mLifetimeRandomScale != 0.0f) {
			life *= 1.0f - mLifetimeRandomScale * getRandomF();
		}
		particle->mLifetime = life;
		particle->unk50     = unk114;
		mDraw.initParticle(particle);

		return particle;
	}

	return nullptr;
}

void JPABaseEmitter::calcCurrentRateTimerStep()
{
	if (mEmitInterval == 0) {
		mEmitRate = 1.0f;
	} else {
		mEmitRate = 1.0f / (mEmitInterval + 1.0f) + 1e-07f;
	}
}

int JPABaseEmitter::calcCreateParticle()
{
	int numToCreate = 0;

	JPAEmitterInfo& eio = JPAEmitterInfoObj;

	if (checkStatus(STATUS_EMIT_NEXT_FRAME)) {

		if (!checkFlag(EMIT_FLAG_FIXED_INTERVAL)) {
			f32 createRate = mChildSpawnRate;

			if (mChildSpawnRateVariance != 0.0f) {
				createRate
				    += createRate * mChildSpawnRateVariance * getRandomRF();
			}

			mChildSpawnTimer += createRate;

			if (mChildSpawnTimer >= 1.0f) {
				numToCreate = (s32)mChildSpawnTimer;
				mChildSpawnTimer
				    = mChildSpawnTimer - (f32)(s32)mChildSpawnTimer;
			} else if (createRate > 0.0f && checkStatus(STATUS_FIRST_EMIT)) {
				numToCreate = 1;
			} else {
				numToCreate = 0;
			}
		} else {
			if (mVolumeType == VOLUME_TYPE_SPHERE) {

				eio.mSphereParticlesEmittedForCurrentLayer = 0;
				eio.mSphereCurrentPitch                    = 0;
				eio.mSphereCurrentYaw                      = 0;
				eio.mSphereParticlesInCurrentLayer         = 2;
				eio.mHemisphereFlipFlop                    = true;
				eio.mSphereParticlesInCurrentLayer         = 0;

				numToCreate = 2;
				for (int i = 0; i < mVolumeSubdivision; ++i) {
					numToCreate += eio.mSphereParticlesInCurrentLayer;
					eio.mSphereParticlesInCurrentLayer += 4;
					numToCreate += eio.mSphereParticlesInCurrentLayer;
				}

				// 65536 is 360 degree
				eio.mSphereYawStep
				    = (s16)((s32)(65536.0f * mVolumeYawSweep)
				            / eio.mSphereParticlesInCurrentLayer);
				// 90 degrees divided by the number of layers
				eio.mSpherePitchStep = (s16)(65536 / 4 / mVolumeSubdivision);
			} else if (mVolumeType == VOLUME_TYPE_CIRCLE
			           || mVolumeType == VOLUME_TYPE_LINE) {
				numToCreate = mVolumeSubdivision;
			}
		}

		JPAEmitterInfoObj.mEmitCount = (s16)numToCreate;

		if (checkStatus(STATUS_STOP_EMIT))
			numToCreate = 0;

		if (numToCreate != 0) {
			for (int i = 0; i < numToCreate; ++i) {
				eio.mVolumeEmitIdx = i;
				if (createParticle() == nullptr)
					break;
			}
		}
	}

	if (mEmitRate == 1.0f) {
		setStatus(STATUS_EMIT_NEXT_FRAME);
	} else {
		mEmitTimer += mEmitRate;

		if (mEmitTimer >= 1.0f) {
			mEmitTimer -= 1.0f;
			setStatus(STATUS_EMIT_NEXT_FRAME);
		} else {
			clearStatus(STATUS_EMIT_NEXT_FRAME);
		}
	}

	clearStatus(STATUS_FIRST_EMIT);

	return numToCreate;
}

bool JPABaseEmitter::checkStartFrame()
{
	if ((s16)unk18.getFrame() >= mStartFrame)
		return true;

	unk18.incFrame();
	return false;
}

bool JPABaseEmitter::checkMaxFrame()
{
	if (isContinuousParticle())
		return false;

	if (mMaxFrame < 0) {
		setStatus(STATUS_ENABLE_DELETE);
		return true;
	}

	if ((s32)unk10.getFrame() >= mMaxFrame) {
		setStatus(STATUS_ENABLE_DELETE);

		if (checkStatus(STATUS_IMMORTAL))
			return false;

		return true;
	}

	return false;
}

void JPABaseEmitter::doParticle()
{
	JSUList<JPABaseParticle>* list = getParticleList();

	JSULink<JPABaseParticle>* link = mParticleList.getFirst();
	while (link) {
		JPABaseParticle* particle      = link->getObject();
		JSULink<JPABaseParticle>* next = link->getNext();

		particle->incTimer();
		if (!particle->checkStatus(JPABaseParticle::FLAG_DEAD)) {
			particle->calcVelocity();
			particle->executeParticleCallBack(this);
			if (!particle->checkStatus(JPABaseParticle::FLAG_DELETE)) {
				mDraw.calcParticle(particle);
				if (particle->checkCreateChildParticle())
					createChildParticle(particle);
				particle->calcGlobalPosition();
			}
		} else {
			particle->setDeleteParticleFlag();
		}

		if (particle->checkStatus(JPABaseParticle::FLAG_DELETE))
			deleteBaseParticle(particle, list);

		link = next;
	}
}

void JPABaseEmitter::doChildParticle()
{
	JSUList<JPABaseParticle>* list = getChildParticleList();

	JSULink<JPABaseParticle>* link = mChildParticleList.getFirst();
	while (link) {
		JPABaseParticle* particle      = link->getObject();
		JSULink<JPABaseParticle>* next = link->getNext();

		particle->incTimer();
		if (!particle->checkStatus(JPABaseParticle::FLAG_DEAD)) {
			if (particle->getAge() != 0)
				particle->calcVelocity();
			particle->executeParticleCallBack(this);
			if (!particle->checkStatus(JPABaseParticle::FLAG_DELETE)) {
				mDraw.calcChild(particle);
				particle->calcGlobalPosition();
			}
		} else {
			particle->setDeleteParticleFlag();
		}

		if (particle->checkStatus(JPABaseParticle::FLAG_DELETE)) {
			deleteBaseParticle(particle, list);
		}

		link = next;
	}
}

f32 JPABaseEmitter::getKeyValue(f32 time, u16 frame_num, f32* frames)
{
	return JPAGetKeyFrameValue(time, frame_num, frames);
}

void JPABaseEmitter::calcKeyFrameAnime()
{
	u32 keyNum = mEmitterDataBlockInfo->getKeyNum();
	if (!keyNum)
		return;

	u32 mask = mKeyAnmTypeMask;

	JPAKeyFrameAnime** animeFrames = mEmitterDataBlockInfo->getKey();

	u32 bit    = 1;
	u32 bitIdx = 0;

	for (int i = 0; i < keyNum; ++i) {

		u32 stop = false;
		do {
			if ((mask & bit) != 0)
				stop = true;
			bit <<= 1;
			++bitIdx;
		} while (!stop);

		f32 time       = unk10.getFrame();
		u8* animeData  = (u8*)animeFrames[i]->mRawData;
		f32* keyFrames = (f32*)(animeData + 0x20);
		u8 frameNum    = animeData[0x10];
		if (animeData[0x12] ? true : false) {
			s32 keyFrame = (s32)keyFrames[(frameNum - 1) * 4] + 1;
			s32 tmp      = (s32)unk10.getFrame() / keyFrame;
			time -= tmp * keyFrame;
		}

		switch (bitIdx - 1) {
		case 0:
			mChildSpawnRate = getKeyValue(time, frameNum, keyFrames);
			break;
		case 1:
			mVolumeSize = getKeyValue(time, frameNum, keyFrames);
			break;
		case 2:
			mVolumeYawSweep = getKeyValue(time, frameNum, keyFrames);
			break;
		case 3:
			unk20C = getKeyValue(time, frameNum, keyFrames);
			break;
		case 4:
			mBaseLifetime = getKeyValue(time, frameNum, keyFrames);
			break;
		case 5:
			mBaseWeight = getKeyValue(time, frameNum, keyFrames);
			break;
		case 6:
			unk1FC = getKeyValue(time, frameNum, keyFrames);
			break;
		case 7:
			unk1FC = getKeyValue(time, frameNum, keyFrames);
			break;
		case 8:
			unk208 = getKeyValue(time, frameNum, keyFrames);
			break;
		case 9:
			unk1E4 = getKeyValue(time, frameNum, keyFrames);
			break;
		case 10:
			unk1E4 = getKeyValue(time, frameNum, keyFrames);
			break;
		}
	}
}

void JPABaseEmitter::calc()
{
	JPAEmitterInfoObj.mEmitCount = 0;

	if (!checkStatus(STATUS_STOP_CALC))
		calcKeyFrameAnime();

	executeBeforeCallBack();

	if (!checkStatus(STATUS_STOP_CALC)) {
		calcEmitterGlobalParams();
		mDraw.calc();
		mFieldManager.calcFieldParams();
		if (!checkStatus(STATUS_ENABLE_DELETE))
			calcCreateParticle();
	}

	executeAfterCallBack();

	if (!checkStatus(STATUS_STOP_CALC)) {
		doParticle();
		doChildParticle();
		unk10.incFrame();
	}
}

void JPABaseEmitter::setGlobalRMatrix(MtxPtr) { }

void JPABaseEmitter::setGlobalRTMatrix(MtxPtr param_1)
{
	JPAGetRMtxTVecElement(param_1, unk124, unk160);
}

void JPABaseEmitter::setGlobalSRTMatrix(MtxPtr param_1)
{
	JPAGetRMtxSTVecElement(param_1, unk124, unk154, unk160);
}

void JPABaseEmitter::getPivotX() { }

void JPABaseEmitter::getPivotY() { }
