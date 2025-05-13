#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjTurn.hpp>
#include <MoveBG/MapObjMessenger.hpp>
#include <MoveBG/MapObjLibWave.hpp>
#include <MoveBG/MapObjVibration.hpp>
#include <MoveBG/MapObjWave.hpp>
#include <MoveBG/MapObjTown.hpp>
#include <Enemy/Conductor.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/ModelWaterManager.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapModel.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAMath.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <stdlib.h>
#include <ctype.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

bool TMapObjBase::isHideObj(THitActor* param_1)
{
	if (param_1->isActorType(0x40000320) || param_1->isActorType(0x40000013)
	    || param_1->isActorType(0x40000012) || param_1->isActorType(0x40000018)
	    || param_1->isActorType(0x40000019) || param_1->isActorType(0x4000001a)
	    || param_1->isActorType(0x40000020) || param_1->isActorType(0x4000003b))
		return true;

	return false;
}

bool TMapObjBase::isDemo()
{
	bool b1 = true;
	if (gpMarDirector->unk124 != 1 && gpMarDirector->unk124 != 2)
		b1 = false;

	if (!b1) {
		// TODO: should be OR, but need fancy inlines for that...
		bool b2 = true;
		if (gpMarDirector->unk124 != 3 && gpMarDirector->unk124 != 4)
			b2 = false;
		if (b2) {
			return true;
		}
	}
	return false;
}

void TMapObjBase::loadHideObjInfo(JSUMemoryInputStream& stream, s32* param_2,
                                  f32* param_3, f32* param_4, s32* param_5)
{
	stream.read(param_2, 4);
	f32 val;
	stream.read(&val, 4);
	*param_3 = val * 0.06f;
	stream.read(&val, 4);
	if (val < 0.0f)
		val = 20.0f;
	*param_4 = val;
	s32 val2;
	stream.read(&val2, 4);
	if (val2 <= 0)
		val2 = 1200;
	else
		val2 *= 10;
	*param_5 = val2;
}

void TMapObjBase::checkOnManhole()
{
	unkC8 = gpMap->checkGround(mPosition.x, mPosition.y + 20.0f, mPosition.z,
	                           &unkC4);
	if (unkC4->unk44 && unkC4->unk44->isActorType(0x4000000b)) {
		((TManhole*)unkC4->unk44)->makeManholeUnuseful(this);
	}
}

void TMapObjBase::throwObjToOverhead(TMapObjBase* param_1, f32 param_2,
                                     f32 param_3) const
{
}

void TMapObjBase::throwObjToFront(TMapObjBase* param_1, f32 param_2,
                                  f32 param_3, f32 param_4) const
{
	param_1->appear();
	param_1->mPosition.set(mPosition.x, mPosition.y + param_2, mPosition.z);
	if (unk74) {
		MtxPtr mtx = getModel()->getAnmMtx(0);
		param_1->unkAC.set(mtx[0][2] * param_3, mtx[1][2] * param_3 + param_4,
		                   mtx[2][2] * param_3);
		param_1->offLiveFlag(0x10);
	} else {
		Mtx mtx;
		MsMtxSetRotRPH(mtx, mRotation.x, mRotation.y, mRotation.z);
		param_1->unkAC.set(mtx[0][2] * param_3, mtx[1][2] * param_3 + param_4,
		                   mtx[2][2] * param_3);
		param_1->offLiveFlag(0x10);
	}
}

void TMapObjBase::throwObjToFrontFromPoint(TMapObjBase* param_1,
                                           const JGeometry::TVec3<f32>& param_2,
                                           f32 param_3, f32 param_4) const
{
	param_1->appear();
	param_1->mPosition.set(param_2);
	if (unk74) {
		MtxPtr mtx = getModel()->getAnmMtx(0);
		param_1->unkAC.set(mtx[0][2] * param_3, mtx[1][2] * param_3 + param_4,
		                   mtx[2][2] * param_3);
		param_1->offLiveFlag(0x10);
	} else {
		Mtx mtx;
		MsMtxSetRotRPH(mtx, mRotation.x, mRotation.y, mRotation.z);
		param_1->unkAC.set(mtx[0][2] * param_3, mtx[1][2] * param_3 + param_4,
		                   mtx[2][2] * param_3);
		param_1->offLiveFlag(0x10);
	}
}

void TMapObjBase::throwObjFromPointWithRot(TMapObjBase* param_1,
                                           const JGeometry::TVec3<f32>& param_2,
                                           const JGeometry::TVec3<f32>& param_3,
                                           f32 param_4, f32 param_5)
{
	param_1->appear();
	param_1->mPosition.set(param_2);

	Mtx mtx;
	MsMtxSetRotRPH(mtx, param_3.x, param_3.y, param_3.z);
	param_1->unkAC.set(mtx[0][2] * param_4, mtx[1][2] * param_4 + param_5,
	                   mtx[2][2] * param_4);
	param_1->offLiveFlag(0x10);
}

bool TMapObjBase::isCoin(THitActor* param_1)
{
	if (param_1->isActorType(0x2000000e) || param_1->isActorType(0x2000000f)
	    || param_1->isActorType(0x20000010))
		return true;

	return false;
}

bool TMapObjBase::isFruit(THitActor* param_1)
{
	if (param_1->isActorType(0x40000390) || param_1->isActorType(0x40000391)
	    || param_1->isActorType(0x40000392) || param_1->isActorType(0x40000393)
	    || param_1->isActorType(0x40000394) || param_1->isActorType(0x40000395))
		return true;
	return false;
}

void TMapObjBase::initPacketMatColor(J3DModel* param_1, GXTevRegID param_2,
                                     const GXColorS10* param_3)
{
	for (u16 i = 0; i < param_1->getModelData()->getMaterialNum(); ++i)
		SMS_InitPacket_OneTevColor(param_1, i, param_2, param_3);
}

void TMapObjBase::startAllAnim(MActor* param_1, const char* param_2)
{
	char lowerCased[128];
	makeLowerStr(param_2, lowerCased);

	if (param_1->checkAnmFileExist(lowerCased, 0))
		param_1->setBck(lowerCased);
	if (param_1->checkAnmFileExist(lowerCased, 5))
		param_1->setBrk(lowerCased);
	if (param_1->checkAnmFileExist(lowerCased, 2))
		param_1->setBpk(lowerCased);
	if (param_1->checkAnmFileExist(lowerCased, 3))
		param_1->setBtp(lowerCased);
	if (param_1->checkAnmFileExist(lowerCased, 4))
		param_1->setBtk(lowerCased);
}

void TMapObjBase::joinToGroup(const char* param_1, THitActor* param_2)
{
	// TODO: The group type here is a wild guess
	JDrama::TNameRefGen::search<JDrama::TViewObjPtrListT<THitActor> >(param_1)
	    ->push_back(param_2);
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

TMapCollisionMove*
TMapObjBase::newAndInitBuildingCollisionMove(int param_1, TLiveActor* param_2)
{
	TMapCollisionMove* move = new TMapCollisionMove;
	char buffer[64];
	snprintf(buffer, 64, "/scene/map/map/building%02d.col", param_1);
	move->init(buffer, 0, param_2);
	return move;
}

TMapCollisionStatic* TMapObjBase::newAndInitBuildingCollisionStatic(int,
                                                                    TLiveActor*)
{
}

J3DJoint* TMapObjBase::getBuildingJoint(int i)
{
	return getBuildingJointObj(i)->getJoint();
}

TJointObj* TMapObjBase::getBuildingJointObj(int i)
{
	return gpMap->getModelManager()->getJointModel(0)->getChild(0)->getChild(
	    (u16)i);
}

void TMapObjBase::getMapMActor() { }

void TMapObjBase::getMapModelData() { }

void TMapObjBase::getMapModel() { }

void TMapObjBase::calcMap()
{
	gpMap->getModelManager()->getJointModel(0)->getModel()->calc();
}

void TMapObjBase::setJointScaleZ(J3DJoint*, f32) { }

void TMapObjBase::setJointScaleY(J3DJoint* joint, f32 scale)
{
	J3DTransformInfo& info = joint->getTransformInfo();
	info.mScale.y          = scale;
	joint->setTransformInfo(info);
}

void TMapObjBase::setJointScaleX(J3DJoint*, f32) { }

void TMapObjBase::setJointScale(J3DJoint*, f32, f32, f32) { }

f32 TMapObjBase::getJointScaleZ(J3DJoint* joint)
{
	return joint->getTransformInfo().mScale.z;
}

f32 TMapObjBase::getJointScaleY(J3DJoint* joint)
{
	return joint->getTransformInfo().mScale.y;
}

f32 TMapObjBase::getJointScaleX(J3DJoint* joint)
{
	return joint->getTransformInfo().mScale.x;
}

void TMapObjBase::rotateJointZ(J3DJoint*, f32) { }

void TMapObjBase::rotateJointY(J3DJoint*, f32) { }

void TMapObjBase::rotateJointX(J3DJoint*, f32) { }

void TMapObjBase::setJointRotateZ(J3DJoint*, short) { }

void TMapObjBase::setJointRotateY(J3DJoint*, short) { }

void TMapObjBase::setJointRotateX(J3DJoint*, short) { }

void TMapObjBase::setJointRotate(J3DJoint*, short, short, short) { }

f32 TMapObjBase::getJointRotateZ(J3DJoint* joint)
{
	return joint->getTransformInfo().mRotation.z;
}

f32 TMapObjBase::getJointRotateY(J3DJoint* joint)
{
	return joint->getTransformInfo().mRotation.y;
}

f32 TMapObjBase::getJointRotateX(J3DJoint* joint)
{
	return joint->getTransformInfo().mRotation.x;
}

void TMapObjBase::setJointTransZ(J3DJoint* joint, f32 z)
{
	J3DTransformInfo& info = joint->getTransformInfo();
	info.mTranslate.z      = z;
	joint->setTransformInfo(info);
}

void TMapObjBase::setJointTransY(J3DJoint* joint, f32 y)
{
	J3DTransformInfo& info = joint->getTransformInfo();
	info.mTranslate.y      = y;
	joint->setTransformInfo(info);
}

void TMapObjBase::setJointTransX(J3DJoint* joint, f32 x)
{
	J3DTransformInfo& info = joint->getTransformInfo();
	info.mTranslate.x      = x;
	joint->setTransformInfo(info);
}

void TMapObjBase::setJointTrans(J3DJoint*, f32, f32, f32) { }

f32 TMapObjBase::getJointTransZ(J3DJoint* joint)
{
	return joint->getTransformInfo().mTranslate.z;
}

f32 TMapObjBase::getJointTransY(J3DJoint* joint)
{
	return joint->getTransformInfo().mTranslate.y;
}

f32 TMapObjBase::getJointTransX(J3DJoint* joint)
{
	return joint->getTransformInfo().mTranslate.x;
}

void TMapObjBase::moveJoint(J3DJoint* joint, f32 x, f32 y, f32 z)
{
	J3DTransformInfo& info = joint->getTransformInfo();
	info.mTranslate.x += x;
	info.mTranslate.y += y;
	info.mTranslate.z += z;
	joint->setTransformInfo(info);
}

void TMapObjBase::makeLowerStr(const char* in, char* out)
{
	for (; *in; ++in, ++out)
		*out = std::tolower(*in);
	*out = 0;
}

void TMapObjBase::makeRootMtxRotZ(MtxPtr ptr)
{
	f32 fVar1 = sinf(mRotation.z * (M_PI / 180.0f));
	f32 fVar2 = cosf(mRotation.z * (M_PI / 180.0f));

	ptr[0][0] = fVar2;
	ptr[0][1] = -fVar1;
	ptr[0][2] = 0.0f;
	ptr[0][3] = mPosition.x;

	ptr[1][0] = fVar1;
	ptr[1][1] = fVar2;
	ptr[1][2] = 0.0f;
	ptr[1][3] = mPosition.y - unk108;

	ptr[2][0] = 0.0f;
	ptr[2][1] = 0.0f;
	ptr[2][2] = 1.0f;
	ptr[2][3] = mPosition.z;
}

void TMapObjBase::setRootMtxRotZ()
{
	makeRootMtxRotZ(getModel()->getAnmMtx(0));
}

void TMapObjBase::makeRootMtxRotY(MtxPtr ptr)
{
	f32 fVar1 = sinf(mRotation.y * (M_PI / 180.0f));
	f32 fVar2 = cosf(mRotation.y * (M_PI / 180.0f));

	ptr[0][0] = fVar2;
	ptr[0][1] = 0.0f;
	ptr[0][2] = fVar1;
	ptr[0][3] = mPosition.x;

	ptr[1][0] = 0.0f;
	ptr[1][1] = 1.0f;
	ptr[1][2] = 0.0f;
	ptr[1][3] = mPosition.y - unk108;

	ptr[2][0] = -fVar1;
	ptr[2][1] = 0.0f;
	ptr[2][2] = fVar2;
	ptr[2][3] = mPosition.z;
}

void TMapObjBase::setRootMtxRotY()
{
	makeRootMtxRotY(getModel()->getAnmMtx(0));
}

void TMapObjBase::makeRootMtxRotX(MtxPtr ptr)
{
	f32 fVar1 = sinf(mRotation.x * (M_PI / 180.0f));
	f32 fVar2 = cosf(mRotation.x * (M_PI / 180.0f));

	ptr[0][0] = 1.0f;
	ptr[0][1] = 0.0f;
	ptr[0][2] = 0.0f;
	ptr[0][3] = mPosition.x;

	ptr[1][0] = 0.0f;
	ptr[1][1] = fVar2;
	ptr[1][2] = -fVar1;
	ptr[1][3] = mPosition.y - unk108;

	ptr[2][0] = 0.0f;
	ptr[2][1] = fVar1;
	ptr[2][2] = fVar2;
	ptr[2][3] = mPosition.z;
}

void TMapObjBase::setRootMtxRotX()
{
	makeRootMtxRotX(getModel()->getAnmMtx(0));
}

void TMapObjBase::updateRootMtxTrans()
{
	MtxPtr mtx = getModel()->getAnmMtx(0);
	// BUG: oops, all coord go into the same matrix cell!
	mtx[0][3] = mPosition.x;
	mtx[0][3] = mPosition.y - unk108;
	mtx[0][3] = mPosition.z;
}

void TMapObjBase::makeRootMtxTrans(MtxPtr) { }

void TMapObjBase::setRootMtxTrans() { }

void TMapObjBase::updateObjMtx()
{
	JGeometry::TVec3<f32> pos2(mPosition.x, mPosition.y - unk108, mPosition.z);
	const JGeometry::TVec3<f32>& rot = getRotation();
	MsMtxSetXYZRPH(getModel()->getAnmMtx(0), pos2.x, pos2.y, pos2.z,
	               rot.x * (65536.0f / 360.0f), rot.y * (65536.0f / 360.0f),
	               rot.z * (65536.0f / 360.0f));
}

void TMapObjBase::concatOnlyRotFromLeft(MtxPtr param_1, MtxPtr param_2,
                                        MtxPtr param_3)
{
	f32 fVar1 = param_2[0][3];
	f32 fVar2 = param_2[1][3];
	f32 fVar3 = param_2[2][3];

	param_2[0][3] = 0.0f;
	param_2[1][3] = 0.0f;
	param_2[2][3] = 0.0f;

	MTXConcat(param_1, param_2, param_3);

	param_3[0][3] = fVar1;
	param_3[1][3] = fVar2;
	param_3[2][3] = fVar3;
}

void TMapObjBase::concatOnlyRotFromRight(MtxPtr param_1, MtxPtr param_2,
                                         MtxPtr param_3)
{
	f32 fVar1 = param_1[0][3];
	f32 fVar2 = param_1[1][3];
	f32 fVar3 = param_1[2][3];

	param_1[0][3] = 0.0f;
	param_1[1][3] = 0.0f;
	param_1[2][3] = 0.0f;

	MTXConcat(param_1, param_2, param_3);

	param_3[0][3] = fVar1;
	param_3[1][3] = fVar2;
	param_3[2][3] = fVar3;
}

void TMapObjBase::makeMtxRotByAxis(const JGeometry::TVec3<f32>&, f32, MtxPtr) {
}

void TMapObjBase::makeObjMtxRotByAxis(const JGeometry::TVec3<f32>&, f32,
                                      MtxPtr) const
{
}

void TMapObjBase::calcReflectingVelocity(const TBGCheckData* param_1,
                                         f32 param_2,
                                         JGeometry::TVec3<f32>* param_3) const
{
	f32 fVar2 = param_3->dot(param_1->mNormal);
	param_3->x -= (param_2 + 1.0f) * fVar2 * param_1->mNormal.x;
	param_3->y -= (param_2 + 1.0f) * fVar2 * param_1->mNormal.y;
	param_3->z -= (param_2 + 1.0f) * fVar2 * param_1->mNormal.z;
}

void TMapObjBase::getVerticalVecToTargetXZ(f32, f32,
                                           JGeometry::TVec3<f32>*) const
{
}

void TMapObjBase::getVerticalVecFromOffsetXZ(f32, f32, JGeometry::TVec3<f32>*)
{
}

void TMapObjBase::rotateVecByAxisY(JGeometry::TVec3<f32>*, f32) { }

void TMapObjBase::getNormalVecFromOffsetXZ(f32, f32, JGeometry::TVec3<f32>*) { }

void TMapObjBase::getNormalVecFromTargetXZ(f32 param_1, f32 param_2,
                                           JGeometry::TVec3<f32>* param_3) const
{
	param_3->set(param_1 - mPosition.x, 0.0f, param_2 - mPosition.z);
	MsVECNormalize(param_3, param_3);
}

void TMapObjBase::getNormalVecFromOffset(f32, f32, f32, JGeometry::TVec3<f32>*)
{
}

void TMapObjBase::getNormalVecFromTarget(f32 param_1, f32 param_2, f32 param_3,
                                         JGeometry::TVec3<f32>* param_4) const
{
	// BUG: copy-pasta from above but forgot to change x to y
	param_4->set(param_1 - mPosition.x, param_2 - mPosition.x,
	             param_3 - mPosition.z);
	MsVECNormalize(param_4, param_4);
}

void TMapObjBase::makeVecToLocalZ(f32 param_1,
                                  JGeometry::TVec3<f32>* param_2) const
{
	Mtx mtx;
	MsMtxSetRotRPH(mtx, mRotation.x, mRotation.y, mRotation.z);
	param_2->x = 0.0f;
	param_2->y = 0.0f;
	param_2->z = param_1;
	MTXMultVec(mtx, param_2, param_2);
}

void TMapObjBase::makeVecToLocalX(f32 param_1,
                                  JGeometry::TVec3<f32>* param_2) const
{
	Mtx mtx;
	MsMtxSetRotRPH(mtx, mRotation.x, mRotation.y, mRotation.z);
	param_2->x = param_1;
	param_2->y = 0.0f;
	param_2->z = 0.0f;
	MTXMultVec(mtx, param_2, param_2);
}

f32 TMapObjBase::getRotYFromAxisX(const JGeometry::TVec3<f32>& param_1) const
{
	return std::atan2f(param_1.z - mPosition.z, param_1.x - mPosition.x);
}

f32 TMapObjBase::getRotYFromAxisZ(const JGeometry::TVec3<f32>& param_1) const
{
	return std::atan2f(param_1.z - mPosition.z, param_1.x - mPosition.x);
}

f32 TMapObjBase::getDistanceXZ(const JGeometry::TVec3<f32>& param_1) const
{
	return JGeometry::TUtil<f32>::sqrt(
	    (param_1.x - mPosition.x) * (param_1.x - mPosition.x)
	    + (param_1.z - mPosition.z) * (param_1.z - mPosition.z));
}

f32 TMapObjBase::getDistance(const JGeometry::TVec3<f32>& param_1) const
{
	JGeometry::TVec3<f32> diff;
	diff.sub(param_1, mPosition);
	return diff.length();
}

void TMapObjBase::getWaterID(THitActor*) { }

void TMapObjBase::getWaterPlane(THitActor*) { }

void TMapObjBase::getWaterSpeed(THitActor*) { }

void TMapObjBase::getWaterPos(THitActor*) { }

void TMapObjBase::waterHitPlane(THitActor*) { }

void TMapObjBase::sendMsg(u32 param_1, u32 param_2)
{
	for (int i = 0; i < getColNum(); ++i) {
		THitActor* col = getCollision(i);
		if (col->isActorType(param_1))
			col->receiveMessage(this, param_2);
	}
}

void TMapObjBase::sendMsgToAll(u32) { }

void TMapObjBase::actorIsOn(TLiveActor*) const { }

bool TMapObjBase::marioIsOn(const TLiveActor* param_1)
{
	if (SMS_GetMarioGrPlane()->getUnk44() == param_1
	    && SMS_IsMarioTouchGround4cm())
		return true;

	return false;
}

bool TMapObjBase::marioIsOn() const
{
	if (SMS_GetMarioGrPlane()->getUnk44() == this
	    && SMS_IsMarioTouchGround4cm())
		return true;

	return false;
}

bool TMapObjBase::marioHeadAttack() const { }

void TMapObjBase::marioHipAttack() const { }

void TMapObjBase::emitColumnWater() { }

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

JPABaseEmitter* TMapObjBase::emitAndRotate(s32, u8,
                                           const JGeometry::TVec3<f32>*) const
{
}

void TMapObjVibration::startSlowly(f32) { }

void TMapObjVibration::startQuickly(f32) { }

void TMapObjVibration::movement() { }

TMapObjVibration::TMapObjVibration() { }

void TMapObjLibWave::push()
{
	unk8 += unk10;
	if (unk8 > unkC)
		unk8 = unkC;
}

f32 TMapObjLibWave::getCurrentHeight(f32 param_1, f32 param_2) const
{
	return gpMapObjWave->getWaveHeight(param_1, param_2) - unk8;
}

f32 TMapObjLibWave::mWaveSpeed = 0.7f;

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

TMapObjLibWave::TMapObjLibWave(f32 param_1, f32 param_2, f32 param_3,
                               f32 param_4)
{
	unk0  = rand() * (1.0f / (RAND_MAX + 1)) * 360.0f;
	unk4  = param_1;
	unk8  = 0.0f;
	unkC  = param_2;
	unk10 = param_3;
	unk14 = param_4;
}

u32 TMapObjMessenger::receiveMessage(THitActor*, u32) { }

TMapObjMessenger::TMapObjMessenger(const char* name)
    : THitActor(name)
    , unk68(0)
{
}

u32 TMapObjTurn::touchWater(THitActor*) { }

void TMapObjTurn::turn() { }

void TMapObjTurn::control() { }

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

void TMapObjTurn::loadAfter()
{
	THideObjBase::loadAfter();
	if (unk138)
		unk168 = 1;
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
