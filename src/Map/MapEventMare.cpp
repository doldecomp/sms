#include <Map/MapEventMare.hpp>
#include <Map/Map.hpp>
#include <Map/MapModel.hpp>
#include <Map/MapStaticObject.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/PollutionManager.hpp>
#include <Map/JointObj.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <System/Particles.hpp>
#include <System/EmitterViewObj.hpp>
#include <MSound/MSound.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <Camera/CameraShake.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <stdio.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

f32 TMareWallRock::mAppearSpeed       = 3.0f;
f32 TMareWallRock::mDepressSpeed      = 3.0f;
u32 TMareWallRock::mCleanedDegree     = 50;
u32 TMareWallRock::mWaitTimeToAppear  = 240;
u32 TMareWallRock::mWaitTimeToDepress = 1200;

u32 TMareEventDepressWall::mCleanedDegree   = 42;
f32 TMareEventDepressWall::mDepressSpeed    = 1.0f;
f32 TMareEventDepressWall::mRiseSpeed       = 5.0f;
u32 TMareEventDepressWall::mWaitTimeToWatch = 120;

J3DModel* TMareWallRock::getMapModel() const
{
	return gpMap->getModelManager()->getJointModel(0)->getModel();
}

void TMareWallRock::depress() { }

void TMareWallRock::appear()
{
	// TODO: hack, remove me
	(void)0;
	(void)0;
	(void)0;

	unk10C[0]->setUp();
	unk104->alive();
	JGeometry::TVec3<f32> t(0.0f, 0.0f, unkFC);
	unk10C[0]->moveTrans(t);
	f32 rotY = unk128;

	if (JPABaseEmitter* em = gpMarioParticleManager->emit(
	        MAP_MAP_MS_MARE_OBJUP_A, &unk11C, 0, &unk11C)) {
		em->setRotation(JGeometry::TVec3<f32>(0.0f, rotY, 0.0f));
		em->unk154.set(unk110);
	}

	if (JPABaseEmitter* em = gpMarioParticleManager->emit(
	        MAP_MAP_MS_MARE_OBJUP_B, &unk11C, 2, &unk11C)) {
		em->setRotation(JGeometry::TVec3<f32>(0.0f, rotY, 0.0f));
		em->unk154.set(unk110);
	}

	unkF4 = 2;
}

void TMareWallRock::movement()
{
	switch (unkF4) {
	case 0:
		if (((TPollutionLayer*)gpPollution->getJointModel(unk108))
		        ->getPollutionDegree()
		    < mCleanedDegree) {
			appear();
			gpPollution->offLayer(unk108);
		}
		break;

	case 2: {
		J3DJoint* joint                 = unk104->getJoint();
		J3DTransformInfo& transformInfo = joint->getTransformInfo();
		if (!TMapObjBase::isDemo()) {
			transformInfo.mTranslate.z -= mAppearSpeed;
			if (TMapObjBase::marioIsOn(this))
				mPosition.z -= mAppearSpeed;
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_QUAKE, &mPosition, 0,
			                                nullptr, 0, 4);
			SMSRumbleMgr->start(0x13, -1, (f32*)nullptr);
			gpCameraShake->keepShake(CAM_SHAKE_MODE_UNK5, 0.5f);
		}

		unk104->getJoint()->setTransformInfo(transformInfo);
		getMapModel()->calc();
		f32 z = transformInfo.mTranslate.z;
		if (z < 0.0f) {
			unk100 = mWaitTimeToDepress;
			unk10C[1]->setUp();
			unkF4 = 1;
			SMSRumbleMgr->stop(0x13);
			return;
		}
		JGeometry::TVec3<f32> t(0.0f, 0.0f, z);
		unk10C[0]->moveTrans(t);
		break;
	}

	case 3:
		if (!TMapObjBase::isDemo()) {
			unk100 -= 1;
			if (unk100 <= 0) {
				appear();
				return;
			}
		}
		break;

	case 4: {
		J3DJoint* joint                 = unk104->getJoint();
		J3DTransformInfo& transformInfo = joint->getTransformInfo();
		if (!TMapObjBase::isDemo()) {
			transformInfo.mTranslate.z += mDepressSpeed;
			if (TMapObjBase::marioIsOn(this))
				mPosition.z += mDepressSpeed;
		}

		unk104->getJoint()->setTransformInfo(transformInfo);
		getMapModel()->calc();
		f32 z = transformInfo.mTranslate.z;
		if (z > unkFC) {
			unk10C[0]->remove();
			unk104->kill();
			unk100 = mWaitTimeToAppear;
			unkF4  = 3;
			return;
		}
		JGeometry::TVec3<f32> t(0.0f, 0.0f, z);
		unk10C[0]->moveTrans(t);
		break;
	}
	}
}

void TMareWallRock::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE)
		movement();
	THitActor::perform(cue, graphics);
}

void TMareWallRock::initEffect()
{
	switch (unkF8 + 1) {
	case 1:
		unk110.set(1.3f, 4.0f, 1.0f);
		unk11C.set(-7085.0f, 1390.0f, 3600.0f);
		break;
	case 2:
		unk110.set(1.0f, 1.0f, 1.0f);
		unk11C.set(-8300.0f, 2650.0f, 4300.0f);
		break;
	case 3:
		unk110.set(0.9f, 1.2f, 1.0f);
		unk11C.set(-7120.0f, 3430.0f, 3820.0f);
		break;
	case 4:
		unk110.set(2.3f, 1.0f, 1.0f);
		unk11C.set(-5150.0f, 4700.0f, 4100.0f);
		break;
	case 5:
		unk110.set(1.1f, 0.6f, 1.0f);
		unk11C.set(-7606.0f, 5480.0f, 5400.0f);
		break;
	case 6:
		unk110.set(1.4f, 1.6f, 1.0f);
		unk11C.set(-5000.0f, 7160.0f, 4950.0f);
		break;
	case 7:
		unk110.set(1.3f, 1.3f, 1.0f);
		unk11C.set(-6880.0f, 7780.0f, 5300.0f);
		break;
	}
}

void TMareWallRock::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	unk10C    = new TMapCollisionBase*[2];
	unk10C[0] = new TMapCollisionMove;
	unk10C[1] = new TMapCollisionWarp;

	char buf[0x100];
	snprintf(buf, 0x100, "/map/map/building%02d", unkF8 + 1);
	unk10C[0]->init(buf, 0, this);
	unk10C[1]->init(buf, 0, this);
	unk104          = TMapObjBase::getBuildingJointObj(unkF8 + 1);
	unk108          = unkF8;
	J3DJoint* joint = unk104->getJoint();
	const Vec& max  = joint->getMax();
	const Vec& min  = joint->getMin();
	mPosition.x     = (max.x + min.x) / 2.0f;
	mPosition.y     = (max.y + min.y) / 2.0f;
	mPosition.z     = (max.z + min.z) / 2.0f;
	unkFC           = 100.0f + (max.z - min.z);
	TMapObjBase::moveJoint(unk104->mJoint, 0.0f, 0.0f, unkFC);
	unk104->kill();
	initHitActor(0x4000022C, 1, 0, 0.0f, 0.0f, 0.0f, 0.0f);
	initEffect();
}

void TMareWallRock::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);
}

TMareWallRock::TMareWallRock(const char*) { }
// NOTE: m2c shows TMareWallRock has no-arg ctor (TMareWallRock()); a const
// char* overload appears in source but is UNUSED (eliminated). Keeping the
// stub.

void TMareEventWallRock::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);
	unk14 = new TMareWallRock[unk10];
	JDrama::TViewObjPtrListT<JDrama::TViewObj>* group
	    = JDrama::TNameRefGen::search<
	        JDrama::TViewObjPtrListT<JDrama::TViewObj> >("マップグループ");
	for (int i = 0; i < unk10; ++i) {
		unk14[i].unkF8 = i;
		group->getChildren().push_back((JDrama::TViewObj*)&unk14[i]);
	}
}

TMareEventWallRock::TMareEventWallRock(const char* name)
    : JDrama::TViewObj(name)
{
	unk10 = 7;
	unk14 = nullptr;
}

void TMareEventDepressWall::finishEvent() { }

void TMareEventDepressWall::setJointPosX(float, int) { }

void TMareEventDepressWall::rising()
{
	f32 x = TMapObjBase::getJointTransX(unk30[unk48]);
	SMSGetMSound()->startSoundActor(MSD_SE_OBJ_QUAKE, &unk34[unk48], 0, nullptr,
	                                0, 4);

	{
		int idx = unk48;
		if (JPABaseEmitter* em = gpMarioParticleManager->emit(
		        MAP_MAP_MS_MARE_BLOCKUP, &unk34[idx], 1, &unk34[idx])) {
			em->setScale(unk38[idx]);
			em->mChildSpawnRate = unk3C[idx];
			em->unk174.setAll(unk40[idx]);
		}
	}

	if (unk1C[unk48]) {
		if (x > 0.0f) {
			x -= mRiseSpeed;
			int idx = unk48;
			TMapObjBase::setJointTransX(unk30[idx], x);
			JGeometry::TVec3<f32> t(x, 0.0f, 0.0f);
			unk28[idx].moveTrans(t);
			return;
		}
		JGeometry::TVec3<f32> zero(0.0f, 0.0f, 0.0f);
		unk28[unk48].remove();
		unk24[unk48].setUpTrans(zero);
		TMapObjBase::setJointTransX(unk30[unk48], 0.0f);
		SMSRumbleMgr->stop(0x13);
		unk48 += 1;
		if (unk48 == unk10) {
			unk44 = 4;
			unk4C = mWaitTimeToWatch;
			return;
		}
		unk4C = unk18[unk48];
		unk44 = 2;
		return;
	}
	if (x < 0.0f) {
		x += mRiseSpeed;
		int idx = unk48;
		TMapObjBase::setJointTransX(unk30[idx], x);
		JGeometry::TVec3<f32> t(x, 0.0f, 0.0f);
		unk28[idx].moveTrans(t);
		return;
	}
	JGeometry::TVec3<f32> zero(0.0f, 0.0f, 0.0f);
	unk28[unk48].remove();
	unk24[unk48].setUpTrans(zero);
	TMapObjBase::setJointTransX(unk30[unk48], 0.0f);
	SMSRumbleMgr->stop(0x13);
	unk48 += 1;
	if (unk48 == unk10) {
		unk44 = 4;
		unk4C = mWaitTimeToWatch;
		return;
	}
	unk4C = unk18[unk48];
	unk44 = 2;
}

void TMareEventDepressWall::startToRise() { }

void TMareEventDepressWall::emitEffect(int) { }

void TMareEventDepressWall::depressing()
{
	int doneCount = 0;
	for (int i = 0; i < unk10; ++i) {
		f32 x = TMapObjBase::getJointTransX(unk30[i]);
		if (unk1C[i]) {
			if (x < unk20[i]) {
				if (!TMapObjBase::isDemo()) {
					x += mDepressSpeed;
					SMSRumbleMgr->start(0x13, -1, (f32*)nullptr);
					gpCameraShake->keepShake(CAM_SHAKE_MODE_UNK5, 0.5f);
					SMSGetMSound()->startSoundActor(MSD_SE_OBJ_QUAKE, &unk34[i],
					                                0, nullptr, 0, 4);
					JPABaseEmitter* em = gpMarioParticleManager->emit(
					    MAP_MAP_MS_MARE_BLOCKUP, &unk34[i], 1, &unk34[i]);
					if (em) {
						em->setScale(unk38[i]);
						em->mChildSpawnRate = unk3C[i];
						em->unk174.setAll(unk40[i]);
					}
				} else {
					SMSRumbleMgr->stop(0x13);
				}
				TMapObjBase::setJointTransX(unk30[i], x);
				JGeometry::TVec3<f32> t(x, 0.0f, 0.0f);
				unk28[i].moveTrans(t);
				if (x >= unk20[i]) {
					unk28[i].remove();
					JGeometry::TVec3<f32> t2(x, 0.0f, 0.0f);
					unk24[i].setUpTrans(t2);
					doneCount += 1;
				}
			} else {
				doneCount += 1;
			}
		} else {
			if (x > -unk20[i]) {
				if (!TMapObjBase::isDemo()) {
					x -= mDepressSpeed;
					SMSRumbleMgr->start(0x13, -1, (f32*)nullptr);
					gpCameraShake->keepShake(CAM_SHAKE_MODE_UNK5, 0.5f);
					SMSGetMSound()->startSoundActor(MSD_SE_OBJ_QUAKE, &unk34[i],
					                                0, nullptr, 0, 4);
					JPABaseEmitter* em = gpMarioParticleManager->emit(
					    MAP_MAP_MS_MARE_BLOCKUP, &unk34[i], 1, &unk34[i]);
					if (em) {
						em->setScale(unk38[i]);
						em->mChildSpawnRate = unk3C[i];
						em->unk174.setAll(unk40[i]);
					}
				} else {
					SMSRumbleMgr->stop(0x13);
				}
				TMapObjBase::setJointTransX(unk30[i], x);
				JGeometry::TVec3<f32> t(x, 0.0f, 0.0f);
				unk28[i].moveTrans(t);
				if (x <= -unk20[i]) {
					unk28[i].remove();
					JGeometry::TVec3<f32> t2(x, 0.0f, 0.0f);
					unk24[i].setUpTrans(t2);
					doneCount += 1;
				}
			} else {
				doneCount += 1;
			}
		}
	}
	if (doneCount == unk10) {
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_CLEAR_SIGN_BIG, 0, nullptr,
		                                   0);
		SMSRumbleMgr->stop(0x13);
		unk48 = 0;
		unk4C = unk18[unk48];
		unk44 = 2;
	}
}

void TMareEventDepressWall::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		switch (unk44) {
		case 0:
			break;
		case 1:
			depressing();
			((TMapObjBase*)this)->calcMap();
			return;
		case 2:
			if (!TMapObjBase::isDemo()) {
				if (unk4C == 0) {
					unk28[unk48].setUp();
					unk24[unk48].remove();
					unk44 = 3;
					return;
				}
				unk4C -= 1;
				return;
			}
			break;
		case 3:
			rising();
			((TMapObjBase*)this)->calcMap();
			return;
		case 4:
			if (!TMapObjBase::isDemo()) {
				if (unk4C == 0) {
					unk44 = 0;
					return;
				}
				unk4C -= 1;
			}
			break;
		}
	}
}

bool TMareEventDepressWall::startEvent()
{
	if (unk44 != 0)
		return false;
	for (int i = 0; i < unk10; ++i) {
		unk28[i].setUp();
		unk24[i].remove();
	}
	unk44 = 1;
	return true;
}

void TMareEventDepressWall::initCommon()
{
	unk18 = new u32[unk10];
	unk20 = new f32[unk10];
	unk1C = new bool[unk10];

	for (int i = 0; i < unk10; ++i) {
		unk20[i] = 400.0f;
		unk1C[i] = true;
	}

	unk24 = new TMapCollisionWarp[unk10];
	unk28 = new TMapCollisionMove[unk10];

	for (int i = 0; i < unk10; ++i) {
		char buf[0x100];
		snprintf(buf, 0x100, "/scene/map/map/building%02d.col", i + unk14);
		unk24[i].init(buf, 0, nullptr);
		unk24[i].setUp();
		unk28[i].init(buf, 0, nullptr);
		unk28[i].setUp();
		unk28[i].remove();
	}

	unk30 = new J3DJoint*[unk10];
	unk34 = new JGeometry::TVec3<f32>[unk10];
	unk38 = new JGeometry::TVec3<f32>[unk10];
	unk3C = new f32[unk10];
	unk40 = new f32[unk10];

	J3DNode* joint = gpMap->getModelManager()
	                     ->getJointModel(0)
	                     ->getModelData()
	                     ->getJointNodePointer(0)
	                     ->getChild()
	                     ->getYounger()
	                     ->getChild();

	int skipCount = 0x43 - (unk14 + (unk10 - 1));
	for (int i = 0; i < skipCount; ++i)
		joint = joint->getYounger();

	for (int i = 0; i < unk10; ++i) {
		int idx          = (unk10 - 1) - i;
		J3DJoint* jJoint = (J3DJoint*)joint;
		unk30[idx]       = jJoint;
		unk34[idx].set((jJoint->mMin.x + jJoint->mMax.x) / 2.0f,
		               (jJoint->mMin.y + jJoint->mMax.y) / 2.0f,
		               (jJoint->mMin.z + jJoint->mMax.z) / 2.0f);

		unk38[idx].x = (jJoint->mMax.x - jJoint->mMin.x) / 100.0f;
		unk38[idx].y = (jJoint->mMax.y - jJoint->mMin.y) / 100.0f;
		unk38[idx].z = (jJoint->mMax.z - jJoint->mMin.z) / 100.0f;

		f32 volume = unk38[idx].x * unk38[idx].y * unk38[idx].z;
		unk3C[idx] = 0.1f * MsSqrtf(volume);
		unk40[idx] = (0.002f * volume) + 1.5f;
		joint      = joint->getYounger();
	}

	SMS_LoadParticle("/scene/map/map/ms_mare_blockup.jpa",
	                 MAP_MAP_MS_MARE_BLOCKUP);
}

void TMareEventDepressWall::init3rdEvent()
{
	unk10 = 7;
	unk14 = 43;
	initCommon();
	TMapModelActor* actor
	    = JDrama::TNameRefGen::search<TMapModelActor>("mareEP2");
	if (actor) {
		actor->setActor((MActor*)this);
		unk18[0] = 2400;
		for (int i = 1; i < unk10; ++i)
			unk18[i] = 120;
		unk20[43 - unk14] = 900.0f;
		unk20[44 - unk14] = 1300.0f;
		unk20[45 - unk14] = 1300.0f;
		unk20[46 - unk14] = 1300.0f;
		unk20[47 - unk14] = 400.0f;
		unk20[48 - unk14] = 300.0f;
		unk20[49 - unk14] = 200.0f;
		unk1C[47 - unk14] = 0;
		unk1C[48 - unk14] = 0;
		unk1C[49 - unk14] = 0;
	}
}

void TMareEventDepressWall::init2ndEvent()
{
	unk10 = 15;
	unk14 = 19;
	initCommon();
	TMapModelActor* actor
	    = JDrama::TNameRefGen::search<TMapModelActor>("mareEP1");
	if (actor) {
		actor->setActor((MActor*)this);
		unk18[0] = 3600;
		for (int i = 1; i < unk10; ++i)
			unk18[i] = 120;
	}
}

void TMareEventDepressWall::init1stEvent()
{
	unk10 = 11;
	unk14 = 8;
	initCommon();
	TMapModelActor* actor
	    = JDrama::TNameRefGen::search<TMapModelActor>("mareEP0");
	if (actor) {
		actor->setActor((MActor*)this);
		unk18[0] = 3600;
		for (int i = 1; i < unk10; ++i)
			unk18[i] = 120;
	}
}

TMareEventDepressWall::TMareEventDepressWall(const char* name)
    : JDrama::TViewObj(name)
{
	unk10 = 0;
	unk14 = 0;
	unk18 = nullptr;
	unk1C = nullptr;
	unk20 = nullptr;
	unk24 = nullptr;
	unk28 = nullptr;
	unk30 = nullptr;
	unk34 = nullptr;
	unk38 = nullptr;
	unk3C = nullptr;
	unk40 = nullptr;
	unk44 = 0;
	unk48 = 0;
	unk4C = 0;
}

u32 TMareEventBumpyWall::touchWater(THitActor*)
{
	switch (unk150) {
	case 0:
		mState = 2;
		break;
	case 2:
		mState = 3;
		break;
	case 1:
		mState = 4;
		break;
	case 3:
		mState = 5;
		break;
	}
	unk148->remove();
	unk14C->setUp();
	return 1;
}

void TMareEventBumpyWall::bumpDownZ()
{
	f32 z = TMapObjBase::getJointTransZ(unk13C);
	JGeometry::TVec3<f32> trans(z, 0.0f, 0.0f);
	if (z > -unk144) {
		if (!TMapObjBase::isDemo()) {
			z -= unk140;
			SMSRumbleMgr->start(0x13, -1, (f32*)nullptr);
			gpCameraShake->keepShake(CAM_SHAKE_MODE_UNK5, 0.5f);
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_QUAKE, &mPosition, 0,
			                                nullptr, 0, 4);
		} else {
			SMSRumbleMgr->stop();
		}
		TMapObjBase::setJointTransZ(unk13C, z);
		unk14C->moveTrans(trans);
		return;
	}
	trans.z = -unk144;
	TMapObjBase::setJointTransZ(unk13C, -unk144);
	unk14C->remove();
	unk148->setUpTrans(trans);
	SMSRumbleMgr->stop(0x13);
	kill();
}

void TMareEventBumpyWall::bumpUpZ()
{
	f32 z = TMapObjBase::getJointTransZ(unk13C);
	JGeometry::TVec3<f32> trans(z, 0.0f, 0.0f);
	if (z < unk144) {
		if (!TMapObjBase::isDemo()) {
			z += unk140;
			SMSRumbleMgr->start(0x13, -1, (f32*)nullptr);
			gpCameraShake->keepShake(CAM_SHAKE_MODE_UNK5, 0.5f);
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_QUAKE, &mPosition, 0,
			                                nullptr, 0, 4);
		} else {
			SMSRumbleMgr->stop(0x13);
		}
		TMapObjBase::setJointTransZ(unk13C, z);
		unk14C->moveTrans(trans);
		return;
	}
	trans.z = unk144;
	TMapObjBase::setJointTransZ(unk13C, unk144);
	unk14C->remove();
	unk148->setUpTrans(trans);
	SMSRumbleMgr->stop(0x13);
	kill();
}

void TMareEventBumpyWall::bumpDownX()
{
	f32 x = TMapObjBase::getJointTransX(unk13C);
	JGeometry::TVec3<f32> trans(x, 0.0f, 0.0f);
	if (x > -unk144) {
		if (!TMapObjBase::isDemo()) {
			x -= unk140;
			SMSRumbleMgr->start(0x13, -1, (f32*)nullptr);
			gpCameraShake->keepShake(CAM_SHAKE_MODE_UNK5, 0.5f);
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_QUAKE, &mPosition, 0,
			                                nullptr, 0, 4);
		} else {
			SMSRumbleMgr->stop(0x13);
		}
		TMapObjBase::setJointTransX(unk13C, x);
		unk14C->moveTrans(trans);
		return;
	}
	trans.x = -unk144;
	TMapObjBase::setJointTransX(unk13C, -unk144);
	unk14C->remove();
	unk148->setUpTrans(trans);
	SMSRumbleMgr->stop(0x13);
	kill();
}

void TMareEventBumpyWall::bumpUpX()
{
	f32 x = TMapObjBase::getJointTransX(unk13C);
	JGeometry::TVec3<f32> trans(x, 0.0f, 0.0f);
	if (x < unk144) {
		if (!TMapObjBase::isDemo()) {
			x += unk140;
			SMSRumbleMgr->start(0x13, -1, (f32*)nullptr);
			gpCameraShake->keepShake(CAM_SHAKE_MODE_UNK5, 0.5f);
			SMSGetMSound()->startSoundActor(MSD_SE_OBJ_QUAKE, &mPosition, 0,
			                                nullptr, 0, 4);
		} else {
			SMSRumbleMgr->stop(0x13);
		}
		TMapObjBase::setJointTransX(unk13C, x);
		unk14C->moveTrans(trans);
		return;
	}
	trans.x = unk144;
	TMapObjBase::setJointTransX(unk13C, unk144);
	unk14C->remove();
	unk148->setUpTrans(trans);
	SMSRumbleMgr->stop(0x13);
	kill();
}

void TMareEventBumpyWall::control()
{
	TMapObjBase::control();
	switch (mState) {
	case 1:
		break;
	case 2:
		bumpUpX();
		break;
	case 3:
		bumpUpZ();
		break;
	case 4:
		bumpDownX();
		break;
	case 5:
		bumpDownZ();
		break;
	}
	if (!isState(STATE_NORMAL))
		calcMap();
}

void TMareEventBumpyWall::load(JSUMemoryInputStream& stream)
{
	TMapObjBase::load(stream);
	stream >> unk138;
	unk13C = TMapObjBase::getBuildingJoint(unk138);
	unk148 = TMapObjBase::newAndInitBuildingCollisionWarp(unk138, nullptr);
	unk14C = TMapObjBase::newAndInitBuildingCollisionMove(unk138, nullptr);
	unk148->setUp();
	unk140 = 2.0f;
	unk144 = 400.0f;
	switch (unk138) {
	case 0x32:
	case 0x33:
	case 0x34:
	case 0x35:
	case 0x36:
	case 0x37:
	case 0x38:
	case 0x39:
	case 0x3A:
	case 0x3F:
	case 0x40:
	case 0x41:
	case 0x42:
	case 0x43:
		unk150 = 0;
		break;
	case 0x22:
	case 0x23:
	case 0x24:
	case 0x25:
	case 0x26:
	case 0x2A:
		unk150 = 1;
		break;
	case 0x27:
	case 0x28:
	case 0x29:
		unk150 = 2;
		break;
	case 0x3B:
	case 0x3C:
	case 0x3D:
	case 0x3E:
		unk150 = 3;
		break;
	}
}

TMareEventBumpyWall::TMareEventBumpyWall(const char* name)
    : TMapObjBase(name)
{
	unk138 = 0;
	unk13C = nullptr;
	unk140 = 0.0f;
	unk144 = 0.0f;
	unk148 = nullptr;
	unk14C = nullptr;
	unk150 = 0;
}
