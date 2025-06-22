#include <MoveBG/MapObjRailBlock.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapData.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <Player/MarioAccess.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Conductor.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

TRailMapObj::TRailMapObj(const char* name)
    : TMapObjBase(name)
    , unk138(nullptr)
    , unk13C(0)
    , unk140(0)
    , unk144(2.0f)
    , unk148(0)
    , unk14A(180)
    , unk14C(1)
{
}

#pragma dont_inline on
// TODO: weird stack frame issues here D:
void TRailMapObj::initGraphTracer(TGraphWeb* graph)
{
	unk138 = new TGraphTracer;

	unk138->unk0 = graph;
	if (graph != nullptr && !graph->isDummy()) {
		unk138->setTo(graph->findNearestNodeIndex(mPosition, 0xffffffff));
		if (graph->unk14 != nullptr) {
			mPosition = unk138->getCurrentPos();
			unk138->moveToShortestNext();
		}

		if (graph->getGraphNode(unk138->getCurrentIndex()).getRailNode()->mFlags
		    & 0x80)
			onRailFlag(2);

		resetStep(unk144);
	}
}
#pragma dont_inline off

void TRailMapObj::resetStep(float param_1)
{
	JGeometry::TVec3<f32> point = unk138->getCurrentPos();
	unk13C                      = VECDistance(&point, &mPosition) / param_1;
}

BOOL TRailMapObj::moveToNextNode(float param_1)
{
	if (!unk138->unk0 || unk138->unk0->isDummy())
		return false;

	if (unk138->unk0->unk14 ? TRUE : FALSE) {

		bool result = unk138->traceSpline(unk138->calcSplineSpeed(param_1));
		JGeometry::TVec3<f32> local_1C;
		JGeometry::TVec3<f32> local_28;
		unk138->unk0->unk14->getPosAndRot(unk138->unk14, &local_1C, &local_28);
		local_1C.sub(mPosition);
		unk94.add(local_1C);

		mRotation = local_28;
		if (result)
			readRailFlag();
		if (unk13C > 0)
			--unk13C;
		return result;
	}

	JGeometry::TVec3<f32> local_40
	    = unk138->unk0->indexToPoint(unk138->mCurrIdx);
	JGeometry::TVec3<f32> local_34;
	local_34.sub(local_40, mPosition);
	if (local_34.squared() < param_1 * param_1 * 2.0f || unk13C == 0) {
		readRailFlag();
		unk138->unk0->getGraphNode(unk138->mCurrIdx).getPoint(mPosition);
		return true;
	} else {
		VECNormalize(&local_34, &local_34);
		local_34.scale(param_1);
		unk94.add(local_34);
		if (unk13C > 0)
			--unk13C;
		return false;
	}
}

bool TRailMapObj::checkMarioRiding()
{
	TBGCheckData* data = SMS_GetMarioGrPlane();
	if (!checkRailFlag(1)) {
		if (data && data->getUnk44() == this && SMS_IsMarioTouchGround4cm()) {
			u32 status = SMS_GetMarioStatus(SMS_GetMarioHitActor());
			if ((status & 0x200) && !(status & 0x200000)) {
				onRailFlag(1);
				offRailFlag(2);
			}
		}
	} else {
		if (!data || data->getUnk44() != this || !SMS_IsMarioTouchGround4cm()) {
			offRailFlag(1);
		}
	}

	if (checkRailFlag(1))
		return true;
	else
		return false;
}

BOOL TRailMapObj::calcRecycle()
{
	switch (unk148) {
	case 0:
		return false;
	case 1:
		if (unk14A < 180) {
			++unk14A;
			return true;
		} else {
			unk148 = 0;
			unk14A = 180;
			unk14C = 1;
			return false;
		}
	case 2:
		if (unk14A > 0) {
			--unk14A;
			if (unk14A < 90) {
				int uVar2 = gpMarDirector->unk58 / 4;
				if (uVar2 % 2 > 0)
					unk14C = 1;
				else
					unk14C = 0;
			} else {
				int uVar2 = gpMarDirector->unk58 / 4;
				if (uVar2 % 4 > 0)
					unk14C = 1;
				else
					unk14C = 0;
			}
			return true;
		}
		resetPosition();
		return false;
	}

	return false;
}

void TRailMapObj::resetPosition()
{
	mPosition = unk10C;
	mRotation = unk118;
	unk148    = 0;
	unk14A    = 180;
	unk14C    = 1;
	unk138->setToNearest(mPosition);
	readRailFlag();
}

void TRailMapObj::initMapObj()
{
	TMapObjBase::initMapObj();
	offHitFlag(0x1);
	unk74->setLightType(2);
}

void TRailMapObj::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);
	unkF4 = stream.readString();
	char buffer[256];
	stream.readString(buffer, 256);
	unk10C = mPosition;
	unk118 = mRotation;
	unk124 = mScaling;
	initGraphTracer(gpConductor->getGraphByName(buffer));
	initMapObj();
	makeObjAppeared();
}

void TRailMapObj::setGroundCollision()
{
	if (!unkEC)
		return;

	if (unk14A != 0 && (!checkMapObjFlag(2) || getColNum() != 0)) {
		TMtx34f mtx;
		mtx.set(getModel()->getAnmMtx(0));
		if (TMapCollisionBase* col = unkEC->unk8)
			col->moveMtx(mtx);
	}
}

u32 TRailMapObj::getShadowType() { return 1; }

void TRailMapObj::readRailFlag()
{
	TGraphNode& graphNode = unk138->getCurrent();

	if (graphNode.getRailNode()->mFlags & 0x100)
		onRailFlag(2);

	if ((graphNode.getRailNode()->mFlags & 0x1000)) {
		unk14A = 180;
		unk148 = 2;
	}
}

void TRailMapObj::control()
{
	TMapObjBase::control();
	checkMarioRiding();
	if (!calcRecycle() && !checkRailFlag(2)) {
		if (moveToNextNode(getUnk144())) {
			readRailFlag();
			unk138->moveToShortestNext();
			resetStep(getUnk144());
		}
	}
}

void TRailMapObj::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if ((param_1 & 0x200) && unk14C == 0)
		param_1 &= ~0x200;
	TMapObjBase::perform(param_1, param_2);
}

TNormalLift::TNormalLift(const char* name)
    : TRailMapObj(name)
    , unk150(0)
    , unk152(0)
    , unk154(0.0f)
    , unk158(0)
{
	unk144 = 1.0f;
}

void TNormalLift::resetPosition()
{
	TRailMapObj::resetPosition();
	unk158 = 1;
}

void TNormalLift::load(JSUMemoryInputStream& stream)
{
	TRailMapObj::load(stream);

	stream.read(&unk154, 4);
	if (unk154 > 0.0f && unkEC) {
		TMapCollisionBase* col = unkEC->getUnk8();
		col->setAllBGType(7);
		col->setAllActor(this);
		col->setAllData(unk154);
	}
}

void TNormalLift::readRailFlag()
{
	TRailMapObj::readRailFlag();

	TGraphWeb* graph = unk138->unk0;

	if (!unk138->unk0)
		return;

	if (!graph->isDummy())
		return;

	TRailNode* railNode = graph->getCurrentNode().getRailNode();
	if (railNode->mFlags & 0x800) {
		unk150 = railNode->mPitch;
	}
	if (railNode->mFlags & 0x1000) {
		u16 roll = railNode->mRoll;
		if (roll == 0xffff)
			roll = 0;
		unk152 = roll;
	}
}

void TNormalLift::initMapObj() { TRailMapObj::initMapObj(); }

void TNormalLift::control()
{
	TMapObjBase::control();
	checkMarioRiding();
	if (unk158 != 0 && unk152 != 0) {
		--unk152;
		if (unk152 == 0)
			unk158 = 0;
	} else {
		if (unk150 != 0) {
			--unk150;
		} else {
			if (!checkRailFlag(2) && !calcRecycle()) {
				if (moveToNextNode(getUnk144())) {
					readRailFlag();
					unk138->moveToShortestNext();

					u32 yaw = unk138->getCurrent().getRailNode()->mYaw;
					if (yaw != 0xffff)
						unk144 = yaw * 0.01f;

					resetStep(getUnk144());
				}
			}
		}
	}
}

void TNormalLift::setGroundCollision()
{
	if (unk158 == 0 || unk152 == 0)
		TRailMapObj::setGroundCollision();
}

void TNormalLift::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (unk158 && unk152 && (param_1 & 0x200))
		param_1 &= ~0x200;

	TRailMapObj::perform(param_1, param_2);
}

TRailBlock::TRailBlock(const char* name)
    : TRailMapObj(name)
    , unk150(0.0f)
    , unk154(0.0f)
    , unk158(0.0f)
    , unk15C(0.0f, 0.0f, 0.0f)
    , unk168(0.0f, 0.0f, 0.0f)
{
	MTXIdentity(unk174);
}

void TRailBlock::initMapObj()
{
	TRailMapObj::initMapObj();
	onLiveFlag(0x400);
	unk15C = mRotation;
	unk168 = mRotation;
}

Mtx* TRailBlock::getRootJointMtx() const
{
	return (Mtx*)getModel()->getBaseTRMtx();
}

void TRailBlock::calcRootMatrix()
{
	J3DModel* model = getModel();
	MtxPtr mtx      = model->getBaseTRMtx();
	MsMtxSetRotRPH(mtx, unk168.x, unk168.y, unk168.z);
	MTXConcat(mtx, unk174, mtx);
	mtx[0][3]    = mPosition.x;
	mtx[1][3]    = mPosition.y;
	mtx[2][3]    = mPosition.z;
	model->unk14 = mScaling;
}

void TRailBlock::control() { }

TRollBlock::TRollBlock(const char* name)
    : TMapObjBase(name)
    , unk138(0.0f)
    , unk13C(0.0f)
{
}

void TRollBlock::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);
	unkF4 = stream.readString();
	int local_18;
	stream.read(&local_18, 4);
	unk13C = local_18 * 0.01f;
	initMapObj();
	makeObjAppeared();
}

void TRollBlock::setGroundCollision()
{
	if (unkEC == nullptr)
		return;

	MtxPtr mtx = getModel()->getAnmMtx(0);
	if (TMapCollisionBase* col = unkEC->getUnk8())
		col->moveMtx(mtx);
}

Mtx* TRollBlock::getRootJointMtx() const
{
	return (Mtx*)getModel()->getAnmMtx(0);
}

void TRollBlock::calcRootMatrix() { }

void TRollBlock::control()
{
	TMapObjBase::control();
	unk138 += unk13C;
	f32 angle = unk138;
	while (angle >= 360.0f)
		angle -= 360.0f;
	while (angle < 0.0f)
		angle += 360.0f;
	unk138 = angle;
}

TWoodBlock::TWoodBlock(const char* name)
    : TNormalLift(name)
{
	unk164.r = 0;
	unk164.g = 0;
	unk164.b = 0;
	unk164.a = 0;

	unk15C = unk164;
}

BOOL TWoodBlock::calcRecycle()
{
	switch (unk148) {
	case 0:
		unk14C = 1;
		unk164 = unk15C;
		return false;
	case 1:
		if (unk14A < 180) {
			++unk14A;
			unk164 = unk15C;
			return true;
		} else {
			unk148 = 0;
			unk14A = 180;
			unk14C = 1;
			return false;
		}
	case 2:
		if (unk14A > 0) {
			--unk14A;
			f32 mult = unk14A / 180.0f;
			unk164.r = (int)(unk15C.r * mult) & 0xff;
			unk164.g = (int)(unk15C.g * mult) & 0xff;
			unk164.b = (int)(unk15C.b * mult) & 0xff;
			unk164.a = (int)(unk15C.a * mult) & 0xff;
			unk14C   = 1;
			return true;
		}
		if (JPABaseEmitter* emitter
		    = gpMarioParticleManager->emit(0x6D, &mPosition, 0, nullptr)) {
			f32 scale = (mScaling.x + mScaling.y + mScaling.z) / 3.0f;
			emitter->unk154.set(scale, scale, scale);
			emitter->unk174.set(1.0f, 1.0f, 0.0f);
		}
		resetPosition();
		unk164 = unk15C;
		return 0;
	}

	return 0;
}

void TWoodBlock::load(JSUMemoryInputStream& stream)
{
	TNormalLift::load(stream);

	int local_20;
	int local_24;
	int local_28;
	int local_2C;
	stream.read(&local_20, 4);
	stream.read(&local_24, 4);
	stream.read(&local_28, 4);
	stream.read(&local_2C, 4);
	unk164.r = local_20 & 0xff;
	unk164.g = local_24 & 0xff;
	unk164.b = local_28 & 0xff;
	unk164.a = 0xFF;
	unk15C   = unk164;
	SMS_InitPacket_OneTevColor(getModel(), 0, GX_TEVREG0, &unk164);
}
