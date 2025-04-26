#include <MoveBG/MapObjBase.hpp>
#include <Map/Map.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

TMapObjBase::TMapObjBase(const char* name)
    : TLiveActor(name)
    , unkF4(nullptr)
    , unkF8(0)
    , unkFC(1)
    , unkFE(0xffff)
    , unk100(0xffff)
    , unk102(0)
    , unk104(0)
    , unk108(0.0f)
    , unk130(nullptr)
    , unk134(0)
{
	unk10C.x = unk10C.y = unk10C.z = 0.0f;
	unk118.x = unk118.y = unk118.z = 0.0f;

	unk124.x = 1.0f;
	unk124.y = 1.0f;
	unk124.z = 1.0f;
}

void TMapObjBase::load(JSUMemoryInputStream& stream)
{
	TActor::load(stream);

	unkF4 = stream.readString();
	loadBeforeInit(stream);
	initMapObj();
	unkF8 &= ~0x40000;
	makeObjAppeared();
	if (unkF8 & 0x80) {
		f32 value;
		stream.read(&value, 4);
		mDamageHeight = value;
		calcEntryRadius();
		mAttackHeight = value;
		calcEntryRadius();
		unk64 &= ~4;
		unk64 &= ~2;
	}
}

void TMapObjBase::loadAfter()
{
	TLiveActor::loadAfter();
	unkC8 = gpMap->checkGround(mPosition, &unkC4);
}

void TMapObjBase::initAndRegister(const char* param_1)
{
	unkF4 = param_1;
	initMapObj();
	// TODO: what is unk130?
}

u32 TMapObjBase::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == 5 && (unkF8 & 0x40)) {
		unk6C = (TTakeActor*)param_1;
		return 1;
	} else if (param_2 == 0xF) {
		return touchWater(param_1);
	} else {
		return false;
	}
}

void TMapObjBase::calcRootMatrix()
{
	J3DModel* model                  = getModel();
	const JGeometry::TVec3<f32>& pos = getPosition();
	const JGeometry::TVec3<f32>& rot = getRotation();
	MsMtxSetXYZRPH(model->unk20, pos.x, pos.y - unk108, pos.z,
	               rot.x * (65536.0f / 360.0f), rot.y * (65536.0f / 360.0f),
	               rot.z * (65536.0f / 360.0f));
	model->unk14 = mScaling;
}

Mtx* TMapObjBase::getRootJointMtx() const
{
	if (!(unkF8 & 8) && unk108 != 0.0f)
		return (Mtx*)unk74->getUnk4()->getAnmMtx(0);
	return nullptr;
}

u32 TMapObjBase::getShadowType()
{
	if (isActorType(0x40000034) || isActorType(0x40000035)
	    || isActorType(0x40000036) || isActorType(0x40000037)
	    || isActorType(0x40000039)) {
		return 2;
	} else if (unkF8 & 0x2000) {
		return 1;
	} else {
		return 0;
	}
}

void TMapObjBase::perform(u32, JDrama::TGraphics*) { }

void TMapObjBase::setGroundCollision() { }

void TMapObjBase::control()
{
	for (int i = 0; i < mColCount; ++i)
		touchActor(mCollisions[i]);
}

void TMapObjBase::ensureTakeSituation() { }

void TMapObjBase::touchActor(THitActor*) { }

void TMapObjBase::touchPlayer(THitActor*) { }

void TMapObjBase::touchEnemy(THitActor*) { }

void TMapObjBase::touchBoss(THitActor*) { }

void TMapObjBase::moveByBck() { }

void TMapObjBase::makeObjAppeared() { }

void TMapObjBase::makeObjDead() { }

void TMapObjBase::makeObjDefault() { }

void TMapObjBase::startAnim(u16) { }

void TMapObjBase::startBck(const char*) { }

void TMapObjBase::startControlAnim(u16) { }

void TMapObjBase::stopAnim() { }

void TMapObjBase::animIsFinished() const { }

void TMapObjBase::hasAnim(u16) const { }

void TMapObjBase::hasModelOrAnimData(u16) const { }

void TMapObjBase::startSound(u16) { }

void TMapObjBase::soundBas(u32, f32, f32) { }

void TMapObjBase::setUpMapCollision(u16) { }

void TMapObjBase::setUpCurrentMapCollision() { }

void TMapObjBase::removeMapCollision() { }

void TMapObjBase::setObjHitData(u16) { }

void TMapObjBase::sleep() { }

void TMapObjBase::awake() { }

u32 TMapObjBase::getSDLModelFlag() const { }

void TMapObjBase::changeObjSRT(const JGeometry::TVec3<f32>&,
                               const JGeometry::TVec3<f32>&,
                               const JGeometry::TVec3<f32>&)
{
}

void TMapObjBase::changeObjMtx(MtxPtr) { }
