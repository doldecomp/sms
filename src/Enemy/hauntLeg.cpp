#include <Enemy/Conductor.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/HauntLeg.hpp>
#include <Enemy/Spider.hpp>
#include <Enemy/Walker.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JGeometry/JGPosition3.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <Map/MapData.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>

#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static THauntLeg* gpCurHauntLeg;

static int HauntLegCallback(J3DNode* param_1, int param_2)
{
	if (param_2 == 0) {
		if (gpCurHauntLeg == nullptr || !gpCurHauntLeg->isUseCallBack())
			return true;
		MtxPtr mtx = gpCurHauntLeg->getMActor()->getModel()->getAnmMtx(
		    ((J3DJoint*)param_1)->getJntNo());
		Mtx rotation;
		MsMtxSetRotZ(rotation, gpCurHauntLeg->unk1AC);
		MTXConcat(mtx, rotation, mtx);
		MTXConcat(J3DSys::mCurrentMtx, rotation, J3DSys::mCurrentMtx);
	}
	return true;
}

THauntLegManager::THauntLegManager(const char* param_1)
    : TSmallEnemyManager(param_1)
{
	gpCurHauntLeg = nullptr;
}

void THauntLegManager::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemyManager::load(param_1);
	unk38 = new TWalkerEnemyParams("/enemy/hauntLeg.prm");
}

TSpineEnemy* THauntLegManager::createEnemyInstance() { return new THauntLeg; }

void THauntLegManager::initSetEnemies()
{
	static const GXColorS10 tevColorData1[] = {
		{ 0, 0, 120, 255 },   { 120, 0, 0, 255 },     { 0, 120, 0, 255 },
		{ 120, 120, 0, 255 }, { 120, 0, 120, 255 },   { 100, 200, 0, 255 },
		{ 0, 100, 200, 255 }, { 200, 100, 150, 255 },
	};
	static const GXColorS10 tevColorData2[] = {
		{ 0, 0, 250, 255 },   { 250, 0, 0, 255 },     { 0, 250, 0, 255 },
		{ 250, 250, 0, 255 }, { 250, 0, 250, 255 },   { 150, 250, 0, 255 },
		{ 0, 150, 250, 255 }, { 250, 150, 200, 255 },
	};
	int colorIndex = 0;
	for (int i = 0; i < mObjNum; ++i) {
		TGraphWeb* graph = gpConductor->getGraphByName("main");
		THauntLeg* enemy = (THauntLeg*)getObj(i);
		JGeometry::TVec3<f32> position;
		graph->getGraphNode(TMsRange<s32>(0, graph->getNodeNum()).rand())
		    .getPoint(&position);
		enemy->mPosition = position;
		enemy->mPosition.y += 5.0f;
		enemy->onLiveFlag(LIVE_FLAG_AIRBORNE);
		enemy->reset();
		for (u16 j = 0;
		     j
		     < enemy->getMActor()->getModel()->getModelData()->getMaterialNum();
		     ++j) {
			SMS_InitPacket_TwoTevColor(getObj(i)->getMActor()->getModel(), j,
			                           GX_TEVREG0, &tevColorData1[colorIndex],
			                           GX_TEVREG1, &tevColorData2[colorIndex]);
		}
		if (++colorIndex >= 8)
			colorIndex = 0;
	}
}

void THauntLegManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "hauntleg.bmd", 0x10220000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

BOOL THauntedObject::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 <= HIT_MESSAGE_HIP_DROP) {
		unk68->kill();
		return true;
	}
	if (param_2 == HIT_MESSAGE_SPRAYED_BY_WATER)
		return true;
	return false;
}

void THauntedObject::checkHit()
{
	for (int i = getColNum(); i > 0; --i) { }
}

void THauntedObject::kill() { onHitFilter(HIT_FILTER_NO_COLLISION); }

THauntLeg::THauntLeg(const char* param_1)
    : TWalkerEnemy(param_1)
    , unk194(nullptr)
    , unk198(0)
    , unk199(1)
    , unk19C(nullptr)
{
}

void THauntLeg::init(TLiveManager* param_1)
{
	TWalkerEnemy::init(param_1);
	mActorType = ACTOR_TYPE_HAUNT_LEG;
	unk150     = 0x3a;
	onHitFilter(HIT_CATEGORY_ITEM | HIT_CATEGORY_MAP_OBJECT);
	getWalker()->setMode(1);
	unk130 = 2;
	getMActor()->setJointCallback(1, HauntLegCallback);
	unk194 = new THauntedObject;
	static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"))
	    ->getChildren()
	    .push_back(unk194);
	f32 radius = 30.0f * mBodyScale;
	unk194->initHitActor(ACTOR_TYPE_HAUNT_LEG, 2, HIT_CATEGORY_PLAYER, radius,
	                     radius, radius, radius);
	unk194->unk68 = this;
}

void THauntLeg::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("hauntleg.bmd", 3);
}

void THauntLeg::reset()
{
	unk19C = nullptr;
	unk198 = 0;
	unk199 = 1;
	TWalkerEnemy::reset();
}

void THauntLeg::calcRootMatrix()
{
	gpCurHauntLeg = this;
	if (isEaten())
		return;
	getModel()->setBaseScale(mScaling);
	MtxPtr matrix = getModel()->getBaseTRMtx();
	if (getWalker()->getSpider()->getWallAttachRate() > 0.0f
	    && unk138 != nullptr) {
		JGeometry::TVec3<f32> zAxis(0.0f, 1.0f, 0.0f);
		JGeometry::TVec3<f32> normal = unk138->getNormal();
		JGeometry::TVec3<f32> xAxis;
		xAxis.cross(normal, zAxis);
		MsVECNormalize(&xAxis, &xAxis);
		zAxis.cross(xAxis, normal);
		MsVECNormalize(&zAxis, &zAxis);
		matrix[0][0] = xAxis.x;
		matrix[1][0] = xAxis.y;
		matrix[2][0] = xAxis.z;
		matrix[0][1] = normal.x;
		matrix[1][1] = normal.y;
		matrix[2][1] = normal.z;
		matrix[0][2] = zAxis.x;
		matrix[1][2] = zAxis.y;
		matrix[2][2] = zAxis.z;
		matrix[0][3] = 0.0f;
		matrix[1][3] = 0.0f;
		matrix[2][3] = 0.0f;
		Mtx rotation;
		MsMtxSetRotX(
		    rotation,
		    90.0f * (1.0f - getWalker()->getSpider()->getWallAttachRate()));
		MTXConcat(matrix, rotation, matrix);
	} else {
		JGeometry::TVec3<f32> zAxis(MsSin(mRotation.y), 0.0f,
		                            MsCos(mRotation.y));
		JGeometry::TVec3<f32> normal = mGroundPlane->getNormal();
		JGeometry::TVec3<f32> xAxis;
		xAxis.cross(normal, zAxis);
		MsVECNormalize(&xAxis, &xAxis);
		zAxis.cross(xAxis, normal);
		MsVECNormalize(&zAxis, &zAxis);
		matrix[0][0] = xAxis.x;
		matrix[1][0] = xAxis.y;
		matrix[2][0] = xAxis.z;
		matrix[0][1] = normal.x;
		matrix[1][1] = normal.y;
		matrix[2][1] = normal.z;
		matrix[0][2] = zAxis.x;
		matrix[1][2] = zAxis.y;
		matrix[2][2] = zAxis.z;
	}
	matrix[0][3] = mPosition.x;
	matrix[1][3] = mPosition.y;
	matrix[2][3] = mPosition.z;
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		unk194->mPosition = mPosition;
	} else {
		MtxPtr joint        = getModel()->getAnmMtx(2);
		unk194->mPosition.x = joint[0][3];
		unk194->mPosition.y = joint[1][3];
		unk194->mPosition.z = joint[2][3];
	}
	unk194->checkHit();
}

void THauntLeg::setGenerateAnm() { setBckAnm(0); }

void THauntLeg::setWaitAnm() { setBckAnm(2); }

void THauntLeg::setWalkAnm() { setBckAnm(1); }

void THauntLeg::setRunAnm() { setBckAnm(1); }

void THauntLeg::setDeadAnm()
{
	if (unk19C != nullptr) {
		unk19C->receiveMessage(this, HIT_MESSAGE_PUT);
		mHolder     = nullptr;
		mHeldObject = nullptr;
	}
	unk194->kill();
}

void THauntLeg::attackToMario()
{
	updateSquareToMario();
	if (mDistToMarioSquared < 10000.0f)
		sendAttackMsgToMario();
}

bool THauntLeg::isCollidMove(THitActor* param_1)
{
	if (mSpine->getCurrentNerve() != &TNerveHauntLegHaunt::theNerve() && !unk198
	    && !checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		u32 type = param_1->getActorType() & HIT_CATEGORY_MASK;
		if (type == HIT_CATEGORY_ITEM || type == HIT_CATEGORY_MAP_OBJECT) {
			TTakeActor* actor = (TTakeActor*)param_1;
			if (actor->getHolder() == nullptr || actor != unk19C) {
				unk19C = actor;
				mSpine->setNext(&TNerveHauntLegHaunt::theNerve());
			}
			return false;
		}
	}
	return false;
}

static const char* hauntleg_bastable[] = {
	nullptr,
	nullptr,
	nullptr,
};

const char** THauntLeg::getBasNameTable() const { return hauntleg_bastable; }

MtxPtr THauntLeg::getTakingMtx()
{
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		TPosition3f matrix;
		matrix.translation(mPosition.x, mPosition.y, mPosition.z);
		getMActor()->getModel()->setBaseTRMtx(matrix);
		return getMActor()->getModel()->getBaseTRMtx();
	}
	return getMActor()->getModel()->getAnmMtx(2);
}

bool THauntLeg::isUseCallBack()
{
	if (mSpine->getCurrentNerve() == &TNerveHauntLegHaunt::theNerve())
		return true;
	return false;
}

DEFINE_NERVE(TNerveHauntLegHaunt, TLiveActor)
{
	THauntLeg* enemy = (THauntLeg*)spine->getBody();
	if (spine->getTime() == 0) {
		enemy->unk1A0 = enemy->calcVelocityToJumpToY(
		    enemy->unk19C->mPosition, 10.0f, enemy->getGravityY());
		enemy->mVelocity = enemy->unk1A0;
		enemy->mPosition.y += 10.0f;
		enemy->onLiveFlag(LIVE_FLAG_AIRBORNE);
		enemy->unk199 = 1;
	} else if (!enemy->isAirborne()) {
		if (enemy->unk199) {
			enemy->mVelocity = enemy->unk1A0;
			enemy->mPosition.y += 10.0f;
			enemy->unk199 = 0;
			JGeometry::TVec3<f32> distance
			    = enemy->mPosition - enemy->unk19C->mPosition;
			if (distance.length() < 200.0f
			    && enemy->unk19C->getHolder() == nullptr
			    && enemy->unk19C->receiveMessage(enemy, HIT_MESSAGE_TAKE)) {
				enemy->mHeldObject = enemy->unk19C;
				enemy->unk198      = 1;
			}
		} else {
			enemy->unk1AC = 0.0f;
			spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
			return true;
		}
	}
	if (enemy->isAirborne()) {
		if (enemy->unk199)
			enemy->unk1AC = MsClamp(2.0f + enemy->unk1AC, 0.0f, 180.0f);
		else
			enemy->unk1AC = MsClamp(2.0f + enemy->unk1AC, 0.0f, 360.0f);
	}
	return false;
}
