#include <Enemy/EnemyAttachment.hpp>
#include <Enemy/Conductor.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/SharedParts.hpp>
#include <System/MarDirector.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/Camera.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/PollutionManager.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

TEnemyAttachment::TEnemyAttachment(const char* name)
    : TSpineEnemy(name)
    , unk154(0)
    , unk158(0)
    , unk160(nullptr)
    , unk164(1.0f)
    , unk168(0)
{
}

void TEnemyAttachment::loadInit(TSpineEnemy* param_1, const char* param_2)
{
	unk160        = param_1;
	mMActorKeeper = new TMActorKeeper(unk160->getManager(), 1);
	mMActor       = mMActorKeeper->createMActor(param_2, 3);
}

void TEnemyAttachment::behaveToHitGround()
{
	unk168 = 1;
	offLiveFlag(0x80);
	unkAC.x = 0.0f;
	unkAC.y = -0.3f;
	unkAC.z = 0.0f;
	rebirth();
}

void TEnemyAttachment::behaveToHitWall(const TBGCheckData*) { rebirth(); }

void TEnemyAttachment::forceKill()
{
	if (!unkC4->checkSomething2() && gpMap->isInArea(mPosition.x, mPosition.z))
		return;

	kill();

	if (unk160->checkLiveFlag(0x2)) {
		unk160->kill();
		unk160->onLiveFlag(0x20000);
	}
}

void TEnemyAttachment::recoverScale()
{
	if (mScaling.x < unk164) {
		VECScale(&mScaling, &mScaling, 2.0f);
	} else {
		mScaling.x = mScaling.y = mScaling.z = unk164;
	}
}

void TEnemyAttachment::bind()
{
	JGeometry::TVec3<f32> local_1C = mPosition;
	local_1C += unk94;
	local_1C += unkAC;
	setBehavior();
	unkAC.y -= getNowGravity();
	if (unkAC.y < mVelocityMinY)
		unkAC.y = mVelocityMinY;
	if (!unk168) {
		const TBGCheckData* local_18;
		unkC8 = gpMap->checkGround(local_1C.x, local_1C.y + unkC0, local_1C.z,
		                           &local_18);
		unkC8 += 1.0f;
	}

	if (local_1C.y + unkAC.y <= unkC8)
		behaveToHitGround();
	else
		onLiveFlag(0x80);

	JGeometry::TVec3<f32> p = local_1C;
	p.y += unkC0;
	TBGWallCheckRecord local_48(p, unkBC * 2.0f, 1, 0);
	if (gpMap->isTouchedWallsAndMoveXZ(&local_48))
		behaveToHitWall(local_48.unk1C[0]);

	mPosition                      = local_1C;
	JGeometry::TVec3<f32> local_68 = local_1C;
	local_68 -= mPosition;
	unk94 = local_68;

	setBehavior();
	forceKill();

	mPosition += unk94;
}

void TEnemyAttachment::rebirth()
{
	unk150 = 0;
	unk158 = 0;
	onHitFlag(0x1);
	unkAC.y = 0.0f;
}

void TEnemyAttachment::kill()
{
	unk150 = 0;
	unk158 = 0;
	onHitFlag(0x1);
}

void TEnemyAttachment::set()
{
	mPosition = unk160->getPosition();
	mRotation = unk160->getRotation();
}

void TEnemyAttachment::moveObject()
{
	if (unk150 == 1) {
		set();
	} else {
		calcRideMomentum();
		sendMessage();
		bind();
		calcRidePos();
	}
}

void TEnemyAttachment::sendMessage()
{
	for (int i = 0; i < mColCount; ++i) {
		if (mCollisions[i]->isActorType(0x80000001)) {
			SMS_SendMessageToMario(this, 0xE);
			continue;
		}

		if (mCollisions[i] != unk160) {
			((TLiveActor*)mCollisions[i])->kill();
		}
	}
}

void TEnemyAttachment::calcRootMatrix()
{
	MsMtxSetXYZRPH(getMActor()->getModel()->getBaseTRMtx(), mPosition.x,
	               mPosition.y, mPosition.z, mRotation.x, mRotation.y,
	               mRotation.z);
	getMActor()->getModel()->setBaseScale(mScaling);
}

void TEnemyAttachment::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (unk150 == nullptr) {
		if (param_1 & 2)
			kill();
		return;
	}

	// TODO: wtf is this inline???
	bool bVar2 = true;
	if (gpMarDirector->unk124 != 3 && gpMarDirector->unk124 != 4) {
		bVar2 = false;
	}
	if (!bVar2) {
		bVar2 = true;
		if (gpMarDirector->unk124 != 1 && gpMarDirector->unk124 != 2)
			bVar2 = false;
		if (bVar2) {
			performOnlyDraw(param_1, param_2);
			return;
		}
	}

	if (param_1 & 0x1)
		moveObject();

	if (param_1 & 0x2) {
		calcRootMatrix();
		getMActor()->calcAnm();
	}

	if (param_1 & 0x4)
		getMActor()->viewCalc();

	if (param_1 & 0x200)
		getMActor()->entry();
}

void TEnemyPolluteModelManager::init(TLiveActor* param_1)
{
	unk18 = new TEnemyPolluteModel*[unk14];
}

void TEnemyPolluteModelManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 2) {
		f32 f31 = 100.0f;
		SetViewFrustumClipCheckPerspective(
		    gpCamera->getFovy(), gpCamera->getAspect(), param_2->getUnkE8(),
		    gpConductor->getCondParams().mEnemyFarClip.get());

		for (int i = 0; i < unk14; ++i) {
			if (unk18[i]->unk5D) {
				if (ViewFrustumClipCheck(param_2, &unk18[i]->unk44, f31))
					unk18[i]->unk5C = false;
				else
					unk18[i]->unk5C = true;
			}
		}
	}

	for (int i = 0; i < unk14; ++i)
		unk18[i]->perform(param_1, param_2);
}

void TEnemyPolluteModelManager::generatePolluteModel(
    JGeometry::TVec3<f32>& param_1, JGeometry::TVec3<f32>& param_2)
{
	TEnemyPolluteModel* model = unk18[unk10];

	const TBGCheckData* check;
	gpMap->checkGround(param_1, &check);
	if (!check->checkFlag2(0x10) && !check->isWaterSurface())
		model->generate(param_1, param_2);

	++unk10;
	if (unk10 >= unk14)
		unk10 = 0;
}

TEnemyPolluteModel::TEnemyPolluteModel(TLiveActor* param_1, int param_2,
                                       SDLModelData* param_3, const char* name)
    : JDrama::TViewObj(name)
    , unk5C(1)
    , unk5D(0)
{
	unk10 = new TSharedParts(param_1, param_2, param_3, 3);
}

void TEnemyPolluteModel::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!unk5D || unk5C)
		return;

	if (param_1 & 2) {
		if (unk10->unk18->curAnmEndsNext(0, nullptr)) {
			unk5D = false;
			return;
		}

		unk10->unk18->getModel()->setBaseTRMtx(unk14);
		unk10->unk18->getModel()->setBaseScale(unk50);
		unk10->unk18->calcAnm();
	}

	if (param_1 & 0x200)
		gpPollution->stampModel(unk10->unk18->getModel());
}

// TODO: is this TPosition3 or am I trippin
struct Magic : TRotation3f {
	void translate(f32 x, f32 y, f32 z)
	{
		identity33();
		this->mMtx[0][3] = x;
		this->mMtx[1][3] = y;
		this->mMtx[2][3] = z;
	}
};

void TEnemyPolluteModel::generate(JGeometry::TVec3<f32>& param_1,
                                  JGeometry::TVec3<f32>& param_2)
{
	unk44 = param_1;
	unk50 = param_2;

	Magic TStack_58;
	TStack_58.translate(param_1.x, param_1.y, param_1.z);

	(*(Magic*)&unk14).translate(param_1.x, param_1.y, param_1.z);
	unk10->unk18->getModel()->setBaseTRMtx(TStack_58);
	unk5D = true;
	unk5C = false;
	setAnm();
}
