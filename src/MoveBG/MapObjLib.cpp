#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjTurn.hpp>
#include <MoveBG/MapObjMessenger.hpp>
#include <MoveBG/MapObjLibWave.hpp>
#include <MoveBG/MapObjVibration.hpp>
#include <MoveBG/MapObjWave.hpp>
#include <Enemy/Conductor.hpp>
#include <System/EmitterViewObj.hpp>
#include <Player/MarioAccess.hpp>
#include <Map/Map.hpp>
#include <Map/MapModel.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAMath.hpp>
#include <stdlib.h>

TMapObjTurn::TMapObjTurn(const char* name)
    : THideObjBase(name)
    , unk150(0)
    , unk154(0.0f)
    , unk158(0.0f)
    , unk15C(0.0f)
    , unk160(0.0f)
    , unk164(0.0f)
    , unk168(0)
{
}

void TMapObjTurn::initMapObj()
{
	THideObjBase::initMapObj();
	if (isActorType(0x4000007E)) {
		unk150 = 1;
		unk15C = 0.5f;
		unk164 = 10.0f;
		unk160 = 0.01f;
	}
}

void TMapObjTurn::loadAfter()
{
	THideObjBase::loadAfter();
	if (unk138)
		unk168 = 1;
}

u32 TMapObjTurn::receiveMessage(THitActor* param_1, u32 param_2)
{
	u32 result;
	if (param_2 == 5
	    && (param_1->isActorType(0x2000000E) || param_1->isActorType(0x2000000F)
	        || param_1->isActorType(0x20000010))) {
		unk168 = 1;
		result = 1;
	} else {
		// BUG: should've been THideObjBase?
		result = TMapObjBase::receiveMessage(param_1, param_2);
	}
	return result;
}

void TMapObjTurn::control() { }

void TMapObjTurn::turn() { }

u32 TMapObjTurn::touchWater(THitActor*) { }

TMapObjMessenger::TMapObjMessenger(const char* name)
    : THitActor(name)
    , unk68(0)
{
}

u32 TMapObjMessenger::receiveMessage(THitActor*, u32) { }

f32 TMapObjLibWave::mWaveSpeed = 0.7f;

TMapObjLibWave::TMapObjLibWave(f32 param_1, f32 param_2, f32 param_3,
                               f32 param_4)
{
	unk0  = rand() * (1.0f / RAND_MAX) * 360.0f;
	unk4  = param_1;
	unk8  = 0.0f;
	unkC  = param_2;
	unk10 = param_3;
	unk14 = param_4;
}

void TMapObjLibWave::movement()
{
	if (unk8 > 0.0f) {
		unk8 -= unk14;
		if (unk8 < 0.0f)
			unk8 = 0.0f;
	}

	unk0 += mWaveSpeed;

	if (unk0 > 360.0f)
		unk0 -= 360.0f;
}

f32 TMapObjLibWave::getCurrentHeight(f32 param_1, f32 param_2) const
{
	return gpMapObjWave->getWaveHeight(param_1, param_2) - unk8;
}

void TMapObjLibWave::push()
{
	unk8 += unk10;
	if (unk8 > unkC)
		unk8 = unkC;
}

TMapObjVibration::TMapObjVibration() { }

void TMapObjVibration::movement() { }

void TMapObjVibration::startQuickly(f32) { }

void TMapObjVibration::startSlowly(f32) { }

JPABaseEmitter* TMapObjBase::emitAndRotate(s32, u8,
                                           const JGeometry::TVec3<f32>*) const
{
}

JPABaseEmitter*
TMapObjBase::emitAndScale(s32 param_1, u8 param_2,
                          const JGeometry::TVec3<f32>* param_3,
                          const JGeometry::TVec3<f32>& param_4) const
{
	JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(param_1, param_3, param_2, this);

	if (emitter) {
		emitter->unk154.x = param_4.x;
		emitter->unk154.y = param_4.y;
		emitter->unk154.z = param_4.z;
		emitter->unk174.x = param_4.x;
		emitter->unk174.y = param_4.y;
		emitter->unk174.z = param_4.z;
	}

	return emitter;
}

JPABaseEmitter*
TMapObjBase::emitAndBindScale(s32 param_1, u8 param_2,
                              const JGeometry::TVec3<f32>* param_3,
                              const JGeometry::TVec3<f32>& param_4) const
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
	    param_1, param_3, param_2, this);

	if (emitter) {
		emitter->unk154.x = param_4.x;
		emitter->unk154.y = param_4.y;
		emitter->unk154.z = param_4.z;
		emitter->unk174.x = param_4.x;
		emitter->unk174.y = param_4.y;
		emitter->unk174.z = param_4.z;
	}

	return emitter;
}

JPABaseEmitter*
TMapObjBase::emitAndScale(s32 param_1, u8 param_2,
                          const JGeometry::TVec3<f32>* param_3) const
{
	JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(param_1, param_3, param_2, this);

	if (emitter) {
		emitter->unk154.x = mScaling.x;
		emitter->unk154.y = mScaling.y;
		emitter->unk154.z = mScaling.z;
		emitter->unk174.x = mScaling.x;
		emitter->unk174.y = mScaling.y;
		emitter->unk174.z = mScaling.z;
	}

	return emitter;
}

JPABaseEmitter*
TMapObjBase::emitAndRotateScale(s32 param_1, u8 param_2,
                                const JGeometry::TVec3<f32>* param_3) const
{
	JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(param_1, param_3, param_2, this);

	if (emitter) {
		emitter->unk16C.x = mRotation.x / 180.0f * 32768.0f;
		emitter->unk16C.y = mRotation.y / 180.0f * 32768.0f;
		emitter->unk16C.z = mRotation.z / 180.0f * 32768.0f;
		JPAGetXYZRotateMtx(emitter->unk16C.x, emitter->unk16C.y,
		                   emitter->unk16C.z, emitter->unk124);

		emitter->unk154.x = mScaling.x;
		emitter->unk154.y = mScaling.y;
		emitter->unk154.z = mScaling.z;
		emitter->unk174.x = mScaling.x;
		emitter->unk174.y = mScaling.y;
		emitter->unk174.z = mScaling.z;
	}

	return emitter;
}

JPABaseEmitter* TMapObjBase::emitAndSRT(s32 param_1, u8 param_2,
                                        const JGeometry::TVec3<f32>* param_3,
                                        const JGeometry::TVec3<f32>& param_4,
                                        const JGeometry::TVec3<f32>& param_5)
{
	JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(param_1, param_3, param_2, this);

	if (emitter) {
		emitter->unk16C.x = param_3->x / 180.0f * 32768.0f;
		emitter->unk16C.y = param_3->y / 180.0f * 32768.0f;
		emitter->unk16C.z = param_3->z / 180.0f * 32768.0f;
		JPAGetXYZRotateMtx(emitter->unk16C.x, emitter->unk16C.y,
		                   emitter->unk16C.z, emitter->unk124);

		emitter->unk154.x = param_4.x;
		emitter->unk154.y = param_4.y;
		emitter->unk154.z = param_4.z;
		emitter->unk174.x = param_4.x;
		emitter->unk174.y = param_4.y;
		emitter->unk174.z = param_4.z;
	}

	return emitter;
}

void TMapObjBase::emitColumnWater() { }

void TMapObjBase::marioHipAttack() const { }

void TMapObjBase::marioHeadAttack() const { }

void TMapObjBase::marioIsOn() const { }

void TMapObjBase::marioIsOn(const TLiveActor*) { }

void TMapObjBase::actorIsOn(TLiveActor*) const { }

void TMapObjBase::sendMsgToAll(u32) { }

void TMapObjBase::sendMsg(u32, u32) { }

void TMapObjBase::waterHitPlane(THitActor*) { }

void TMapObjBase::getWaterPos(THitActor*) { }

void TMapObjBase::getWaterSpeed(THitActor*) { }

void TMapObjBase::getWaterPlane(THitActor*) { }

void TMapObjBase::getWaterID(THitActor* actor) { }

void TMapObjBase::getDistance(const JGeometry::TVec3<f32>&) const { }

void TMapObjBase::getDistanceXZ(const JGeometry::TVec3<f32>&) const { }

void TMapObjBase::getRotYFromAxisZ(const JGeometry::TVec3<f32>&) const { }

void TMapObjBase::getRotYFromAxisX(const JGeometry::TVec3<f32>&) const { }

void TMapObjBase::makeVecToLocalX(f32, JGeometry::TVec3<f32>*) const { }

void TMapObjBase::makeVecToLocalZ(f32, JGeometry::TVec3<f32>*) const { }

void TMapObjBase::getNormalVecFromTarget(f32, f32, f32,
                                         JGeometry::TVec3<f32>*) const
{
}

void TMapObjBase::getNormalVecFromOffset(f32, f32, f32, JGeometry::TVec3<f32>*)
{
}

void TMapObjBase::getNormalVecFromTargetXZ(f32, f32,
                                           JGeometry::TVec3<f32>*) const
{
}

void TMapObjBase::getNormalVecFromOffsetXZ(f32, f32, JGeometry::TVec3<f32>*) { }

void TMapObjBase::rotateVecByAxisY(JGeometry::TVec3<f32>*, f32) { }

void TMapObjBase::getVerticalVecFromOffsetXZ(f32, f32, JGeometry::TVec3<f32>*)
{
}

void TMapObjBase::getVerticalVecToTargetXZ(f32, f32,
                                           JGeometry::TVec3<f32>*) const
{
}

void TMapObjBase::calcReflectingVelocity(const TBGCheckData*, f32,
                                         JGeometry::TVec3<f32>*) const
{
}

void TMapObjBase::makeObjMtxRotByAxis(const JGeometry::TVec3<f32>&, f32,
                                      f32 (*)[4]) const
{
}

void TMapObjBase::makeMtxRotByAxis(const JGeometry::TVec3<f32>&, f32,
                                   f32 (*)[4])
{
}

void TMapObjBase::concatOnlyRotFromRight(f32 (*)[4], f32 (*)[4], f32 (*)[4]) { }

void TMapObjBase::concatOnlyRotFromLeft(f32 (*)[4], f32 (*)[4], f32 (*)[4]) { }

void TMapObjBase::updateObjMtx() { }

void TMapObjBase::setRootMtxTrans() { }

void TMapObjBase::makeRootMtxTrans(f32 (*)[4]) { }

void TMapObjBase::updateRootMtxTrans() { }

void TMapObjBase::setRootMtxRotX() { }

void TMapObjBase::makeRootMtxRotX(f32 (*)[4]) { }

void TMapObjBase::setRootMtxRotY() { }

void TMapObjBase::makeRootMtxRotY(f32 (*)[4]) { }

void TMapObjBase::setRootMtxRotZ() { }

void TMapObjBase::makeRootMtxRotZ(f32 (*)[4]) { }

void TMapObjBase::makeLowerStr(const char*, char*) { }

void TMapObjBase::moveJoint(J3DJoint*, f32, f32, f32) { }

f32 TMapObjBase::getJointTransX(J3DJoint* joint)
{
	return joint->getTransformInfo().mTranslate.x;
}

f32 TMapObjBase::getJointTransY(J3DJoint* joint)
{
	return joint->getTransformInfo().mTranslate.y;
}

f32 TMapObjBase::getJointTransZ(J3DJoint* joint)
{
	return joint->getTransformInfo().mTranslate.z;
}

void TMapObjBase::setJointTrans(J3DJoint*, f32, f32, f32) { }

void TMapObjBase::setJointTransX(J3DJoint* joint, f32 x)
{
	J3DTransformInfo& info = joint->getTransformInfo();
	info.mTranslate.x      = x;
	joint->setTransformInfo(info);
}

void TMapObjBase::setJointTransY(J3DJoint* joint, f32 y)
{
	J3DTransformInfo& info = joint->getTransformInfo();
	info.mTranslate.y      = y;
	joint->setTransformInfo(info);
}

void TMapObjBase::setJointTransZ(J3DJoint* joint, f32 z)
{
	J3DTransformInfo& info = joint->getTransformInfo();
	info.mTranslate.z      = z;
	joint->setTransformInfo(info);
}

f32 TMapObjBase::getJointRotateX(J3DJoint* joint)
{
	return joint->getTransformInfo().mRotation.x;
}

f32 TMapObjBase::getJointRotateY(J3DJoint* joint)
{
	return joint->getTransformInfo().mRotation.y;
}

f32 TMapObjBase::getJointRotateZ(J3DJoint* joint)
{
	return joint->getTransformInfo().mRotation.z;
}

void TMapObjBase::setJointRotate(J3DJoint*, short, short, short) { }

void TMapObjBase::setJointRotateX(J3DJoint*, short) { }

void TMapObjBase::setJointRotateY(J3DJoint*, short) { }

void TMapObjBase::setJointRotateZ(J3DJoint*, short) { }

void TMapObjBase::rotateJointX(J3DJoint*, f32) { }

void TMapObjBase::rotateJointY(J3DJoint*, f32) { }

void TMapObjBase::rotateJointZ(J3DJoint*, f32) { }

f32 TMapObjBase::getJointScaleX(J3DJoint* joint)
{
	return joint->getTransformInfo().mScale.x;
}

f32 TMapObjBase::getJointScaleY(J3DJoint* joint)
{
	return joint->getTransformInfo().mScale.y;
}

f32 TMapObjBase::getJointScaleZ(J3DJoint* joint)
{
	return joint->getTransformInfo().mScale.z;
}

void TMapObjBase::setJointScale(J3DJoint*, f32, f32, f32) { }

void TMapObjBase::setJointScaleX(J3DJoint*, f32) { }

void TMapObjBase::setJointScaleY(J3DJoint*, f32) { }

void TMapObjBase::setJointScaleZ(J3DJoint*, f32) { }

void TMapObjBase::calcMap() { }

void TMapObjBase::getMapModel() { }

void TMapObjBase::getMapModelData() { }

void TMapObjBase::getMapMActor() { }

TJointObj* TMapObjBase::getBuildingJointObj(int i)
{
	return gpMap->getModelManager()->getJointModel(0)->getChild(0)->getChild(
	    (u16)i);
}

J3DJoint* TMapObjBase::getBuildingJoint(int i)
{
	return getBuildingJointObj(i)->getJoint();
}

TMapCollisionStatic* TMapObjBase::newAndInitBuildingCollisionStatic(int,
                                                                    TLiveActor*)
{
}

TMapCollisionMove*
TMapObjBase::newAndInitBuildingCollisionMove(int param_1, TLiveActor* param_2)
{
	TMapCollisionMove* move = new TMapCollisionMove;
	char buffer[64];
	snprintf(buffer, 64, "/scene/map/map/building%02d.col", param_1);
	move->init(buffer, 0, param_2);
	return move;
}

TMapCollisionWarp*
TMapObjBase::newAndInitBuildingCollisionWarp(int param_1, TLiveActor* param_2)
{
	TMapCollisionWarp* warp = new TMapCollisionWarp;
	char buffer[64];
	snprintf(buffer, 64, "/scene/map/map/building%02d.col", param_1);
	warp->init(buffer, 0, param_2);
	return warp;
}

void TMapObjBase::joinToGroup(const char*, THitActor*) { }

void TMapObjBase::startAllAnim(MActor*, const char*) { }

void TMapObjBase::initPacketMatColor(J3DModel*, _GXTevRegID, const _GXColorS10*)
{
}

void TMapObjBase::isFruit(THitActor*) { }

void TMapObjBase::isCoin(THitActor*) { }

void TMapObjBase::throwObjFromPointWithRot(TMapObjBase*,
                                           const JGeometry::TVec3<f32>&,
                                           const JGeometry::TVec3<f32>&, f32,
                                           f32)
{
}

void TMapObjBase::throwObjToFrontFromPoint(TMapObjBase*,
                                           const JGeometry::TVec3<f32>&, f32,
                                           f32) const
{
}

void TMapObjBase::throwObjToFront(TMapObjBase*, f32, f32, f32) const { }

void TMapObjBase::throwObjToOverhead(TMapObjBase*, f32, f32) const { }

void TMapObjBase::checkOnManhole() { }

void TMapObjBase::loadHideObjInfo(JSUMemoryInputStream&, s32*, f32*, f32*, s32*)
{
}

void TMapObjBase::isDemo() { }

void TMapObjBase::isHideObj(THitActor*) { }
