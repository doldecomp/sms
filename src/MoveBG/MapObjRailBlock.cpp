#include <MoveBG/MapObjRailBlock.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapData.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <Player/MarioAccess.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/LightUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Conductor.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JMath.hpp>
#include <math.h>

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

// TODO: the pragma is the only thing keeping this a `bl` where
// TRailMapObj::load reaches it at depth 1, and it is pre-existing. Removing it
// takes load 100% -> 0%. The depth-1 allowance is fourteen statements, so
// retail's body had fifteen or more; this reconstruction counts about seven
// (the `new`, the graph store, setTo, the position copy, moveToShortestNext,
// onRailFlag and resetStep), and it is already 99.9% instruction-exact, so the
// missing statements have to be spellings that cost no code -- naming the
// findNearestNodeIndex result, the node and its rail node, and so on. Not
// resolved here.
#pragma dont_inline on
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
	TGraphWeb* web = unk138->getGraph();
	if (!web || web->isDummy())
		return false;

	if (unk138->unk0->unk14 ? TRUE : FALSE) {

		BOOL result = unk138->traceSpline(unk138->calcSplineSpeed(param_1));
		JGeometry::TVec3<f32> pos;
		JGeometry::TVec3<f32> rot;
		unk138->unk0->unk14->getPosAndRot(unk138->unk14, &pos, &rot);
		pos.sub(getPosition());
		mLinearVelocity.add(pos);

		mRotation = rot;
		if (result)
			readRailFlag();
		if (unk13C > 0)
			--unk13C;
		return result;
	}

	JGeometry::TVec3<f32> toNext = unk138->getCurrentPos();
	toNext.sub(getPosition());
	if (toNext.squared() < param_1 * param_1 * 2.0f || unk13C == 0) {
		readRailFlag();
		web->getGraphNode(unk138->getCurGraphIndex()).getPoint(getPosition());
		return true;
	} else {
		VECNormalize(&toNext, &toNext);
		toNext.scale(param_1);
		mLinearVelocity.add(toNext);
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
	TGraphTracer* tracer = unk138;
	tracer->setToNearest(mPosition);
	readRailFlag();
}

void TRailMapObj::initMapObj()
{
	TMapObjBase::initMapObj();
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mMActor->setLightType(LIGHT_TYPE_MAPOBJECT);
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
	// The graph is named in its own statement: that is the tenth statement
	// of this function, which is what keeps it a `bl` where TWoodBlock::load
	// reaches it at depth 2 (the depth-2 allowance is nine).
	TGraphWeb* graph = gpConductor->getGraphByName(buffer);
	initGraphTracer(graph);
	initMapObj();
	makeObjAppeared();
}

void TRailMapObj::setGroundCollision()
{
	if (!mMapCollisionManager)
		return;

	if (unk14A != 0
	    && (!checkMapObjFlag(MAP_OBJ_FLAG_UNK2) || getColNum() != 0)) {
		TPosition3f mtx;
		mtx.set(getModel()->getAnmMtx(0));
		if (TMapCollisionBase* col = mMapCollisionManager->unk8)
			col->moveMtx(mtx);
	}
}

u32 TRailMapObj::getShadowType() { return SHADOW_TYPE_SQUARE; }

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

// Binding level worth +8 of low region, landing TNormalLift::readRailFlag's
// frame at 0x80 (batch 121).
static inline int MapObjRailBlockGetCurGraphIndex(const TGraphTracer* p)
{
	int curGraphIndex = p->getCurGraphIndex();
	return curGraphIndex;
}

void TNormalLift::readRailFlag()
{
	TRailMapObj::readRailFlag();

	TGraphWeb* graph = getTracer()->getGraph();

	if (!graph)
		return;

	if (graph->isDummy())
		return;

	TGraphNode& node
	    = graph->getGraphNode(MapObjRailBlockGetCurGraphIndex(unk138));

	if (node.getRailNode()->mFlags & 0x800)
		unk150 = node.getRailNode()->mPitch;

	if (node.getRailNode()->mFlags & 0x1000) {
		u16 roll = node.getRailNode()->mRoll;
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
	setDamageParams(300.0f, 50.0f);
	checkMarioRiding();

	if (!calcRecycle() && !checkRailFlag(2)) {
		if (moveToNextNode(getUnk144())) {
			TGraphTracer* tracer = unk138;
			TGraphWeb* web       = tracer->getGraph();

			if (web->getGraphNode(tracer->getCurGraphIndex())
			        .getRailNode()
			        ->mFlags
			    & 0x1000) {
				unk14A = 180;
				unk148 = 2;
			}

			unk138->moveToShortestNext();

			u32 speed = unk138->getCurrent().getRailNode()->mSpeed;
			if (speed != 0xffff)
				unk144 = 0.01f * speed;

			resetStep(getUnk144());

			if (checkRailFlag(2)) {
				MTXIdentity(unk174);
				unk168.zero();
			} else {
				unk168 = unk15C;

				Mtx mtx;
				MsMtxSetRotRPH(mtx, unk168.x, unk168.y, unk168.z);
				MTXConcat(mtx, unk174, unk174);
				unk168.zero();

				JGeometry::TVec3<f32> xDir;
				xDir.x = unk174[0][0];
				xDir.y = unk174[1][0];
				xDir.z = unk174[2][0];

				JGeometry::TVec3<f32> yDir;
				yDir.x = unk174[0][1];
				yDir.y = unk174[1][1];
				yDir.z = unk174[2][1];

				JGeometry::TVec3<f32> zDir;
				zDir.x = unk174[0][2];
				zDir.y = unk174[1][2];
				zDir.z = unk174[2][2];

				VECNormalize(&xDir, &xDir);
				VECNormalize(&yDir, &yDir);
				VECNormalize(&zDir, &zDir);

				xDir.x -= 1.0f;
				yDir.y -= 1.0f;
				zDir.z -= 1.0f;

				if (fabsf(xDir.x) < 0.02f && fabsf(xDir.y) < 0.02f
				    && fabsf(xDir.z) < 0.02f && fabsf(yDir.x) < 0.02f
				    && fabsf(yDir.y) < 0.02f && fabsf(yDir.z) < 0.02f
				    && fabsf(zDir.x) < 0.02f && fabsf(zDir.y) < 0.02f
				    && fabsf(zDir.z) < 0.02f)
					MTXIdentity(unk174);
			}

			JGeometry::TVec3<f32> point;
			TGraphNode& node = web->getGraphNode(unk138->getCurGraphIndex());
			node.getPoint(point);

			f32 frames = VECDistance(&mPosition, &point) / unk144;

			TRailNode* rail = node.getRailNode();
			unk15C.set(rail->mPitch, rail->mYaw, rail->mRoll);

			unk150 = MsAngleDiff(unk15C.x, unk168.x) / frames;
			unk154 = MsAngleDiff(unk15C.y, unk168.y) / frames;
			unk158 = MsAngleDiff(unk15C.z, unk168.z) / frames;
		} else {
			mRotation.x += unk150;
			mRotation.y += unk154;
			mRotation.z += unk158;

			unk168.x += unk150;
			unk168.y += unk154;
			unk168.z += unk158;

			f32 pitch = mRotation.x;
			while (pitch >= 360.0f)
				pitch -= 360.0f;
			while (pitch < 0.0f)
				pitch += 360.0f;
			mRotation.x = pitch;

			f32 yaw = mRotation.y;
			while (yaw >= 360.0f)
				yaw -= 360.0f;
			while (yaw < 0.0f)
				yaw += 360.0f;
			mRotation.y = yaw;

			f32 roll = mRotation.z;
			while (roll >= 360.0f)
				roll -= 360.0f;
			while (roll < 0.0f)
				roll += 360.0f;
			mRotation.z = roll;
		}
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
	if (TMapCollisionBase* col = mMapCollisionManager->unk8)
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
	// TODO: 8 bytes of frame short (0x88 vs 0x90) and the local matrix sits
	// at 0x20 where retail has it at 0x2c, so the whole residue is the low
	// region. Reading mPosition through getPosition() moves it: three reads
	// give the matrix its 0x2c but overshoot the frame to 0x98, two give the
	// exact frame and 0x28, and one bound reference gives the frame and
	// leaves the matrix at 0x20. None is exact and a mixture of raw and
	// accessor reads of one member is not plausible source, so this keeps
	// the plain reads.
	MsMtxSetXYZRPH(mtx, mPosition.x, mPosition.y - mYOffset, mPosition.z,
	               mInitialRotation.x, mInitialRotation.y, mInitialRotation.z);
	model->setBaseScale(getScaling());

	f32 sinRoll = JMASin(unk138);
	f32 cosRoll = JMACos(unk138);

	Mtx roll;
	roll[0][0] = cosRoll;
	roll[0][1] = -sinRoll;
	roll[0][2] = 0.0f;
	roll[0][3] = 0.0f;
	roll[1][0] = sinRoll;
	roll[1][1] = cosRoll;
	roll[1][2] = 0.0f;
	roll[1][3] = 0.0f;
	roll[2][0] = 0.0f;
	roll[2][1] = 0.0f;
	roll[2][2] = 1.0f;
	roll[2][3] = 0.0f;

	MTXConcat(mtx, roll, mtx);
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
		if (JPABaseEmitter* emitter = gpMarioParticleManager->emit(
		        PARTICLE_MS_EX_CUBE_DISA, &mPosition, 0, nullptr)) {
			f32 scale
			    = (getScaling().x + getScaling().y + getScaling().z) / 3.0f;
			emitter->setGlobalDynamicsScale(
			    JGeometry::TVec3<f32>(scale, scale, scale));
			emitter->setGlobalParticleScale(
			    JGeometry::TVec3<f32>(1.0f, 1.0f, 0.0f));
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
