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

		if (graph->getGraphNode(unk138->getCurGraphIndex())
		        .getRailNode()
		        ->mFlags
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
		mLinearVelocity.add(local_1C);

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
		mLinearVelocity.add(local_34);
		if (unk13C > 0)
			--unk13C;
		return false;
	}
}

bool TRailMapObj::checkMarioRiding()
{
	const TBGCheckData* data = SMS_GetMarioGrPlane();
	if (!checkRailFlag(1)) {
		if (data && data->getActor() == this && SMS_IsMarioTouchGround4cm()) {
			u32 status = SMS_GetMarioStatus(SMS_GetMarioHitActor());
			if ((status & MARIO_STATUS_FLAG_UNK200)
			    && !(status & MARIO_STATUS_FLAG_UNK200000)) {
				onRailFlag(1);
				offRailFlag(2);
			}
		}
	} else {
		if (!data || data->getActor() != this || !SMS_IsMarioTouchGround4cm()) {
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
	mPosition = mInitialPosition;
	mRotation = mInitialRotation;
	unk148    = 0;
	unk14A    = 180;
	unk14C    = 1;
	unk138->setToNearest(mPosition);
	readRailFlag();
}

void TRailMapObj::initMapObj()
{
	TMapObjBase::initMapObj();
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mMActor->setLightType(2);
}

void TRailMapObj::load(JSUMemoryInputStream& stream)
{
	JDrama::TActor::load(stream);
	unkF4 = stream.readString();
	char buffer[256];
	stream.readString(buffer, 256);
	mInitialPosition = mPosition;
	mInitialRotation = mRotation;
	mInitialScaling  = mScaling;
	initGraphTracer(gpConductor->getGraphByName(buffer));
	initMapObj();
	makeObjAppeared();
}

void TRailMapObj::setGroundCollision()
{
	if (!mMapCollisionManager)
		return;

	if (unk14A != 0
	    && (!checkMapObjFlag(MAP_OBJ_FLAG_UNK2) || getColNum() != 0)) {
		TMtx34f mtx;
		mtx.set(getModel()->getAnmMtx(0));
		if (TMapCollisionBase* col = mMapCollisionManager->unk8)
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

void TRailMapObj::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if ((cue & CUE_ENTRY) && unk14C == 0)
		cue &= ~CUE_ENTRY;
	TMapObjBase::perform(cue, graphics);
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

	stream >> unk154;
	if (unk154 > 0.0f && mMapCollisionManager) {
		TMapCollisionBase* col = mMapCollisionManager->getUnk8();
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

void TNormalLift::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (unk158 && unk152 && (cue & CUE_ENTRY))
		cue &= ~CUE_ENTRY;

	TRailMapObj::perform(cue, graphics);
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
	onLiveFlag(LIVE_FLAG_UNK400);
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
	mtx[0][3] = mPosition.x;
	mtx[1][3] = mPosition.y;
	mtx[2][3] = mPosition.z;
	model->setBaseScale(mScaling);
}

void TRailBlock::control()
{
	TMapObjBase::control();
	mDamageRadius = 300.0f;
	mDamageHeight = 50.0f;
	calcEntryRadius();

	checkMarioRiding();
	if (calcRecycle() || checkRailFlag(2))
		return;

	if (moveToNextNode(unk144)) {
		TGraphNode& node = unk138->getCurrent();
		if (node.getRailNode()->mFlags & 0x1000) {
			unk14A = 180;
			unk148 = 2;
		}

		unk138->moveToShortestNext();

		TRailNode* nextNode = unk138->getCurrent().getRailNode();
		u16 speed           = nextNode->mSpeed;
		if (speed != 0xffff)
			unk144 = speed * 0.01f;

		JGeometry::TVec3<f32> nextPoint
		    = unk138->unk0->indexToPoint(unk138->mCurrIdx);
		f32 step = VECDistance(&nextPoint, &mPosition) / unk144;
		unk13C   = step;

		if (checkRailFlag(2)) {
			MTXIdentity(unk174);
			unk168.x = 0.0f;
			unk168.y = 0.0f;
			unk168.z = 0.0f;
			return;
		}

		unk168 = unk15C;

		Mtx rotMtx;
		MsMtxSetRotRPH(rotMtx, unk168.x, unk168.y, unk168.z);
		MTXConcat(rotMtx, unk174, unk174);

		unk168.x = 0.0f;
		unk168.y = 0.0f;
		unk168.z = 0.0f;

		JGeometry::TVec3<f32> xAxis(unk174[0][0], unk174[1][0], unk174[2][0]);
		JGeometry::TVec3<f32> yAxis(unk174[0][1], unk174[1][1], unk174[2][1]);
		JGeometry::TVec3<f32> zAxis(unk174[0][2], unk174[1][2], unk174[2][2]);
		PSVECNormalize(&xAxis, &xAxis);
		PSVECNormalize(&yAxis, &yAxis);
		PSVECNormalize(&zAxis, &zAxis);

		xAxis.x -= 1.0f;
		yAxis.y -= 1.0f;
		zAxis.z -= 1.0f;
		if (fabsf(xAxis.x) < 0.02f && fabsf(xAxis.y) < 0.02f
		    && fabsf(xAxis.z) < 0.02f && fabsf(yAxis.x) < 0.02f
		    && fabsf(yAxis.y) < 0.02f && fabsf(yAxis.z) < 0.02f
		    && fabsf(zAxis.x) < 0.02f && fabsf(zAxis.y) < 0.02f
		    && fabsf(zAxis.z) < 0.02f)
			MTXIdentity(unk174);

		JGeometry::TVec3<f32> point;
		TGraphNode& rotateNode = unk138->getCurrent();
		rotateNode.getPoint(&point);
		f32 rotateStep      = VECDistance(&mPosition, &point) / unk144;
		TRailNode* railNode = rotateNode.getRailNode();
		unk15C.x            = railNode->mPitch;
		unk15C.y            = railNode->mYaw;
		unk15C.z            = railNode->mRoll;
		unk150              = MsAngleDiff(unk15C.x, unk168.x) / rotateStep;
		unk154              = MsAngleDiff(unk15C.y, unk168.y) / rotateStep;
		unk158              = MsAngleDiff(unk15C.z, unk168.z) / rotateStep;
	} else {
		mRotation.x += unk150;
		mRotation.y += unk154;
		mRotation.z += unk158;
		unk168.x += unk150;
		unk168.y += unk154;
		unk168.z += unk158;

		mRotation.x = MsWrap<f32>(mRotation.x, 0.0f, 360.0f);
		mRotation.y = MsWrap<f32>(mRotation.y, 0.0f, 360.0f);
		mRotation.z = MsWrap<f32>(mRotation.z, 0.0f, 360.0f);
	}
}

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
	s32 local_18;
	stream >> local_18;
	unk13C = local_18 * 0.01f;
	initMapObj();
	makeObjAppeared();
}

void TRollBlock::setGroundCollision()
{
	if (mMapCollisionManager == nullptr)
		return;

	MtxPtr mtx = getModel()->getAnmMtx(0);
	if (TMapCollisionBase* col = mMapCollisionManager->getUnk8())
		col->moveMtx(mtx);
}

Mtx* TRollBlock::getRootJointMtx() const
{
	return (Mtx*)getModel()->getAnmMtx(0);
}

void TRollBlock::calcRootMatrix()
{
	J3DModel* model = getModel();
	MtxPtr mtx      = model->getBaseTRMtx();
	s16 rotZ        = mRotation.z * (65536.0f / 360.0f);
	s16 rotY        = mRotation.y * (65536.0f / 360.0f);
	s16 rotX        = mRotation.x * (65536.0f / 360.0f);
	MsMtxSetXYZRPH(mtx, mPosition.x, mPosition.y - mYOffset, mPosition.z, rotX,
	               rotY, rotZ);
	model->setBaseScale(mScaling);

	f32 sinV = JMASin(unk138);
	f32 cosV = JMACos(unk138);

	Mtx rot;
	rot[0][0] = cosV;
	rot[0][1] = -sinV;
	rot[0][2] = 0.0f;
	rot[0][3] = 0.0f;
	rot[1][0] = sinV;
	rot[1][1] = cosV;
	rot[1][2] = 0.0f;
	rot[1][3] = 0.0f;
	rot[2][0] = 0.0f;
	rot[2][1] = 0.0f;
	rot[2][2] = 1.0f;
	rot[2][3] = 0.0f;
	MTXConcat(mtx, rot, mtx);
}

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

	s32 local_20, local_24, local_28, local_2C;
	stream >> local_20 >> local_24 >> local_28 >> local_2C;
	unk164.r = local_20 & 0xff;
	unk164.g = local_24 & 0xff;
	unk164.b = local_28 & 0xff;
	unk164.a = 0xFF;
	unk15C   = unk164;
	SMS_InitPacket_OneTevColor(getModel(), 0, GX_TEVREG0, &unk164);
}
